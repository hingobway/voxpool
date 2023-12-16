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
		gainKnob.reset(new Knob(vts, "gain"));
		depthKnob.reset(new Knob(vts, "depth"));


		addAndMakeVisible(gainKnob.get());
		addAndMakeVisible(depthKnob.get());
	}

	void paint(juce::Graphics& g) override
	{
		juce::Path p{};
		p.addRoundedRectangle(getLocalBounds(), 8);

		g.setColour(tw::c(tw::ZINC_800));
		g.fillPath(p);
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

		fb.performLayout(getLocalBounds().withTrimmedLeft(PX).withTrimmedRight(PX).withTrimmedBottom(10));
	}

private:

	int PX = 24;

	std::unique_ptr<Knob> gainKnob;
	std::unique_ptr<Knob> depthKnob;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterBlock)
};
