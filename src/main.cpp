#include <Arduino.h>
#include <Servo.h>

const int ENA = 6;  // left motor
const int IN1 = 8;  // left motor forward
const int IN2 = 7;  // left motor backward
const int ENB = 5;  // right motor
const int IN3 = 4;  // right motor forward
const int IN4 = 2;  // right motor backward

int speed[3] = { 85, 170, 255 };  // speed settings

const int servoPin = 3;
Servo myservo;

// controls the speed at which the motor operates by setting the PWM-values of the enable pins
void setSpeed(int speed)
{
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

// rotoates both motors forward
void forward()
{
  digitalWrite(IN1, HIGH);  // left motor forward enable
  digitalWrite(IN2, LOW);   // left motor backward disable
  digitalWrite(IN3, HIGH);  // right motor forward enable
  digitalWrite(IN4, LOW);   // right motor backward disable
}

// rotates both motors back
void back()
{
  digitalWrite(IN1, LOW);   // left motor forward disable
  digitalWrite(IN2, HIGH);  // left motor backward enable
  digitalWrite(IN3, LOW);   // right motor forward disable
  digitalWrite(IN4, HIGH);  // right motor backward enable
}

// rotates left motor back and right motor forward
void left()
{
  digitalWrite(IN1, LOW);   // left motor forward disable
  digitalWrite(IN2, HIGH);  // left motor backward enable
  digitalWrite(IN3, HIGH);  // right motor forward enable
  digitalWrite(IN4, LOW);   // right motor backward disable
}

// rotates left motor forward and right motor back
void right()
{
  digitalWrite(IN1, HIGH);  // left motor forward enable
  digitalWrite(IN2, LOW);   // left motor backward disable
  digitalWrite(IN3, LOW);   // right motor forward disable
  digitalWrite(IN4, HIGH);  // right motor backward enable
}

// stops the rotation of the motors
void stop()
{
  digitalWrite(IN1, LOW);  // left motor forward disable
  digitalWrite(IN2, LOW);  // left motor backward disable
  digitalWrite(IN3, LOW);  // right motor forward disable
  digitalWrite(IN4, LOW);  // right motor backward disable
}

// rover is in obstacle-avoidance mode
void pathfinderMode()
{
}

void setup()
{
  Serial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  setSpeed(speed[0]);

  myservo.attach(servoPin);
  myservo.write(90);  // move servos to center position -> 90°
}

void loop()
{
  if (Serial.available() > 0)
  {
    char command = Serial.read();
    stop(); // initialize with motors stopped

    switch (command)
    {
      case 'F':
        forward();
        break;
      case 'B':
        back();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case '1':
        setSpeed(speed[0]);
        break;
      case '2':
        setSpeed(speed[1]);
        break;
      case '3':
        setSpeed(speed[2]);
        break;
    }
  }
}
