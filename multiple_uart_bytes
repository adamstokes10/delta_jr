//USED TO RECEIVE MULTIPLE BYTES FROM UART AND PUT THEM INTO AN ARRAY!!!! SKELETON!!!!

int start = 126;
uint8_t buffer[2];
int x_angle = 0;
int y_angle = 0;

#define startMarker 125
#define endMarker 126

float bytee;

boolean inProgress = false;
boolean startFound = false;
boolean allReceived = false;

byte bytesRecvd = 0;
byte dataSentNum = 0;
byte dataRecvCount = 0;


byte dataRecvd[2]; 
byte dataSend[2];  
byte tempBuffer[4];


const byte numChars = 2;
char receivedChars[numChars]; 

void setup() {
    Serial.begin(115200); // Start serial communication at 9600 baud
}
// Example 2 - Receive with an end-marker
void loop() {
    /*
    if (Serial.available() > 0) { // Check if data is available
        byte incomingByte = Serial.read(); // Read the incoming byte
        Serial.print("Received: ");
        Serial.println(incomingByte, DEC); // Print the byte in decimal format
    }
    */
    getSerialData();
    //bytee = Serial.read();
    //Serial.println(bytee,DEC);
    //Serial.print(tempBuffer[0],tempBuffer[1]);
    //post processing, get actual servo angles
    //x_angle = angle_algo(7);
    //y_angle = angle_algo(12);
    Serial.print(tempBuffer[0]);
    Serial.print(tempBuffer[1]);
    Serial.print(tempBuffer[2]);
    Serial.println(tempBuffer[3]);

    
    
    /*
    Serial.println("buffer 0: ");
    Serial.println(buffer[0]);
    Serial.println("buffer 1: ");
    Serial.println(buffer[1]);
    
    Serial.print("X: ");
    Serial.print(x_angle);
    Serial.print(", Y: ");
    Serial.println(y_angle);
    */
    
}

  // an array to store the received data

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


float angle_algo(float raw){
    if(raw < 51){
      return raw/10;
    }
    else if(raw > 51){
      return -((raw-50)/10);
    }
}
