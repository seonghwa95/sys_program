#include <stdio.h>
#include <signal.h>
#include "signal_handler.h"

// SIGUSR1 시그널을 처리하는 함수
void handle_signal(int sig) {
    if (sig == SIGUSR1) {
        printf("공격이 성공했습니다!\n");
    }
}
