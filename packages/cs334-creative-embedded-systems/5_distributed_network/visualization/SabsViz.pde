String[] mytext;
PImage img;
float xDiff = 30;
float yDiff = 30;
int numLines = 26;

float danielRotation = 1;
boolean sarimBool = false;
int sarimCount = 0;
int sabMultiplier = 1;

class SabsViz {
  SabsViz() {
    mytext = loadStrings("sabs_media/sabs.txt");
    textSize(20);

    img = loadImage("sabs_media/lava.jpg");
    tint(255, 0); 
    img.resize(width, height);
  }

  void draw(int alexiData, int danielData, int sarimData, int[] samData, int[] sabrinaData) {

    int[][]rd = sarimViz.generateRandomData();
    alexiData = rd[4][0];
    danielData = rd[1][0];
    samData = rd[0];
    sarimData = rd[2][0];
    sabrinaData = rd[3];

    // SARIM ------------------------------------------------------
    if (sarimData == 1 || sarimBool == true) {
      image(img, 0, 0);
      tint(255, 180);
      sarimBool = true;
      sarimCount += 1;
      if (sarimCount > 6) sarimBool = false;
    } else {
      image(img, 0, 0);
      tint(255, 0);
    }

    // SABRINA ------------------------------------------------------

    if (sabrinaData[1] <= 3) {
      sabMultiplier *= 3;
    } else if (sabrinaData[1] > 3) {
      sabMultiplier /= 3;
    }
    print(sabMultiplier);

    // DANIEL and ALEXI ------------------------------------------------------
    if (danielData == 1) {
      danielRotation += .02;
    }

    pushMatrix();
    translate(width/4, height/2); 
    rotate(danielRotation * radians(45));
    fill(#f8964b); // orange
    if (alexiData % 2 == 0) ellipse(0, 0, 40, 40);
    else rect(0, 0, 40 * sabMultiplier, 40 * sabMultiplier);
    popMatrix();

    pushMatrix();
    translate(width/2, height/4); 
    rotate(2 * danielRotation * radians(45));
    fill(#63aaf8); // blue
    if (alexiData % 2 == 0) ellipse(0, 0, 70, 70);
    else rect(0, 0, 70 * sabMultiplier, 70 * sabMultiplier);
    popMatrix();

    pushMatrix();
    translate(width/2, 3*height/4); 
    rotate(.5 * danielRotation * radians(45));
    fill(#a6f87c); // green
    if (alexiData % 2 == 0) ellipse(0, 0, 100, 100);
    else rect(0, 0, 100, 100);
    popMatrix();

    pushMatrix();
    translate(3*width/4, height/2); 
    rotate(4 * danielRotation * radians(45));
    fill(#e5b1f8); // purple
    if (alexiData % 2 == 0) ellipse(0, 0, 10, 10);
    else rect(0, 0, 10, 10);
    popMatrix();

    // SAM ------------------------------------------------------

    float x = 0;
    float y = height;
    pushMatrix();
    translate(x, y);
    rotate(-HALF_PI);
    fill(#f8eb48, 150); // yellow

    xDiff = map(samData[0], 0, 600, 30, 50);
    yDiff = map(samData[1], 0, 600, 20, 40);

    for (int i = 0; i < numLines / 2; i++) {
      text(mytext[i], 10 + i*yDiff, 10 + i*xDiff);
    }
    for (int i = numLines / 2; i < 3* numLines / 4; i++) {
      text(mytext[i], height - 1.2*i*yDiff, 10 + i*xDiff);
    }
    for (int i = 3* numLines / 4; i < numLines; i++) {
      text(mytext[i], 10 + i*yDiff/2, 10 + i*xDiff);
    }
    popMatrix();
    
    delay(50);
  }
}
