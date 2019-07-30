#include "SC_PlugIn.h"
#include <math.h>

// Global pointer used by SC, has to be named ft
static InterfaceTable *ft;

// Use a struct to define the UGen class, inherits from Unit
struct SimpleChaos : public Unit 
{
	double x0, y0, xn, yn, xnm1, ynm1;
	float counter;
};


// Functions external to the UGen:
extern "C"
{
	// Definition for the audio calculation function
	void SimpleChaos_next(SimpleChaos *unit, int numSamples);

	// Definition of the constructor
	void SimpleChaos_Ctor(SimpleChaos *unit);
};


// The constructor, with reference to unit or custom struct
void SimpleChaos_Ctor(SimpleChaos* unit)
{
	// Specify which function is used for audio calculation
	SETCALC(SimpleChaos_next);

	// Initialize unit
	unit->x0 = ZIN0(4);
	unit->xn = unit->x0;
	unit->counter = 0.f;

	// Calculate one sample
	SimpleChaos_next(unit, 1);
}


// The audio calculation function 
void SimpleChaos_next(SimpleChaos *unit, int inNumSamples)
{
	// Reference to the output
	float *leftout = ZOUT(0);
	// float *rightout = ZOUT(1);
	// float *leftout1 = ZOUT(2);
	// float *rightout2 = ZOUT(3);

	// Obtain the frequency
	float freq = ZIN0(0);

	// Get the iterative parameters
	double a = ZIN0(1);
	double b = ZIN0(2);
	double c = ZIN0(3);
	double x0 = ZIN0(4);

	double xn = unit->xn;
	float counter = unit->counter;

	// Calculate how many samples per cycle will be produced (based on frequency)
	float samplesPerCycle;

	if(freq < unit->mRate->mSampleRate)
		samplesPerCycle = unit->mRate->mSampleRate / sc_max(freq, 0.001f);
	else samplesPerCycle = 1.f;

	// Update with user input if needed
	if(unit->x0 != x0) {
		unit->x0 = xn = x0;
	}

	// Iterate and calculate only new values when freq cycle is reached
	for (int i=0; i<inNumSamples; ++i) 
	{
		if(counter >= samplesPerCycle)
		{
			counter -= samplesPerCycle;
			xn = a * xn * xn + b * xn + c;
		}
		
		counter++;
		ZXP(leftout)  = xn;
		// ZXP(rightout) = xn * 0.75;
		// ZXP(leftout1)  = xn * 0.5;
		// ZXP(rightout2) = xn * 0.25;
	}

	unit->xn = xn;
	unit->counter = counter;
}


// Macro used by the host when plugin is loaded, needs naming conventions
PluginLoad(SimpleChaos)
{
	// inTable is defined in the macro, 
	ft = inTable;

    // Register UGen
	DefineSimpleUnit(SimpleChaos);
}
