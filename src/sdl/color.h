#ifndef CPPSDL2_SDL_COLOR_H
#define CPPSDL2_SDL_COLOR_H

#include <glm/vec4.hpp>
#include <imgui.h>
#include <SDL_pixels.h>

#include <string_view>
#include <string>
#include <vector>
#include <algorithm>

namespace sdl {

	namespace color {

		constexpr int hexToInt(char hex) noexcept;

		constexpr char intToHex(int nbr) noexcept;

		constexpr bool isValidHexChar(char hex) noexcept;

		constexpr bool isValidHexColor(std::string_view hex) noexcept;

	}

	class Color {
	public:
		constexpr Color() noexcept = default;

		static constexpr Color createU32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) noexcept;

		explicit constexpr Color(float red, float green, float blue, float alpha = 1.f) noexcept;

		explicit constexpr Color(std::string_view hex) noexcept;

		constexpr uint8_t redByte() const noexcept {
			return static_cast<uint8_t>((value_ >> IM_COL32_R_SHIFT) & 0xff);
		}

		constexpr uint8_t greenByte() const noexcept {
			return static_cast<uint8_t>((value_ >> IM_COL32_G_SHIFT) & 0xff);
		}

		constexpr uint8_t blueByte() const noexcept {
			return static_cast<uint8_t>((value_ >> IM_COL32_B_SHIFT) & 0xff);
		}

		constexpr uint8_t alphaByte() const noexcept {
			return static_cast<uint8_t>((value_ >> IM_COL32_A_SHIFT) & 0xff);
		}
		
		constexpr float red() const noexcept {
			return static_cast<float>(redByte()) / 255.f;
		}

		constexpr float green() const noexcept {
			return static_cast<float>(greenByte()) / 255.f;
		}

		constexpr float blue() const noexcept {
			return static_cast<float>(blueByte()) / 255.f;
		}

		constexpr float alpha() const noexcept {
			return static_cast<float>(alphaByte()) / 255.f;
		}

		operator ImVec4() const noexcept
		{
			return {red(), green(), blue(), alpha()};
		}

		explicit operator ImColor() const noexcept {
			return value_;
		}

		constexpr operator SDL_Color() const noexcept {
			return {redByte(), greenByte(), blueByte(), alphaByte()};
		}

		friend constexpr bool operator==(Color left, Color right) noexcept;

		friend constexpr bool operator!=(Color left, Color right) noexcept;

		friend constexpr Color operator*(Color left, Color right) noexcept;

		friend constexpr Color operator+(Color left, Color right) noexcept;

		friend constexpr Color operator-(Color left, Color right) noexcept;

		friend constexpr Color& operator*=(Color& left, Color right) noexcept;

		friend constexpr Color& operator+=(Color& left, Color right) noexcept;

		friend constexpr Color& operator-=(Color& left, Color right) noexcept;

		constexpr ImU32 toImU32() const noexcept {
			return value_;
		}

		std::string toHexString() const;

	private:
		explicit constexpr Color(ImU32 value) noexcept
			: value_{value} {
		}

		static constexpr uint8_t hexToByte(char first, char second) noexcept {
			return color::hexToInt(first) << 4 | color::hexToInt(second);
		}

		static constexpr ImU32 toByte(float value) noexcept {
			return std::clamp<ImU32>(static_cast<ImU32>(value * 255.f + 0.5f), 0, 255);
		}

		ImU32 value_ = 0;
	};
	
	inline constexpr Color lerp(Color a, Color b, Color t) noexcept {
		return a + t * (b - a);
	}

	inline constexpr bool operator==(Color left, Color right) noexcept {
		return left.value_ == right.value_;
	}

	inline constexpr bool operator!=(Color left, Color right) noexcept {
		return left.value_ != right.value_;
	}

	inline constexpr Color operator*(Color left, Color right) noexcept {
		return Color{left.red() * right.redByte(), left.green() * right.greenByte(), left.blue() * right.blueByte(), left.alpha() * right.alphaByte()};
	}

	inline constexpr Color operator+(Color left, Color right) noexcept {
		return Color{left.red() + right.red(), left.green() + right.green(), left.blue() + right.blue(), left.alpha() + right.alpha()};
	}

	inline constexpr Color operator-(Color left, Color right) noexcept {
		return Color{left.red() - right.red(), left.green() - right.green(), left.blue() - right.blue(), left.alpha() - right.alpha()};
	}

	inline constexpr Color& operator*=(Color& left, Color right) noexcept {
		return left = left * right;
	}

	inline constexpr Color& operator+=(Color& left, Color right) noexcept {
		return left = left + right;;
	}

	inline constexpr Color& operator-=(Color& left, Color right) noexcept {
		return left = left - right;
	}

	inline constexpr Color::Color(float red, float green, float blue, float alpha) noexcept
		: value_{(toByte(red) << IM_COL32_R_SHIFT)
		| (toByte(green) << IM_COL32_G_SHIFT)
		| (toByte(blue) << IM_COL32_B_SHIFT)
		| (toByte(alpha) << IM_COL32_A_SHIFT)} {

		assert(red >= 0 || green >= 0 || blue >= 0 || alpha >= 0);
	}

	inline constexpr Color::Color(std::string_view hex) noexcept {
		if (hex.size() == 4) {
			value_ = createU32(hexToByte(hex[1], hex[1])
				, hexToByte(hex[2], hex[2])
				, hexToByte(hex[3], hex[3])).value_;
		} else if (hex.size() == 7) {
			value_ = createU32(hexToByte(hex[1], hex[2])
				, hexToByte(hex[3], hex[4])
				, hexToByte(hex[5], hex[6])).value_;
		} else if (hex.size() == 9) {
			value_ = createU32(hexToByte(hex[1], hex[2])
				, hexToByte(hex[3], hex[4])
				, hexToByte(hex[5], hex[6])
				, hexToByte(hex[7], hex[8])).value_;
		}
	}

	inline constexpr Color Color::createU32(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) noexcept {
		return Color{(static_cast<ImU32>(red) << IM_COL32_R_SHIFT) | (static_cast<ImU32>(green) << IM_COL32_G_SHIFT) | (static_cast<ImU32>(blue) << IM_COL32_B_SHIFT) | (static_cast<ImU32>(alpha) << IM_COL32_A_SHIFT)};
	}

	namespace color {

		inline constexpr int hexToInt(char hex) noexcept {
			switch (hex) {
				case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
					return static_cast<int>(hex - '0');
				case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':
					return static_cast<int>(hex - 'a') + 10;
				case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':
					return static_cast<int>(hex - 'A') + 10;
			}
			return 0;
		}

		inline constexpr char intToHex(int nbr) noexcept {
			switch (nbr) {
				case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
					return '0' + nbr;
				case 10: case 11:case 12:case 13:case 14:case 15:
					return 'a' + nbr - 10;
			}
			return '0';
		}

		inline constexpr bool isValidHexChar(char hex) noexcept {
			switch (hex) {
				case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
				case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':
				case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':
					return true;
			}
			return false;
		}

		inline constexpr bool isValidHexColor(std::string_view hex) noexcept {
			if (hex.empty() || hex[0] != '#') {
				return false;
			}

			if (hex.size() == 4 || hex.size() == 7 || hex.size() == 9) {
				for (std::size_t i = 1; i < hex.size(); ++i) {
					if (!isValidHexChar(hex[i])) {
						return false;
					}
				}
				return true;
			}
			return false;
		}

		constexpr Color Black{0, 0, 0};
		constexpr Color White{1, 1, 1};
		constexpr Color Red{1, 0, 0};
		constexpr Color Green{0, 1, 0};
		constexpr Color Blue{0, 0, 1};
		constexpr Color Transparent{0, 0, 0, 0};

	}

	namespace color::html {

		// Pink colors
		constexpr sdl::Color MediumVioletRed{"#C71585"};
		constexpr sdl::Color DeepPink{"#FF1493"};
		constexpr sdl::Color PaleVioletRed{"#DB7093"};
		constexpr sdl::Color HotPink{"#FF69B4"};
		constexpr sdl::Color LightPink{"#FFB6C1"};
		constexpr sdl::Color Pink{"#FFC0CB"};

		// Red colors
		constexpr sdl::Color DarkRed{"#8B0000"};
		constexpr sdl::Color Red{"#FF0000"};
		constexpr sdl::Color Firebrick{"#B22222"};
		constexpr sdl::Color Crimson{"#DC143C"};
		constexpr sdl::Color IndianRed{"#CD5C5C"};
		constexpr sdl::Color LightCoral{"#F08080"};
		constexpr sdl::Color Salmon{"#FA8072"};
		constexpr sdl::Color DarkSalmon{"#E9967A"};
		constexpr sdl::Color LightSalmon{"#FFA07A"};

		// Orange colors
		constexpr sdl::Color OrangeRed{"#FF4500"};
		constexpr sdl::Color Tomato{"#FF6347"};
		constexpr sdl::Color DarkOrange{"#FF8C00"};
		constexpr sdl::Color Coral{"#FF7F50"};
		constexpr sdl::Color Orange{"#FFA500"};

		// Yellow colors
		constexpr sdl::Color DarkKhaki{"#BDB76B"};
		constexpr sdl::Color Gold{"#FFD700"};
		constexpr sdl::Color Khaki{"#F0E68C"};
		constexpr sdl::Color PeachPuff{"#FFDAB9"};
		constexpr sdl::Color Yellow{"#FFFF00"};
		constexpr sdl::Color PaleGoldenrod{"#EEE8AA"};
		constexpr sdl::Color Moccasin{"#FFE4B5"};
		constexpr sdl::Color PapayaWhip{"#FFEFD5"};
		constexpr sdl::Color LightGoldenrodYellow{"#FAFAD2"};
		constexpr sdl::Color LemonChiffon{"#FFFACD"};
		constexpr sdl::Color LightYellow{"#FFFFE0"};

		// Brown colors
		constexpr sdl::Color Maroon{"#800000"};
		constexpr sdl::Color Brown{"#A52A2A"};
		constexpr sdl::Color SaddleBrown{"#8B4513"};
		constexpr sdl::Color Sienna{"#A0522D"};
		constexpr sdl::Color Chocolate{"#D2691E"};
		constexpr sdl::Color DarkGoldenrod{"#B8860B"};
		constexpr sdl::Color Peru{"#CD853F"};
		constexpr sdl::Color RosyBrown{"#BC8F8F"};
		constexpr sdl::Color Goldenrod{"#DAA520"};
		constexpr sdl::Color SandyBrown{"#F4A460"};
		constexpr sdl::Color Tan{"#D2B48C"};
		constexpr sdl::Color Burlywood{"#DEB887"};
		constexpr sdl::Color Wheat{"#F5DEB3"};
		constexpr sdl::Color NavajoWhite{"#FFDEAD"};
		constexpr sdl::Color Bisque{"#FFE4C4"};
		constexpr sdl::Color BlanchedAlmond{"#FFEBCD"};
		constexpr sdl::Color Cornsilk{"#FFF8DC"};

		// Green colors
		constexpr sdl::Color DarkGreen{"#006400"};
		constexpr sdl::Color Green{"#008000"};
		constexpr sdl::Color DarkOliveGreen{"#556B2F"};
		constexpr sdl::Color ForestGreen{"#228B22"};
		constexpr sdl::Color SeaGreen{"#2E8B57"};
		constexpr sdl::Color Olive{"#808000"};
		constexpr sdl::Color OliveDrab{"#6B8E23"};
		constexpr sdl::Color MediumSeaGreen{"#3CB371"};
		constexpr sdl::Color LimeGreen{"#32CD32"};
		constexpr sdl::Color Lime{"#00FF00"};
		constexpr sdl::Color SpringGreen{"#00FF7F"};
		constexpr sdl::Color MediumSpringGreen{"#00FA9A"};
		constexpr sdl::Color DarkSeaGreen{"#8FBC8F"};
		constexpr sdl::Color MediumAquamarine{"#66CDAA"};
		constexpr sdl::Color YellowGreen{"#9ACD32"};
		constexpr sdl::Color LawnGreen{"#7CFC00"};
		constexpr sdl::Color Chartreuse{"#7FFF00"};
		constexpr sdl::Color LightGreen{"#90EE90"};
		constexpr sdl::Color GreenYellow{"#ADFF2F"};
		constexpr sdl::Color PaleGreen{"#98FB98"};
		
		// Cyan colors
		constexpr sdl::Color Teal{"#008080"};
		constexpr sdl::Color DarkCyan{"#008B8B"};
		constexpr sdl::Color LightSeaGreen{"#20B2AA"};
		constexpr sdl::Color CadetBlue{"#5F9EA0"};
		constexpr sdl::Color DarkTurquoise{"#00CED1"};
		constexpr sdl::Color MediumTurquoise{"#48D1CC"};
		constexpr sdl::Color Turquoise{"#40E0D0"};
		constexpr sdl::Color Aqua{"#00FFFF"};
		constexpr sdl::Color Cyan{"#00FFFF"};
		constexpr sdl::Color Aquamarine{"#7FFFD4"};
		constexpr sdl::Color PaleTurquoise{"#AFEEEE"};
		constexpr sdl::Color LightCyan{"#E0FFFF"};
		
		// Blue colors
		constexpr sdl::Color Navy{"#000080"};
		constexpr sdl::Color DarkBlue{"#00008B"};
		constexpr sdl::Color MediumBlue{"#0000CD"};
		constexpr sdl::Color Blue{"#0000FF"};
		constexpr sdl::Color MidnightBlue{"#191970"};
		constexpr sdl::Color RoyalBlue{"#4169E1"};
		constexpr sdl::Color SteelBlue{"#4682B4"};
		constexpr sdl::Color DodgerBlue{"#1E90FF"};
		constexpr sdl::Color DeepSkyBlue{"#00BFFF"};
		constexpr sdl::Color CornflowerBlue{"#6495ED"};
		constexpr sdl::Color SkyBlue{"#87CEEB"};
		constexpr sdl::Color LightSkyBlue{"#87CEFA"};
		constexpr sdl::Color LightSteelBlue{"#B0C4DE"};
		constexpr sdl::Color LightBlue{"#ADD8E6"};
		constexpr sdl::Color PowderBlue{"#B0E0E6"};

		// Purple, violet, and magenta colors
		constexpr sdl::Color Indigo{"#4B0082"};
		constexpr sdl::Color Purple{"#800080"};
		constexpr sdl::Color DarkMagenta{"#8B008B"};
		constexpr sdl::Color DarkViolet{"#9400D3"};
		constexpr sdl::Color DarkSlateBlue{"#483D8B"};
		constexpr sdl::Color BlueViolet{"#8A2BE2"};
		constexpr sdl::Color DarkOrchid{"#9932CC"};
		constexpr sdl::Color Fuchsia{"#FF00FF"};
		constexpr sdl::Color Magenta{"#FF00FF"};
		constexpr sdl::Color SlateBlue{"#6A5ACD"};
		constexpr sdl::Color MediumSlateBlue{"#7B68EE"};
		constexpr sdl::Color MediumOrchid{"#BA55D3"};
		constexpr sdl::Color MediumPurple{"#9370DB"};
		constexpr sdl::Color Orchid{"#DA70D6"};
		constexpr sdl::Color Violet{"#EE82EE"};
		constexpr sdl::Color Plum{"#DDA0DD"};
		constexpr sdl::Color Thistle{"#D8BFD8"};
		constexpr sdl::Color Lavender{"#E6E6FA"};

		// White colors
		constexpr sdl::Color MistyRose{"#FFE4E1"};
		constexpr sdl::Color AntiqueWhite{"#FAEBD7"};
		constexpr sdl::Color Linen{"#FAF0E6"};
		constexpr sdl::Color Beige{"#F5F5DC"};
		constexpr sdl::Color WhiteSmoke{"#F5F5F5"};
		constexpr sdl::Color LavenderBlush{"#FFF0F5"};
		constexpr sdl::Color OldLace{"#FDF5E6"};
		constexpr sdl::Color AliceBlue{"#F0F8FF"};
		constexpr sdl::Color Seashell{"#FFF5EE"};
		constexpr sdl::Color GhostWhite{"#F8F8FF"};
		constexpr sdl::Color Honeydew{"#F0FFF0"};
		constexpr sdl::Color FloralWhite{"#FFFAF0"};
		constexpr sdl::Color Azure{"#F0FFFF"};
		constexpr sdl::Color MintCream{"#F5FFFA"};
		constexpr sdl::Color Snow{"#FFFAFA"};
		constexpr sdl::Color Ivory{"#FFFFF0"};
		constexpr sdl::Color White{"#FFFFFF"};

		// Gray and black colors
		constexpr sdl::Color Black{"#000000"};
		constexpr sdl::Color DarkSlateGray{"#2F4F4F"};
		constexpr sdl::Color DimGray{"#696969"};
		constexpr sdl::Color SlateGray{"#708090"};
		constexpr sdl::Color Gray{"#808080"};
		constexpr sdl::Color LightSlateGray{"#778899"};
		constexpr sdl::Color DarkGray{"#A9A9A9"};
		constexpr sdl::Color Silver{"#C0C0C0"};
		constexpr sdl::Color LightGray{"#D3D3D3"};
		constexpr sdl::Color Gainsboro{"#DCDCDC"};

		struct Pair {
			std::string name;
			sdl::Color color;
		};

		std::vector<Pair> getHtmlColors();
	}

}

#endif
