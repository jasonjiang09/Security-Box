/*
 * Jason's Box of Death
 * by Tanner Hornsby
 */
//including neccesary libraries
#include <Servo.h>
#define SerialMonitor
#include <IRremote.h>

int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

unsigned long openUp = 0xFF02FD;
unsigned long closeDown = 0xFFA25D;

//button variables
const int buttonPin = 2;    // the number of the pushbutton pin
bool button = LOW;


const int out=10;
const int in=11;
int pos = 90;
unsigned long currentMillis = 0;
//servo time variables
int servoPosition = 110;     // the current angle of the servo - starting at 45.
int servoFastInterval = 0;
int servoInterval = 100; // initial millisecs between servo moves
int servoDegrees = 50;       // amount servo moves at each step 
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
   Serial.begin(9600);
  
  pinMode(buttonPin, INPUT);

  // set initial LED state
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  pinMode(out,OUTPUT);
  pinMode(in,INPUT);

  irrecv.enableIRIn(); // Start the receiver

  //servo
  servo_9.attach(9);
  servo_6.attach(6);
  servo_6.write(110);
  servo_9.write(45);
}

void loop() {

  currentMillis = millis();
  interval = currentMillis - previousTopMillis;
  //debounceButton();
  //button = digitalRead(buttonPin);
  boxTop();
  protector(button);
  
  if (irrecv.decode(&results)) {
   // Serial.println(results.value, HEX);
    if(results.value == openUp ) {
      button = HIGH;
    }
    if(results.value == closeDown) {
      button = LOW;
    }

    irrecv.resume(); // Receive the next value
  }
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
     servoPosition -= servoDegrees;


     if(servoPosition >=360 || servoPosition < -90) {
      servoPosition = 110;
      servo_6.write(servoPosition);
     }


     previousServoMillis += servoInterval;
    }
  }
  else {
    servo_6.write(180);
  }
}
