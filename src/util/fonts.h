#pragma once

#include <JuceHeader.h>

// fonts stored here
namespace fonts {
	static juce::Typeface::Ptr Atkinson400() {
		static auto font = juce::Typeface::createSystemTypefaceFor(
			BinaryData::AtkinsonHyperlegibleRegular_ttf, BinaryData::AtkinsonHyperlegibleRegular_ttfSize);
		return font;
	}
	static juce::Typeface::Ptr Atkinson700() {
		static auto font = juce::Typeface::createSystemTypefaceFor(
			BinaryData::AtkinsonHyperlegibleBold_ttf, BinaryData::AtkinsonHyperlegibleBold_ttfSize);
		return font;
	}
}