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

  Knob(juce::AudioProcessorValueTreeState &vts, juce::String param, bool useDB = false) : vts(vts), param(param)
  {
    // init knob slider
    knob.setLookAndFeel(&lafKnob);
    knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    knob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 55, 20);
    if (useDB)
      knob.setTextValueSuffix("dB");
    knob.setScrollWheelEnabled(false);
    addAndMakeVisible(knob);

    DBG("attempting to attach param " << param);

    attKnob.reset(new SliderAttachment(vts, param, knob));
  }

  void paint(juce::Graphics &g) override
  {
    juce::ignoreUnused(g);
  }

  void resized() override
  {
    knob.setBounds(getLocalBounds());
  }

private:
  juce::AudioProcessorValueTreeState &vts;
  juce::String param;

  LAFKnob lafKnob;
  juce::Slider knob;
  std::unique_ptr<SliderAttachment> attKnob;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob)
};
