SimpleClip : PureUGen  {
	*ar { arg in = 0.0, thres = 0.25, mul = 1.0, add = 0.0;
		^this.multiNew('audio', in, thres).madd(mul, add)
	}
	*kr { arg in = 0.0, thres = 0.25, mul = 1.0, add = 0.0;
		^this.multiNew('control', in, thres).madd(mul, add)
	}
}