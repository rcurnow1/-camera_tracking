#include <Servo.h>

Servo panServo; 
Servo tiltServo; 

byte redledPin = 2; 
byte yellowledPin = 3; 
byte greenledPin = 4; 

const byte buffSize = 40; 
char inputBuffer[buffSize]; 
const char startMarker = '<'; 
const char endMarker = '>'; 
byte bytesRecvd = 0; 
boolean readinProgress = false; 
boolean newDataFromPC = false; 

float panServoAngle = 90.0; 
float tiltServoAngle = 90.0; 
int LED_state = 0; 

void setup(){
  Serial.begin(115200); 
   
  panServo.attach(8); 
  tiltServo.attach(9); 

  pinMode(redledPin, OUTPUT); //configures the specified pin to behave either as an input or an output
  pinMode(yellowledPin, OUTPUT); 
  pinMode(greenledPin, OUTPUT); 

  //moveServo(); 

  start_sequence(); 

  delay(200);

  Serial.println("We are moving");
    
 }

void loop(){
   getDataFromPC(); 
   replyToPC(); 
   moveServo(); 
   setLED(); 
}

void getDataFromPC() {
   //revieve data from PC ans qsafe it into inputBuffer
   if(Serial.available() > 0) {
     char x = Serial.read(); 

     if (x == endMarker) {
       readinProgress = false; 
       newDataFromPC = true; 
       inputBuffer[bytesRecvd]=0; 
       processData();         
     }
     if(readinProgress) {
       inputBuffer[bytesRecvd] = x; 
       bytesRecvd ++; 
       if(bytesRecvd == buffSize){
         bytesRecvd = buffSize - 1;   
       }  
     }

     if(x == startMarker) {
       bytesRecvd = 0; 
       readinProgress = true;   
     }        
    }
   }

void processData() {
  char * strtokIndx; //breaks the string pointed to into a sequence of token, each of which is delimited by a character from the string pointed to  

    strtokIndx = strtok(inputBuffer, ","); //get the first part 
    panServoAngle = atof(strtokIndx); //converting C-style "string" to the nearest floating point number that can be represented in the computer with this compiler

    strtokIndx = strtok(NULL,","); // get the second part(this cont where the previous call left off) 
    tiltServoAngle = atof(strtokIndx); //convert this part to a float

    strtokIndx = strtok(NULL, ","); //get the last part
    LED_state = atoi(strtokIndx); //convert this into an integer (string to int)   
}

void replyToPC() {
  if (newDataFromPC) {
    newDataFromPC = false; 
    Serial.print("<"); 
    Serial.print(panServo.read()); 
    Serial.print(","); 
    Serial.print(tiltServo.read()); 
    Serial.print(">");   
  }  
}

void moveServo() {
  panServo.write(panServoAngle); 
  tiltServo.write(tiltServoAngle);   
}

void setLED() {
  if(LED_state == 2){
    digitalWrite(redledPin, LOW); 
    digitalWrite(yellowledPin, HIGH); 
    digitalWrite(greenledPin, LOW); 
    }  
  else if(LED_state == 1){
    digitalWrite(redledPin, LOW); 
    digitalWrite(yellowledPin, LOW); 
    digitalWrite(greenledPin, HIGH); 
    }
  else if(LED_state == 0){
    digitalWrite(redledPin, HIGH); 
    digitalWrite(yellowledPin, LOW); 
    digitalWrite(greenledPin, LOW); 
    }
  else if(LED_state == 3){
    digitalWrite(redledPin, HIGH); 
    digitalWrite(yellowledPin, HIGH); 
    digitalWrite(greenledPin, HIGH); 
    }
  else{
    digitalWrite(redledPin, LOW); 
    digitalWrite(yellowledPin, LOW); 
    digitalWrite(greenledPin, LOW); 
    }
}

void start_sequence() {
    panServo.write(90); 
    tiltServo.write(90); 
    delay(200); 

    digitalWrite(redledPin, HIGH); 
    delay(100); 
    digitalWrite(redledPin, LOW); 
    digitalWrite(yellowledPin, HIGH);
    delay(100); 
    digitalWrite(yellowledPin, LOW); 
    digitalWrite(greenledPin, HIGH); 
    delay(100); 

    digitalWrite(redledPin, LOW); 
    digitalWrite(yellowledPin, LOW); 
    digitalWrite(greenledPin, LOW); 
    delay(100); 
    digitalWrite(redledPin, HIGH); 
    digitalWrite(yellowledPin, HIGH); 
    digitalWrite(greenledPin, HIGH); 
    delay(100); 
    digitalWrite(redledPin, LOW); 
    digitalWrite(yellowledPin, LOW); 
    digitalWrite(greenledPin, LOW); 
  }

 
