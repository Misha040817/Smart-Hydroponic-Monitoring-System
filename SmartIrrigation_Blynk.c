/**
 * Project: ESP32 Automated Smart Irrigation System with Blynk IoT Integration
 * Language: Pure C-Style Procedural Firmware
 * Description: Connects to a localized Wi-Fi access point, interfaces with the 
 * Blynk Cloud Platform via Virtual Pins, reads environment telemetry, 
 * executes automation, and mirrors device state variables to the cloud dashboard.
 */

#define BLYNK_TEMPLATE_ID   "TMPL000000000" // Replace with your unique Blynk Template ID
#define BLYNK_TEMPLATE_NAME "Smart Irrigation"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// ==========================================
// 1. HARDWARE PIN DEFINITIONS (ESP32 DevKitC)
// ==========================================
#define PERISTALTICPUMP_PIN_COIL1    1   // Digital Out: Water pump switch line
#define BUZZER_PIN_SIG               0   // Digital Out: Piezo alarm horn
#define SOLENOIDVALVE_PIN_COIL1      4   // Digital Out: Water valve switch line
#define RELAYMODULE_PIN_SIGNAL       5   // Digital Out: Auxiliary warning relay
#define LEDSTRIPS_PIN_SIG            3   // PWM Out: Illumination indicator strip
#define POTENTIOMETER_3V3_PIN_SIG   12   // Analog In (ADC): Threshold configuration knob
#define DHT_PIN_DATA                 2   // Digital I/O: Ambient temperature/humidity line
#define SOILMOISTURE_3V3_PIN_SIG    13   // Analog In (ADC): Ground hydration probe
#define WATERLEVELSENSOR_3V3_PIN_SIG 14  // Analog In (ADC): Liquid level monitor

// ==========================================
// 2. NETWORK & CLOUD NETWORK PARAMETERS
// ==========================================
const char blynk_auth_token[] = "Your_Blynk_Auth_Token_Here"; // Blynk Device Authentication Token
const char wifi_ssid[]        = "Your_WiFi_SSID";             // Wireless Network Name
const char wifi_pass[]        = "Your_WiFi_Password";         // Wireless Network WPA2 Passphrase

#define DRY_SOIL_THRESHOLD    1500   // ADC value indicating parched dirt matrix
#define CRITICAL_WATER_LEVEL   500   // ADC value indicating empty water tank
#define CLOUD_SYNC_INTERVAL   3000   // Telemetry synchronization rate (3000ms / 3s)

// ==========================================
// 3. BLYNK VIRTUAL PIN ASSIGNMENTS
// ==========================================
#define VPIN_TEMPERATURE     V1  // Gauge Widget: Floating point Celcius telemetry
#define VPIN_HUMIDITY        V2  // Gauge Widget: Floating point Relative Humidity pct
#define VPIN_SOIL_MOISTURE   V3  // Value Display Widget: Raw ground moisture integer
#define VPIN_WATER_LEVEL     V4  // Level Widget: Liquid containment capacity remaining
#define VPIN_POT_THRESHOLD   V5  // Value Display: Manual threshold benchmark value
#define VPIN_PUMP_STATUS     V6  // LED Widget: Pump operational tracking flag
#define VPIN_VALVE_STATUS    V7  // LED Widget: Inline solenoid status flag
#define VPIN_ALARM_STATUS    V8  // LED / Terminal Widget: Critical hazard marker flag

// ==========================================
// 4. PROCEDURAL C STRUCT DEFINITIONS
// ==========================================
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

// ==========================================
// 5. C FUNCTION PROTOTYPES
// ==========================================
void initialize_hardware_channel(struct ActuatorState* actuator, int gpio_pin);
void modify_hardware_output(struct ActuatorState* actuator, int execution_level);
void scan_hardware_telemetry(struct TelemetryDataset* data);
void apply_automation_rules(const struct TelemetryDataset* data, 
                            struct ActuatorState* pump, struct ActuatorState* valve, 
                            struct ActuatorState* relay, struct ActuatorState* horn);
void sync_to_blynk_cloud(const struct TelemetryDataset* data, 
                         const struct ActuatorState* pump, const struct ActuatorState* valve, 
                         const struct ActuatorState* horn);

// Global state tracking objects allocated inline within memory
struct ActuatorState pumpMotor;
struct ActuatorState waterValve;
struct ActuatorState warningRelay;
struct ActuatorState alarmBuzzer;
unsigned long lastCloudSyncTimestamp = 0;

/**
 * Function: setup
 * Purpose: Embedded application bootstrap configuration
 */
void setup() 
{
    Serial.begin(9600);
    while(!Serial);
    Serial.println("\n--- BOOTING SYSTEM WITH BLYNK IOT FIRMWARE (C STYLE) ---");

    // Initialize individual digital hardware actuator structures
    initialize_hardware_channel(&pumpMotor, PERISTALTICPUMP_PIN_COIL1);
    initialize_hardware_channel(&waterValve, SOLENOIDVALVE_PIN_COIL1);
    initialize_hardware_channel(&warningRelay, RELAYMODULE_PIN_SIGNAL);
    initialize_hardware_channel(&alarmBuzzer, BUZZER_PIN_SIG);

    // Assert digital input pullup state configurations
    pinMode(DHT_PIN_DATA, INPUT_PULLUP);

    // Initialize network interface loops and register secure cloud handle channels
    Serial.print("Connecting network to SSID: "); Serial.println(wifi_ssid);
    Blynk.begin(blynk_auth_token, wifi_ssid, wifi_pass);
    Serial.println("Network socket connection stable. Sync engine ready.");
}

/**
 * Function: loop
 * Purpose: Cyclic operating sequence managing automation logic loops
 */
void loop() 
{
    // Execute underlying cloud connection housekeeping updates
    Blynk.run();

    // Instantiate temporary structured data workspace
    struct TelemetryDataset activeMetrics;

    // 1. Gather all system sensor measurements
    scan_hardware_telemetry(&activeMetrics);

    // 2. Assess safety boundaries and run edge decision paths
    apply_automation_rules(&activeMetrics, &pumpMotor, &waterValve, &warningRelay, &alarmBuzzer);

    // 3. Handle cloud logging without using blocking code delays
    if (millis() - lastCloudSyncTimestamp >= CLOUD_SYNC_INTERVAL) 
    {
        sync_to_blynk_cloud(&activeMetrics, &pumpMotor, &waterValve, &alarmBuzzer);
        lastCloudSyncTimestamp = millis();
    }

    // Small processing delay loop stabilization window
    delay(50);
}

// ==========================================
// 6. DETAILED IMPLEMENTATION DECLARATIONS
// ==========================================

void initialize_hardware_channel(struct ActuatorState* actuator, int gpio_pin) 
{
    actuator->pin = gpio_pin;
    actuator->logicalState = 0;
    pinMode(actuator->pin, OUTPUT);
    digitalWrite(actuator->pin, LOW); // Force safe closed state on startup
}

void modify_hardware_output(struct ActuatorState* actuator, int execution_level) 
{
    actuator->logicalState = execution_level;
    digitalWrite(actuator->pin, execution_level);
}

void scan_hardware_telemetry(struct TelemetryDataset* data) 
{
    // Simulated fixed telemetry for digital 1-Wire DHT elements (mocked C-equivalent implementation)
    data->ambientTempC = 27.8;
    data->ambientHumidity = 62.4;

    // Query ADC lines directly using native registers
    data->soilMoistureRaw = analogRead(SOILMOISTURE_3V3_PIN_SIG);
    data->reservoirLevelRaw = analogRead(WATERLEVELSENSOR_3V3_PIN_SIG);
    data->controlPotentiometerRaw = analogRead(POTENTIOMETER_3V3_PIN_SIG);
}

void apply_automation_rules(const struct TelemetryDataset* data, 
                            struct ActuatorState* pump, struct ActuatorState* valve, 
                            struct ActuatorState* relay, struct ActuatorState* horn) 
{
    // Verification block 1: Soil hydration loop
    if (data->soilMoistureRaw > DRY_SOIL_THRESHOLD) 
    {
        modify_hardware_output(pump, 1);  // Turn pump on
        modify_hardware_output(valve, 1); // Open mechanical valve
    } 
    else 
    {
        modify_hardware_output(pump, 0);  // Turn pump off
        modify_hardware_output(valve, 0); // Close mechanical valve
    }

    // Verification block 2: Tank reserve monitoring
    if (data->reservoirLevelRaw < CRITICAL_WATER_LEVEL) 
    {
        modify_hardware_output(horn, 1);  // Fire audible alarm
        modify_hardware_output(relay, 1); // Engage alert line switch
    } 
    else 
    {
        modify_hardware_output(horn, 0);  // Clear acoustic alert
        modify_hardware_output(relay, 0); // Open safety switch line
    }

    // PWM Adjustment: Ambient light output management based on thermal feedback
    if (data->ambientTempC > 30.0) 
    {
        analogWrite(LEDSTRIPS_PIN_SIG, 50);   // Restrict output loop when environment overheats
    } 
    else 
    {
        analogWrite(LEDSTRIPS_PIN_SIG, 255);  // Maximum bright state configuration
    }
}

void sync_to_blynk_cloud(const struct TelemetryDataset* data, 
                         const struct ActuatorState* pump, const struct ActuatorState* valve, 
                         const struct ActuatorState* horn) 
{
    // Stream updated values into targeted Blynk cloud dashboard structures
    Blynk.virtualWrite(VPIN_TEMPERATURE, data->ambientTempC);
    Blynk.virtualWrite(VPIN_HUMIDITY, data->ambientHumidity);
    Blynk.virtualWrite(VPIN_SOIL_MOISTURE, data->soilMoistureRaw);
    Blynk.virtualWrite(VPIN_WATER_LEVEL, data->reservoirLevelRaw);
    Blynk.virtualWrite(VPIN_POT_THRESHOLD, data->controlPotentiometerRaw);

    // Map hardware statuses into matching virtual interface switches or indicators
    Blynk.virtualWrite(VPIN_PUMP_STATUS, pump->logicalState ? 255 : 0);
    Blynk.virtualWrite(VPIN_VALVE_STATUS, valve->logicalState ? 255 : 0);
    Blynk.virtualWrite(VPIN_ALARM_STATUS, horn->logicalState ? 255 : 0);

    // Also mirror status events directly down the debug serial interface terminal
    Serial.printf("[BLYNK IOT SYNC] Transmitted fields successfully. Temp: %.2fC | Soil: %d | Pump: %d\n", 
                  data->ambientTempC, data->soilMoistureRaw, pump->logicalState);
}