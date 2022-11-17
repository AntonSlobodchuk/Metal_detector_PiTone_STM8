#include "iostm8s003f3.h"
#include "intrinsics.h"     //����� ������� ������� __enable_interrupt()
#include "itoa_ltoa.h"

char buf[10];
unsigned int with, adc_bat;
unsigned char n,i;

//******************************************************************************
// ������������� UART1 STM8S003
//******************************************************************************.
void uart_init(unsigned long baud_rate, unsigned long f_master){
//�������� �������� BRR
unsigned long brr;
//����������� TX �� �����, � RX �� ����
PD_DDR_bit.DDR5 = 1;  //TX
PD_DDR_bit.DDR6 = 0;  //RX
//RX - ��������� ����
PD_CR1_bit.C16 = 0;
//��������� ������� ���������� ��� RX.
PD_CR2_bit.C26 = 0;

//��������� �������� ��������
brr = f_master/baud_rate;
 
UART1_BRR2 = brr & 0x000F;
UART1_BRR2 |= brr >> 12;
UART1_BRR1 = (brr >> 4) & 0x00FF;
//�������� ���������
UART1_CR1_PIEN = 0;
//�������� �������� ��������
UART1_CR1_PCEN = 0;
//8-������ �����
UART1_CR1_M = 0;
//�������� UART
UART1_CR1_UART0 = 0;
//��������� ���������� �� ����������� ��������. ��������
UART1_CR2_TIEN = 0;
//��������� ���������� �� ���������� ��������
UART1_CR2_TCIEN = 0;
//��������� ���������� �� ���������� ��������� ��������
UART1_CR2_RIEN = 0;
//��������� ���������� �� ������������ �����
UART1_CR2_ILIEN = 0;
//���������� ��������
UART1_CR2_TEN = 1;
//�������� ��������
UART1_CR2_REN = 1;
//�� �������� break-������
UART1_CR2_SBK = 0;
//���� ����-���
UART1_CR3_STOP = 0;
};

void usart_tx(unsigned char data)
{
  while(!UART1_SR_TXE); // ����� ������������ ������.
            UART1_DR = data; // ��������� ����.
}


void usart_print(char *usart_string){ //������� �������� ������ �� USART
	unsigned char tmp=0;
	while (usart_string[tmp]) // ���� �� ����� ������...
		{
		 while(!UART1_SR_TXE); // ����� ������������ ������.
		 UART1_DR =usart_string[tmp]; tmp++; // ��������� ����.
		}
};


void delay_ms(unsigned long int temp_cpu){
  temp_cpu=temp_cpu*550;
  while(temp_cpu){temp_cpu--;};

};

void delay_us(unsigned long int temp_cpu){
  temp_cpu=temp_cpu/2;
  while(temp_cpu){temp_cpu--;};

};

#pragma vector=TIM1_OVR_UIF_vector
 __interrupt void TIM1_OVR_UIF(void)
 {
TIM1_SR1 = 0;
//with=0;
 

 }

#pragma vector=TIM1_CAPCOM_CC1IF_vector
 __interrupt void TIM1_CAPCOM_CC1IF(void)
 {
   //PD_ODR_bit.ODR4=1;
 TIM1_SR1 = 0; // clear all interrupt flags
 TIM1_CR1 = 0;
 TIM1_CCER1 &= ~0x11; // stop timer
 
 with=TIM1_CCR2H;
 with=with<<8;
 with=with+TIM1_CCR2L;
/*
 itoa(with, buf);
    usart_print(buf);
    usart_tx(0x0d);
 */   
    TIM1_CCER1 |= 0x11;
  TIM1_CR1_CEN = 1; // CEN
 
 };
 
 void setup_adc( void ) {
   ADC_CR1  = 0x61; // enable ADC
   ADC_CR2  = 0x08; // right alignment
   ADC_CR3  = 0x00;
   ADC_TDRL = 0x60; // disable Schmitt triggers
};

unsigned int update_adc( void ) {
   unsigned char adc_temp_l, adc_temp_h;
   ADC_CSR  = 2;  // channel 2
   delay_ms(10);
   ADC_CR1 |= 0x01; // start conversion
   while (!(ADC_CSR & 0x80)); // wait
   adc_temp_l = ADC_DRL; // LSB first
   adc_temp_h = ADC_DRH; // LSB first
   return (adc_temp_h<<8)+adc_temp_l; 

};


void main( void )
{
    ///////////////////���������� �� ���������� ��������� 16 ���  
    CLK_ICKR = 0; // ���������� ������� ����������� ������������
    CLK_ICKR_HSIEN = 1; // �������� ���������� ��������� HSI
    CLK_ECKR = 0; // ��������� ������� ���������
    while (CLK_ICKR_HSIRDY == 0); // ��� ������������ ����������� ����������
    CLK_CKDIVR = 0; // ������������� ������������ �������
    CLK_PCKENR1 = 0xff; // �������� ��� ���������
    CLK_PCKENR2 = 0xff; // ���� �����
    CLK_CCOR = 0; // ��������� CCO.
    CLK_HSITRIMR = 0; // Turn off any HSIU trimming.
    CLK_SWIMCCR = 0; // Set SWIM to run at clock / 2.
    CLK_SWR = 0xe1; // ���������� HSI � �������� ��������� ��������
    CLK_SWCR = 0; // ����� ����� ������������ �����������
    CLK_SWCR_SWEN = 1; // �������� ������������ �� HSI
    while (CLK_SWCR_SWBSY != 0); // �����, ���� ��������� ������������
    
  
  //�������������� GPIO

  PC_DDR_bit.DDR7=1;        //PC7 - �� �����  ����������� �������
  PC_CR1_bit.C17=1;         //PC7 - ����������� �����
  PC_CR2_bit.C27=0;         //PC7 - ����������� �������� ���������
  
  PC_ODR_bit.ODR7=1; // ������� IRF740
    
  PA_DDR_bit.DDR3=1;        //PA3 - �� �����
  PA_CR1_bit.C13=1;         //PA3 - ����������� �����
  PA_CR2_bit.C23=0;         //PA3 - ����������� �������� ���������
 
  PC_DDR_bit.DDR6=0;        //PC6 - �� ����
  PC_CR1_bit.C16=0;         //PC6 - ��������� ����
  PC_CR2_bit.C26=0;         //PC6 - ������� ���������� ���������
  
  //������������ ���������
  PD_DDR_bit.DDR1=1;        //PD1 - �� �����
  PD_CR1_bit.C11=1;         //PD1 - ����������� �����
  PD_CR2_bit.C21=0;         //PD1 - ����������� �������� ���������
  
  PD_DDR_bit.DDR2=1;        //PD2 - �� �����
  PD_CR1_bit.C12=1;         //PD2 - ����������� �����
  PD_CR2_bit.C22=0;         //PD2 - ����������� �������� ���������
  
  PD_DDR_bit.DDR3=1;        //PD3 - �� �����
  PD_CR1_bit.C13=1;         //PD3 - ����������� �����
  PD_CR2_bit.C23=0;         //PD3 - ����������� �������� ���������
  
  PD_DDR_bit.DDR4=1;        //PD4 - �� �����
  PD_CR1_bit.C14=1;         //PD4 - ����������� �����
  PD_CR2_bit.C24=0;         //PD4 - ����������� �������� ���������
  
  PD_DDR_bit.DDR5=1;        //PD5 - �� �����
  PD_CR1_bit.C15=1;         //PD5 - ����������� �����
  PD_CR2_bit.C25=0;         //PD5 - ����������� �������� ���������
  
  PD_DDR_bit.DDR6=1;        //PD6 - �� �����
  PD_CR1_bit.C16=1;         //PD6 - ����������� �����
  PD_CR2_bit.C26=0;         //PD6 - ����������� �������� ���������
  
  PA_DDR_bit.DDR1=1;        //PA1 - �� �����
  PA_CR1_bit.C11=1;         //PA1 - ����������� �����
  PA_CR2_bit.C21=0;         //PA1 - ����������� �������� ���������
  
  PA_DDR_bit.DDR2=1;        //PA2 - �� �����
  PA_CR1_bit.C12=1;         //PA2 - ����������� �����
  PA_CR2_bit.C22=0;         //PA2 - ����������� �������� ���������
  
  

  //�������������� TIM2
      TIM2_PSCR = 0x07;	 // ��������= 2^7=128.
      TIM2_ARRH = 0x0;	 // ������� ���� �����
      TIM2_ARRL = 0xff;	 // ������� ���� �����
      TIM2_CCR3H = 0x0;	 // ������� ���� �����
      TIM2_CCR3L = 0x80;	 // ������� ���� �����
      TIM2_CCER2=0x03;
      TIM2_CCMR3_OC3M = 6; // ����� ��� -PWM Mode 1
      TIM2_CR1_CEN = 1;	 // ���������� ������

  TIM1_PSCRH = 0;
  TIM1_PSCRL = 0x00;
  TIM1_IER = 0x07;
  TIM1_CCMR1 = 0x01;
  TIM1_CCMR2 = 0x02;
  TIM1_CCER1 = 0x20;
  TIM1_SMCR = 0x54;
  
  
  
 //uart_init(9600, 16000000);

  __enable_interrupt();     //��������� ����������
  
  TIM1_CCER1 |= 0x11;
  TIM1_CR1_CEN = 1; // CEN
  
  delay_ms(100);
/*  
  setup_adc();
  
  adc_bat=update_adc();
  delay_ms(10);
  adc_bat=update_adc();
  
  if(adc_bat>700){TIM2_CCR3L = 0x80; delay_ms(1000); TIM2_CCR3L = 0x00;};

  delay_ms(500);
 */ 
       while(1)
  {
    
   PC_ODR_bit.ODR7=0; // ������� IRF740
   delay_us(150);
   PC_ODR_bit.ODR7=1; // ������� IRF740
   delay_ms(15);
   if(with<200){TIM2_PSCR = 0x00; TIM2_CCR3L = 0;}else{TIM2_CCR3L = 0x80;};
   
   if(with>200){TIM2_PSCR = 0x08; PD_ODR_bit.ODR1=1;}else{PD_ODR_bit.ODR1=0;};
   if(with>225){TIM2_PSCR = 0x07; PD_ODR_bit.ODR2=1;}else{PD_ODR_bit.ODR2=0;};
   if(with>250){TIM2_PSCR = 0x06; PD_ODR_bit.ODR3=1;}else{PD_ODR_bit.ODR3=0;};
   if(with>300){TIM2_PSCR = 0x05; PD_ODR_bit.ODR6=1;}else{PD_ODR_bit.ODR6=0;};
   if(with>350){TIM2_PSCR = 0x04; PD_ODR_bit.ODR5=1;}else{PD_ODR_bit.ODR5=0;};
   if(with>400){TIM2_PSCR = 0x03; PD_ODR_bit.ODR4=1;}else{PD_ODR_bit.ODR4=0;};
   if(with>450){TIM2_PSCR = 0x03; PA_ODR_bit.ODR1=1;}else{PA_ODR_bit.ODR1=0;};
   if(with>500){TIM2_PSCR = 0x03; PA_ODR_bit.ODR2=1;}else{PA_ODR_bit.ODR2=0;};
   

    
    
  }
     
     
     
     
  

}



