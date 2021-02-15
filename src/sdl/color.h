#ifndef CPPSDL2_SDL_COLOR_H
#define CPPSDL2_SDL_COLOR_H

#include <glm/vec4.hpp>
#include <imgui.h>

#include <SDL_pixels.h>

namespace sdl {

	class Color {
	public:
		constexpr Color() noexcept = default;

		explicit constexpr Color(uint32_t value) noexcept
			: value_{value} {
		}

		Color(float red, float green, float blue, float alpha = 1.f) noexcept
			: value_{ImGui::ColorConvertFloat4ToU32({red, green, blue, alpha})} {
			
			assert(red >= 0 || green >= 0 || blue >= 0 || alpha >= 0);
		}

		constexpr float red() const noexcept {
			return static_cast<float>(value_ & 0x00'00'00'ff) / 255.f;
		}

		constexpr float green() const noexcept {
			return static_cast<float>((value_ & 0x00'00'ff'00) >> 8) / 255.f;
		}

		constexpr float blue() const noexcept {
			return static_cast<float>((value_ & 0x00'ff'00'00) >> 16) / 255.f;
		}

		constexpr float alpha() const noexcept {
			return static_cast<float>((value_ & 0xff'00'00'00) >> 24) / 255.f;;
		}

		operator ImVec4() const noexcept
		{
			return {red(), green(), blue(), alpha()};
		}

		explicit operator ImColor() const noexcept {
			return value_;
		}

		constexpr operator SDL_Color() const noexcept {
			return {static_cast<Uint8>(value_ & 0x00'00'00'ff),
				static_cast<Uint8>((value_ & 0x00'00'ff'00) >> 8),
				static_cast<Uint8>((value_ & 0x00'ff'00'00) >> 16),
				static_cast<Uint8>((value_ & 0xff'00'00'00) >> 24)};
		}

		constexpr bool operator==(Color other) const noexcept {
			return value_ == other.value_;
		}

		constexpr bool operator!=(Color other) const noexcept {
			return value_ != other.value_;
		}

		constexpr ImU32 toImU32() const noexcept {
			return value_;
		}
		
	private:
		uint32_t value_{};
	};

	constexpr Color Black{0x00'00'00'ff};
	constexpr Color White{0xff'ff'ff'ff};
	constexpr Color Red{0xff'00'00'ff};
	constexpr Color Green{0x00'ff'00'ff};
	constexpr Color Blue{0x00'00'ff'ff};

}

#endif
