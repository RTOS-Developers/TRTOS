#include <..\USER\Prj_TP502T\DS18B20_Driver.h>

uint16  DS1_PIN=GPIO_Pin_9;
GPIO_TypeDef *DS1_GROUP=GPIOB;

void DS_SetPort(GPIO_TypeDef *DS1_Group,uint16  DS1_Port)
{
	DS1_GROUP=DS1_Group;
	DS1_PIN=DS1_Port
}
void DS_DelayUs(uint16 D)
{
	uint16 i=0;
	while(D--)
	{
		i=7;//5~8都可以
		while(i--);
	}
}
void DS_DelayMs(uint16 n)
{
	uint16 i=0;
	while(n--)
	{
		i=12000;
		while(i--);
	}
}
void DS_PortIN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DS1_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//浮动输入
	GPIO_Init(DS1_GROUP,&GPIO_InitStructure);
}
void DS_PortOUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DS1_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_Init(DS1_GROUP,&GPIO_InitStructure);
}
void DS_Rst(void)	   
{                 
	DS_PortOUT(); //SET PA0 OUTPUT
	GPIO_ResetBits(DS1_GROUP,DS1_PIN);
  DS_DelayUs(750);    //拉低750us
	GPIO_SetBits(DS1_GROUP,DS1_PIN);
	DS_DelayUs(15);     //15US
}
u8 DS_Check(void) 	   
{   
	u8 retry=0;
	DS_PortIN();//SET PA0 INPUT	 
    while (GPIO_ReadInputDataBit(DS1_GROUP,DS1_PIN)&&retry</*200*/200)
	{
		retry++;
		DS_DelayUs(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!GPIO_ReadInputDataBit(DS1_GROUP,DS1_PIN)&&retry<240)
	{
		retry++;
		DS_DelayUs(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}
u8 DS_Read_Bit(void) 			 // read one bit
{
    u8 data;
	DS_PortOUT();
  GPIO_ResetBits(DS1_GROUP,DS1_PIN);
	DS_DelayUs(2);
 	GPIO_SetBits(DS1_GROUP,DS1_PIN);
	DS_PortIN();
	DS_DelayUs(12);
	if(GPIO_ReadInputDataBit(DS1_GROUP,DS1_PIN))data=1;
  else data=0;	 
  DS_DelayUs(50);           
  return data;
}
u8 DS_Read_Byte(void)    // read one byte
{        
    u8 i,j,dat;
    dat=0;
	DIS_INT//麻痹。关我中断才可以，以后不会用你了
	for (i=1;i<=8;i++) 
	{
        j=DS_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
	EN_INT
    return dat;
}
void DS_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	 DS_PortOUT();//SET PA0 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            GPIO_ResetBits(DS1_GROUP,DS1_PIN);
            DS_DelayUs(2);                            
            GPIO_SetBits(DS1_GROUP,DS1_PIN);
            DS_DelayUs(60);             
        }
        else 
        {
            GPIO_ResetBits(DS1_GROUP,DS1_PIN);
            DS_DelayUs(60);             
            GPIO_SetBits(DS1_GROUP,DS1_PIN);
            DS_DelayUs(2);                          
        }
    }
}
 //开始温度转换
void DS_Start(void)// ds1820 start convert
{   						               
    DS_Rst();	   
	  DS_Check();	 
    DS_Write_Byte(0xcc);// skip rom
    DS_Write_Byte(0x44);// convert
} 
u8 DS18B20_Init(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PORTG口时钟 
 	GPIO_InitStructure.GPIO_Pin = DS1_PIN;				//PORTG.11 推挽输出
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(DS1_GROUP, &GPIO_InitStructure);
 	GPIO_SetBits(DS1_GROUP,DS1_PIN);    //输出1
	DS_Rst();
	return DS_Check();
} 
uint8 DS_Get_Temp(float *TP)
{
		uint8 c,Buf[9];
	  float T;

	  uint16 Temp;
    DS_Start ();                    // ds1820 start convert
    DS_Rst();
    c=DS_Check();	 
		if(c)return 1;
    DS_Write_Byte(0xcc);// skip rom
    DS_Write_Byte(0xbe);// convert	    
		for(c=0;c<9;c++)
	{
		Buf[c]=DS_Read_Byte(); 
	}
	c=Tools_GetCRC8(&Buf[0],8);
	if(c!=Buf[8])return 1;//校验不通过
    if(Buf[1]>7)
    {
        Buf[1]=~Buf[1];
        Buf[0]=~Buf[0]; 
				Temp=Buf[1];Temp<<=8;Temp|=Buf[0];
				T=0-(float)Temp;
			  
    }else 
		{	  	  
				Temp=Buf[1];Temp<<=8;Temp|=Buf[0];
				T=(float)Temp;
		}
		T*=0.625; 
    *TP=T/10;
		return 0;
} 
u8 Sensor_Init(void)
{
	uint8 Ds18B20CHOK=0x00;
	if(UIShowValue.SaveValue.WorkBit&WCB_DTH11)
	{
		DS1_PIN=GPIO_Pin_9;
		DS1_GROUP=GPIOB;
		if(!DHT11_Read(Null,Null))Ds18B20CHOK+=2;
	}else
	{
		DS1_PIN=GPIO_Pin_9;
		DS1_GROUP=GPIOB;
		if(!DS18B20_Init())Ds18B20CHOK++;
		DS1_PIN=GPIO_Pin_8;
		DS1_GROUP=GPIOB;
		if(!DS18B20_Init())Ds18B20CHOK++;

	}
	return Ds18B20CHOK;
} 
//其实可以实现单线混合读取，但是考虑到比较浪费CPU还是算了
void DS_Read_Value(float *Ch1,float *Ch2)
{
	uint8 u=0;
	static uint8 Buf[2];
	UIShowValue.RunFlag&=~(WRF_SensorCH1Connected|WRF_SensorCH2Connected);
	if(UIShowValue.SaveValue.WorkBit&WCB_DTH11)
	{
		DHT11_PIN=GPIO_Pin_9;
    DHT11_GROUP=GPIOB;
		if(DHT11_Read(Ch1,Ch2))
		{
			Buf[0]++;
			if(Buf[0]>5)UIShowValue.RunFlag&=~(WRF_SensorCH1Connected|WRF_SensorCH2Connected);
		}else {Buf[0]=0;UIShowValue.RunFlag|=(WRF_SensorCH1Connected|WRF_SensorCH2Connected);}
	}
	else
	{
		DS_SetPort(GPIOB,GPIO_Pin_9);
		u=DS_Get_Temp(Ch1);
		if(u)
		{
			Buf[0]++;
			if(Buf[0]>5)
			{
			DS18B20_Init();
			UIShowValue.RunFlag&=~WRF_SensorCH1Connected;
			}
		}else {Buf[0]=0;UIShowValue.RunFlag|=WRF_SensorCH1Connected;}
		DS_SetPort(GPIOB,GPIO_Pin_8);
		u=DS_Get_Temp(Ch2);
		if(u)
		{
			Buf[1]++;
			if(Buf[1]>5)
			{
			DS18B20_Init();
			UIShowValue.RunFlag&=~WRF_SensorCH2Connected;
			}
		}else {Buf[1]=0;UIShowValue.RunFlag|=WRF_SensorCH2Connected;}
	}
}
