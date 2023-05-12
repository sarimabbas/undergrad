int current = 0;

PImage[] lava = new PImage[24];
PImage[] intro = new PImage[17];
PImage[] avatarstate = new PImage[23];
PImage[] airball = new PImage[6];

class DanielViz {
    DanielViz() {

        // load images
        //airball gif
        airball[0] = loadImage( "./frames/airball/frame_0_delay-0.08s.gif" );
        airball[1] = loadImage( "./frames/airball/frame_1_delay-0.08s.gif" );
        airball[2] = loadImage( "./frames/airball/frame_2_delay-0.08s.gif" );
        airball[3] = loadImage( "./frames/airball/frame_3_delay-0.08s.gif" );
        airball[4] = loadImage( "./frames/airball/frame_4_delay-0.08s.gif" );
        airball[5] = loadImage( "./frames/airball/frame_5_delay-0.08s.gif" );

        //avatar state gif
        avatarstate[0] = loadImage( "./frames/avatarstate/frame_00_delay-0.1s.gif" );
        avatarstate[1] = loadImage( "./frames/avatarstate/frame_01_delay-0.1s.gif" );
        avatarstate[2] = loadImage( "./frames/avatarstate/frame_02_delay-0.1s.gif" );
        avatarstate[3] = loadImage( "./frames/avatarstate/frame_03_delay-0.1s.gif" );
        avatarstate[4] = loadImage( "./frames/avatarstate/frame_04_delay-0.1s.gif" );
        avatarstate[5] = loadImage( "./frames/avatarstate/frame_05_delay-0.1s.gif" );
        avatarstate[6] = loadImage( "./frames/avatarstate/frame_06_delay-0.1s.gif" );
        avatarstate[7] = loadImage( "./frames/avatarstate/frame_07_delay-0.1s.gif" );
        avatarstate[8] = loadImage( "./frames/avatarstate/frame_08_delay-0.1s.gif" );
        avatarstate[9] = loadImage( "./frames/avatarstate/frame_09_delay-0.1s.gif" );
        avatarstate[10] = loadImage( "./frames/avatarstate/frame_10_delay-0.1s.gif" );
        avatarstate[11] = loadImage( "./frames/avatarstate/frame_11_delay-0.2s.gif" );
        avatarstate[12] = loadImage( "./frames/avatarstate/frame_12_delay-0.2s.gif" );
        avatarstate[13] = loadImage( "./frames/avatarstate/frame_13_delay-0.1s.gif" );
        avatarstate[14] = loadImage( "./frames/avatarstate/frame_14_delay-0.1s.gif" );
        avatarstate[15] = loadImage( "./frames/avatarstate/frame_15_delay-0.1s.gif" );
        avatarstate[16] = loadImage( "./frames/avatarstate/frame_16_delay-0.1s.gif" );
        avatarstate[17] = loadImage( "./frames/avatarstate/frame_17_delay-0.1s.gif" );
        avatarstate[18] = loadImage( "./frames/avatarstate/frame_18_delay-0.1s.gif" );
        avatarstate[19] = loadImage( "./frames/avatarstate/frame_19_delay-0.1s.gif" );
        avatarstate[20] = loadImage( "./frames/avatarstate/frame_20_delay-0.1s.gif" );
        avatarstate[21] = loadImage( "./frames/avatarstate/frame_21_delay-0.1s.gif" );
        avatarstate[22] = loadImage( "./frames/avatarstate/frame_22_delay-0.1s.gif" );

        //intro gif
        intro[0] = loadImage( "./frames/intro/frame_00_delay-0.1s.gif" );
        intro[1] = loadImage( "./frames/intro/frame_01_delay-0.1s.gif" );
        intro[2] = loadImage( "./frames/intro/frame_02_delay-0.1s.gif" );
        intro[3] = loadImage( "./frames/intro/frame_03_delay-0.1s.gif" );
        intro[4] = loadImage( "./frames/intro/frame_04_delay-0.1s.gif" );
        intro[5] = loadImage( "./frames/intro/frame_05_delay-0.1s.gif" );
        intro[6] = loadImage( "./frames/intro/frame_06_delay-0.1s.gif" );
        intro[7] = loadImage( "./frames/intro/frame_07_delay-0.1s.gif" );
        intro[8] = loadImage( "./frames/intro/frame_08_delay-0.1s.gif" );
        intro[9] = loadImage( "./frames/intro/frame_09_delay-0.1s.gif" );
        intro[10] = loadImage( "./frames/intro/frame_10_delay-0.1s.gif" );
        intro[11] = loadImage( "./frames/intro/frame_11_delay-0.1s.gif" );
        intro[12] = loadImage( "./frames/intro/frame_12_delay-0.1s.gif" );
        intro[13] = loadImage( "./frames/intro/frame_13_delay-0.1s.gif" );
        intro[14] = loadImage( "./frames/intro/frame_14_delay-0.1s.gif" );
        intro[15] = loadImage( "./frames/intro/frame_15_delay-0.1s.gif" );
        intro[16] = loadImage( "./frames/intro/frame_16_delay-0.1s.gif" );

        //lava gif
        lava[0] = loadImage( "./frames/lava/frame_00_delay-0.15s.gif" );
        lava[1] = loadImage( "./frames/lava/frame_01_delay-0.15s.gif" );
        lava[2] = loadImage( "./frames/lava/frame_02_delay-0.15s.gif" );
        lava[3] = loadImage( "./frames/lava/frame_03_delay-0.15s.gif" );
        lava[4] = loadImage( "./frames/lava/frame_04_delay-0.15s.gif" );
        lava[5] = loadImage( "./frames/lava/frame_05_delay-0.15s.gif" );
        lava[6] = loadImage( "./frames/lava/frame_06_delay-0.15s.gif" );
        lava[7] = loadImage( "./frames/lava/frame_07_delay-0.15s.gif" );
        lava[8] = loadImage( "./frames/lava/frame_08_delay-0.15s.gif" );
        lava[9] = loadImage( "./frames/lava/frame_09_delay-0.15s.gif" );
        lava[10] = loadImage( "./frames/lava/frame_10_delay-0.15s.gif" );
        lava[11] = loadImage( "./frames/lava/frame_11_delay-0.15s.gif" );
        lava[12] = loadImage( "./frames/lava/frame_12_delay-0.15s.gif" );
        lava[13] = loadImage( "./frames/lava/frame_13_delay-0.15s.gif" );
        lava[14] = loadImage( "./frames/lava/frame_14_delay-0.15s.gif" );
        lava[15] = loadImage( "./frames/lava/frame_15_delay-0.15s.gif" );
        lava[16] = loadImage( "./frames/lava/frame_16_delay-0.15s.gif" );
        lava[17] = loadImage( "./frames/lava/frame_17_delay-0.15s.gif" );
        lava[18] = loadImage( "./frames/lava/frame_18_delay-0.15s.gif" );
        lava[19] = loadImage( "./frames/lava/frame_19_delay-0.15s.gif" );
        lava[20] = loadImage( "./frames/lava/frame_20_delay-0.15s.gif" );
        lava[21] = loadImage( "./frames/lava/frame_21_delay-0.15s.gif" );
        lava[22] = loadImage( "./frames/lava/frame_22_delay-0.15s.gif" );
        lava[23] = loadImage( "./frames/lava/frame_23_delay-0.15s.gif" );
    }

    void draw(int alexiData, int danielData, int sarimData, int[] samData, int[] sabrinaData) {
        switch(sarimData) {
            case 0:
                if(current == 0) {
                    image( airball[frameCount%6], 0, 0, width, height );
                    if(frameCount%18 == 0) {
                        current = 0;
                        frameCount = 0;
                    }
                } else if(current == 1) {
                    image( avatarstate[frameCount%23], 0, 0, width, height );
                    if(frameCount%23 == 0) {
                        current += 1;
                        frameCount = 0;
                    }
                } else {
                    image( intro[frameCount%17], 0, 0, width, height );
                    if(frameCount%17 == 0) {
                        current = 0;
                        frameCount = 0;
                    }
                }
                break;
            case 1:
                image( lava[frameCount%24], 0, 0, width, height );
                if(frameCount%24 == 0) {
                    current += 1;
                    frameCount = 0;
                }
                break;
        }
    }

}
