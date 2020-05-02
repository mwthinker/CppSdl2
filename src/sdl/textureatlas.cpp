#include "textureatlas.h"

#include <spdlog/spdlog.h>

namespace sdl {

	std::shared_ptr<TextureAtlas::Node> TextureAtlas::createRoot(std::unique_ptr<Node>& root,
		int width, int height, const Surface& surface, int border) {
		
		// Image doesn't fit?
		if (surface.getWidth() > width || surface.getHeight() > height) {
			// Image to large!
			return nullptr;
		}

		root = std::make_unique<Node>(Rect{0, 0, width, height});
		return root->insert(surface, border);
	}

	TextureAtlas::Node::Node(Rect rect)
		: rect_{rect} {
	}

	std::shared_ptr<TextureAtlas::Node> TextureAtlas::Node::insert(const Surface& surface, int border) {
		// Is not a leaf!
		if (left_ != nullptr && right_ != nullptr) {
			if (auto node = left_->insert(surface, border); node != nullptr) {
				// Image inserted.
				return node;
			}
			// Image did not fit, try the other node.
			return right_->insert(surface, border);
		} else {
			if (image) {
				// Node is already filled!
				return nullptr;
			}

			if (surface.getWidth() + 2 * border > rect_.w || surface.getHeight() + 2 * border > rect_.h) {
				// Image to large!
				return nullptr;
			}

			// Fits perfectly?
			if (surface.getWidth() + 2 * border == rect_.w && surface.getHeight() + 2 * border == rect_.h) {
				image = true;
				return shared_from_this();
			}

			// Split the node in half.
			if (rect_.w - surface.getWidth() < rect_.h - surface.getHeight()) { // Split vertical.
				left_ = std::make_shared<Node>(Rect{rect_.x, rect_.y,
					rect_.w, surface.getHeight() + 2 * border}); // Up.

				right_ = std::make_shared<Node>(Rect{rect_.x, rect_.y + surface.getHeight() + 2 * border,
					rect_.w, rect_.h - surface.getHeight() - 2 * border}); // Down.
			
			} else { // Split horizontal.
				left_ = std::make_shared<Node>(Rect{rect_.x, rect_.y,
					surface.getWidth() + 2 * border, rect_.h}); // Left.
				
				right_ = std::make_shared<Node>(Rect{rect_.x + surface.getWidth() + 2 * border, rect_.y,
					rect_.w - surface.getWidth() - 2 * border, rect_.h}); // Right.
			}

			// Insert the image in the left node.
			return left_->insert(surface, border);
		}
	}

	TextureAtlas::TextureAtlas(int width, int height, std::function<void()>&& filter)
		: sprite_{Surface{width, height}, std::move(filter)} {

	}

	const Sprite& TextureAtlas::add(const std::string& filename, int border, const std::string& uniqueKey) {
		std::string key = filename + uniqueKey;
		auto it = images_.find(key);
		if (it == images_.end()) {
			Surface surface{filename};
			if (surface.isLoaded()) {
				return add(std::move(surface), border, key);
			} else {
				spdlog::warn("[sdl::TextureAtlas] Image {} failed to load: {}", filename, IMG_GetError());
				return images_[""];
			}
		}
		return it->second;
	}

	const Sprite& TextureAtlas::add(const Surface& surface, int border, const std::string& key) {
		auto it = images_.find(key);
		if (key.empty() || it == images_.end()) {
			if (surface.isLoaded()) {
				std::shared_ptr<Node> node;
				if (root_) {
					node = root_->insert(surface, border);
				} else {
					node = createRoot(root_,
						sprite_.getTextureWidth(), sprite_.getTextureHeight(),
						surface, border);
				}
				if (node) {
					// Only when atlas is not full.
					Rect rect = node->getRect();
					rect.w -= 2 * border;
					rect.h -= 2 * border;
					rect.x += border;
					rect.y += border;
					sprite_.blit(surface, rect);
					return images_[key] = Sprite{sprite_, rect};
				} else {
					spdlog::warn("[sdl::TextureAtlas] Not enough image space to insert image");
					return images_[""];
				}
			} else {
				return images_[""];
			}
		}
		return it->second; // Return already loaded sprite.
	}

	const Sprite& TextureAtlas::get(const std::string& key) const {
		if (auto it = images_.find(key); it != images_.end()) {
			return it->second;
		}
		return images_[""];
	}

	const Sprite& TextureAtlas::get() const {
		return sprite_;
	}

	TextureView TextureAtlas::getTextureView(const std::string& key) const {
		if (auto it = images_.find(key); it != images_.end()) {
			return it->second.getTextureView();
		}
		return {};
	}

	TextureView TextureAtlas::getTextureView() const {
		return {};
	}

} // Namespace sdl.
