#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Parameters.h"
#include <vector>
#include "Voice.h"

class MikaMicro : public IPlug
{
public:
	MikaMicro(IPlugInstanceInfo instanceInfo);

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessMidiMsg(IMidiMsg *message) { midiQueue.Add(message); }
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
	void InitParameters();
	void InitGraphics();
	void FlushMidi(int sample);

	IGraphics* pGraphics;
	std::array<double, kNumParameters> parameters;
	IMidiQueue midiQueue;
	double dt = 0.0;
	std::vector<Voice> voices;
};

#endif
