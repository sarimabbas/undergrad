class SarimViz {

    SarimViz() {}

    /* ------------------------------- Properties ------------------------------- */

    int numElements = 5;

    color elementColors[][] = {
        {#5562D3, #717AD3, #959AD3, #95C3D3, #30A8D3}, // sam - water 
        {#86A4AF, #94C4D3, #9AA6AF, #CEDFEB, #94A1A9}, // daniel - air 
        {#483123, #FFDD80, #AD7C54, #201710, #FF8080}, // sarim - lava
        {#9C4749, #D24749, #D29B47, #AF8339, #AF3464}, // sabrina - fire
        {#30D392, #186948, #4B9322, #496D34, #304823}, // alexi - earth
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
    
    void draw(int alexiData, 
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
                this.elementColors[int(random(5))][int(random(5))], 
                this.elementColors[int(random(5))][int(random(5))]
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

    int[][] constructNewData(int[] samData, 
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

    int[][] generateRandomData() {
        int[] samData = {int(random(5)), int(random(5)), int(random(5))};
        int danielData = int(random(5));
        int sarimData = int(random(5));
        int[] sabrinaData = {int(random(5)), int(random(5))};
        int alexiData = int(random(5));
        int randomData[][] = this.constructNewData(samData, danielData, sarimData, sabrinaData, alexiData);
        return randomData;
    }

    boolean dataChanged(int[] prev, int[] curr) {
        for (int i = 0; i < 3; i++) {
            if (prev[i] != curr[i]) {
                return true;
            }
        }
        return false;
    }


    void drawCircle(color[] elementColor) {
        float size = random (80, 300);
        float x = random(width);
        float y = random(height);
        for (float ring = size; ring >= 0; ring -= random(2, 10)) {
            int randomColorIndex = int(random(5));
            fill(elementColor[randomColorIndex]);
            ellipse(x, y, ring, ring);
        }
    }

    void setGradient(int x, int y, float w, float h, color c1, color c2) {
        noFill();
        for (int i = y; i <= y+h; i++) {
            float inter = map(i, y, y+h, 0, 1);
            color c = lerpColor(c1, c2, inter);
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