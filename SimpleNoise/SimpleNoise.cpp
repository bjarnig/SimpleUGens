#include "SC_PlugIn.h"
#include <math.h>

// Global pointer used by SC, has to be named ft
static InterfaceTable *ft;

// Use a struct to define the UGen class, inherits from Unit
struct SimpleNoise : public Unit 
{
};


// Functions external to the UGen:
extern "C"
{
	// Definition for the audio calculation function
	void SimpleNoise_next(SimpleNoise *unit, int numSamples);

	// Definition of the constructor
	void SimpleNoise_Ctor(SimpleNoise *unit);
};


// The constructor, with reference to unit or custom struct
void SimpleNoise_Ctor(SimpleNoise* unit)
{
	// Specify which function is used for audio calculation
	SETCALC(SimpleNoise_next);
	
	// Calculate one sample
	SimpleNoise_next(unit, 1);
}


// The audio calculation function 
void SimpleNoise_next(SimpleNoise *unit, int numSamples)
{
	// Pointer to output buffer
	float *out = ZOUT(0);

	// Use LOOP macro to iterate and ZXP to advance pointers
	LOOP(numSamples,
		ZXP(out) = (float) rand()/RAND_MAX * 2 - 1;
	)
}


// Macro used by the host when plugin is loaded, needs naming conventions
PluginLoad(SimpleNoise)
{
	// inTable is defined in the macro, 
	ft = inTable;

    // Register UGen
	DefineSimpleUnit(SimpleNoise);
}
