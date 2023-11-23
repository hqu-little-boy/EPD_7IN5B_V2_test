#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include "src/EPD_Test.h"   //Examples
#include "lib/Config/DEV_Config.h"

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit\r\n");
    DEV_Module_Exit();

    exit(0);
}

int main(void)
{
    // Exception handling:ctrl + c
    signal(SIGINT, Handler);

    EPD_7in5b_V2_test();

    
    return 0;
}

