#ifndef   _SAMPLE_DATA_H_
#define   _SAMPLE_DATA_H_

void initDevice(void);
void getTempHumi(void);
void getAdcSensorValue(void);

#ifdef CO2_ENABLED
void getCO2(void);
#endif

#endif
