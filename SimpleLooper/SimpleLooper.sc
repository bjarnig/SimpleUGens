SimpleLooper : PureUGen  {
	*ar { arg bufnum=0, rate=1.0, endFrames=0, mul = 1.0, add = 0.0;
		^this.multiNew('audio',  bufnum, rate, endFrames).madd(mul, add);
	}
}