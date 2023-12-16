#pragma once

#include <JuceHeader.h>

#include "../util/colors.h"
#include "../util/fonts.h"

#include "Meter.h"

// TODO can/should we link this to NUM_CHANNELS?
#define NUM_FADERS (8)
#define NUM_CHAN_2 (4)
// again, fix this /\

//==============================================================================
/*
* FaderBlock
*   this component holds controls for one channel (most importantly, its weight fader)
*/
class FaderBlock : public juce::Component
{
public:
	using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

	FaderBlock(juce::AudioProcessorValueTreeState& vts, juce::Array<types::MeterVal>& mvs, int number) : mvs(mvs), number(number), enabled()
	{
		// init fader number label
		std::stringstream s{};
		s << number;
		numLabel.setText(s.str(), juce::dontSendNotification);
		numLabel.setColour(juce::Label::textColourId, tw::c(tw::ZINC_100));
		numLabel.setJustificationType(juce::Justification::centred);
		numLabel.setFont(juce::Font(fonts::Atkinson700()).withHeight(30));

		// init weight fader
		if (number <= NUM_CHAN_2) { // TODO this shouldn't be here
			s = std::stringstream();
			s << "ch" << std::setfill('0') << std::setw(2) << number << ".weight";
			weightAtt.reset(new SliderAttachment(vts, s.str(), weightSlider));
			enabled = true;
		}
		weightSlider.setSliderStyle(juce::Slider::LinearVertical);
		weightSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 42, 20);
		weightSlider.setEnabled(enabled);

		// init meter
		meter.reset(new Meter());
		//meter.get()->setValue(0.2 * (number-1));


		// add children

		addAndMakeVisible(numLabel);
		addAndMakeVisible(weightSlider);
		addAndMakeVisible(meter.get());
	}

	void paint(juce::Graphics& g) override
	{
		juce::Path p{};
		p.addRoundedRectangle(getLocalBounds(), 8);

		g.setColour(tw::c(tw::ZINC_800));
		g.fillPath(p);


		if (number <= NUM_CHAN_2) {
			meter.get()->setValue(mvs[number - 1].level);
			//DBG("setting level #" << number << " to " << mvs[number - 1].level);
		}
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
		juce::FlexBox fb2;

		fb.flexDirection = juce::FlexBox::Direction::column;
		{
			fb2.flexDirection = juce::FlexBox::Direction::row;
			{
				fb2.items.add(juce::FlexItem(weightSlider).withFlex(1));
				fb2.items.add(juce::FlexItem(*meter.get()).withWidth(8));
				fb2.items.add(juce::FlexItem().withWidth(5));
			}
			fb.items.add(juce::FlexItem(fb2).withFlex(1));
			fb.items.add(juce::FlexItem(numLabel).withHeight(50));

			// TODO later: for meters, paint the dark part over the colors, so that you can do different colors like in the demo!
		}

		fb.performLayout(getLocalBounds().reduced(10));
	}

private:
	juce::Array<types::MeterVal>& mvs;


	int number;
	bool enabled;

	juce::Slider weightSlider;
	std::unique_ptr<SliderAttachment> weightAtt;

	std::unique_ptr<Meter> meter;

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
	FaderWrapper(juce::AudioProcessorValueTreeState& vts, juce::Array<types::MeterVal>& mvs) {
		for (int i = 0; i < NUM_FADERS; i++) {
			FaderBlock* fader = faders.add(new FaderBlock(vts, mvs, i + 1));
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
