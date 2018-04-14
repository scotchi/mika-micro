#include "MikaMicro.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

MikaMicro::MikaMicro(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(0, 128, instanceInfo)
{
	TRACE;

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

	IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 60);

	AttachGraphics(pGraphics);

	MakeDefaultPreset("-", 128);

	osc.SetWaveform(kSaw);
	osc.SetFrequency(880.0);
	env.SetAttack(2.0);
	env.SetDecay(2.0);
	env.SetSustain(.5);
	env.SetRelease(2.0);
	env.stage = kAttack;
}

MikaMicro::~MikaMicro() {}

void MikaMicro::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		env.Update();
		auto out = osc.Next() * env.Get();
		outputs[0][s] = outputs[1][s] = out * .25;
	}
}

void MikaMicro::Reset()
{
	TRACE;
	IMutexLock lock(this);
	osc.SetSampleRate(GetSampleRate());
	env.SetSampleRate(GetSampleRate());
}

void MikaMicro::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);
}
