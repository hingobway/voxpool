#pragma once

#include <JuceHeader.h>

#include "../util/colors.h"
#include "../util/fonts.h"

// TODO can/should we link this to NUM_CHANNELS?
#define NUM_FADERS (8)

//==============================================================================
/*
* FaderBlock
*   this component holds controls for one channel (most importantly, its weight fader)
*/
class FaderBlock : public juce::Component
{
public:
	using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

	FaderBlock(juce::AudioProcessorValueTreeState& vts, int number) : number(number), enabled()
	{
		std::stringstream s{};
		s << number;
		numLabel.setText(s.str(), juce::dontSendNotification);
		numLabel.setColour(juce::Label::textColourId, tw::c(tw::ZINC_100));
		numLabel.setJustificationType(juce::Justification::centred);
		numLabel.setFont(juce::Font(fonts::Atkinson700()).withHeight(30));

		if (number <= 4) { // TODO this shouldn't be here
			s = std::stringstream();
			s << "ch" << std::setfill('0') << std::setw(2) << number << ".weight";
			weightAtt.reset(new SliderAttachment(vts, s.str(), weightSlider));
			enabled = true;
		}

		weightSlider.setSliderStyle(juce::Slider::LinearVertical);
		weightSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
		weightSlider.setEnabled(enabled);

		addAndMakeVisible(numLabel);
		addAndMakeVisible(weightSlider);
	}

	void paint(juce::Graphics& g) override
	{
		juce::Path p{};
		p.addRoundedRectangle(getLocalBounds(), 8);

		g.setColour(tw::c(tw::ZINC_800));
		g.fillPath(p);
	}

	void paintOverChildren(juce::Graphics& g) override
	{
		if (!enabled) {
			g.setColour(tw::c(tw::ZINC_700, 0.9));
			g.fillAll();
		}
	}

	void resized() override
	{
		juce::FlexBox fb;

		fb.flexDirection = juce::FlexBox::Direction::column;
		{
			fb.items.add(juce::FlexItem(weightSlider).withFlex(1));
			fb.items.add(juce::FlexItem(numLabel).withHeight(50));

			// TODO later: for meters, paint the dark part over the colors, so that you can do different colors like in the demo!
		}

		fb.performLayout(getLocalBounds());
	}

private:
	int number;
	bool enabled;

	juce::Slider weightSlider;
	std::unique_ptr<SliderAttachment> weightAtt;

	juce::Label numLabel;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaderBlock)
};

/*
* FaderWrapper
*   this component holds and instantiates the 8 fader blocks within it
*/
class FaderWrapper : public juce::Component
{
public:
	FaderWrapper(juce::AudioProcessorValueTreeState& vts) {
		for (int i = 0; i < NUM_FADERS; i++) {
			FaderBlock* fader = faders.add(new FaderBlock(vts, i + 1));
			addAndMakeVisible(fader);
		}
	}

	void paint(juce::Graphics& g) override {

	}

	void resized() override {
		juce::FlexBox fb;

		fb.flexDirection = juce::FlexBox::Direction::row;
		{
			for (int i = 0; i < faders.size(); i++) {
				if (i) fb.items.add(juce::FlexItem().withWidth(GAP));
				fb.items.add(juce::FlexItem(*faders[i]).withFlex(1));
			}
		}

		fb.performLayout(getLocalBounds());
	}

private:

	int GAP = 10;

	juce::OwnedArray<FaderBlock> faders;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaderWrapper)
};
