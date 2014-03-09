#include <SPI.h>
#include "hue_lights_client.h"
#include "ccspi.h"
#include "log.h"

#define IDLE_TIMEOUT_MS  3000

#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

#define WLAN_SECURITY   WLAN_SEC_WPA2

HueLightsClient::HueLightsClient(char*        _host,
                                 char*        _site_root,
                                 const char*  _wlan_ssid,
                                 const char*  _wlan_password) :
  cc3000(Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER)),
  host(_host),
  site_root(_site_root) {
  init(_wlan_ssid, _wlan_password);
}

bool HueLightsClient::connect() {
  if (ipAddress != 0) {
    client = cc3000.connectTCP(ipAddress, 80);
  }
  return client.connected();
}

bool HueLightsClient::disconnect() {
  if (client.connected()) {
    cc3000.disconnect();
  }
  return client.connected();
}

bool HueLightsClient::turnOffLight(uint8_t light) {
  if (client.connected()) {
    client.fastrprint(F("PUT "));
    client.fastrprint(site_root);
    client.fastrprint(F(" HTTP/1.1\r\n"));
    client.fastrprint(F("Host: ")); client.fastrprint(host); client.fastrprint(F("\r\n"));
    client.fastrprint(F("Accept: application/json\r\n"));
    client.fastrprint(F("Content-Length: 11\r\n\r\n"));
    client.fastrprint(F("{\"on\":true}"));
    client.fastrprint(F("\r\n"));
    client.println();
  } else {
    ERROR_LOG(F("Connection failed"));
    return false;
  }

  /* Read data until either the connection is closed, or the idle timeout is reached. */
  unsigned long lastRead = millis();
  while (client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (client.available()) {
      char c = client.read();
      Serial.print(c);
      lastRead = millis();
    }
  }
  client.close();
  return true;
}

void HueLightsClient::init(const char* _wlan_ssid, const char* _wlan_password) {
  ipAddress = 0;

  HALT_ON_ERROR(cc3000.begin(), F("Couldn't begin()! Check your wiring?"));
  HALT_ON_ERROR(cc3000.connectToAP(_wlan_ssid, _wlan_password, WLAN_SECURITY), F("Connect Failed!"));
  DBUG_LOG(F("Connected, Request DHCP"));
  while (!cc3000.checkDHCP()) {
    delay(100);
  }
  while (!displayConnectionDetails()) {
    delay(100);
  }
  while (ipAddress == 0) {
    if (!cc3000.getHostByName(host, &ipAddress)) {
      ERROR_LOG(F("Couldn't resolve!"));
    }
    delay(500);
  }
}

bool HueLightsClient::displayConnectionDetails() {
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv)) {
    ERROR_LOG(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  } else {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}
