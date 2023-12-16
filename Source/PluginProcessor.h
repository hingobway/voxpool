/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "util/types.h"

#define ATTACK (0.010)
#define RELEASE (0.270)

// TODO fix this only 4 right now
#define NUM_CHANNELS (4)

//==============================================================================
/**
*/
class VoxPoolAudioProcessor : public juce::AudioProcessor
{
public:
	//==============================================================================
	VoxPoolAudioProcessor();
	~VoxPoolAudioProcessor() override;

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

	//==============================================================================
	juce::AudioProcessorEditor* createEditor() override;
	bool hasEditor() const override;

	//==============================================================================
	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	//==============================================================================
	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	//==============================================================================
	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	juce::Array<types::MeterVal> getMeterVals();

private:

	juce::AudioProcessorValueTreeState vts;


	typedef struct pChan {
		std::atomic<float>* weight = nullptr;
		std::atomic<float>* on = nullptr;
	};
	std::array<pChan, NUM_CHANNELS> params;

	std::atomic<float>* pGain = nullptr;
	std::atomic<float>* pDepth = nullptr;

	float meterVal; // meter value
	juce::Array<float> meterVals;
	int meterCount; // number of samples in meterVal
	bool meterReset; // reset meterVal for next block

	juce::Array<float> env_last;
	juce::Array<float> poolGain;
	float fac_at;
	float fac_rl;

	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoxPoolAudioProcessor)
};
