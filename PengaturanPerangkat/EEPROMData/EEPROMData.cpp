#include "EEPROMData.h"
#include <Arduino.h>
#include <EEPROM.h>

// Private
String EEPROMData::readData(int start, int length)
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca data dari EPROM
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  String data = "";
  for (int i = 0; i < length; i++)
  {
    byte x = EEPROM.read(start + i);
    if ((x != 0) && (x != 255))
    {
      data += (char)x;
    }
  }
  return data;
}

void EEPROMData::writeData(String data, int start, int length)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis data ke EPROM.
  */

  byte buffer[length];
  for (int i = 0; i < length; i++)
  {
    buffer[i] = 0;
  }
  for (unsigned int i = 0; i < data.length(); i++)
  {
    buffer[i] = data[i];
  }
  for (int i = 0; i < length; i++)
  {
    EEPROM.write(start + i, buffer[i]);
  }
  EEPROM.commit();
}
//------------------------------------------------------------------------------

// Public
void EEPROMData::begin()
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk inisialisasi EPROM sebelum dilakukan pembacaan dan penulisan data.
  */

  EEPROM.begin(SPI_FLASH_SEC_SIZE);
}

String EEPROMData::readWifiSSID()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Wi-Fi SSID.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 0), data_length);
}

String EEPROMData::readWifiPassword()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Wi-Fi Password.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 1), data_length);
}

String EEPROMData::readMQTTBroker()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca alamat MQQT Broker.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 2), data_length);
}

String EEPROMData::readDeviceName()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca nama device.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 3), data_length);
}

String EEPROMData::readPrefix()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca prefix.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 4), data_length);
}

String EEPROMData::readInterval()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca interval.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 5), data_length);
}

String EEPROMData::readUsername()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Username.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 6), data_length);
}

String EEPROMData::readPassword()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Password.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 7), data_length);
}
String EEPROMData::readNamaHeader()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca NamaHeader.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 8), data_length);
}
String EEPROMData::readCodeHeader1()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Code Header.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 9), data_length);
}
String EEPROMData::readCodeHeader2()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Code Header.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 10), data_length);
}
String EEPROMData::readCodeHeader3()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Code Header.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 11), data_length);
}
String EEPROMData::readCodeHeader4()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Code Header.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 12), data_length);
}
String EEPROMData::readCodeHeader5()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Code Header.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 13), data_length);
}
String EEPROMData::readCodeHeader6()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Code Header.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 14), data_length);
}
String EEPROMData::readCodeHeader7()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Code Header.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 15), data_length);
}
String EEPROMData::readCodeHeader8()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Code Header.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 16), data_length);
}
String EEPROMData::readChipIDD()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membaca Password.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return readData(start_index + (data_length * 17), data_length);
}

void EEPROMData::writeWifiSSID(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Wi-Fi SSID ke EPROM.
  */

  return writeData(data, start_index + (data_length * 0), data_length);
}

void EEPROMData::writeWifiPassword(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Wi-Fi Password ke EPROM.
  */

  return writeData(data, start_index + (data_length * 1), data_length);
}

void EEPROMData::writeMQTTBroker(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis alamat MQTT Broker ke EPROM.
  */

  return writeData(data, start_index + (data_length * 2), data_length);
}

void EEPROMData::writeDeviceName(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis device name.
  */

  return writeData(data, start_index + (data_length * 3), data_length);
}

void EEPROMData::writePrefix(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis prefix.
  */

  return writeData(data, start_index + (data_length * 4), data_length);
}

void EEPROMData::writeInterval(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis interval.
  */

  return writeData(data, start_index + (data_length * 5), data_length);
}

void EEPROMData::writeUsername(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Username ke EPROM.
  */

  return writeData(data, start_index + (data_length * 6), data_length);
}

void EEPROMData::writePassword(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Password ke EPROM.
  */

  return writeData(data, start_index + (data_length * 7), data_length);
}

void EEPROMData::writeNamaHeader(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Nama Header ke EPROM.
  */

  return writeData(data, start_index + (data_length * 8), data_length);
}
void EEPROMData::writeCodeHeader1(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Code Header ke EPROM.
  */

  return writeData(data, start_index + (data_length * 9), data_length);
}
void EEPROMData::writeCodeHeader2(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Code Header ke EPROM.
  */

  return writeData(data, start_index + (data_length * 10), data_length);
}
void EEPROMData::writeCodeHeader3(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Code Header ke EPROM.
  */

  return writeData(data, start_index + (data_length * 11), data_length);
}
void EEPROMData::writeCodeHeader4(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Code Header ke EPROM.
  */

  return writeData(data, start_index + (data_length * 12), data_length);
}
void EEPROMData::writeCodeHeader5(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Code Header ke EPROM.
  */

  return writeData(data, start_index + (data_length * 13), data_length);
}
void EEPROMData::writeCodeHeader6(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Code Header ke EPROM.
  */

  return writeData(data, start_index + (data_length * 14), data_length);
}
void EEPROMData::writeCodeHeader7(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Code Header ke EPROM.
  */

  return writeData(data, start_index + (data_length * 15), data_length);
}
void EEPROMData::writeCodeHeader8(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis Code Header ke EPROM.
  */

  return writeData(data, start_index + (data_length * 16), data_length);
}
void EEPROMData::writeChipIDD(String data)
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk menulis CHIP_IDD ke EPROM.
  */

  return writeData(data, start_index + (data_length * 17), data_length);
}

//------------------------------------------------------------------------------