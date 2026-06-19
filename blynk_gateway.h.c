#ifndef BLYNK_GATEWAY_H
#define BLYNK_GATEWAY_H

#include "system_structures.h"

void sync_to_blynk_cloud(const struct TelemetryDataset* data, 
                         const struct ActuatorState* pump, const struct ActuatorState* valve, 
                         const struct ActuatorState* horn);

#endif