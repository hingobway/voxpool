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
                         .withInput("Automix Ch05", juce::AudioChannelSet::mono(), true)
                         .withInput("Automix Ch06", juce::AudioChannelSet::mono(), true)
                         .withInput("Automix Ch07", juce::AudioChannelSet::mono(), true)
                         .withInput("Automix Ch08", juce::AudioChannelSet::mono(), true)

                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),

      vts(*this, nullptr, juce::Identifier("FosterVoxPool"),
          {// mixer properties
           std::make_unique<juce::AudioParameterFloat>(
               juce::ParameterID("depth", 1), "Depth", 0.01, 1.0, 0.85),
           std::make_unique<juce::AudioParameterFloat>(
               juce::ParameterID("gain", 1), "Output Gain", -15.0, 15.0, 0.0),

           // ch01
           std::make_unique<juce::AudioParameterFloat>(
               juce::ParameterID("ch01.weight", 1), "Ch01 Weight", -10.0, 10.0, 0.0),
           std::make_unique<juce::AudioParameterBool>(
               juce::ParameterID("ch01.on", 1), "Ch01 On", true),
           // ch02
           std::make_unique<juce::AudioParameterFloat>(
               juce::ParameterID("ch02.weight", 1), "Ch02 Weight", -10.0, 10.0, 0.0),
           std::make_unique<juce::AudioParameterBool>(
               juce::ParameterID("ch02.on", 1), "Ch02 On", true),
           // ch03
           std::make_unique<juce::AudioParameterFloat>(
               juce::ParameterID("ch03.weight", 1), "Ch03 Weight", -10.0, 10.0, 0.0),
           std::make_unique<juce::AudioParameterBool>(
               juce::ParameterID("ch03.on", 1), "Ch03 On", true),
           // ch04
           std::make_unique<juce::AudioParameterFloat>(
               juce::ParameterID("ch04.weight", 1), "Ch04 Weight", -10.0, 10.0, 0.0),
           std::make_unique<juce::AudioParameterBool>(
               juce::ParameterID("ch04.on", 1), "Ch04 On", true),
           // ch05
           std::make_unique<juce::AudioParameterFloat>(
               juce::ParameterID("ch05.weight", 1), "Ch05 Weight", -10.0, 10.0, 0.0),
           std::make_unique<juce::AudioParameterBool>(
               juce::ParameterID("ch05.on", 1), "Ch05 On", true),
           // ch06
           std::make_unique<juce::AudioParameterFloat>(
               juce::ParameterID("ch06.weight", 1), "Ch06 Weight", -10.0, 10.0, 0.0),
           std::make_unique<juce::AudioParameterBool>(
               juce::ParameterID("ch06.on", 1), "Ch06 On", true),
           // ch07
           std::make_unique<juce::AudioParameterFloat>(
               juce::ParameterID("ch07.weight", 1), "Ch07 Weight", -10.0, 10.0, 0.0),
           std::make_unique<juce::AudioParameterBool>(
               juce::ParameterID("ch07.on", 1), "Ch07 On", true),
           // ch08
           std::make_unique<juce::AudioParameterFloat>(
               juce::ParameterID("ch08.weight", 1), "Ch08 Weight", -10.0, 10.0, 0.0),
           std::make_unique<juce::AudioParameterBool>(
               juce::ParameterID("ch08.on", 1), "Ch08 On", true)}),
      // init multichannel arrays
      levelSum(0.0, NUM_CHANNELS), poolSum(0.0, NUM_CHANNELS), env_last(0.0, NUM_CHANNELS)

#endif
{
  DBG("constructed audioprocessor");

  // assign pointers to plugin parameters

  pGain = vts.getRawParameterValue("gain");
  pDepth = vts.getRawParameterValue("depth");
  for (int i = 0; i < NUM_CHANNELS; i++)
  {
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
  return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
            // so this should be at least 1, even if you're not really implementing programs.
}

int VoxPoolAudioProcessor::getCurrentProgram()
{
  return 0;
}

void VoxPoolAudioProcessor::setCurrentProgram(int index)
{

  juce::ignoreUnused(index);
}

const juce::String VoxPoolAudioProcessor::getProgramName(int index)
{
  juce::ignoreUnused(index);

  return {};
}

void VoxPoolAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
  juce::ignoreUnused(index);
  juce::ignoreUnused(newName);
}

//==============================================================================
void VoxPoolAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  juce::ignoreUnused(samplesPerBlock);

  // calculate IIR sample gain based on tuned attack/release times
  float TC = logf(9.0);
  fac_at = expf(-TC / (float)(ATTACK * sampleRate));
  fac_rl = expf(-TC / (float)(RELEASE * sampleRate));
}

void VoxPoolAudioProcessor::releaseResources()
{
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VoxPoolAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
  // make sure mixer inputs are stereo or mono.
  // we don't care about main bus layouts as we will simply output a mono mix to all channels.
  // (this allows routing flexibility for some daws.)
  for (int i = 0; i < layouts.inputBuses.size(); i++)
  {
    if (layouts.getChannelSet(true, i) == juce::AudioChannelSet::disabled())
      return false;
    if (layouts.getNumChannels(true, i) > 2)
      return false;
  }

  return true;
}
#endif

void VoxPoolAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer, juce::MidiBuffer &midiMessages)
{

  juce::ignoreUnused(midiMessages);
  juce::ScopedNoDenormals noDenormals;

  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  // some daws like ableton will always send stereo busses; if so we'll sum those to mono.
  bool stereo_ins{};
  if (totalNumInputChannels >= 2 * NUM_CHANNELS)
    stereo_ins = true;

  // clear empty channels (safeguard)
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  // convert weights from dB
  juce::Array<float> g(0.0, NUM_CHANNELS);
  for (int c = 0; c < NUM_CHANNELS; c++)
  {
    g.set(c, powf(10, (*params[c].weight) / 20.0f));
  }
  float outGain = powf(10, (*pGain) / 20.0f);

  // process audio
  auto audio = buffer.getArrayOfWritePointers();
  for (int i = 0; i < buffer.getNumSamples(); i++)
  {

    juce::Array<float> env(0.0, NUM_CHANNELS);

    // calculate envelope value for each channel
    for (int c = 0; c < NUM_CHANNELS; c++)
    {
      // sum stereo inputs if necessary
      if (stereo_ins)
      {
        int cs = c * 2;
        audio[c][i] = (audio[cs][i] + audio[cs + 1][i]) * 0.5f;
      }
      float x = g[c] * audio[c][i]; // apply user-defined weight first
      audio[c][i] = x;              // from now on everything will be in mono

      float xsq = x * x; // rectified sample
      float fg = 0.0f;   // filter gain

      // choose whether to use attack or release time based on last value
      if (xsq > env_last[c])
        fg = fac_at;
      else
        fg = fac_rl;

      env.set(c, (1.0f - fg) * xsq + fg * env_last[c]); // IIR resonant LPF
      env_last.set(c, env[c]);                          // store sample for next iteration
    }

    // sum envelopes to determine channel gain
    float totalGain = 0.0;
    for (const float &e : env)
      totalGain += e;

    // calculate pool gain and modified signal
    float y = 0.0;
    for (int c = 0; c < NUM_CHANNELS; c++)
    {
      float depth = *pDepth;
      float poolGain = env[c] / (depth * totalGain + (1.0f - depth) * env[c]);

      // as long as this channel is turned on, add modified signal to output
      if (*params[c].on)
      {
        y += poolGain * audio[c][i];
      }

      // update meter values
      levelSum.set(c, levelSum[c] + audio[c][i] * audio[c][i]);
      poolSum.set(c, poolSum[c] + poolGain);
    }
    meterCount++; // must keep count of how many samples are in meter buffer for averaging

    // output to all output channels
    for (int c = 0; c < totalNumOutputChannels; c++)
    {
      audio[c][i] = outGain * y;
    }
  }
}

juce::Array<types::MeterVal> VoxPoolAudioProcessor::getMeterVals()
{
  // meterval type contains one fader's meters: a gain pool level and an audio level together
  juce::Array<types::MeterVal> mvs{};

  // calculate RMS/averages
  for (int i = 0; i < NUM_CHANNELS; i++)
  {
    types::MeterVal mv{};
    mv.level = sqrtf(levelSum[i] / (float)meterCount) * 2;
    mv.pool = poolSum[i] / (float)meterCount;

    mvs.add(mv);

    // reset for next measurement
    levelSum.set(i, 0.0);
    poolSum.set(i, 0.0);
  }

  meterCount = 0;
  return mvs;
}

//==============================================================================
bool VoxPoolAudioProcessor::hasEditor() const
{
  return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *VoxPoolAudioProcessor::createEditor()
{
  return new VoxPoolAudioProcessorEditor(*this, vts);
}

//==============================================================================
void VoxPoolAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
  DBG("called get state info");

  juce::ValueTree state = vts.copyState(); // copy current state

  // convert to xml version of current state
  std::unique_ptr<juce::XmlElement> xml(state.createXml()); // pointer to xml object
  // copy that xml into provided memory block
  copyXmlToBinary(*xml, destData);
}

void VoxPoolAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
  DBG("called set state info");

  // convert stored state to xml
  std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

  // ensure state is valid
  if (xml.get() == nullptr)
    return;
  if (!xml->hasTagName(vts.state.getType()))
    return;

  // swap current value tree for the one in the memory block
  vts.replaceState(juce::ValueTree::fromXml(*xml));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
  return new VoxPoolAudioProcessor();
}
