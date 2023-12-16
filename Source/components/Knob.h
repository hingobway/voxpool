#pragma once

#include <JuceHeader.h>

#include "LAFs.h"

//==============================================================================
/*
*/
class Knob : public juce::Component
{
public:
	using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

	Knob(juce::AudioProcessorValueTreeState& vts, juce::String param) : vts(vts), param(param)
	{
		knob.setLookAndFeel(&lafKnob);
		knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
		knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
		addAndMakeVisible(knob);

		DBG("attempting to attach param " << param);

		attKnob.reset(new SliderAttachment(vts, param, knob));

	}

	void paint(juce::Graphics& g) override
	{

	}

	void resized() override
	{
		knob.setBounds(getLocalBounds());
	}

private:
	juce::AudioProcessorValueTreeState& vts;
	juce::String param;

	LAFKnob lafKnob;
	juce::Slider knob;
	std::unique_ptr<SliderAttachment> attKnob;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob)
};
