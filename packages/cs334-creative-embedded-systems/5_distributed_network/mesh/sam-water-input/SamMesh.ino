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

String ampValL, ampValC, ampValR;
int ampPinL = 32;
int ampPinC = 33;
int ampPinR = 35;

// User stub
void sendMessage(); // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  String msg = String("sam--" + ampValL + "--" + ampValC + "--" + ampValR + "--");
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval(TASK_SECOND * 0.1);
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


void setup() {
  Serial.begin(115200);
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( ssid, password, &userScheduler, port );

  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();


  pinMode(ampPinL, INPUT);
  pinMode(ampPinC, INPUT);
  pinMode(ampPinR, INPUT);
}

void loop() {
  ampValL = String(analogRead(ampPinL));
  ampValC = String(analogRead(ampPinC));
  ampValR = String(analogRead(ampPinR));
  Serial.print(ampValL);//print the sound value to serial
  Serial.print(" ");
  Serial.print(ampValC);//print the sound value to serial
  Serial.print(" ");
  Serial.println(ampValR);//print the sound value to serial
  delay(100);
      
  mesh.update();
}
