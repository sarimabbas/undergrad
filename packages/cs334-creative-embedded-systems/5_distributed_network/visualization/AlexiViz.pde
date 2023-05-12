class AlexiViz {
    AlexiViz() {}

    int numElements = 5;

    color elementColors[][] = {
        { #5562D3, #717AD3, #959AD3, #95C3D3, #30A8D3 }, // sam - water
        { #86A4AF, #94C4D3, #9AA6AF, #CEDFEB, #94A1A9 }, // daniel - air
        { #483123, #FFDD80, #AD7C54, #201710, #FF8080 }, // sarim - lava
        { #9C4749, #D24749, #D29B47, #AF8339, #AF3464 }, // sabrina - fire
        { #30D392, #186948, #4B9322, #496D34, #304823 }, // alexi - earth
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

    int[][] constructNewData(int[] samData, int danielData, int sarimData, int[] sabrinaData,int alexiData) {
        summation[0] += samData[0] / 1500.0 + samData[1] / 1500.0 + samData[2] / 1500.0;
        summation[1] += danielData / 4096.0;
        summation[2] += sarimData;
        summation[3] += sabrinaData[0] / 4096.0 + sabrinaData[1] / 4096.0;
        summation[4] += alexiData / 4096.0;

        int newData[][] = {
            {samData[0], samData[1], samData[2]},   // sam
            {danielData, 0, 0},                     // daniel
            {sarimData, 0, 0},                      // sarim
            {sabrinaData[0], sabrinaData[1], 0},    // sabrina
            {alexiData, 0, 0}                       // alexi
        };
        return newData;
    }

    boolean dataChanged(int[] prev, int[] curr) {
        for (int i = 0; i < 3; i++) {
            if (prev[i] != curr[i]) {
                return true;
            }
        }
        return false;
    }


    void drawChart(int[][] data) {
        int i = 0;
        for (float rad = 0; rad < TWO_PI; rad += TWO_PI / 5.0) {
            fill(elementColors[i][0]);
            noStroke();

            arc(width / 2, height / 2, width / 2, width / 2, rad, rad + TWO_PI / 5.0, PIE);

            fill(255, 255, 255);
            text(summation[i], width / 2 + width / 8 * cos(rad + TWO_PI / 10.0), height / 2 + width / 8 * sin(rad + TWO_PI / 10.0));

            i++;
        }

    }


    void draw(int alexiData, int danielData, int sarimData, int[] samData, int[] sabrinaData) {
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
