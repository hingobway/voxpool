#pragma once

#include <JuceHeader.h>

#include "../util/colors.h"

//==============================================================================
/*
*/
class FooterBlock : public juce::Component
{
public:
	FooterBlock()
	{

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

	}

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FooterBlock)
};
