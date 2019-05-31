/**
 * ESPCrashMonitor.cpp
 * Version 1.0.0
 * Author
 *      Cyrus Brunner <cyrusbuilt@gmail.com>
 * 
 * This library provides a means of monitoring for and capturing debug data
 * related to firmware crashes and hardware resets.
 */


#include "ESPCrashMonitor.h"

ESPCrashMonitorClass::ESPCrashMonitorClass() {

}

struct bootflags_t ESPCrashMonitorClass::detectBootMode() {
    int resetReason;
    int bootMode;
    asm (
        "movi %0, 0x60000600\n\t"
        "movi %1, 0x60000200\n\t"
        "l32i %0, %0, 0x114\n\t"
        "l32i %1, %1, 0x118\n\t"
        : "+r" (resetReason), "+r" (bootMode) /* Outputs */
        : /* Inputs (none) */
        : "memory" /* Clobbered */
    );

    struct bootflags_t flags;
    flags.rawRstCause = (resetReason & 0xF);
    flags.rawBootDevice = ((bootMode >> 0x10) & 0x7);
    flags.rawBootMode = ((bootMode >> 0x1D) & 0x7);
    flags.rstNormalBoot = flags.rawRstCause == 0x1;
    flags.rstResetPin = flags.rawRstCause == 0x2;
    flags.rstWatchdog = flags.rawRstCause == 0x4;
    flags.bootDeviceRam = flags.rawBootDevice == 0x1;
    flags.bootDeviceFlash = flags.rawBootDevice == 0x3;
    return flags;
}

void ESPCrashMonitorClass::disableWatchdog() {
    ESP.wdtDisable();
}

void ESPCrashMonitorClass::dump(Print &destination) {
    rst_info* rinfo = ESP.getResetInfoPtr();
    destination.println(F("========================================="));
    destination.println();
    destination.printf("Reset reason: %d, %s\n", rinfo->reason, ESP.getResetReason().c_str());
    destination.printf("Exception cause: %d\n", rinfo->exccause);
    destination.printf("Exception vector address: %d\n", rinfo->excvaddr);
    destination.printf("EPC1: %d\n", rinfo->epc1);
    destination.printf("EPC2: %d\n", rinfo->epc2);
    destination.printf("EPC3: %d\n", rinfo->epc3);
    destination.printf("DEPC: %d\n", rinfo->depc);
    
    struct bootflags_t bflags = this->detectBootMode();
    destination.println();
    destination.printf("Reset cause: %d\n", bflags.rawRstCause);
    destination.printf("Boot device: %d\n", bflags.rawBootDevice);
    destination.printf("Boot mode:   %d\n", bflags.rawBootMode);
    destination.printf("Normal boot: %d\n", bflags.rstNormalBoot);
    destination.printf("Reset pin:   %d\n", bflags.rstResetPin);
    destination.printf("Watchdog:    %d\n", bflags.rstWatchdog);
    destination.printf("Boot device RAM:   %d\n", bflags.bootDeviceRam);
    destination.printf("Boot device flash: %d\n", bflags.bootDeviceFlash);
    destination.printf("Reset reason code: %d\n\n", rinfo->reason);
    destination.println(F("========================================="));
}

void ESPCrashMonitorClass::defer() {
    yield();
}

void ESPCrashMonitorClass::enableWatchdog(ETimeout timeout) {
    ESP.wdtEnable(timeout);
}

void ESPCrashMonitorClass::iAmAlive() {
    ESP.wdtFeed();
}

ESPCrashMonitorClass ESPCrashMonitor;