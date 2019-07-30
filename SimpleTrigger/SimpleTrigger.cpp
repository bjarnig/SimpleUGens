#include "SC_PlugIn.h"
#include <math.h>

// Global pointer used by SC, has to be named ft
static InterfaceTable *ft;

// Use a struct to define the UGen class, inherits from Unit
struct SimpleTrigger : public Unit 
{
	float previousTrigger, outputValue;
};


// Functions external to the UGen:
extern "C"
{
	// Definition for the audio calculation function
	void SimpleTrigger_next(SimpleTrigger *unit, int numSamples);

	// Definition of the constructor
	void SimpleTrigger_Ctor(SimpleTrigger *unit);
};


// The constructor, with reference to unit or custom struct
void SimpleTrigger_Ctor(SimpleTrigger* unit)
{
	// Specify which function is used for audio calculation
	SETCALC(SimpleTrigger_next);

	// Get the initial trigger
	unit->previousTrigger = ZIN0(0);
	
	// Calculate one sample
	unit->outputValue = 0.0f;
	SimpleTrigger_next(unit, 1);
}


// The audio calculation function 
void SimpleTrigger_next(SimpleTrigger *unit, int numSamples)
{
	// Pointer to output buffer
	float *out = ZOUT(0);

	// Get the trigger value 
	float *trig = ZIN(0);

	// Get the previous trigger value
	float prev = unit->previousTrigger;

	// Get the last output value
	float outval = unit->outputValue;

	// Container for next trigger value
	float next;

	// Use LOOP macro to iterate and ZXP to advance pointers
	LOOP(numSamples,
		
		// Get current trigger value
		next = ZXP(trig);  
		
		// Set a new random number to output and outval if a trigger has occured
		if (next > 0.f &&  prev <= 0.f) 
		{
			ZXP(out) = outval = (float) rand()/RAND_MAX * 2 - 1;
		} 
		else 
		{
			ZXP(out) = outval;
		}

		// Advance the prev variable
		prev = next;
	)

	unit->previousTrigger = next;
	unit->outputValue = outval;
}


// Macro used by the host when plugin is loaded, needs naming conventions
PluginLoad(SimpleTrigger)
{
	// inTable is defined in the macro, 
	ft = inTable;

    // Register UGen
	DefineSimpleUnit(SimpleTrigger);
}
