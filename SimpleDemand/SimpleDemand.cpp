#include "SC_PlugIn.h"
#include <math.h>

// Global pointer used by SC, has to be named ft
static InterfaceTable *ft;

// Use a struct to define the UGen class, inherits from Unit
struct SimpleDemand : public Unit 
{
};


// Functions external to the UGen:
extern "C"
{
	// Definition for the audio calculation function
	void SimpleDemand_next(SimpleDemand *unit, int numSamples);

	// Definition of the constructor
	void SimpleDemand_Ctor(SimpleDemand *unit);
};


// The constructor, with reference to unit or custom struct
void SimpleDemand_Ctor(SimpleDemand* unit)
{
	// Specify which function is used for audio calculation
	SETCALC(SimpleDemand_next);
	
	// Calculate one sample
	SimpleDemand_next(unit, 1);

	OUT0(0) = 0.f;
}


// The calculation function 
void SimpleDemand_next(SimpleDemand *unit, int numSamples)
{
 	// Get the range
	float range = ZIN0(0);

	// Multiply the range with a random number from 0 - 1
	OUT0(0) = (float) rand()/RAND_MAX * range;
}


// Macro used by the host when plugin is loaded, needs naming conventions
PluginLoad(Demand)
{
	// inTable is defined in the macro, 
	ft = inTable;

    // Register UGen
	DefineSimpleUnit(SimpleDemand);
}
