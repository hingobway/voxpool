#pragma once

#include <JuceHeader.h>

#include "../util/colors.h"
#include "../util/types.h"

#include "FaderBlock.h"
#include "FooterBlock.h"

//==============================================================================
/*
*/
class ControlsBlock : public juce::Component
{
public:
	ControlsBlock(juce::AudioProcessorValueTreeState& vts, juce::Array<types::MeterVal>& mvs)
	{
		// init children
		faderWrapper.reset(new FaderWrapper(vts, mvs));
		footerBlock.reset(new FooterBlock());

		addAndMakeVisible(faderWrapper.get());
		addAndMakeVisible(footerBlock.get());
	}

	void paint(juce::Graphics& g) override
	{
		juce::Path p{};
		p.addRoundedRectangle(getLocalBounds(), 8 + GAP);

		g.setColour(tw::c(tw::ZINC_700));
		g.fillPath(p);
	}

	void resized() override
	{
		juce::FlexBox fb;

		fb.flexDirection = juce::FlexBox::Direction::column;
		{
			fb.items.add(juce::FlexItem(*faderWrapper.get()).withFlex(1));
			fb.items.add(juce::FlexItem().withHeight(GAP));
			fb.items.add(juce::FlexItem(*footerBlock.get()).withHeight(83));
		}

		fb.performLayout(getLocalBounds().reduced(GAP));
	}

private:

	int GAP = 10;

	// children
	std::unique_ptr<FaderWrapper> faderWrapper;
	std::unique_ptr<FooterBlock> footerBlock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ControlsBlock)
};
