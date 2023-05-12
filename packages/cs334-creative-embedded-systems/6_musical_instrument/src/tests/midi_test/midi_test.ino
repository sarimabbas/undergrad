#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

void setup()
{
   MIDI.begin(MIDI_CHANNEL_OMNI);  // Listen to all incoming messages
   Serial.begin(115200);
}

void loop()
{
   // Send note 42 with velocity 127 on channel 1
   MIDI.sendNoteOn(42, 127, 1);

   // Read incoming messages
   MIDI.read();

   delay(1000);
}
