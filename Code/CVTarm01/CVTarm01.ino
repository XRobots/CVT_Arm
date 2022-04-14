// Main arm code comparing pots and driving servos

#include <Servo.h>
Servo myservo1; 
Servo myservo2; 
Servo myservo3; 

float pot1;
float pot2;
float pot3;
float pot4;
float pot5;
float pot6;
float pot1Filtered;
float pot2Filtered;
float pot3Filtered;
float pot4Filtered;
float pot5Filtered;
float pot6Filtered;
int axis1Diff;
int axis2Diff;
int axis3Diff;

int servoL;
int servoR;

void setup() {
  myservo1.attach(7);   // waist
  myservo2.attach(6);   // left hand  
  myservo3.attach(5);   // right hand
  pinMode(A0, INPUT);   // demand
  pinMode(A1, INPUT);   // waist axis
  pinMode(A2, INPUT);   // demand
  pinMode(A3, INPUT);   // shoulder axis
  pinMode(A4, INPUT);   // demand
  pinMode(A5, INPUT);   // elbow axis
  Serial.begin(115200);
}

void loop() {

  pot1 = analogRead(A0);        // waist demand
  pot2 = analogRead(A1);        // waist axis position

  pot3 = analogRead(A2);        // shoulder demand
  pot4 = analogRead(A3);        // shoulder axis position

  pot5 = analogRead(A4);        // elbow demand
  pot6 = analogRead(A5);        // elbow axis position
 
  pot1 = map(pot1,0,1023,-512,512);         // scale pots - waist
  pot2 = map(pot2,0,1023,-512,512);

  pot3 = map(pot3,0,1023,-512,512);         // scale pots - shoulder
  pot4 = map(pot4,0,1023,-512,512);

  pot5 = map(pot5,0,1023,-512,512);         // scale pots - elbow
  pot6 = map(pot6,0,1023,-512,512);

  pot1Filtered = filter(pot1, pot1Filtered, 15);    // filter for noise
  pot2Filtered = filter(pot2, pot2Filtered, 15);

  pot3Filtered = filter(pot3, pot3Filtered, 15);    // filter for noise
  pot4Filtered = filter(pot4, pot4Filtered, 15);

  pot5Filtered = filter(pot5, pot5Filtered, 15);    // filter for noise
  pot6Filtered = filter(pot6, pot6Filtered, 15);
  
  axis1Diff = (pot2Filtered-pot1Filtered)*-3;       // work out the difference and scale up for more gain
  axis2Diff = (pot4Filtered-pot3Filtered)*-2;       // work out the difference and scale up for more gain
  axis3Diff = (pot6Filtered-pot5Filtered)*-2;       // work out the difference and scale up for more gain

  servoL = axis2Diff - axis3Diff;
  servoR = axis2Diff + axis3Diff;

  axis1Diff = map(axis1Diff,-512,512,800,2400);     // scale outout for driving servos
  axis1Diff = constrain(axis1Diff,800,2400);        // constrain output so we keep to the physical limits of the servo

  servoL = map(servoL,-512,512,800,2400);     // scale outout for driving servos
  servoL = constrain(servoL,800,2400);        // constrain output so we keep to the physical limits of the servo

  servoR = map(servoR,-512,512,800,2400);     // scale outout for driving servos
  servoR = constrain(servoR,800,2400);        // constrain output so we keep to the physical limits of the servo

  Serial.print(axis2Diff);
  Serial.print(" , ");

  Serial.print(axis3Diff);
  Serial.print(" , ");

  Serial.print(servoL);
  Serial.print(" , ");
  Serial.println(servoR);

  myservo1.writeMicroseconds(axis1Diff);             // drive servo
  myservo2.writeMicroseconds(servoL);             // drive servo LEFT
  myservo3.writeMicroseconds(servoR);             // drive servo RIGHT

  delay(10);

}

// motion filter to filter noise from pots

float filter(float prevValue, float currentValue, int filter) {  
  float lengthFiltered =  (prevValue + (currentValue * filter)) / (filter + 1);  
  return lengthFiltered;  
}
