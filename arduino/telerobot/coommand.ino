/*
 * Commands List:
 * '0' - Stop motors
 * '1,direction' - Start motors
 * '2,speed' - Set motors speed
 * '3,angle' - rotate camera left and right
 * '4,angle' - rotate camera up and down
 */
void handleCommand(String cmd) {
  String params[10];
  char delimeter = ',';
  int paramsCount = 0;
  int dir = 0;

  // Parse cmd String
  if(cmd.indexOf(delimeter) > 0) {
    while(cmd.indexOf(delimeter) > 0) {
      params[paramsCount] = cmd.substring(0, cmd.indexOf(delimeter));
      paramsCount++;
      cmd = cmd.substring(cmd.indexOf(delimeter)+1, cmd.length()+1);
    }  
    params[paramsCount] = cmd;
    paramsCount++;
  } else {
    params[paramsCount] = cmd;
    paramsCount++;
  }

  switch(params[0].toInt()){
    case 0:
      state = STOP;
      motorsStop();
    break;
    case 1:
      dir = params[1].toInt();
      motorsStop();
      state = dir;
      motorsStart(dir);
    break;
    case 2:
      motorsSetSpeed(params[1].toInt());
    break;
    case 3:
      horizontalServo.write(params[1].toInt());
    break;
    case 4:
      int pos = params[1].toInt();
      if (pos > 140) { pos = 140; }
      if (pos < 85) { pos = 85; }
      verticalServo.write(pos);
    break;
  }
}
