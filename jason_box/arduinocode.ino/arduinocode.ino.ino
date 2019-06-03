int lidNum;
char input;
void setup() {
  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0) {
    input = Serial.read();
    Serial.print(input);
    //Serial.print("hello");
    //Serial.println(input);
   // lidNum = Serial.parseInt(); //1==Jason, 2 == Tanner, 3 == env
    //Serial.println(lidNum);      
    
  }
}
