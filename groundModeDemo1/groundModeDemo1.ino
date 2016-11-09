#include <Servo.h>

boolean groundMode = true;

int modePin = 0;
int u1Pin = A0;  //forward/backward (vertical) axel-1
int u2Pin = A1;  //left-right (horizontal) axel-1
//int u3Pin = 4;  //top left - bottom right axel-2
//int u4Pin = 5;  //top right - bottom left axel-2


Servo groundLeft;  //leftmost motor on tracks
Servo groundRight;  //rightmost motor on tracks

//Servo airTop;     //topmost motor on q-copter
//Servo airBottom;  //bottommost motor on q-copter
//Servo airLeft;    //leftmost motor on q-copter
//Servo airRight;   //rightmost motor on q-copter

void setup() {
  // put your setup code here, to run once:

  //configure one of the switches on the RF controller to
  //groundMode or flightMode
  pinMode(modePin, INPUT);

  //attach the ground motors to the appropriate pins
  groundLeft.attach(9);
  groundRight.attach(10);

  //attach the flight motors to the appropriate pins 
//  airTop.attach(6);
//  airBottom.attach(9);
//  airLeft.attach(10);
//  airRight.attach(11);
  
}

//not needed since RC motors are bi-directional
//void setMotorDirection(int verSpeed, int horSpeed){
//  if(verSpeed > 128){
//    groundModeForwardBackward(horSpeed);
//  } else{
//    
//  }
//}

void groundModeForward(int u1_Speed, int u2_Speed){
  
  if (u1_Speed == u2_Speed)  { //forward linearly
    //apply the same speed to both motors to move forward
    groundLeft.write(u1_Speed);
    groundRight.write(u2_Speed);
  } else {

    if(u2_Speed < 86) { //forward-left
      //to turn left, leftMotor needs to reverse
      //and rightMotor needs to move forward

      //we always apply the greater of the two
      //speeds, u1 & u2, to the rightMotor and
      //the lower of the two speeds to the leftMotor
      
      if (u1_Speed < u2_Speed) {

        //u2 is slower than u1
        //so we reverse the polarity of u2
        //and apply it to the leftMotor
        
        u2_Speed = map(u2_Speed, 0, 180, 180, 0);
      
        groundLeft.write(u2_Speed);   //lower speed
        groundRight.write(u1_Speed);  //higher speed
        
      } else if(u1_Speed > u2_Speed) { //do the opposite

        //u1 is slower than u2
        u1_Speed = map(u1_Speed, 0, 180, 180, 0);
      
        groundLeft.write(u1_Speed);   //lower speed
        groundRight.write(u2_Speed);  //higher speed
        
      }
    } else if (u2_Speed > 86) { //forward-right
      //to turn right, rightMotor needs to reverse 
      //and leftMotor needs to move forward

      //we always apply the greater of the two
      //speeds, u1 & u2, to the leftMotor and
      //the lower of the two speeds to the rightMotor

      //get the equivalent value of u2_Speed between 0 and 90
      int temp_u2_Speed = map(u2_Speed, 0, 180, 180, 0);
      
      if(u1_Speed > temp_u2_Speed) { //u1 is slower than u2

        //get the equivalent value of u1_Speed between 90 and 180
        int temp_u1_Speed = map(u1_Speed, 0, 180, 180, 0);
        
        groundLeft.write(temp_u2_Speed);  //forward
        groundRight.write(temp_u1_Speed); //backward
      } else if(u1_Speed < temp_u2_Speed) {  //u1 is faster than u2
        groundLeft.write(u1_Speed);
        groundRight.write(u2_Speed);
      }
      
    } 
  }
  
}

void groundModeReverse(int u1_Speed, int u2_Speed){

  if (u1_Speed == u2_Speed) {//reverse linearly
    //apply the same speed to both motors to move forward, only
    groundLeft.write(u1_Speed);
    groundRight.write(u2_Speed);
  } else {
    
    if(u2_Speed < 86) { //reverse-left
      //to reverse to the left, both motors are going
      //move in reverse but the magnitude of rightMotor has
      //to be greater than that of leftMotor 

      //reverse the direction of u2_Speed, since
      //it is currently less than 90
      u2_Speed = map(u2_Speed, 0, 180, 180, 0);

      //apply the greater speed to rightMotor
      if(u2_Speed > u1_Speed) {
         groundLeft.write(u1_Speed);
         groundRight.write(u2_Speed);
      } else {
        groundLeft.write(u1_Speed);
        groundRight.write(u2_Speed);
      }
    } else if (u2_Speed > 86) { //reverse-right
      //to reverse to the right, both motors are going
      //move in reverse but the magnitude of leftMotor has
      //to be greater than that of rightMotor 

      //both u1 and u2 are greater than 90,
      //so they are already moving in reverse

      //apply the greater speed to leftMotor
      if(u1_Speed > u2_Speed) {
         groundLeft.write(u1_Speed);
         groundRight.write(u2_Speed);
      } else {
        groundLeft.write(u2_Speed);
        groundRight.write(u1_Speed);
      }
      
    }
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(groundMode) {  //ground system code will be executed
   
   int u1_Speed = analogRead(u1Pin);
   int u2_Speed = analogRead(u2Pin);

   //map values to angle of rotation;
   //we can also use PWM to interact with the motors
   u1_Speed = map(u1_Speed, 0, 1023, 0, 180);
   u2_Speed = map(u2_Speed, 0, 1023, 0, 180);

   //delay(1000);
   Serial.println("u1_Speed: " + u1_Speed);
   Serial.println("u2_Speed: " + u2_Speed);
  
   //0 is full-speed of the motor, in one direction
   //180 is full-speed of the motor, in the opposite direction
   //90 is no motion
   
   if(u1_Speed < 81 && u2_Speed == 86) { //just forward motion
    
      //write the same speed to both motors
      u2_Speed = u1_Speed;
      groundModeForward(u1_Speed, u2_Speed);
   } else if (u1_Speed > 81 && u2_Speed == 86) { //just reverse motion

      //write the same speed to both motors
      u2_Speed = u1_Speed;
      groundModeReverse(u1_Speed, u2_Speed);
   } else if (u1_Speed != 81 && u2_Speed != 86) { //every other combination (in the four quadrants)
      if(u1_Speed < 90) { //forward motion in left or right
        groundModeForward(u1_Speed, u2_Speed);
      } else if (u1_Speed > 81) { //backward motion in left or right
        groundModeReverse(u1_Speed, u2_Speed);
      }
   } else if (u1_Speed == 81) { //no motion or rotate in place

      if(u2_Speed != 86) { //turn in place, that is, rotate about a fixed point
        
      } else if(u2_Speed == 86) { //no motion at all
        
        //we can do nothing here, or we can write a speed of 90
        //to either of the two motion functions
      }
   }
   
   //check to see if mode has changed
   groundMode = digitalRead(modePin);
  } else { //flight system code will be executed

    //check to see if mode has changed
    groundMode = digitalRead(modePin);
  }

}
