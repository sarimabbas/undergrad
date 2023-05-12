#include <AxisJoystick.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// pin stuff
#define TOUCH_PIN 4
#define TOUCH_THRESHOLD 35
#define JOY_PIN 32
#define PHOTO_PIN 33

// WiFi stuff
#define CONSOLE_IP "192.168.1.2"
#define CONSOLE_PORT 57222
const char* ssid = "esp32";
const char* password = "12345678";
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
WebServer server(80);
WiFiUDP udp;

// hardware stuff
AxisJoystick jstick(-1, -1, JOY_PIN);
bool touched = false;
int photoVal = 0;

void readTouch() {
    int val = touchRead(TOUCH_PIN);
    // sendPacket(String(val));
    if (val < TOUCH_THRESHOLD) {
        delay(50);
        if (touchRead(TOUCH_PIN) < TOUCH_THRESHOLD) {
            touched = true;
        } else {
            touched = false;
        }
    }
}

void readPhoto() { photoVal = analogRead(PHOTO_PIN); }

void sendPacket(String msg) {
    udp.beginPacket(CONSOLE_IP, CONSOLE_PORT);
    udp.printf(msg.c_str());
    udp.endPacket();
}

void setup() {
    // comms stuff
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    server.begin();

    // hardware stuff
    jstick.calibrate(0, 4095);
}

void loop() {
    // read inputs
    jstick.multipleRead();
    readTouch();
    readPhoto();

    if (touched) {
        sendPacket("momentary");
    }

    if (jstick.isUp()) {
        sendPacket("joyLeft");
    } else if (jstick.isDown()) {
        sendPacket("joyRight");
    } else {
        sendPacket("joyNone");
    }
}
