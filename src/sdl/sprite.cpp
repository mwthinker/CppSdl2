#include "sprite.h"
#include "opengl.h"

#include <string>
#include <functional>

namespace sdl {

	Sprite::Sprite(const std::string& image, const std::function<void()>& filter) :
		texture_(image, filter),
		dx_(static_cast<float>(texture_.getWidth())),
		dy_(static_cast<float>(texture_.getHeight())) {
	}

	Sprite::Sprite(const Texture& texture)
		: Sprite(texture, 0, 0, static_cast<float>(texture.getWidth()), static_cast<float>(texture.getHeight())) {
	}

	Sprite::Sprite(const Texture& texture, float x, float y, float dx, float dy) :
		texture_(texture),
		x_(x),
		y_(y),
		dx_(dx),
		dy_(dy) {
	}

	Sprite::Sprite(Sprite&& sprite) noexcept :
		texture_(std::move(sprite.texture_)),
		x_(sprite.x_), y_(sprite.y_), dx_(sprite.dx_), dy_(sprite.dy_) {

		sprite.x_ = 0;
		sprite.y_ = 0;
		sprite.dx_ = 0;
		sprite.dy_ = 0;
	}

	Sprite& Sprite::operator=(Sprite&& sprite) noexcept {
		texture_ = std::move(sprite.texture_);
		x_ = sprite.x_;
		y_ = sprite.y_;
		dx_ = sprite.dx_;
		dy_ = sprite.dy_;

		sprite.x_ = 0;
		sprite.y_ = 0;
		sprite.dx_ = 0;
		sprite.dy_ = 0;
		return *this;
	}

	void Sprite::setTexture(const Texture& texture) {
		texture_ = texture;
		x_ = 0;
		y_ = 0;
		dx_ = (float) texture.getWidth();
		dy_ = (float) texture.getHeight();
	}

	void Sprite::setTexture(const Texture& texture, float x, float y, float dx, float dy) {
		texture_ = texture;
		x_ = x;
		y_ = y;
		dx_ = dx;
		dy_ = dy;
	}

} // Namespace sdl.
