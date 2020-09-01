#ifdef __SWITCH__

#include <switch.h>
#include <stdio.h>
#include <unistd.h>

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
    hidSetNpadJoyHoldType(HidJoyHoldType_Horizontal);
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

    hidLaSetExplainText(&arg, "P1 (Red)", CONTROLLER_PLAYER_1);
    arg.identification_color[CONTROLLER_PLAYER_1] = (HidLaControllerSupportArgColor) {.r = 222, .g = 79, .b = 87, .a = 0xFF};

    hidLaSetExplainText(&arg, "P2 (Green)", CONTROLLER_PLAYER_2);
    arg.identification_color[CONTROLLER_PLAYER_2] = (HidLaControllerSupportArgColor) {.r = 123, .g = 236, .b = 94, .a = 0xFF};

    hidLaSetExplainText(&arg, "P3 (Yellow)", CONTROLLER_PLAYER_3);
    arg.identification_color[CONTROLLER_PLAYER_3] = (HidLaControllerSupportArgColor) {.r = 224, .g = 190, .b = 87, .a = 0xFF};

    hidLaSetExplainText(&arg, "P4 (Blue)", CONTROLLER_PLAYER_4);
    arg.identification_color[CONTROLLER_PLAYER_4] = (HidLaControllerSupportArgColor) {.r = 73, .g = 112, .b = 230, .a = 0xFF};

    Result rc = hidLaShowControllerSupportForSystem(&resultInfo, &arg, false);
    printf("%s: hidLaShowControllerSupportForSystem exited with rc %x\n", __func__, rc);
}

#endif
