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
void HueLightsClient::lanConnect(const char*  _wlanSSID, const char*  _wlanPassword) {
  serverIpAddress = 0;
  HALT_ON_ERROR(cc3000.begin(), F("Couldn't begin()! Check your wiring?"));
  HALT_ON_ERROR(cc3000.connectToAP(_wlanSSID, _wlanPassword, WLAN_SECURITY), F("Connect Failed!"));
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

// light commands
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
    delay(250);
    if (httpRequest(F("PUT"), &url, &headers, &body)) {
      uint16_t httpStatus = readHTTPResponseStatus();
      if (httpStatus == 200) {
        status = true;
      } else {
        ERROR_LOG(F("Set light on failed for id:"));
        ERROR_LOG(lightID);
      }
      siteClose();
    }
  }
  return status;
}

bool HueLightsClient::setAllLightsOn(bool on) {
  bool status = true;
  for (int i = 1; i <= getLightCount(); i++) {
    if (!setLightOn(i, on)) {
      status = false;
      break;
    }
  }
  return status;
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
        status = true;
      } else {
        ERROR_LOG(F("Set light color failed"));
      }
      siteClose();
    }
  }
  return status;
}

void HueLightsClient::setCurrentScene(uint8_t sceneID) {
  setCurrentSceneID(sceneID);
  HueLightsScene currentScene;
  scenesEEPROM.read(sceneID, currentScene);
  for (uint8_t i = 1; i <= getLightCount(); i++) {
    setLightColor(i, currentScene.lights[i]);
  }
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
  numberOfLightsEEPROM.read(0, count);
  return count.numberOfLights;
}

// scene commands
uint8_t HueLightsClient::createScene(const HueLightsScene& scene) {
  uint8_t index;
  scenesEEPROM.create(index, scene);
  setCurrentSceneID(index);
  return index;
}

void HueLightsClient::updateScene(uint8_t sceneID, const HueLightsScene& scene) {
  scenesEEPROM.update(sceneID, scene);
}

void HueLightsClient::removeScene(uint8_t sceneID) {
  scenesEEPROM.remove(sceneID);
}

void HueLightsClient::nextScene(uint8_t& sceneID, HueLightsScene& scene) {
  scenesEEPROM.next(sceneID, scene);
}

void HueLightsClient::getScene(uint8_t sceneID, HueLightsScene& scene) {
  scenesEEPROM.read(sceneID, scene);
}

uint8_t HueLightsClient::getSceneCount() {
  return scenesEEPROM.count();
}

uint8_t HueLightsClient::getCurrentSceneID() {
  HueLightsCurrentSceneID currentID;
  currentSceneIDEEPROM.read(0, currentID);
  return currentID.currentSceneID;
}

// private
bool HueLightsClient::siteConnect() {
  if (serverIpAddress != 0) {
    // hardware workaround
    delay(250);
    client = cc3000.connectTCP(serverIpAddress, 80);
    return client.connected();
  } else {
    ERROR_LOG(F("Cannot connect to server becuase server IP is not available:"));
    ERROR_LOG(serverIpAddress, HEX);
    return false;
  }
}

bool HueLightsClient::siteClose() {
  // hardware workaround
  delay(500);
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
  while (count < MAX_HTTP_CONNECT_TRIES && !status) {
    if (siteConnect()) {
      char urlBuffer[url->length()+1];
      url->toCharArray(urlBuffer, url->length()+1);
      client.fastrprint(method); client.fastrprint(" ");
      client.fastrprint(urlBuffer);
      client.fastrprint(F(" HTTP/1.1\r\n"));
      client.fastrprint(F("Host: ")); client.fastrprint(host); client.fastrprint(F("\r\n"));
      if (headers) {
        char headersBuffer[headers->length()+1];
        headers->toCharArray(headersBuffer, headers->length()+1);
        client.fastrprint(headersBuffer);
        client.fastrprint(F("\r\n"));
      }
      if (body) {
        char bodyBuffer[body->length()+1];
        body->toCharArray(bodyBuffer, body->length()+1);
        client.fastrprint(bodyBuffer);
      }
      client.fastrprint(F("\r\n"));
      client.println();
      status = true;
    } else {
      ERROR_LOG(F("Connection failed: count"));
      ERROR_LOG(count);
      siteClose();
      count++;
    }
  }
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

void  HueLightsClient::setCurrentSceneID(uint8_t sceneID) {
  HueLightsCurrentSceneID currentID = {0x01, sceneID};
  currentSceneIDEEPROM.update(0, currentID);
}

