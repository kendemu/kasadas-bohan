#include "Arduino.h"
#include "Math.h"
#include <lchica.h>
#ifndef Arduino_h
#define Arduino_h
#endif
#define RED_LED_NUM 4
#define BLUE_LED_NUM 2
#define YELLOW_LED_NUM 2
#define LED_PIN_NUM 8
#define CLOCKWISE true
#define REVERSE false

const int ledpin[LED_PIN_NUM]= {46,47,48,49,50,51,52,53};
const int blueledpin[BLUE_LED_NUM]={ledpin[0], ledpin[4]};
const int redledpin[RED_LED_NUM]={ledpin[1],ledpin[3],ledpin[5],ledpin[7]};
const int touchpin = 3;
const int lightpin = 4;
const int buzzerpin = 7;
const int fingerpin = 8;
const bool fadein = true;
const bool fadeout = false;
const bool fast = true;
const bool slow = false;

bool led_pinstate[LED_PIN_NUM] = {true, false,true,false,true,false,true,false};

void BuzzerOn(int pin){
  digitalWrite(pin,HIGH);
}

void BuzzerOff(int pin){
  digitalWrite(pin,LOW);
}

void ToggleLED(int pin){
  if(led_pinstate[pin] == true)
    led_pinstate[pin] = false;
  else led_pinstate[pin] = true;
  digitalWrite(pin, led_pinstate[pin]);
}

bool isTouched(int pin){
  if(digitalRead(pin))
    return true;
  else return false;
}

bool isLight(int pin){
    if(digitalRead(pin))
      return true;
    else return false;
}

void setLight(int pin){
  bool state[LED_PIN_NUM];
  for(int i =0; i < LED_PIN_NUM; i++){
    state[i] = false;
  }
  state[pin] = true;
  for(int i=0; i < LED_PIN_NUM; i++){
    digitalWrite(ledpin[i],state[i]);
  }
}

bool isFinger(int pin){
   if(digitalRead(pin)) return true;
   else return false;
}

void fadeLight(double hz, bool rotate, bool vel, bool dir){
   bool state[LED_PIN_NUM];
   if(dir){
     for(int i=0; i < LED_PIN_NUM; i++){
          state[i] = true;
     }
   }
   else{
      for(int i=0; i < LED_PIN_NUM; i++){
          state[i] = false;
     }
   }
   for(int j =0; j < LED_PIN_NUM;j++){
      digitalWrite(ledpin[j],state[j]);
   }
   if(isTouched(touchpin) || isFinger(fingerpin)) return;
   delay(1000/hz);
   if(rotate){
     for(int i =0; i < LED_PIN_NUM;i++){
       if(dir){
        state[i] = false;
       }
       else{
         state[i] = true;
       }
        for(int j =0; j < LED_PIN_NUM;j++){
          digitalWrite(ledpin[j],state[j]);
        }
        if(isTouched(touchpin)) break;
        if(vel){
        hz--; 
        }
        else hz++;
        double time = 1000 / exp(log(hz));
        delay(time);
     }
   }
   else{
     for(int i =0; i < LED_PIN_NUM;i++){
       if(dir){
        state[LED_PIN_NUM - i] = false;
       }
       else {
         state[LED_PIN_NUM-i] = true;
       }
          for(int j =0; j < LED_PIN_NUM;j++){
          digitalWrite(ledpin[j],state[j]);
        }
        if(isTouched(touchpin)) break;
          if(vel){
        hz--; 
        }
        else hz++;
        double time = 1000 / exp(log(hz));
        delay(time);
     }
   }
}

void spinLight(){
   bool state[LED_PIN_NUM];
  for(int i =0; i < LED_PIN_NUM; i++){
    for(int j =0; j < LED_PIN_NUM; j++){
      state[j] = false;
    }
    state[i] = true;
    for(int j = 0; j < LED_PIN_NUM;j++){
    digitalWrite(ledpin[j],state[j]);
    }
    if(isTouched(touchpin)) break;
    delay(200);
  }
}
double i = 0;

void LightOff(){
  bool state[LED_PIN_NUM];
  for(int i = 0; i < LED_PIN_NUM; i++){
    state[i] = false;
    digitalWrite(ledpin[i],state[i]);
  }
}

void BlueLight(){
  bool state[LED_PIN_NUM];
  LightOff();
  state[0] = true;
  state[4] = true;
  for(int i = 0; i < LED_PIN_NUM; i++){
    digitalWrite(ledpin[i], state[i]);
  }
}


void Emergency(){
  i += 0.07;
  bool state[LED_PIN_NUM];
  for(int i = 0; i < LED_PIN_NUM; i++)
    state[i] = false;
  state[1] = true;
  state[3] = true;
  state[7] = true;
  for(int i = 0; i < LED_PIN_NUM; i++){
    digitalWrite(ledpin[i], state[i]);
  }
  BuzzerOn(buzzerpin);
  delay(exp(6.214 - i) + 20);
  state[1] = false;
  state[3] = false;
  state[7] = false;
   for(int i = 0; i < LED_PIN_NUM; i++){
    digitalWrite(ledpin[i], state[i]);
  }
  BuzzerOff(buzzerpin);
  delay(exp(6.214 - i) + 20);
}

void Nuregress(){
  Serial.println("Nugress start");
  static int point = 0;
  while(true){
    if(isFinger(fingerpin)) point++;
    Serial.print("point:");
    Serial.print(point);
    BlueLight();
    BuzzerOn(buzzerpin);
    delay(50);
    if(isTouched(touchpin)){
      Serial.print("final point:");
      Serial.print(point);
      point = 0;
    }
    if(isFinger(fingerpin)){
      delay(50);
      point++;
      BuzzerOff(buzzerpin);
      delay(50);
      continue;
    }
    LightOff();
    BuzzerOff(buzzerpin);
    delay(50);
  }
  BuzzerOff(buzzerpin);
  Serial.println("Nugress end");
  delay(1000);
}


const int baud = 115200;
void setup(){
  pinMode(ledpin[0],OUTPUT);
  pinMode(ledpin[1],OUTPUT);
  pinMode(ledpin[2],OUTPUT);
  pinMode(ledpin[3],OUTPUT);
  pinMode(ledpin[4],OUTPUT);
  pinMode(ledpin[5],OUTPUT);
  pinMode(ledpin[6],OUTPUT);
  pinMode(ledpin[7],OUTPUT);
  pinMode(touchpin,INPUT);
  pinMode(lightpin,INPUT);
  pinMode(buzzerpin,OUTPUT);
  pinMode(fingerpin,INPUT);
  Serial.begin(115200);
}

int point = 0;
void Point(){
  point++;
  //BlueLightOn();
  BuzzerOn(buzzerpin);
  delay(100);
  BuzzerOff(buzzerpin);
  //BlueLightOff();
  Serial.print("point;");
  Serial.print(point);
  Serial.print("\n");
  Serial.flush();
  delay(100);
}

void loop(){
  if(isFinger(fingerpin)){
    Point(); 
  }
  if(isLight(lightpin)){
    if(isTouched(touchpin)){
      //Nuregress();
      Emergency();
    }
    else{
      fadeLight(9,CLOCKWISE,fast,fadein);
      fadeLight(1,REVERSE,slow,fadeout);
      fadeLight(9,CLOCKWISE,slow,fadein);
      fadeLight(1,REVERSE,fast,fadeout);
    }
  }
  else{
    spinLight();//NightMode();
  }
  delay(100);
}
