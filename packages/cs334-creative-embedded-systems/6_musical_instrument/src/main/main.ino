// imports
#include <MIDI.h>
#include <Encoder.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// photosensor stuff
#define NUM_PINS 13
int pins[] = {34, 35, 32, 33, 25, 26, 27, 14, 4, 0, 2, 13, 15};
int readings[13] = { 0 };
void sensorDebug();
void sensorRead();
void sensorSetup();
int lows[13] =  { 0 };
int highs[13] = { 0 };
int thresholds[13] = { 0 };
void sequenceCalibration();

// midi stuff
MIDI_CREATE_DEFAULT_INSTANCE();
int notes[NUM_PINS] = { 0 };
int midi_c1[NUM_PINS] = { 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36 };
int midi_c2[NUM_PINS] = { 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48 };
int midi_c3[NUM_PINS] = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60 };
int midi_c4[NUM_PINS] = { 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72 };
int midi_c5[NUM_PINS] = { 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84 };
int midi_c6[NUM_PINS] = { 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96 };
int midi_c7[NUM_PINS] = { 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108 };
bool beamStates[NUM_PINS] = { false, false, false, false, false, false, false, false, false, false, false, false, false };
const int KEY_NEUTRAL = 0;
const int KEY_OFF = 1;
const int KEY_ON = 2;
int keyStates[NUM_PINS] = { 0 };


// display IO pins and thresholds
const int displayD0 = 19;
const int displayD1 = 18; 
const int displayRES = 5;
const int displayDC = 17; 
const int displayCS = 16;
const int MAX_WIDTH = 128;
const int MIN_WIDTH = 0;
const int MAX_HEIGHT = 64;
const int MIN_HEIGHT = 0;
// Adafruit_SSD1306 disp(128, 64, OLED_SI, OLED_CLK, OLED_DC, OLED_RST, OLED_CS);
Adafruit_SSD1306 disp(MAX_WIDTH, MAX_HEIGHT, displayD1, displayD0, displayDC, displayRES, displayCS);
void displaySetup();
void displayClear();
void displayText(String str);

// rotary encoder stuff
Encoder dial(23, 22);
void dialToOctave();
int currentOctave = -1;
void switchOctave(String octaveStr);
long dialPos;
void dialSetup();
void dialRead();
const int DIAL_NEUTRAL = 0;
const int DIAL_MAX = 100;
const int DIAL_MIN = -100;

// MAIN FUNCTIONS

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Serial.begin(115200);
  sensorSetup();
  displaySetup();
  sequenceCalibration();
}

void loop() {
  dialToOctave();
  sensorRead();
//  sensorDebug(); 
  sendMidi();
}

// HELPER FUNCTIONS

void displaySetup() {
  disp.begin(SSD1306_SWITCHCAPVCC);
  disp.clearDisplay();
  disp.fillScreen(BLACK);
  disp.setTextColor(WHITE);
  disp.setTextSize(1);
  disp.setCursor(0,0);
}

void displayClear() {
  disp.clearDisplay();
  disp.fillScreen(BLACK);
}

void displayText(String str) {
  displayClear();
  disp.println(str);
  disp.display();
}

void sequenceCalibration() {  
  // give notice that calibration is starting
  for (int i = 5; i > 0; i--) {
    disp.setCursor(0,0);
    displayText("Starting calibration. \nCountdown: " + String(i));
    delay(1000);  
  }

  // give notice for low read
  for (int i = 5; i > 0; i--) {
    disp.setCursor(0,0);
    displayText("Reading sensors. \nCountdown: " + String(i));
    delay(1000);  
  }

  // get all the photosensor readings in LOW state
  int value = -1;
  for (int i = 0; i < NUM_PINS; i++) {
    value = analogRead(pins[i]); 
    lows[i] = value;
  }
  
  // have the user turn the lasers on
  for (int i = 10; i > 0; i--) {
    disp.setCursor(0,0);
    displayText("Turn lasers on. \nCountdown: " + String(i));
    delay(1000);  
  }

  // get all the photosensor readings in HIGH state
  value = -1;
  for (int i = 0; i < NUM_PINS; i++) {
    value = analogRead(pins[i]); 
    highs[i] = value;
  }

  // do a calculation to determine thresholds for photosensors and update the array 
  // the threshold is HIGH - % of the HIGH-LOW range
  int range; 
  for (int i = 0; i < NUM_PINS; i++) {
    range = highs[i] + lows[i];
    thresholds[i] = highs[i] - ((range * 35) / 100);
  }

  // show completion notice
  for (int i = 5; i > 0; i--) {
    disp.setCursor(0,0);
    displayText("Calibration complete. \nCountdown: " + String(i));
    delay(1000);  
  }
}

void dialSetup() {
   dial.write(DIAL_NEUTRAL);
}

void dialRead() {
  dialPos = dial.read();
  if (dialPos > DIAL_MAX) {
    dial.write(dialPos);
  } else if (dialPos < DIAL_MIN) {
    dial.write(DIAL_MIN);
  } 
}

void dialToOctave() {
  dialRead();
  if (dialPos >= -100 && dialPos < -72) {
    if (currentOctave == 1) {
      return;
    }
    switchOctave("C1");
    for (int i = 0; i < NUM_PINS; i++) {
      notes[i] = midi_c1[i];
    }
    currentOctave = 1;
  } else if (dialPos >= -72 && dialPos < -44) {
    if (currentOctave == 2) {
      return;
    }
    switchOctave("C2");
    for (int i = 0; i < NUM_PINS; i++) {
      notes[i] = midi_c2[i];
    }
    currentOctave = 2;
  } else if (dialPos >= -44 && dialPos < -16) {
    if (currentOctave == 3) {
      return;
    }
    switchOctave("C3");
    for (int i = 0; i < NUM_PINS; i++) {
      notes[i] = midi_c3[i];
    }
    currentOctave = 3;
  } else if (dialPos >= -16 && dialPos < 12) {
    if (currentOctave == 4) {
      return;
    }
    switchOctave("C4");
    for (int i = 0; i < NUM_PINS; i++) {
      notes[i] = midi_c4[i];
    }
    currentOctave = 4;
  } else if (dialPos >= 12 && dialPos < 40) {
    if (currentOctave == 5) {
      return;
    }
    switchOctave("C5");
    for (int i = 0; i < NUM_PINS; i++) {
      notes[i] = midi_c5[i];
    }
    currentOctave = 5;
  } else if (dialPos >= 40 && dialPos < 68) {
    if (currentOctave == 6) {
      return;
    }
    switchOctave("C6");
    for (int i = 0; i < NUM_PINS; i++) {
      notes[i] = midi_c6[i];
    }
    currentOctave = 6;
  } else if (dialPos >= 68 && dialPos < 100) {
    if (currentOctave == 7) {
      return;
    }
    switchOctave("C7");
    for (int i = 0; i < NUM_PINS; i++) {
      notes[i] = midi_c7[i];
    }
    currentOctave = 7;
  }
}

void switchOctave(String octaveStr) {
  // send all curr octave as OFF to prevent bug
  for (int i = 0; i < NUM_PINS; i++) {
    MIDI.sendNoteOff(notes[i], 127, 1);
  }
  
  // show the change in octave
  disp.setCursor(52, 27);
  disp.setTextSize(2);
  displayText(octaveStr);
}

void sensorSetup() {
  for (int i = 0; i < NUM_PINS; i++) {
    pinMode(pins[i], INPUT);
  }
}

void sensorDebug() {
  Serial.print("[");
  for (int i = 0; i < NUM_PINS; i++) {
    Serial.print(readings[i]);
    Serial.print(", ");
  }
  Serial.println("]");
}

void sensorRead() {
  int value = -1;
  for (int i = 0; i < NUM_PINS; i++) {
    value = analogRead(pins[i]); 
    readings[i] = value;
  }
}

void sendMidi() {
  // for each pin
  for (int i = 0; i < NUM_PINS; i++) {

    // figure out which state it is in

    // in neutral state
    if (keyStates[i] == KEY_NEUTRAL) {
       // finger pressed? 
       if (readings[i] < thresholds[i]) {
           keyStates[i] = KEY_ON;
           MIDI.sendNoteOn(notes[i], 127, 1);
       }
       // else send no messages
    }
    else if (keyStates[i] == KEY_ON) {
        // finger removed?
        if (readings[i] >= thresholds[i]) {
           keyStates[i] = KEY_NEUTRAL;
           MIDI.sendNoteOff(notes[i], 127, 1);
        }
        // else send no messages
    }
  }
  // force a delay to debounce
//  delay(10); 
}
