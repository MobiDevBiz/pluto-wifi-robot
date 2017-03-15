#include <TimerOne.h>
#include <Servo.h>

/*
 * For future: I know what PID controllers speed algorithms are, but they are not working good enough for this robot
 */

// Motors Pins
#define MotorLeftSpeedPin    5  // Левый (А) мотор СКОРОСТЬ — ENA
#define MotorLeftForwardPin  11 // Левый (А) мотор ВПЕРЕД — IN1
#define MotorLeftBackPin     9  // Левый (А) мотор НАЗАД — IN2
#define MotorRightForwardPin 8  // Правый (В) мотор ВПЕРЕД — IN3
#define MotorRightBackPin    7  // Правый (В) мотор НАЗАД — IN4
#define MotorRightSpeedPin   6  // Правый (В) мотор СКОРОСТЬ — ENB

// Servo Pins
#define HorizontalServoPin  10  // Servo to rotate camera left and right
#define VerticalServoPin    12  // Servo to rotate camera up and down

#define ServoInitPos        95  // Initial position of servos

// Speed encoders Pins (only pins 2 and 3 support interrupts)
#define LeftMotorSpeedSensorPin  3  // Encoder for left Motor
#define RightMotorSpeedSensorPin 2  // Encoder for right Motor
#define TurnHolesCount           40 // Number of signals from speed encoder for a single turn (20 holes in encoder)

#define MotorSpeedDif 0.9 // Left motor rotates 25% faster
#define DelayBetweenMotorsCommands 500 // Needed not to overload motors by quickly changing rotation direction (1s)
#define MeasureSpeed false
#define MeasureSpeedInterval 1000000 // 1s

#define MinSpeed 100 // 0..255
#define MaxSpeed 200 // 0..255

// Program states
#define FORWARD  0
#define BACKWARD 1
#define LEFT     2
#define RIGHT    3
#define STOP     4

volatile int state = 0; // Current program state
volatile int currentSpeed = 100; // Current motors speed

volatile int course = 0;
volatile int leftSpeedSensorCounter = 0;
volatile int rightSpeedSensorCounter = 0;

// Servos
Servo horizontalServo;
Servo verticalServo;

void handleLeftSpeedSensor() {
  if (MeasureSpeed) leftSpeedSensorCounter++;
  if (state == STOP) {
    return;
  } else {
    course--;
  }
  switch(state) {
    case FORWARD:
      if(course > 0) { rotateLeft(true); } else { rotateRight(true); } // syncs number of rotations between motors (for straight movement)
    break;
    case BACKWARD:
      if(course > 0) { rotateLeft(false); } else { rotateRight(false); } // syncs number of rotations between motors (for straight movement)
    break;
    case LEFT:
      if (abs(course) >= TurnHolesCount) {
        motorsStop();
      }
    break;
  }
}

void handleRightSpeedSensor() {
  if (MeasureSpeed) rightSpeedSensorCounter++;
  if (state == STOP) {
    return;
  } else {
    course++;
  }
  switch(state) {
    case FORWARD:
      if(course < 0) { rotateRight(true); } else { rotateLeft(true); } // syncs number of rotations between motors (for straight movement)
    break;
    case BACKWARD:
      if(course < 0) { rotateRight(false); } else { rotateLeft(false); } // syncs number of rotations between motors (for straight movement)
    break;
    case RIGHT:
      if (abs(course) >= TurnHolesCount) {
        motorsStop();
      }
    break;
  }
}

void setup() {
  Serial.begin(9600);
  // Setup Pins
  pinMode(MotorLeftForwardPin, OUTPUT);
  pinMode(MotorLeftBackPin, OUTPUT);
  pinMode(MotorLeftSpeedPin, OUTPUT);
  pinMode(MotorRightForwardPin, OUTPUT);
  pinMode(MotorRightBackPin, OUTPUT);
  pinMode(MotorRightSpeedPin, OUTPUT);
  pinMode(LeftMotorSpeedSensorPin, INPUT_PULLUP);
  pinMode(RightMotorSpeedSensorPin, INPUT_PULLUP);

  // Servos
  horizontalServo.attach(HorizontalServoPin);
  horizontalServo.write(ServoInitPos);
  verticalServo.attach(VerticalServoPin);
  verticalServo.write(ServoInitPos);

  // Setup Speed Encoders
  attachInterrupt(digitalPinToInterrupt(LeftMotorSpeedSensorPin), handleLeftSpeedSensor, RISING);  // increase counter when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt(RightMotorSpeedSensorPin), handleRightSpeedSensor, RISING);  // increase counter when speed sensor pin goes High

  // Setup Timer for speed measuring
  if (MeasureSpeed) {
    Timer1.initialize(MeasureSpeedInterval);
    Timer1.attachInterrupt(debugMotorsSpeed);
  }
}

void loop() {
  if (Serial.available() > 0) {
    handleCommand(Serial.readString());
  }
}
