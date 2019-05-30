# ESPCrashMonitor
Arduino crash monitor library for the ESP8266.

## Description

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