#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/pwr_mgt/IOPM.h>
#include <IOKit/pwr_mgt/IOPMLib.h>

extern IOReturn IOPMSetSystemPowerSetting(CFStringRef key, CFTypeRef value);
extern CFDictionaryRef IOPMCopySystemPowerSettings();

bool sleep_disabled() {
    CFDictionaryRef settings = IOPMCopySystemPowerSettings();
    if (!settings) return false;

    CFBooleanRef value = (CFBooleanRef)CFDictionaryGetValue(settings, CFSTR("SleepDisabled"));
    CFRelease(settings);

    return value && CFBooleanGetValue(value);
}

bool sleep_enable(bool enable) {
    return IOPMSetSystemPowerSetting(CFSTR("SleepDisabled"), enable ? kCFBooleanFalse : kCFBooleanTrue) == kIOReturnSuccess;
}

IOReturn sleep_now() {
    io_connect_t connection = IOPMFindPowerManagement(MACH_PORT_NULL);
    if (connection == MACH_PORT_NULL) return kIOReturnError;

    IOReturn result = IOPMSleepSystem(connection);
    IOServiceClose(connection);
    return result;
}

bool lid_closed() {
    io_service_t s = IOServiceGetMatchingService(kIOMainPortDefault, IOServiceMatching("IOPMrootDomain"));
    if (s == MACH_PORT_NULL) return false;

    CFTypeRef v = IORegistryEntryCreateCFProperty(s, CFSTR(kAppleClamshellStateKey), kCFAllocatorDefault, 0);
    IOObjectRelease(s);

    bool closed = v && CFGetTypeID(v) == CFBooleanGetTypeID() && CFBooleanGetValue((CFBooleanRef)v);

    if (v) CFRelease(v);
    return closed;
}

void quit() {
    if (sleep_disabled()) {
        printf("Re-enabling sleep ...\n");
        sleep_enable(true);
    } else if (lid_closed()) {
        printf("Going to sleep ...\n");
        sleep_now();
    }
    exit(EXIT_SUCCESS);
}

int main() {
    if (getuid()) {
        printf("Please run as root\n");
        exit(EXIT_FAILURE);
    }

    // The only way to close the program is a signal
    signal(SIGINT, quit);
    signal(SIGTERM, quit);

    printf("Disabling sleep ...\n");
    sleep_enable(false);

    printf("Waiting for the lid to close ... ");
    fflush(stdout);
    while (!lid_closed()) sleep(1);
    printf("done\n");

    sleep_enable(true);  // Sleep can be re-enabled as soon as the lid is closed

    pause();
}
