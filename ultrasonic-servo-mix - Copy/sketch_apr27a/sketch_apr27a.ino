//SERVO
#include <Servo.h>
Servo servo1; //one at tower WHITE CABLE
Servo servo2; //one at garden BLUE CABLE
int DELAY = 1500;
int loop_DELAY = 20;
int s1_garden = 90; //WITH WHILE LOOP GETTIN IT TO 180 DEGREES
int s1_tower = 180; //WITH WHILE LOOP GETTING IT TO 90 DEGREES
int s2_hold = 90; 
int s2_letGo = 180;

//HCSR-04
int triggerPin = 12;
int echoPin = 11;
int pingTravelTime;


//autoRotate
Servo servo3;
int iniServo3pos = 0;
int servo3pin = 7;
int pos;
int flagRotate = 1;



void setup() {
  //SERVO
  servo1.attach(10);
  servo2.attach(9);
  servo1.write(180);
  Serial.begin(9600);

  //HCSR-04
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  //autoRotate

  servo3.attach(servo3pin);
  pos = iniServo3pos;
  servo3.write(pos);
  
}

void loop() {
  
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(10);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pingTravelTime = pulseIn(echoPin, HIGH);
  delay(loop_DELAY);
//  Serial.println(pingTravelTime);
  Serial.println(pos);
  Serial.println(pingTravelTime);

//  autorotate -START-

  if (flagRotate == 1){
    if(pos < 180){
      pos += 5;
      servo3.write(pos);
    }
    else{
      flagRotate = 0;
      }
  }
  else if(flagRotate == 0){
    if(pos>0){
      pos -= 5;
      servo3.write(pos);
      }
     else{flagRotate = 1;}
    }

  
//  autorotate -END-
  
  if(pingTravelTime <= 1500){
    while (s1_tower != 90){
      s1_tower--;
      servo1.write(s1_tower);
      delay(loop_DELAY);
      }
    delay(DELAY);
    servo2.write(s2_letGo);
    delay(DELAY);
    while (s1_garden != 180){
      s1_garden++;
      servo1.write(s1_garden);
      delay(loop_DELAY);
    }
    delay(DELAY);
    servo2.write(s2_hold);
    delay(DELAY);
    s1_tower = 180;
    s1_garden = 90;
  }
}
