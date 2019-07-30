SimpleDemand : DUGen  {
	*new { arg range = 10.0, length = inf;
		^this.multiNew('demand', range);
	}
}