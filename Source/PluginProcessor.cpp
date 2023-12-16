/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VoxPoolAudioProcessor::VoxPoolAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
		.withInput("Automix Ch01", juce::AudioChannelSet::mono(), true)
		.withInput("Automix Ch02", juce::AudioChannelSet::mono(), true)
		.withInput("Automix Ch03", juce::AudioChannelSet::mono(), true)
		.withInput("Automix Ch04", juce::AudioChannelSet::mono(), true)
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	), vts(*this, nullptr, juce::Identifier("FosterVoxPool"), {
		// ch01
		std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID("ch01.weight", 0), "Ch01 Weight", -15.0, 15.0, 0.0),
		std::make_unique<juce::AudioParameterBool>(
			juce::ParameterID("ch01.on", 0), "Ch01 On", true),
		// ch02
		std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID("ch02.weight", 0), "Ch02 Weight", -15.0, 15.0, 0.0),
		std::make_unique<juce::AudioParameterBool>(
			juce::ParameterID("ch02.on", 0), "Ch02 On", true),
		// ch03
		std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID("ch03.weight", 0), "Ch03 Weight", -15.0, 15.0, 0.0),
		std::make_unique<juce::AudioParameterBool>(
			juce::ParameterID("ch03.on", 0), "Ch03 On", true),
		// ch04
		std::make_unique<juce::AudioParameterFloat>(
			juce::ParameterID("ch04.weight", 0), "Ch04 Weight", -15.0, 15.0, 0.0),
		std::make_unique<juce::AudioParameterBool>(
			juce::ParameterID("ch04.on", 0), "Ch04 On", true)
		})
#endif
{
	DBG("constructed audioprocessor");

	// assign pointers to plugin parameters
	for (int i = 0; i < NUM_CHANNELS; i++) {
		std::stringstream s{};
		s << "ch" << std::setfill('0') << std::setw(2) << i + 1;
		juce::String prefix = s.str();
		s.clear();
		s.str(std::string());

		s << prefix << ".weight";
		params[i].weight = vts.getRawParameterValue(s.str());
		s.clear();
		s.str(std::string());

		s << prefix << ".on";
		params[i].on = vts.getRawParameterValue(s.str());
	}


	meterVals.resize(NUM_CHANNELS);
}

VoxPoolAudioProcessor::~VoxPoolAudioProcessor()
{
}

//==============================================================================
const juce::String VoxPoolAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool VoxPoolAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool VoxPoolAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool VoxPoolAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double VoxPoolAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int VoxPoolAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int VoxPoolAudioProcessor::getCurrentProgram()
{
	return 0;
}

void VoxPoolAudioProcessor::setCurrentProgram(int index)
{
}

const juce::String VoxPoolAudioProcessor::getProgramName(int index)
{
	return {};
}

void VoxPoolAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void VoxPoolAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void VoxPoolAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VoxPoolAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
	// make sure mixer inputs are stereo or mono. 
	// we don't care about main bus layouts as we will simply output a mono mix to all channels.
	// (this allows routing flexibility for some daws.)
	for (int i = 0; i < layouts.inputBuses.size(); i++) {
		if (layouts.getNumChannels(true, i) > 2) return false;
	}

	return true;
}
#endif

void VoxPoolAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// some daws like ableton will always send stereo busses; if so we'll sum those to mono.
	bool stereo_ins{}; // TODO test whether this works again
	if (totalNumInputChannels == 2 * NUM_CHANNELS) stereo_ins = true;

	//DBG("input chans:" << totalNumInputChannels);
	//DBG("output chans:" << totalNumOutputChannels);

	// clear empty channels (safeguard)
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// convert weights from dB
	std::array<float, NUM_CHANNELS> g{};
	for (int c = 0; c < NUM_CHANNELS; c++) {
		g[c] = powf(10, (*params[c].weight) / 20.0);
	}

	// process audio
	auto audio = buffer.getArrayOfWritePointers();
	for (int i = 0; i < buffer.getNumSamples(); i++) {
		float y = 0.0;

		// sum data
		for (int c = 0; c < NUM_CHANNELS; c++) {
			float x = audio[c][i];
			if (stereo_ins) {
				int cs = c * 2;
				x = (audio[cs][i] + audio[cs + 1][i]) * 0.5;
			}
			x = g[c] * x;

			y += x;

			// update meter for this channel
			meterVals.set(c, meterVals[c] + powf(x, 2.0));
		}

		y /= 4.0; // normalize

		// update global meter
		meterVal += powf(y, 2.0);
		meterCount++; // this stays when above line gets removed

		// output to all channels
		for (int c = 0; c < totalNumOutputChannels; c++) {
			audio[c][i] = y;
		}
	}
}

juce::Array<types::MeterVal> VoxPoolAudioProcessor::getMeterVals()
{

	juce::Array<types::MeterVal> mvs{};

	for (float& v : meterVals) {
		float m = v;
		float c = (float)meterCount;
		m = sqrtf(m / c) * 2;
		//if (m > 1.0) m = 1.0;

		types::MeterVal mv{};
		mv.level = m;
		mv.pool = 0.5; // TODO fix this

		mvs.add(mv);

		v = 0.0; // reset for next measurement
	}

	//DBG("meterVal/meterCount/out " << meterVal << "/" << meterCount << "/" << m);

	//meterVal = 0.0;

	meterCount = 0;
	return mvs;
}

//==============================================================================
bool VoxPoolAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VoxPoolAudioProcessor::createEditor()
{
	// TODO custom editor
	return new VoxPoolAudioProcessorEditor(*this, vts);

	//return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void VoxPoolAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	DBG("called get state info");

	juce::ValueTree state = vts.copyState(); // copy current state

	// convert to xml version of current state
	std::unique_ptr<juce::XmlElement> xml(state.createXml()); // pointer to xml object
	// copy that xml into provided memory block
	copyXmlToBinary(*xml, destData);
}

void VoxPoolAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	DBG("called set state info");

	// convert stored state to xml
	std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

	// ensure state is valid
	if (xml.get() == nullptr) return;
	if (!xml->hasTagName(vts.state.getType())) return;

	// swap current value tree for the one in the memory block
	vts.replaceState(juce::ValueTree::fromXml(*xml));
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new VoxPoolAudioProcessor();
}
