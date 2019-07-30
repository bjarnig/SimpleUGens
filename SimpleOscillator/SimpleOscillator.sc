SimpleOscillator : PureUGen  {
	*ar { arg freq = 200.0, mul = 1.0, add = 0.0;
		^this.multiNew('audio', freq).madd(mul, add)
	}
	*kr { arg freq = 1, mul = 1.0, add = 0.0;
		^this.multiNew('control', freq).madd(mul, add)
	}
}