/*
 * Start robot movement
 * direction: 0 - forward, 1 - backward, 2 - turn left, 3 - turn right
 */
void motorsStart(int dir) {
  delay(DelayBetweenMotorsCommands);
  switch(dir) {
    case FORWARD:
      rotateBothForward();
    break;
    case BACKWARD:
      rotateBothBackward();
    break;
    case LEFT:
      rotateLeft(true);
    break;
    case RIGHT:
      rotateRight(true);
    break;
  }

  motorsSetSpeed(currentSpeed);
  course = 0;
}

void rotateBothForward() {
  digitalWrite(MotorRightForwardPin, HIGH);
  digitalWrite(MotorLeftForwardPin, HIGH);
  digitalWrite(MotorRightBackPin, LOW);
  digitalWrite(MotorLeftBackPin, LOW);
}

void rotateBothBackward() {
  digitalWrite(MotorRightForwardPin, LOW);
  digitalWrite(MotorLeftForwardPin, LOW);
  digitalWrite(MotorRightBackPin, HIGH);
  digitalWrite(MotorLeftBackPin, HIGH);
}

void rotateLeft(boolean fwd) {
  if (fwd) {//rotate forward
    digitalWrite(MotorLeftForwardPin, HIGH);
    digitalWrite(MotorLeftBackPin, LOW);
  } else {
    digitalWrite(MotorLeftForwardPin, LOW);
    digitalWrite(MotorLeftBackPin, HIGH);
  }
  digitalWrite(MotorRightForwardPin, LOW);
  digitalWrite(MotorRightBackPin, LOW);
}

void rotateRight(boolean fwd) {
  if (fwd) {//rotate forward
    digitalWrite(MotorRightForwardPin, HIGH);
    digitalWrite(MotorRightBackPin, LOW);
  } else {
    digitalWrite(MotorRightForwardPin, LOW);
    digitalWrite(MotorRightBackPin, HIGH);
  }
  digitalWrite(MotorLeftForwardPin, LOW);
  digitalWrite(MotorLeftBackPin, LOW);
}

/*
 * Stop robot movement
 */
void motorsStop() {
  digitalWrite(MotorRightForwardPin, LOW);
  digitalWrite(MotorLeftForwardPin, LOW);
  digitalWrite(MotorRightBackPin, LOW);
  digitalWrite(MotorLeftBackPin, LOW);
}

/*
 * Set robot movement speed for 2 motors
 */
void motorsSetSpeed(int s) {
  currentSpeed = s;
  if (currentSpeed < MinSpeed) currentSpeed = MinSpeed;
  if (currentSpeed > MaxSpeed) currentSpeed = MaxSpeed;
  int leftMotorSpeed = (int) currentSpeed*MotorSpeedDif;
  analogWrite(MotorRightSpeedPin, currentSpeed);
  analogWrite(MotorLeftSpeedPin, leftMotorSpeed);
}

void debugMotorsSpeed() {
  char buffer [200];
  sprintf(buffer, "LSP: %4d | RSP: %4d", leftSpeedSensorCounter, rightSpeedSensorCounter);
  Serial.println(buffer); 
  leftSpeedSensorCounter = 0;
  rightSpeedSensorCounter = 0;
}
