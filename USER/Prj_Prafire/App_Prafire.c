#include <include.h>
#include <..\USER\Prj_Prafire\App_Prafire.h>
#include <..\FS\FATEST\FATEST.h>
int res;  


FATFS fs;
uint8 Bufdd[100];
void FS_PrintfFileContext(char *FilePath)
{
	int res;
	FIL fdst;
	uint32 br=1; 
  uint8 Buf[512]; 	
	res = f_open(&fdst, FilePath, FA_OPEN_EXISTING | FA_READ); /* 打开文件 */		
  for (;;) 
	{
		BufferFill(&Buf[0],0,sizeof(Buf));	 
  	res = f_read( &fdst, &Bufdd[0], sizeof(Bufdd), &br ); /* 将文件里面的内容读到缓冲区 */
		DeBug_Get();
		Tos_TaskDelay(10);
		DeBug("%s",&Bufdd[0],Infor_Infor);		
  	if (res || br == 0) break;                          /* 错误或者到了文件尾 */        	    	
  }
	f_close(&fdst);	
}
void FS_AppendFileContext(char *FilePath,char *Context)
{
	uint32 bw;
	FIL fdst;
	res = f_open(&fdst,FilePath,FA_CREATE_NEW | FA_WRITE);
	if(res ==FR_EXIST)
	res=f_open(&fdst,FilePath,FA_OPEN_ALWAYS | FA__WRITTEN|FA_WRITE);
	if ( res == FR_OK )
 	 { 
		res = f_lseek(&fdst, f_size(&fdst));
		res = f_write(&fdst, &Context[0], GetTextLength(&Context[0]), &bw); 
		res = f_write(&fdst, &Context[0], GetTextLength(&Context[0]), &bw);   
   		 f_close(&fdst);      
  	}
   else if ( res == FR_EXIST )
 	 {
		DeBug( "File Exist" ,Infor_Infor);
 	 }
}
FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof lfn;
#endif


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        i = GetTextLength(path);
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                DeBug("%s%s",&path[i], fn,Infor_Infor);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                DeBug("%s/%s\n", path, fn,Infor_Infor);
            }
        }
    }

    return res;
}
typedef struct tagRGBQUAD 
{

    BYTE rgbBlue; 													//该颜色的蓝色分量
    BYTE rgbGreen; 													//该颜色的绿色分量
    BYTE rgbRed; 														//该颜色的红色分量
    BYTE rgbReserved;											 	//保留值
} RGBQUAD,tagRGBQUAD;


FATFS bmpfs[2]; 
FIL bmpfsrc, bmpfdst; 
FRESULT bmpres;
#define GETR_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )RGB565) >>11)<<3))		  			//返回8位 R
#define GETG_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )(RGB565 & 0x7ff)) >>5)<<2)) 	//返回8位 G
#define GETB_FROM_RGB16(RGB565)  ((unsigned char)(( ((unsigned short int )(RGB565 & 0x1f))<<3)))       	//返回8位 B
int Screen_shot(unsigned short int x, unsigned short int y, unsigned short int Width, unsigned short int Height, unsigned char *filename)
{
    unsigned char header[54] =
    {
        0x42, 0x4d, 0, 0, 0, 0, 
        0, 0, 0, 0, 54, 0, 
        0, 0, 40,0, 0, 0, 
        0, 0, 0, 0, 0, 0, 
        0, 0, 1, 0, 24, 0, 
        0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 
        0, 0, 0
    };
    int i;
    int j;
    long file_size;     
    long width;
    long height;
    unsigned short int tmp_rgb;
    unsigned char r,g,b;	
    unsigned char tmp_name[30];
    unsigned int mybw;
    char kk[4]={0,0,0,0};
    
    file_size = (long)Width * (long)Height * 3 + Height*(Width%4) + 54;		//宽*高 +补充的字节 + 头部信息
    
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    
    
    width=Width;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;
    
    height = Height;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;
  
    FmtToBuf((char*)tmp_name,"0:%s.bmp",filename);
   
   f_mount(0, &bmpfs[0]);
    bmpres = f_open( &bmpfsrc , (char*)tmp_name, FA_CREATE_NEW | FA_WRITE );
		if(bmpres!=FR_OK)return FR_EXIST;
    f_close(&bmpfsrc);	
	 
	 bmpres = f_open( &bmpfsrc , (char*)tmp_name,  FA_OPEN_EXISTING | FA_WRITE);

    if ( bmpres == FR_OK )
    {    
        bmpres = f_write(&bmpfsrc, header,sizeof(unsigned char)*54, &mybw);
        for(i=0;i<Height;i++)						//高
        {
            if(!(Width%4))
            {
                for(j=0;j<Width;j++)  	//宽
                { 
                    tmp_rgb = LCD_Get_Pixel(j+y,Height-i+x);
                    r =  GETR_FROM_RGB16(tmp_rgb);
                    g =  GETG_FROM_RGB16(tmp_rgb);
                    b =  GETB_FROM_RGB16(tmp_rgb);     
                    bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);
                    
                }
                
            }
            else
            {
                for(j=0;j<Width;j++)
                { 
                    tmp_rgb = LCD_Get_Pixel(j+y,Height-i+x);
                    r =  GETR_FROM_RGB16(tmp_rgb);
                    g =  GETG_FROM_RGB16(tmp_rgb);
                    b =  GETB_FROM_RGB16(tmp_rgb);       
                    bmpres = f_write(&bmpfsrc, &b,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &g,sizeof(unsigned char), &mybw);
                    bmpres = f_write(&bmpfsrc, &r,sizeof(unsigned char), &mybw);   
                }
                
                bmpres = f_write(&bmpfsrc, kk,sizeof(unsigned char)*(Width%4), &mybw);
            }	
        }
        f_close(&bmpfsrc); 
        return 0;
    }
    else if ( bmpres == FR_EXIST )  //如果文件已经存在
    {
        return FR_EXIST;	 					//8
    }
    
    else
    {
        return -1;
    }    
}
void Task_Scan_Files()
{
	
		NVIC_SD_Configuration();
	
	SD_Init();
	
	f_mount(0,&fs);	
	DeBug_Get();
	//scan_files("0:/");
	FS_AppendFileContext("0:/Demo1.TXT","Hello Word!");
	FS_PrintfFileContext("0:/Demo1.TXT");
}
int res;
FIL fdst;
void Task_SDFAT(void *Tags)
{
	uint8 i='a';
	uint8 Name[20];
	if(f_mount(0,&fs)==SD_OK)
	{
		DeBug_Get();
		DeBug("挂载成功",Infor_Infor);
	}
	StringCoppy("fdsfsaf",&Name[0]);
	FS_PrintfFileContext("0:/Demo1.TXT");
	while(1)
	{
		  Tos_TaskDelay(1009);	//
			DeBug_OutSet(DeBug_USART2);
			FS_AppendFileContext("0:/Demo1.TXT","我想这次可以了!");
		Name[2]=i++;
		//Screen_shot(0, 0, 240,320 , Name);
	   
	}
}
