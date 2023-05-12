#include <MIDI.h>

// constants
#define NUM_BEAMS 13
#define BEAM_HIGH 4095
#define BEAM_ACTIVATION_THRESHOLD 3500

// init MIDI 
MIDI_CREATE_DEFAULT_INSTANCE();

// prototypes
void readPins();
void printBeamStates();

// laser diode beam tracking 
int pins[NUM_BEAMS] = { 34, 35, 32, 33, 25, 26, 27, 14, 12, 13, 4, 0, 2 };
bool beamStates[NUM_BEAMS] = { false, false, false, false, false, false, false, false, false, false, false, false, false };
int notes[NUM_BEAMS] = { 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72 };

void setup() { 
  // start midi over serial
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200);

  // init all beams
  for(int i = 0; i < NUM_BEAMS; i++) {
    pinMode(pins[i], INPUT);
  }
}

void loop() {
  readPins();
  delay(80);
}

void readPins() {
  int value = BEAM_HIGH;
  for (int i = 0; i < NUM_BEAMS; i++) {
    value = analogRead(pins[i]);
    if (value > BEAM_ACTIVATION_THRESHOLD) {
        // only send note if not already sent
        if (beamStates[i] != true) {
          MIDI.sendNoteOn(notes[i], 127, 1);
        }
        beamStates[i] = true;
    } else {
        beamStates[i] = false;
        MIDI.sendNoteOff(notes[i], 127, 1);
    }
  }
}

void printBeamStates() {
  Serial.print("[");
  for (int i = 0; i < NUM_BEAMS; i++) {
    Serial.print(beamStates[i]);
    Serial.print(", ");
  }
  Serial.println("]");
}
