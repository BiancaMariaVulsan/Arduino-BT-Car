#include <UCMotor.h>

#define TRIG_PIN A2
#define ECHO_PIN A3
#define beepFrequence 50
#define TURN_DIST 30

UC_DCMotor leftMotor1(3, MOTOR34_64KHZ);
UC_DCMotor rightMotor1(4, MOTOR34_64KHZ);
UC_DCMotor leftMotor2(1, MOTOR34_64KHZ);
UC_DCMotor rightMotor2(2, MOTOR34_64KHZ);

String motorSet = "";

int MAX_SPEED_LEFT ;
int MAX_SPEED_RIGHT;
int SPEED = 100;

int buzzerPin = 2;
int flag = 0;

bool detMode = false;

void setup()  {

  pinMode(buzzerPin, OUTPUT);
  pinMode(ECHO_PIN, INPUT); //Set the connection pin output mode Echo pin
  pinMode(TRIG_PIN, OUTPUT);//Set the connection pin output mode trog pin

  Serial.begin(9600);
  delay(5000);

  MAX_SPEED_LEFT = SPEED;
  MAX_SPEED_RIGHT = SPEED;
}

void loop()
{
  char state = 'X';
  int temp = readPing();
  if ( temp <= TURN_DIST && temp > 0 && !detMode) {
    moveStop(); BEEP_INT();
  }

  if ( temp <= TURN_DIST && temp > 0 && detMode) {
    Serial.println("Tutning left!");
    for (int i = 0; i < 10; i++) {
      turnLeft();
    }
    delay(1000); 
    state = 'M';
    flag = 1;
    BEEP_INT();
  }
  
  if (Serial.available() > 0) {
    state = Serial.read();
    flag = 1;
  }
  if (flag == 1) {
    switch (state)
    {
      case 'F' : {
          detMode = false;
          Serial.println("Forward!");
          int temp = readPing();
          if ( temp <= TURN_DIST && temp > 0 ) {
            moveStop(); BEEP_INT();
          } else {
            digitalWrite(buzzerPin, LOW);
            moveForward();
          }
          break;
        }
      case 'B' : {
          detMode = false;
          Serial.println("Backwards!");
          moveBackward();
          break;
        }
      case 'L' : {
          detMode = false;
          Serial.println("Left!");
          turnLeft();
          break;
        }
      case 'R' : {
          detMode = false;
          Serial.println("Right!");
          turnRight();
          break;
        }
      case 'S' : {
          detMode = false;
          Serial.println("Stop!");
          moveStop();
          break;
        }
      case 'M' : {
          detMode = true;
          Serial.println("Detection mode!");
          digitalWrite(buzzerPin, LOW);
          moveForward();
          break;
        }
      default : {
          break;
        }
    }
    flag = 0;
  }
  Serial.flush();
  delay(100);
}

void moveForward(void)
{
  motorSet = "FORWARD";
  leftMotor1.run(FORWARD);
  rightMotor1.run(BACKWARD);
  leftMotor2.run(BACKWARD);
  rightMotor2.run(BACKWARD);
  leftMotor1.setSpeed(MAX_SPEED_LEFT);
  rightMotor1.setSpeed(MAX_SPEED_RIGHT);
  leftMotor2.setSpeed(MAX_SPEED_LEFT);
  rightMotor2.setSpeed(MAX_SPEED_RIGHT);
}

void moveBackward(void)
{
  motorSet = "BACKWARD";
  leftMotor1.run(BACKWARD);
  rightMotor1.run(FORWARD);
  leftMotor2.run(FORWARD);
  rightMotor2.run(FORWARD);
  leftMotor1.setSpeed(MAX_SPEED_LEFT);
  rightMotor1.setSpeed(MAX_SPEED_RIGHT);
  leftMotor2.setSpeed(MAX_SPEED_LEFT);
  rightMotor2.setSpeed(MAX_SPEED_RIGHT);
}

void turnLeft(void)
{
  static int MAX_SPEED_LEFT_AL, MAX_SPEED_RIGHT_AL;
  motorSet = "LEFT";
  leftMotor1.run(FORWARD);
  rightMotor1.run(BACKWARD);
  leftMotor2.run(FORWARD);
  rightMotor2.run(FORWARD);
  MAX_SPEED_LEFT_AL = MAX_SPEED_LEFT > 200 ? 200 : MAX_SPEED_LEFT;
  MAX_SPEED_RIGHT_AL = MAX_SPEED_RIGHT > 200 ? 200 : MAX_SPEED_RIGHT;
  leftMotor1.setSpeed(MAX_SPEED_LEFT_AL);
  rightMotor1.setSpeed(MAX_SPEED_RIGHT_AL);
  leftMotor2.setSpeed(MAX_SPEED_LEFT_AL);
  rightMotor2.setSpeed(MAX_SPEED_RIGHT_AL);
}

void turnRight(void)
{
  static int MAX_SPEED_LEFT_AR, MAX_SPEED_RIGHT_AR;
  motorSet = "RIGHT";
  leftMotor1.run(BACKWARD);
  rightMotor1.run(FORWARD);
  leftMotor2.run(BACKWARD);
  rightMotor2.run(BACKWARD);
  MAX_SPEED_LEFT_AR = MAX_SPEED_LEFT > 200 ? 200 : MAX_SPEED_LEFT;
  MAX_SPEED_RIGHT_AR = MAX_SPEED_RIGHT > 200 ? 200 : MAX_SPEED_RIGHT;
  leftMotor1.setSpeed(MAX_SPEED_LEFT_AR);
  rightMotor1.setSpeed(MAX_SPEED_RIGHT_AR);
  leftMotor2.setSpeed(MAX_SPEED_LEFT_AR);
  rightMotor2.setSpeed(MAX_SPEED_RIGHT_AR);
}

void moveStop(void)
{
  leftMotor1.run(5); rightMotor1.run(5);
  leftMotor2.run(5); rightMotor2.run(5);
}

int readPing()
{
  // establish variables for duration of the ping,
  // and the distance result in inches and centimeters:
  long duration, cm;
  // The PING is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, LOW);

  pinMode(ECHO_PIN, INPUT);
  duration = pulseIn(ECHO_PIN, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  if ( cm < TURN_DIST) {
    Serial.print("Distance: ");
    Serial.print(cm);
    Serial.println(" cm");
  }

  return cm ;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void BEEP_INT (void) {
  digitalWrite(buzzerPin, HIGH);
  delay(beepFrequence);
  digitalWrite(buzzerPin, LOW);
  delay(beepFrequence);
}
