#include "HTMLForm.h"
#include <Arduino.h>

// Public
String HTMLForm::login(String target, String message)
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membuat kode HTML halaman login.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  String html = "";
  html += "<html>";
  html += "<head>";
  html += "<title>Honicel IOT</title>";
  html += "</head>";
  html += "<body>";
  html += "<form action=\"/login\" method=\"post\">";
  html += "<table>";
  html += "<tr>";
  html += "<td>Username : </td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_USERNAME) + "\" placeholder=\"Username\" maxlength=\"100\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Password : </td>";
  html += "<td><input type=\"password\" name=\"" + getArg(ARG_PASSWORD) + "\" placeholder=\"Password\" maxlength=\"100\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td><button type=\"submit\">Login</button></td>";
  html += "<td></td>";
  html += "</tr>";
  html += "</table>";
  html += "</form>";

  if (message != "")
  {
    html += "<script type=\"text/javascript\">alert(\"";
    html += message;
    html += "\");</script>";
  }
  html += "</body>";
  html += "</html>";
  return html;
}

String HTMLForm::pengaturanKoneksi(String target, String message, String link_pengaturan_koneksi, String link_pengaturan_pengguna, String link_informasi_firmware, String link_logout, String wifi_ssid, String wifi_password, String mqtt_broker, String device_name, String prefix, String interval, String nama_header, String code_header1, String code_header2, String code_header3, String code_header4, String code_header5, String code_header6, String code_header7, String code_header8, String chipid_device)
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membuat kode HTML halaman pengaturan koneksi.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  String html = "";
  html += "<html>";
  html += "<head>";
  html += "<title>Honicel IOT</title>";
  html += "</head>";
  html += "<body>";
  html += "<a href=\"" + link_pengaturan_koneksi + "\">Pengaturan Koneksi</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_pengaturan_pengguna + "\">Pengaturan Pengguna</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_informasi_firmware + "\">Informasi Firmware</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_logout + "\">Logout</a>";
  html += "<br>";
  html += "<br>";
  html += "<form action=\"" + link_pengaturan_koneksi + "\" method=\"post\">";
  html += "Pengaturan Koneksi";
  html += "<table>";
  html += "<tr>";
  html += "<td>Wi-FI SSID </td>";
  html += "<td>:</td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_WIFI_SSID) + "\" placeholder=\"Wi-FI SSID\" value=\"" + wifi_ssid + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Wi-FI Password </td>";
  html += "<td>:</td>";
  html += "<td><input type=\"password\" name=\"" + getArg(ARG_WIFI_PASSWORD) + "\" placeholder=\"Wi-FI Password\" value=\"" + wifi_password + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>MQTT Broker</td>";
  html += "<td>:</td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_MQTT_BROKER) + "\" Placeholder=\"MQTT Broker\" value=\"" + mqtt_broker + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Device Name</td>";
  html += "<td>:</td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_DEVICE_NAME) + "\" Placeholder=\"Device Name\" value=\"" + device_name + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Prefix</td>";
  html += "<td>:</td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_PREFIX) + "\" Placeholder=\"Prefix\" value=\"" + prefix + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Interval</td>";
  html += "<td>:</td>";
  html += "<td><input type=\"number\" name=\"" + getArg(ARG_INTERVAL) + "\" Placeholder=\"Interval\" value=\"" + interval + "\"></td>";
  html += "</tr>";
  // html += "<td>Nama_Header</td>";
  // html += "<td>:</td>";
  // html += "<td><input type=\"text\" name=\"" + getArg(ARG_NAMA_HEADER) + "\" Placeholder=\"Nama_Header\" value=\"" + nama_header + "\"></td>";
  // html += "</tr>";
  // html += "<td>Code_Header1</td>";
  // html += "<td>:</td>";
  // html += "<td><input type=\"text\" name=\"" + getArg(ARG_CODE_HEADER1) + "\" Placeholder=\"Code_Header1\" value=\"" + code_header1 + "\"></td>";
  // html += "</tr>";
  // html += "<td>Code_Header2</td>";
  // html += "<td>:</td>";
  // html += "<td><input type=\"text\" name=\"" + getArg(ARG_CODE_HEADER2) + "\" Placeholder=\"Code_Header2\" value=\"" + code_header2 + "\"></td>";
  // html += "</tr>";
  // html += "<td>Code_Header3</td>";
  // html += "<td>:</td>";
  // html += "<td><input type=\"text\" name=\"" + getArg(ARG_CODE_HEADER3) + "\" Placeholder=\"Code_Header3\" value=\"" + code_header3 + "\"></td>";
  // html += "</tr>";
  // html += "<td>Code_Header4</td>";
  // html += "<td>:</td>";
  // html += "<td><input type=\"text\" name=\"" + getArg(ARG_CODE_HEADER4) + "\" Placeholder=\"Code_Header4\" value=\"" + code_header4 + "\"></td>";
  // html += "</tr>";
  // html += "<td>Code_Header5</td>";
  // html += "<td>:</td>";
  // html += "<td><input type=\"text\" name=\"" + getArg(ARG_CODE_HEADER5) + "\" Placeholder=\"CodeHeader5\" value=\"" + code_header5 + "\"></td>";
  // html += "</tr>";
  // html += "<td>Code_Header6</td>";
  // html += "<td>:</td>";
  // html += "<td><input type=\"text\" name=\"" + getArg(ARG_CODE_HEADER6) + "\" Placeholder=\"CodeHeader6\" value=\"" + code_header6 + "\"></td>";
  // html += "</tr>";
  // html += "<td>Code_Header7</td>";
  // html += "<td>:</td>";
  // html += "<td><input type=\"text\" name=\"" + getArg(ARG_CODE_HEADER7) + "\" Placeholder=\"CodeHeader7\" value=\"" + code_header7 + "\"></td>";
  // html += "</tr>";
  // html += "<td>Key Device</td>";
  // html += "<td>:</td>";
  // html += "<td><input type=\"text\" name=\"" + getArg(ARG_CODE_HEADER8) + "\" Placeholder=\"Key_Device\" value=\"" + code_header8 + "\"></td>";
  // html += "</tr>";
  html += "<td>Chipid_device</td>";
  html += "<td>:</td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_CHIP_IDD) + "\" Placeholder=\"Chipid_device\" value=\"" + ESP.getChipId() + "\"></td>";
  html += "</tr>";
  html += "</table>";
  html += "<button type=\"submit\">Simpan</button>";
  html += "</form>";
  if (message != "")
  {
    html += "<script type=\"text/javascript\">alert(\"";
    html += message;
    html += "\");</script>";
  }
  html += "</body>";
  html += "</html>";
  return html;
}
// chipid_device
String HTMLForm::pengaturanPengguna(String target, String message, String link_pengaturan_koneksi, String link_pengaturan_pengguna, String link_informasi_firmware, String link_logout, String username, String password, String konfirmasi_password)
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membuat kode HTML halaman pengaturan pengguna.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  String html = "";
  html += "<html>";
  html += "<head>";
  html += "<title>Honicel IOT</title>";
  html += "</head>";
  html += "<body>";
  html += "<a href=\"" + link_pengaturan_koneksi + "\">Pengaturan Koneksi</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_pengaturan_pengguna + "\">Pengaturan Pengguna</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_informasi_firmware + "\">Informasi Firmware</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_logout + "\">Logout</a>";
  html += "<br>";
  html += "<br>";
  html += "<form action=\"" + link_pengaturan_pengguna + "\" method=\"post\">";
  html += "Pengaturan Pengguna";
  html += "<table>";
  html += "<tr>";
  html += "<td>Username </td>";
  html += "<td>:</td>";
  html += "<td><input type=\"text\" name=\"" + getArg(ARG_USERNAME) + "\" placeholder=\"Wi-FI SSID\" value=\"" + username + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Password </td>";
  html += "<td>:</td>";
  html += "<td><input type=\"password\" name=\"" + getArg(ARG_PASSWORD) + "\" placeholder=\"Wi-FI Password\" value=\"" + password + "\"></td>";
  html += "</tr>";
  html += "<tr>";
  html += "<td>Konfirmasi Password </td>";
  html += "<td>:</td>";
  html += "<td><input type=\"password\" name=\"" + getArg(ARG_KONFIRMASI_PASSWORD) + "\" placeholder=\"Wi-FI Password\" value=\"" + password + "\"></td>";
  html += "</tr>";
  html += "</table>";
  html += "<button type=\"submit\">Simpan</button>";
  html += "</form>";
  if (message != "")
  {
    html += "<script type=\"text/javascript\">alert(\"";
    html += message;
    html += "\");</script>";
  }
  html += "</body>";
  html += "</html>";
  return html;
}

String HTMLForm::informasiFirmware(FIRMWARE_IN informasi_firmware, String list_topic, String link_pengaturan_koneksi, String link_pengaturan_pengguna, String link_informasi_firmware, String link_logout)
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk membuat kode HTML halaman informasi firmware.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  String html = "";
  html += "<html>";
  html += "<head>";
  html += "<title>Honicel IOT</title>";
  html += "</head>";
  html += "<body>";
  html += "<a href=\"" + link_pengaturan_koneksi + "\">Pengaturan Koneksi</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_pengaturan_pengguna + "\">Pengaturan Pengguna</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_informasi_firmware + "\">Informasi Firmware</a>&nbsp;&nbsp;";
  html += "<a href=\"" + link_logout + "\">Logout</a>";
  html += "<br>";
  html += "<br>";
  html += "Informasi Firmware<br>";
  html += "<br>Firmware Name : ";
  html += informasi_firmware.firmwareName;
  html += "<br>Firmware Description : ";
  html += informasi_firmware.firmwareDescription;
  html += "<br>Firmware Version : ";
  html += informasi_firmware.firmwareVersion;
  html += "<br>Legal Copyright : ";
  html += informasi_firmware.legalCopyright;
  html += "<br>Company Name : ";
  html += informasi_firmware.companyName;
  html += "<br>Firmware Type : ";
  html += informasi_firmware.firmwareType;
  html += list_topic;
  html += "</body>";
  html += "</html>";
  return html;
}

String HTMLForm::getArg(HTML_FORM_ARG arg)
{
  /*
  Spesifikasi :
  - Fungsi ini digunakan untuk mendapatkan arg pada Form HTML.
  - Keluaran dari fungsi ini berupa objek kelas String.
  */

  return htmlFormArg[arg];
}
//------------------------------------------------------------------------------
