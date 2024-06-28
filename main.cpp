#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WebServer.h>
//#include <RTClib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include "../lib/MQTT/PubSubClient.h"
//#include "../lib/HLW8012/HLW8012.h"
#include <WiFiUdp.h>    //tambahan untuk arduionOTA
#include <ArduinoOTA.h> //tambahan untuk arduinoOTA

// include library button
//#include <ezButton.h>

// Include Sensor
#include "Sensor/DigitalSensor/DigitalSensor.h"
//#include "Sensor/EnergySensor/EnergySensor.h"
//------------------------------------------------------------------------------

// Include Aktuator
#include "Aktuator/Led/Led.h"
//#include "Aktuator/Relay/Switch/Switch.h"
//------------------------------------------------------------------------------

// Include Pengaturan Perangkat
#include "PengaturanPerangkat/EEPROMData/EEPROMData.h"
#include "PengaturanPerangkat/HTMLForm/HTMLForm.h"
//------------------------------------------------------------------------------
#include "FirmwareInformation/FirmwareInformation.h"

#define UPDATE_TIME 5000

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//led sensor
#define ledG 13
#define ledR 15
#define ledK 2
int counter = 0;
boolean state = true;



//Relay sensor
const int relay = 14;
// Set timer untuk lama waktu tekan
//const int start_opsi_Satu = 300;
//const int stop_opsi_Satu = 1999;
const int start_opsi_Dua = 2000;
const int stop_opsi_Dua = 3500;
const int start_opsi_Tiga = 300;
const int stop_opsi_Tiga = 1999;
const int start_opsi_Empat = 4000;
const int stop_opsi_Empat = 5000;

int lastState = LOW;
int lastState2 = LOW;
int currentState = LOW;
int currentState2 = LOW;
unsigned long waktuTekan = 0;
unsigned long waktuLepas = 0;
unsigned long waktuTekan2 = 0;
unsigned long waktuLepas2 =0;


int buttonState = 0;
//int count = 0;

unsigned long previousMillis = 0;
const long waktuserial = 2000;
String arraydata[10];
//String con1, con2, con3, con4, con5, con6, con7, con8, con9, con10;

// konfurasi wemos
String ap_ssid = "honicel";
String chip_id, prefix, ip_device, nama_alat, t_chipid, t_ip_addr, t_nama_alat, t_buzzer, t_kondisi, t_status;
int interval;

const char *ap_password = "";
ESP8266WebServer server(80);
WiFiClient espClient;
PubSubClient MQTT(espClient);

HTMLForm form;
EEPROMData pengaturan;

Led led(LED_BUILTIN);
DigitalSensor jumper(16);
//DigitalSensor jumper2(12);
//Switch sw;
//ezButton button(16);

String session;

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

// Web Server
String generateSession()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membuat nama session secara random dengan panjang 100 digit.
  - Keluaran fungsi berupa objek kelas String.
  */

  String char_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";
  String str = "HONICEL_AUTOMATION_SESSION=";
  for (int i = 0; i < 10; i++)
  {
    str += char_set[random(char_set.length())];
  }
  return str;
}

bool is_authentified()
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk mengecek apakah pengguna telah login atau telah logout.
  - Keluaran fungsi berupa tipe data boolean.
  - true = apabila user telah login.
  - false = apabila user telah logout.
  */

  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie"))
  {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf(session) != -1)
    {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

void handleLogin()
{
  /*
  Spesifikasi :
  - Apabila pengguna belum login server akan memanggil prosedur ini, sehingga pengguna
    diminta untuk login terlebih dahulu sebelum masuk ke halaman pengaturan.
  */

  String message = "";
  if (server.hasHeader("Cookie"))
  {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("disconnect"))
  {
    session = generateSession();
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  if (server.hasArg(form.getArg(ARG_USERNAME)) && server.hasArg(form.getArg(ARG_PASSWORD)))
  {
    if ((server.arg(form.getArg(ARG_USERNAME)) == pengaturan.readUsername()) && (server.arg(form.getArg(ARG_PASSWORD)) == pengaturan.readPassword()))
    {
      session = generateSession();
      server.sendHeader("Location", "/");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", session);
      server.send(301);
      Serial.println("Log in Successful");
      return;
    }
    message = "Username atau password salah.";
    Serial.println("Log in Failed");
  }
  server.send(200, "text/html", form.login("/login", message));
}

void handleKoneksi()
{
  /*
  Spesifikasi :
  - Apabila pengguna telah login server membuka halaman / server akan memanggil prosedur ini,
    sehingga pengguna dapat masuk ke halaman pengaturan koneksi.
  - Apabila pengguna belum login server akan memanggil prosedur handleLogin(),
    sehingga pengguna diminta untuk login terlebih dahulu sebelum masuk ke halaman ini.
  */

  String message = "";
  if (!is_authentified())
  {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }

  if (server.hasArg(form.getArg(ARG_WIFI_SSID)) && server.hasArg(form.getArg(ARG_WIFI_PASSWORD)) && server.hasArg(form.getArg(ARG_MQTT_BROKER)) && server.hasArg(form.getArg(ARG_DEVICE_NAME)) && server.hasArg(form.getArg(ARG_PREFIX)) && server.hasArg(form.getArg(ARG_INTERVAL)))
  {
    String error_message = "";
    if (server.arg(form.getArg(ARG_WIFI_SSID)) == "")
    {
      error_message += "Wi-Fi SSID tidak boleh kosong.\\n";
    }

    if (server.arg(form.getArg(ARG_WIFI_PASSWORD)) == "")
    {
      error_message += "Wi-Fi Password tidak boleh kosong.\\n";
    }

    if (server.arg(form.getArg(ARG_MQTT_BROKER)) == "")
    {
      error_message += "MQTT Broker tidak boleh kosong.\\n";
    }

    if (server.arg(form.getArg(ARG_DEVICE_NAME)) == "")
    {
      error_message += "Nomor perangkat tidak boleh kosong.\\n";
    }

    if (server.arg(form.getArg(ARG_PREFIX)) == "")
    {
      error_message += "Prefix tidak boleh kosong.\\n";
    }

    if (server.arg(form.getArg(ARG_INTERVAL)) == "")
    {
      error_message += "Interval tidak boleh kosong.\\n";
    }

    if (error_message == "")
    {
      if (pengaturan.readWifiSSID() != server.arg(form.getArg(ARG_WIFI_SSID)))
      {
        pengaturan.writeWifiSSID(server.arg(form.getArg(ARG_WIFI_SSID)));
      }

      if (pengaturan.readWifiPassword() != server.arg(form.getArg(ARG_WIFI_PASSWORD)))
      {
        pengaturan.writeWifiPassword(server.arg(form.getArg(ARG_WIFI_PASSWORD)));
      }

      if (pengaturan.readMQTTBroker() != server.arg(form.getArg(ARG_MQTT_BROKER)))
      {
        pengaturan.writeMQTTBroker(server.arg(form.getArg(ARG_MQTT_BROKER)));
      }

      if (pengaturan.readDeviceName() != server.arg(form.getArg(ARG_DEVICE_NAME)))
      {
        pengaturan.writeDeviceName(server.arg(form.getArg(ARG_DEVICE_NAME)));
      }

      if (pengaturan.readPrefix() != server.arg(form.getArg(ARG_PREFIX)))
      {
        pengaturan.writePrefix(server.arg(form.getArg(ARG_PREFIX)));
      }

      if (pengaturan.readInterval() != server.arg(form.getArg(ARG_INTERVAL)))
      {
        pengaturan.writeInterval(server.arg(form.getArg(ARG_INTERVAL)));
      }

      // if (pengaturan.readNamaHeader() != server.arg(form.getArg(ARG_NAMA_HEADER)))
      // {
      //   pengaturan.writeNamaHeader(server.arg(form.getArg(ARG_NAMA_HEADER)));
      // }
      // if (pengaturan.readCodeHeader1() != server.arg(form.getArg(ARG_CODE_HEADER1)))
      // {
      //   pengaturan.writeCodeHeader1(server.arg(form.getArg(ARG_CODE_HEADER1)));
      // }
      // if (pengaturan.readCodeHeader2() != server.arg(form.getArg(ARG_CODE_HEADER2)))
      // {
      //   pengaturan.writeCodeHeader2(server.arg(form.getArg(ARG_CODE_HEADER2)));
      // }
      // if (pengaturan.readCodeHeader3() != server.arg(form.getArg(ARG_CODE_HEADER3)))
      // {
      //   pengaturan.writeCodeHeader3(server.arg(form.getArg(ARG_CODE_HEADER3)));
      // }
      // if (pengaturan.readCodeHeader4() != server.arg(form.getArg(ARG_CODE_HEADER4)))
      // {
      //   pengaturan.writeCodeHeader4(server.arg(form.getArg(ARG_CODE_HEADER4)));
      // }
      // if (pengaturan.readCodeHeader5() != server.arg(form.getArg(ARG_CODE_HEADER5)))
      // {
      //   pengaturan.writeCodeHeader5(server.arg(form.getArg(ARG_CODE_HEADER5)));
      // }
      // if (pengaturan.readCodeHeader6() != server.arg(form.getArg(ARG_CODE_HEADER6)))
      // {
      //   pengaturan.writeCodeHeader6(server.arg(form.getArg(ARG_CODE_HEADER6)));
      // }
      // if (pengaturan.readCodeHeader7() != server.arg(form.getArg(ARG_CODE_HEADER7)))
      // {
      //   pengaturan.writeCodeHeader7(server.arg(form.getArg(ARG_CODE_HEADER7)));
      // }
      // if (pengaturan.readCodeHeader8() != server.arg(form.getArg(ARG_CODE_HEADER8)))
      // {
      //   pengaturan.writeCodeHeader8(server.arg(form.getArg(ARG_CODE_HEADER8)));
      // }
      if (pengaturan.readChipIDD() != server.arg(form.getArg(ARG_CHIP_IDD)))
      {
        pengaturan.writeChipIDD(server.arg(form.getArg(ARG_CHIP_IDD)));
      }

      message = "Pengaturan koneksi telah tersimpan.";
    }
    else
    {
      message = error_message;
    }
  }
  server.send(200, "text/html", form.pengaturanKoneksi("/", message, "/", "/pengguna", "/firmware", "/login?disconnect=1", pengaturan.readWifiSSID(), pengaturan.readWifiPassword(), pengaturan.readMQTTBroker(), pengaturan.readDeviceName(), pengaturan.readPrefix(), pengaturan.readInterval(), pengaturan.readNamaHeader(), pengaturan.readCodeHeader1(), pengaturan.readCodeHeader2(), pengaturan.readCodeHeader3(), pengaturan.readCodeHeader4(), pengaturan.readCodeHeader5(), pengaturan.readCodeHeader6(), pengaturan.readCodeHeader7(), pengaturan.readCodeHeader8(), pengaturan.readChipIDD()));
}

void handlePengguna()
{
  /*
  Spesifikasi :
  - Apabila pengguna telah login server membuka halaman /pengguna server akan memanggil prosedur ini,
    sehingga pengguna dapat masuk ke halaman pengaturan pengguna.
  - Apabila pengguna belum login server akan memanggil prosedur handleLogin(),
    sehingga pengguna diminta untuk login terlebih dahulu sebelum masuk ke halaman ini.
  */

  String message = "";
  if (!is_authentified())
  {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }

  if (server.hasArg(form.getArg(ARG_USERNAME)) && server.hasArg(form.getArg(ARG_PASSWORD)) && server.hasArg(form.getArg(ARG_KONFIRMASI_PASSWORD)))
  {
    String error_message = "";
    if (server.arg(form.getArg(ARG_USERNAME)) == "")
    {
      error_message += "Username tidak boleh kosong.\\n";
    }

    if (server.arg(form.getArg(ARG_PASSWORD)) == "")
    {
      error_message += "Password tidak boleh kosong.\\n";
    }

    if (server.arg(form.getArg(ARG_KONFIRMASI_PASSWORD)) == "")
    {
      error_message += "Konfirmasi password tidak boleh kosong.\\n";
    }

    if (server.arg(form.getArg(ARG_PASSWORD)) != server.arg(form.getArg(ARG_KONFIRMASI_PASSWORD)))
    {
      error_message += "Konfirmasi password tidak sesuai.\\n";
    }

    if (error_message == "")
    {
      if (pengaturan.readUsername() != server.arg(form.getArg(ARG_USERNAME)))
      {
        pengaturan.writeUsername(server.arg(form.getArg(ARG_USERNAME)));
      }

      if (pengaturan.readPassword() != server.arg(form.getArg(ARG_PASSWORD)))
      {
        pengaturan.writePassword(server.arg(form.getArg(ARG_PASSWORD)));
      }

      message = "Pengaturan pengguna telah tersimpan.";
    }
    else
    {
      message = error_message;
    }
  }
  server.send(200, "text/html", form.pengaturanPengguna("/pengguna", message, "/", "/pengguna", "/firmware", "/login?disconnect=1", pengaturan.readUsername(), pengaturan.readPassword(), pengaturan.readPassword()));
}

void handleFirmware()
{
  /*
  Spesifikasi :
  - Apabila pengguna telah login server membuka halaman /pengguna server akan memanggil prosedur ini,
    sehingga pengguna dapat masuk ke halaman informasi firmware.
  - Apabila pengguna belum login server akan memanggil prosedur handleLogin(),
    sehingga pengguna diminta untuk login terlebih dahulu sebelum masuk ke halaman ini.
  */

  String message = "";
  if (!is_authentified())
  {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }

  FIRMWARE_IN informasi_firmware;
  informasi_firmware.firmwareName = firmware_name;
  informasi_firmware.firmwareDescription = firmware_description;
  informasi_firmware.firmwareVersion = firmware_version;
  informasi_firmware.legalCopyright = legal_copyright;
  informasi_firmware.companyName = company_name;
  informasi_firmware.firmwareType = firmware_type;

  t_ip_addr = prefix;
  t_ip_addr += "/";
  t_ip_addr += firmware_name;
  t_ip_addr += "/";
  t_ip_addr += chip_id;
  t_ip_addr += "/ip_addr";

  t_nama_alat = prefix;
  t_nama_alat += "/";
  t_nama_alat += firmware_name;
  t_nama_alat += "/";
  t_nama_alat += chip_id;
  t_nama_alat += "/nama_alat";

  t_chipid = prefix;
  t_chipid += "/";
  t_chipid += firmware_name;
  t_chipid += "/";
  t_chipid += chip_id;
  t_chipid += "/chipid";

  t_buzzer += "buzzer";
  t_buzzer += "/";
  t_buzzer += chip_id;

  t_kondisi = prefix;
  t_kondisi += "/";
  t_kondisi += firmware_name;
  t_kondisi += "/";
  t_kondisi += chip_id;
  t_kondisi += "/kondisibuzzer";

  t_status = prefix;
  t_status += "/";
  t_status += firmware_name;
  t_status += "/";
  t_status += chip_id;
  t_status += "/status";


  String list_topic = "<br><br>List Topic MQTT : <br>";
  list_topic += t_ip_addr;
  list_topic += "<br>";
  list_topic += t_nama_alat;
  list_topic += "<br>";
  list_topic += t_chipid;
  list_topic += "<br>";
  list_topic += t_buzzer;
  list_topic += "<br>";
  list_topic += t_kondisi;
  list_topic += "<br>";
  list_topic += t_status;
  server.send(200, "text/html", form.informasiFirmware(informasi_firmware, list_topic, "/", "/pengguna", "/firmware", "/login?disconnect=1"));
}

void handleNotFound()
{
  /*
  Spesifikasi :
  - Apabila pengguna membuka halaman selain /login, /, dan /pengaturan server akan menjalankan prosedur ini,
    sehingga akan muncul status "Halaman tida ditemukan." di web browser.
  */

  String message = "Halaman tidak ditemukan.\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
//------------------------------------------------------------------------------

void setup_wifi()
{
  /*
  Spesifikasi :
  - Prosedur ini digunakan untuk keneksi perangkat ke Wi-Fi.
  */

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(pengaturan.readWifiSSID());
  WiFi.begin(pengaturan.readWifiSSID().c_str(), pengaturan.readWifiPassword().c_str());
  

  while (WiFi.status() != WL_CONNECTED)
  {
    if (jumper.read() == HIGH)
    {
      ESP.restart();
    }
    lcd.setCursor(0, 0);
    lcd.print("Connecting To");
    lcd.setCursor(0, 1);
    lcd.print(pengaturan.readWifiSSID());
    lcd.clear();
    Serial.print(".");
    digitalWrite(ledR, HIGH);
    digitalWrite(ledG, HIGH);
    delay(500);
    digitalWrite(ledR, LOW);
    digitalWrite(ledG, LOW);
    
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  ip_device = WiFi.localIP().toString();

  lcd.setCursor(0, 0);
  lcd.print("Connected To");
  lcd.setCursor(0, 1);
  lcd.print(pengaturan.readWifiSSID());
  delay(500);
  lcd.clear();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  /*
  Spesifikasi :
  - Prosedur ini akan terpanggil apabila ada perubahan topic yang telah di subscribe perangkat.
  */

  // Serial.print("Message arrived [");
  // Serial.print(topic);
  // Serial.print("] ");

  String _topic = topic;

  for (unsigned int i = 0; i < length; i++)
  {
    //Serial.print((char)payload[i]);
    //(char) payload[i];
  }
  Serial.println();

  if (_topic.indexOf("5783528") != -1)
  {
    if ((char)payload[0] == '1')
    {
      //sw.setClosedCircuit();
      lcd.setCursor(10, 0);
      lcd.print("             ");
      digitalWrite(relay, LOW);
      digitalWrite(ledG, HIGH);
      digitalWrite(ledR, LOW);
            MQTT.publish(t_kondisi.c_str(), "ON");
      lcd.setCursor(0, 0);
      lcd.print("Buzzer : ON");
      
    }
    else
    {
      lcd.setCursor(10, 0);
      lcd.print("          ");
      //sw.setOpenCircuit();
      digitalWrite(relay, HIGH);
      digitalWrite(ledG, LOW);
      digitalWrite(ledR, HIGH);
      MQTT.publish(t_kondisi.c_str(), "OFF");
      lcd.setCursor(0, 0);
      lcd.print("Buzzer : OFF");
    }
  }
}

void reconnect()
{
  /*
  Spesifikasi :
  - Apabila koneksi MQTT Broker terputus perangkat akan memanggil prosedur ini.
  */

  while (!MQTT.connected())
  {
    if (jumper.read() == HIGH)
    {
      ESP.restart();
    }

    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (MQTT.connect(clientId.c_str(), t_status.c_str(), 2, 1, "offline"))
    {
      Serial.println("connected");
      MQTT.publish(t_status.c_str(), "online", 1);
      //MQTT.subscribe(t_buzzer.c_str());
      MQTT.subscribe("buzzer/5783528");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(MQTT.state());
      Serial.println(" try again in 1 seconds");
    }

    // led.setOn();
    // delay(500);
    // led.setOff();
    // delay(500);
  }
}
//------------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledK, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("WELCOME");
  lcd.setCursor(1, 1);
  lcd.print("HONICEL - TEAM");
  delay(500);
  lcd.clear();

  // if (!rtc.begin())
  // {
  //   Serial.println("Couldn't find RTC");
  //   Serial.flush();
  //   while (1)
  //     delay(10);
  // }
  // if (rtc.lostPower())
  // {
  //   Serial.println("RTC lost power, let's set the time!");
  //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // }

  randomSeed(micros()); // Inisialisasi random
  //sw.setPin(D5);
  pengaturan.begin();

  /*
  Inisialisasi Username & Password
  Dilakukan ketika username & password belum di-set
  kondisi tersebut ketika menggunakan device baru yang memiliki data eeprom yg masih kosong.
  */
  // pengaturan.writeUsername("admin");
  // pengaturan.writePassword("admin");
  //---------------------------
  chip_id = String(ESP.getChipId());
  // chip_id = "10754670"; // yang lama rusak terpaksa dibuat fix dulu
  Serial.println(chip_id);
  ap_ssid += chip_id;
  prefix = pengaturan.readPrefix();
  interval = pengaturan.readInterval().toInt();
  nama_alat = pengaturan.readDeviceName();

  t_ip_addr = prefix;
  t_ip_addr += "/";
  t_ip_addr += firmware_name;
  t_ip_addr += "/";
  t_ip_addr += chip_id;
  t_ip_addr += "/ip_addr";

  t_nama_alat = prefix;
  t_nama_alat += "/";
  t_nama_alat += firmware_name;
  t_nama_alat += "/";
  t_nama_alat += chip_id;
  t_nama_alat += "/nama_alat";

  t_chipid = prefix;
  t_chipid += "/";
  t_chipid += firmware_name;
  t_chipid += "/";
  t_chipid += chip_id;
  t_chipid += "/chipid";

  t_buzzer += "buzzer";
  t_buzzer += "/";
  t_buzzer += chip_id;

  t_kondisi = prefix;
  t_kondisi += "/";
  t_kondisi += firmware_name;
  t_kondisi += "/";
  t_kondisi += chip_id;
  t_kondisi += "/kondisibuzzer";

  t_status = prefix;
  t_status += "/";
  t_status += firmware_name;
  t_status += "/";
  t_status += chip_id;
  t_status += "/status";

  Serial.println(t_ip_addr);
  Serial.println(t_nama_alat);
  Serial.println(t_chipid);
  Serial.println(t_buzzer);
  Serial.println(t_kondisi);
  Serial.println(t_status);

  if (jumper.read() == HIGH)
  {
    WiFi.softAP(ap_ssid.c_str(), ap_password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Please Setting");
    lcd.setCursor(0, 1);
    lcd.print("To : ");
    lcd.print(myIP);
    session = generateSession();
    server.on("/", handleKoneksi);
    server.on("/pengguna", handlePengguna);
    server.on("/firmware", handleFirmware);
    server.on("/login", handleLogin);
    server.onNotFound(handleNotFound);

    const char *headerkeys[] = {"User-Agent", "Cookie"};
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char *);
    server.collectHeaders(headerkeys, headerkeyssize);
    server.begin();
  }
  else
  {
    setup_wifi();
    String broker = pengaturan.readMQTTBroker();
    Serial.println(broker.c_str());
    MQTT.setServer(strdup(broker.c_str()), 1883);
    MQTT.setCallback(callback);

    /*
    // memunculkan laman web di normal operation
    session = generateSession();
    server.on("/", handleKoneksi);
    server.on("/pengguna", handlePengguna);
    server.on("/firmware", handleFirmware);
    server.on("/login", handleLogin);
    server.onNotFound(handleNotFound);
    const char *headerkeys[] = {"User-Agent", "Cookie"};
    size_t headerkeyssize = sizeof(headerkeys) / sizeof(char *);
    server.collectHeaders(headerkeys, headerkeyssize);
    server.begin();
    // selesai memunculkan laman web di normal operation
    */
  }
  led.blink();

  /*
  setting untuk OTA
  */
  ArduinoOTA.onStart([]()
                     {
                       String type;
                       if (ArduinoOTA.getCommand() == U_FLASH)
                       {
                         type = "sketch";
                       }
                       else
                       { // U_FS
                         type = "filesystem";
                       }
                       // NOTE: if updating FS this would be the place to unmount FS using FS.end()
                       Serial.println("Start updating " + type); });
  ArduinoOTA.onEnd([]()
                   { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
                       Serial.printf("Error[%u]: ", error);
                       if (error == OTA_AUTH_ERROR)
                       {
                         Serial.println("Auth Failed");
                       }
                       else if (error == OTA_BEGIN_ERROR)
                       {
                         Serial.println("Begin Failed");
                       }
                       else if (error == OTA_CONNECT_ERROR)
                       {
                         Serial.println("Connect Failed");
                       }
                       else if (error == OTA_RECEIVE_ERROR)
                       {
                         Serial.println("Receive Failed");
                       }
                       else if (error == OTA_END_ERROR)
                       {
                         Serial.println("End Failed");
                       } });
  ArduinoOTA.begin();
}

//int counter = 0;

void loop()
{
  // Arduino OTA Update
  ArduinoOTA.handle();

  

  if (jumper.read() == LOW)
  {
    server.handleClient();
    // Jika MQTT tidak Terkoneksi
    if (!MQTT.connected())
    {
      // ESP.restart();
      reconnect();
    }
    String chpid = pengaturan.readChipIDD();
        String msin = pengaturan.readDeviceName();
    // if (counter < interval)
    // {
    //   counter++;
    // }
    // else
    // {
       //String con1, con2, con3, con4, con5, con6, con7, con8, con9, con10;
      static unsigned long last = 0;
      if ((millis() - last) >= 2000)
      {
        last = millis();
        String data = "";
        
        while(Serial.available()>0)
        {
          data += char(Serial.read());
        }
        data.trim();
        if(data != ""){
          int index;
          for(int i=0; i<=data.length(); i++)
          {
            char delimiter = '#';
            if(data[i] != delimiter)
            {
              arraydata[index] += data[i];
            }
            else 
            {
              index++;
            }
          }
          if(index >= 0)
          {
            //tampilkan data
            String con1 = arraydata[0];
            String con2 = arraydata[1];
            String con3 = arraydata[2];
            String con4 = arraydata[3];
            String con5 = arraydata[4];
            String con6 = arraydata[5];
            String con7 = arraydata[6];
            String con8 = arraydata[7];
            String con9 = arraydata[8];
            String con10 = arraydata[9];
            String con11 = arraydata[10];
            
            Serial.print("mesin1: ");
            Serial.println(con1);
            Serial.print("mesin2: ");
            Serial.println(con2);
            Serial.print("mesin3: ");
            Serial.println(con3);
            Serial.print("mesin4: ");
            Serial.println(con4);
            Serial.print("mesin5: ");
            Serial.println(con5);
            Serial.print("mesin6: ");
            Serial.println(con6);
            Serial.print("mesin7: ");
            Serial.println(con7);
            Serial.print("mesin8: ");
            Serial.println(con8);
            Serial.print("mesin9: ");
            Serial.println(con9);
            Serial.print("mesin10: ");
            Serial.println(con10);
            digitalWrite(ledK, HIGH);
            
            char buffer[256];
            const int capacity = JSON_ARRAY_SIZE(10) + 2 * JSON_OBJECT_SIZE(10);
            StaticJsonDocument<capacity> doc;

            JsonObject obj = doc.createNestedObject();
            obj["chip_id"] = "5783528"; //2462610, 6425973, 5783528
            obj["mesin"] = "siku"; //laminating, eck, siku

            JsonArray nilai = obj.createNestedArray("nilai");
            nilai.add(con1);
            nilai.add(con2);
            nilai.add(con3);
            nilai.add(con4);
            nilai.add(con5);
            nilai.add(con6);
            nilai.add(con7);
            nilai.add(con8);
            nilai.add(con9);
            nilai.add(con10);
            size_t n = serializeJson(doc, buffer);
            // lastMsg = now;
            
            ++value;
            snprintf(msg, MSG_BUFFER_SIZE, "%ld", value);
            Serial.print("Publish message: ");
            Serial.println(n);
            MQTT.publish("mgdm/ct", buffer, n);
           // MQTT.publish(t_ip_addr.c_str(), ip_device.c_str());

          }
          arraydata[0] = "";
          arraydata[1] = "";
          arraydata[2] = "";
          arraydata[3] = "";
          arraydata[4] = "";
          arraydata[5] = "";
          arraydata[6] = "";
          arraydata[7] = "";
          arraydata[8] = "";
          arraydata[9] = "";
          
          digitalWrite(ledK, HIGH);
            delay(500);
            digitalWrite(ledK, LOW);
        }
        counter = 0;
        last=millis();
        
        //send = millis();
        
        // digitalWrite(ledK, HIGH);
        // delay(200);
        // digitalWrite(ledK, LOW);
      }


      
    // }
    MQTT.loop();
    delay(1);
  }
  else
  {
    server.handleClient();
  }

  //Untuk melakukan konfigurasi dan restart nodemcu
  currentState = jumper.read();
  if (lastState == LOW && currentState == HIGH)
  {
    waktuTekan = millis();
  }
  else if (lastState == HIGH && currentState == LOW)
  {
    waktuLepas = millis();
    long lamaTekan = waktuLepas - waktuTekan;
    Serial.print("Press :");
    Serial.println(lamaTekan);
    //lcd.clear();
    // lcd.setCursor(0, 1);
    // lcd.print("Lama Tekan:");
    lcd.setCursor(11, 1);
    lcd.print(lamaTekan);
    delay(300);
    lcd.setCursor(11, 1);
    lcd.print("     ");
    // if (jumper.read() == HIGH)
    // if (lamaTekan >= start_opsi_Satu && lamaTekan <= stop_opsi_Satu)
    // {
    //   String chip_idd = String(ESP.getChipId());
    //     MQTT.publish(t_chipid.c_str(), String(chip_idd).c_str());
    //     MQTT.publish(t_ip_addr.c_str(), ip_device.c_str());
    //     MQTT.publish(t_nama_alat.c_str(), nama_alat.c_str());
    //     MQTT.publish(t_count.c_str(), String(count).c_str());

    //     Serial.print("sent :");
    //     Serial.println(count);
    // }
    if (lamaTekan >= start_opsi_Dua && lamaTekan <= stop_opsi_Dua)
    {
      if (!MQTT.connected())
      {
        reconnect();
      }
      WiFi.softAP(ap_ssid.c_str(), ap_password);
      IPAddress myIP = WiFi.softAPIP();
      Serial.print("AP IP address: ");
      Serial.println(myIP);
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("Setting in 20S");
      lcd.setCursor(0, 1);
      lcd.print("To : ");
      lcd.print(myIP);
      session = generateSession();
      server.on("/", handleKoneksi);
      server.on("/pengguna", handlePengguna);
      server.on("/firmware", handleFirmware);
      server.on("/login", handleLogin);
      server.onNotFound(handleNotFound);

      const char *headerkeys[] = {"User-Agent", "Cookie"};
      size_t headerkeyssize = sizeof(headerkeys) / sizeof(char *);
      server.collectHeaders(headerkeys, headerkeyssize);
      server.begin();
      // server.handleClient();
      
      delay(300);
      lcd.clear();
    }
    if (lamaTekan >= start_opsi_Tiga && lamaTekan <= stop_opsi_Tiga)
    {
     ESP.restart(); 
    }
    if (lamaTekan >= start_opsi_Empat && lamaTekan <= stop_opsi_Empat)
    {
      char buffer[256];
            const int capacity = JSON_ARRAY_SIZE(10) + 2 * JSON_OBJECT_SIZE(10);
            StaticJsonDocument<capacity> doc;

            JsonObject obj = doc.createNestedObject();
            obj["chip_id"] = "5783528";
            obj["nilai"] = "0";
            size_t n = serializeJson(doc, buffer);
            // lastMsg = now;
            ++value;
            snprintf(msg, MSG_BUFFER_SIZE, "%ld", value);
            Serial.print("Publish message: ");
            Serial.println(n);
            MQTT.publish("buzzer", buffer, n);
    }
  }
  lastState = currentState;

  //   currentState2 = jumper2.read();
  // if (lastState2 == LOW && currentState2 == HIGH)
  // {
  //   waktuTekan2 = millis();
  // }
  // else if (lastState2 == HIGH && currentState2 == LOW)
  // {
  //   waktuLepas2 = millis();
  //   long lamaTekan2 = waktuLepas2 - waktuTekan2;
  //   //Serial.print("Press :");
  //   //Serial.println(lamaTekan2);
  //   lcd.setCursor(11, 1);
  //   lcd.print(lamaTekan2);
  //   delay(300);
  //   lcd.setCursor(11, 1);
  //   lcd.print("     ");
  //   if (lamaTekan2 >= start_opsi_Empat)
  //   {
  //     digitalWrite(relay, HIGH);
  //     char buffer[256];
  //           const int capacity = JSON_ARRAY_SIZE(10) + 2 * JSON_OBJECT_SIZE(10);
  //           StaticJsonDocument<capacity> doc;

  //           JsonObject obj = doc.createNestedObject();
  //           obj["chip_id"] = chip_id;
  //           obj["nilai"] = "0";
  //           size_t n = serializeJson(doc, buffer);
  //           // lastMsg = now;
  //           ++value;
  //           snprintf(msg, MSG_BUFFER_SIZE, "%ld", value);
  //           Serial.print("Publish message: ");
  //           Serial.println(n);
  //           MQTT.publish("buzzer", buffer, n);
  //   }
  // }
  // lastState2 = currentState2;
}