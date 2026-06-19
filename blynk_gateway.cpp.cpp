#include "blynk_gateway.h"
#include "config.h"
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>

void sync_to_blynk_cloud(const struct TelemetryDataset* data, 
                         const struct ActuatorState* pump, const struct ActuatorState* valve, 
                         const struct ActuatorState* horn) 
{
    Blynk.virtualWrite(VPIN_TEMPERATURE, data->ambientTempC);
    Blynk.virtualWrite(VPIN_HUMIDITY, data->ambientHumidity);
    Blynk.virtualWrite(VPIN_SOIL_MOISTURE, data->soilMoistureRaw);
    Blynk.virtualWrite(VPIN_WATER_LEVEL, data->reservoirLevelRaw);
    Blynk.virtualWrite(VPIN_POT_THRESHOLD, data->controlPotentiometerRaw);

    Blynk.virtualWrite(VPIN_PUMP_STATUS, pump->logicalState ? 255 : 0);
    Blynk.virtualWrite(VPIN_VALVE_STATUS, valve->logicalState ? 255 : 0);
    Blynk.virtualWrite(VPIN_ALARM_STATUS, horn->logicalState ? 255 : 0);

    Serial.printf("[CLOUD SYNC OK] Data synchronized. Temperature: %.1fC | Soil Moisture: %d\n", 
                  data->ambientTempC, data->soilMoistureRaw);
}