s.boot;

NetAddr.langPort;
NetAddr.localAddr;

// Open port and create function for receiving OSC messages
n = NetAddr.new("127.0.0.1", 44444);
o = OSCFunc({arg msg, time, addr, recvPort; [msg, time, addr, recvPort].postln; }, '/goodbye', n, 44444);

o.free;

// Function to read and parse osc messages. Check sender of message and then sets output of the synth
(
f = {|msg, time, addr, recvPort|
	if(msg[0] != '/status.reply') {
		~data = msg[1].asString.split($-).postln;

		if (~data[0] == "sam", {
			if ((~data[1].asInteger + ~data[2].asInteger + ~data[3].asInteger) > 1000, {
				a = Array.with(~data[1].asInteger, ~data[2].asInteger, ~data[3].asInteger);
				if ((a[0] > a[1]) && (a[0] > a[2]), {
				~sound.set(\ampEarth, 0.7, \ampWater, 0.15, \ampFire, 0.15)
				});
				if ((a[1] > a[0]) && (a[1] > a[2]), {
				~sound.set(\ampEarth, 0.15, \ampWater, 0.7, \ampFire, 0.15)
				});
				if ((a[2] > a[1]) && (a[2] > a[0]), {
				~sound.set(\ampEarth, 0.15, \ampWater, 0.15, \ampFire, 0.7)
				});
			})
		})

		if (~data[0] == "sabrina", {
			~sound.set(\lpCutoff, ~data[1].asInteger.linexp(0, 4095, 800, 20000))
		});

		if (~data[0] == "alexi", {
			~sound.set(\hpCutoff, ~data[1].asInteger.linexp(0, 40, 20, 800))
		});
	}
};
thisProcess.addOSCRecvFunc(f);
);

thisProcess.removeOSCRecvFunc(f);

// Read sound files
(
b = Buffer.readDir(("akwSounds/").resolveRelative, 0, server: s);
)

// Synth Def for sound file playback
SynthDef(\player, {|bufEarth, bufFire, bufWater, ampEarth=0, ampFire=0, ampWater=0, hpCutoff=30, lpCutoff=18000, start=0, rate=1, loop=1|
	var sigEarth = PlayBuf.ar(2, bufEarth, rate, 1, start, loop);
	var sigFire = PlayBuf.ar(2, bufFire, rate, 1, start, loop);
	var sigWater = PlayBuf.ar(2, bufWater, rate, 1, start, loop);
	var sigElement = Mix([sigEarth * ampEarth, sigFire * ampFire, sigWater * ampWater]);
	var sigElement = RHPF.ar(sigElement, hpCutoff, 0.5);
	var sigElement = RLPF.ar(sigElement, lpCutoff, 0.5);
	Out.ar(0, sigElement);
}, [0, 0, 0, 2, 2, 2, 2, 2] // These add some lag time when arguments are set
).add;

// Instantiate Synth
~sound = Synth(\player, [\bufEarth, b[0], \bufFire, b[1], \bufWater, b[2]]);
