#include <Servo.h>

int x_dir; //value from pin A0
int y_dir; //value from pin A1
Servo servo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  delay(1000);
  
  x_dir = analogRead(A0);
  x_dir = map(x_dir, 0, 1023, 0, 180);

  servo.write(x_dir);
  
  y_dir = analogRead(A1);
  y_dir = map(y_dir, 0, 1023, 0, 180);

  Serial.print("x: ");
  Serial.println(x_dir);

  Serial.print("y: ");
  Serial.println(y_dir);
  Serial.println();

}
