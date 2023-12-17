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
	using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

	FaderBlock(juce::AudioProcessorValueTreeState& vts, juce::Array<types::MeterVal>& mvs, int number) : mvs(mvs), number(number), enabled(), onButton("onButton", juce::DrawableButton::ButtonStyle::ImageFitted)
	{
		// init fader number label
		std::stringstream s{};
		s << number;
		numLabel.setText(s.str(), juce::dontSendNotification);
		numLabel.setColour(juce::Label::textColourId, tw::c(tw::ZINC_100));
		numLabel.setJustificationType(juce::Justification::centred);
		numLabel.setFont(juce::Font(fonts::Atkinson700()).withHeight(30));

		// init attachments
		if (number <= NUM_CHAN_2) { // TODO this shouldn't be here
			enabled = true;

			s = std::stringstream();
			s << "ch" << std::setfill('0') << std::setw(2) << number << ".weight";
			weightAtt.reset(new SliderAttachment(vts, s.str(), weightSlider));

			s = std::stringstream();
			s << "ch" << std::setfill('0') << std::setw(2) << number << ".on";
			onAtt.reset(new ButtonAttachment(vts, s.str(), onButton));
		}

		// weight fader
		weightSlider.setLookAndFeel(&lafFader);
		weightSlider.setSliderStyle(juce::Slider::LinearVertical);
		weightSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
		weightSlider.setTextValueSuffix("dB");
		weightSlider.setScrollWheelEnabled(false);
		weightSlider.setEnabled(enabled);

		// on button
		onButton.setClickingTogglesState(true);
		onButton.setLookAndFeel(&lafOnButton);

		// init meter
		meter.reset(new Meter());


		// add children

		addAndMakeVisible(numLabel);
		addAndMakeVisible(weightSlider);
		addAndMakeVisible(onButton);
		addAndMakeVisible(meter.get());
	}
	~FaderBlock() {
		weightSlider.setLookAndFeel(nullptr);
		onButton.setLookAndFeel(nullptr);
	}

	void paint(juce::Graphics& g) override
	{
		juce::Path p{};
		p.addRoundedRectangle(getLocalBounds(), 8);

		g.setColour(tw::c(tw::ZINC_800));
		g.fillPath(p);


		if (number <= NUM_CHAN_2) {
			meter.get()->setValue(mvs[number - 1].level);
			weightSlider.getProperties().set("weight", mvs[number - 1].pool);
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
		juce::FlexBox fb; // wrapper
		juce::FlexBox fb2; // fader/meter
		juce::FlexBox fb3; // number/on

		fb.flexDirection = juce::FlexBox::Direction::column;
		{
			fb2.flexDirection = juce::FlexBox::Direction::row;
			{
				fb2.items.add(juce::FlexItem(weightSlider).withFlex(1));
				fb2.items.add(juce::FlexItem(*meter.get()).withWidth(8));
				fb2.items.add(juce::FlexItem().withWidth(5));
			}
			fb.items.add(juce::FlexItem(fb2).withFlex(1));

			fb.items.add(juce::FlexItem().withHeight(10));

			fb3.flexDirection = juce::FlexBox::Direction::row;
			fb3.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
			{
				fb3.items.add(juce::FlexItem(onButton).withWidth(30));
				fb3.items.add(juce::FlexItem(numLabel).withWidth(30));
			}
			fb.items.add(juce::FlexItem(fb3).withHeight(30));

		}

		fb.performLayout(getLocalBounds().reduced(10));
	}

private:
	juce::Array<types::MeterVal>& mvs;


	int number;
	bool enabled;

	juce::Slider weightSlider;
	std::unique_ptr<SliderAttachment> weightAtt;

	juce::DrawableButton onButton;
	std::unique_ptr<ButtonAttachment> onAtt;

	std::unique_ptr<Meter> meter;

	juce::Label numLabel;


	LAFFader lafFader;
	LAFOnButton lafOnButton;

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
