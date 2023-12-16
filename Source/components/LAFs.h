#pragma once

#include <JuceHeader.h>

#include "../util/fonts.h"

// GLOBAL LAF (follows down to all children)
class GlobalLAF : public juce::LookAndFeel_V4 {
public:
	GlobalLAF() {
		setDefaultLookAndFeel(this);
	}
	~GlobalLAF() {
		setDefaultLookAndFeel(nullptr);
	}

	juce::Typeface::Ptr getTypefaceForFont(const juce::Font& font) override
	{
		return fonts::Atkinson400();
	}

private:
};
