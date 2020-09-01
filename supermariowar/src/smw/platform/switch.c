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
}

void userAppExit() {
    socketExit();
    if(g_logfile != NULL) {
        fclose(g_logfile);
    }
}

#endif
