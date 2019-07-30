SimpleDivider : MultiOutUGen  {
	*ar { arg numChannels, bufnum=0, rate=1.0, losize = 0.1, hisize = 0.2;
		^this.multiNew('audio', numChannels, bufnum, rate, losize, hisize)
	}
	
	init { arg argNumChannels ... theInputs;
		inputs = theInputs;
		^this.initOutputs(argNumChannels, rate);
	}

	argNamesInputsOffset { ^2 }
}