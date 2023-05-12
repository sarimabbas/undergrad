#include <Encoder.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


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
void sequenceCalibration();
void dialToOctave();
void switchOctave(String octaveStr);

// Encoder stuff
Encoder dial(23, 22);
long dialPos;
void dialSetup();
void dialRead();
const int DIAL_NEUTRAL = 0;
const int DIAL_MAX = 100;
const int DIAL_MIN = -100;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  // display initialization
  
  displaySetup();

  delay(100);

//  sequenceCalibration();
}

void loop() {
  // put your main code here, to run repeatedly:
  dialToOctave();
}


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
    displayText("Starting calibration. \nCountdown: " + String(i));
    delay(1000);  
  }

  // get all the photosensor readings in LOW state

  // have the user turn the lasers on
  for (int i = 10; i > 0; i--) {
    displayText("Turn lasers on. \nCountdown: " + String(i));
    delay(1000);  
  }

  // get all the photosensor readings in HIGH state

  // do a calculation to determine thresholds for photosensors and update the array 

  // show completion notice
  for (int i = 5; i > 0; i--) {
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
  } else {
    Serial.println(dialPos);
  }
}

void dialToOctave() {
  dialRead();
  if (dialPos >= -100 && dialPos < -72) {
    switchOctave("C1");
  } else if (dialPos >= -72 && dialPos < -44) {
    switchOctave("C2");
  } else if (dialPos >= -44 && dialPos < -16) {
    switchOctave("C3");
  } else if (dialPos >= -16 && dialPos < 12) {
    switchOctave("C4");
  } else if (dialPos >= 12 && dialPos < 40) {
    switchOctave("C5");
  } else if (dialPos >= 40 && dialPos < 68) {
    switchOctave("C6");
  } else if (dialPos >= 68 && dialPos < 100) {
    switchOctave("C7");
  }
}

void switchOctave(String octaveStr) {
  // get reading from rotary encoder

  // based on that update the notes array 

  // show the change in octave
  disp.setCursor(52, 27);
  disp.setTextSize(2);
  displayText(octaveStr);
  
}
