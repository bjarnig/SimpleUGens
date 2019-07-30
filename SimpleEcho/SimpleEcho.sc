SimpleEcho : PureUGen  {
	*ar { arg in = 0.0, amount=0.5, mul = 1.0, add = 0.0;
		^this.multiNew('audio', in, amount).madd(mul, add)
	}
	*kr { arg in = 0.0, amount=0.5, mul = 1.0, add = 0.0;
		^this.multiNew('control', in, amount).madd(mul, add)
	}
}