  
// Required dependencies:
// From Arduino library manager: ArduinoJson, TaskScheduler
// From Github (https://github.com/me-no-dev/AsyncTCP): AsyncTCP
    // to install, download as zip file
    // Sketch --> Include Library --> Add .zip libary
    
#include <painlessMesh.h>
#include <AsyncTCP.h>

Scheduler userScheduler; // to control your personal task
painlessMesh mesh;

#define   ssid     "akw-takeover"
#define   password   "scott-squad"
#define   port       5555

//constant for the pin where daniel's sensor is plugged into
const int danielPin = 32;

//constant for the sarim's pin
const int sarimPin = 18;

//global to keep track of current pin
int current = danielPin;

//Keeps track of this many values when averaging
const int numReadings = 10;

int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

// User stub
void sendMessage(); // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

String readSensors(int pin) {
  // subtract last reading
  total = total - readings[readIndex];
  // read from sensor
  if(pin == danielPin) {
    readings[readIndex] = analogRead(pin);
  } else {
    readings[readIndex] = digitalRead(pin);
  }
  Serial.println(digitalRead(sarimPin));
  // add the reading to total
  total = total + readings[readIndex];
  // advance to next position in array
  readIndex = readIndex + 1;

  //if at the end of array
  if (readIndex >= numReadings) {
    // wrap around to beginning
    readIndex = 0;
  }

  // average time
  average = total / numReadings;
  
  String returnavg = String(average);
  String returnVal = "";
  if(pin == danielPin) {
    returnVal = String("daniel-" + returnavg + "-");
  } else {
    returnVal = String("sarim-" + String(readings[readIndex]) + "-");
  }

//  Serial.println(average);
  return returnVal;
}

void sendMessage() {
  String msg = readSensors(current);
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval(random( TASK_SECOND * 1, TASK_SECOND * 5 ));
  if(current == danielPin) {
    current = sarimPin;
  } else {
    current = danielPin;
  }
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup()
{
  Serial.begin(9600);
  // initialize readings to 0
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  pinMode(sarimPin, INPUT_PULLUP);

  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( ssid, password, &userScheduler, port );

  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();
}

void loop()
{
  mesh.update();
}
