  

import processing.io.*;

// On the Raspberry Pi GPIO 4 is physical pin 7 on the header

void setup() {
  // INPUT_PULLUP enables the built-in pull-up resistor for this pin
  // left alone, the pin will read as HIGH
  // connected to ground (via e.g. a button or switch) it will read LOW
  GPIO.pinMode(4, GPIO.INPUT);
  GPIO.pinMode(17, GPIO.INPUT);
  GPIO.pinMode(27, GPIO.INPUT);
  GPIO.pinMode(22, GPIO.INPUT);
  size(500, 500);
  rectMode(CENTER);
  ellipseMode(CENTER);
}

int wdraw = 0;
int hdraw = 0;

int m = 1;

int x = width / 2;
int y = height / 2;

int rwidth = 100;
int rheight = 100;


void draw() {
  if(GPIO.digitalRead(22) == GPIO.HIGH){
   delay(100);
   if(GPIO.digitalRead(22) == GPIO.HIGH){
      if(m == 1){
       m = -1; 
      }
      else if(m == -1){
       m = 2; 
      }
      else if(m == 2){
       m = 1; 
      }
   }
  }
  
  
  // left
  if (GPIO.digitalRead(4) == GPIO.LOW) {
    // button is pressed
    //print("GPIO 4 LOW");
    wdraw = 5 * m;
  } 
  else{
    wdraw = 0;
  }
  
  // up
  if (GPIO.digitalRead(17) == GPIO.LOW) {
    // button is pressed
    //print("GPIO 17 LOW");
    hdraw = 5 * m;
  } 
  else{
   hdraw = 0; 
  }
  
  x = (x + wdraw) ;
  y = (y + hdraw) ;
  if(x < 0){
    x += width;
  }
  else if(x > width){
   x = x - width; 
  }
  
  if(y < 0){
   y = y + height; 
  }
  else if(y > height){
   y = y - height; 
  }
   
  background(0, 0,0);
  fill(255,255,255);
  if (GPIO.digitalRead(27) == GPIO.LOW) {
    ellipse(x, y, rwidth, rheight);
  } else {
    rect(x,y, rwidth, rheight);
  }
  
  //print("X: " + x);

  print("m :" + m);
}
