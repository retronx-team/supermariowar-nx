#ifdef __SWITCH__

#include <switch.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL_events.h>

#include "switch.h"

static FILE* g_logfile = NULL;

void userAppInit() {
    Result rc = socketInitializeDefault();
    int stdioRes = -1;

    if(R_SUCCEEDED(rc)) {
        stdioRes = nxlinkStdio();
    }

    if(stdioRes == -1) {
        g_logfile = fopen("smw.log", "w");
        if(g_logfile != NULL) {
            fflush(stdout);
            dup2(fileno(g_logfile), STDOUT_FILENO);
            fflush(stderr);
            dup2(fileno(g_logfile), STDERR_FILENO);
        }
        printf("nxlink failed with res: %d\n", stdioRes);
    }

    if(R_FAILED(rc)) {
        printf("socketInitialize failed with rc: %x\n", rc);
    }

    rc = appletInitializeGamePlayRecording();
    if(R_FAILED(rc)) {
        printf("appletInitializeGamePlayRecording failed with rc: %x\n", rc);
    }
    appletSetGamePlayRecordingState(1);
    hidSetNpadJoyHoldType(HidNpadJoyHoldType_Horizontal);
}

void userAppExit() {
    socketExit();
    if(g_logfile != NULL) {
        fclose(g_logfile);
    }
}

void platformSwitchShowControllerOptions(void) {
    HidLaControllerSupportArg arg;
    HidLaControllerSupportResultInfo resultInfo;
    hidLaCreateControllerSupportArg(&arg);
    arg.enable_explain_text = 1;
    arg.hdr.enable_identification_color = 1;

    hidLaSetExplainText(&arg, "P1 (Red)", HidNpadIdType_No1);
    arg.identification_color[HidNpadIdType_No1] = (HidLaControllerSupportArgColor) {.r = 222, .g = 79, .b = 87, .a = 0xFF};

    hidLaSetExplainText(&arg, "P2 (Green)", HidNpadIdType_No2);
    arg.identification_color[HidNpadIdType_No2] = (HidLaControllerSupportArgColor) {.r = 123, .g = 236, .b = 94, .a = 0xFF};

    hidLaSetExplainText(&arg, "P3 (Yellow)", HidNpadIdType_No3);
    arg.identification_color[HidNpadIdType_No3] = (HidLaControllerSupportArgColor) {.r = 224, .g = 190, .b = 87, .a = 0xFF};

    hidLaSetExplainText(&arg, "P4 (Blue)", HidNpadIdType_No4);
    arg.identification_color[HidNpadIdType_No4] = (HidLaControllerSupportArgColor) {.r = 73, .g = 112, .b = 230, .a = 0xFF};

    Result rc = hidLaShowControllerSupportForSystem(&resultInfo, &arg, false);
    printf("%s: hidLaShowControllerSupportForSystem exited with rc %x\n", __func__, rc);
}

bool platformSwitchOSKInput(char* header, char* initialValue, char* out, size_t maxlen) {
    printf("%s: header = \"%s\", initialValue = \"%s\", maxlen = %u);\n", __func__, header, initialValue ? initialValue : "NULL", maxlen);
    SwkbdConfig kbd;
    Result rc = swkbdCreate(&kbd, 0);
    printf("swkbdCreate(): 0x%x\n", rc);

    if (R_SUCCEEDED(rc)) {
        swkbdConfigMakePresetDefault(&kbd);
        swkbdConfigSetHeaderText(&kbd, header);
        kbd.arg.arg.arg.stringLenMax = maxlen-1;
        if(initialValue) {
            swkbdConfigSetInitialText(&kbd, initialValue);
        }
        rc = swkbdShow(&kbd, out, maxlen);
        printf("swkbdShow(): 0x%x\n", rc);
        if (R_SUCCEEDED(rc)) {
            printf("swkbd out str: %s\n", out);
        }
        swkbdClose(&kbd);
    }

    return R_SUCCEEDED(rc);
}

bool platformSwitchSDLEventTransform(SDL_Event* event) {

    switch(event->type) {
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            if(event->jbutton.button >= SWITCH_SDL_JOY_BTN_DPAD_LEFT && event->jbutton.button <= SWITCH_SDL_JOY_BTN_DPAD_DOWN) {
                event->jbutton.button += (SWITCH_SDL_JOY_BTN_LSTICK_LEFT - SWITCH_SDL_JOY_BTN_DPAD_LEFT);
            }
            return true;
        case SDL_JOYHATMOTION:
        case SDL_JOYAXISMOTION:
            return false;
    }
    return true;
}

#endif
