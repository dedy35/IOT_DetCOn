#ifndef HTMLForm_h
#define HTMLForm_h

#include <Arduino.h>

enum HTML_FORM_ARG
{
  ARG_USERNAME,
  ARG_PASSWORD,
  ARG_KONFIRMASI_PASSWORD,
  ARG_WIFI_SSID,
  ARG_WIFI_PASSWORD,
  ARG_MQTT_BROKER,
  ARG_DEVICE_NAME,
  ARG_PREFIX,
  ARG_INTERVAL,
  ARG_NAMA_HEADER,
  ARG_CODE_HEADER1,
  ARG_CODE_HEADER2,
  ARG_CODE_HEADER3,
  ARG_CODE_HEADER4,
  ARG_CODE_HEADER5,
  ARG_CODE_HEADER6,
  ARG_CODE_HEADER7,
  ARG_CODE_HEADER8,
  ARG_CHIP_IDD

};

typedef struct
{
  String firmwareName;
  String firmwareDescription;
  String firmwareVersion;
  String legalCopyright;
  String companyName;
  String firmwareType;
} FIRMWARE_IN;

class HTMLForm
{
private:
  String htmlFormArg[19] = {
      "username",
      "password",
      "konfirmasi_password",
      "wifi_ssid",
      "wifi_password",
      "mqtt_broker",
      "device_name",
      "prefix",
      "interval",
      "nama_header",
      "code_header1",
      "code_header2",
      "code_header3",
      "code_header4",
      "code_header5",
      "code_header6",
      "code_header7",
      "code_header8",
      "chipid_device"};

public:
  String login(String target, String message);
  String pengaturanKoneksi(String target, String message, String link_pengaturan_koneksi, String link_pengaturan_pengguna, String link_informasi_firmware, String link_logout, String wifi_ssid, String wifi_password, String mqtt_broker, String device_name, String prefix, String interval, String nama_header, String code_header1, String code_header2, String code_header3, String code_header4, String code_header5, String code_header6, String code_header7, String code_header8, String chipid_device);
  String pengaturanPengguna(String target, String message, String link_pengaturan_koneksi, String link_pengaturan_pengguna, String link_informasi_firmware, String link_logout, String username, String password, String konfirmasi_password);
  String informasiFirmware(FIRMWARE_IN informasi_firmware, String list_topic, String link_pengaturan_koneksi, String link_pengaturan_pengguna, String link_informasi_firmware, String link_logout);
  String getArg(HTML_FORM_ARG arg);
};

#endif
