SimpleTrigger : PureUGen  {
	*ar { arg trig = 0.0, mul = 1.0, add = 0.0;
		^this.multiNew('audio', trig).madd(mul, add)
	}
	*kr { arg trig = 0.0, mul = 1.0, add = 0.0;
		^this.multiNew('control', trig).madd(mul, add)
	}
}