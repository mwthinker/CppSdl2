#include "textureatlas.h"
#include "logger.h"

namespace sdl {

	void TextureAtlas::uploadSdlSurfaceToTexture(SDL_Surface* image, SDL_Rect dstRec, Texture& texture) {
		// In computer memory?
		if (texture.imageData_->texture_ == 0) {
			image = SDL_ConvertSurface(image, texture.imageData_->preLoadSurface_->format, 0);
			SDL_BlitSurface(image, 0, texture.imageData_->preLoadSurface_, &dstRec);
		} else { // In graphic memory.
			helper::flipVertical(image);
			SDL_Surface* surface = helper::createSurface(image->w, image->h);
			SDL_BlitSurface(image, 0, surface, &image->clip_rect);
			texture.bindTexture();
			glTexSubImage2D(GL_TEXTURE_2D, 0,
				dstRec.x, texture.height_ - image->h - dstRec.y, // Sdl uses upp-left, opengl uses down-left.
				image->w, image->h,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				surface->pixels);
			SDL_FreeSurface(surface);
		}
	}

	std::shared_ptr<TextureAtlas::Node> TextureAtlas::Node::createRoot(std::shared_ptr<Node>& root,
		int width, int height, SDL_Surface* image, int border) {
		
		// Image doesn't fit?
		if (image->w > width || image->h > height) {
			// Image to large!
			return nullptr;
		}
		root = std::make_shared<Node>(0, 0, width, height);
		return root->insert(image, border);
	}

	TextureAtlas::Node::Node(int x, int y, int w, int h) : image_(nullptr), rect_{x, y, w, h} {
	}

	std::shared_ptr<TextureAtlas::Node> TextureAtlas::Node::insert(SDL_Surface* image, int border) {
		// Is not a leaf!
		if (left_ != 0 && right_ != 0) {
			auto node = left_->insert(image, border);
			if (node != nullptr) {
				// Image inserted.
				return node;
			}
			// Image didn't fit, try the other node.
			return right_->insert(image, border);
		} else {
			if (image_ != nullptr) {
				// Node is already filled!
				return nullptr;
			}
			
			if (image->w + 2 * border > rect_.w || image->h + 2 * border > rect_.h) {
				// Image to large!
				return nullptr;
			}

			// Fits perfectly?
			if (image->w + 2 * border == rect_.w && image->h + 2 * border == rect_.h) {
				image_ = image;
				return shared_from_this();
			}

			// Split the node in half.
			if (rect_.w - image->w < rect_.h - image->h) { // Split vertical.
				left_ = std::make_shared<Node>(rect_.x, rect_.y,
					rect_.w, image->h + 2 * border); // Up.

				right_ = std::make_shared<Node>(rect_.x, rect_.y + image->h + 2 * border,
					rect_.w, rect_.h - image->h - 2 * border); // Down.
			
			} else { // Split horizontal.
				left_ = std::make_shared<Node>(rect_.x, rect_.y,
					image->w + 2 * border, rect_.h); // Left.
				
				right_ = std::make_shared<Node>(rect_.x + image->w + 2 * border, rect_.y,
					rect_.w - image->w - 2 * border, rect_.h); // Right.
			}

			// Insert the image in the left node.
			return left_->insert(image, border);
		}
	}

	TextureAtlas::TextureAtlas(int width, int height, const std::function<void()>& filter)
		: texture_(width, height, filter), root_(nullptr), width_(width), height_(width) {
	}

	TextureAtlas::TextureAtlas(TextureAtlas&& other) noexcept :
		texture_(std::move(other.texture_)), images_(std::move(other.images_)),
		root_(std::move(other.root_)), width_(other.width_), height_(other.height_) {

		other.width_ = 0;
		other.height_ = 0;
	}

	TextureAtlas& TextureAtlas::operator=(TextureAtlas&& other) noexcept {
		texture_ = std::move(other.texture_);
		images_ = std::move(other.images_);
		root_ = std::move(other.root_);
		width_ = other.width_;
		height_ = other.height_;

		other.width_ = 0;
		other.height_ = 0;
		return *this;
	}

	Sprite TextureAtlas::add(const std::string& filename, int border, const std::string& uniqueKey) {
		std::string key = filename + uniqueKey;
		auto it = images_.find(key);
		if (it == images_.end()) {
			SDL_Surface* image = IMG_Load(filename.c_str());
			if (image != nullptr) {
				Sprite sprite = add(image, border, key);
				SDL_FreeSurface(image);
				return sprite;
			} else {
				logger()->warn("[TextureAtlas] Image {} failed to load: {}", filename, IMG_GetError());
				return Sprite();
			}
		}
		return it->second;
	}

	Sprite TextureAtlas::add(SDL_Surface* image, int border, const std::string& key) {
		auto it = images_.find(key);
		if (key.empty() || it == images_.end()) {
			std::shared_ptr<Node> node = nullptr;
			if (texture_.isValid()) {
				if (root_) {
					node = root_->insert(image, border);
				} else {
					node = Node::createRoot(root_, width_, height_, image, border);
				}
				if (node) {
					// Only when atlas is not full.
					SDL_Rect rect = node->getRect();
					rect.w -= 2 * border;
					rect.h -= 2 * border;
					rect.x += border;
					rect.y += border;
					uploadSdlSurfaceToTexture(image, rect, texture_);
					// If key is empty, then the default sprite is overridden.
					Sprite& sprite = images_[key];
					sprite = Sprite(texture_,
						(float) rect.x, (float) (texture_.getHeight() - rect.y - image->h),
						(float) image->w, (float) image->h);
					return sprite;
				} else {
					logger()->warn("[TextureAtlas] Not enough image space to insert image");
					return Sprite();
				}
			} else {
				return Sprite();
			}
		}
		return it->second; // Return already loaded sprite.
	}

	Sprite TextureAtlas::get(const std::string& key) const {
		auto it = images_.find(key);
		if (it != images_.end()) {
			return it->second;
		}
		return Sprite();
	}

} // Namespace sdl.
