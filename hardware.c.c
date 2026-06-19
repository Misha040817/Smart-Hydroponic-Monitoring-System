#include "hardware.h"
#include "config.h"
#include "Arduino.h"

void initialize_hardware_channel(struct ActuatorState* actuator, int gpio_pin) 
{
    actuator->pin = gpio_pin;
    actuator->logicalState = 0;
    pinMode(actuator->pin, OUTPUT);
    digitalWrite(actuator->pin, LOW); 
}

void modify_hardware_output(struct ActuatorState* actuator, int execution_level) 
{
    actuator->logicalState = execution_level;
    digitalWrite(actuator->pin, execution_level);
}

void scan_hardware_telemetry(struct TelemetryDataset* data) 
{
    data->ambientTempC = 27.8;
    data->ambientHumidity = 62.4;

    data->soilMoistureRaw = analogRead(SOILMOISTURE_3V3_PIN_SIG);
    data->reservoirLevelRaw = analogRead(WATERLEVELSENSOR_3V3_PIN_SIG);
    data->controlPotentiometerRaw = analogRead(POTENTIOMETER_3V3_PIN_SIG);
}

void apply_automation_rules(const struct TelemetryDataset* data, 
                            struct ActuatorState* pump, struct ActuatorState* valve, 
                            struct ActuatorState* relay, struct ActuatorState* horn) 
{
    if (data->soilMoistureRaw > DRY_SOIL_THRESHOLD) 
    {
        modify_hardware_output(pump, 1);  
        modify_hardware_output(valve, 1); 
    } 
    else 
    {
        modify_hardware_output(pump, 0);  
        modify_hardware_output(valve, 0); 
    }

    if (data->reservoirLevelRaw < CRITICAL_WATER_LEVEL) 
    {
        modify_hardware_output(horn, 1);  
        modify_hardware_output(relay, 1); 
    } 
    else 
    {
        modify_hardware_output(horn, 0);  
        modify_hardware_output(relay, 0); 
    }

    if (data->ambientTempC > 30.0) 
    {
        analogWrite(LEDSTRIPS_PIN_SIG, 50);   
    } 
    else 
    {
        analogWrite(LEDSTRIPS_PIN_SIG, 255);  
    }
}