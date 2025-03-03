//FEB 11 SCRIPT
//MOST RECENT

#include <Wire.h>
#include <Servo.h>
#include <BasicLinearAlgebra.h>
#include <Servo.h>
#include <math.h>

#define SLAVE_ADDRESS 0x12
#define startMarker 125
#define endMarker 126

//UART VARIABLES
boolean inProgress = false;
boolean startFound = false;
boolean allReceived = false;
byte bytesRecvd = 0;
byte tempBuffer[4];

using namespace BLA;

Servo s1;
Servo s2;
Servo s3;

int s1_comp = 92;
int s2_comp = 98;
int s3_comp = 94;
int sv = 90;

int start = 126;
uint8_t buffer[3];

//random
double previous = 0;
double angle = 0;
float x_angle = 0;
float y_angle = 0;
//current_angles = [0,0,0];
int arm_rot = 120;

//kinematic constants (robot dimensions)
int pk = 58;
int bk = 46;
int b = 30;
int c = 81;

//matricies for servo angle calculations
BLA::Matrix<3,3> A;
BLA::Matrix<3,1> p1 = {pk,0,0};
BLA::Matrix<3,1> p2 = {-0.5*pk,pk*sin((120*PI)/180),0};
BLA::Matrix<3,1> p3 = {-0.5*pk,pk*sin((240*PI)/180),0};

BLA::Matrix<3,1> b1 = {bk,0,0};
BLA::Matrix<3,1> b2 = {-0.5*bk,bk*sin((120*PI)/180),0};
BLA::Matrix<3,1> b3 = {-0.5*bk,bk*sin((240*PI)/180),0};

float theta = 0;
float psi = 0;
float sigma = 0;
BLA::Matrix<3,3> Ry = {cos(theta*PI/180),0,sin(theta*PI/180),0,1,0,-sin(theta*PI/180),0,cos(theta*PI/180)};
BLA::Matrix<3,3> Rx = {1,0,0,0,cos(sigma*PI/180),-sin(sigma*PI/180),0,sin(sigma*PI/180),cos(sigma*PI/180)};
BLA::Matrix<3,3> Rz = {cos(arm_rot*PI/180),-sin(arm_rot*PI/180),0,sin(arm_rot*PI/180),cos(arm_rot*PI/180),0,0,0,1};

BLA::Matrix<3,1> T = {0,0,80};


float x = 0;
float y = 0;

void setup() {
  Serial.begin(115200);
  //Serial1.begin(9600);
  Serial.println("started");

  s1.attach(9);
  s2.attach(10);
  s3.attach(11);

  s1.write(s1_comp);
  s2.write(s2_comp);
  s3.write(s3_comp);

  Serial.println("test");
  servo_angles(0,0);
}

float bytee = 0;
void loop() {
    
    getSerialData();

    //this is THE line!!

    x = angle_algo(tempBuffer[1]);
    y = angle_algo(tempBuffer[2]);

    servo_angles(-x,-y);
    //Serial.print(x);
    //Serial.println(y);
    
    /*
    Serial.print("X: ");
    Serial.print(-x*2);
    Serial.print(", Y: ");
    Serial.println(-y*2);

    
    Serial.print("buff: ");
    Serial.print(buffer[0]);
    Serial.print(", : ");
    Serial.println(buffer[1]);
    */

    
    //servo_angles(buffer[0],buffer[1]);


    //servo_angles(0,0);
    /*
    servo_angles(10,0);
    delay(1500);
    servo_angles(0,10);
    delay(1500);
    servo_angles(10,10);
    delay(1500);
    servo_angles(0,0);
    delay(1500);
      */
  
}

float servo_angles(float x_angle, float y_angle){
  //this is where the kinematics will go, when I get an x angle and a y angle I sent it here
  theta = x_angle *PI/180;
  psi = y_angle *PI/180;
  BLA::Matrix<3,3> R = {cos(psi),sin(psi)*sin(theta),sin(psi)*cos(theta),0,cos(theta),-sin(theta),-sin(psi),cos(psi)*sin(theta),cos(psi)*cos(theta)};
  
  BLA::Matrix<3,1> p1_base = R*p1;
  BLA::Matrix<3,1> p2_base = R*p2;
  BLA::Matrix<3,1> p3_base = R*p3;

  

  BLA::Matrix<3,1> l1 = T + R*p1 - b1;
  BLA::Matrix<3,1> l2 = T + R*p2 - b2;
  BLA::Matrix<3,1> l3 = T + R*p3 - b3;

  float l1_norm = mag(l1(0),l1(1),l1(2));
  float l2_norm = mag(l2(0),l2(1),l2(2));
  float l3_norm = mag(l3(0),l3(1),l3(2));

  float l2_rot = 360 - (180 - abs((180/PI)*atan((abs(l2(1))/abs(l2(0))))));
  float l3_rot = 180 - abs((180/PI)*atan((abs(l3(1))/abs(l3(0)))));

  

  BLA::Matrix<3,3> Z = {cos(l2_rot*PI/180),-sin(l2_rot*PI/180),0,sin(l2_rot*PI/180),cos(l2_rot*PI/180),0,0,0,1};
  BLA::Matrix<3,3> Z2 = {cos(l3_rot*PI/180),-sin(l3_rot*PI/180),0,sin(l3_rot*PI/180),cos(l3_rot*PI/180),0,0,0,1};

  BLA::Matrix<3,1> l1_0plane;
  BLA::Matrix<3,1> l2_120plane = Z * l2;
  BLA::Matrix<3,1> l3_240plane = Z2 * l3;

  

  float angle_1 = trig(l1(0),l1(2),l1_norm,c,b);
  float angle_2 = trig(abs(l2_120plane(0)),abs(l2_120plane(2)),l2_norm,c,b);
  float angle_3 = trig(abs(l3_240plane(0)),abs(l3_240plane(2)),l3_norm,c,b);

  /*
  Serial.print("p1 base: ");
  Serial.println(p1_base);
  Serial.print("p2 base: ");
  Serial.println(p2_base);
  Serial.print("p3 base ");
  Serial.println(p3_base);

  Serial.println("l2 rotation angle: ");
  Serial.println(l2_rot);
  Serial.println("l3 rotation angle: ");
  Serial.println(l3_rot);

  Serial.println("base L2: ");
  Serial.println(l2);
  Serial.println("rotated L2: ");
  Serial.println(l2_120plane);
  Serial.println("rotated L3: ");
  Serial.println(l3_240plane);

  
  Serial.print("l1: ");
  Serial.println(l1);
  Serial.print("l2: ");
  Serial.println(l2);
  Serial.print("l3: ");
  Serial.println(l3);

  Serial.print("norm l1: ");
  Serial.println(l1_norm);
  Serial.print("norm l2: ");
  Serial.println(l2_norm);
  Serial.print("norm l2: ");
  Serial.println(l3_norm);
  */
  //Serial.print("angle 1: ");
  Serial.println(angle_1);
  //Serial.print("angle 2: ");
  //Serial.print(angle_2);
  //Serial.print("angle 3: ");
  //Serial.println(angle_3);

  
  s1.write(s1_comp - angle_1);
  s2.write(s2_comp - angle_2);
  s3.write(s3_comp - angle_3);

  //float numbers[] = {10, 20, 30};  // Static array to persist memory
  //return numbers;  // Return pointer to the array
}

float mag(float x,float y,float z){
  float magnitude = sqrt(x*x + y*y + z*z);

  return magnitude;
}

float trig(double x, double y, double a, double b, double c) {
    double term1 = atan(y / x) * (180/PI);
    double term2 = acos((b * b - c * c - a * a) / (-2.0 * a * c)) * (180/PI);
    //Serial.println(term1);
    //Serial.println(term2);
    return term1-term2;
}


float angle_algo(float raw){
    if(raw < 51){
      return raw/10;
    }
    else if(raw > 51){
      return -((raw-50)/10);
    }
}

void getSerialData() {
     // Receives data into tempBuffer[]
     //   saves the number of bytes that the PC said it sent - which will be in tempBuffer[1]
     //   uses decodeHighBytes() to copy data from tempBuffer to dataRecvd[]
     
     // the Arduino program will use the data it finds in dataRecvd[]
  if(Serial.available() > 0) {

    byte x = Serial.read();
    if (x == startMarker) { 
      //Serial.println("start");
      bytesRecvd = 0; 
      inProgress = true;
      // blinkLED(2);
      // debugToPC("start received");
    }
      
    if(inProgress) {
      tempBuffer[bytesRecvd] = x;
      bytesRecvd ++;
    }

    if (x == endMarker) {
      inProgress = false;
      allReceived = true;
      
        // save the number of bytes that were sent
  
    }
  }
}
