#include <Homie.h>
#include <ArduinoOTA.h>

#define PIN_LED     13      // PIN 12 / HSPI_MOSI; UART0_CTS MTCK
#define PIN_COLD 12
#define PIN_WARM 14

/**
  * Arduino IDE instructions
  *
  * select "generic ESP8266 Module"
  * 80Mhz CPU, 40Mhz Flash
  * Upload Speed 460800
  * 1M (64K SPIFFS)
  * ck
  *
*/

#define FW_NAME "sonoff-led"
#define FW_VERSION "1.0.0"

HomieNode lightNode("strip", "led");

bool white1Handler(const HomieRange& range, const String& value) {
  Homie.getLogger() << "w1 " << ": " << value << endl;
  int white = value.toInt();
  analogWrite(PIN_COLD, white * 4);

  lightNode.setProperty("white1").send(value);
  return true;
}

bool white2Handler(const HomieRange& range, const String& value) {
  Homie.getLogger() << "w2 " << ": " << value << endl;
  int white = value.toInt();
  analogWrite(PIN_WARM, white * 4);

  lightNode.setProperty("white2").send(value);
  return true;
}


void setup() { 
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie_setBrand(FW_NAME);
  
  pinMode(PIN_COLD, OUTPUT);
  pinMode(PIN_WARM, OUTPUT);
  
  lightNode.advertise("white1").settable(white1Handler);
  lightNode.advertise("white2").settable(white2Handler);
  
  Homie.setup();
  ArduinoOTA.setHostname(Homie.getConfiguration().deviceId);
  ArduinoOTA.begin();
}

void loop() {
  Homie.loop();
  ArduinoOTA.handle();
}
