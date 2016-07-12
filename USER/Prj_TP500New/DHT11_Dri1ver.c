#include <..\USER\Prj_TP500New\DHT11_Driver.h>

uint16  DHT11_PIN=GPIO_Pin_9;
GPIO_TypeDef *DHT11_GROUP=GPIOB;

void DHT11_DelayUs(uint16 D)
{
	uint16 i=0;
	while(D--)
	{
		i=7;//5~8都可以
		while(i--);
	}
}
void DHT11_DelayMs(uint16 n)
{
	uint16 i=0;
	while(n--)
	{
		i=12000;
		while(i--);
	}
}
void DHT11_PortIN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	//浮动输入
	GPIO_Init(DHT11_GROUP,&GPIO_InitStructure);
}
void DHT11_PortOUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_Init(DHT11_GROUP,&GPIO_InitStructure);
}

void DHT11_Rst(void)	   
{                 
	DHT11_PortOUT();
  GPIO_ResetBits(DHT11_GROUP,DHT11_PIN);
  DHT11_DelayMs(10);    	//拉低至少18ms
  GPIO_SetBits(DHT11_GROUP,DHT11_PIN);
	DHT11_DelayUs(80);     	//主机拉高20~40us
}
uint8 DHT11_Check(void) 	   
{   
	uint8 retry=0;
	DHT11_PortIN();//SET INPUT	 
    while (GPIO_ReadInputDataBit(DHT11_GROUP,DHT11_PIN)&&retry<100)//DHT11会拉低40~80us    //跳过Tgo
	{
		retry++;
		DHT11_DelayUs(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!GPIO_ReadInputDataBit(DHT11_GROUP,DHT11_PIN)&&retry<100)//DHT11拉低后会再次拉高40~80us     //跳过Trel
	{
		retry++;
		DHT11_DelayUs(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}
uint8 DHT11_Read_Bit(void) 			 
{
 	uint8 retry=0;
	while(GPIO_ReadInputDataBit(DHT11_GROUP,DHT11_PIN)&&retry<100)//等待变为低电平  //跳过Treh
	{
		retry++;
		DHT11_DelayUs(1);
	}
	retry=0;
	while(!GPIO_ReadInputDataBit(DHT11_GROUP,DHT11_PIN)&&retry<100)//等待变高电平   //跳过TLOW
	{
		retry++;
		DHT11_DelayUs(1);
	}
	DHT11_DelayUs(/*40*/35);//等待40us
	if(GPIO_ReadInputDataBit(DHT11_GROUP,DHT11_PIN))return 1;
	else return 0;		   
}
uint8 DHT11_Read_Byte(void)    
{        
  uint8 i,dat;
  dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}
void DHT11_GetFloat(uint8 *Data,float *Temp)
{
	uint16 T16;
	float T;
	T16=Data[0];T16*=0xff;T16+=Data[1];
	if(T16&0x8000)
	{
		T16&=~0x8000;
		T=T16;T/=10;T=0-T;
	}else 
	{
		T=T16;T/=10;
	}
	if((T>-50)&(T<100))*Temp=T;
}
uint8 DHT11_Read(float *Temp,float *Humi)
{
	uint8 Data[5],i,c;
	DHT11_Rst();
	if(DHT11_Check()){return 1;}
	for(i=0;i<5;i++)
	{
		Data[i]=DHT11_Read_Byte();
	}
	c=Data[0]+Data[1]+Data[2]+Data[3];
	if(Data[4]!=c){return 1;}
	if(Temp)DHT11_GetFloat(&Data[0],Humi);
	if(Humi)DHT11_GetFloat(&Data[2],Temp);
	return 0;
}
