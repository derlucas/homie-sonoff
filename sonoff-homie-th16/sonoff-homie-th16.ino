#include <Homie.h>
#include <ArduinoOTA.h>
#include "DHT.h"

/**
  * Arduino IDE instructions
  *
  * select "generic ESP8266 Module"
  * 80Mhz CPU, 40Mhz Flash
  * Upload Speed 460800
  * 1M (64K SPIFFS)
  * ck
  *
  * dependencies: Adafruit DHT sensor Library
*/

#define PIN_LED     13      // Pin 12 / HSPI_MOSI; UART0_CTS / GPI13
#define PIN_RELAY   12      // Pin 10 / MTDI / HSPIQ MISO / GPIO12 U0DR
#define PIN_BUTTON  0       // Pin 15 / GPIO 0

#define TEM1        14      // Pin 9 / MTMS / SCL / GPIO14
#define TEM2         4      // Pin 16 / GPIO4

#define DHTPIN      TEM1
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

#define FW_NAME "sonoff-th16"
#define FW_VERSION "1.0.0"

const int INTERVAL = 10000;
unsigned long lastSent = 0;
unsigned long buttonDownTime = 0;
byte lastButtonState = 1;
byte buttonPressHandled = 0;

DHT dht(DHTPIN, DHTTYPE);
HomieNode relayNode("switch", "switch");
HomieNode humidityNode("humidity", "humidity");
HomieNode temperatureNode("temperature", "temperature");
HomieNode heatIndexNode("heatindex", "hic");



void toggleRelay() {
  bool on = digitalRead(PIN_RELAY) == HIGH;
  digitalWrite(PIN_RELAY, on ? LOW : HIGH);
  relayNode.setProperty("on").send(on ? "false" : "true");
  Homie.getLogger() << "Switch is " << (on ? "off" : "on") << endl;
}

void loopHandler() {
  byte buttonState = digitalRead(PIN_BUTTON);
  if ( buttonState != lastButtonState ) {
    if (buttonState == LOW) {
      buttonDownTime = millis();
      buttonPressHandled = 0;
    }
    else {
      unsigned long dt = millis() - buttonDownTime;
      if ( dt >= 90 && dt <= 900 && buttonPressHandled == 0 ) {
        toggleRelay();
        buttonPressHandled = 1;
      }
    }
    lastButtonState = buttonState;
  }

  
 
  if (millis() - lastSent >= INTERVAL || lastSent == 0) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    if (isnan(h) || isnan(t)) {
      Homie.getLogger() << "Failed to read from DHT sensor!" << endl;
    } else {
      // Compute heat index in Celsius (isFahreheit = false)
      float hic = dht.computeHeatIndex(t, h, false);        
      temperatureNode.setProperty("temperature").send(String(t));
      humidityNode.setProperty("humidity").send(String(h));
      heatIndexNode.setProperty("hic").send(String(hic));
    }

    lastSent = millis();
   }

  ArduinoOTA.handle();
}


bool relayHandler(const HomieRange& range, const String& value) {
  if (value != "true" && value != "false") return false;

  bool on = (value == "true");
  digitalWrite(PIN_RELAY, on ? HIGH : LOW);
  relayNode.setProperty("on").send(value);
  Homie.getLogger() << "switch is " << (on ? "on" : "off") << endl;

  return true;
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  //Homie.getLogger() << "hello " << endl;
  Homie.disableLogging();
  
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  digitalWrite(PIN_RELAY, LOW);

  
  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie_setBrand(FW_NAME);
  Homie.setLedPin(PIN_LED, LOW);
  Homie.setLoopFunction(loopHandler);
  
  
  relayNode.advertise("on").settable(relayHandler);
  humidityNode.advertise("humidity");
  temperatureNode.advertise("temperature");
  heatIndexNode.advertise("heatindex");
  
  Homie.setup();

  ArduinoOTA.setHostname(Homie.getConfiguration().deviceId);
  ArduinoOTA.begin();

  dht.begin();
}

void loop() {
  Homie.loop();
}
