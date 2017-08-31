#include <msp430.h>
//#include <msp430f6638.h>
#include "dr_sdcard.h"
#include "dr_audio.h"
#include "fw_queue.h"
#include "dr_tft.h"
#include <string.h>
#include <stdio.h>
//#include <stdint.h>
#include <math.h>

//////////////////////////////////////

int free=100;
int fre=102;
int sin_table[200];
int *sin_data_pr;
char rcc;
int timing=0,ttmp=0;
int j;
int state=0;
int playpoint;
void interrupt_key()
{
			P4DIR |= BIT5; // Set P4.5 to output direction
	        P4REN |= BIT1; // Enable P2.6 internal resistance
	        P4OUT |= BIT1; // Set P2.6 as pull‐Up resistance
	        P4IES |= BIT1; // P4.0 Hi/Lo edge
	        P4IFG &= ~BIT1; // P4.0 IFG cleared
	        P4IE |= BIT1; // P P4.0 interrupt enabled
}
void scomm_ini()
{
	P3DIR |= BIT4; //控制TS3A24157芯片，6638的UCA1连接到USB虚拟的串口
		P3OUT &= ~BIT4;
		P8SEL |= 0x0c; //模块功能接口设置，即P8.2与P8.3作为USCI的接收口与发射口
		UCA1CTL1|=UCSWRST; //复位USCI
		UCA1CTL1|=UCSSEL_1; //设置辅助时钟，用于发生特定波特率
		UCA1BR0=0x03; //设置波特率
		UCA1BR1=0x00;
		UCA1MCTL=UCBRS_3+UCBRF_0;
		UCA1CTL1&=~UCSWRST; //结束复位
		UCA1IE|=UCRXIE; //使能接收中断
}

void changefre()
{
	double fr=free;
		fr=fr/100;
		fr=2000/fr;
		TA0CCTL0 |= CCIE; // CCR0 interrupt enabled
		TA0CTL = TASSEL_2 + MC_1 + TACLR;// SMCLK, upmode, clearTAR
		TA0CCR0 = (int)fr;
}

void initClock()
{
	 P1DIR |= BIT2;                            // P1.1 output
		 P4DIR|=BIT5;
  while(BAKCTL & LOCKIO) // Unlock XT1 pins for operation
    BAKCTL &= ~(LOCKIO);
  UCSCTL6 &= ~XT1OFF; //启动XT1
  P7SEL |= BIT2 + BIT3; //XT2引脚功能选择
  UCSCTL6 &= ~XT2OFF; //启动XT2
  while (SFRIFG1 & OFIFG) { //等待XT1、XT2与DCO稳定
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG); 
    SFRIFG1 &= ~OFIFG; 
  }
  
  UCSCTL4 = SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK; //避免DCO调整中跑飞
  
  UCSCTL1 = DCORSEL_5; //6000kHz~23.7MHz
  UCSCTL2 = 20000000 / (4000000 / 16); //XT2频率较高，分频后作为基准可获得更高的精度
  UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__16; //XT2进行16分频后作为基准
  while (SFRIFG1 & OFIFG) { //等待XT1、XT2与DCO稳定
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG); 
    SFRIFG1 &= ~OFIFG; 
  }
  UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1; //设定几个CLK的分频
  UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK; //设定几个CLK的时钟源
}

#define BUFFER_SIZE 4096
uint8_t buffer[BUFFER_SIZE];

Queue wav_stream;

FRESULT rc;                                       /* Result code */
FATFS fatfs;                                      /* File system object */
DIRS dir;                                         /* Directory object */
FILINFO fno;                                      /* File information object */
FIL fil;                                          /* File object */
uint16_t wavsize;
uint16_t size;
uint16_t lastSec;
char display[41];
//curVolume = 50; //初始音量

#define FBUF_SIZE 256
char fbuf[FBUF_SIZE];

void strupr(char* str)
{
  char *ptr = str;
  while(!*ptr)
  {
    if(*ptr >= 'a' && *ptr <='z')
      *ptr -= 'a' - 'A';
    ptr++;
  }
}

int __low_level_init(void)
{
  WDTCTL = WDTPW + WDTHOLD ;//stop wdt
  return 1;
}

//void idleTasks()
//{
 // usbmsc_IdleTask(); //处理U盘空闲时任务
                     //对于本程序，由于其他代码的存在，调用频率可以保证不会过高
//}

void ini_myaudio()
{
	DAC12_0CTL0 = DAC12IR + DAC12SREF_0 + DAC12AMP_5 +DAC12ENC + DAC12CALON+DAC12OPS;
	DAC12_0CTL1 &= ~DAC12OG;
	  	P5DIR=BIT6;//打开扬声器的运放
	  	P5OUT=~BIT6;

	  	TA0CCTL0 |= CCIE; // CCR0 interrupt enabled
	  	TA0CCR0 =2000;				//100Hz
	  	TA0CTL = TASSEL_2 + MC_1 + TACLR;// SMCLK, upmode, clearTAR
}
int main()
{
  // Stop watchdog timer to prevent time out reset
 WDTCTL = WDTPW + WDTHOLD;

  initClock();


  //
  scomm_ini();
 //wave_ini();
  //initAudio();
  ini_myaudio();
  etft_AreaSet(0,0,319,239,65535);
  _DINT();
  //按键IO初始化
  P4REN |= 0x1F; //使能按键端口上的上下拉电阻
  P4OUT |= 0x1F; //上拉状态
  uint8_t last_btn = 0x1F, cur_btn, temp;
  P4DIR |= BIT5 + BIT6;
  P4OUT &= ~(BIT5 + BIT6);
  
  uint8_t volumecontrol=50;
  	double i=0;
  	for(j=0;j<51;j++)
  	{

  		sin_table[j]=2000;
  		sin_table[102-j+1]=0;
  	}
  	sin_data_pr=&sin_table[0];
  	interrupt_key();

  	_EINT();

  	initTFT();

  	etft_AreaSet(0,0,319,239,65535);
  	etft_DisplayString("Play your music", 20, 48, 0, 65535);
  	state=0;
  for(;;)
  {
	if (state==0) {
		//etft_DisplayString("time enter", 0, 80, 0, 65535);
		__bis_SR_register(CPUOFF + GIE); // Enter LPM0
		//etft_DisplayString("time out", 0, 80, 0, 65535);
			if (state==1) {
				continue;
			}
				DAC12_0DAT=*sin_data_pr++;
				//audio_SetOutput(*sin_data_pr++);
				if (sin_data_pr >= &sin_table[fre])
				{
					sin_data_pr = &sin_table[0];
				}
				timing++;
				if (timing>50)
				{
					ttmp++;
					timing=0;
				}
				if ((ttmp>free))
				{
					TA0CCTL0&= ~CCIE; // CCR0 interrupt disabled
					//etft_DisplayString("tune over", 0, 32, 0, 65535);
					ttmp=0;
				}

	}
	if (state==1)
	{
		f_mount(0, &fatfs);
		rc = f_opendir(&dir, "");
		if(rc)
		{
			etft_DisplayString("ERROR: No SDCard detected!", 0, 0, 0, 65535);
		}

		for (;;)
		{
			if (state==0)
			{
				audio_DecoderReset(); //复位解码器
				break;
			}
			rc = f_readdir(&dir, &fno);                        // Read a directory item
			if (rc || !fno.fname[0]) break;                    // Error or end of dir
			if (fno.fattrib & AM_DIR)                          //this is a directory
			{

			}
			else                                               //this is a file
			{
				strcpy(fbuf, fno.fname);
				strupr(fbuf);

				if(strstr(fbuf, ".WAV") == 0)
					continue; //检查文件扩展名

				f_open(&fil, fno.fname, FA_READ); //打开文件


				sprintf(display, "Play SD music :%-30s", fno.fname);
				etft_DisplayString(display, 0, 0, 0, 65535);
				int flag = 0; //基本信息是否已经显示完
				lastSec = 0xFFFF;

				for (;;)
				{
					if (state==0)
					{
						audio_DecoderReset(); //复位解码器
						f_close(&fil); //关闭文件
						break;
					}
					wavsize = queue_CanWrite(&wav_stream); //检查音频缓冲区中还有多少空间
					size = FBUF_SIZE < wavsize ? FBUF_SIZE : wavsize; //取音频缓冲区和文件读取缓冲区剩余空间中较小者

					if(wavsize > FBUF_SIZE) //队列更空，SD速率不够，此时不执行其它空闲时任务
					{
						P4OUT |= BIT5; //LED5
						P4OUT &= ~BIT6;
					}
					else //SD速率足够
					{
						P4OUT &= ~BIT5;
						P4OUT |= BIT6; //LED4
					}

					if(size > 0)
					{
						rc = f_read(&fil, fbuf, size, &size);
						if (rc || !size) break;
						queue_Write(&wav_stream, (uint8_t*)fbuf, size); // 将读出的数据放入音频缓冲区
					}

					if(wavsize <= FBUF_SIZE) //缓冲区数据充足，有空处理显示
					{
						const AudioDecoderStatus *ads = audio_DecoderStatus(); //获取解码器状态
						if(ads->wFlags & ADI_INVALID_FILE) //此文件有错，跳出并结束播放
							break;

						if(ads->wFlags & ADI_PLAYING) //真正播放中，可以读出文件播放状态
						{
							if(lastSec != ads->wCurPosSec) //播放秒值有更新，需要刷新显示
							{
								sprintf(display, "%02d:%02d/%02d:%02d", ads->wCurPosSec / 60
										, ads->wCurPosSec % 60, ads->wFullSec / 60, ads->wFullSec % 60);
								etft_DisplayString(display, 0, 16, 0, 65535); //显示当前秒数
								lastSec = ads->wCurPosSec;



							}
						}
					}
				}

				audio_DecoderReset(); //复位解码器
				f_close(&fil); //关闭文件

			}
		}
	}
  }
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
	__bic_SR_register_on_exit(CPUOFF);

}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void)
{
	switch(__even_in_range(UCA1IV,4))
	{
	case 0:break; // Vector 0 - no interrupt
	case 2: // Vector 2 - RXIFG
		rcc = UCA1RXBUF;
		while (!(UCA1IFG&UCTXIFG));
		if (state==1)break;
		switch(rcc)
		{
		case 'a': free=523;break;
		case 's':free=587;break;
		case 'd':free=659;break;
		case 'f':free=698;break;
		case 'g':free=784;break;
		case 'h':free=880;break;
		case 'j':free=988;break;
		case 'k':free=1046;break;
		case 'l':free=1175;break;
		case 'A':free=262;break;
		case 'S':free=294;break;
		case 'D':free=330;break;
		case 'F':free=349;break;
		case 'G':free=392;break;
		case 'H':free=440;break;
		case 'J':free=494;break;
		case 'K':free=523;break;
		default: free=1;break;
		}
		ttmp=0;
		if (free!=1){
			changefre();
			etft_DisplayString("set tune", 0, 80, 0, 65535);

		}
		UCA1TXBUF = rcc;
		// TX -> RXed character
		break;
	case 4:break; // Vector 4 - TXIFG
	default: break;
	}
}
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
        __delay_cycles(200000);                                                // disappears shakes
        if(P4IFG & BIT1){
        	__bic_SR_register_on_exit(CPUOFF);
        	state=!state;
        	if (state==1){
        		P4OUT|=BIT5;
        		initAudio();
        		TA0CCTL0&= ~CCIE;
        		 initQueue(&wav_stream, buffer, BUFFER_SIZE); //初始化音乐播放使用的音频缓冲区
        		 audio_SetWavStream(&wav_stream); //设定解码模块从指定缓冲区中获取需要解码的数据
        		 initTFT();
        		 etft_AreaSet(0,0,319,239,65535);
        	}
        	if (state==0){/*
        		P4OUT&=~BIT5;
        		TA2CCTL0&= ~CCIE;
        		//TA0CCTL0|= CCIE;
        		ini_myaudio();
        		free=100;
        		ttmp=0;
        		initTFT();
        		etft_AreaSet(0,0,319,239,65535);
        		etft_DisplayString("0", 0, 16, 0, 65535);*/
        		WDTCTL=0;
        	}
        	P4IFG &= ~BIT1; // P4.0 IFG cleared
        }
}
