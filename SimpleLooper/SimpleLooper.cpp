#include "SC_PlugIn.h"
#include <math.h>

// Global pointer used by SC, has to be named ft
static InterfaceTable *ft;

// Use a struct to define the UGen class, inherits from Unit, needs naming convention
struct SimpleLooper : public Unit 
{
	// Index used for reading
	double m_phase;   

	// The buf number
	float m_fbufnum; 

	// A pointer to the buffer
	SndBuf *m_buf;
};


// Functions external to the UGen:
extern "C"
{
	// Definition for the audio calculation function
	void SimpleLooper_next(SimpleLooper *unit, int numSamples);

	// Definition of the constructor
	void SimpleLooper_Ctor(SimpleLooper *unit);
};


// The constructor, with reference to unit or custom struct
void SimpleLooper_Ctor(SimpleLooper* unit)
{
	// Specify which function is used for audio calculation
	SETCALC(SimpleLooper_next);

	// Initial buffer number
	unit->m_fbufnum = -1 ;

	// Set index to zero
	unit->m_phase = 0.0;
}


// The audio calculation function 
void SimpleLooper_next(SimpleLooper *unit, int numSamples)
{
	// Obtain the given rate argument (default is 1)
	float rate = ZIN0(1);

	// Obtain the given endFrams argument (default is -1)
	double endFrames = ZIN0(2);

	// Gets a reference to the buffer and updates the SimpleLooper struct
	GET_BUF_SHARED
	
	// The bufData variable is declared in GET_BUF_SHARED, first check if there is data
	if (!bufData)
	{
		unit->mDone = true; 
		return; 
	}			

	// Pointer to output buffer
	float *out = ZOUT(0); 	

	// If the endFrames is not specified it is set to the number of frames in the buffer
	if(endFrames < 1) 
	{
		endFrames = (double) buf->frames;
	}

	// Get the phase of the buffer
	double phase = unit->m_phase;
	
	LOOP(numSamples,

		// Read buffer, cast phase to 32-bit int and send to the output
		ZXP(out) = bufData[(int32) phase]; 

		// Increment phase index
		phase += rate; 
		
		// Check if the phase index has reached the end of the buffer, if yes reset (loop)
		if (phase >= endFrames) 
		{
			phase = 0;	
		} 
	)
	
	unit->m_phase = phase;
}


// Macro used by the host when plugin is loaded, needs naming conventions
PluginLoad(SimpleLooper)
{
	// inTable is defined in the macro, 
	ft = inTable;

    // Register UGen
	DefineSimpleUnit(SimpleLooper);
}
