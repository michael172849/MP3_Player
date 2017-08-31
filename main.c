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
	        P4OUT |= BIT1; // Set P2.6 as pull�\Up resistance
	        P4IES |= BIT1; // P4.0 Hi/Lo edge
	        P4IFG &= ~BIT1; // P4.0 IFG cleared
	        P4IE |= BIT1; // P P4.0 interrupt enabled
}
void scomm_ini()
{
	P3DIR |= BIT4; //����TS3A24157оƬ��6638��UCA1���ӵ�USB����Ĵ���
		P3OUT &= ~BIT4;
		P8SEL |= 0x0c; //ģ�鹦�ܽӿ����ã���P8.2��P8.3��ΪUSCI�Ľ��տ��뷢���
		UCA1CTL1|=UCSWRST; //��λUSCI
		UCA1CTL1|=UCSSEL_1; //���ø���ʱ�ӣ����ڷ����ض�������
		UCA1BR0=0x03; //���ò�����
		UCA1BR1=0x00;
		UCA1MCTL=UCBRS_3+UCBRF_0;
		UCA1CTL1&=~UCSWRST; //������λ
		UCA1IE|=UCRXIE; //ʹ�ܽ����ж�
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
  UCSCTL6 &= ~XT1OFF; //����XT1
  P7SEL |= BIT2 + BIT3; //XT2���Ź���ѡ��
  UCSCTL6 &= ~XT2OFF; //����XT2
  while (SFRIFG1 & OFIFG) { //�ȴ�XT1��XT2��DCO�ȶ�
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG); 
    SFRIFG1 &= ~OFIFG; 
  }
  
  UCSCTL4 = SELA__XT1CLK + SELS__XT2CLK + SELM__XT2CLK; //����DCO�������ܷ�
  
  UCSCTL1 = DCORSEL_5; //6000kHz~23.7MHz
  UCSCTL2 = 20000000 / (4000000 / 16); //XT2Ƶ�ʽϸߣ���Ƶ����Ϊ��׼�ɻ�ø��ߵľ���
  UCSCTL3 = SELREF__XT2CLK + FLLREFDIV__16; //XT2����16��Ƶ����Ϊ��׼
  while (SFRIFG1 & OFIFG) { //�ȴ�XT1��XT2��DCO�ȶ�
    UCSCTL7 &= ~(DCOFFG+XT1LFOFFG+XT2OFFG); 
    SFRIFG1 &= ~OFIFG; 
  }
  UCSCTL5 = DIVA__1 + DIVS__1 + DIVM__1; //�趨����CLK�ķ�Ƶ
  UCSCTL4 = SELA__XT1CLK + SELS__DCOCLK + SELM__DCOCLK; //�趨����CLK��ʱ��Դ
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
//curVolume = 50; //��ʼ����

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
 // usbmsc_IdleTask(); //����U�̿���ʱ����
                     //���ڱ�����������������Ĵ��ڣ�����Ƶ�ʿ��Ա�֤�������
//}

void ini_myaudio()
{
	DAC12_0CTL0 = DAC12IR + DAC12SREF_0 + DAC12AMP_5 +DAC12ENC + DAC12CALON+DAC12OPS;
	DAC12_0CTL1 &= ~DAC12OG;
	  	P5DIR=BIT6;//�����������˷�
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
  //����IO��ʼ��
  P4REN |= 0x1F; //ʹ�ܰ����˿��ϵ�����������
  P4OUT |= 0x1F; //����״̬
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
				audio_DecoderReset(); //��λ������
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
					continue; //����ļ���չ��

				f_open(&fil, fno.fname, FA_READ); //���ļ�


				sprintf(display, "Play SD music :%-30s", fno.fname);
				etft_DisplayString(display, 0, 0, 0, 65535);
				int flag = 0; //������Ϣ�Ƿ��Ѿ���ʾ��
				lastSec = 0xFFFF;

				for (;;)
				{
					if (state==0)
					{
						audio_DecoderReset(); //��λ������
						f_close(&fil); //�ر��ļ�
						break;
					}
					wavsize = queue_CanWrite(&wav_stream); //�����Ƶ�������л��ж��ٿռ�
					size = FBUF_SIZE < wavsize ? FBUF_SIZE : wavsize; //ȡ��Ƶ���������ļ���ȡ������ʣ��ռ��н�С��

					if(wavsize > FBUF_SIZE) //���и��գ�SD���ʲ�������ʱ��ִ����������ʱ����
					{
						P4OUT |= BIT5; //LED5
						P4OUT &= ~BIT6;
					}
					else //SD�����㹻
					{
						P4OUT &= ~BIT5;
						P4OUT |= BIT6; //LED4
					}

					if(size > 0)
					{
						rc = f_read(&fil, fbuf, size, &size);
						if (rc || !size) break;
						queue_Write(&wav_stream, (uint8_t*)fbuf, size); // �����������ݷ�����Ƶ������
					}

					if(wavsize <= FBUF_SIZE) //���������ݳ��㣬�пմ�����ʾ
					{
						const AudioDecoderStatus *ads = audio_DecoderStatus(); //��ȡ������״̬
						if(ads->wFlags & ADI_INVALID_FILE) //���ļ��д���������������
							break;

						if(ads->wFlags & ADI_PLAYING) //���������У����Զ����ļ�����״̬
						{
							if(lastSec != ads->wCurPosSec) //������ֵ�и��£���Ҫˢ����ʾ
							{
								sprintf(display, "%02d:%02d/%02d:%02d", ads->wCurPosSec / 60
										, ads->wCurPosSec % 60, ads->wFullSec / 60, ads->wFullSec % 60);
								etft_DisplayString(display, 0, 16, 0, 65535); //��ʾ��ǰ����
								lastSec = ads->wCurPosSec;



							}
						}
					}
				}

				audio_DecoderReset(); //��λ������
				f_close(&fil); //�ر��ļ�

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
        		 initQueue(&wav_stream, buffer, BUFFER_SIZE); //��ʼ�����ֲ���ʹ�õ���Ƶ������
        		 audio_SetWavStream(&wav_stream); //�趨����ģ���ָ���������л�ȡ��Ҫ���������
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
