#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "Envelope.h"
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Oscillator.h"
#include "Parameters.h"
#include "PresetMenu.h"
#include "Util.h"
#include <vector>
#include "Voice.h"

class MikaMicro : public IPlug
{
public:
	MikaMicro(IPlugInstanceInfo instanceInfo);
	~MikaMicro();

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
	void ProcessMidiMsg(IMidiMsg* message) { midiQueue.Add(message); }

private:
	void InitParameters();
	void InitGraphics();
	void InitPresets();
	void FlushMidi(int sample);
	void SmoothOscMix();
	double GetDriftValue();
	void GrayOutControls();

	double dt = 0.0;
	double oscMix = 0.0;
	double driftVelocity = 0.0;
	double driftPhase = 0.0;
	std::vector<int> heldNotes;

	IGraphics* pGraphics;
	IMidiQueue midiQueue;
	Oscillator lfo;
	std::array<Voice, 8> voices;
};

#endif
