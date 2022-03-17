
/*
*项目需求：使用按键控制 4 只 LED 循环闪烁
*项目编写时间：2022年3月17日
*项目编写人：李俊辉
*/

#include "ioCC2530.h"

#define LED1 P1_0
#define LED2 P1_1
#define LED3 P1_3
#define LED4 P1_4
#define SW1 P1_2

#define TRUE 1

unsigned char count = 0;        //用于计算按键按下次数

/*
*函数名称：Initial_GPIO()
*函数功能：初始化GPIO
*函数入口：void Initial_GPIO()
*函数形参：无
*函数返回值：无
*/
void Initial_GPIO()
{
  P1SEL &= ~0x1F;               //设置P1_0、P1_1、P1_2、P1_3、P1_4为GPIO
  P1DIR |= 0x1B;                 //设置P1_0、P1_1、P1_3、P1_4为输出端口
  P1DIR &= ~0x04;                //设置P1_2为输入端口
  P1INP &= ~0x04;               //设置P1_2为上拉/下拉模式
  P2INP &= ~0x40;               //设置P1端口为上拉模式
  P1 = 0x00;                     //关闭LED灯
}

/*
*函数名称：Initial_Interrupt()
*函数功能：初始化端口中断
*函数入口：void Initial_Interrupt()
*函数形参：无
*函数返回值：无
*/
void Initial_Interrupt()
{
  IEN2 |= 0x10;                 //使能端口1中断
  P1IEN |= 0x04;                //端口P1_2中断使能
  PICTL |= 0x02;                //端口P1_2下降沿触发
  EA = 1;                        //全局中断使能
}

/*
*函数名称：P1_ISR()
*函数功能：中断服务程序
*函数入口：void P1_ISR(void)
*函数形参：无
*函数返回值：无
*/
#pragma vector = P1INP_VECTOR
__interrupt void P1_ISR(void)
{
  if(P1IFG == 0x04)
  {
    count++;
    switch(count)
    {
    case 1:LED1 = 1;break;         //点亮LED1
    case 2:LED1 = 0;LED2 = 1;break;//先关闭LED1，在点亮LED2
    case 3:LED2 = 0;LED3 = 1;break;//先关闭LED2，再点亮LED3
    case 4:LED3 = 0;LED4 = 1;break;//先关闭LED3，再点亮LED4
    default:
      P1 = 0x00;                    //关闭所有LED灯
      count = 0x00;                 //清零程序计数器
      break;
    }
  }
  P1IF = 0x00;                       //清P1端口中断标志位
  P1IFG = 0x00;                      //清P1_2中断标志位
}

/*
*函数名称：main()
*函数功能：主函数，程序入口
*函数入口：void main(void)
*函数形参：无
*函数返回值：无
*/
void main(void)
{
  Initial_GPIO();                    //调用初始化 GPIO 函数
  Initial_Interrupt();                //调用初始化中断函数
  while(TRUE);
}