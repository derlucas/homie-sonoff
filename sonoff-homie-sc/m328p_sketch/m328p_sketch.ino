#include "global.h"
#include "sample_data.h"

static bool get_temp_humi_flag = false;
static bool get_ad_value_flag = false;
static bool update_value_flag = false;

long dht11Millis = 0;
long adcMillis = 0;
long updateMillis = 0;

void interCallback() {
  get_ad_value_flag = true;
}

void debugData(void) {
  Serial.print(sensor_dev[3].temp_humi_average[0]);
  Serial.print(",");
  Serial.print(sensor_dev[3].temp_humi_average[1]);
  Serial.print(",");
  Serial.print(sensor_dev[1].average_value);
  Serial.print(",");
  Serial.print(sensor_dev[2].average_value);
  Serial.print(",");
  Serial.print(sensor_dev[0].average_value);
  Serial.print(",");
#ifdef CO2_ENABLED
  Serial.print(sensor_dev[4].average_value);
#endif
  Serial.println(",");
}

void setup() {
  Serial.begin(19200);
  initDevice();
  attachInterrupt(0, interCallback, RISING);
}

void loop() {
  long currentMillis = millis();
  
  if(currentMillis - dht11Millis > 1000) {
    dht11Millis = currentMillis;
    getTempHumi();
#ifdef CO2_ENABLED
    getCO2();
#endif
  }
  
  // timing when we sample the ADC seems critical, so this is based on ISR
  if(get_ad_value_flag) {
    get_ad_value_flag = false;
    adcMillis = currentMillis;
    getAdcSensorValue(); 
  }
  
  if(currentMillis - updateMillis > 1000) {
      debugData();
      updateMillis = currentMillis;
  }
}
