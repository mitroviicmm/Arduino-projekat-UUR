#include <AFMotor.h>
#include <Servo.h>

// Pretpostavka: M3 = levi točak, M4 = desni točak
AF_DCMotor motorL(3);   // M3
AF_DCMotor motorR(4);   // M4

Servo servo1;           // servo na koji je zakacen ultrazvuk

// ---------------- Podesivi parametri ----------------
const int SPEED_FWD   = 180;          // brzina za vožnju pravo (0-255)
const int SPEED_TURN  = 180;          // brzina za okret

const unsigned long SIDE_TIME = 2000; // ms po stranici kvadrata
const unsigned long TURN_TIME = 450;  // ms za ~90° (doteraj po potrebi)

const int OBSTACLE_CM = 25;           // prag prepreke (cm)
const unsigned long PING_INTERVAL = 80; // ms izmedju merenja tokom voznje
const unsigned long CLEAR_MOVE_TIME = 400; // ms malo napred posle izbegavanja

// Servo (skener) parametri
const int SERVO_PIN     = 9;
const int SERVO_CENTER  = 90;
const int SERVO_SCAN    = 45;         // skeniranje ±45°

// Ultrazvuk pinovi (NAPOMENA: sa Motor Shield v1 D7/D8 konfliktuje!)
const int TRIG_PIN = 2;   // D7 - TRIG
const int ECHO_PIN = 13;   // D8 - ECHO

// Interno
unsigned long lastPing = 0;

// ---------------- Setup ----------------
void setup() {
  Serial.begin(9600);
  Serial.println("Start: kvadrat + detekcija prepreka tokom voznje...");

  motorL.setSpeed(SPEED_FWD);
  motorR.setSpeed(SPEED_FWD);

  servo1.attach(SERVO_PIN);
  servo1.write(SERVO_CENTER);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  delay(500);
}

// ---------------- Loop ----------------
void loop() {
  drawSquareWithObstacleRestart();
  // Loop se vrti dalje i crta sledeci kvadrat
}

void drawSquareWithObstacleRestart() {
  Serial.println("Start kvadrata sa proverom prepreka...");

  int side = 0;
  while (side < 4) {
    Serial.print("Stranica ");
    Serial.println(side + 1);

    bool ok = driveSideWithObstacleCheck(SIDE_TIME);

    if (!ok) {
      // prepreka detektovana usred stranice -> izbegni i restartuj kvadrat
      avoidAndReposition();
      Serial.println("Restart kvadrata od pocetka!");
      side = 0;
      continue;
    }

    // Stranica uspesno zavrsena -> skretanje udesno
    Serial.println("Skretanje udesno...");
    turnRight90();
    side++;
  }

  Serial.println("Kvadrat zavrsen -> mahanje...");
  // waveFlag();
}

bool driveSideWithObstacleCheck(unsigned long sideTimeMs) {
  // Vozi jednu stranicu i stalno proverava prepreku.
  // true = zavrsena stranica bez prepreke
  // false = detektovana prepreka

  unsigned long start = millis();
  goForward();

  while (millis() - start < sideTimeMs) {
    if (millis() - lastPing >= PING_INTERVAL) {
      lastPing = millis();

      int d = readDistanceCM();
      if (d == 0) d = 999; // nema eha -> tretiraj kao daleko

      // Debug (mozes da iskljucis ako smara)
      // Serial.print("Front cm: ");
      // Serial.println(d);

      if (d <= OBSTACLE_CM) {
        stopMotors();
        delay(150);
        return false;
      }
    }
  }

  stopMotors();
  delay(200);
  return true;
}

void avoidAndReposition() {
  // Skenira levo/desno, bira slobodniju putanju, okrece se i malo ide napred.

  Serial.println("Obstacle tokom stranice -> skeniram levo/desno...");

  stopMotors();
  delay(150);

  int dLeft  = scanLeft();
  int dRight = scanRight();

  Serial.print("Left cm: ");
  Serial.println(dLeft);
  Serial.print("Right cm: ");
  Serial.println(dRight);

  servo1.write(SERVO_CENTER);
  delay(150);

  bool leftClear  = (dLeft == 0)  || (dLeft > OBSTACLE_CM);
  bool rightClear = (dRight == 0) || (dRight > OBSTACLE_CM);

  if (leftClear && !rightClear) {
    Serial.println("Biramo LEVO.");
    turnLeft90();
  } else if (rightClear && !leftClear) {
    Serial.println("Biramo DESNO.");
    turnRight90();
  } else if (leftClear && rightClear) {
    int dl = (dLeft == 0) ? 999 : dLeft;
    int dr = (dRight == 0) ? 999 : dRight;

    if (dl >= dr) {
      Serial.println("Obe strane ok -> LEVO (vise prostora).");
      turnLeft90();
    } else {
      Serial.println("Obe strane ok -> DESNO (vise prostora).");
      turnRight90();
    }
  } else {
    Serial.println("Levo/desno blokirano -> fallback DESNO.");
    turnRight90();
  }

  // Malo napred da se odmakne od prepreke
  goForward();
  delay(CLEAR_MOVE_TIME);
  stopMotors();
  delay(200);
}

int readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Timeout 30ms (~5m)
  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000UL);
  if (duration == 0) return 0; // nema eha

  int cm = (int)(duration / 58UL);
  return cm;
}

int scanLeft() {
  int leftPos = SERVO_CENTER - SERVO_SCAN;
  if (leftPos < 0) leftPos = 0;

  servo1.write(leftPos);
  delay(350);
  return readDistanceCM();
}

int scanRight() {
  int rightPos = SERVO_CENTER + SERVO_SCAN;
  if (rightPos > 180) rightPos = 180;

  servo1.write(rightPos);
  delay(350);
  return readDistanceCM();
}


void goForward() {
  motorL.setSpeed(SPEED_FWD);
  motorR.setSpeed(SPEED_FWD);

  motorL.run(FORWARD);
  motorR.run(FORWARD);
}

void goBackward() {
  motorL.setSpeed(SPEED_FWD);
  motorR.setSpeed(SPEED_FWD);

  motorL.run(BACKWARD);
  motorR.run(BACKWARD);
}

void turnRight() {
  // Okret na mestu udesno: levi napred, desni nazad
  motorL.setSpeed(SPEED_TURN);
  motorR.setSpeed(SPEED_TURN);

  motorL.run(FORWARD);
  motorR.run(BACKWARD);
}

void turnLeft() {
  // Okret na mestu ulevo: levi nazad, desni napred
  motorL.setSpeed(SPEED_TURN);
  motorR.setSpeed(SPEED_TURN);

  motorL.run(BACKWARD);
  motorR.run(FORWARD);
}

void turnRight90() {
  turnRight();
  delay(TURN_TIME);
  stopMotors();
  delay(200);
}

void turnLeft90() {
  turnLeft();
  delay(TURN_TIME);
  stopMotors();
  delay(200);
}

void stopMotors() {
  motorL.run(RELEASE);
  motorR.run(RELEASE);
}

void waveFlag() {
  int leftPos  = SERVO_CENTER - 40;
  int rightPos = SERVO_CENTER + 40;

  servo1.write(SERVO_CENTER);
  delay(200);

  for (int i = 0; i < 3; i++) {
    servo1.write(leftPos);
    delay(250);
    servo1.write(rightPos);
    delay(250);
  }

  servo1.write(SERVO_CENTER);
  delay(400);
}
