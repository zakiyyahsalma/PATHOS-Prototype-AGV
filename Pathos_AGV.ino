#include <Servo.h>
int val1, val2, val3, val4, val5 = 999;

// Sensor and Motor Pins
const int sensorPins[] = { A7, A6, A5, A4, A3 };
const int trigPin = 9;
const int echoPin = 8;
const int enA = 3, enB = 5;
const int in1 = 2, in2 = 4, in3 = 7, in4 = 6;
const int servoPin = 11;

Servo lifting;

// PID Constants
float kp = 1.5;
float ki = 0.0;
float kd = 0.5;

int baseSpeed = 55;
int threshold = 500;
float interval = 90;
float previousTime;
float integral;
int currentposition;
int error;
int previousError = 0;
bool logic = false;
bool finish = false;
float distance;

const int trigpin = 9;
const int echopin = 8;
// long duration;
// int distance;

int room = 11;
bool room11, room_12, room_13, room_21, room_22, room_23 = false;
bool isLineFollowing, lineFollowing1, lineFollowing2, lineFollowing3, lineFollowing4 = false;
bool isTurnLeft, isTurnRight = false;
bool isMoveForward, isRotate, isRotate2 = false;
bool isTurnLeft2 = false;
bool isLifting = false;

float getDistance() {
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  long duration = pulseIn(echopin, HIGH);
  return duration * 0.034 / 2;
}

void printValues() {
  Serial.print("val1: ");
  Serial.print(val1);
  Serial.print("  val2: ");
  Serial.print(val2);
  Serial.print("  val3: ");
  Serial.print(val3);
  Serial.print("  val4: ");
  Serial.print(val4);
  Serial.print("  val5: ");
  Serial.println(val5);  // use println for new line
}

void controlMotors(int leftspeed, int rightspeed) {
  analogWrite(enA, leftspeed);
  analogWrite(enB, rightspeed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  // Serial.print(leftspeed);
  // Serial.print(" ");
  // Serial.println(rightspeed);
}

void moveBackwards(int leftspeed, int rightspeed) {
  analogWrite(enA, leftspeed);
  analogWrite(enB, rightspeed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  // Serial.print(leftspeed);
  // Serial.print(" ");
  // Serial.println(rightspeed);
}

bool detectIntersection() {
  int val1 = analogRead(sensorPins[0]);
  int val2 = analogRead(sensorPins[1]);
  int val3 = analogRead(sensorPins[2]);
  int val4 = analogRead(sensorPins[3]);
  int val5 = analogRead(sensorPins[4]);

  if (val1 < threshold && val2 < threshold && val3 < threshold && val4 < threshold && val5 < threshold) {
    return true;
  }
  return false;
}


bool detectStraightLine() {
  int val1 = analogRead(sensorPins[0]);
  int val2 = analogRead(sensorPins[1]);
  int val3 = analogRead(sensorPins[2]);
  int val4 = analogRead(sensorPins[3]);
  int val5 = analogRead(sensorPins[4]);

  if (val1 > threshold && val2 > threshold && val3 < threshold && val4 > threshold && val5 > threshold
      // || (val1 > threshold && val2 < threshold && val3 > threshold && val4 > threshold && val5 > threshold)
      // || (val1 > threshold && val2 > threshold && val3 > threshold && val4 < threshold && val5 > threshold)
  ) {
    return true;
  }
  return false;
}

bool detectRightLine() {
  int val1 = analogRead(sensorPins[0]);
  int val2 = analogRead(sensorPins[1]);
  int val3 = analogRead(sensorPins[2]);
  int val4 = analogRead(sensorPins[3]);
  int val5 = analogRead(sensorPins[4]);

  if (val1 > threshold && val2 > threshold && val3 > threshold && val4 < threshold && val5 < threshold) {
    return true;
  }
  return false;
}

void turnLeft() {
  controlMotors(40, 0);
  // delay(350);  // Tune as needed
}

void turnRight() {
  controlMotors(0, 50);
  // delay(350);
}

void turnLeft(int L, int R) {
  controlMotors(L, R);
  // delay(350);  // Tune as needed
}

void rotateCCW() {
  analogWrite(enA, 35);
  analogWrite(enB, 35);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void moveForward() {
  controlMotors(50, 50);
  // delay(350);
}

void stopMotors() {
  controlMotors(0, 0);
}

int limit(int value, int minValue, int maxValue) {
  if (value < minValue) {
    return minValue;
  } else if (value > maxValue) {
    return maxValue;
  } else {
    return value;
  }
}

void left() {
  if (val1 < threshold && val2 < threshold && val3 < threshold && val4 < threshold && val5 < threshold) {
    turnLeft();
    if (val1 > threshold && val2 > threshold && val3 < threshold && val4 > threshold && val5 > threshold) {
      stopMotors();
    }
  }
}

void readSensors() {
  val1 = analogRead(sensorPins[0]);
  val2 = analogRead(sensorPins[1]);
  val3 = analogRead(sensorPins[2]);
  val4 = analogRead(sensorPins[3]);
  val5 = analogRead(sensorPins[4]);
}

int readError() {

  val1 = analogRead(sensorPins[0]);
  val2 = analogRead(sensorPins[1]);
  val3 = analogRead(sensorPins[2]);
  val4 = analogRead(sensorPins[3]);
  val5 = analogRead(sensorPins[4]);

  if (val3 < threshold) {
    // Serial.println("3");

    currentposition = 0;

  } else {
    if (val1 < threshold) {
      // Serial.println("2");

      currentposition = 10;
    }

    else if (val5 < threshold) {
      // Serial.println("4");

      currentposition = -10;
    } else {
      currentposition;
    }
  }

  error = 0 - currentposition;
  return error;
}

int readError2() {

  val1 = analogRead(sensorPins[0]);
  val2 = analogRead(sensorPins[1]);
  val3 = analogRead(sensorPins[2]);
  val4 = analogRead(sensorPins[3]);
  val5 = analogRead(sensorPins[4]);

  if (val3 < threshold) {
    // Serial.println("3");

    currentposition = 0;

  } else {
    if (val1 < threshold) {
      // Serial.println("2");

      currentposition = -10;
    }

    else if (val5 < threshold) {
      // Serial.println("4");

      currentposition = 10;
    } else {
      currentposition;
    }
  }

  error = 0 - currentposition;
  return error;
}

void pidControl() {
  unsigned long now = millis();
  if (now - previousTime >= interval) {
    error = readError();

    previousTime = now;
    previousError = error;

    float proportional = kp * error;
    // integral += error * (interval);
    // integral = (error + previousError) * interval / 90;
    integral = 0;
    float derivative = (error - previousError) / (interval / 90);

    int correction = proportional + ki * integral + kd * derivative;


    //     // Serial.println(error);
    //     // Serial.println(correction);

    //     // int leftspeed = baseSpeed + correction;
    //     // int rightspeed = baseSpeed - correction;

    //     // int rs = constrain(rightspeed, 30, 255);
    //     // int ls = constrain(leftspeed, 30, 255);

    int leftspeed = limit(baseSpeed + correction, 10, 60);
    int rightspeed = limit(baseSpeed - correction - 10, 10, 60);


    controlMotors(rightspeed, leftspeed);
  }
}

void pidControl2() {
  unsigned long now = millis();
  if (now - previousTime >= interval) {
    error = readError2();

    previousTime = now;
    previousError = error;

    float proportional = kp * error;
    // integral += error * (interval);
    integral = (error + previousError) * interval / 90;
    float derivative = (error - previousError) / (interval / 90);

    int correction = proportional + ki * integral + kd * derivative;


    //     // Serial.println(error);
    //     // Serial.println(correction);

    //     // int leftspeed = baseSpeed + correction;
    //     // int rightspeed = baseSpeed - correction;

    //     // int rs = constrain(rightspeed, 30, 255);
    //     // int ls = constrain(leftspeed, 30, 255);

    int rightspeed = limit(baseSpeed + correction, 10, 60);
    int leftspeed = limit(baseSpeed - correction - 10, 10, 60);


    analogWrite(enA, leftspeed);
    analogWrite(enB, rightspeed);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
}

void setup() {
  Serial.begin(9600);

  lifting.attach(servoPin);
  lifting.writeMicroseconds(1500);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  for (int i = 0; i < 5; i++) {
    pinMode(sensorPins[i], INPUT);
  }
}

void loop() {
  // int val1, val2, val3, val4, val5 = 999;

  switch (room) {

    // bool lineFollowing, lineFollowing1, lineFollowing2, lineFollowing3, lineFollowing4 = false;
    // Serial.println(999);
    // isLineFollowing = false;
    // isTurnLeft, isTurnRight, isMoveForward = true;
    case 11:
      isTurnLeft, isTurnRight, isMoveForward, isRotate, isLifting, isRotate2 = true;
      isLineFollowing = false;
      long duration;
      float distance;
      // Serial.println(1);

      while (!isLineFollowing) {
        pidControl();
        // printValues();
        // Serial.println(2);
        if ((val1 < threshold && val2 < threshold && val3 < threshold && val4 < threshold && val5 < threshold)
            // || (val1 < threshold && val2 < threshold && val3 < threshold)
            // || (val4 < threshold && val5 < threshold && val3 < threshold)
            // || (val1 < threshold && val2 < threshold)
            // || (val4 < threshold && val5 < threshold)
        ) {
          isLineFollowing = true;
          isTurnLeft = false;
          // break;
        }
      }

      while (!isTurnLeft) {
        turnLeft();
        // Serial.println(3);
        readSensors();
        // printValues();
        if (val1 > threshold && val2 > threshold && val3 < threshold && val4 > threshold && val5 > threshold) {
          // stopMotors();
          // Serial.println(4);
          // delay(2000);
          isTurnLeft = true;
          isLineFollowing = false;
        }
      }

      while (!isLineFollowing) {
        pidControl();
        // printValues();
        // Serial.println(5);
        if ((val1 > threshold && val2 > threshold && val3 < threshold && val4 < threshold && val5 < threshold)
            || (val1 > threshold && val2 > threshold && val3 > threshold && val4 < threshold && val5 < threshold)) {
          isLineFollowing = true;
          isTurnRight = false;
          // break;
        }
      }

      while (!isTurnRight) {
        turnRight();
        // Serial.println(6);
        readSensors();
        // printValues();
        if (val1 > threshold && val2 < threshold && val3 > threshold && val4 > threshold && val5 > threshold) {
          // stopMotors();
          // Serial.println(7);
          turnLeft();
          delay(200);
          // stopMotors();
          // delay(5000);
          isTurnRight = true;
          isLineFollowing = false;
        }
      }

      while (!isLineFollowing) {
        pidControl();
        // printValues();
        // Serial.println(8);
        if (val1 < threshold && val2 < threshold && (val3 < threshold || val3 > threshold) && val4 > threshold && val5 > threshold) {
          isLineFollowing = true;
          isTurnLeft = false;
          isTurnLeft2 = true;
          // break;
        }
      }

      while (!isTurnLeft) {
        while (isTurnLeft2) {
          turnLeft();
          delay(800);
          // stopMotors();
          // delay(500);
          isTurnLeft2 = false;
        }
        turnLeft();
        // Serial.println(9);
        // delay(1000);
        readSensors();
        // printValues();
        if (val1 > threshold && val2 > threshold && val3 < threshold && val4 > threshold && val5 > threshold) {

          turnLeft();
          delay(200);
          analogWrite(enA, 30);
          analogWrite(enB, 35);
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          delay(700);
          stopMotors();
          // Serial.println(10);
          delay(1000);
          isTurnLeft = true;
          isLineFollowing = false;
          long duration;
          float distance;
        }
      }

      while (!isLineFollowing) {
        long duration;
        float distance;
        pidControl();
        // printValues();
        // Serial.println(11);
        distance = getDistance();
        // if (val1 > threshold && val2 > threshold && val3 > threshold && val4 > threshold && val5 > threshold)
        // // || (val1 > threshold && val2 > threshold)
        // // || (val4 > threshold && val5 > threshold))
        // {
        if (distance <= 10) {
          isLineFollowing = true;
          // isRotate = true;
          isLifting = false;
          // break;
        }
      }

      while (!isLifting) {
        stopMotors();
        delay(1000);
        lifting.writeMicroseconds(1400);
        delay(4500);
        lifting.writeMicroseconds(1500);
        delay(4000);
        isLifting = true;
        isRotate = false;
        isRotate2 = false;
      }

      while (!isRotate) {
        while (!isRotate2) {
          analogWrite(enA, 30);
          analogWrite(enB, 35);
          digitalWrite(in1, LOW);
          digitalWrite(in2, HIGH);
          digitalWrite(in3, HIGH);
          digitalWrite(in4, LOW);
          delay(1300);
          stopMotors();
          delay(500);
          rotateCCW();
          delay(700);
          isRotate2 = true;
        }
        // turnLeft();
        rotateCCW();
        // pidControl2();
        // Serial.println(12);
        readSensors();
        // delay(2000);
        // printValues();
        if (val1 > threshold && val2 > threshold && val3 < threshold && val4 > threshold && val5 > threshold) {
          // stopMotors();
          // Serial.println(13);
          // delay(2000);
          rotateCCW();
          delay(500);
          controlMotors(25, 30);
          // analogWrite(enA, 27);
          // analogWrite(enB, 30);
          // digitalWrite(in1, LOW);
          // digitalWrite(in2, HIGH);
          // digitalWrite(in3, HIGH);
          // digitalWrite(in4, LOW);
          delay(400);
          stopMotors();
          delay(500);
          turnRight();
          delay(300);
          stopMotors();
          delay(500);
          isRotate = true;
          // isLineFollowing = false;
          room = 12;
          // break;
        }
      }

    case 12:
      isLineFollowing = false;
      isTurnLeft, isTurnRight, isTurnLeft2 = true;
      isMoveForward, isRotate = true;

      while (!isLineFollowing) {
        pidControl();
        if ((val1 < threshold && val2 < threshold && val3 < threshold && val4 < threshold && val5 < threshold)
            || (val1 < threshold && val2 < threshold && val3 < threshold && val4 < threshold && val5 > threshold)
            || (val1 < threshold && val2 < threshold && val3 < threshold && val4 > threshold && val5 > threshold)
            || (val1 > threshold && val2 < threshold && val3 < threshold && val4 < threshold && val5 < threshold)
            || (val1 > threshold && val2 > threshold && val3 < threshold && val4 < threshold && val5 < threshold)) {
          isLineFollowing = true;
          isTurnLeft = false;
        }
      }

      while (!isTurnLeft) {
        turnLeft();
        // Serial.println(3);
        readSensors();
        // printValues();
        if (val1 > threshold && val2 > threshold && val3 < threshold && val4 > threshold && val5 > threshold) {
          // stopMotors();
          turnLeft();
          delay(300);
          // Serial.println(4);
          stopMotors();
          delay(3000);
          isTurnLeft = true;
          isLineFollowing = false;
        }
      }

      while (!isLineFollowing) {
        pidControl();
        // printValues();
        // Serial.println(8);
        if ((val1 < threshold && val2 < threshold && val3 < threshold && val4 > threshold && val5 > threshold)
            || (val1 < threshold && val2 < threshold && val3 > threshold && val4 > threshold && val5 > threshold)
            || (val1 < threshold && val2 < threshold && val3 < threshold && val4 < threshold && val5 > threshold)) {
          stopMotors();
          delay(1000);
          isLineFollowing = true;
          isTurnLeft = false;
          isTurnLeft2 = false;
          // break;
        }
      }

      while (!isTurnLeft) {
        while (isTurnLeft2) {
          turnLeft();
          delay(800);
          stopMotors();
          delay(500);
          isTurnLeft2 = false;
        }
        turnLeft();
        // Serial.println(9);
        // delay(1000);
        readSensors();
        // printValues();
        if (val1 > threshold && val2 > threshold && val3 > threshold && val4 < threshold && val5 > threshold) {
          turnLeft();
          delay(400);
          stopMotors();
          Serial.println(10);
          delay(2000);
          isTurnLeft = true;
          isLineFollowing = false;
        }
      }

      while (!isLineFollowing) {
        pidControl();
        // printValues();
        // Serial.println(11);
        if (val1 > threshold && val2 > threshold && val3 > threshold && val4 > threshold && val5 > threshold)
        // || (val1 > threshold && val2 > threshold)
        // || (val4 > threshold && val5 > threshold))
        {
          isLineFollowing = true;
          // isRotate = false;
          isLifting = false;
          // break;
        }
      }

      while (!isLifting) {
        stopMotors();
        delay(1000);
        lifting.writeMicroseconds(1600);
        delay(2000);
        lifting.writeMicroseconds(1500);
        delay(2000);
        isLifting = true;
        room = 13;
      }

      // while (!isRotate) {
      //   // turnLeft();
      //   rotateCCW();
      //   // pidControl2();
      //   // Serial.println(12);
      //   readSensors();
      //   // delay(2000);
      //   // printValues();
      //   if (val1 > threshold && val2 > threshold && val3 < threshold && val4 > threshold && val5 > threshold) {
      //     // stopMotors();
      //     // Serial.println(13);
      //     // delay(2000);
      //     rotateCCW();
      //     delay(125);
      //     stopMotors();
      //     delay(5000);
      //     isRotate = true;
      //     // isLineFollowing = false;
      //     room = 13;
      //     // break;
      //   }
      // }

    case 13:
      stopMotors();
  }
}

// void loop() {
//   pidControl();
// }