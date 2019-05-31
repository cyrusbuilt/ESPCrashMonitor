# ESPCrashMonitor
Arduino crash monitor library for the ESP8266.

[![Build Status](https://travis-ci.com/cyrusbuilt/ESPCrashMonitor.svg?branch=master)](https://travis-ci.com/cyrusbuilt/ESPCrashMonitor)

## Description

Similar to [ArduinoCrashMonitor](https://github.com/cyrusbuilt/ArduinoCrashMonitor),
This library takes advantage of the ESP8266's watchdog capability to get the
cause of a reset and boot information in the event that the Arduino sketch
(firmware) becomes unresponsive due to a crash (divide by zero, WiFi fails to
initialize, etc). The general idea behind this is: during each iteration of
loop(), CrashMonitor's iAmAlive() method should be called to signal to
CrashMonitor that the sketch is still running. If CrashMonitor does not
receive the iAmAlive() signal before the end of the timeout, then the built-in
watchdog will reset store information about the crash and reset the device.
You can then use the dump() method to load the crash reports and dump them to
a Serial port.

## How to install

For PlatformIO:
```bash
$ pio lib install ESPCrashMonitor
```

For Arduino IDE:

See <https://www.arduino.cc/en/Guide/Libraries>

## How to use

After installing this library, include ESPCrashMonitor.h in your sketch and then
see the example below:

```cpp
#include <Arduino.h>
#include "ESPCrashMonitor.h"

bool firstLoopDone = false;

void setup() {
    Serial.begin(9600);
    ESPCrashMonitor.disableWatchdog(); // Make sure it is turned off during init.
    ESPCrashMonitor.dump(Serial);      // Dump any crash data to the console.

    // Turn the watchdog on. NOTE: For now the time parameter is isn't relevant.
    ESPCrashMonitor.enableWatchdog(ESPCrashMonitorClass::ETimeout::Timeout_2s);
}

void loop() {
    if (!firstLoopDone) {
        // On the first loop, tell the crash monitor we are alive so we get the full timeout.
        ESPCrashMonitor.iAmAlive();
        firstLoopDone = true;
    }
    else {
        // We completed the first loop. Now trigger a crash.
        // Following the crash, the device will reset and dump the crash data to console.
        while(true) {
            ;
        }
    }
}
```