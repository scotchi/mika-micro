#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include "IPlug_include_in_plug_hdr.h"
#include "Envelope.h"
#include "Oscillator.h"

class MikaMicro : public IPlug
{
public:
	MikaMicro(IPlugInstanceInfo instanceInfo);

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
	void InitGraphics();

	Envelope env;
	EnvelopeParameters envParams{ 1.0, 1.0, .5, 1.0 };
	Oscillator osc;
	double dt = 0.0;
};

#endif
