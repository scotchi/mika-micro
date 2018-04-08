#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "Oscillator.h"
#include "Parameters.h"
#include "PresetMenu.h"
#include <random>
#include <vector>
#include "Voice.h"

class MikaMicro : public IPlug
{
public:
	MikaMicro(IPlugInstanceInfo instanceInfo);

	void Reset();
	void GrayOutControls();
	void OnParamChange(int paramIdx);
	void ProcessMidiMsg(IMidiMsg *message) { midiQueue.Add(message); }
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
	void InitParameters();
	void InitPresets();
	void InitGraphics();
	void FlushMidi(int sample);
	double GetDriftValue();
	double GetVoices();

	IGraphics* pGraphics;
	std::array<double, kNumParameters> parameters;
	IMidiQueue midiQueue;
	std::vector<int> heldNotes;
	double dt = 0.0;
	Oscillator lfo;
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_real_distribution<> dist;
	double driftVelocity = 0.0;
	double driftPhase = 0.0;
	std::array<Voice, 8> voices{
		Voice(parameters),
		Voice(parameters),
		Voice(parameters),
		Voice(parameters),
		Voice(parameters),
		Voice(parameters),
		Voice(parameters), 
		Voice(parameters) };
};

#endif
