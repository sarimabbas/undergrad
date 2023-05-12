#ifndef DRIVER
#define DRIVER

#include <Servo.h>

#define MAX_POS 180
#define NEUTRAL_POS 90
#define MIN_POS 0
#define MOVE_PADDING_SMALL 20
#define MOVE_PADDING_LARGE 40

// forward backward arm movement, "FB"
// vertical arm movement, "UD"
// gripper open close, "GR"
/// circular rotation, "BS"
extern Servo frontBackServo, upDownServo, gripperServo, baseServo;
extern long frontBackPos, upDownPos, gripperPos, basePos;

// attach to pins and initialize
int driverSetup(int frontBackPin, int upDownPin, int gripperPin, int basePin);

// low level precise control, with saturation 
int manipulateServo(const char* servo, int pos);

// convenience methods based on empirical thresholds
int gripperOpen(void); 
int gripperClose(void);
int moveLeft(void);
int moveRight(void);
int moveMiddle(void);
int moveUp(void);
int moveDown(void);
int manipulateNeutral(const char* servo);
long getServoPos(const char* servo);

#endif
