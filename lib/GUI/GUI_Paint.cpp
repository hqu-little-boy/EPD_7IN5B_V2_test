/******************************************************************************
* | File      	:   GUI_this->c
* | Author      :   Waveshare electronics
* | Function    :	Achieve drawing: draw points, lines, boxes, circles and
*                   their size, solid dotted line, solid rectangle hollow
*                   rectangle, solid circle hollow circle.
* | Info        :
*   Achieve display characters: Display a single character, string, number
*   Achieve time display: adaptive size display time minutes and seconds
*----------------
* |	This version:   V3.2
* | Date        :   2020-07-10
* | Info        :
* -----------------------------------------------------------------------------
* V3.2(2020-07-10):
* 1.Change: this->Paint_SetScale(UBYTE scale)
*		 Add scale 7 for 5.65f e-Parper
* 2.Change: this->Paint_SetPixel(UWORD Xpoint, UWORD Ypoint, UWORD Color)
*		 Add the branch for scale 7
* 3.Change: this->Paint_Clear(UWORD Color)
*		 Add the branch for scale 7
* -----------------------------------------------------------------------------
* V3.1(2019-10-10):
* 1. Add gray level
*   PAINT Add Scale
* 2. Add void this->Paint_SetScale(UBYTE scale);
* -----------------------------------------------------------------------------
* V3.0(2019-04-18):
* 1.Change: 
*    this->Paint_DrawPoint(..., DOT_STYLE DOT_STYLE)
* => this->Paint_DrawPoint(..., DOT_STYLE Dot_Style)
*    this->Paint_DrawLine(..., LINE_STYLE Line_Style, DOT_PIXEL Dot_Pixel)
* => this->Paint_DrawLine(..., DOT_PIXEL Line_width, LINE_STYLE Line_Style)
*    this->Paint_DrawRectangle(..., DRAW_FILL Filled, DOT_PIXEL Dot_Pixel)
* => this->Paint_DrawRectangle(..., DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)
*    this->Paint_DrawCircle(..., DRAW_FILL Draw_Fill, DOT_PIXEL Dot_Pixel)
* => this->Paint_DrawCircle(..., DOT_PIXEL Line_width, DRAW_FILL Draw_Filll)
*
* -----------------------------------------------------------------------------
* V2.0(2018-11-15):
* 1.add: this->Paint_NewImage()
*    Create an image's properties
* 2.add: this->Paint_SelectImage()
*    Select the picture to be drawn
* 3.add: this->Paint_SetRotate()
*    Set the direction of the cache    
* 4.add: this->Paint_RotateImage() 
*    Can flip the picture, Support 0-360 degrees, 
*    but only 90.180.270 rotation is better
* 4.add: this->Paint_SetMirroring() 
*    Can Mirroring the picture, horizontal, vertical, origin
* 5.add: this->Paint_DrawString_CN() 
*    Can display Chinese(GB1312)   
*
* ----------------------------------------------------------------------------- 
* V1.0(2018-07-17):
*   Create library
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documnetation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to  whom the Software is
* furished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
******************************************************************************/
#include "../GUI/GUI_Paint.h"
#include "../Config/DEV_Config.h"
#include "../Config/Debug.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h> //memset()
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
/******************************************************************************
function: Create Image
parameter:
    image   :   Pointer to the image cache
    width   :   The width of the picture
    Height  :   The height of the picture
    Color   :   Whether the picture is inverted
******************************************************************************/
void Paint::Paint_NewImage(UBYTE *image, UWORD Width, UWORD Height, UWORD Rotate, UWORD Color)
{
    this->Image = NULL;
    this->Image = image;

    this->WidthMemory = Width;
    this->HeightMemory = Height;
    this->Color = Color;    
    this->Scale = 2;
    this->WidthByte = (Width % 8 == 0)? (Width / 8 ): (Width / 8 + 1);
    this->HeightByte = Height;    
//    printf("WidthByte = %d, HeightByte = %d\r\n", this->WidthByte, this->HeightByte);
//    printf(" EPD_WIDTH / 8 = %d\r\n",  122 / 8);
   
    this->Rotate = Rotate;
    this->Mirror = MIRROR_NONE;
    
    if(Rotate == ROTATE_0 || Rotate == ROTATE_180) {
        this->Width = Width;
        this->Height = Height;
    } else {
        this->Width = Height;
        this->Height = Width;
    }
}

/******************************************************************************
function: Select Image
parameter:
    image : Pointer to the image cache
******************************************************************************/
void Paint::Paint_SelectImage(UBYTE *image)
{
    this->Image = image;
}

/******************************************************************************
function: Select Image Rotate
parameter:
    Rotate : 0,90,180,270
******************************************************************************/
void Paint::Paint_SetRotate(UWORD Rotate)
{
    if(Rotate == ROTATE_0 || Rotate == ROTATE_90 || Rotate == ROTATE_180 || Rotate == ROTATE_270) {
        Debug("Set image Rotate %d\r\n", Rotate);
        this->Rotate = Rotate;
    } else {
        Debug("rotate = 0, 90, 180, 270\r\n");
    }
}

/******************************************************************************
function:	Select Image mirror
parameter:
    mirror   :Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
void Paint::Paint_SetMirroring(UBYTE mirror)
{
    if(mirror == MIRROR_NONE || mirror == MIRROR_HORIZONTAL || 
        mirror == MIRROR_VERTICAL || mirror == MIRROR_ORIGIN) {
        Debug("mirror image x:%s, y:%s\r\n",(mirror & 0x01)? "mirror":"none", ((mirror >> 1) & 0x01)? "mirror":"none");
        this->Mirror = mirror;
    } else {
        Debug("mirror should be MIRROR_NONE, MIRROR_HORIZONTAL, \
        MIRROR_VERTICAL or MIRROR_ORIGIN\r\n");
    }    
}

void Paint::Paint_SetScale(UBYTE scale)
{
    if(scale == 2){
        this->Scale = scale;
        this->WidthByte = (this->WidthMemory % 8 == 0)? (this->WidthMemory / 8 ): (this->WidthMemory / 8 + 1);
    }else if(scale == 4){
        this->Scale = scale;
        this->WidthByte = (this->WidthMemory % 4 == 0)? (this->WidthMemory / 4 ): (this->WidthMemory / 4 + 1);
    }else if(scale == 7 || scale == 16){
        /* 7 colours are only applicable with 5in65 e-Paper */
        /* 16 colours are used for dithering */
		this->Scale = scale;
		this->WidthByte = (this->WidthMemory % 2 == 0)? (this->WidthMemory / 2 ): (this->WidthMemory / 2 + 1);;
	}else{
        Debug("Set Scale Input parameter error\r\n");
        Debug("Scale Only support: 2 4 7 16\r\n");
    }
}
/******************************************************************************
function: Draw Pixels
parameter:
    Xpoint : At point X
    Ypoint : At point Y
    Color  : Painted colors
******************************************************************************/
void Paint::Paint_SetPixel(UWORD Xpoint, UWORD Ypoint, UWORD Color)
{
    if(Xpoint > this->Width || Ypoint > this->Height){
        Debug("Exceeding display boundaries\r\n");
        return;
    }      
    UWORD X, Y;
    switch(this->Rotate) {
    case 0:
        X = Xpoint;
        Y = Ypoint;  
        break;
    case 90:
        X = this->WidthMemory - Ypoint - 1;
        Y = Xpoint;
        break;
    case 180:
        X = this->WidthMemory - Xpoint - 1;
        Y = this->HeightMemory - Ypoint - 1;
        break;
    case 270:
        X = Ypoint;
        Y = this->HeightMemory - Xpoint - 1;
        break;
    default:
        return;
    }
    
    switch(this->Mirror) {
    case MIRROR_NONE:
        break;
    case MIRROR_HORIZONTAL:
        X = this->WidthMemory - X - 1;
        break;
    case MIRROR_VERTICAL:
        Y = this->HeightMemory - Y - 1;
        break;
    case MIRROR_ORIGIN:
        X = this->WidthMemory - X - 1;
        Y = this->HeightMemory - Y - 1;
        break;
    default:
        return;
    }

    if(X > this->WidthMemory || Y > this->HeightMemory){
        Debug("Exceeding display boundaries\r\n");
        return;
    }
    
    if(this->Scale == 2){
        UDOUBLE Addr = X / 8 + Y * this->WidthByte;
        UBYTE Rdata = this->Image[Addr];
        if(Color == BLACK)
            this->Image[Addr] = Rdata & ~(0x80 >> (X % 8));
        else
            this->Image[Addr] = Rdata | (0x80 >> (X % 8));
    }else if(this->Scale == 4){
        UDOUBLE Addr = X / 4 + Y * this->WidthByte;
        Color = Color % 4;//Guaranteed color scale is 4  --- 0~3
        UBYTE Rdata = this->Image[Addr];
        Rdata = Rdata & (~(0xC0 >> ((X % 4)*2)));//Clear first, then set value
        this->Image[Addr] = Rdata | ((Color << 6) >> ((X % 4)*2));
    }else if(this->Scale == 7 || this->Scale == 16){
		UDOUBLE Addr = X / 2  + Y * this->WidthByte;
		UBYTE Rdata = this->Image[Addr];
		Rdata = Rdata & (~(0xF0 >> ((X % 2)*4)));//Clear first, then set value
		this->Image[Addr] = Rdata | ((Color << 4) >> ((X % 2)*4));
		// printf("Add =  %d ,data = %d\r\n",Addr,Rdata);
	}
}

/******************************************************************************
function: Clear the color of the picture
parameter:
    Color : Painted colors
******************************************************************************/
void Paint::Paint_Clear(UWORD Color)
{	
	if(this->Scale == 2) {
		for (UWORD Y = 0; Y < this->HeightByte; Y++) {
			for (UWORD X = 0; X < this->WidthByte; X++ ) {//8 pixel =  1 byte
				UDOUBLE Addr = X + Y*this->WidthByte;
				this->Image[Addr] = Color;
			}
		}		
    }else if(this->Scale == 4) {
        for (UWORD Y = 0; Y < this->HeightByte; Y++) {
			for (UWORD X = 0; X < this->WidthByte; X++ ) {
				UDOUBLE Addr = X + Y*this->WidthByte;
				this->Image[Addr] = (Color<<6)|(Color<<4)|(Color<<2)|Color;
			}
		}		
	}else if(this->Scale == 7 || this->Scale == 16) {
		for (UWORD Y = 0; Y < this->HeightByte; Y++) {
			for (UWORD X = 0; X < this->WidthByte; X++ ) {
				UDOUBLE Addr = X + Y*this->WidthByte;
				this->Image[Addr] = (Color<<4)|Color;
			}
		}		
	}
}

/******************************************************************************
function: Clear the color of a window
parameter:
    Xstart : x starting point
    Ystart : Y starting point
    Xend   : x end point
    Yend   : y end point
    Color  : Painted colors
******************************************************************************/
void Paint::Paint_ClearWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color)
{
    UWORD X, Y;
    for (Y = Ystart; Y < Yend; Y++) {
        for (X = Xstart; X < Xend; X++) {//8 pixel =  1 byte
            this->Paint_SetPixel(X, Y, Color);
        }
    }
}

/******************************************************************************
function: Draw Point(Xpoint, Ypoint) Fill the color
parameter:
    Xpoint		: The Xpoint coordinate of the point
    Ypoint		: The Ypoint coordinate of the point
    Color		: Painted color
    Dot_Pixel	: point size
    Dot_Style	: point Style
******************************************************************************/
void Paint::Paint_DrawPoint(UWORD Xpoint, UWORD Ypoint, UWORD Color,
                     DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style)
{
    if (Xpoint > this->Width || Ypoint > this->Height) {
        Debug("Paint_DrawPoint Input exceeds the normal display range\r\n");
        return;
    }

    int16_t XDir_Num , YDir_Num;
    if (Dot_Style == DOT_FILL_AROUND) {
        for (XDir_Num = 0; XDir_Num < 2 * Dot_Pixel - 1; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++) {
                if(Xpoint + XDir_Num - Dot_Pixel < 0 || Ypoint + YDir_Num - Dot_Pixel < 0)
                    break;
                // printf("x = %d, y = %d\r\n", Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel);
                this->Paint_SetPixel(Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
            }
        }
    } else {
        for (XDir_Num = 0; XDir_Num <  Dot_Pixel; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num <  Dot_Pixel; YDir_Num++) {
                this->Paint_SetPixel(Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
            }
        }
    }
}

/******************************************************************************
function: Draw a line of arbitrary slope
parameter:
    Xstart ：Starting Xpoint point coordinates
    Ystart ：Starting Xpoint point coordinates
    Xend   ：End point Xpoint coordinate
    Yend   ：End point Ypoint coordinate
    Color  ：The color of the line segment
    Line_width : Line width
    Line_Style: Solid and dotted lines
******************************************************************************/
void Paint::Paint_DrawLine(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,
                    UWORD Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style)
{
    if (Xstart > this->Width || Ystart > this->Height ||
        Xend > this->Width || Yend > this->Height) {
        Debug("Paint_DrawLine Input exceeds the normal display range\r\n");
        return;
    }

    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;
    int dx = (int)Xend - (int)Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
    int dy = (int)Yend - (int)Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

    // Increment direction, 1 is positive, -1 is counter;
    int XAddway = Xstart < Xend ? 1 : -1;
    int YAddway = Ystart < Yend ? 1 : -1;

    //Cumulative error
    int Esp = dx + dy;
    char Dotted_Len = 0;

    for (;;) {
        Dotted_Len++;
        //Painted dotted line, 2 point is really virtual
        if (Line_Style == LINE_STYLE_DOTTED && Dotted_Len % 3 == 0) {
            //Debug("LINE_DOTTED\r\n");
            this->Paint_DrawPoint(Xpoint, Ypoint, IMAGE_BACKGROUND, Line_width, DOT_STYLE_DFT);
            Dotted_Len = 0;
        } else {
            this->Paint_DrawPoint(Xpoint, Ypoint, Color, Line_width, DOT_STYLE_DFT);
        }
        if (2 * Esp >= dy) {
            if (Xpoint == Xend)
                break;
            Esp += dy;
            Xpoint += XAddway;
        }
        if (2 * Esp <= dx) {
            if (Ypoint == Yend)
                break;
            Esp += dx;
            Ypoint += YAddway;
        }
    }
}

/******************************************************************************
function: Draw a rectangle
parameter:
    Xstart ：Rectangular  Starting Xpoint point coordinates
    Ystart ：Rectangular  Starting Xpoint point coordinates
    Xend   ：Rectangular  End point Xpoint coordinate
    Yend   ：Rectangular  End point Ypoint coordinate
    Color  ：The color of the Rectangular segment
    Line_width: Line width
    Draw_Fill : Whether to fill the inside of the rectangle
******************************************************************************/
void Paint::Paint_DrawRectangle(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,
                         UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)
{
    if (Xstart > this->Width || Ystart > this->Height ||
        Xend > this->Width || Yend > this->Height) {
        Debug("Input exceeds the normal display range\r\n");
        return;
    }

    if (Draw_Fill) {
        UWORD Ypoint;
        for(Ypoint = Ystart; Ypoint < Yend; Ypoint++) {
            this->Paint_DrawLine(Xstart, Ypoint, Xend, Ypoint, Color , Line_width, LINE_STYLE_SOLID);
        }
    } else {
        this->Paint_DrawLine(Xstart, Ystart, Xend, Ystart, Color, Line_width, LINE_STYLE_SOLID);
        this->Paint_DrawLine(Xstart, Ystart, Xstart, Yend, Color, Line_width, LINE_STYLE_SOLID);
        this->Paint_DrawLine(Xend, Yend, Xend, Ystart, Color, Line_width, LINE_STYLE_SOLID);
        this->Paint_DrawLine(Xend, Yend, Xstart, Yend, Color, Line_width, LINE_STYLE_SOLID);
    }
}

/******************************************************************************
function: Use the 8-point method to draw a circle of the
            specified size at the specified position->
parameter:
    X_Center  ：Center X coordinate
    Y_Center  ：Center Y coordinate
    Radius    ：circle Radius
    Color     ：The color of the ：circle segment
    Line_width: Line width
    Draw_Fill : Whether to fill the inside of the Circle
******************************************************************************/
void Paint::Paint_DrawCircle(UWORD X_Center, UWORD Y_Center, UWORD Radius,
                      UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill)
{
    if (X_Center > this->Width || Y_Center >= this->Height) {
        Debug("Paint_DrawCircle Input exceeds the normal display range\r\n");
        return;
    }

    //Draw a circle from(0, R) as a starting point
    int16_t XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;

    //Cumulative error,judge the next point of the logo
    int16_t Esp = 3 - (Radius << 1 );

    int16_t sCountY;
    if (Draw_Fill == DRAW_FILL_FULL) {
        while (XCurrent <= YCurrent ) { //Realistic circles
            for (sCountY = XCurrent; sCountY <= YCurrent; sCountY ++ ) {
                this->Paint_DrawPoint(X_Center + XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//1
                this->Paint_DrawPoint(X_Center - XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//2
                this->Paint_DrawPoint(X_Center - sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//3
                this->Paint_DrawPoint(X_Center - sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//4
                this->Paint_DrawPoint(X_Center - XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//5
                this->Paint_DrawPoint(X_Center + XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//6
                this->Paint_DrawPoint(X_Center + sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//7
                this->Paint_DrawPoint(X_Center + sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            }
            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    } else { //Draw a hollow circle
        while (XCurrent <= YCurrent ) {
            this->Paint_DrawPoint(X_Center + XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//1
            this->Paint_DrawPoint(X_Center - XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//2
            this->Paint_DrawPoint(X_Center - YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//3
            this->Paint_DrawPoint(X_Center - YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//4
            this->Paint_DrawPoint(X_Center - XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//5
            this->Paint_DrawPoint(X_Center + XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//6
            this->Paint_DrawPoint(X_Center + YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//7
            this->Paint_DrawPoint(X_Center + YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//0

            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    }
}

/******************************************************************************
function: Show English characters
parameter:
    Xpoint           ：X coordinate
    Ypoint           ：Y coordinate
    Acsii_Char       ：To display the English characters
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Paint::Paint_DrawChar(UWORD Xpoint, UWORD Ypoint, const char Acsii_Char,
                    sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Page, Column;

    if (Xpoint > this->Width || Ypoint > this->Height) {
        Debug("Paint_DrawChar Input exceeds the normal display range\r\n");
        return;
    }

    uint32_t Char_Offset = (Acsii_Char - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
    const unsigned char *ptr = &Font->table[Char_Offset];

    for (Page = 0; Page < Font->Height; Page ++ ) {
        for (Column = 0; Column < Font->Width; Column ++ ) {

            //To determine whether the font background color and screen background color is consistent
            if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
                if (*ptr & (0x80 >> (Column % 8)))
                    this->Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // this->Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            } else {
                if (*ptr & (0x80 >> (Column % 8))) {
                    this->Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Foreground);
                    // this->Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                } else {
                    this->Paint_SetPixel(Xpoint + Column, Ypoint + Page, Color_Background);
                    // this->Paint_DrawPoint(Xpoint + Column, Ypoint + Page, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                }
            }
            //One pixel is 8 bits
            if (Column % 8 == 7)
                ptr++;
        }// Write a line
        if (Font->Width % 8 != 0)
            ptr++;
    }// Write all
}

/******************************************************************************
function:	Display the string
parameter:
    Xstart           ：X coordinate
    Ystart           ：Y coordinate
    pString          ：The first address of the English string to be displayed
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Paint::Paint_DrawString_EN(UWORD Xstart, UWORD Ystart, const char * pString,
                         sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;

    if (Xstart > this->Width || Ystart > this->Height) {
        Debug("Paint_DrawString_EN Input exceeds the normal display range\r\n");
        return;
    }

    while (* pString != '\0') {
        //if X direction filled , reposition to(Xstart,Ypoint),Ypoint is Y direction plus the Height of the character
        if ((Xpoint + Font->Width ) > this->Width ) {
            Xpoint = Xstart;
            Ypoint += Font->Height;
        }

        // If the Y direction is full, reposition to(Xstart, Ystart)
        if ((Ypoint  + Font->Height ) > this->Height ) {
            Xpoint = Xstart;
            Ypoint = Ystart;
        }
        this->Paint_DrawChar(Xpoint, Ypoint, * pString, Font, Color_Background, Color_Foreground);

        //The next character of the address
        pString ++;

        //The next word of the abscissa increases the font of the broadband
        Xpoint += Font->Width;
    }
}


/******************************************************************************
function: Display the string
parameter:
    Xstart  ：X coordinate
    Ystart  ：Y coordinate
    pString ：The first address of the Chinese string and English
              string to be displayed
    Font    ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Paint::Paint_DrawString_CN(UWORD Xstart, UWORD Ystart, const char * pString, cFONT* font,
                        UWORD Color_Foreground, UWORD Color_Background)
{
    const char* p_text = pString;
    int x = Xstart, y = Ystart;
    int i, j,Num;

    /* Send the string character by character on EPD */
    while (*p_text != 0) {
        if(*p_text <= 0x7F) {  //ASCII < 126
            for(Num = 0; Num < font->size; Num++) {
                if(*p_text== font->table[Num].index[0]) {
                    const char* ptr = &font->table[Num].matrix[0];

                    for (j = 0; j < font->Height; j++) {
                        for (i = 0; i < font->Width; i++) {
                            if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
                                if (*ptr & (0x80 >> (i % 8))) {
                                    this->Paint_SetPixel(x + i, y + j, Color_Foreground);
                                    // this->Paint_DrawPoint(x + i, y + j, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                                }
                            } else {
                                if (*ptr & (0x80 >> (i % 8))) {
                                    this->Paint_SetPixel(x + i, y + j, Color_Foreground);
                                    // this->Paint_DrawPoint(x + i, y + j, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                                } else {
                                    this->Paint_SetPixel(x + i, y + j, Color_Background);
                                    // this->Paint_DrawPoint(x + i, y + j, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                                }
                            }
                            if (i % 8 == 7) {
                                ptr++;
                            }
                        }
                        if (font->Width % 8 != 0) {
                            ptr++;
                        }
                    }
                    break;
                }
            }
            /* Point on the next character */
            p_text += 1;
            /* Decrement the column position by 16 */
            x += font->ASCII_Width;
        } else {        //Chinese
            for(Num = 0; Num < font->size; Num++) {
                if((*p_text== font->table[Num].index[0]) && (*(p_text+1) == font->table[Num].index[1])) {
                    const char* ptr = &font->table[Num].matrix[0];

                    for (j = 0; j < font->Height; j++) {
                        for (i = 0; i < font->Width; i++) {
                            if (FONT_BACKGROUND == Color_Background) { //this process is to speed up the scan
                                if (*ptr & (0x80 >> (i % 8))) {
                                    this->Paint_SetPixel(x + i, y + j, Color_Foreground);
                                    // this->Paint_DrawPoint(x + i, y + j, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                                }
                            } else {
                                if (*ptr & (0x80 >> (i % 8))) {
                                    this->Paint_SetPixel(x + i, y + j, Color_Foreground);
                                    // this->Paint_DrawPoint(x + i, y + j, Color_Foreground, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                                } else {
                                    this->Paint_SetPixel(x + i, y + j, Color_Background);
                                    // this->Paint_DrawPoint(x + i, y + j, Color_Background, DOT_PIXEL_DFT, DOT_STYLE_DFT);
                                }
                            }
                            if (i % 8 == 7) {
                                ptr++;
                            }
                        }
                        if (font->Width % 8 != 0) {
                            ptr++;
                        }
                    }
                    break;
                }
            }
            /* Point on the next character */
            p_text += 2;
            /* Decrement the column position by 16 */
            x += font->Width;
        }
    }
}

/******************************************************************************
function:	Display nummber
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    Nummber          : The number displayed
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
#define  ARRAY_LEN 255
void Paint::Paint_DrawNum(UWORD Xpoint, UWORD Ypoint, int32_t Nummber,
                   sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{

    int16_t Num_Bit = 0, Str_Bit = 0;
    uint8_t Str_Array[ARRAY_LEN] = {0}, Num_Array[ARRAY_LEN] = {0};
    uint8_t *pStr = Str_Array;

    if (Xpoint > this->Width || Ypoint > this->Height) {
        Debug("Paint_DisNum Input exceeds the normal display range\r\n");
        return;
    }

    //Converts a number to a string
    do {
        Num_Array[Num_Bit] = Nummber % 10 + '0';
        Num_Bit++;
        Nummber /= 10;
    } while(Nummber);
    

    //The string is inverted
    while (Num_Bit > 0) {
        Str_Array[Str_Bit] = Num_Array[Num_Bit - 1];
        Str_Bit ++;
        Num_Bit --;
    }

    //show
    this->Paint_DrawString_EN(Xpoint, Ypoint, (const char*)pStr, Font, Color_Background, Color_Foreground);
}

/******************************************************************************
function:	Display nummber (Able to display decimals)
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    Nummber          : The number displayed
    Font             ：A structure pointer that displays a character size
    Digit            : Fractional width
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Paint::Paint_DrawNumDecimals(UWORD Xpoint, UWORD Ypoint, double Nummber,
                    sFONT* Font, UWORD Digit, UWORD Color_Foreground, UWORD Color_Background)
{
    int16_t Num_Bit = 0, Str_Bit = 0;
    uint8_t Str_Array[ARRAY_LEN] = {0}, Num_Array[ARRAY_LEN] = {0};
    uint8_t *pStr = Str_Array;
	int temp = Nummber;
	float decimals;
	uint8_t i;
    if (Xpoint > this->Width || Ypoint > this->Height) {
        Debug("Paint_DisNum Input exceeds the normal display range\r\n");
        return;
    }

	if(Digit > 0) {		
		decimals = Nummber - temp;
		for(i=Digit; i > 0; i--) {
			decimals*=10;
		}
		temp = decimals;
		//Converts a number to a string
		for(i=Digit; i>0; i--) {
			Num_Array[Num_Bit] = temp % 10 + '0';
			Num_Bit++;
			temp /= 10;						
		}	
		Num_Array[Num_Bit] = '.';
		Num_Bit++;
	}

	temp = Nummber;
    //Converts a number to a string
    do {
        Num_Array[Num_Bit] = temp % 10 + '0';
        Num_Bit++;
        temp /= 10;
    } while(temp);

    //The string is inverted
    while (Num_Bit > 0) {
        Str_Array[Str_Bit] = Num_Array[Num_Bit - 1];
        Str_Bit ++;
        Num_Bit --;
    }

    //show
    this->Paint_DrawString_EN(Xpoint, Ypoint, (const char*)pStr, Font, Color_Background, Color_Foreground);
}

/******************************************************************************
function:	Display time
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    pTime            : Time-related structures
    Font             ：A structure pointer that displays a character size
    Color_Foreground : Select the foreground color
    Color_Background : Select the background color
******************************************************************************/
void Paint::Paint_DrawTime(UWORD Xstart, UWORD Ystart, PAINT_TIME *pTime, sFONT* Font, UWORD Color_Foreground, UWORD Color_Background)
{
    uint8_t value[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    UWORD Dx = Font->Width;

    //Write data into the cache
    this->Paint_DrawChar(Xstart                           , Ystart, value[pTime->Hour / 10], Font, Color_Background, Color_Foreground);
    this->Paint_DrawChar(Xstart + Dx                      , Ystart, value[pTime->Hour % 10], Font, Color_Background, Color_Foreground);
    this->Paint_DrawChar(Xstart + Dx  + Dx / 4 + Dx / 2   , Ystart, ':'                    , Font, Color_Background, Color_Foreground);
    this->Paint_DrawChar(Xstart + Dx * 2 + Dx / 2         , Ystart, value[pTime->Min / 10] , Font, Color_Background, Color_Foreground);
    this->Paint_DrawChar(Xstart + Dx * 3 + Dx / 2         , Ystart, value[pTime->Min % 10] , Font, Color_Background, Color_Foreground);
    this->Paint_DrawChar(Xstart + Dx * 4 + Dx / 2 - Dx / 4, Ystart, ':'                    , Font, Color_Background, Color_Foreground);
    this->Paint_DrawChar(Xstart + Dx * 5                  , Ystart, value[pTime->Sec / 10] , Font, Color_Background, Color_Foreground);
    this->Paint_DrawChar(Xstart + Dx * 6                  , Ystart, value[pTime->Sec % 10] , Font, Color_Background, Color_Foreground);
}

/******************************************************************************
function:	Display monochrome bitmap
parameter:
    image_buffer ：A picture data converted to a bitmap
info:
    Use a computer to convert the image into a corresponding array,
    and then embed the array directly into Imagedata.cpp as a .c file.
******************************************************************************/
void Paint::Paint_DrawBitMap(const unsigned char* image_buffer)
{
    UWORD x, y;
    UDOUBLE Addr = 0;

    for (y = 0; y < this->HeightByte; y++) {
        for (x = 0; x < this->WidthByte; x++) {//8 pixel =  1 byte
            Addr = x + y * this->WidthByte;
            this->Image[Addr] = (unsigned char)image_buffer[Addr];
        }
    }
}

/*****************************************************************************
* | File      	:   GUI_BMPfile.h
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master
*                and enhance portability
*----------------
* |	This version:   V2.3
* | Date        :   2022-07-27
* | Info        :   
* -----------------------------------------------------------------------------
* V2.3(2022-07-27):
* 1.Add GUI_ReadBmp_RGB_4Color()
* V2.2(2020-07-08):
* 1.Add GUI_ReadBmp_RGB_7Color()
* V2.1(2019-10-10):
* 1.Add GUI_ReadBmp_4Gray()
* V2.0(2018-11-12):
* 1.Change file name: GUI_BMP.h -> GUI_BMPfile.h
* 2.fix: GUI_ReadBmp()
*   Now Xstart and Xstart can control the position of the picture normally,
*   and support the display of images of any size. If it is larger than
*   the actual display range, it will not be displayed.
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

UBYTE Paint::GUI_ReadBmp(const char *path, UWORD Xstart, UWORD Ystart)
{
    FILE *fp;                     //Define a file pointer
    BMPFILEHEADER bmpFileHeader;  //Define a bmp file header structure
    BMPINFOHEADER bmpInfoHeader;  //Define a bmp info header structure


    // Binary file open
   if((fp = fopen(path, "rb")) == NULL) {
        Debug("Cann't open the file!\n");
        exit(0);
    }

    // Set the file pointer from the beginning
    fseek(fp, 0, SEEK_SET);
    fread(&bmpFileHeader, sizeof(BMPFILEHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 14
    fread(&bmpInfoHeader, sizeof(BMPINFOHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 50
    printf("pixel = %d * %d\r\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    UWORD Image_Width_Byte = (bmpInfoHeader.biWidth % 8 == 0)? (bmpInfoHeader.biWidth / 8): (bmpInfoHeader.biWidth / 8 + 1);
    UWORD Bmp_Width_Byte = (Image_Width_Byte % 4 == 0) ? Image_Width_Byte: ((Image_Width_Byte / 4 + 1) * 4);
    UBYTE Image[Image_Width_Byte * bmpInfoHeader.biHeight];
    memset(Image, 0xFF, Image_Width_Byte * bmpInfoHeader.biHeight);

    // Determine if it is a monochrome bitmap
    int readbyte = bmpInfoHeader.biBitCount;
    if(readbyte != 1) {
        Debug("the bmp Image is not a monochrome bitmap!\n");
        exit(0);
    }

    // Determine black and white based on the palette
    UWORD i;
    UWORD Bcolor, Wcolor;
    UWORD bmprgbquadsize = pow(2, bmpInfoHeader.biBitCount);// 2^1 = 2
    BMPRGBQUAD bmprgbquad[bmprgbquadsize];        //palette
    // BMPRGBQUAD bmprgbquad[2];        //palette

    for(i = 0; i < bmprgbquadsize; i++){
    // for(i = 0; i < 2; i++) {
        fread(&bmprgbquad[i], sizeof(BMPRGBQUAD), 1, fp);
    }
    if(bmprgbquad[0].rgbBlue == 0xff && bmprgbquad[0].rgbGreen == 0xff && bmprgbquad[0].rgbRed == 0xff) {
        Bcolor = BLACK;
        Wcolor = WHITE;
    } else {
        Bcolor = WHITE;
        Wcolor = BLACK;
    }

    // Read image data into the cache
    UWORD x, y;
    UBYTE Rdata;
    fseek(fp, bmpFileHeader.bOffset, SEEK_SET);
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {//Total display column
        for(x = 0; x < Bmp_Width_Byte; x++) {//Show a line in the line
            if(fread((char *)&Rdata, 1, readbyte, fp) != readbyte) {
                perror("get bmpdata:\r\n");
                break;
            }
            if(x < Image_Width_Byte) { //bmp
                Image[x + (bmpInfoHeader.biHeight - y - 1) * Image_Width_Byte] =  Rdata;
                // printf("rdata = %d\r\n", Rdata);
            }
        }
    }
    fclose(fp);

    // Refresh the image to the display buffer based on the displayed orientation
    UBYTE color, temp;
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {
        for(x = 0; x < bmpInfoHeader.biWidth; x++) {
            if(x > this->Width || y > this->Height) {
                break;
            }
            temp = Image[(x / 8) + (y * Image_Width_Byte)];
            color = (((temp << (x%8)) & 0x80) == 0x80) ?Bcolor:Wcolor;
            this->Paint_SetPixel(Xstart + x, Ystart + y, color);
        }
    }
    return 0;
}
/*************************************************************************

*************************************************************************/
UBYTE Paint::GUI_ReadBmp_4Gray(const char *path, UWORD Xstart, UWORD Ystart)
{
    FILE *fp;                     //Define a file pointer
    BMPFILEHEADER bmpFileHeader;  //Define a bmp file header structure
    BMPINFOHEADER bmpInfoHeader;  //Define a bmp info header structure
    
    // Binary file open
    if((fp = fopen(path, "rb")) == NULL) {
        Debug("Cann't open the file!\n");
        exit(0);
    }
 
    // Set the file pointer from the beginning
    fseek(fp, 0, SEEK_SET);
    fread(&bmpFileHeader, sizeof(BMPFILEHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 14
    fread(&bmpInfoHeader, sizeof(BMPINFOHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 50
    printf("pixel = %d * %d\r\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    UWORD Image_Width_Byte = (bmpInfoHeader.biWidth % 4 == 0)? (bmpInfoHeader.biWidth / 4): (bmpInfoHeader.biWidth / 4 + 1);
    UWORD Bmp_Width_Byte = (bmpInfoHeader.biWidth % 2 == 0)? (bmpInfoHeader.biWidth / 2): (bmpInfoHeader.biWidth / 2 + 1);
    UBYTE Image[Image_Width_Byte * bmpInfoHeader.biHeight * 2];
    memset(Image, 0xFF, Image_Width_Byte * bmpInfoHeader.biHeight * 2);

    // Determine if it is a monochrome bitmap
    int readbyte = bmpInfoHeader.biBitCount;
    printf("biBitCount = %d\r\n",readbyte);
    if(readbyte != 4){
        Debug("Bmp image is not a 4-color bitmap!\n");
        exit(0);
    }
    // Read image data into the cache
    UWORD x, y;
    UBYTE Rdata;
    fseek(fp, bmpFileHeader.bOffset, SEEK_SET);
    
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {//Total display column
        for(x = 0; x < Bmp_Width_Byte; x++) {//Show a line in the line
            if(fread((char *)&Rdata, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
            if(x < Image_Width_Byte*2) { //bmp
                Image[x + (bmpInfoHeader.biHeight - y - 1) * Image_Width_Byte*2] =  Rdata;
            }
        }
    }
    fclose(fp);
    
    // Refresh the image to the display buffer based on the displayed orientation
    UBYTE color, temp;
    printf("bmpInfoHeader.biWidth = %d\r\n",bmpInfoHeader.biWidth);
    printf("bmpInfoHeader.biHeight = %d\r\n",bmpInfoHeader.biHeight);
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {
        for(x = 0; x < bmpInfoHeader.biWidth; x++) {
            if(x > this->Width || y > this->Height) {
                break;
            }
            temp = Image[x/2 + y * bmpInfoHeader.biWidth/2] >> ((x%2)? 0:4);//0xf 0x8 0x7 0x0 
            color = temp>>2;                           //11  10  01  00  
            this->Paint_SetPixel(Xstart + x, Ystart + y, color);
        }
    }
    return 0;
}

UBYTE Paint::GUI_ReadBmp_16Gray(const char *path, UWORD Xstart, UWORD Ystart)
{
    FILE *fp;                     //Define a file pointer
    BMPFILEHEADER bmpFileHeader;  //Define a bmp file header structure
    BMPINFOHEADER bmpInfoHeader;  //Define a bmp info header structure

    // Binary file open
    if((fp = fopen(path, "rb")) == NULL) {
        Debug("Cann't open the file!\n");
        exit(0);
    }

    // Set the file pointer from the beginning
    fseek(fp, 0, SEEK_SET);
    fread(&bmpFileHeader, sizeof(BMPFILEHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 14
    fread(&bmpInfoHeader, sizeof(BMPINFOHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 50
    printf("pixel = %d * %d\r\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    // They are both the same width in bytes
    // round up to the next byte
    UWORD Width_Byte = (bmpInfoHeader.biWidth + 1) / 2;
    UBYTE Image[Width_Byte * bmpInfoHeader.biHeight];
    memset(Image, 0xFF, Width_Byte * bmpInfoHeader.biHeight);

    // Determine if it is a monochrome bitmap
    int readbyte = bmpInfoHeader.biBitCount;
    printf("biBitCount = %d\r\n",readbyte);
    if(readbyte != 4) {
        Debug("Bmp image is not a 4-bit bitmap!\n");
        exit(0);
    }

    // Determine colors based on the palette

    // A map from palette entry to color
    UBYTE colors[16];
    UBYTE i;
    BMPRGBQUAD rgbData;

    for (i = 0; i < 16; i++){
        fread(&rgbData, sizeof(BMPRGBQUAD), 1, fp);

        // Work out the closest colour
        // 16 colours over 0-255 => 0-8 => 0, 9-25 => 1 (17), 26-42 => 2 (34), etc

        // Base it on red
        colors[i] = (rgbData.rgbRed + 8) / 17;
    }

    // Read image data into the cache
    UWORD x, y;
    UBYTE Rdata;
    fseek(fp, bmpFileHeader.bOffset, SEEK_SET);

    for (y = 0; y < bmpInfoHeader.biHeight; y++) {//Total display column
        for (x = 0; x < Width_Byte; x++) {//Show a line in the line
            if (fread((char *) &Rdata, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
            Image[x + (bmpInfoHeader.biHeight - y - 1) * Width_Byte] = Rdata;
        }
    }
    fclose(fp);

    // Refresh the image to the display buffer based on the displayed orientation
    UBYTE coloridx;
    printf("bmpInfoHeader.biWidth = %d\r\n", bmpInfoHeader.biWidth);
    printf("bmpInfoHeader.biHeight = %d\r\n", bmpInfoHeader.biHeight);
    for (y = 0; y < bmpInfoHeader.biHeight; y++) {
        for (x = 0; x < bmpInfoHeader.biWidth; x++) {
            if (Xstart + x > this->Width || Ystart + y > this->Height)
                break;

            coloridx = (Image[x / 2 + y * Width_Byte] >> ((x % 2) ? 0 : 4)) & 15;
            this->Paint_SetPixel(Xstart + x, Ystart + y, colors[coloridx]);
        }
    }
    return 0;
}

UBYTE Paint::GUI_ReadBmp_RGB_7Color(const char *path, UWORD Xstart, UWORD Ystart)
{
    FILE *fp;                     //Define a file pointer
    BMPFILEHEADER bmpFileHeader;  //Define a bmp file header structure
    BMPINFOHEADER bmpInfoHeader;  //Define a bmp info header structure
    
    // Binary file open
    if((fp = fopen(path, "rb")) == NULL) {
        Debug("Cann't open the file!\n");
        exit(0);
    }

    // Set the file pointer from the beginning
    fseek(fp, 0, SEEK_SET);
    fread(&bmpFileHeader, sizeof(BMPFILEHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 14
    fread(&bmpInfoHeader, sizeof(BMPINFOHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 50
    printf("pixel = %d * %d\r\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
	
    UDOUBLE Image_Byte = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3;
    UBYTE Image[Image_Byte];
    memset(Image, 0xFF, Image_Byte);

    // Determine if it is a monochrome bitmap
    int readbyte = bmpInfoHeader.biBitCount;
    if(readbyte != 24){
        Debug("Bmp image is not 24 bitmap!\n");
        exit(0);
    }
    // Read image data into the cache
    UWORD x, y;
    UBYTE Rdata[3];
    fseek(fp, bmpFileHeader.bOffset, SEEK_SET);
    
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {//Total display column
        for(x = 0; x < bmpInfoHeader.biWidth ; x++) {//Show a line in the line
            if(fread((char *)Rdata, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
			if(fread((char *)Rdata+1, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
			if(fread((char *)Rdata+2, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }

			if(Rdata[0] == 0 && Rdata[1] == 0 && Rdata[2] == 0){
				Image[x+(y* bmpInfoHeader.biWidth )] =  0;//Black
			}else if(Rdata[0] == 255 && Rdata[1] == 255 && Rdata[2] == 255){
				Image[x+(y* bmpInfoHeader.biWidth )] =  1;//White
			}else if(Rdata[0] == 0 && Rdata[1] == 255 && Rdata[2] == 0){
				Image[x+(y* bmpInfoHeader.biWidth )] =  2;//Green
			}else if(Rdata[0] == 255 && Rdata[1] == 0 && Rdata[2] == 0){
				Image[x+(y* bmpInfoHeader.biWidth )] =  3;//Blue
			}else if(Rdata[0] == 0 && Rdata[1] == 0 && Rdata[2] == 255){
				Image[x+(y* bmpInfoHeader.biWidth )] =  4;//Red
			}else if(Rdata[0] == 0 && Rdata[1] == 255 && Rdata[2] == 255){
				Image[x+(y* bmpInfoHeader.biWidth )] =  5;//Yellow
			}else if(Rdata[0] == 0 && Rdata[1] == 128 && Rdata[2] == 255){
				Image[x+(y* bmpInfoHeader.biWidth )] =  6;//Orange
			}
        }
    }
    fclose(fp);
   
    // Refresh the image to the display buffer based on the displayed orientation
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {
        for(x = 0; x < bmpInfoHeader.biWidth; x++) {
            if(x > this->Width || y > this->Height) {
                break;
            }
            this->Paint_SetPixel(Xstart + x, Ystart + y, Image[bmpInfoHeader.biHeight *  bmpInfoHeader.biWidth - 1 -(bmpInfoHeader.biWidth-x-1+(y* bmpInfoHeader.biWidth))]);
		}
    }
    return 0;
}

UBYTE Paint::GUI_ReadBmp_RGB_4Color(const char *path, UWORD Xstart, UWORD Ystart)
{
    FILE *fp;                     //Define a file pointer
    BMPFILEHEADER bmpFileHeader;  //Define a bmp file header structure
    BMPINFOHEADER bmpInfoHeader;  //Define a bmp info header structure
    
    // Binary file open
    if((fp = fopen(path, "rb")) == NULL) {
        Debug("Cann't open the file!\n");
        exit(0);
    }

    // Set the file pointer from the beginning
    fseek(fp, 0, SEEK_SET);
    fread(&bmpFileHeader, sizeof(BMPFILEHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 14
    fread(&bmpInfoHeader, sizeof(BMPINFOHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 50
    printf("pixel = %d * %d\r\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
	
    UDOUBLE Image_Byte = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3;
    UBYTE Image[Image_Byte];
    memset(Image, 0xFF, Image_Byte);

    // Determine if it is a monochrome bitmap
    int readbyte = bmpInfoHeader.biBitCount;
    if(readbyte != 24){
        Debug("Bmp image is not 24 bitmap!\n");
        exit(0);
    }
    // Read image data into the cache
    UWORD x, y;
    UBYTE Rdata[3];
    fseek(fp, bmpFileHeader.bOffset, SEEK_SET);
    
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {//Total display column
        for(x = 0; x < bmpInfoHeader.biWidth ; x++) {//Show a line in the line
            if(fread((char *)Rdata, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
			if(fread((char *)Rdata+1, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
			if(fread((char *)Rdata+2, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
			if(Rdata[0] < 128 && Rdata[1] < 128 && Rdata[2] < 128){
				Image[x+(y* bmpInfoHeader.biWidth )] =  0;//Black
			}else if(Rdata[0] > 127 && Rdata[1] > 127 && Rdata[2] > 127){
				Image[x+(y* bmpInfoHeader.biWidth )] =  1;//White
			}else if(Rdata[0] < 128 && Rdata[1] > 127 && Rdata[2] > 127){
				Image[x+(y* bmpInfoHeader.biWidth )] =  2;//Yellow
			}else if(Rdata[0] < 128 && Rdata[1] < 128 && Rdata[2] > 127){
				Image[x+(y* bmpInfoHeader.biWidth )] =  3;//Red
			}
        }
        if(bmpInfoHeader.biWidth % 4 != 0)
        {
            for (UWORD i = 0; i < (bmpInfoHeader.biWidth % 4); i++)
            {
                if(fread((char *)Rdata, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
                }
            }
        }
    }
    fclose(fp);
   
    // Refresh the image to the display buffer based on the displayed orientation
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {
        for(x = 0; x < bmpInfoHeader.biWidth; x++) {
            if(x > this->Width || y > this->Height) {
                break;
            }
            this->Paint_SetPixel(Xstart + x, Ystart + y, Image[bmpInfoHeader.biHeight *  bmpInfoHeader.biWidth - 1 -(bmpInfoHeader.biWidth-x-1+(y* bmpInfoHeader.biWidth))]);
		}
    }
    return 0;
}