#pragma once

#include "JuceHeader.h"

namespace util {
	// web-formatted color utiltity: provide a hex code and a numeric alpha value together
	inline juce::Colour hexA(uint32_t h, float a) {
		return juce::Colour(0xff000000 | h).withAlpha(a);
	};
}

namespace tw {

	// these are some colors from the Tailwind color system:
	// https://tailwindcss.com/docs/customizing-colors
	enum TW_COLORS {
		ZINC_950 = 0x09090b,
		ZINC_900 = 0x18181b,
		ZINC_800 = 0x27272a,
		ZINC_700 = 0x3f3f46,
		ZINC_600 = 0x52525b,
		ZINC_500 = 0x71717a,
		ZINC_400 = 0xa1a1aa,
		ZINC_300 = 0xd4d4d8,
		ZINC_200 = 0xe4e4e7,
		ZINC_100 = 0xf4f4f5,
		ZINC_050 = 0xfafafa,

		RED_700 = 0xb91c1c,
		RED_600 = 0xdc2626,
		AMBER_600 = 0xd97706,
		AMBER_400 = 0xfbbf24,
		YELLOW_600 = 0xca8a04,
		GREEN_800 = 0x166534,
		GREEN_600 = 0x16a34a,
		EMERALD_600 = 0x059669,
		EMERALD_700 = 0x047857,
		SKY_600 = 0x0284c7,
		SKY_500 = 0x0ea5e9,
		SKY_400 = 0x38bdf8,
		VIOLET_600 = 0x7c3aed
	};

	inline juce::Colour c(TW_COLORS h, float a) {
		return juce::Colour(0xff000000 | h).withAlpha(a);
	}
	inline juce::Colour c(TW_COLORS h) {
		return juce::Colour(0xff000000 | h);
	}
}
