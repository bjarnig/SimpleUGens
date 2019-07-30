#include "SC_PlugIn.h"


// Global pointer used by SC, has to be named ft
static InterfaceTable *ft;

// Use a struct to define the UGen class, inherits from Unit
struct SimpleClip : public Unit
{
};


// Functions external to the UGen:
extern "C"
{
	// Definition for the audio calculation function
	void SimpleClip_next(SimpleClip *unit, int numSamples);

	// Definition of the constructor
	void SimpleClip_Ctor(SimpleClip *unit);
};


// Function used internal to the UGen
float clip (float thres, float unit);


// The constructor, with reference to unit or custom struct
void SimpleClip_Ctor(SimpleClip* unit)
{
	// Specify which function is used for audio calculation
	SETCALC(SimpleClip_next);
	
	// Calculate one sample
	SimpleClip_next(unit, 1);
}


// The audio calculation function 
void SimpleClip_next(SimpleClip *unit, int numSamples)
{
	// Pointer to output buffer
	float *out = ZOUT(0);
	
	// Pointer to the input buffer
	float *in = ZIN(0);
	
	// Obtain the threshold argument
	float thres = ZIN0(1);

	// Use LOOP macro to iterate and ZXP to advance pointers
	LOOP(numSamples,
		ZXP(out) = clip(ZXP(in), thres);
	)
}


// Macro used by the host when plugin is loaded, needs naming conventions
PluginLoad(SimpleClip)
{
	// inTable is defined in the macro, 
	ft = inTable;

    // Register UGen
	DefineSimpleUnit(SimpleClip);
}


// Clip the sample value by threshold
float clip (float sample, float thres)
{
	if(sample > thres)
	{
		return thres;
	}
	else if(sample < (thres * -1.0f))
	{
		return thres * -1.0f;
	}

	return sample;
}