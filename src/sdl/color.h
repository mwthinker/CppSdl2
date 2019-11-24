#ifndef CPPSDL2_SDL_COLOR_H
#define CPPSDL2_SDL_COLOR_H

#include <glm/vec4.hpp>
#include <imgui.h>

#include <SDL_pixels.h>

namespace sdl {

	class Color {
	public:
		constexpr Color() noexcept = default;

		constexpr Color(glm::vec4 vec) noexcept : value_{vec} {
		}

		constexpr Color(const Color& other) noexcept = default;
		constexpr Color& operator=(const Color& other) noexcept = default;

		constexpr Color(Color&& other) noexcept = default;
		constexpr Color& operator=(Color&& other) noexcept = default;

		Color(ImColor vec) noexcept
			: value_{vec.Value.x, vec.Value.y, vec.Value.z, vec.Value.w} {
		}

		constexpr Color(float red, float green, float blue, float alpha = 1.f) noexcept
			: value_{red, green, blue, alpha} {
		}

		constexpr operator glm::vec4() const noexcept {
			return value_;
		}

		operator ImColor() const noexcept {
			return {value_.r, value_.g, value_.b, value_.a};
		}

		operator SDL_Color() const noexcept {
			return {static_cast<uint8_t>(value_.r * 255), static_cast<uint8_t>(value_.g * 255),
				static_cast<uint8_t>(value_.b * 255), static_cast<uint8_t>(255 * value_.a)};
		}

		ImU32 toImU32() const noexcept {
			return ImGui::ColorConvertFloat4ToU32({value_.r, value_.g, value_.b, value_.a});
		}

		constexpr float r() const noexcept {
			return value_.r;
		}
		
		constexpr float g() const noexcept {
			return value_.g;
		}

		constexpr float b() const noexcept {
			return value_.b;
		}

		constexpr float a() const noexcept {
			return value_.a;
		}

		bool operator==(const Color& other) const noexcept {
			return toImU32() == other.toImU32();
		}

		bool operator!=(const Color& other) const noexcept {
			return toImU32() != other.toImU32();
		}

	private:
		glm::vec4 value_{};
	};

	constexpr Color WHITE{1, 1, 1};
	constexpr Color RED{1, 0, 0};
	constexpr Color GREEN{1, 0, 0};
	constexpr Color BLUE{0, 0, 1};

} // Namespace sdl.

#endif // CPPSDL2_SDL_COLOR_H
