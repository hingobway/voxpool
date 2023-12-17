#pragma once

#include <JuceHeader.h>

#include "../util/fonts.h"
#include "../util/colors.h"

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
		svgKnob = juce::Drawable::createFromImageData(BinaryData::knob2_svg, BinaryData::knob2_svgSize);
	}

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
	{
		juce::Rectangle<int> bounds(x, y, width, height);

		float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		juce::Point<float> sc = svgKnob->getDrawableBounds().getCentre();
		svgKnob->setDrawableTransform(juce::AffineTransform::rotation(angle, sc.getX(), sc.getY()));
		svgKnob->drawWithin(g, bounds.reduced(5).toFloat(), juce::RectanglePlacement::centred, 1.0);
	}

	void drawLabel(juce::Graphics& g, juce::Label& label)
	{
		const juce::Colour textColor = tw::c(tw::ZINC_400);
		label.setColour(juce::Label::textColourId, textColor);

		if (!label.isBeingEdited())
		{
			auto alpha = label.isEnabled() ? 1.0f : 0.5f;
			const juce::Font font(getLabelFont(label));

			g.setColour(label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));
			g.setFont(font);

			auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

			g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
				juce::jmax(1, (int)((float)textArea.getHeight() / font.getHeight())),
				label.getMinimumHorizontalScale());

			g.setColour(label.findColour(juce::Label::outlineColourId).withMultipliedAlpha(alpha));
		}

	}

private:
	std::unique_ptr<juce::Drawable> svgKnob;

};

class LAFFader : public juce::LookAndFeel_V4 {
public:
	LAFFader() {

	}

	void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
		float sliderPos,
		float minSliderPos,
		float maxSliderPos,
		const juce::Slider::SliderStyle style, juce::Slider& slider) override
	{

		const int sliderWidth = 10;
		const int sliderRadius = sliderWidth / 2;

		const juce::Colour trackColour = tw::c(tw::ZINC_900);
		const juce::Colour weightColour = tw::c(tw::SKY_500);
		const juce::Colour bgColor = tw::c(tw::ZINC_800);

		// draw vertical slider

		int ix = x + ((float)width / 2) - (sliderWidth / 2);
		juce::Rectangle<int> track(ix, y, sliderWidth, height);

		g.setColour(weightColour);
		g.fillRect(track);



		float val = slider.getProperties().getWithDefault("weight", 0.0);
		if (val > 1.0) val = 1.0;
		if (val < 0.0) val = 0.0;
		if (!(val <= 1.0 && val >= 0.0)) val = 0.0;

		int fill = roundf((1.0 - val) * height);

		g.setColour(trackColour);
		g.fillRect(track.withHeight(fill));


		// clip track into rounded corners
		juce::Path ro;
		juce::Path ri;
		ro.setUsingNonZeroWinding(false);
		ro.addRectangle(track);
		ri.addRoundedRectangle(track, sliderRadius);
		ro.addPath(ri);
		g.setColour(bgColor);
		g.fillPath(ro);


		// thumb

		const int thumbHeight = 16;
		const int thumbWidth = 42;
		juce::Colour thumbColour = tw::c(tw::ZINC_200);

		const int thumbRadius = thumbHeight / 2;
		float kx = (float)x + (float)width * 0.5f;
		float ky = sliderPos;

		juce::Path thumb;
		thumb.addRoundedRectangle((float)x + ((float)width - thumbWidth) / 2.0, sliderPos - thumbRadius, thumbWidth, thumbHeight, thumbRadius);

		g.setColour(thumbColour);
		g.fillPath(thumb);
	}

	void drawLabel(juce::Graphics& g, juce::Label& label)
	{
		const juce::Colour textColor = tw::c(tw::ZINC_400);
		label.setColour(juce::Label::textColourId, textColor);

		if (!label.isBeingEdited())
		{
			auto alpha = label.isEnabled() ? 1.0f : 0.5f;
			const juce::Font font(getLabelFont(label));

			g.setColour(label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));
			g.setFont(font);

			auto textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

			g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
				juce::jmax(1, (int)((float)textArea.getHeight() / font.getHeight())),
				label.getMinimumHorizontalScale());

			g.setColour(label.findColour(juce::Label::outlineColourId).withMultipliedAlpha(alpha));
		}

	}

private:
};

class LAFOnButton : public juce::LookAndFeel_V4 {
public:
	LAFOnButton() {
		svgOn = juce::Drawable::createFromImageData(BinaryData::button_on_svg, BinaryData::button_on_svgSize);
		svgOff = juce::Drawable::createFromImageData(BinaryData::button_off_svg, BinaryData::button_off_svgSize);
	}

	void drawDrawableButton(juce::Graphics& g, juce::DrawableButton& button,
		bool /*shouldDrawButtonAsHighlighted*/, bool /*shouldDrawButtonAsDown*/) override
	{
		bool toggleState = button.getToggleState();

		const int size = 30;
		const int radius = 3;
		const juce::Colour bgColor(tw::c(tw::ZINC_900));

		auto baseColor = bgColor.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
			.withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

		juce::Rectangle<int> bounds(0, 0, size, size);

		// draw button backgroudn
		juce::Path p;
		p.addRoundedRectangle(bounds, radius);
		g.setColour(baseColor);
		g.fillPath(p);

		// draw button icon
		int bcx = bounds.getCentreX();
		int bcy = bounds.getCentreY();
		if (toggleState) {
			int cx = svgOn->getBounds().getCentreX();
			int cy = svgOn->getBounds().getCentreY();

			svgOn->drawAt(g, bcx - cx, bcy - cy, 1.0);
		}
		else {
			int cx = svgOff->getBounds().getCentreX();
			int cy = svgOff->getBounds().getCentreY();

			svgOff->drawAt(g, bcx - cx, bcy - cy, 1.0);
		}
	}

private:

	std::unique_ptr<juce::Drawable> svgOn;
	std::unique_ptr<juce::Drawable> svgOff;

};
