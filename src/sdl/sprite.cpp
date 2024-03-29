#include "sprite.h"
#include "surface.h"
#include "opengl.h"

#include <spdlog/spdlog.h>

#include <string>
#include <functional>

namespace sdl {

	Sprite::Sprite(const std::string& image, std::function<void()>&& filter)
		: image_{std::make_shared<ImageVariant>()} {
		
		Surface surface{image};
		textureWidth_ = surface.getWidth();
		textureHeight_ = surface.getHeight();
		rect_.w = textureWidth_;
		rect_.h = textureHeight_;
		*image_ = SurfaceData{std::move(surface), filter};
	}

	Sprite::Sprite(const std::string& text, const Font& font, std::function<void()>&& filter) {
		if (text.empty()) {
			return;
		}

		if (!font.isLoaded()) {
			spdlog::warn("[sdl::Sprite] font not loaded");
		}

		Surface surface{text, font, color::White};
		textureWidth_ = surface.getWidth();
		textureHeight_ = surface.getHeight();
		rect_.w = textureWidth_;
		rect_.h = textureHeight_;
		image_ = std::make_shared<ImageVariant>();
		*image_ = SurfaceData{std::move(surface), filter};
	}

	Sprite::Sprite(Surface&& surface, std::function<void()>&& filter)
		: image_{std::make_shared<ImageVariant>()}
		, rect_{0, 0, surface.getWidth(), surface.getHeight()}
		, textureWidth_{surface.getWidth()}
		, textureHeight_{surface.getHeight()} {

		*image_ = SurfaceData{std::move(surface), filter};
	}

	Sprite::Sprite(Surface&& surface, const Rect& rect, std::function<void()>&& filter)
		: image_{std::make_shared<ImageVariant>()}
		, rect_{rect}
		, textureWidth_{surface.getWidth()}
		, textureHeight_{surface.getHeight()} {

		*image_ = SurfaceData{std::move(surface), filter};
	}

	Sprite::Sprite(Sprite&& other) noexcept
		: image_{std::move(other.image_)}
		, rect_{other.rect_}
		, textureWidth_{other.textureWidth_}
		, textureHeight_{other.textureHeight_} {
	}

	Sprite& Sprite::operator=(Sprite&& other) noexcept {
		image_ = std::move(other.image_);
		rect_ = other.rect_;
		textureWidth_ = other.textureWidth_;
		textureHeight_ = other.textureHeight_;
		return *this;
	}

	Sprite::Sprite(const Sprite& sprite, const Rect& rect)
		: image_{sprite.image_}
		, rect_{rect}
		, textureWidth_{sprite.textureWidth_}
		, textureHeight_{sprite.textureHeight_} {
	}

	void Sprite::bind() {
		if (!image_) {
			return;
		}

		if (std::holds_alternative<SurfaceData>(*image_)) {
			Texture texture{};
			texture.generate();
			texture.texImage(std::get<SurfaceData>(*image_).surface, std::move(std::get<SurfaceData>(*image_).filter));
			texture.bind();
			*image_ = std::move(texture);
		} else {
			std::get<Texture>(*image_).bind();
		}
	}

	TextureView Sprite::getTextureView() const {
		if (!image_) {
			return {};
		}

		try {
			return {std::get<Texture>(*image_), getX() / textureWidth_, getY() / textureHeight_, getWidth() / textureWidth_, getHeight() / textureHeight_};
		} catch (std::bad_variant_access&) {
			return {};
		}
	}

	float Sprite::getX() const noexcept {
		return static_cast<float>(rect_.x);
	}

	float Sprite::getY() const noexcept {
		return static_cast<float>(rect_.y);
	}

	glm::vec2 Sprite::getPosition() const noexcept {
		return {getX(), getY()};
	}

	float Sprite::getWidth() const noexcept {
		return static_cast<float>(rect_.w);
	}

	float Sprite::getHeight() const noexcept {
		return static_cast<float>(rect_.h);
	}

	glm::vec2 Sprite::getSize() const noexcept {
		return {getWidth(), getHeight()};
	}

	int Sprite::getTextureWidth() const noexcept {
		return textureWidth_;
	}

	int Sprite::getTextureHeight() const noexcept {
		return textureHeight_;
	}

	Size Sprite::getTextureSize() const noexcept {
		return {textureWidth_, textureHeight_};
	}

	bool Sprite::isValid() const noexcept {
		if (!image_) {
			return false;
		}

		if (std::holds_alternative<SurfaceData>(*image_)) {
			return std::get<SurfaceData>(*image_).surface.isLoaded();
		} else {
			return std::get<Texture>(*image_).isValid();
		}
	}

	bool Sprite::equalSource(const Sprite& s1, const Sprite& s2) noexcept {
		return s1.image_ == s2.image_;
	}

	void Sprite::blit(const Surface& src, const Rect& dstRect) {
		if (image_) {
			if (std::holds_alternative<SurfaceData>(*image_)) {
				std::get<SurfaceData>(*image_).surface.blitSurface(src, dstRect);
			} else {
				std::get<Texture>(*image_).texSubImage(src, dstRect);
			}
		}
	}

}
