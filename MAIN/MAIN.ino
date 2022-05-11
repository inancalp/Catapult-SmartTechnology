#include <IRremote.h>
#include <Servo.h>

Servo servo1; //one at tower WHITE CABLE
Servo servo2; //one at garden BLUE CABLE
Servo servo3;

int IRpin = 8;
IRrecv IR(IRpin);
decode_results cmd;
String Mode = "";
String manualMode;

int DELAY = 500;
int loop_DELAY = 20;
int s1_garden; //WITH WHILE LOOP GETTIN IT TO 180 DEGREES
int s1_tower; //WITH WHILE LOOP GETTING IT TO 90 DEGREES
int s2_hold; 
int s2_letGo;

//HCSR-04
int triggerPin = 12;
int echoPin = 11;
int pingTravelTime;


//SERVO3 N ROTATION

String flagResult = "FIRST";
int servo3pin = 7; //servo3 pin
int iniServo3pos = 90;
int pos;//initial servo3 postion
int OK = 0; // 0 -> manual && 1 -> auto;
unsigned long lastCode; //last HEX code received from the remote


void setup() {
  
  servo1.attach(10);
  servo2.attach(9);
  servo3.attach(7);
  servo1.write(180);


  //SERVO3 SETUP
  pos = iniServo3pos;
  servo3.write(pos);
  //SERVO3SETUP
  Serial.begin(9600);
  IR.enableIRIn();


  //HCSR-04
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  s1_garden = 90;
  s1_tower = 180; 
  s2_hold = 90; 
  s2_letGo = 180; 
}

void loop() {

 
  
  Serial.println("Press 'hashtag' Button to continue in manual");
  Serial.println("Press 'star' Button to continue in automatic");
  
  while (IR.decode(&cmd) == 0){}

  Serial.println("FIRST PORTION:");
  Serial.println(cmd.value, HEX);
  delay(DELAY);
  IR.resume();
  
//  if(cmd.value == 0xFF22DD){
//    manualMode="left";
//    Serial.print("manualMode State -> ");
//    Serial.println(manualMode);
//  }
//  if(cmd.value == 0xFFC23D){
//    manualMode="right";
//    Serial.print("manualMode State -> ");
//    Serial.println(manualMode);
//  }
  

  if(cmd.value == 0xFF42BD)
  {
    Mode="star";
    Serial.print("Mode State -> ");
    Serial.println(Mode);
  }
  if(cmd.value == 0xFF52AD)
  {
    Mode="hashtag";
    Serial.print("Mode State -> ");
    Serial.println(Mode);
  }

//  ----------------------------------
//  ----------------------------------

      //AUTO    
    
   if (Mode == "star"){
    while(Mode == "star"){
       s1_garden = 90;
       s1_tower = 180; 
       s2_hold = 90; 
       s2_letGo = 180; 
      digitalWrite(triggerPin, LOW);
      delayMicroseconds(10);
      digitalWrite(triggerPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(triggerPin, LOW);
      pingTravelTime = pulseIn(echoPin, HIGH);
      delay(loop_DELAY);
      Serial.println(pingTravelTime);
      
      if(pingTravelTime <= 1500 && pingTravelTime >= 0){
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
        Mode = "";
      } 
    }  
   }


   //MANUAL

   if (Mode == "hashtag"){
    
            while (IR.decode(&cmd) == 0){}
            Serial.println("Second Portion:");
            Serial.println(cmd.value, HEX);
            delay(DELAY);
            IR.resume();

  if(cmd.value == 0xFF02FD)
            {
              manualMode="ok";
              Serial.print("manualMode State -> ");
              Serial.println(manualMode);
            }
            
// rotation comes here!   -BEG- 
   
            
   while(manualMode != "ok"){
    
    while (IR.decode(&cmd) == 0){}
    Serial.println(cmd.value, HEX);

    if(cmd.value == 0xFF02FD)
            {
              manualMode="ok";
              Serial.print("manualMode State -> ");
              Serial.println(manualMode);
              delay(DELAY);
              IR.resume();
            }
            
    if(IR.decode(&cmd)){
      Serial.print("flagResult value = ");
      Serial.println(flagResult);
      Serial.print("Result value = ");
      Serial.println(cmd.value);
      
      if(cmd.value == 0xFFFFFFFF){
          cmd.value = lastCode;
      }
  
      if(cmd.value == 0xFF22DD){
          lastCode = cmd.value;
          pos+=3;
          if(pos > 180){pos = 180;}
          servo3.write(pos); 
      }

      if(cmd.value == 0xFFC23D){
          lastCode = cmd.value;
          pos-=3;
          if(pos<0){pos = 0;}
          servo3.write(pos); 
        } 

       flagResult = (cmd.value, HEX);
       delay(1);
       IR.resume();
   }
    }

            
// rotation comes here!      -END-
        

            
           if (Mode == "hashtag" && manualMode == "ok")
           {    
             s1_garden = 90;
             s1_tower = 180; 
             s2_hold = 90; 
             s2_letGo = 180;    
              
             while (s1_tower != 90)
             {
              s1_tower--;
              servo1.write(s1_tower);
              delay(loop_DELAY);
             }  
              
              delay(DELAY);    
              servo2.write(s2_letGo);  
              delay(DELAY); 
              
              while (s1_garden != 180)
              {
                s1_garden++;
                servo1.write(s1_garden);
                delay(loop_DELAY);
              }
              
             delay(DELAY);
             servo2.write(s2_hold);
             delay(DELAY);
             manualMode = "";
           }
    
    }
   
  }  
