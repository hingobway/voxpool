#pragma once

#include <JuceHeader.h>

#include "../util/colors.h"

//==============================================================================
/*
*/
class Meter : public juce::Component {
public:
	Meter(float val) : meterVal(val) {

	}
	Meter() : meterVal() {}

	void paint(juce::Graphics& g) override {
		int w = getLocalBounds().getWidth();
		int h = getLocalBounds().getHeight();

		int rdx = 0.06 * h;
		int ylx = 0.22 * h;
		int grx = h - rdx - ylx;

		g.setColour(tw::c(tw::RED_700));
		g.fillRect(0, 0, w, rdx);
		g.setColour(tw::c(tw::AMBER_400));
		g.fillRect(0, rdx, w, ylx);
		g.setColour(tw::c(tw::GREEN_800));
		g.fillRect(0, rdx + ylx, w, grx);

		// set current meter value
		float val = meterVal;
		//float val = (METER_DB_MIN - meterVal) / METER_DB_MIN;
		if (val > 1.0) val = 1.0;
		if (val < 0.0) val = 0.0;
		if (!(val <= 1.0 && val >= 0.0)) val = 0.0;

		// draw meter value
		int fill = roundf((1.0 - val) * h);
		g.setColour(tw::c(tw::ZINC_950, 0.9));
		g.fillRect(0, 0, w, fill);
	}

	void resized() override {

	}

	void setValue(float val) { meterVal = val; }

private:
	float meterVal;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Meter)
};
