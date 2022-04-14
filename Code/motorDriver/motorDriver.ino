// drive brushless motor from pot input using servo library

#include <Servo.h>
Servo myservo; 

int pot;

void setup() {

  myservo.attach(7); 
  pinMode(A0, INPUT);
  Serial.begin(115200);

}

void loop() {

  pot = analogRead(A0);
  pot = map(pot,0,1023,1500,2500);
  Serial.println(pot);

  myservo.write(pot); 


}
