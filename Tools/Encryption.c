#include <Include.h>
#include <Encryption.h>


//********************************************************
//变量声明

//static uint8  PasswordSum[8];    //密码暂存
uint8  YwTemp;      //移位值存放单元


//*********************************************************
//函数声明
void StationSwitch(uint8  *ptr);  //位置变换
void XorP(uint8  *ptr);           //异或运算
void LrolP(uint8  *ptr,uint8 cb); //循环左移cb位
void LrorP(uint8  *ptr,uint8 cb); //循环右移cb位 


//***************************************************************************************
//位置变换(数据加密)
//原位置 1 2 3 4 5 6 7 8
//变换后 7 5 2 1 6 3 8 4

//***************************************************************************************
void StationSwitchE(uint8  *ptr)
{
   uint8  i;

   uint8 TempSum[8];
   TempSum[3]=*ptr;
   TempSum[2]=*(ptr+1);
   TempSum[5]=*(ptr+2);
   TempSum[7]=*(ptr+3);
   TempSum[1]=*(ptr+4);
   TempSum[4]=*(ptr+5);
   TempSum[0]=*(ptr+6);
   TempSum[6]=*(ptr+7);
   
   for(i=0;i<8;i++)
   {
      *ptr++=TempSum[i];   
   }

}

//***************************************************************************************
//位置变换(数据解密)
//原位置 7 5 2 1 6 3 8 4
//变换后 1 2 3 4 5 6 7 8

//***************************************************************************************
void StationSwitchD(uint8  *ptr)
{
   uint8  i;

   uint8 TempSum[8];
   TempSum[6]=*ptr;
   TempSum[4]=*(ptr+1);
   TempSum[1]=*(ptr+2);
   TempSum[0]=*(ptr+3);
   TempSum[5]=*(ptr+4);
   TempSum[2]=*(ptr+5);
   TempSum[7]=*(ptr+6);
   TempSum[3]=*(ptr+7);
   
   for(i=0;i<8;i++)
   {
      *ptr++=TempSum[i];   
   }

}


//***************************************************************************************
//异或运算
//64位明文，每8位为一字节，从高到低，共8字节。分别与 0x54,0x98,0xeb,0xdc,
//0x09,0x71,0xd7,0xcf进行位异或
//***************************************************************************************
void XorP(uint8  *ptr)
{
   uint8 i;
   i=*ptr;
   *ptr=i^0x54;
   i=*(ptr+1);
   *(ptr+1)=i^0x98;
   i=*(ptr+2);
   *(ptr+2)=i^0xeb;
   i=*(ptr+3);
   *(ptr+3)=i^0xdc;	   //A组

   i=*(ptr+4);
   *(ptr+4)=i^0x09;
   i=*(ptr+5);
   *(ptr+5)=i^0x71;
   i=*(ptr+6);
   *(ptr+6)=i^0xd7;
   i=*(ptr+7);
   *(ptr+7)=i^0xcf;	   //B组

}



//***************************************************************************************
//循环左移cb位
//cb取值1--31
//***************************************************************************************
void LrolP(uint8  *ptr,uint8 cb)
{
   UILONG mSD;
   uint8 i;
   uint32  ATemp,BTemp;
   for(i=0;i<4;i++)
   {
      mSD.UcType[i]=*(ptr+i);   
   }

   ATemp=mSD.UlType>>(32-cb);
   BTemp=mSD.UlType<<cb;
   mSD.UlType=ATemp|BTemp;
   for(i=0;i<4;i++)
   {
      *ptr++=mSD.UcType[i];   
   }
   
}

//***************************************************************************************
//循环右移cb位
//cb取值1--31
//***************************************************************************************
void LrorP(uint8  *ptr,uint8 cb)
{
   UILONG mSD;
   uint32  ATemp,BTemp;
   uint8 i;
   for(i=0;i<4;i++)
   {
      mSD.UcType[i]=*(ptr+i);   
   }

   ATemp=mSD.UlType<<(32-cb);
   BTemp=mSD.UlType>>cb;
   mSD.UlType=ATemp|BTemp;
   for(i=0;i<4;i++)
   {
      *ptr++=mSD.UcType[i];   
   }
   
}


//***************************************************************************************
//对64位数据加密
//加密过程：
//1.位置变换
//2.分组(64位明文的高32位为A组，低32位为B组),A组数据，B组分别与某一特定数据进行位异或
//3.A组数据循环左移YW位
//4.B组数据循环右移YW位
//5.位置变换
//***************************************************************************************
void EncryptP(uint8   *ptr)
{
   uint8 i;
   
   	StationSwitchE(ptr);             //位置变换(数据加密)
	XorP(ptr);                       //异或运算
        if((YwTemp==0)||(YwTemp>31))
        {
          i=M_DEFAULT_YW;
        }
        else
        {
          i=YwTemp;
        };
        
	LrolP(ptr,i);               //对A组数据循环左移(要加密明文的高32位)
 	LrorP((ptr+4),i);           //对B组数据循环右移(要加密明文的低32位)
          
        //StationSwitchD(ptr);        //位置变换	

}

//***************************************************************************************
//对64位数据解密
//***************************************************************************************
void DecryptP(uint8   *ptr)
{
  uint8 i;

   	//StationSwitchE(ptr);             //位置变换
        if((YwTemp==0)||(YwTemp>31))
        {
          i=M_DEFAULT_YW;
        }
        else
        {
          i=YwTemp;
        };    

	LrorP(ptr,i);               //对A组数据循环右移(要加密明文的高32位)
	LrolP((ptr+4),i);           //对B组数据循环左移(要加密明文的低32位)
        XorP(ptr);                  //异或运算	
        StationSwitchD(ptr);        //位置变换(数据解密)
}






