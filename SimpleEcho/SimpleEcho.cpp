#include "SC_PlugIn.h"
#include <math.h>

// Global pointer used by SC, has to be named ft
static InterfaceTable *ft;

// Use a struct to define the UGen class, inherits from Unit
struct SimpleEcho : public Unit 
{
	float delaysize, fwdhop, readpos;
	int writepos;
	
	// a pointer to the memory we'll use for our internal delay
	float *delayline;
};


// Functions external to the UGen:
extern "C"
{
	// Definition for the audio calculation function
	void SimpleEcho_next(SimpleEcho *unit, int numSamples);

	// Definition of the constructor
	void SimpleEcho_Ctor(SimpleEcho *unit);

	// Definition of the destructor
	void SimpleEcho_Dtor(SimpleEcho *unit);
};


// The constructor, with reference to unit or custom struct
void SimpleEcho_Ctor(SimpleEcho* unit)
{
	// Set delaysize to 100 ms
	unit->delaysize = SAMPLERATE * 0.1f;
	
	// Initialize the writing at zero
	unit->writepos = 0;

	// Offset the reading position
	unit->readpos = 0;
	
	// Allocate the delay line
	unit->delayline = (float*)RTAlloc(unit->mWorld, unit->delaysize * sizeof(float));

	// Initialise it to zeroes
	memset(unit->delayline, 0, unit->delaysize * sizeof(float));

	// Specify which function is used for audio calculation
	SETCALC(SimpleEcho_next);
	
	// Calculate one sample
	SimpleEcho_next(unit, 1);
}


// The audio calculation function 
void SimpleEcho_next(SimpleEcho *unit, int numSamples)
{
	// Pointer to input buffer
	float *in = ZIN(0);
	
	// Pointer to output buffer
	float *out = ZOUT(0);
	
	// Delay amount 
	float amount = ZIN0(1);
	
	// Get last read pos
	float readpos = unit->readpos;
	
	// A pointer to the the delay line
	float *delayline = unit->delayline;

	// Get the last writing position
	int writepos  = unit->writepos;

	// Get the delay size
	int delaysize = unit->delaysize;

	// Containers for input and delayed signal
	float val, delayed; 

	LOOP(numSamples,

		// Get the input
		val = ZXP(in); // in[i];
		
		// Write to the delay line
		delayline[writepos++] = val;

		// Bounds check the writing position
		if(writepos==delaysize)
			writepos = 0;
		
		// Read from the delay line
		delayed = delayline[(int)readpos++];
	
		// Advance the reading by a slightly higher value		
		readpos += 1.1;
		
		// Check bounds and reset if reached
		if((int)readpos >= delaysize)
			readpos -= delaysize;

		// Output a combination of the delayed signal with the input
		ZXP(out) = val + (delayed * amount);
	)
	
	// Update the pointers
	unit->writepos = writepos;
	unit->readpos = readpos;
}


// The destructor, used to free the memory required for the delay line
void SimpleEcho_Dtor( SimpleEcho *unit ) {
	RTFree(unit->mWorld, unit->delayline);
}	


// Macro used by the host when plugin is loaded, needs naming conventions
PluginLoad(SimpleEcho)
{
	// inTable is defined in the macro, 
	ft = inTable;

    // Register UGen, define it with destructor
	DefineDtorUnit(SimpleEcho);
}
