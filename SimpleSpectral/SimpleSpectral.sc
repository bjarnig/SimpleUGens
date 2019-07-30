SimpleSpectral : PV_ChainUGen 
{
	*new { arg buffer, prob = 0.5;
		^this.multiNew('control', buffer, prob)
	}
}