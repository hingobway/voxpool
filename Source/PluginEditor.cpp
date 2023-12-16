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
	: AudioProcessorEditor(&p), audioProcessor(p), vts(vts), meterVal()
{
	setLookAndFeel(&globalLAF);

	// init children

	titleBlock.reset(new TitleBlock());
	controlsBlock.reset(new ControlsBlock(vts));

	addAndMakeVisible(titleBlock.get());
	addAndMakeVisible(controlsBlock.get());


	// setup window

	setSize(860, 490);
	setResizable(false, false);

	startTimer(50);

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

	//// (Our component is opaque, so we must completely fill the background with a solid colour)
	//g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	//int METER_H = 250;
	//int METER_W = 8;
	//int METER_X = 90;
	//int METER_Y = 96;

	//// set current meter dimensions
	////int h = (int)roundf(meterVal * METER_H);
	//int h = (int)roundf((METER_DB_MIN - meterVal) / METER_DB_MIN * METER_H);
	//auto meter = juce::Rectangle<int>(METER_W, h);
	//meter.setPosition(METER_X, METER_Y + METER_H - h);

	//// draw meter
	//g.setColour(juce::Colour(0xff000000));
	//g.fillRect(METER_X, METER_Y, METER_W, METER_H);
	//g.setColour(juce::Colour(0xff0022ff));
	//g.fillRect(meter);
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
	meterVal = audioProcessor.getMeterVal();
	float a{};
	if (meterVal < 0.001) a = METER_DB_MIN;
	else a = 20.0 * log10f(meterVal);

	meterVal = a;

	repaint();
}
