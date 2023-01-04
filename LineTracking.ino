#include <NewPing.h>

#define trigger         A1 //output
#define echo            A0 //input
#define maxDistance     400 //Max Ping distance

#define speedRS         6 //RS = Right Side DC motors
#define speedLS         5 //LS = Left Side DC motors

#define leftSensor      9 //left line tracking sensor
#define middleSensor    10 //middle line tracking sensor
#define rightSensor     11 //right line tracking sensor

NewPing sonar(trigger, echo, maxDistance);

const int LF = 4; //left front DC motor
const int LB = 2; //left back DC motor
const int RF = 8; //right front DC motor
const int RB = 7; //right back DC motor

unsigned char LS; //left sensor state
unsigned char MS; //middle sensor state
unsigned char RS; //right sensor state

//const int object = 25;

/*                    FUNCTIONS                           */
void rpm(int mph){ //sets wheel speed
  analogWrite(speedRS, mph);
  analogWrite(speedLS, mph);
}

void forward(){
  digitalWrite(RF, HIGH);
  digitalWrite(RB, LOW);
  digitalWrite(LF, HIGH);
  digitalWrite(LB, LOW);
}

void reverse(){
  digitalWrite(RF, LOW);
  digitalWrite(RB, HIGH);
  digitalWrite(LF, LOW);
  digitalWrite(LB, HIGH);
}

void stahp(){
  digitalWrite(RF, HIGH);
  digitalWrite(RB, HIGH);
  digitalWrite(LF, HIGH);
  digitalWrite(LB, HIGH);
}

void turnLeft(){
  digitalWrite(RF, LOW);
  digitalWrite(RB, HIGH);
  digitalWrite(LF, HIGH);
  digitalWrite(LB, LOW);
}

void turnRight(){
  digitalWrite(RF, HIGH);
  digitalWrite(RB, LOW);
  digitalWrite(LF, LOW);
  digitalWrite(LB, HIGH);
}

void checkSensors(void){ //if the sensor status is == high, its over a black surface
  LS = digitalRead(leftSensor);
  MS = digitalRead(middleSensor);
  RS = digitalRead(rightSensor);
}


/*                    SETUP AND LOOP                           */
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  pinMode(speedRS, OUTPUT);
  pinMode(speedLS, OUTPUT);
  pinMode(LF, OUTPUT);
  pinMode(LB, OUTPUT);
  pinMode(RF, OUTPUT);
  pinMode(RB, OUTPUT);

  pinMode(leftSensor, INPUT);
  pinMode(middleSensor, INPUT);
  pinMode(rightSensor, INPUT);
}

void loop() {
//  delay(1000); //listen for ping
//  unsigned int uS= sonar.ping(); //store micro seconds value into an integer
//  int distance = uS / US_ROUNDTRIP_CM; //convert from micro seconds to cm

    checkSensors();
    if(MS == HIGH){ // if middle line sensor is on black surface
      if(RS == HIGH && LS == LOW){ //if right line sensor is on black surface and left line sensor isnt
        turnRight();
        rpm(200);
      }
      else if(RS == LOW && LS == HIGH){ //if left line sensor is on black surface and right line sensor isnt
        turnLeft();
        rpm(200);
      }
      else{ //if only the middle sensor or all 3 sensors are on the black surface, then just go straight at half speed
        forward();
        rpm(180);
      }
    }
    else if(MS == LOW){
      checkSensors();
      if(RS == HIGH && LS == HIGH){ //if right and left sensor are on black surface and middle isnt, go straight at half speed
        forward();
        rpm(180);
      }
      else if(RS == HIGH && LS == LOW){ //if right line sensor is on black surface and left line sensor isnt
        turnRight();
        rpm(200);
      }
      else if (RS == LOW && LS == HIGH){ //if left line sensor is on black surface and right line sensor isnt
        turnLeft();
        rpm(200);
      }
      else{ //all sensors on white surface
        digitalWrite(LED_BUILTIN, HIGH);
        reverse();
        rpm(150);
        delay(100);
        stahp();
        digitalWrite(LED_BUILTIN, LOW);
      }
    }
}
