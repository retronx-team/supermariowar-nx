#if !defined(PLATFORM_SWITCH_H)
#define PLATFORM_SWITCH_H

#ifndef __SWITCH__
#error "Switch specific header included"
#endif

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

void platformSwitchShowControllerOptions(void);
bool platformSwitchOSKInput(char* header, char* initialValue, char* out, size_t maxlen);

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_SWITCH_H
