#include "SC_PlugIn.h"
#include <math.h>

// Global pointer used by SC, has to be named ft
static InterfaceTable *ft;

// Use a struct to define the UGen class, inherits from Unit
struct SimpleDivider : public Unit 
{
	double m_phase;
	float m_fbufnum;
	double m_max;
	SndBuf *m_buf;
};


// Functions external to the UGen:
extern "C"
{
	// Definition for the audio calculation function
	void SimpleDivider_next(SimpleDivider *unit, int numSamples);

	// Definition of the constructor
	void SimpleDivider_Ctor(SimpleDivider *unit);
};


// The constructor, with reference to unit or custom struct
void SimpleDivider_Ctor(SimpleDivider* unit)
{
	// Specify which function is used for audio calculation
	SETCALC(SimpleDivider_next);
	
	// Initialize unit values
	unit->m_fbufnum = -1e9f;
	unit->m_phase = 0.0;
	unit->m_max = 0.0;

	// Clear the outputs
	ClearUnitOutputs(unit,1);
}


// Checks for buffer. if not present, cleans and returns (from DelayUGens.cpp)
static inline bool checkBuffer(Unit * unit, const float * bufData, uint32 bufChannels,
							   uint32 expectedChannels, int inNumSamples)
{
	if (!bufData)
		goto handle_failure;

	if (expectedChannels > bufChannels) {
		if(unit->mWorld->mVerbosity > -1 && !unit->mDone)
			Print("Buffer UGen channel mismatch: expected %i, yet buffer has %i channels\n",
				  expectedChannels, bufChannels);
		goto handle_failure;
	}
	return true;

handle_failure:
	unit->mDone = true;
	ClearUnitOutputs(unit, inNumSamples);
	return false;
}


// Handles the looping (from DelayUGens.cpp)
inline double sc_loop(Unit *unit, double in, double hi, int loop)
{
	// avoid the divide if possible
	if (in >= hi) {
		if (!loop) {
			unit->mDone = true;
			return hi;
		}
		in -= hi;
		if (in < hi) return in;
	} else if (in < 0.) {
		if (!loop) {
			unit->mDone = true;
			return 0.;
		}
		in += hi;
		if (in >= 0.) return in;
	} else return in;

	return in - hi * floor(in/hi);
}


// Produces multi-channel output with cubic interpolation (from DelayUGens.cpp)
#define LOOP_BODY_4_DIVIDER(SAMPLE_INDEX) \
		if (phase >  m_max) { \
			phase = frand(s1,s2,s3) * startMax; \
			m_max = frand(s1,s2,s3) * range + losize + phase; \
		} \
		int32 iphase = (int32)phase; \
		const float* table1 = bufData + iphase * bufChannels; \
		const float* table0 = table1 - bufChannels; \
		const float* table2 = table1 + bufChannels; \
		const float* table3 = table2 + bufChannels; \
		if (iphase == 0) { \
			if (loop) { \
				table0 += bufSamples; \
			} else { \
				table0 += bufChannels; \
			} \
		} else if (iphase >= guardFrame) { \
			if (iphase == guardFrame) { \
				if (loop) { \
					table3 -= bufSamples; \
				} else { \
					table3 -= bufChannels; \
				} \
			} else { \
				if (loop) { \
					table2 -= bufSamples; \
					table3 -= bufSamples; \
				} else { \
					table2 -= bufChannels; \
					table3 -= 2 * bufChannels; \
				} \
			} \
		} \
		int32 index = 0; \
		float fracphase = phase - (double)iphase; \
		for (uint32 channel=0; channel<numOutputs; ++channel) { \
			float a = table0[index]; \
			float b = table1[index]; \
			float c = table2[index]; \
			float d = table3[index]; \
			OUT(channel)[SAMPLE_INDEX] = cubicinterp(fracphase, a, b, c, d); \
			index++; \
		}


// The audio calculation function 
void SimpleDivider_next(SimpleDivider *unit, int inNumSamples)
{
	// Playback rate
	float rate = ZIN0(1);

	// Converts seconds to number of samples
	float losize = ZIN0(2) * SAMPLERATE;  
	float hisize = ZIN0(3) * SAMPLERATE;  

	// Bounds check the sizes
	if (losize > hisize)  
	{
		float temp = losize;
		hisize = losize;
		losize = temp;
	}

	// Set the range and hi size
	double range = hisize - losize;
	double m_max = unit->m_max;
	
	// Call get buff shared to fill the unit
	GET_BUF_SHARED

	int32 loop = 1;
	int numOutputs = unit->mNumOutputs;
	if (!checkBuffer(unit, bufData, bufChannels, numOutputs, inNumSamples))
		return;	
	
	// Call RGET to copy random state variables to local ones
	RGET
	
	// startMax : the highest index for the initial start point
	double startMax;

	// Bounds check
	if (hisize < bufFrames)
		startMax = bufFrames - hisize - 1;
	else
	{  // if hisize is bigger than the buffer, make corrections
		startMax = 0;
		hisize = bufFrames - 1;
		range = bufFrames - 1;
		losize = 0;
	}

	// Set working variable to the unit phase
	double phase = unit->m_phase;
	
	// Read a value from each channel in the buffer, interpolate and output
	for (int i = 0; i<inNumSamples; ++i){
		LOOP_BODY_4_DIVIDER(i)		
		phase += rate;
	}
	
	// Set the random state parameters again
	RPUT

	unit->m_phase = phase;
	unit->m_max = m_max;
}


// Used by the host when plugin is loaded, needs naming conventions
PluginLoad(SimpleDivider)
{
	// inTable is defined in the macro, 
	ft = inTable;

    // Register UGen with interpolation
	DefineSimpleCantAliasUnit(SimpleDivider);
}
