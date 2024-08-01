#pragma once

#include <JuceHeader.h>

#include "../util/colors.h"
#include "../util/fonts.h"

//==============================================================================
/*
 */
class TitleBlock : public juce::Component
{
public:
  TitleBlock()
  {
    // init title
    title.setText("vox/pool", juce::dontSendNotification);
    title.setColour(juce::Label::textColourId, tw::c(tw::ZINC_100));
    title.setFont(juce::FontOptions(fonts::Atkinson400()).withHeight(30));
    title.setJustificationType(juce::Justification::centredLeft);

    // init author
    author.setText("[foster]", juce::dontSendNotification);
    author.setColour(juce::Label::textColourId, tw::c(tw::ZINC_400));
    author.setFont(juce::FontOptions(fonts::Atkinson700()).withHeight(15));
    author.setJustificationType(juce::Justification::centredRight);

    // add children

    addAndMakeVisible(title);
    addAndMakeVisible(author);
  }

  void paint(juce::Graphics &g) override
  {
    juce::Path p{};
    p.addRoundedRectangle(getLocalBounds(), 12);

    g.setColour(tw::c(tw::ZINC_800));
    g.fillPath(p);
  }

  void resized() override
  {
    juce::FlexBox fb;

    fb.flexDirection = juce::FlexBox::Direction::row;
    {
      fb.items.add(juce::FlexItem(title).withFlex(1));
      fb.items.add(juce::FlexItem(author).withFlex(1));
    }

    fb.performLayout(getLocalBounds().withTrimmedLeft(PX).withTrimmedRight(PX));
  }

private:
  int PX = 24;

  juce::Label title;
  juce::Label author;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitleBlock)
};
