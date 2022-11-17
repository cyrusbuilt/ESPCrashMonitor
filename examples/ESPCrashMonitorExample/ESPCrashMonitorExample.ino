#include <Arduino.h>
#include "ESPCrashMonitor.h"

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        delay(10);
    }

    ESPCrashMonitor.disableWatchdog();
    ESPCrashMonitor.dump(Serial);
    ESPCrashMonitor.enableWatchdog(ESPCrashMonitorClass::ETimeout::Timeout_2s);
}

void loop() {
    ESPCrashMonitor.iAmAlive();  // Signal that we are alive first to get the full timeout period.
    while (true) {
        ;  // Now trigger a crash.
    }
}
