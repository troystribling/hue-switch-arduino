#include <SPI.h>
#include "hue_lights_client.h"
#include "ccspi.h"
#include "log.h"

#define IDLE_TIMEOUT_MS  5000

#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

#define WLAN_SECURITY               WLAN_SEC_WPA2
#define HTTP_HEADERS                "Accept: application/json\r\nContent-Length: "
#define MAX_HTTP_CONNECT_TRIES      5

HueLightsClient::HueLightsClient(char*        _host,
                                 char*        _siteRoot) :
  cc3000(Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER)),
  host(_host),
  siteRoot(_siteRoot),
  numberOfLightsEEPROM(EEPROMObject<NumberOfLights>(NUMBER_OF_LIGHTS_OFFSET, MAX_NUMBER_OF_LIGHTS)),
  scenesEEPROM(EEPROMObject<HueLightsScene>(HUE_LIGHTS_SCENES_OFFSET, HUE_LIGHTS_MAX_SCENES)),
  currentSceneIDEEPROM(EEPROMObject<HueLightsCurrentSceneID>(HUE_CURRENT_SCENE_ID_OFFSET, MAX_NUMBER_OF_HUE_CURRENT_ID_SCENES)) {}

// network
bool HueLightsClient::lanConnect(const char*  _wlanSSID, const char*  _wlanPassword) {
  serverIpAddress = 0;
  HALT_ON_ERROR(cc3000.begin(), F("Couldn't begin()! Check your wiring?"));
  HALT_ON_ERROR(cc3000.connectToAP(_wlanSSID, _wlanPassword, WLAN_SECURITY), F("Connect Failed!"));
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

// commands
bool HueLightsClient::setLightOn(uint8_t lightID, bool on) {
  bool status = false;
  uint8_t count = 0;
  String url = String(siteRoot);
  url += String("/lights/");
  url += String(lightID);
  url += String("/state");
  String body;
  if (on) {
    body = "{\"on\":true}";
  } else {
    body = "{\"on\":false}";
  }
  String headers = String(HTTP_HEADERS);
  headers += String(body.length());
  headers += String("\r\n");
  while (count < MAX_HTTP_CONNECT_TRIES && !status) {
    if (httpRequest(F("PUT"), &url, &headers, &body)) {
      uint16_t httpStatus = readHTTPResponseStatus();
      if (httpStatus == 200) {
        DBUG_LOG(F("Set light on successful"));
        status = true;
      } else {
        ERROR_LOG(F("Set light on failed"));
      }
      siteClose();
    }
  }
  return status;
}

bool HueLightsClient::setAllLightsOn(bool on) {
  return true;
}

bool HueLightsClient::addScene(char* name) {

}

bool HueLightsClient::removeScene(uint8_t sceneID) {

}

bool HueLightsClient::nextScene() {

}

String HueLightsClient::getSceneName() {

}

bool HueLightsClient::setSceneName() {

}

uint8_t HueLightsClient::getSceneID() {

}

bool HueLightsClient::setSceneID(uint8_t sceneID) {

}

bool HueLightsClient::setLightColor(uint8_t lightID, const HueLight& light) {
  bool status = false;
  uint8_t count = 0;
  String url = String(siteRoot);
  url += String("/lights/");
  url += String(lightID);
  url += String("/state");
  String body = String("{\"bri\":");
  body += String(light.brightness);
  body += String(",\"hue\":");
  body += String(light.hue);
  body += String(",\"sat\":");
  body += String(light.saturation);
  body += String("}");
  String headers = String(HTTP_HEADERS);
  headers += String(body.length());
  headers += String("\r\n");
  while (count < MAX_HTTP_CONNECT_TRIES && !status) {
    if (httpRequest(F("PUT"), &url, &headers, &body)) {
      uint16_t httpStatus = readHTTPResponseStatus();
      if (httpStatus == 200) {
        DBUG_LOG(F("Set light color successful"));
        status = true;
      } else {
        ERROR_LOG(F("Set light color failed"));
      }
      siteClose();
    }
  }
  return status;
}

bool HueLightsClient::setLightCount() {
  bool status = false;
  uint8_t count = 0;
  String url = String(siteRoot);
  url += String("/lights");
  while (count < MAX_HTTP_CONNECT_TRIES && !status) {
    if (httpRequest(F("GET"), &url, NULL, NULL)) {
      uint16_t httpStatus = readHTTPResponseStatus();
      if (httpStatus == 200) {
        uint8_t lightCount = readHTTPLightsResponse();
        if (lightCount > 0) {
          DBUG_LOG(F("Light count:"));
          DBUG_LOG(lightCount, DEC);
          NumberOfLights count = {0x01, lightCount};
          numberOfLightsEEPROM.update(0, count);
          status = true;
        }
      } else {
        ERROR_LOG(F("Get light count failed"));
      }
      siteClose();
    }
  }
  return status;
}

uint8_t HueLightsClient::getLightCount() {
  NumberOfLights count;
  uint16_t bytes = numberOfLightsEEPROM.read(0, count);
  DBUG_LOG(F("Light count:"));
  DBUG_LOG(count.numberOfLights);
  return count.numberOfLights;
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

bool HueLightsClient::httpRequest(const __FlashStringHelper* method, String* url, String* headers, String* body) {
  uint8_t count = 0;
  bool status = false;
  DBUG_FREE_MEMORY;
  while (count < MAX_HTTP_CONNECT_TRIES && !status) {
    if (siteConnect()) {
      char urlBuffer[url->length()+1];
      url->toCharArray(urlBuffer, url->length()+1);
      DBUG_LOG(F("HTTP URL:"));
      DBUG_LOG(urlBuffer);
      DBUG_LOG(url->length(), DEC);
      client.fastrprint(method); client.fastrprint(" ");
      client.fastrprint(urlBuffer);
      client.fastrprint(F(" HTTP/1.1\r\n"));
      client.fastrprint(F("Host: ")); client.fastrprint(host); client.fastrprint(F("\r\n"));
      if (headers) {
        char headersBuffer[headers->length()+1];
        headers->toCharArray(headersBuffer, headers->length()+1);
        DBUG_LOG(F("HTTP HEADERS:"));
        DBUG_LOG(headersBuffer);
        DBUG_LOG(headers->length(), DEC);
        client.fastrprint(headersBuffer);
        client.fastrprint(F("\r\n"));
      }
      if (body) {
        char bodyBuffer[body->length()+1];
        body->toCharArray(bodyBuffer, body->length()+1);
        DBUG_LOG(F("HTTP BODY:"));
        DBUG_LOG(bodyBuffer);
        DBUG_LOG(body->length(), DEC);
        client.fastrprint(bodyBuffer);
      }
      client.fastrprint(F("\r\n"));
      client.println();
      status = true;
    } else {
      ERROR_LOG(F("Connection failed"));
      count++;
    }
  }
  DBUG_FREE_MEMORY;
  return status;
}

uint16_t HueLightsClient::readHTTPResponseStatus() {
  unsigned long lastRead = millis();
  uint16_t msgCount = 0, httpStatus = 0;
  char statusBuffer[4];
  statusBuffer[3] = '\0';
  while (client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (client.available()) {
      char c = client.read();
      // get status
      if (msgCount >= 9 && msgCount <= 11) {
        statusBuffer[msgCount - 9] = c;
      }
      if (msgCount >= 11) {
        break;
      }
      msgCount++;
      lastRead = millis();
    }
    if (msgCount >= 11) {
      break;
    }
  }
  httpStatus = atoi(statusBuffer);
  DBUG_LOG(F("HTTP Status:"));
  DBUG_LOG(httpStatus);
  return httpStatus;
}

uint8_t HueLightsClient::readHTTPLightsResponse() {
  unsigned long lastRead = millis();
  uint8_t count = 0, level = 0, quote = 0;
  while (client.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (client.available()) {
      char c = client.read();
      if (c == '{') {
        level++;
      } else if (c == '}') {
        level--;
      } else if (c == '"' && quote == 0 && level == 1) {
        quote = 1;
      } else if (c == '"' && quote == 1 && level == 1) {
        count++;
        quote = 0;
      }
      lastRead = millis();
    }
  }
  return count;
}

