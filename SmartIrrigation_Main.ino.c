#include "config.h"
#include "system_structures.h"
#include "hardware.h"
#include "blynk_gateway.h"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

const char blynk_auth_token[] = "Your_Blynk_Auth_Token_Here"; 
const char wifi_ssid[]        = "Your_WiFi_SSID";             
const char wifi_pass[]        = "Your_WiFi_Password";         

struct ActuatorState pumpMotor;
struct ActuatorState waterValve;
struct ActuatorState warningRelay;
struct ActuatorState alarmBuzzer;
unsigned long lastCloudSyncTimestamp = 0;

void setup() 
{
    Serial.begin(9600);
    while(!Serial);
    Serial.println("\n=== SYSTEM RESET: LOADING MODULAR BLYNK IOT FIRMWARE ===");

    initialize_hardware_channel(&pumpMotor, PERISTALTICPUMP_PIN_COIL1);
    initialize_hardware_channel(&waterValve, SOLENOIDVALVE_PIN_COIL1);
    initialize_hardware_channel(&warningRelay, RELAYMODULE_PIN_SIGNAL);
    initialize_hardware_channel(&alarmBuzzer, BUZZER_PIN_SIG);

    pinMode(DHT_PIN_DATA, INPUT_PULLUP);

    Serial.printf("Connecting to network SSID: %s...\n", wifi_ssid);
    Blynk.begin(blynk_auth_token, wifi_ssid, wifi_pass);
    Serial.println("System initialized successfully. Loop sequence active.");
}

void loop() 
{
    Blynk.run();

    struct TelemetryDataset activeMetrics;

    scan_hardware_telemetry(&activeMetrics);

    apply_automation_rules(&activeMetrics, &pumpMotor, &waterValve, &warningRelay, &alarmBuzzer);

    if (millis() - lastCloudSyncTimestamp >= CLOUD_SYNC_INTERVAL) 
    {
        sync_to_blynk_cloud(&activeMetrics, &pumpMotor, &waterValve, &alarmBuzzer);
        lastCloudSyncTimestamp = millis();
    }

    delay(50);
}