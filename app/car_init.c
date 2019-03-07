#include "car_init.h"
 
void Init_All()
{
  Init_Setpara();
  Init_Paranum();       
  Init_Key();           
  OLED_Init(1);          //OLED��Ļ��ʼ��
  Init_Motor();
  Init_Steer();
  Init_ADC();
  Init_PIT();           //�����жϳ�ʼ��
  Init_I2C();           //MPU6050��ʼ��
  Init_Flash();         //FLASH������ȡ��ʼ��
  Init_Sdhc();          //SD��ģ���ʼ��
  Init_All_Pulse_Counter();//��������ʼ��
  Init_Uart_Interr();
 // Init_Temporary();
}
 
void Init_Temporary()
{
 // mycar.SpeedError              =          0;
}

void Init_Setpara()    //�ɵ�������ʼֵ
{
    track.TrackAmpDyc = 600;
    track.AmpK = 600; 
    setpara.Ampk = 600;
    
  mycar.Status                 =            0;
  
  setpara.Steer.mid             =         895;
  setpara.Steer.maxturn         =         182;
  
  setpara.SpeedDutyMax          =     6000;
  setpara.diff_slope            =     1000;
  
  setpara.SteerPD.Kp              =         110;  
  setpara.SteerPD.Kd              =         200;
  setpara.SteerPD.Kpchange        =          0;
  
  setpara.SpeedPID.Kp           =         38;
  setpara.SpeedPID.Ki           =         13;
  setpara.SpeedPID.Kd           =         11;
  
  setpara.LStraightMax          =        250;
  setpara.LStraightMin          =        200;
  
  setpara.RightSpeedMax         =        230;
  setpara.RightSpeedMin         =        210;
  
  setpara.RightThreshold        =        500;  
  
  setpara.StraightMax           =         240;
  setpara.StraightMin           =         220;
  
  setpara.KFar                  =         95;
  setpara.KNear                 =         50; 
  
  setpara.FarCal[0]	            =       1000;
  setpara.FarCal[1]	            =        950;
  setpara.FarCal[2]	            =        880;
  setpara.FarCal[3]	            =	 790;
  setpara.FarCal[4]	            =	 700;
  setpara.FarCal[5]	            =	 550;
  setpara.FarCal[6]	            =	 440;
  setpara.FarCal[7]	            =	 250;
  setpara.FarCal[8]	            =	 120;
  setpara.FarCal[9]	            =	  40;
  setpara.FarCal[10]         	=      0;
  setpara.FarCal[11]     	    =       0;
  setpara.WeightNear[0]	        =	   0;
  setpara.WeightNear[1]	        =	   0;
  setpara.WeightNear[2]	        =	 140;
  setpara.WeightNear[3]	        =	 360;
  setpara.WeightNear[4] 	    =	 540;
  setpara.WeightNear[5] 	    =	 670;
  setpara.WeightNear[6] 	    =	 780;
  setpara.WeightNear[7] 	    =	 880;
  setpara.WeightNear[8]	        =	 950;
  setpara.WeightNear[9]	        =	 990;
  setpara.WeightNear[10]	    =	1000;
  setpara.WeightNear[11]	    =	1000;
  setpara.WeightFar[0]  	    =	1100;
  setpara.WeightFar[1]  	    =	 960;
  setpara.WeightFar[2]  	    =	 860;
  setpara.WeightFar[3]  	    =	 750;
  setpara.WeightFar[4] 	        =	 620;
  setpara.WeightFar[5] 	        =	 560;
  setpara.WeightFar[6] 	        =	 440;
  setpara.WeightFar[7]  	    =	 380;
  setpara.WeightFar[8]  	    =	 250;
  setpara.WeightFar[9] 	        =	  80;
  setpara.WeightFar[10]	        =	   0;
  setpara.WeightFar[11] 	    =	   0;
  
  setpara.LoseThreshold         =       400;
  setpara.Status   =    1;
  //status = 0;
  
  
  
  mycar.Reed                    =      0;
}

void Init_Paranum()   //�ɵ�������������
{
  while(paralist[oled.ParaNum].precision)
    oled.ParaNum++;
  
  oled.ShowPageMax = 2;
  oled.ShowPageMin = 0;
}

void Init_Key()      //��ť�����ֿ��صȰ�����ʼ��
{
  GPIO_InitTypeDef gpio_init_struct={0};
  
  gpio_init_struct.GPIO_PTx = PTC;              //PORTC
  gpio_init_struct.GPIO_Pins = GPIO_Pin2|GPIO_Pin3|GPIO_Pin4|GPIO_Pin5|GPIO_Pin6|GPIO_Pin7|GPIO_Pin8;     //����3��4��5��6��7��8
  gpio_init_struct.GPIO_Dir = DIR_INPUT;        //����
  gpio_init_struct.GPIO_PinControl = INPUT_PF_EN|IRQC_FA;   //�����ͨ�˲����½����ж�
  gpio_init_struct.GPIO_Isr = Portc_Isr;        //�жϺ���
  LPLD_GPIO_Init(gpio_init_struct);
  //ʹ���ж�
  LPLD_GPIO_EnableIrq(gpio_init_struct);
}

void Init_Motor()      //�����ʼ��   
{
  FTM_InitTypeDef ftm0_init_struct={0};
  
  ftm0_init_struct.FTM_Ftmx = FTM0;	        //ʹ��FTM0ͨ��
  ftm0_init_struct.FTM_Mode = FTM_MODE_PWM;	//ʹ��PWMģʽ
  ftm0_init_struct.FTM_PwmFreq = 15000;	    //PWMƵ��15000Hz
  
  LPLD_FTM_Init(ftm0_init_struct);
  
  /*****************************************************
  *------------    --------------- -PWM���뷽ʽ
  *--------------- --------------�˿ں�  ��
  *----------------------��ʼռ�ձȡ�    ��
  *--------------------ͨ����  ��  ��    ��
  *-----------------ģ��š�   ��  ��    ��
  *                   ��  ��   ��  ��    ��       */
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch4,0,PTD4,ALIGN_LEFT);
  LPLD_FTM_PWM_Enable(FTM0,FTM_Ch5,0,PTD5,ALIGN_LEFT);
}

void Init_Steer()
{
  FTM_InitTypeDef ftm_init_struct={0};
  
  ftm_init_struct.FTM_Ftmx = FTM2;	        //ʹ��FTM2ͨ��
  ftm_init_struct.FTM_Mode = FTM_MODE_PWM;	//ʹ��PWMģʽ
  ftm_init_struct.FTM_PwmFreq = 50;	        //PWMƵ��50Hz
  
  LPLD_FTM_Init(ftm_init_struct);
  
  LPLD_FTM_PWM_Enable(FTM2,FTM_Ch0,150,PTB18,ALIGN_LEFT);
}

void Init_ADC()
{
  ADC_InitTypeDef adc_init_struct={0};
  
  adc_init_struct.ADC_Adcx = ADC0;                      //����ADC��������
  adc_init_struct.ADC_BitMode = SE_10BIT;               //����8λ����
  adc_init_struct.ADC_SampleTimeCfg = SAMTIME_SHORT;    //������ʱ��
  adc_init_struct.ADC_HwAvgSel = HW_32AVG;               //32��Ӳ��ƽ��
  adc_init_struct.ADC_CalEnable = TRUE;                 //ʹ�ܳ�ʼ��У��
  
  LPLD_ADC_Init(adc_init_struct);                       //��ʼ��ADC

  adc_init_struct.ADC_Adcx = ADC1;

  //��ʼ��ADC1
  LPLD_ADC_Init(adc_init_struct);
  //ʹ����ӦADCͨ��
  LPLD_ADC_Chn_Enable(ADC0, AD23);      //ʹ����ӦADCͨ��_��ص�ѹ

  LPLD_ADC_Chn_Enable(ADC0, AD8);       //PTB0
  LPLD_ADC_Chn_Enable(ADC0, AD9);       //PTB1
  LPLD_ADC_Chn_Enable(ADC0, AD12);      //PTB2
  LPLD_ADC_Chn_Enable(ADC0, AD13);      //PTB3
  LPLD_ADC_Chn_Enable(ADC0, AD14);      //PTC0
  LPLD_ADC_Chn_Enable(ADC0, AD15);      //PTC1
  LPLD_ADC_Chn_Enable(ADC1, AD14);      //PTB10
  LPLD_ADC_Chn_Enable(ADC1, AD15);      //PTB11
  
}

void Init_PIT()         //�����жϳ�ʼ��
{
  PIT_InitTypeDef pit0_init_struct={PIT0};
  //����PIT0����
  pit0_init_struct.PIT_Pitx = PIT0;
  pit0_init_struct.PIT_PeriodMs = PIT0_MS;     //��ʱ����
  pit0_init_struct.PIT_Isr = Pit0_Isr;  //�����жϺ���
  //��ʼ��PIT0
  LPLD_PIT_Init(pit0_init_struct);  
  
  //ʹ��PIT0
  LPLD_PIT_EnableIrq(pit0_init_struct);
}


void Init_I2C()
{
  uint8 device_id = MPU6050_Init();
  //ͨ���豸ID�ж��豸�Ƿ�ΪMPU6050
  if(device_id == MPU6050_ID)
  {
    printf("MPU6050��ʼ���ɹ�!\r\n");
    printf("�豸ID: 0x%X\r\n", device_id);
  }
  else
  {
    printf("MPU6050��ʼ��ʧ��!\r\n");
    printf("�豸ID: 0x%X\r\n", device_id);
    //while(1);
  }
}

void Init_Flash()
{
  LPLD_Flash_Init();
}

void Init_Sdhc()
{
  int i = disk_initialize(0);
  if(i==0) printf("SD����ʼ���ɹ�.\n");
  else printf("SD����ʼ��ʧ��.\n");
}

//���������ٳ�ʼ��
void Init_All_Pulse_Counter()
{
  Init_Pulse_Counter(PTC17,DMA_CH0,PTE6);
}
void Init_Pulse_Counter(PortPinsEnum_Type counter_port,
                        uint8 dma_chx,
                        PortPinsEnum_Type dir_port)
{
  GPIO_InitTypeDef gpio_init_struct={0};
  uint32 PTx_BASE = PTA_BASE+(counter_port>>5)*0x40u;
  gpio_init_struct.GPIO_PTx = (GPIO_Type *)PTx_BASE;
  gpio_init_struct.GPIO_Pins = 1<<(uint8)(counter_port&0x1Fu);
  gpio_init_struct.GPIO_Dir = DIR_INPUT;
  gpio_init_struct.GPIO_PinControl = IRQC_DMARI | INPUT_PF_EN;
  LPLD_GPIO_Init(gpio_init_struct);
  
  DMA_InitTypeDef dma_init_struct={0};                            //DMA��������
  dma_init_struct.DMA_CHx = dma_chx;                              //ͨ����
  dma_init_struct.DMA_Req = PORTA_DMAREQ+(counter_port>>5);       //DMA����Դ
  dma_init_struct.DMA_MajorLoopCnt = 0x7FFF;                      //��ѭ������ֵ
  dma_init_struct.DMA_MinorByteCnt = 1;                           //��ѭ���ֽڼ���
  dma_init_struct.DMA_SourceAddr = 0x4004000C;                    //Դ��ַ
  dma_init_struct.DMA_DestAddr = 0x4004000C;                      //Ŀ�ĵ�ַ
  LPLD_DMA_Init(dma_init_struct);                                 //��ʼ��DMA
  LPLD_DMA_EnableReq(dma_chx);                                    //ʹ��DMA����
  GPIO(dir_port,DIR_INPUT);
}

void Init_Uart_Interr()
{
  UART_InitTypeDef term_port_structure={0};
  term_port_structure.UART_Uartx = TERM_PORT;
  term_port_structure.UART_BaudRate = TERMINAL_BAUD;
  term_port_structure.UART_RxPin = PTB16;
  term_port_structure.UART_TxPin = PTB17;
  term_port_structure.UART_RxIntEnable = TRUE;    //ʹ�ܽ����ж�
  term_port_structure.UART_RxIsr = Uart_Isr;      //���ý����жϺ���
  LPLD_UART_Init(term_port_structure);
  LPLD_UART_EnableIrq(term_port_structure);    //Enable uart interrupt
}


