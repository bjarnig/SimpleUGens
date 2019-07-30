#include "SC_PlugIn.h"
#include <math.h>

// Global pointer used by SC, has to be named ft
static InterfaceTable *ft;

// Use a struct to define the UGen class, inherits from Unit
struct SimpleOscillator : public Unit
{
	float lastSample;
};


// Functions external to the UGen:
extern "C"
{
	// Definition for the audio calculation function
	void SimpleOscillator_next(SimpleOscillator *unit, int numSamples);

	// Definition of the constructor
	void SimpleOscillator_Ctor(SimpleOscillator *unit);
};


// The constructor, with reference to unit or custom struct
void SimpleOscillator_Ctor(SimpleOscillator* unit)
{
	// Set the first sample
	unit->lastSample = 0.0;

	// Specify which function is used for audio calculation
	SETCALC(SimpleOscillator_next);
	
	// Calculate one sample
	SimpleOscillator_next(unit, 1);
}


// The audio calculation function 
void SimpleOscillator_next(SimpleOscillator *unit, int numSamples)
{
	// Pointer to output buffer
	float *out = ZOUT(0);

	// Obtain the frequency argument
	float freq = ZIN0(0);

	// Calculate the increment
	float increment = ((freq * 2.0) / SAMPLERATE);

	// Use LOOP macro to iterate and ZXP to advance pointers
	LOOP(numSamples,

		// Output sample
		ZXP(out) = unit->lastSample;
		
		// Apply the increment
        unit->lastSample += increment;
        
        // When signal reaches 1.0 it is inverted
        if( unit->lastSample >= 1.0f ) unit->lastSample += -2.0f;
	)
}


// Macro used by the host when plugin is loaded, needs naming conventions
PluginLoad(SimpleOscillator)
{
	// inTable is defined in the macro, 
	ft = inTable;

    // Register UGen
	DefineSimpleUnit(SimpleOscillator);
}
