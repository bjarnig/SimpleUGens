SimpleChaos : PureUGen  {
	*ar { arg freq=22050, a=1, b= -1, c= -0.75, xi=0, mul=1, add=0;
		^this.multiNew('audio', freq, a, b, c, xi).madd(mul, add)
	}
}