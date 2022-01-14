
#include <Servo.h>
int leftServoPin = 12;
int rightServoPin = 10;
Servo rightServo;
Servo leftServo;

int gripServoPin = 11;
Servo gripServo;

#include <PS2X_lib.h>
PS2X ps2x;
int error = 0;
int type = 0;
int mode = 0;
int motor_speed = 1500;
int imotor_speed = 1500;

#include <EIRremote.h>
int IRPin = 2;
IRrecv myIR(IRPin);
decode_results results;


int BlueLED = 7;
int GreenLED = 4;
int button1 = 3;
int ControlState = 0;
bool button1_State;
int ActiveController = 0;
int ButtonCount = 0;

void ButtonRead() {
  button1_State = digitalRead(button1);
  if (button1_State == 1)
  {
    ButtonCount++;
  }
  if (ButtonCount > 32766)
  {
    ButtonCount = 0;
    ControlSwitch();
  }
}
void ControlSwitch() {
  if (mode == 0) {
    mode = 1;
    digitalWrite(BlueLED, LOW);
    digitalWrite(GreenLED, HIGH);
    Serial.println("IR Remote");
  }
  else {
    mode = 0;
    digitalWrite(BlueLED, HIGH);
    digitalWrite(GreenLED, LOW);
    Serial.println("PlayStation");
  }
}

void setup() {
  rightServo.attach(rightServoPin);
  leftServo.attach(leftServoPin);

  Serial.begin(57600);
  pinMode(BlueLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
  digitalWrite(BlueLED, LOW);
  digitalWrite(GreenLED, LOW);

  rightServo.writeMicroseconds(1490);
  leftServo.writeMicroseconds(1490);

  gripServo.attach(gripServoPin);
  gripServo.write(90);

  pinMode(IRPin, INPUT);
  myIR.enableIRIn();

  ps2x.config_gamepad(9, 5, 6, 8, true, false);
  ps2x.readType();


}
void loop() {
  // put your main code here, to run repeatedly:

  switch (mode) {
    case 0://0 is ps2 controller
    ButtonRead();
      ps2x.read_gamepad();
      joy_stick_move();
      if (ps2x.ButtonPressed(PSB_RED)) {
        openGrip();
      }
      if (ps2x.ButtonPressed(PSB_BLUE)) {
        closeGrip();
      }
      if (ps2x.Button(PSB_L1)) {
        turnLeft();
      }
      if (ps2x.Button(PSB_R1)) {
        turnRight();
      }
      delay(20);
      leftServo.writeMicroseconds(motor_speed);
      rightServo.writeMicroseconds(imotor_speed);
      break;
    case 1:// 1 is for IR remote
    ButtonRead();
      if (myIR.decode(&results)) {
        Serial.println(results.value, HEX);
        translateIR();
        myIR.resume();
      }

      break;
      
  }
  //Serial.println(imotor_speed);
  //Serial.println(motor_speed);
}

void forward() {
  rightServo.writeMicroseconds(1000);
  leftServo.writeMicroseconds(2000);
  Serial.println("forward");
}

void reverse() {
  rightServo.writeMicroseconds(2000);
  leftServo.writeMicroseconds(1000);
  Serial.println("reverse");
}


void spinRight() {
  rightServo.writeMicroseconds(1700);
  leftServo.writeMicroseconds(1700);
  Serial.println("Spinning Right");
}

void spinLeft() {
  rightServo.writeMicroseconds(1300);
  leftServo.writeMicroseconds(1300);
  Serial.println("Spinning Left");
}

void Stop() {
  rightServo.writeMicroseconds(1500);
  leftServo.writeMicroseconds(1500);
  Serial.println("Stopping");
}

void openGrip() {
  gripServo.write(0);
  Serial.println("Opening");
}

void closeGrip() {
  gripServo.write(180);
  Serial.println("Closing");
}

void turnLeft() {
  motor_speed = 1600;
  imotor_speed = 1450;
}
void turnRight() {
  motor_speed = 1550;
  imotor_speed = 1250;
}
void joy_stick_move() {
  int ly = ps2x.Analog(PSS_LY);
  int ry = ps2x.Analog(PSS_RY);
  imotor_speed = map(ry, 0, 255, 1000, 2000);
  motor_speed = map(ly, 0, 255, 2000, 1000);
  motor_speed += 2;
  motor_speed = constrain(motor_speed, 1000, 2000);
  imotor_speed -= 2;
  imotor_speed = constrain(imotor_speed, 1000, 2000);
  if (motor_speed > 1490 && motor_speed < 1510) {
    motor_speed = 1490;
  }
  if (imotor_speed > 1490 && imotor_speed < 1510) {
    imotor_speed = 1490;
  }
}

void translateIR() {
  switch (results.value) {
    case 0xFF906F:
      Serial.println("UP");
      forward();
      break;
    case 0xFFE01F:
      Serial.println("DOWN");
      reverse();
      break;
    case 0xFFC23D:
      Serial.println("FAST FORWARD");
      turnRight();
      break;
    case 0xFF22DD:
      Serial.println("REWIND");
      turnLeft();
      break;
    case 0xFF629D:
      Serial.println("VOL+");
      spinRight();
      break;
    case 0xFFA857:
      Serial.println("VOL-");
      spinLeft();
      break;
    case 0xFF6897:
      Serial.println("0");
      openGrip();
      break;
    case 0xFF30CF:
      Serial.println("1");
      closeGrip();
      break;
    case 0xFF7A85:
      Serial.println("3");
      Stop();
      break;
    default:
      break;

  }
}
