#ifndef EnergySensor_h
#define EnergySensor_h

#include <Arduino.h>
#include "../lib/HLW8012/HLW8012.h"

class EnergySensor
{
private:
  HLW8012 sensor;
  double _current_multiplier; // Unit: us/A
  double _voltage_multiplier; // Unit: us/V
  double _power_multiplier;   // Unit: us/W
  const int currentMode = HIGH;
  const double currentResistor = 0.002; // 0.002;
  const double voltageResistorUpstream = 2350000;
  const double voltageResistorDownstream = 1000;
  const int pulseTimeout = 5000000;
  int activePower;
  int voltage;
  double current;
  double apparentPower;
  double powerFactor;
  double energy;
  // void unblockingDelay(int mseconds);
  void unblockingDelay(unsigned long mseconds);

public:
  EnergySensor(int cfPin, int cf1Pin, int selPin);
  void calibrate(int expectedActivePower, int expectedVoltage);
  double getCurrentMultiplier();
  double getVoltageMultiplier();
  double getPowerMultiplier();
  void setCalibrate(double currentMultiplier, double voltageMultiplier, double powerMultiplier);
  void read();
  int getActivePower();
  int getVoltage();
  double getCurrent();
  int getApparentPower();
  double getPowerFactor();
  double getEnergy();
};

#endif
