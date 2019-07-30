#include "FFT_UGens.h"

// Global pointer used by SC, has to be named ft (not static for Spectral UGens)
InterfaceTable *ft;

// Use a struct to define the UGen class, inherits from Unit
struct SimpleSpectral : public Unit 
{
};


// Functions external to the UGen:
extern "C"
{
	// Definition for the audio calculation function
	void SimpleSpectral_next(SimpleSpectral *unit, int numSamples);

	// Definition of the constructor
	void SimpleSpectral_Ctor(SimpleSpectral *unit);
};


// The constructor, with reference to unit or custom struct
void SimpleSpectral_Ctor(SimpleSpectral* unit)
{
	// Specify which function is used for audio calculation
	SETCALC(SimpleSpectral_next);
	
	// Clear outputs
	ZOUT0(0) = ZIN0(0);
}


// The audio calculation function 
void SimpleSpectral_next(SimpleSpectral *unit, int inNumSamples)
{
	// Gets buffer and fills number of bins
	PV_GET_BUF

    // Get random numbers
	RGET
	
	// Get the porbability input
	float prob = ZIN0(1);

	// Convert to complex numbers
	SCComplexBuf *p = ToComplexApx(buf);

	

	// Iterate through available bins
	for (int i = 0; i < numbins; ++i) { 

		// Random number in the range 0-1
		float r = frand(s1, s2, s3); 
		
        // Set some bins to zero
		if (r > prob) { // possibly zero both the real and imaginary parts of the complex number for a bin
			p->bin[i].real = 0.0;
			p->bin[i].imag = 0.0;
		}
	}
	
	// Put the random numbers 
	RPUT
}

// Declarations needed for Spectral UGens
void init_SCComplex(InterfaceTable *inTable);  

#define DefinePVUnit(name) \
	(*ft->fDefineUnit)(#name, sizeof(PV_Unit), (UnitCtorFunc)&name##_Ctor, 0, 0);

PluginLoad(Spectral)
{
	// inTable is defined in the macro, 
	ft = inTable;

	// Init the Spectral UGen
	init_SCComplex(inTable);

	// Register UGen
	DefinePVUnit(SimpleSpectral);
}
 