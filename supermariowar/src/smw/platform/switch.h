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
bool platformSwitchSDLEventTransform(SDL_Event* event);

#define SWITCH_SDL_JOY_BTN_A 0
#define SWITCH_SDL_JOY_BTN_B 1
#define SWITCH_SDL_JOY_BTN_X 2
#define SWITCH_SDL_JOY_BTN_Y 3
#define SWITCH_SDL_JOY_BTN_LSTICK 4
#define SWITCH_SDL_JOY_BTN_RSTICK 5
#define SWITCH_SDL_JOY_BTN_L 6
#define SWITCH_SDL_JOY_BTN_R 7
#define SWITCH_SDL_JOY_BTN_ZL 8
#define SWITCH_SDL_JOY_BTN_ZR 9
#define SWITCH_SDL_JOY_BTN_PLUS 10
#define SWITCH_SDL_JOY_BTN_MINUS 11
#define SWITCH_SDL_JOY_BTN_DPAD_LEFT 12
#define SWITCH_SDL_JOY_BTN_DPAD_UP 13
#define SWITCH_SDL_JOY_BTN_DPAD_RIGHT 14
#define SWITCH_SDL_JOY_BTN_DPAD_DOWN 15
#define SWITCH_SDL_JOY_BTN_LSTICK_LEFT 16
#define SWITCH_SDL_JOY_BTN_LSTICK_UP 17
#define SWITCH_SDL_JOY_BTN_LSTICK_RIGHT 18
#define SWITCH_SDL_JOY_BTN_LSTICK_DOWN 19
#define SWITCH_SDL_JOY_BTN_RSTICK_LEFT 20
#define SWITCH_SDL_JOY_BTN_RSTICK_UP 21
#define SWITCH_SDL_JOY_BTN_RSTICK_RIGHT 22
#define SWITCH_SDL_JOY_BTN_RSTICK_DOWN 23

#define SWITCH_SMW_BTN(x) (JOY_BUTTON_START + SWITCH_SDL_JOY_BTN_ ## x)

#ifdef __cplusplus
}
#endif

#endif // PLATFORM_SWITCH_H
