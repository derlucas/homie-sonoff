#include <Homie.h>

#define REL_PIN              12           // GPIO 12 = Red Led and Relay (0 = Off, 1 = On)

#define FW_NAME "sonoff-relay"
#define FW_VERSION "1.0.0"


HomieNode relayNode("switch", "switch");

bool relayHandler(const HomieRange& range, const String& value) {
  if (value != "true" && value != "false") return false;

  bool on = (value == "true");
  digitalWrite(REL_PIN, on ? HIGH : LOW);
  relayNode.setProperty("on").send(value);
  Homie.getLogger() << "switch is " << (on ? "on" : "off") << endl;

  return true;
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  
  pinMode(REL_PIN, OUTPUT);
  digitalWrite(REL_PIN, LOW);

  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie_setBrand("sonoff-switch");
  relayNode.advertise("on").settable(relayHandler);
  Homie.setup();
}

void loop() {
  Homie.loop();
}
