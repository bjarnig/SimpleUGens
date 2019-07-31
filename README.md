## SimpleUGens

Sources for 10 UGens for SuperCollider covering various bases such as:
- Waveforms
- Noises
- Buffers
- Iterative functions
- Processing
- Delay lines
- FFT 
- Demand rates

The sources can be compiled with cmake:

```javascript

// To install home-brew:
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)”

// Install cmake with homebrew
brew install cmake

// The source code for SuperCollider should be downloaded.
http://sourceforge.net/projects/supercollider/files/Source/

// Run Cmake with link to the source
cmake -DSC_PATH=/Users/bjarni/Dev/SuperCollider/supercollider-3.9 -DCMAKE_OSX_ARCHITECTURES='i386;x86_64'

```

Once compiled, copy the .scx and .sc files to the extensions folder of SuperCollider. Example of usage:

```javascript

////////////////////////////////////////////////////////////////
//
//                   (Simple) Custom UGens
//
////////////////////////////////////////////////////////////////

/* 01 - SimpleNoise */

(

{ SimpleNoise.ar(0.1) ! 2 }.play

)


/* 02 - SimpleClip */

b = Buffer.read(s,"/Users/bjarni/source/vocal.wav");

(

{ SimpleClip.ar( PlayBuf.ar(2, b.bufnum), 0.001, 15) ! 2 }.play

)


/* 03 - SimpleOscillator */

(

{ SimpleOscillator.ar(50, 0.1) ! 2 }.play

)


////////////////////////////////////////////////////////////////
//
//              Spectral, Chaos  & Chopping
//
////////////////////////////////////////////////////////////////


b = Buffer.read(s, "/Volumes/DATA/_snd/vocalm.wav", bufnum: 0);


/* 01 - SimpleChaos (quadratic map) */

(

{
	SimpleChaos.ar(200, 0.9, -1.4, xi:0.12) ! 2  * 0.5
}.play;

)

/* 02 - SimpleSpectral (thins out bins) */

(

var in, chain, frames = 2048, thin;
{

in = PlayBuf.ar(1, b.bufnum, BufRateScale.kr(b.bufnum), loop: 1);
chain = FFT(LocalBuf(frames), in);
chain = PV_MagAbove(chain, MouseX.kr(0, 10));
chain = SimpleSpectral(chain, 0.1) * 5;
Out.ar(out, 0.1 * IFFT(chain).dup);

// manipulation
thin = SimpleSpectral(chain, 0.1) * 5;
(IFFT(thin) ! 2) * 2;
}.play

)

/* 03 - Divide (chopping) */

(

{ SimpleDivider.ar(1, b.bufnum, losize: 0.1, hisize: 0.2) }.play

)

////////////////////////////////////////////////////////////////
//
//                        Buffers & Delays
//
////////////////////////////////////////////////////////////////


/* 01 - SimpleLooper */

b = Buffer.read(s, "/Volumes/DATA/Courses/Resources/SuperCollider/_snd/vocalm.wav");
b.numFrames

{ SimpleLooper.ar(b.bufnum, 1, 10000) * 0.5 }.play



/* 02 - SimpleTrigger */

(

{
SinOsc.ar( SimpleTrigger.kr(Impulse.kr(5)).range(100,500).poll) * 0.1
}.play;

)


/* 03 - SimpleDemand */

(

{
    var dem, snd;
	dem = SimpleDemand(100);
    snd = Demand.kr( Impulse.kr(10), 0, dem).poll;
	Saw.ar(snd, mul:0.1)
}.play;

)


/* 04 - SimpleEcho */

(

{
SimpleEcho.ar(Impulse.ar(10)) * 0.5
}.play

)
```
