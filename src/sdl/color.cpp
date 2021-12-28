#include "color.h"

namespace sdl {

	using namespace color;

	std::string Color::toHexString() const {
		return {'#'
			, intToHex((value_ & (0xf0 << IM_COL32_R_SHIFT)) >> IM_COL32_R_SHIFT >> 4), intToHex((value_ & (0x0f << IM_COL32_R_SHIFT)) >> IM_COL32_R_SHIFT)
			, intToHex((value_ & (0xf0 << IM_COL32_G_SHIFT)) >> IM_COL32_G_SHIFT >> 4), intToHex((value_ & (0x0f << IM_COL32_G_SHIFT)) >> IM_COL32_G_SHIFT)
			, intToHex((value_ & (0xf0 << IM_COL32_B_SHIFT)) >> IM_COL32_B_SHIFT >> 4), intToHex((value_ & (0x0f << IM_COL32_B_SHIFT)) >> IM_COL32_B_SHIFT)
			, intToHex((value_ & (0xf0 << IM_COL32_A_SHIFT)) >> IM_COL32_A_SHIFT >> 4), intToHex((value_ & (0x0f << IM_COL32_A_SHIFT)) >> IM_COL32_A_SHIFT)
		};
	}

#define HTML_COLOR(name) {#name, sdl::color::html::name}

	namespace color::html {

		std::vector<Pair> getHtmlColors() {
			std::vector<Pair> htmlColors;
			htmlColors.push_back(HTML_COLOR(MediumVioletRed));
			htmlColors.push_back(HTML_COLOR(DeepPink));
			htmlColors.push_back(HTML_COLOR(PaleVioletRed));
			htmlColors.push_back(HTML_COLOR(HotPink));
			htmlColors.push_back(HTML_COLOR(LightPink));
			htmlColors.push_back(HTML_COLOR(Pink));
			htmlColors.push_back(HTML_COLOR(DarkRed));
			htmlColors.push_back(HTML_COLOR(Red));
			htmlColors.push_back(HTML_COLOR(Firebrick));
			htmlColors.push_back(HTML_COLOR(Crimson));
			htmlColors.push_back(HTML_COLOR(IndianRed));
			htmlColors.push_back(HTML_COLOR(LightCoral));
			htmlColors.push_back(HTML_COLOR(Salmon));
			htmlColors.push_back(HTML_COLOR(DarkSalmon));
			htmlColors.push_back(HTML_COLOR(LightSalmon));
			htmlColors.push_back(HTML_COLOR(OrangeRed));
			htmlColors.push_back(HTML_COLOR(Tomato));
			htmlColors.push_back(HTML_COLOR(DarkOrange));
			htmlColors.push_back(HTML_COLOR(Coral));
			htmlColors.push_back(HTML_COLOR(Orange));
			htmlColors.push_back(HTML_COLOR(DarkKhaki));
			htmlColors.push_back(HTML_COLOR(Gold));
			htmlColors.push_back(HTML_COLOR(Khaki));
			htmlColors.push_back(HTML_COLOR(PeachPuff));
			htmlColors.push_back(HTML_COLOR(Yellow));
			htmlColors.push_back(HTML_COLOR(PaleGoldenrod));
			htmlColors.push_back(HTML_COLOR(Moccasin));
			htmlColors.push_back(HTML_COLOR(PapayaWhip));
			htmlColors.push_back(HTML_COLOR(LightGoldenrodYellow));
			htmlColors.push_back(HTML_COLOR(LemonChiffon));
			htmlColors.push_back(HTML_COLOR(LightYellow));
			htmlColors.push_back(HTML_COLOR(Maroon));
			htmlColors.push_back(HTML_COLOR(Brown));
			htmlColors.push_back(HTML_COLOR(SaddleBrown));
			htmlColors.push_back(HTML_COLOR(Sienna));
			htmlColors.push_back(HTML_COLOR(Chocolate));
			htmlColors.push_back(HTML_COLOR(DarkGoldenrod));
			htmlColors.push_back(HTML_COLOR(Peru));
			htmlColors.push_back(HTML_COLOR(RosyBrown));
			htmlColors.push_back(HTML_COLOR(Goldenrod));
			htmlColors.push_back(HTML_COLOR(SandyBrown));
			htmlColors.push_back(HTML_COLOR(Tan));
			htmlColors.push_back(HTML_COLOR(Burlywood));
			htmlColors.push_back(HTML_COLOR(Wheat));
			htmlColors.push_back(HTML_COLOR(NavajoWhite));
			htmlColors.push_back(HTML_COLOR(Bisque));
			htmlColors.push_back(HTML_COLOR(BlanchedAlmond));
			htmlColors.push_back(HTML_COLOR(Cornsilk));
			htmlColors.push_back(HTML_COLOR(DarkGreen));
			htmlColors.push_back(HTML_COLOR(Green));
			htmlColors.push_back(HTML_COLOR(DarkOliveGreen));
			htmlColors.push_back(HTML_COLOR(ForestGreen));
			htmlColors.push_back(HTML_COLOR(SeaGreen));
			htmlColors.push_back(HTML_COLOR(Olive));
			htmlColors.push_back(HTML_COLOR(OliveDrab));
			htmlColors.push_back(HTML_COLOR(MediumSeaGreen));
			htmlColors.push_back(HTML_COLOR(LimeGreen));
			htmlColors.push_back(HTML_COLOR(Lime));
			htmlColors.push_back(HTML_COLOR(SpringGreen));
			htmlColors.push_back(HTML_COLOR(MediumSpringGreen));
			htmlColors.push_back(HTML_COLOR(DarkSeaGreen));
			htmlColors.push_back(HTML_COLOR(MediumAquamarine));
			htmlColors.push_back(HTML_COLOR(YellowGreen));
			htmlColors.push_back(HTML_COLOR(LawnGreen));
			htmlColors.push_back(HTML_COLOR(Chartreuse));
			htmlColors.push_back(HTML_COLOR(LightGreen));
			htmlColors.push_back(HTML_COLOR(GreenYellow));
			htmlColors.push_back(HTML_COLOR(PaleGreen));
			htmlColors.push_back(HTML_COLOR(Teal));
			htmlColors.push_back(HTML_COLOR(DarkCyan));
			htmlColors.push_back(HTML_COLOR(LightSeaGreen));
			htmlColors.push_back(HTML_COLOR(CadetBlue));
			htmlColors.push_back(HTML_COLOR(DarkTurquoise));
			htmlColors.push_back(HTML_COLOR(MediumTurquoise));
			htmlColors.push_back(HTML_COLOR(Turquoise));
			htmlColors.push_back(HTML_COLOR(Aqua));
			htmlColors.push_back(HTML_COLOR(Cyan));
			htmlColors.push_back(HTML_COLOR(Aquamarine));
			htmlColors.push_back(HTML_COLOR(PaleTurquoise));
			htmlColors.push_back(HTML_COLOR(LightCyan));
			htmlColors.push_back(HTML_COLOR(Navy));
			htmlColors.push_back(HTML_COLOR(DarkBlue));
			htmlColors.push_back(HTML_COLOR(MediumBlue));
			htmlColors.push_back(HTML_COLOR(Blue));
			htmlColors.push_back(HTML_COLOR(MidnightBlue));
			htmlColors.push_back(HTML_COLOR(RoyalBlue));
			htmlColors.push_back(HTML_COLOR(SteelBlue));
			htmlColors.push_back(HTML_COLOR(DodgerBlue));
			htmlColors.push_back(HTML_COLOR(DeepSkyBlue));
			htmlColors.push_back(HTML_COLOR(CornflowerBlue));
			htmlColors.push_back(HTML_COLOR(SkyBlue));
			htmlColors.push_back(HTML_COLOR(LightSkyBlue));
			htmlColors.push_back(HTML_COLOR(LightSteelBlue));
			htmlColors.push_back(HTML_COLOR(LightBlue));
			htmlColors.push_back(HTML_COLOR(PowderBlue));
			htmlColors.push_back(HTML_COLOR(Indigo));
			htmlColors.push_back(HTML_COLOR(Purple));
			htmlColors.push_back(HTML_COLOR(DarkMagenta));
			htmlColors.push_back(HTML_COLOR(DarkViolet));
			htmlColors.push_back(HTML_COLOR(DarkSlateBlue));
			htmlColors.push_back(HTML_COLOR(BlueViolet));
			htmlColors.push_back(HTML_COLOR(DarkOrchid));
			htmlColors.push_back(HTML_COLOR(Fuchsia));
			htmlColors.push_back(HTML_COLOR(Magenta));
			htmlColors.push_back(HTML_COLOR(SlateBlue));
			htmlColors.push_back(HTML_COLOR(MediumSlateBlue));
			htmlColors.push_back(HTML_COLOR(MediumOrchid));
			htmlColors.push_back(HTML_COLOR(MediumPurple));
			htmlColors.push_back(HTML_COLOR(Orchid));
			htmlColors.push_back(HTML_COLOR(Violet));
			htmlColors.push_back(HTML_COLOR(Plum));
			htmlColors.push_back(HTML_COLOR(Thistle));
			htmlColors.push_back(HTML_COLOR(Lavender));
			htmlColors.push_back(HTML_COLOR(MistyRose));
			htmlColors.push_back(HTML_COLOR(AntiqueWhite));
			htmlColors.push_back(HTML_COLOR(Linen));
			htmlColors.push_back(HTML_COLOR(Beige));
			htmlColors.push_back(HTML_COLOR(WhiteSmoke));
			htmlColors.push_back(HTML_COLOR(LavenderBlush));
			htmlColors.push_back(HTML_COLOR(OldLace));
			htmlColors.push_back(HTML_COLOR(AliceBlue));
			htmlColors.push_back(HTML_COLOR(Seashell));
			htmlColors.push_back(HTML_COLOR(GhostWhite));
			htmlColors.push_back(HTML_COLOR(Honeydew));
			htmlColors.push_back(HTML_COLOR(FloralWhite));
			htmlColors.push_back(HTML_COLOR(Azure));
			htmlColors.push_back(HTML_COLOR(MintCream));
			htmlColors.push_back(HTML_COLOR(Snow));
			htmlColors.push_back(HTML_COLOR(Ivory));
			htmlColors.push_back(HTML_COLOR(White));
			htmlColors.push_back(HTML_COLOR(Black));
			htmlColors.push_back(HTML_COLOR(DarkSlateGray));
			htmlColors.push_back(HTML_COLOR(DimGray));
			htmlColors.push_back(HTML_COLOR(SlateGray));
			htmlColors.push_back(HTML_COLOR(Gray));
			htmlColors.push_back(HTML_COLOR(LightSlateGray));
			htmlColors.push_back(HTML_COLOR(DarkGray));
			htmlColors.push_back(HTML_COLOR(Silver));
			htmlColors.push_back(HTML_COLOR(LightGray));
			htmlColors.push_back(HTML_COLOR(Gainsboro));
			return htmlColors;
		}

	}

}
