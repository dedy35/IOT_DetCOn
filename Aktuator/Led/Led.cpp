#include "Led.h"
#include <Arduino.h>

//Public
Led::Led(int pin){
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk inisialisasi pin Led dengan keluaran berupa sinyal PWM.
  */
  this->pin = pin;
  pinMode(this->pin, OUTPUT);
  digitalWrite(this->pin, HIGH);
}

void Led::setOn(){
  digitalWrite(this->pin, LOW);
}

void Led::setOff(){
  digitalWrite(this->pin, HIGH);
}

void Led::blink(){
  for(int i = 0; i < 10; i++){
    setOn();
    delay(50);
    setOff();
    delay(50);
  }
}
//------------------------------------------------------------------------------