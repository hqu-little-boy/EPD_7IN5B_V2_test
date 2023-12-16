#include <cstdio>
#include <cstdlib>     //exit()
#include <csignal>     //signal()
#include "src/EPD_Test.h"   //Examples
#include "lib/Config/DEV_Config.h"

void  Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit\r\n");
    DEV_Module_Exit();

    exit(0);
}

int main()
{
    // // Exception handling:ctrl + c
    signal(SIGINT, Handler);

    EPD_5in83_V2_test();
    // printf("hello world\n");
    
    return 0;
}

