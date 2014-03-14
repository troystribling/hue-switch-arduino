#include <SPI.h>
#include <EEPROM.h>
#include "hue_lights_client.h"
#include "ccspi.h"
#include "log.h"

#define IDLE_TIMEOUT_MS  5000

#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

#define WLAN_SECURITY   WLAN_SEC_WPA2

HueLightsClient::HueLightsClient(char*        _host,
                                 char*        _site_root) :
  cc3000(Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER)),
  host(_host),
  site_root(_site_root) {}

bool HueLightsClient::lanConnect(const char*  _wlan_ssid, const char*  _wlan_password) {
  serverIpAddress = 0;
  HALT_ON_ERROR(cc3000.begin(), F("Couldn't begin()! Check your wiring?"));
  HALT_ON_ERROR(cc3000.connectToAP(_wlan_ssid, _wlan_password, WLAN_SECURITY), F("Connect Failed!"));
  DBUG_OUT("\n");
  DBUG_LOG(F("Connected to LAN, Doing DHCP Request"));
  while (!cc3000.checkDHCP()) {
    delay(100);
  }
  while (!displayConnectionDetails()) {
    delay(100);
  }
  while (serverIpAddress == 0) {
    if (!cc3000.getHostByName(host, &serverIpAddress)) {
      ERROR_LOG(F("Couldn't resolve!"));
    }
    delay(500);
  }
  DBUG_LOG(F("Server IP Resolved:"));
  DBUG_LOG(serverIpAddress, HEX);
}

bool HueLightsClient::lanConnected() {
  return cc3000.checkConnected();
}

bool HueLightsClient::lanDisconnect() {
  if (lanConnected()) {
    return cc3000.disconnect();
  } else {
    return false;
  }
}

// private
bool HueLightsClient::siteConnect() {
  if (serverIpAddress != 0) {
    DBUG_LOG(F("Attempting connect to server:"));
    DBUG_LOG(serverIpAddress, HEX);
    client = cc3000.connectTCP(serverIpAddress, 80);
    return client.connected();
  } else {
    ERROR_LOG(F("Cannot connect to server becuase server IP is not available:"));
    ERROR_LOG(serverIpAddress, HEX);
  return false;
  }
}

bool HueLightsClient::siteClose() {
  if (client.connected()) {
    return client.close();
  } else {
    return false;
  }
}

bool HueLightsClient::displayConnectionDetails() {
  uint32_t address, netmask, gateway, dhcpserv, dnsserv;
  if(!cc3000.getIPAddress(&address, &netmask, &gateway, &dhcpserv, &dnsserv)) {
    ERROR_LOG(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  } else {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(address);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

bool HueLightsClient::httpRequest(const __FlashStringHelper* method, char* url, char* headers, char* body) {
  if (siteConnect()) {
    client.fastrprint(method); client.fastrprint(" ");
    client.fastrprint(url);
    client.fastrprint(F(" HTTP/1.1\r\n"));
    client.fastrprint(F("Host: ")); client.fastrprint(host); client.fastrprint(F("\r\n"));
    if (headers) {
      client.fastrprint(headers);
      client.fastrprint(F("\r\n"));
    }
    if (body) {
      client.fastrprint(body);
    }
    client.fastrprint(F("\r\n"));
    client.println();
  } else {
    ERROR_LOG(F("Connection failed"));
    return false;
  }
}

uint16_t HueLightsClient::readHTTPResponseStatus() {
  DBUG_LOG(F("HTTP Response"));
  unsigned long lastRead = millis();
  uint16_t count = 0;
  char status_buffer[4];
  status_buffer[3] = '\0';
  while (client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (client.available()) {
      char c = client.read();
      // get status
      if (count >= 9 && count <= 11) {
        status_buffer[count - 9] = c;
      }
      if (count >= 11) {
        break;
      }
      count++;
      DBUG_OUT(c);
      lastRead = millis();
    }
    if (count >= 11) {
      break;
    }
  }
  DBUG_LOG(F("Character Count:"));
  DBUG_LOG(count);
  DBUG_LOG(F("Status:"));
  DBUG_LOG(status_buffer);
  return atoi(status_buffer);
}

uint16_t HueLightsClient::readHTTPLightsResponse() {
  DBUG_LOG(F("HTTP Response"));
  unsigned long lastRead = millis();
  uint16_t count = 0;
  char status_buffer[4];
  status_buffer[3] = '\0';
  while (client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (client.available()) {
      char c = client.read();
      count++;
      DBUG_OUT(c);
      lastRead = millis();
    }
  }
  DBUG_LOG(F("Character Count:"));
  DBUG_LOG(count);
  DBUG_LOG(F("Status:"));
  DBUG_LOG(status_buffer);
  return atoi(status_buffer);
}

bool HueLightsClient::setLightOn(uint8_t light, bool on) {
  bool status = false;
  httpRequest(F("PUT"), "/api/6157582025/lights/1/state", "Accept: application/json\r\nContent-Length: 11\r\n", "{\"on\":true}");
  uint16_t httpStatus = readHTTPResponseStatus();
  if (httpStatus == 200) {
    status = true;
  }
  return status;
}

bool HueLightsClient::getLightCount() {
  bool status = false;
  httpRequest(F("GET"), "/api/6157582025/lights", NULL, NULL);
  uint16_t httpStatus = readHTTPResponseStatus();
  if (httpStatus == 200) {
    status = true;
  }
  return status;
}
