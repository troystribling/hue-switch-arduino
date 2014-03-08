#include <SPI.h>
#include "Adafruit_CC3000.h"
#include "ccspi.h"
#include "log.h"
#include "hue_lights_client.h"


#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

#define WLAN_SECURITY   WLAN_SEC_WPA2

HueLightsClient::HueLightsClient(const __FlashStringHelper* _host,
                                 const __FlashStringHelper* _site_root,
                                 const char* _wlan_ssid,
                                 const char* _wlan_password) :
  cc3000(Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER)),
  host(_host),
  site_root(_site_root) {
  init(_wlan_ssid, _wlan_password)
}

HueLightsClient::init(const char* _wlan_ssid, const char* _wlan_password) {
  ipAddress = 0;

  HALT_ON_ERROR(cc3000.begin(), F("Couldn't begin()! Check your wiring?"))
  HALT_ON_ERROR(cc3000.connectToAP(_wlan_ssid, _wlan_password, WLAN_SECURITY), F("Connect Failed!"})
  DBUG("Connected");
  DBUG(F("Request DHCP"));
  while (!cc3000.checkDHCP()) {
    delay(100);
  }
  while (!displayConnectionDetails()) {
    delay(100);
  }
  while (ipAddress == 0) {
    if (!cc3000.getHostByName(host, &ipAddress)) {
      ERROR(F("Couldn't resolve!"));
    }
    delay(500);
  }
  client = cc3000.connectTCP(ip, 80);
  if (www.connected()) {
    www.fastrprint(F("PUT "));
    www.fastrprint(WEBPAGE);
    www.fastrprint(F(" HTTP/1.1\r\n"));
    www.fastrprint(F("Host: ")); www.fastrprint(WEBSITE); www.fastrprint(F("\r\n"));
    www.fastrprint(F("Accept: application/json\r\n"));
    www.fastrprint(F("Content-Length: 11\r\n\r\n"));
    www.fastrprint(F("{\"on\":true}"));
    www.fastrprint(F("\r\n"));
    www.println();
  } else {
    Serial.println(F("Connection failed"));
    return;
  }

  Serial.println(F("-------------------------------------"));

  /* Read data until either the connection is closed, or the idle timeout is reached. */
  unsigned long lastRead = millis();
  while (www.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (www.available()) {
      char c = www.read();
      Serial.print(c);
      lastRead = millis();
    }
  }
  www.close();
  Serial.println(F("-------------------------------------"));

  /* You need to make sure to clean up after yourself or the CC3000 can freak out */
  /* the next time your try to connect ... */
  Serial.println(F("\n\nDisconnecting"));
  cc3000.disconnect();

}

bool displayConnectionDetails(void) {
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv)) {
    ERROR(F("Unable to retrieve the IP Address!\r\n"));
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
