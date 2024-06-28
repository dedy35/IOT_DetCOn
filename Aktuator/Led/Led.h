#ifndef Led_h
#define Led_h

#include <Arduino.h>

class Led{
private:
  int pin;
public:
  Led(int pin);
  void setOn();
  void setOff();
  void blink();
};

#endif
