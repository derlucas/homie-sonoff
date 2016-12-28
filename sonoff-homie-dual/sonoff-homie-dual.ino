#include <Homie.h>

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

#define FW_NAME "sonoff-dual"
#define FW_VERSION "1.0.0"

boolean relay1 = false;
boolean relay2 = false;

HomieNode relayNode1("switch1", "switch");
HomieNode relayNode2("switch2", "switch");

void setRelays() {
  byte b = 0;
  if (relay1) b++;
  if (relay2) b += 2;
 
  SERIAL_PORT_HARDWARE.write((byte)0xA0);
  SERIAL_PORT_HARDWARE.write((byte)0x04);
  SERIAL_PORT_HARDWARE.write((byte)b);
  SERIAL_PORT_HARDWARE.write((byte)0xA1);;
  
  Serial.flush();
}

bool relayHandler1(const HomieRange& range, const String& value) {
  if (value != "true" && value != "false") return false;

  relay1 = (value == "true");

  setRelays();

  relayNode1.setProperty("on").send(value);
  return true;
}

bool relayHandler2(const HomieRange& range, const String& value) {
  if (value != "true" && value != "false") return false;

  relay2 = (value == "true");

  setRelays();
   
  relayNode2.setProperty("on").send(value);
  return true;
}

void setup() { 
  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie_setBrand(FW_NAME);
  Homie.disableLogging();
  relayNode1.advertise("on").settable(relayHandler1);
  relayNode2.advertise("on").settable(relayHandler2);
  Homie.setup();
  Serial.begin(19200);
  Serial.println("hello");
}

void loop() {
  Homie.loop();
}
