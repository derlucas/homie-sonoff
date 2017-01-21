#define MQTT_MAX_PACKET_SIZE 400
#include <Homie.h>
#include <ArduinoOTA.h>

// comment in/out if you want to use a MH-Z14 CO2 Sensor
#define CO2_ENABLED

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

HomieNode sonoffNode("sc", "sc");

void setupHandler() {
  sonoffNode.setProperty("t-unit").send("c");
  sonoffNode.setProperty("h-unit").send("%");
  sonoffNode.setProperty("l-unit").send("lux");
  sonoffNode.setProperty("n-unit").send("db");
#ifdef CO2_ENABLED
  sonoffNode.setProperty("c-unit").send("ppm");
#endif
}

String temp;
String temperature;
String humidity;
String light;
String noise;
String dusty;
#ifdef CO2_ENABLED
String co2;
#endif

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
 
  temp = Serial.readString();

  temperature = getValue(temp, ',', 0);
  humidity = getValue(temp, ',', 1);
  light = getValue(temp, ',', 2);
  noise = getValue(temp, ',', 3);
  dusty = getValue(temp, ',', 4);
#ifdef CO2_ENABLED
  co2 = getValue(temp, ',', 5);
#endif
    
  if (millis() - lastSent >= INTERVAL || lastSent == 0) {        
    
    if(temperature.length() > 0) {
      sonoffNode.setProperty("temperature").send(temperature);
    }

    if(humidity.length() > 0) {
      sonoffNode.setProperty("humidity").send(humidity);
    }

    if(light.length() > 0) {
      sonoffNode.setProperty("light").send(light);
    }

    if(noise.length() > 0) {
      sonoffNode.setProperty("noise").send(noise);
    }

    if(dusty.length() > 0) {
      sonoffNode.setProperty("dusty").send(dusty);
    }

#ifdef CO2_ENABLED
     if(co2.length() > 0 && co2 != "0" && co2 != "5000") {
      // discard values where the sensor is not initialized yet
      sonoffNode.setProperty("co2").send(co2);
    }
#endif
    
    lastSent = millis();
  }

  ArduinoOTA.handle();
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
  ArduinoOTA.setHostname(Homie.getConfiguration().deviceId);
  ArduinoOTA.begin();
}

void loop() {
  Homie.loop();
}
