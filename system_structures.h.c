#ifndef SYSTEM_STRUCTURES_H
#define SYSTEM_STRUCTURES_H

struct ActuatorState {
    int pin;
    int logicalState; 
};

struct TelemetryDataset {
    float ambientTempC;
    float ambientHumidity;
    int soilMoistureRaw;
    int reservoirLevelRaw;
    int controlPotentiometerRaw;
};

#endif