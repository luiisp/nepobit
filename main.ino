// nepobit (Bluetooth Version + Auto Mode)
// 2024 - Pedro Luis Dias (https://github.com/luiisp)
// forked by https://kuongshun.com (Lesson 2 Bluetooth Car + Lesson 4 Obstacle Avoidance Car)
// f: forward, b: back, l: left, r: right, s: stop, u: obstacle avoidance car, d: dance, m: moonwalk, x: super forward

#include <Servo.h>  


#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define LED 13
#define TRIG A5
#define ECHO A4
#define carSpeed 150


Servo myservo;
int rightDistance = 0, leftDistance = 0, middleDistance = 0;
bool autoMode = false;
char getstr;

void forward() { 
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void superForward() {
  int superSpeed = 240; 
  analogWrite(ENA, superSpeed);
  analogWrite(ENB, superSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Super Forward");
}

void back() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

void left() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH); 
  Serial.println("Left");
}

void right() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
}


int Distance_test() {
  digitalWrite(TRIG, LOW);   
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);  
  delayMicroseconds(20);
  digitalWrite(TRIG, LOW);   
  float Fdistance = pulseIn(ECHO, HIGH);  
  Fdistance = Fdistance / 58;       
  return (int)Fdistance;
}

void greet() {
  for (int i = 0; i < 2; i++) {
    myservo.write(60);  
    delay(400);
    myservo.write(120); 
    delay(400);
  }
  myservo.write(90); 
}

void moonwalk() {
  for (int i = 0; i < 3; i++) {
    back();
    myservo.write(60);

    delay(400);

    myservo.write(120); 
    back();

    delay(400);

    myservo.write(90); 

    delay(150);

    forward();

    delay(150);

    forward();



  }
  myservo.write(90);
  stop(); 
}



void dance() {
  greet();
  int danceSpeed = 150;  
  int stepDelay = 400;   


  forward();
  delay(stepDelay);
  stop();
  delay(200);

  back();
  delay(stepDelay);
  stop();
  delay(200);

  for (int i = 0; i < 4; i++) {
    right();
    delay(stepDelay / 2); 
    left();
    delay(stepDelay / 2);  
  }

  for (int i = 0; i < 3; i++) {
    myservo.write(90);  
    delay(200);
    myservo.write(45); 
    delay(200);
    myservo.write(120); 
    delay(200);
  }

  right();
  delay(1000);
  left();
  delay(1000);


  myservo.write(90);
  stop();
}

void setup() {
  myservo.attach(3);  
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  stop();
}

void loop() {
  if (Serial.available()) {
    getstr = Serial.read();
    switch(getstr) {
      case 'f': forward(); autoMode = false; break;
      case 'b': back(); autoMode = false; break;
      case 'l': left(); autoMode = false; break;
      case 'r': right(); autoMode = false; break;
      case 's': stop(); autoMode = false; break;
      case 'u': 
        autoMode = true; 
        greet(); 
        break;
      case 'd': 
        autoMode = false; 
        dance(); 
        break;
      case 'm': 
        autoMode = false; 
        moonwalk(); 
        break;
      case 'x':  
        autoMode = false;
        stop();
        greet();
        superForward();
        break;


      default: break;
    }
  }

  if (autoMode) {
    myservo.write(90);  
    delay(500); 
    middleDistance = Distance_test();

    if (middleDistance <= 20) {     
      stop();
      delay(500);
      myservo.write(10);          
      delay(1000);      
      rightDistance = Distance_test();

      delay(500);
      myservo.write(90);              
      delay(1000);                                                  
      myservo.write(180);              
      delay(1000); 
      leftDistance = Distance_test();

      delay(500);
      myservo.write(90);              
      delay(1000);
      if (rightDistance > leftDistance) {
        right();
        delay(360);
      } else if (rightDistance < leftDistance) {
        left();
        delay(360);
      } else if ((rightDistance <= 20) || (leftDistance <= 20)) {
        back();
        delay(180);
      } else {
        forward();
      }
    } else {
      forward();
    }                     
  }
}
