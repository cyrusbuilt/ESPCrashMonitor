/**
 * ESPCrashMonitor.h
 * Version 1.0.1
 * Author
 *      Cyrus Brunner <cyrusbuilt@gmail.com>
 * 
 * This library provides a means of monitoring for and capturing debug data
 * related to firmware crashes and hardware resets.
 */

#ifndef ESPCrashMonitor_h
#define ESPCrashMonitor_h

#ifndef ESP8266
    #error "This library is only compatible with the Arduino/ESP8266-based controllers."
#endif

#include <Arduino.h>
#include "Esp.h"
extern "C" {
    #include "user_interface.h"
}

struct bootflags_t
{
    unsigned char rawRstCause : 4;
    unsigned char rawBootDevice : 4;
    unsigned char rawBootMode : 4;
    unsigned char rstNormalBoot : 1;
    unsigned char rstResetPin : 1;
    unsigned char rstWatchdog : 1;
    unsigned char bootDeviceRam : 1;
    unsigned char bootDeviceFlash : 1;
};

class ESPCrashMonitorClass
{
    public:
        /**
        * Possible timeout values.
        */
        enum ETimeout
        {
        Timeout_15ms = WDTO_15MS,
        Timeout_30ms = WDTO_30MS,
        Timeout_60ms = WDTO_60MS,
        Timeout_120ms = WDTO_120MS,
        Timeout_250ms = WDTO_250MS,
        Timeout_500ms = WDTO_500MS,
        Timeout_1s = WDTO_1S,
        Timeout_2s = WDTO_2S,
        #ifdef WDTO_4S
            Timeout_4s = WDTO_4S,
        #endif
        #ifdef WDTO_8S
            Timeout_8s = WDTO_8S
        #endif
        };

        /**
         * Default ctor.
         */
        ESPCrashMonitorClass();

        /**
         * Detects the boot mode and gathers boot-specific information.
         * @returns bootflags_t The boot information struct.
         */
        struct bootflags_t detectBootMode();

        /**
         * Dumps the debug information about the reset cause and boot.
         */
        void dump(Print &destination);

        /**
         * Enables the watchdog which will determine if the sketch is unresponsive
         * by waiting for the 'alive' signal within the given timeout period.
         * @param timeout The timeout period to wait for the alive signal. NOTE:
         * At this time, the Arduino framework for ESP does not actually support
         * changing the timeout value. You can set it, but the parameter is
         * effectively ignored and the default value (which seems to be somewhere
         * between 1.5s and 3.2s, depending on the hardware and who you ask. I
         * recommend timing it yourself to know for sure). We keep this parameter
         * for now as future support is intended and will be automatically implemented
         * once support is added in the Esp class.
         */
        void enableWatchdog(ETimeout timeout);

        /**
         * Disables the watchdog.
         */
        void disableWatchdog();

        /**
         * Defers the watchdog until the caller returns. This has the same
         * effect as calling yield() or delay(0). It just temporarily
         * suspends the watchdog to give the caller some extra processing
         * time without tripping the watchdog.
         */
        void defer();

        /**
         * Notifies the watchdog that the program is still alive and not hung.
         * This should be called from your sketch's loop() method.
         */
        void iAmAlive();
};

extern ESPCrashMonitorClass ESPCrashMonitor;
#endif