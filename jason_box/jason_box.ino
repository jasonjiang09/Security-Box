/*
 * Jason's Box of Death
 * by Tanner Hornsby
 */
#include <NewPing.h>
#include <Servo.h>
#define SerialMonitor

#define TRIGGER_PIN  10  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


const int buttonPin = 2;    // the number of the pushbutton pin
bool button = LOW;

// Variables will change:
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers


const int out=10;
const int in=11;
int pos = 90;
unsigned long currentMillis = 0;
//servo time variables
int servoPosition = 0;     // the current angle of the servo - starting at 90.
int servoFastInterval = 10;
int servoInterval = 10; // initial millisecs between servo moves
int servoDegrees = 360;       // amount servo moves at each step 
                            //    will be changed to negative value for movement in the other direction
unsigned long previousServoMillis = 0;
unsigned long int previousTopMillis = 0;
unsigned long int intervalLong = 500;
unsigned long int intervalTotal= 700;
unsigned long int interval;
unsigned long int previousClosedTopMillis = 0;

Servo servo_9;
Servo servo_6;

void setup()
{
   pinMode(buttonPin, INPUT);

  // set initial LED state
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(9600);
  pinMode(out,OUTPUT);
  pinMode(in,INPUT);
  
  servo_9.attach(9);
  servo_6.attach(6);
  servo_6.write(45);
  servo_9.write(45);
}

void loop() {

  currentMillis = millis();
  interval = currentMillis - previousTopMillis;
  //debounceButton();
  button = digitalRead(buttonPin);
  boxTop();
  protector(button);
}
void boxTop() {

    if(button == HIGH) {
      servo_9.write(180);
    }else{
      servo_9.write(40);
    }
 }
void protector(bool check) {
  if(check == LOW) {
    if (currentMillis - previousServoMillis >= servoInterval) {
     servo_6.write(servoPosition);
     mservoPosition += servoDegrees;
     
     previousServoMillis += servoInterval;
    }
  }
  else {
    servo_6.write(180);
  }
}


/*

#include <LiquidCrystal.h>
 
#define trigger 18
#define echo 19
 
LiquidCrystal lcd(2,3,4,5,6,7);
 
float time=0,distance=0;
 
void setup()
{
 lcd.begin(16,2);
 pinMode(trigger,OUTPUT);
 pinMode(echo,INPUT);
 lcd.print(" Ultra sonic");
 lcd.setCursor(0,1);
 lcd.print("Distance Meter");
 delay(2000);
 lcd.clear();
 lcd.print(" Circuit Digest");
 delay(2000);
}
 
void loop()
{
 lcd.clear();
 digitalWrite(trigger,LOW);
 delayMicroseconds(2);
 digitalWrite(trigger,HIGH);
 delayMicroseconds(10);
 digitalWrite(trigger,LOW);
 delayMicroseconds(2);
 time=pulseIn(echo,HIGH);
 distance=time*340/20000;
 lcd.clear();
 lcd.print("Distance:");
 lcd.print(distance);
 lcd.print("cm");
 lcd.setCursor(0,1);
 lcd.print("Distance:");
 lcd.print(distance/100);
 lcd.print("m");
 delay(1000);
}


*/
