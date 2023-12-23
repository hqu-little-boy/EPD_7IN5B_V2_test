/*****************************************************************************
* | File      	:   EPD_5in83_V2_test.c
* | Author      :   Waveshare team
* | Function    :   5.83inch e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-11-23
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_Test.h"
#include "../lib/e-Paper/EPD_5in83_V2.h"
#include "../entity/Qweather/Weather.h"
#include "../entity/Pic/EinkPic.h"

#include "time.h"

int EPD_5in83_V2_test(void)
{
    printf("EPD_5IN83_V2_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_5in83_V2_Init();
    struct timespec start={0,0}, finish={0,0}; 
    clock_gettime(CLOCK_REALTIME,&start);
    EPD_5in83_V2_Clear();
    clock_gettime(CLOCK_REALTIME,&finish);
    printf("%ld S\r\n",finish.tv_sec-start.tv_sec);
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    Paint paint;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_5in83_V2_WIDTH % 8 == 0)? (EPD_5in83_V2_WIDTH / 8 ): (EPD_5in83_V2_WIDTH / 8 + 1)) * EPD_5in83_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("paint.Paint_NewImage\r\n");
    paint.Paint_NewImage(BlackImage, EPD_5in83_V2_WIDTH, EPD_5in83_V2_HEIGHT, 0, WHITE);

// #if 1   // show bmp
    printf("show bmp------------------------\r\n");
    paint.Paint_SelectImage(BlackImage);
    EinkPic::EinkPic einkPic{"/root/EPD_7IN5B_V2_test/config.json"};
    einkPic.InitImg();
    paint.GUI_ReadMat(einkPic.GetImg(), 0, 0);
    printf("show bmp------------------------\r\n");
    EPD_5in83_V2_Display(BlackImage);
    DEV_Delay_ms(2000);
// #endif        

// #if 1   // show image for array   
    // printf("show image for array\r\n");
    // paint.Paint_SelectImage(BlackImage);
    // paint.Paint_Clear(WHITE);
    // paint.Paint_DrawBitMap(gImage_5in83_V2);
    // EPD_5in83_V2_Display(BlackImage);
    // DEV_Delay_ms(500);
// #endif

#if 1   // Drawing on the image
    //1.Select Image
    // printf("SelectImage:BlackImage\r\n");
    // paint.Paint_SelectImage(BlackImage);
    // paint.Paint_Clear(WHITE);

    // 2.Drawing on the image
    // printf("Drawing:BlackImage\r\n");
    // paint.Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    // paint.Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    // paint.Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    // paint.Paint_DrawLine(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    // paint.Paint_DrawLine(70, 70, 20, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    // paint.Paint_DrawRectangle(20, 70, 70, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // paint.Paint_DrawRectangle(80, 70, 130, 120, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    // paint.Paint_DrawCircle(45, 95, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    // paint.Paint_DrawCircle(105, 95, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    // paint.Paint_DrawLine(85, 95, 125, 95, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    // paint.Paint_DrawLine(105, 75, 105, 115, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    // paint.Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    // paint.Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    // paint.Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    // paint.Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    // paint.Paint_DrawString_CN(130, 0, " ���abc", &Font12CN, BLACK, WHITE);
    // paint.Paint_DrawString_CN(130, 20, "΢ѩ����", &Font24CN, WHITE, BLACK);

    // printf("EPD_Display\r\n");
    // EPD_5in83_V2_Display(BlackImage);
    // DEV_Delay_ms(2000);
#endif

    // printf("Clear...\r\n");
    // EPD_5in83_V2_Clear();

    printf("Goto Sleep...\r\n");
    EPD_5in83_V2_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000);//important, at least 2s
    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

