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

class LAFKnob : public juce::LookAndFeel_V4 {
public:

	LAFKnob() {
		svgKnob = juce::Drawable::createFromImageData(BinaryData::knob_svg, BinaryData::knob_svgSize);
	}

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
	{
		float PI = juce::MathConstants<float>::pi;

		juce::Rectangle<int> bounds(x, y, width, height);
		int bcx = bounds.getCentreX();
		int bcy = bounds.getCentreY();

		int iw = svgKnob->getWidth();
		int ih = svgKnob->getHeight();
		int cx = svgKnob->getBounds().getCentreX();
		int cy = svgKnob->getBounds().getCentreY();

		float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		svgKnob->setTransform(juce::AffineTransform::rotation(angle, cx, cy));
		svgKnob->drawAt(g, bcx - cx, bcy - cy, 1.0);
	}

private:
	std::unique_ptr<juce::Drawable> svgKnob;

};
