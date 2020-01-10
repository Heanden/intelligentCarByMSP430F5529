#ifndef __LQ12864_H__
#define __LQ12864_H__

#define LCD_DC_SET_IO_INIT P8DIR |= BIT2;
#define LCD_DC_SET P8OUT |= BIT2; //dc 1 ¶ÁÐ´Êý¾Ý
#define LCD_DC_CLR P8OUT &= ~BIT2;         //dc 0 ¶ÁÐ´ÃüÁî

#define LCD_RST_SET_IO_INIT P3DIR |= BIT7;
#define LCD_RST_SET P3OUT |= BIT7;  //
#define LCD_RST_CLR P3OUT &= ~BIT7; //

#define LCD_SDA_SET_IO_INIT P4DIR |= BIT0;
#define LCD_SDA_SET P4OUT |= BIT0;  // D1
#define LCD_SDA_CLR P4OUT &= ~BIT0; //

#define LCD_SCL_SET_IO_INIT P4DIR |= BIT3;
#define LCD_SCL_SET P4OUT |= BIT3;  // D0
#define LCD_SCL_CLR P4OUT &= ~BIT3; //

#define X_WIDTH 128
#define Y_WIDTH 64

extern void LCD_Init(void);
extern void LCD_CLS(void);
extern void LCD_P6x8Str(unsigned char x, unsigned char y, unsigned char ch[]);
extern void LCD_P8x16Str(unsigned char x, unsigned char y, unsigned char ch[]);
extern void LCD_P14x16Str(unsigned char x, unsigned char y, unsigned char ch[]);
extern void LCD_Print(unsigned char x, unsigned char y, unsigned char ch[]);
extern void LCD_PutPixel(unsigned char x, unsigned char y);
extern void LCD_Rectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char gif);
extern void Draw_LQLogo(void);
extern void Draw_LibLogo(void);
extern void Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char *bmp);

#endif
