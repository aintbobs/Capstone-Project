#include <LiquidCrystal_I2C.h>
#include <HX711.h>

// Define constants
const int lcdAddr = 0x27;
const int lcdSda = A4;
const int lcdScl = A5;
const int stepPin1 = 2;
const int dirPin1 = 3;
const int stepPin2 = 4;
const int dirPin2 = 5;
const int stepPin3 = 6;
const int dirPin3 = 7;
const int LOADCELL_DOUT = 27;
const int LOADCELL_SCK = 28;
const int in1 = 30;
const int in2 = 31;
const int en = 29;
const int ledRedPin = 25;
const int ledGreenPin = 22;
const int ledWhitePin = 23;
const int ledBluePin = 24;
const int buzzerPin = 26;
const int TRIG_PIN1 = 34; //yellow
const int ECHO_PIN1 = 35; //orange
const int TRIG_PIN2 = 36;
const int ECHO_PIN2 = 37;
const int RELAY_PIN1 = 32;
const int RELAY_PIN2 = 33;
const float calibrationFactor = 0.000001;
const float offset = 0.02;


// Initialize objects
LiquidCrystal_I2C lcd(lcdAddr, lcdSda, lcdScl);
HX711 scale;

bool machineOpen = false;
bool stepperRunning1 = false;
bool stepperRunning2 = false;
bool stepperRunning3 = false;
bool actuatorExtended = false;
unsigned long actuatorExtendTime = 0;
float duration_us, distance_cm;

void setup() {
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(dirPin3, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(en, OUTPUT);
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledWhitePin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);

  digitalWrite(ledGreenPin, HIGH); // Power on indicator
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 1);
  lcd.print("Initializing System.");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("WELCOME USER!                ");
  delay(2000);
  lcd.clear();

  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT, LOADCELL_SCK);
  scale.set_gain(128);
  scale.tare();


  stepperRunning1 = true; // Start stepper 1
}

void loop() {
  if (stepperRunning1) {
    rotateClockwise1(360);
    delay(1);
 
}
  if (stepperRunning2) {
    rotateClockwise2(360);
    delay(1);
    rotateClockwise2(360);
    delay(1);
  }
  if (stepperRunning3) {
    rotateClockwise3(360);
    delay(1);
  }
  stopStepper2();
  stopStepper3();
  stopStepper1();


  long reading = scale.read();
  float weight = (reading * calibrationFactor) - offset;

  if (weight <= 0.63) {
    weight = 0;
    digitalWrite(ledRedPin, HIGH);
    //tone(buzzerPin, 1000);
    

  } else {
    digitalWrite(ledRedPin, LOW);
    noTone(buzzerPin);
    stepperRunning1 = true;
    
  }

  lcd.setCursor(0, 1);
  lcd.println("Grinding.........");
  lcd.setCursor(0, 2);
  lcd.print("Weight: ");
  lcd.print(weight);
  lcd.print("kg   ");

  if (weight >= 1.00 && stepperRunning1) {
    stopStepper1();
    linearActuatorExtend();
    actuatorExtendTime = millis();
    actuatorExtended = true;
stepperRunning1 = false;
noTone(buzzerPin);
  }

  if (actuatorExtended && millis() - actuatorExtendTime >= 6000) {
    linearActuatorRetract();
    stopStepper1();
    actuatorExtended = false;
    digitalWrite(RELAY_PIN1, HIGH);
    delayMicroseconds(5000);
    lcd.setCursor(0, 1);
    lcd.println("Dispensing Starch...");
    digitalWrite(RELAY_PIN2, HIGH);
    delayMicroseconds(5000);
    lcd.setCursor(0, 2);
    lcd.println("Dispensing Water...");
      stepperRunning2 = true;
    delay(1000);
    stepperRunning3 = true;
    delay(1000);
    
  
  }

  if (digitalRead(RELAY_PIN1) == HIGH) {
    digitalWrite(TRIG_PIN1, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN1, LOW);
    long duration_us = pulseIn(ECHO_PIN1, HIGH);
    float distance_cm = duration_us * 0.034 / 2;
    stopStepper1();

    if (distance_cm < 30) {
      digitalWrite(RELAY_PIN1, LOW);
      digitalWrite(ledWhitePin, HIGH);
      //tone(buzzerPin, 1000);
    } else {
      digitalWrite(ledWhitePin, LOW);
      noTone(buzzerPin);
      stepperRunning2 = true;
      stopStepper1();
      
    }
  }
  if (digitalRead(RELAY_PIN2) == HIGH) {
    digitalWrite(TRIG_PIN2, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN2, LOW);
    long duration_us = pulseIn(ECHO_PIN2, HIGH);
    float distance_cm = duration_us * 0.034 / 2;
    stopStepper1();

    if (distance_cm < 13) {
      digitalWrite(RELAY_PIN2, LOW);
      digitalWrite(ledBluePin, HIGH);
      //tone(buzzerPin, 1000);
    } else {
      digitalWrite(ledBluePin, LOW);
      noTone(buzzerPin);
      stepperRunning3 = true;
      stopStepper1();
    }
  }

  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  delay(1);
}

void rotateClockwise1(int degrees) {
  digitalWrite(dirPin1, HIGH);
  int stepsPerRevolution = 1600;
  int steps = map(degrees, 0, 360, 0, stepsPerRevolution);
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin1, HIGH);
    delayMicroseconds(200);
    digitalWrite(stepPin1, LOW);
    delayMicroseconds(500);
  }
}
void rotateClockwise2(int degrees) {
digitalWrite(dirPin2, HIGH);
int stepsPerRevolution = 1600;
int steps = map(degrees, 0, 360, 0, stepsPerRevolution);
for (int i = 0; i < steps; i++) {
digitalWrite(stepPin2, HIGH);
delayMicroseconds(200);
digitalWrite(stepPin2, LOW);
delayMicroseconds(500);
}
}
void rotateClockwise3(int degrees) {
digitalWrite(dirPin3, HIGH);
int stepsPerRevolution = 1600;
int steps = map(degrees, 0, 360, 0, stepsPerRevolution);
for (int i = 0; i < steps; i++) {
digitalWrite(stepPin3, HIGH);
delayMicroseconds(200);
digitalWrite(stepPin3, LOW);
delayMicroseconds(500);
}
}

void linearActuatorExtend() {
digitalWrite(in1, HIGH);
digitalWrite(in2, LOW);
analogWrite(en, 200);
delay(6000);
}

void linearActuatorRetract() {
digitalWrite(in1, LOW);
digitalWrite(in2, HIGH);
analogWrite(en, 200);
delay(5000);
digitalWrite(in1, LOW);
digitalWrite(in2, LOW);
analogWrite(en, 0);
stopStepper1();
}

void stopStepper1() {
digitalWrite(stepPin1, LOW);
digitalWrite(dirPin1, LOW);
}

void stopStepper2() {
digitalWrite(stepPin2, LOW);
digitalWrite(dirPin2, LOW);
}
void stopStepper3() {
digitalWrite(stepPin3, LOW);
digitalWrite(dirPin3, LOW);
}