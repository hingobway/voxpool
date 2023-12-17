#pragma once

#include <JuceHeader.h>

#include "../util/colors.h"

#include "Knob.h"

//==============================================================================
/*
*/
class FooterBlock : public juce::Component
{
public:
	FooterBlock(juce::AudioProcessorValueTreeState& vts)
	{
		// init knobs
		gainKnob.reset(new Knob(vts, "gain", true));
		depthKnob.reset(new Knob(vts, "depth"));

		// load instructions text
		svgInstructions = juce::Drawable::createFromImageData(BinaryData::instructions_svg, BinaryData::instructions_svgSize);

		// add children

		addAndMakeVisible(gainKnob.get());
		addAndMakeVisible(depthKnob.get());
	}

	void paint(juce::Graphics& g) override
	{
		// fill bg
		juce::Path p{};
		p.addRoundedRectangle(getLocalBounds(), 8);
		g.setColour(tw::c(tw::ZINC_800));
		g.fillPath(p);

		// add instructions
		juce::Point<int> cc = getLocalBounds().getCentre();               // container center
		juce::Point<int> sp = svgInstructions->getBounds().getPosition(); // svg position
		juce::Point<int> sc = svgInstructions->getBounds().getCentre();   // svg center
		svgInstructions->drawAt(g, PX - sp.getX(), cc.getY() - sc.getY(), 1.0);
	}

	void resized() override
	{
		juce::FlexBox fb;

		fb.flexDirection = juce::FlexBox::Direction::row;
		{
			fb.items.add(juce::FlexItem().withFlex(1));
			fb.items.add(juce::FlexItem(*depthKnob.get()).withWidth(60));
			fb.items.add(juce::FlexItem().withWidth(10));
			fb.items.add(juce::FlexItem(*gainKnob.get()).withWidth(60));
		}

		fb.performLayout(getLocalBounds().withTrimmedLeft(PX).withTrimmedRight(PX).withTrimmedTop(PY).withTrimmedBottom(PY));
	}

private:

	int PX = 24;
	int PY = 8;

	std::unique_ptr<Knob> gainKnob;
	std::unique_ptr<Knob> depthKnob;

	std::unique_ptr<juce::Drawable> svgInstructions;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterBlock)
};
