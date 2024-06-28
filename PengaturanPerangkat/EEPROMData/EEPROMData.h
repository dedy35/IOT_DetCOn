#ifndef EEPROMData_h
#define EEPROMData_h

#include <Arduino.h>

class EEPROMData
{
private:
  const int start_index = 0;
  const int data_length = 100;
  String readData(int start, int length);
  void writeData(String data, int start, int length);

public:
  void begin();
  String readWifiSSID();
  String readWifiPassword();
  String readMQTTBroker();
  String readDeviceName();
  String readPrefix();
  String readInterval();
  String readUsername();
  String readPassword();
  String readNamaHeader();
  String readCodeHeader1();
  String readCodeHeader2();
  String readCodeHeader3();
  String readCodeHeader4();
  String readCodeHeader5();
  String readCodeHeader6();
  String readCodeHeader7();
  String readCodeHeader8();
  String readChipIDD();

  void writeWifiSSID(String data);
  void writeWifiPassword(String data);
  void writeMQTTBroker(String data);
  void writeDeviceName(String data);
  void writePrefix(String data);
  void writeInterval(String data);
  void writeUsername(String data);
  void writePassword(String data);
  void writeNamaHeader(String data);
  void writeCodeHeader1(String data);
  void writeCodeHeader2(String data);
  void writeCodeHeader3(String data);
  void writeCodeHeader4(String data);
  void writeCodeHeader5(String data);
  void writeCodeHeader6(String data);
  void writeCodeHeader7(String data);
  void writeCodeHeader8(String data);
  void writeChipIDD(String data);
};

#endif
