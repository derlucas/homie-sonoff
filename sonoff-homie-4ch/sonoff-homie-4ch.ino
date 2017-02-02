#include <Homie.h>
#include <ArduinoOTA.h>

#define PIN_LED     13      // PIN 12 / HSPI_MOSI; UART0_CTS MTCK
#define RELAY_1     12
#define RELAY_2      5
#define RELAY_3      4
#define RELAY_4     15
#define BUTTON_1     0
#define BUTTON_2     9
#define BUTTON_3    10
#define BUTTON_4    14

/**
  * Arduino IDE instructions
  *
  * select "generic ESP8285 Module"
  * 80Mhz CPU, 40Mhz Flash
  * Upload Speed 460800
  * 1M (64K SPIFFS)
  * ck
  *
*/

#define FW_NAME "sonoff-4ch"
#define FW_VERSION "1.0.0"

HomieNode relayNode1("switch1", "switch");
HomieNode relayNode2("switch2", "switch");
HomieNode relayNode3("switch3", "switch");
HomieNode relayNode4("switch4", "switch");
Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();
Bounce debouncer3 = Bounce();
Bounce debouncer4 = Bounce();


bool relayHandler1(const HomieRange& range, const String& value) {
  if (value != "true" && value != "false") return false;
  digitalWrite(RELAY_1, value == "true");  
  relayNode1.setProperty("on").send(value);
  return true;
}

bool relayHandler2(const HomieRange& range, const String& value) {
  if (value != "true" && value != "false") return false;
  digitalWrite(RELAY_2, value == "true");  
  relayNode2.setProperty("on").send(value);
  return true;
}

bool relayHandler3(const HomieRange& range, const String& value) {
  if (value != "true" && value != "false") return false;
  digitalWrite(RELAY_3, value == "true");  
  relayNode2.setProperty("on").send(value);
  return true;
}
bool relayHandler4(const HomieRange& range, const String& value) {
  if (value != "true" && value != "false") return false;
  digitalWrite(RELAY_4, value == "true");  
  relayNode2.setProperty("on").send(value);
  return true;
}

void toggleRelay(int pin, HomieNode& node) {
  bool on = digitalRead(pin) == HIGH;
  digitalWrite(pin, on ? LOW : HIGH);
  node.setProperty("on").send(on ? "false" : "true");
}

void loopHandler() {

  ArduinoOTA.handle();
  debouncer1.update();
  debouncer2.update();
  debouncer3.update();
  debouncer4.update();

  if(debouncer1.fell()) {
    toggleRelay(RELAY_1, relayNode1);
  }
  if(debouncer2.fell()) {
    toggleRelay(RELAY_2, relayNode2);
  }
  if(debouncer3.fell()) {
    toggleRelay(RELAY_3, relayNode3);
  }
  if(debouncer4.fell()) {
    toggleRelay(RELAY_4, relayNode4);
  }
  
}

void setup() { 
  Serial.begin(115200);
  Serial << endl << endl;
  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie_setBrand(FW_NAME);
  Homie.disableLogging();

  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
   
  debouncer1.attach(BUTTON_1, INPUT);
  debouncer1.interval(50);
  debouncer2.attach(BUTTON_2, INPUT);
  debouncer2.interval(50);
  debouncer3.attach(BUTTON_3, INPUT);
  debouncer3.interval(50);
  debouncer4.attach(BUTTON_4, INPUT);
  debouncer4.interval(50);
  
  relayNode1.advertise("on").settable(relayHandler1);
  relayNode2.advertise("on").settable(relayHandler2);
  relayNode3.advertise("on").settable(relayHandler3);
  relayNode4.advertise("on").settable(relayHandler4);
  
  Homie.setLedPin(PIN_LED, LOW);
  Homie.setLoopFunction(loopHandler);
  Homie.setResetTrigger(BUTTON_1, LOW, 10000);  // BTN0 = Flash = BTN_A set to 10sec
  Homie.setup();
  
  ArduinoOTA.setHostname(Homie.getConfiguration().deviceId);
  ArduinoOTA.begin();
}

void loop() {
  Homie.loop();
}
