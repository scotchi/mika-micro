#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "Envelope.h"
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Oscillator.h"
#include "Parameters.h"
#include "Voice.h"

class MikaMicro : public IPlug
{
public:
	MikaMicro(IPlugInstanceInfo instanceInfo);
	~MikaMicro();

	void FlushMidi(int sample);

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
	void ProcessMidiMsg(IMidiMsg* message) { midiQueue.Add(message); }

private:
	void InitParameters();
	void InitGraphics();

	IGraphics* pGraphics;
	IMidiQueue midiQueue;
	Oscillator lfo;
	std::array<Voice, 8> voices;
};

#endif
