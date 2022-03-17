
/*
*��Ŀ����ʹ�ð������� 4 ֻ LED ѭ����˸
*��Ŀ��дʱ�䣺2022��3��17��
*��Ŀ��д�ˣ����
*/

#include "ioCC2530.h"

#define LED1 P1_0
#define LED2 P1_1
#define LED3 P1_3
#define LED4 P1_4
#define SW1 P1_2

#define TRUE 1

unsigned char count = 0;        //���ڼ��㰴�����´���

/*
*�������ƣ�Initial_GPIO()
*�������ܣ���ʼ��GPIO
*������ڣ�void Initial_GPIO()
*�����βΣ���
*��������ֵ����
*/
void Initial_GPIO()
{
  P1SEL &= ~0x1F;               //����P1_0��P1_1��P1_2��P1_3��P1_4ΪGPIO
  P1DIR |= 0x1B;                 //����P1_0��P1_1��P1_3��P1_4Ϊ����˿�
  P1DIR &= ~0x04;                //����P1_2Ϊ����˿�
  P1INP &= ~0x04;               //����P1_2Ϊ����/����ģʽ
  P2INP &= ~0x40;               //����P1�˿�Ϊ����ģʽ
  P1 = 0x00;                     //�ر�LED��
}

/*
*�������ƣ�Initial_Interrupt()
*�������ܣ���ʼ���˿��ж�
*������ڣ�void Initial_Interrupt()
*�����βΣ���
*��������ֵ����
*/
void Initial_Interrupt()
{
  IEN2 |= 0x10;                 //ʹ�ܶ˿�1�ж�
  P1IEN |= 0x04;                //�˿�P1_2�ж�ʹ��
  PICTL |= 0x02;                //�˿�P1_2�½��ش���
  EA = 1;                        //ȫ���ж�ʹ��
}

/*
*�������ƣ�P1_ISR()
*�������ܣ��жϷ������
*������ڣ�void P1_ISR(void)
*�����βΣ���
*��������ֵ����
*/
#pragma vector = P1INP_VECTOR
__interrupt void P1_ISR(void)
{
  if(P1IFG == 0x04)
  {
    count++;
    switch(count)
    {
    case 1:LED1 = 1;break;         //����LED1
    case 2:LED1 = 0;LED2 = 1;break;//�ȹر�LED1���ڵ���LED2
    case 3:LED2 = 0;LED3 = 1;break;//�ȹر�LED2���ٵ���LED3
    case 4:LED3 = 0;LED4 = 1;break;//�ȹر�LED3���ٵ���LED4
    default:
      P1 = 0x00;                    //�ر�����LED��
      count = 0x00;                 //������������
      break;
    }
  }
  P1IF = 0x00;                       //��P1�˿��жϱ�־λ
  P1IFG = 0x00;                      //��P1_2�жϱ�־λ
}

/*
*�������ƣ�main()
*�������ܣ����������������
*������ڣ�void main(void)
*�����βΣ���
*��������ֵ����
*/
void main(void)
{
  Initial_GPIO();                    //���ó�ʼ�� GPIO ����
  Initial_Interrupt();                //���ó�ʼ���жϺ���
  while(TRUE);
}