#include <Arduino.h>
#include <Servo.h>          // servo motor lib
#include <NewPing.h>        // ultrasonic sensor lib
#include <SoftwareSerial.h> // lib for communication with the bluetooth module

const int ENA = 6; // left motor
const int IN1 = 8; // left motor forward
const int IN2 = 7; // left motor backward
const int ENB = 5; // right motor
const int IN3 = 4; // right motor forward
const int IN4 = 2; // right motor backward

int speed[3] = {85, 170, 255}; // speed settings
const int max_sensor_distance = 200;
bool btControlMode = true;

// servo motor
const int servoPin = 3;

// ultrasonic sensor
const int trigPin = A1;
const int echoPin = A0;

Servo servo;
NewPing sonar = NewPing(trigPin, echoPin, max_sensor_distance);
SoftwareSerial btSerial(9, 10); // TX, RX

// controls the speed at which the motor operates by setting the PWM-values of the enable pins
void setSpeed(int speed)
{
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

// rotoates both motors forward
void forward()
{
  digitalWrite(IN1, HIGH); // left motor forward enable
  digitalWrite(IN2, LOW);  // left motor backward disable
  digitalWrite(IN3, HIGH); // right motor forward enable
  digitalWrite(IN4, LOW);  // right motor backward disable
}

// rotates both motors back
void back()
{
  digitalWrite(IN1, LOW);  // left motor forward disable
  digitalWrite(IN2, HIGH); // left motor backward enable
  digitalWrite(IN3, LOW);  // right motor forward disable
  digitalWrite(IN4, HIGH); // right motor backward enable
}

// rotates left motor back and right motor forward
void left()
{
  digitalWrite(IN1, LOW);  // left motor forward disable
  digitalWrite(IN2, HIGH); // left motor backward enable
  digitalWrite(IN3, HIGH); // right motor forward enable
  digitalWrite(IN4, LOW);  // right motor backward disable
}

// rotates left motor forward and right motor back
void right()
{
  digitalWrite(IN1, HIGH); // left motor forward enable
  digitalWrite(IN2, LOW);  // left motor backward disable
  digitalWrite(IN3, LOW);  // right motor forward disable
  digitalWrite(IN4, HIGH); // right motor backward enable
}

// stops the rotation of the motors
void stop()
{
  digitalWrite(IN1, LOW); // left motor forward disable
  digitalWrite(IN2, LOW); // left motor backward disable
  digitalWrite(IN3, LOW); // right motor forward disable
  digitalWrite(IN4, LOW); // right motor backward disable
}

// calculates the distance 
int getDistance()
{
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int distance = sonar.ping_cm();
  return distance;
}

// determines the mode in which the rover moves
void determineMode(char command)
{
  switch (command)
  {
  case 'X':
    btControlMode = true;
    break;
  case 'Y':
    btControlMode = false;
    break;
  }
}

// rover moves forward and upon detection of an obstacle scans its environment and moves in the direction with the furthest obstable
void handlePathfinderMode()
{
  int distance[3]; // {forward, left, right}
  setSpeed(speed[0]);
  forward();
  delay(100);
  distance[0] = getDistance();

  if (distance[0] < 30)
  {
    stop();
    delay(1000);

    back();
    delay(500);
    stop();
    delay(300);

    // ping left
    servo.write(160);
    delay(1000);
    distance[1] = getDistance();

    // ping right
    servo.write(30);
    delay(1000);
    distance[2] = getDistance();

    servo.write(90);
    delay(500);
    if (distance[1] < distance[2])
    {
      right();
      delay(300);
    }
    else
    {
      left();
      delay(300);
    }
  }
}

// rover is controlled via smartphone
void handleBTControlMode(char command)
{
  stop(); // start loop with wheels stopped, otherwise they would spin continuously until new command is given

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

void setup()
{
  // motors setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // initialize with lowest speed
  setSpeed(speed[0]);

  // servo setup
  servo.attach(servoPin);
  servo.write(90); // move servo to center position -> 90°

  // ultrasonic setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // initialize software serial monitor
  btSerial.begin(9600);
}

void loop()
{
  if (btSerial.available() > 0)
  {
    char command = btSerial.read();
    determineMode(command);

    if (btControlMode)
    {
      handleBTControlMode(command);
    }
  }

  // this part needs to be outside of the if (btSerial.available() > 0) condition for the sonar to trigger even if no command is given
  if (!btControlMode)
  {
    handlePathfinderMode();
  }
}
