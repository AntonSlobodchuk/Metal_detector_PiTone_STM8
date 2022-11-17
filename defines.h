#ifndef _DEFINES_H_
#define _DEFINES_H_

#define	byte	unsigned char
#define	uchar	unsigned char
#define	uint	unsigned int
#define	ulong	unsigned long
#define	ushort	unsigned short

#define	HIGH(x)	((uchar)(x  >> 8))
#define	LOW(x)	((uchar)(x))


#define	GLCD		//������������ ����������� ����� �� Siemens C75

//#define	CYR_LCD		//��� ������ ������ ���������
#define	LCD_20		//20 �������� � ������

#ifdef	GLCD
#undef	LCD_20
#define	CYR_LCD
#endif

#define	EXT_CLK		//������������ ������� �����

#endif
