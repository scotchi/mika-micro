#ifndef __MIKAMICRO__
#define __MIKAMICRO__

#include <array>
#include "IPlug_include_in_plug_hdr.h"
#include "Parameters.h"
#include "Voice.h"

class MikaMicro : public IPlug
{
public:
	MikaMicro(IPlugInstanceInfo instanceInfo);

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
	void InitParameters();
	void InitGraphics();

	std::array<double, kNumParameters> parameters;
	double dt = 0.0;
	Voice voice;
};

#endif
