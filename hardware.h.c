#ifndef HARDWARE_H
#define HARDWARE_H

#include "system_structures.h"

void initialize_hardware_channel(struct ActuatorState* actuator, int gpio_pin);
void modify_hardware_output(struct ActuatorState* actuator, int execution_level);
void scan_hardware_telemetry(struct TelemetryDataset* data);
void apply_automation_rules(const struct TelemetryDataset* data, 
                            struct ActuatorState* pump, struct ActuatorState* valve, 
                            struct ActuatorState* relay, struct ActuatorState* horn);

#endif