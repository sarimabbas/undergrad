import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import oscP5.*; 
import netP5.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class visualization extends PApplet {




OscP5 oscP5;

String val;

AlexiViz alexiViz;
SarimViz sarimViz;
DanielViz danielViz;
SabsViz sabsViz;

int alexiData = 0;
int danielData = 0;
int sarimData = 0;
int[] samData = {0, 0, 0};
int[] sabrinaData = {0, 0};

public void setup() {
  

  oscP5 = new OscP5(this, 44444);   //listening

  alexiViz = new AlexiViz();
  sarimViz = new SarimViz();
  danielViz = new DanielViz();
  sabsViz = new SabsViz();
}

int frame = 0;
int framesPer = 300 * 1;
public void draw() {

    sabsViz.draw(alexiData, danielData, sarimData, samData, sabrinaData);


  // here we can switch between each of our classes
  // if (frame < framesPer) {
  //   println("alexi");
  //   alexiViz.draw(alexiData, danielData, sarimData, samData, sabrinaData);
  // } else if (frame < framesPer * 2) {
  //   println("sarim");
  //   sarimViz.draw(alexiData, danielData, sarimData, samData, sabrinaData);
  // } else if (frame < framesPer * 3) {
  //   println("daniel");
  //   // daniel
  // } else if (frame < framesPer * 4) {
  //   println("sabrina");
  //   sabsViz.draw(alexiData, danielData, sarimData, samData, sabrinaData);
  // } else {
  //   frame = 0;
  // }

  // frame++;
  println(frame);

}

/* incoming osc message are forwarded to the oscEvent method. */
public void oscEvent(OscMessage theOscMessage) {
  val = theOscMessage.get(0).stringValue();

  String[] parsed = val.split("--");
  try {
    switch(parsed[0]) {
      case "sarim": {
        sarimData = PApplet.parseInt(parsed[1]);
        break;
      }

      case "daniel": {
        danielData = PApplet.parseInt(parsed[1]);
        break;
      }

      case "alexi": {
        alexiData = PApplet.parseInt(parsed[1]);
        break;
      }

      case "sabrina": {
        sabrinaData[0] = PApplet.parseInt(parsed[1]);
        sabrinaData[1] = PApplet.parseInt(parsed[2]);
        break;
      }

      case "sam": {
        samData[0] = PApplet.parseInt(parsed[1]);
        samData[1] = PApplet.parseInt(parsed[2]);
        samData[2] = PApplet.parseInt(parsed[3]);
        break;
      }

      default:
        break;
    }
  } catch(Exception e) {
    println("ERROR: unexpected message");
  }
}
class AlexiViz {
    AlexiViz() {}

    int numElements = 5;

    int elementColors[][] = {
        { 0xff5562D3, 0xff717AD3, 0xff959AD3, 0xff95C3D3, 0xff30A8D3 }, // sam - water
        { 0xff86A4AF, 0xff94C4D3, 0xff9AA6AF, 0xffCEDFEB, 0xff94A1A9 }, // daniel - air
        { 0xff483123, 0xffFFDD80, 0xffAD7C54, 0xff201710, 0xffFF8080 }, // sarim - lava
        { 0xff9C4749, 0xffD24749, 0xffD29B47, 0xffAF8339, 0xffAF3464 }, // sabrina - fire
        { 0xff30D392, 0xff186948, 0xff4B9322, 0xff496D34, 0xff304823 }, // alexi - earth
    };

    int prevData[][] = {
        {0, 0, 0}, // sam
        {0, 0, 0}, // daniel
        {0, 0, 0}, // sarim
        {0, 0, 0}, // sabrina
        {0, 0, 0}  // alexi
    };

    float summation[] = {
        0, // sam
        0, // daniel
        0, // sarim
        0, // sabrina
        0  // alexi
    };

    public int[][] constructNewData(int[] samData, int danielData, int sarimData, int[] sabrinaData, int alexiData) {
        summation[0] += samData[0] / 1500.0f + samData[1] / 1500.0f + samData[2] / 1500.0f;
        summation[1] += danielData / 4096.0f;
        summation[2] += sarimData;
        summation[3] += sabrinaData[0] / 4096.0f + sabrinaData[1] / 4096.0f;
        summation[4] += alexiData / 4096.0f;

        int newData[][] = {
            {samData[0], samData[1], samData[2]},   // sam
            {danielData, 0, 0},                     // daniel
            {sarimData, 0, 0},                      // sarim
            {sabrinaData[0], sabrinaData[1], 0},    // sabrina
            {alexiData, 0, 0}                       // alexi
        };
        return newData;
    }

    public boolean dataChanged(int[] prev, int[] curr) {
        for (int i = 0; i < 3; i++) {
            if (prev[i] != curr[i]) {
                return true;
            }
        }
        return false;
    }


    public void drawChart(int[][] data) {
        int i = 0;
        for (float rad = 0; rad < TWO_PI; rad += TWO_PI / 5.0f) {
            fill(elementColors[i][0]);
            noStroke();
            
            arc(width / 2, height / 2, width / 2, width / 2, rad, rad + TWO_PI / 5.0f, PIE);
            
            fill(255, 255, 255);
            text(summation[i], width / 2 + width / 8 * cos(rad + TWO_PI / 10.0f), height / 2 + width / 8 * sin(rad + TWO_PI / 10.0f));
            
            i++;
        }

    }


    public void draw(int alexiData, int danielData, int sarimData, int[] samData, int[] sabrinaData) {
        int newData[][] = this.constructNewData(samData, danielData, sarimData, sabrinaData, alexiData);
        for (int i = 0; i < numElements; i++) {
            // this.drawChart(newData);
            if (this.dataChanged(this.prevData[i], newData[i])) {
                // print("changed", i);
                this.drawChart(newData);
            } else {
                // print("not changed", i);
            }
        }

    }
}
class DanielViz {
    DanielViz() {
        // load images
    }

    public void draw(int alexiData, int danielData, int sarimData, int[] samData, int[] sabrinaData) {

    }

}
String[] mytext;
PImage img;
float xDiff = 30;
float yDiff = 30;
int numLines = 26;

float danielRotation = 1;
boolean sarimBool = false;
int sarimCount = 0;

class SabsViz {
  SabsViz() {
    mytext = loadStrings("sabs_media/sabs.txt");
    textSize(20);

    img = loadImage("sabs_media/lava.jpg");
    tint(255, 0); 
    img.resize(width, height);
  }

  public void draw(int alexiData, int danielData, int sarimData, int[] samData, int[] sabrinaData) {
    
    // SABRINA ------------------------------------------------------

    if (sabrinaData[1] == 0) {
      background(0);
    } else if (sabrinaData[1] < 3) {
      background(0xfff8f1a5);
    } else if (sabrinaData[1] < 6) {
      background(0xfff88e3a);
    } else if (sabrinaData[1] < 10) {
      background(0xfff86336);
    } else if (sabrinaData[1] >= 10) {
      background(0xfff80600);
    } 

    // SARIM ------------------------------------------------------
    if (sarimData == 1 || sarimBool == true) {
      image(img, width/2, height/2);
      tint(255, 180);
      sarimBool = true;
      sarimCount += 1;
      if (sarimCount > 6) sarimBool = false;
    } else {
      image(img, width/2, height/2);
      tint(255, 0);
    }

    // DANIEL and ALEXI ------------------------------------------------------
    if (danielData == 1) {
      danielRotation += .01f;
    }

    pushMatrix();
    translate(width/4, height/2); 
    rotate(danielRotation * radians(45));
    fill(0xfff8964b); // orange
    if (alexiData == 4095) ellipse(0, 0, 40, 40);
    else rect(0, 0, 40, 40);
    popMatrix();

    pushMatrix();
    translate(width/2, height/4); 
    rotate(2 * danielRotation * radians(45));
    fill(0xff63aaf8); // blue
    if (alexiData == 4095) ellipse(0, 0, 70, 70);
    else rect(0, 0, 70, 70);
    popMatrix();

    pushMatrix();
    translate(width/2, 3*height/4); 
    rotate(.5f * danielRotation * radians(45));
    fill(0xffa6f87c); // green
    if (alexiData == 4095) ellipse(0, 0, 100, 100);
    else rect(0, 0, 100, 100);
    popMatrix();

    pushMatrix();
    translate(3*width/4, height/2); 
    rotate(4 * danielRotation * radians(45));
    fill(0xffe5b1f8); // purple
    if (alexiData == 4095) ellipse(0, 0, 10, 10);
    else rect(0, 0, 10, 10);
    popMatrix();

    // SAM ------------------------------------------------------

    float x = 0;
    float y = height;
    pushMatrix();
    translate(x, y);
    rotate(-HALF_PI);
    fill(0xfff8eb48, 150); // yellow

    xDiff = map(samData[0], 0, 600, 30, 70);
    yDiff = map(samData[1], 0, 600, 20, 40);

    for (int i = 0; i < numLines / 2; i++) {
      text(mytext[i], 10 + i*yDiff, 10 + i*xDiff);
    }
    for (int i = numLines / 2; i < 3* numLines / 4; i++) {
      text(mytext[i], height - 1.2f*i*yDiff, 10 + i*xDiff);
    }
    for (int i = 3* numLines / 4; i < numLines; i++) {
      text(mytext[i], 10 + i*yDiff/2, 10 + i*xDiff);
    }
    popMatrix();
    
  }
}
class SarimViz {

    SarimViz() {}

    /* ------------------------------- Properties ------------------------------- */

    int numElements = 5;

    int elementColors[][] = {
        {0xff5562D3, 0xff717AD3, 0xff959AD3, 0xff95C3D3, 0xff30A8D3}, // sam - water 
        {0xff86A4AF, 0xff94C4D3, 0xff9AA6AF, 0xffCEDFEB, 0xff94A1A9}, // daniel - air 
        {0xff483123, 0xffFFDD80, 0xffAD7C54, 0xff201710, 0xffFF8080}, // sarim - lava
        {0xff9C4749, 0xffD24749, 0xffD29B47, 0xffAF8339, 0xffAF3464}, // sabrina - fire
        {0xff30D392, 0xff186948, 0xff4B9322, 0xff496D34, 0xff304823}, // alexi - earth
    };

    int prevData[][] = {
        {0, 0, 0}, // sam
        {0, 0, 0}, // daniel
        {0, 0, 0}, // sarim
        {0, 0, 0}, // sabrina
        {0, 0, 0}  // alexi
    };

    int elapsedFrames = 0;

    /* --------------------------- Main draw method --------------------------- */
    
    public void draw(int alexiData, 
              int danielData, 
              int sarimData, 
              int[] samData, 
              int[] sabrinaData) {

        // generate gradient if elapsed frames == 0
        if (this.elapsedFrames > 10) {
            this.setGradient(
                0, 0, 
                width, 
                height, 
                this.elementColors[PApplet.parseInt(random(5))][PApplet.parseInt(random(5))], 
                this.elementColors[PApplet.parseInt(random(5))][PApplet.parseInt(random(5))]
            );
            this.elapsedFrames = 0;
        } 
        this.elapsedFrames++;
        
        // if mesh not working, random inputs
        // int newData[][] = this.generateRandomData();
      
        // get new data
        int newData[][] = this.constructNewData(samData, danielData, sarimData, sabrinaData, alexiData);
  
        // draw element circles based on changed readings
        for (int i = 0; i < numElements; i++) {
            if (this.dataChanged(this.prevData[i], newData[i])) {
                print("changed", i);
                this.drawCircle(this.elementColors[i]);
            } else {
                print("not changed", i);
            }
        }
       
        // mandate delay
        // delay(2000);
    }

    /* ----------------------------- Helper methods ----------------------------- */

    public int[][] constructNewData(int[] samData, 
                         int danielData,
                         int sarimData,
                         int[] sabrinaData,
                         int alexiData) {
        int newData[][] = {
            {samData[0], samData[1], samData[2]},   // sam
            {danielData, 0, 0},                     // daniel
            {sarimData, 0, 0},                      // sarim
            {sabrinaData[0], sabrinaData[1], 0},    // sabrina
            {alexiData, 0, 0}                       // alexi
        };
        return newData;
    }

    public int[][] generateRandomData() {
        int[] samData = {PApplet.parseInt(random(5)), PApplet.parseInt(random(5)), PApplet.parseInt(random(5))};
        int danielData = PApplet.parseInt(random(5));
        int sarimData = PApplet.parseInt(random(5));
        int[] sabrinaData = {PApplet.parseInt(random(5)), PApplet.parseInt(random(5))};
        int alexiData = PApplet.parseInt(random(5));
        int randomData[][] = this.constructNewData(samData, danielData, sarimData, sabrinaData, alexiData);
        return randomData;
    }

    public boolean dataChanged(int[] prev, int[] curr) {
        for (int i = 0; i < 3; i++) {
            if (prev[i] != curr[i]) {
                return true;
            }
        }
        return false;
    }


    public void drawCircle(int[] elementColor) {
        float size = random (80, 300);
        float x = random(width);
        float y = random(height);
        for (float ring = size; ring >= 0; ring -= random(2, 10)) {
            int randomColorIndex = PApplet.parseInt(random(5));
            fill(elementColor[randomColorIndex]);
            ellipse(x, y, ring, ring);
        }
    }

    public void setGradient(int x, int y, float w, float h, int c1, int c2) {
        noFill();
        for (int i = y; i <= y+h; i++) {
            float inter = map(i, y, y+h, 0, 1);
            int c = lerpColor(c1, c2, inter);
            stroke(c);
            line(x, i, x+w, i);
        }
    }
}

/* -------------------------------- Test code ------------------------------- */

// SarimViz sv;

// void setup() {
//   size(720, 480); 
//   background(64);
//   noStroke();
//   sv = new SarimViz();
// }

// void draw() {
//   int[][]rd = sv.generateRandomData();
//   int alexiData = rd[4][0];
//   int danielData = rd[1][0];
//   int[] samData = rd[0];
//   int sarimData = rd[2][0];
//   int[] sabrinaData = rd[3];
//   sv.draw(alexiData, danielData, samData, sarimData, sabrinaData);
// }
  public void settings() {  size(720, 480); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "--present", "--window-color=#666666", "--stop-color=#cccccc", "visualization" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
