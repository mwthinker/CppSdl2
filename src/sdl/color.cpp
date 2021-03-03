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

}
