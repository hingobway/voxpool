/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "util/types.h"

#include "components/LAFs.h"
#include "components/TitleBlock.h"
#include "components/ControlsBlock.h"

//==============================================================================
/**
*/
class VoxPoolAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
	VoxPoolAudioProcessorEditor(VoxPoolAudioProcessor&, juce::AudioProcessorValueTreeState&);
	~VoxPoolAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

	void timerCallback() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	VoxPoolAudioProcessor& audioProcessor;
	juce::AudioProcessorValueTreeState& vts;
	GlobalLAF globalLAF;

	// children
	std::unique_ptr<TitleBlock> titleBlock;
	std::unique_ptr<ControlsBlock> controlsBlock;

	juce::Array<types::MeterVal> meterVals;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoxPoolAudioProcessorEditor)
};
