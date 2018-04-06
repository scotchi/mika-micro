#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void MikaMicro::InitParameters()
{
	GetParam(kVolEnvA)->InitDouble("Volume envelope attack", 1.0, 0.1, 1000.0, .01);
	GetParam(kVolEnvD)->InitDouble("Volume envelope decay", 1.0, 0.1, 1000.0, .01);
	GetParam(kVolEnvS)->InitDouble("Volume envelope sustain", 0.5, 0.0, 1.0, .01);
	GetParam(kVolEnvR)->InitDouble("Volume envelope release", 1.0, 0.1, 1000.0, .01);
}

void MikaMicro::InitGraphics()
{
	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

	IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 60);

	AttachGraphics(pGraphics);
}

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParameters, 1, instanceInfo)
{
	TRACE;

	InitParameters();
	InitGraphics();
	for (int i = 0; i < 8; i++) voices.push_back(Voice(parameters));
	MakeDefaultPreset("-", 1);
}

void MikaMicro::FlushMidi(int sample)
{
	while (!midiQueue.Empty())
	{
		auto *message = midiQueue.Peek();
		if (message->mOffset > sample) break;

		if (message->StatusMsg() == IMidiMsg::kNoteOff || (message->StatusMsg() == IMidiMsg::kNoteOn && message->Velocity() == 0))
		{
			for (auto &voice : voices)
				if (voice.GetNote() == message->NoteNumber()) voice.Release();
		}
		else if (message->StatusMsg() == IMidiMsg::kNoteOn)
		{
			auto voice = std::min_element(std::begin(voices), std::end(voices), [](Voice a, Voice b) {
				return a.IsReleased() == b.IsReleased() ? a.GetVolume() < b.GetVolume() : a.IsReleased();
			});
			voice->SetNote(message->NoteNumber());
			voice->Start();
		}

		midiQueue.Remove();
	}
}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		FlushMidi(s);
		auto out = 0.0;
		for (auto &voice : voices) out += voice.Get(dt) * .25;
		outputs[0][s] = outputs[1][s] = out;
	}
}

void MikaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);
	dt = 1.0 / GetSampleRate();
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	parameters[paramIdx] = GetParam(paramIdx)->Value();
}
