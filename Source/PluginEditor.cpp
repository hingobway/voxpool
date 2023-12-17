/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "util/colors.h"

//==============================================================================
VoxPoolAudioProcessorEditor::VoxPoolAudioProcessorEditor(VoxPoolAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
	: AudioProcessorEditor(&p), audioProcessor(p), vts(vts), meterVals()
{
	setLookAndFeel(&globalLAF);

	// init children

	titleBlock.reset(new TitleBlock());
	controlsBlock.reset(new ControlsBlock(vts, meterVals));

	addAndMakeVisible(titleBlock.get());
	addAndMakeVisible(controlsBlock.get());


	// setup window

	setSize(860, 510);
	setResizable(false, false);

	startTimer(120);

}

VoxPoolAudioProcessorEditor::~VoxPoolAudioProcessorEditor()
{
	setLookAndFeel(nullptr);
}

//==============================================================================
void VoxPoolAudioProcessorEditor::paint(juce::Graphics& g)
{
	g.setColour(tw::c(tw::ZINC_900));
	g.fillAll();
}

void VoxPoolAudioProcessorEditor::resized()
{
	int GAP = 10;

	juce::FlexBox fb;

	fb.flexDirection = juce::FlexBox::Direction::column;
	{
		fb.items.add(juce::FlexItem(*titleBlock.get()).withHeight(54));
		fb.items.add(juce::FlexItem().withHeight(GAP));
		fb.items.add(juce::FlexItem(*controlsBlock.get()).withFlex(1));
	}

	fb.performLayout(getLocalBounds().reduced(GAP));
}

void VoxPoolAudioProcessorEditor::timerCallback()
{
	meterVals = audioProcessor.getMeterVals();

	repaint(); // TODO there must be a way to avoid all of these repaints
}
