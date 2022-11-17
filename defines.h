#ifndef _DEFINES_H_
#define _DEFINES_H_

#define	byte	unsigned char
#define	uchar	unsigned char
#define	uint	unsigned int
#define	ulong	unsigned long
#define	ushort	unsigned short

#define	HIGH(x)	((uchar)(x  >> 8))
#define	LOW(x)	((uchar)(x))


#define	GLCD		//использовать графический экран от Siemens C75

//#define	CYR_LCD		//для вывода текста кирилицей
#define	LCD_20		//20 символов в строке

#ifdef	GLCD
#undef	LCD_20
#define	CYR_LCD
#endif

#define	EXT_CLK		//использовать внешний кварц

#endif
