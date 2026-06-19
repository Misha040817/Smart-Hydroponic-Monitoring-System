#ifndef CONFIG_H
#define CONFIG_H

#define BLYNK_TEMPLATE_ID   "TMPL000000000" 
#define BLYNK_TEMPLATE_NAME "Smart Irrigation"

#define PERISTALTICPUMP_PIN_COIL1    1   
#define BUZZER_PIN_SIG               0   
#define SOLENOIDVALVE_PIN_COIL1      4   
#define RELAYMODULE_PIN_SIGNAL       5   
#define LEDSTRIPS_PIN_SIG            3   
#define POTENTIOMETER_3V3_PIN_SIG   12   
#define DHT_PIN_DATA                 2   
#define SOILMOISTURE_3V3_PIN_SIG    13   
#define WATERLEVELSENSOR_3V3_PIN_SIG 14  

extern const char blynk_auth_token[];
extern const char wifi_ssid[];
extern const char wifi_pass[];

#define DRY_SOIL_THRESHOLD    1500   
#define CRITICAL_WATER_LEVEL   500   
#define CLOUD_SYNC_INTERVAL   3000   

#define VPIN_TEMPERATURE     V1  
#define VPIN_HUMIDITY        V2  
#define VPIN_SOIL_MOISTURE   V3  
#define VPIN_WATER_LEVEL     V4  
#define VPIN_POT_THRESHOLD   V5  
#define VPIN_PUMP_STATUS     V6  
#define VPIN_VALVE_STATUS    V7  
#define VPIN_ALARM_STATUS    V8  

#endif