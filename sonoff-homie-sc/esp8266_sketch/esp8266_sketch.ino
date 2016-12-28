#define MQTT_MAX_PACKET_SIZE 400

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

#define PIN_LED     13      // PIN 12 / HSPI_MOSI; UART0_CTS MTCK
#define FW_NAME "sonoff-sc"
#define FW_VERSION "1.0.0"

const int INTERVAL = 10000;
unsigned long lastSent = 0;

HomieNode temperatureNode("temperature", "temperature");
HomieNode humidityNode("humidity", "humidity");
HomieNode lightNode("light", "light");
//HomieNode noiseNode("noise", "noise");
//HomieNode dustyNode("dusty", "dusty");

void setupHandler() {
  temperatureNode.setProperty("unit").send("c");
}

String temp;
int pos, pos2;

String temperature;
String humidity;
String light;
String noise;
String dusty;

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
      if (data.charAt(i) == separator || i == maxIndex) {
          found++;
          strIndex[0] = strIndex[1] + 1;
          strIndex[1] = (i == maxIndex) ? i+1 : i;
      }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void loopHandler() {

  pos = pos2 = -1;
  
  temp = Serial.readString();

  temperature = getValue(temp, ',', 0);
  humidity = getValue(temp, ',', 1);
  light = getValue(temp, ',', 2);
  noise = getValue(temp, ',', 3);
  dusty = getValue(temp, ',', 4);
    
  if (millis() - lastSent >= INTERVAL || lastSent == 0) {        
    
    if(temperature.length() > 0) {
      temperatureNode.setProperty("value").send(temperature);
    }

    if(humidity.length() > 0) {
      humidityNode.setProperty("value").send(humidity);
    }

    if(light.length() > 0) {
      lightNode.setProperty("value").send(light);
    }

    /*
    if(noise.length() > 0) {
      noiseNode.setProperty("value").send(noise);
    }

    if(dusty.length() > 0) {
      dustyNode.setProperty("value").send(dusty);
    } */
    
    lastSent = millis();
  }
}


void setup() { 
  Serial.begin(19200);
  Serial.setTimeout(500);
  
  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie_setBrand(FW_NAME);
  
  Homie.disableLogging();
  Homie.setLedPin(PIN_LED, LOW);
  Homie.setSetupFunction(setupHandler).setLoopFunction(loopHandler);

  Homie.setup();
  
}

void loop() {
  Homie.loop();
}
