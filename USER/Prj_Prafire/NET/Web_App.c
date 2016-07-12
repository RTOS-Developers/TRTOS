const MUI_MenuStruct *CurGetMenu;
int GetInPutHtml(uint8 *Web_Buf,const MUI_MenuStruct *CurM)
{
	int plen=0;
	char String[100];
	plen=fill_tcp_data_p(Web_Buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
	plen=fill_tcp_data_p(Web_Buf,plen,"<html>");
	plen=fill_tcp_data_p(Web_Buf,plen,"<head>");
  plen=fill_tcp_data_p(Web_Buf,plen,"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">");
	plen=fill_tcp_data_p(Web_Buf,plen,"<title>请输入</title>");
  plen=fill_tcp_data_p(Web_Buf,plen,"<body background=http://192.168.0.3/img12.jpg>");	
  plen=fill_tcp_data_p(Web_Buf,plen,"<P align=right><IMG border=0 hspace=0 src=\"http://192.168.0.3/trtos_logo.png\" width=40 align=baseline height=40></P>");
	plen=fill_tcp_data_p(Web_Buf,plen,"<P align=center><STRONG><FONT size=7 color=#008000 ><SPAN id=wiz_font_size_span_285640406 style=\"FONT-SIZE: 72pt\">");
	MUI_GetOptionString(&String[0],CurM);
	plen=fill_tcp_data_p(Web_Buf,plen,(const uint8 *)&String[0]);	
	plen=fill_tcp_data_p(Web_Buf,plen,"</FONT></STRONG></P><P>");	
	plen=fill_tcp_data_p(Web_Buf,plen,"<HR color=#ff0000>");	
	plen=fill_tcp_data_p(Web_Buf,plen,"<center>");
	plen=fill_tcp_data_p(Web_Buf,plen,"<form action=\"input\" method=\"post\">");
	plen=fill_tcp_data_p(Web_Buf,plen,"<P align=center ><br>" );
	plen=fill_tcp_data_p(Web_Buf,plen,"<INPUT style=\"FONT-SIZE: 72pt; WIDTH: 500px\""); 
	plen=fill_tcp_data_p(Web_Buf,plen,"size=4 name=Text1> <P> <INPUT style=\"FONT-SIZE: 72pt;WIDTH: 500px\"  type=submit value=确定 name=ButtonSubmit1></P>");
	plen=fill_tcp_data_p(Web_Buf,plen,"</center>");
	plen=fill_tcp_data_p(Web_Buf,plen,"</form>");
	plen=fill_tcp_data_p(Web_Buf,plen,"</body>");
	plen=fill_tcp_data_p(Web_Buf,plen,"</html>");

	return plen;
}

	
int GetMenuHtml(uint8 *Web_Buf,const MUI_MenuStruct *CurM)
{
	int plen=0;
	char String[100];
	char Data[2]={'0','1'},i=0;
  plen=fill_tcp_data_p(Web_Buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\nPragma: no-cache\r\n\r\n"));
	plen=fill_tcp_data_p(Web_Buf,plen,"<html>");
	plen=fill_tcp_data_p(Web_Buf,plen,"<head>");
	plen=fill_tcp_data_p(Web_Buf,plen,"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">");
	plen=fill_tcp_data_p(Web_Buf,plen,"<title>Web 实验</title>");
	plen=fill_tcp_data_p(Web_Buf,plen,"</head>");
	//plen=fill_tcp_data_p(Web_Buf,plen,"<body bgcolor=\"#000000\">");	
	plen=fill_tcp_data_p(Web_Buf,plen,"<body background=http://192.168.0.3/img12.jpg>");	
 // plen=fill_tcp_data_p(Web_Buf,plen,"<P align=right><IMG border=0 hspace=0 src=\"http://192.168.0.3/trtos_logo.png\" width=40 align=baseline height=40></P>");
	plen=fill_tcp_data_p(Web_Buf,plen,"<P align=center><STRONG><FONT size=7 color=#008000 ><SPAN id=wiz_font_size_span_285640406 style=\"FONT-SIZE: 72pt\">");
	plen=fill_tcp_data_p(Web_Buf,plen,(const uint8 *)MUI_GetCurMenuInfor()->Option);	
	plen=fill_tcp_data_p(Web_Buf,plen,"</FONT></STRONG></P><P>");	
	plen=fill_tcp_data_p(Web_Buf,plen,"<HR color=#ff0000>");	
	plen=fill_tcp_data_p(Web_Buf,plen,"<STRONG>");
	//p"len=fill_tcp_data_p(Web_Buf,plen,"<input style=\"HEIGHT: 58px; WIDTH: 100px\" type=\"button\" value=\"弹出警告框\" onClick=\"AlertButton()\"> ");
//	plen=fill_tcp_data_p(Web_Buf,plen,"<script>function AlertButton(){window.alert(\"要多多光临呀！\");}</script>");
	while(CurM->Type!=VT_End)
	{
		plen=fill_tcp_data_p(Web_Buf,plen,"<P align=left><FONT color=#ff0000 size=7><A href=\"");
		plen=fill_tcp_data(Web_Buf,plen,baseurl);		 // 要在浏览器中输入的网址
    plen=fill_tcp_data(Web_Buf,plen,password);	 // 紧跟在网址后面的密码
		plen=fill_tcp_data(Web_Buf,plen,"/");	 // 紧跟在网址后面的密码
		Data[0]=i%100/10+'0';
		Data[1]=i++%10+'0';
		plen=fill_tcp_data(Web_Buf,plen,(const char *)&Data[0]);
		plen=fill_tcp_data_p(Web_Buf,plen,"\">");
		MUI_GetOptionString(&String[0],CurM);
		plen=fill_tcp_data_p(Web_Buf,plen,(const uint8 *)&String[0]);
		plen=fill_tcp_data_p(Web_Buf,plen,"</A></FONT></P>");
		CurM++;
	}

	plen=fill_tcp_data_p(Web_Buf,plen,"</STRONG>");	
	plen=fill_tcp_data_p(Web_Buf,plen,"<br>");
	plen=fill_tcp_data_p(Web_Buf,plen,"<br>");
	plen=fill_tcp_data_p(Web_Buf,plen,"<HR>");
	plen=fill_tcp_data_p(Web_Buf,plen,"</body>");	
	plen=fill_tcp_data_p(Web_Buf,plen,"</html>");	
	return plen;
}
int GetInforHtml(uint8 *Web_Buf)
{
	return 0;
}
int analyse_get_Index(char *str)
{
	unsigned char i=0;
	int Data=0;
	if (verify_password(str)==0)
	{
		return(-1);
	}
	while(*str && i<10 && *str >',' && *str<'{')
	{
		if (*str=='/')
		{
				str++;
				break;
		}
		i++;str++;
	}
	if (*str <= '9' && *str >='0')Data=*str-0x30;str++;
	if (*str <= '9' && *str >='0'){Data*=10;Data+=(*str-0x30);return Data;}
	return(-2);
}

void Web_SendKey(uint8 KeyData)
{
	MGUI_KeyMsg KeyMsg;
	KeyMsg.KeyValue=KeyData;
	Tos_TaskPutQmsg(KeyIn_Tos_TaskID,KeyBoard_Hand,sizeof(MGUI_KeyMsg),&KeyMsg);
}
void Web_Enter(const MUI_MenuStruct *CurMenu,char *Text1)
{
	typedef   void 	(*FuncUint8)(uint8);
	switch(CurMenu->Type)
	{
		case VT_Menu:MenuValue->OldMenu=MenuValue->CurMenu;
			if(CurMenu->Note>MenuValue->Index){MenuValue->Index=CurMenu->Note;MenuValue->DeepRecord[MenuValue->Index]=0;}
		  else MenuValue->Index=CurMenu->Note;MenuValue->CurMenu=(const MUI_MenuStruct*)CurMenu->Value;MenuValue->DrawMode|=Draw_Menu;break;
		case VT_Task:if(CurMenu->Value)Task_Join((void *)CurMenu->Value,FT_VoidFuncVoid,Null);break;
		case VT_TaskUint8:if(CurMenu->Value)Task_Join((void *)CurMenu->Value,FT_VoidFuncUint8,CurMenu->Note);Task_Join(Web_SendKey,FT_VoidFuncUint8,0);break;
		case VT_Uint8:*(uint8 *)(CurMenu->Value)=GetStringToNumber((uint8 *)Text1);break;
		case VT_Int8:*(int8 *)(CurMenu->Value)=GetStringToNumber((uint8 *)Text1);break;
		case VT_Uint16:*(uint16 *)(CurMenu->Value)=GetStringToNumber((uint8 *)Text1);break;
		case VT_Float:*(float *)(CurMenu->Value)=GetStringToNumber((uint8 *)Text1);break;
		case VT_Str:StringCoppy(Text1,(uint8 *)(CurMenu->Value));break;
		case VT_IP:*(uint32 *)(CurMenu->Value)=StringToIPUint32((uint8 *)Text1);break;
		case VT_BOOL:*(uint8 *)(CurMenu->Value)=!(BOOL)*(uint8 *)(CurMenu->Value);break;
	}
}
extern uint8 KeyBoard_Tos_TaskID;
void  Web_App(void *Tags)
{   
	unsigned int plen, i1 = 0;
	unsigned int dat_p;
	int cmd,ch;
	char *Text1;
	const MUI_MenuStruct *CurM;
	char *Web_Buf1;
	unsigned int payloadlen = 0;
  SPI_Enc28j60_Init();
  enc28j60Init(mymac);
  enc28j60PhyWrite(PHLCON,0x476);	  
	init_ip_arp_udp_tcp(mymac,myip,mywwwport);
  while(1)
  {   
			Tos_TaskDelay(10);	
      plen = enc28j60PacketReceive(Web_Buf_Size, Web_Buf);  
      if(plen==0)
      { 
          continue;
      }
		//	USART_WriteDatas(USART2,(uint8 *)&Web_Buf[0],plen);
      if(eth_type_is_arp_and_my_ip(Web_Buf,plen))
      {
          make_arp_answer_from_request(Web_Buf);          
          continue;
      }		
      if(eth_type_is_ip_and_my_ip(Web_Buf,plen)==0) //检查这是不是我的IP
      {
          continue;
      }      
      
      if(Web_Buf[IP_PROTO_P]==IP_PROTO_ICMP_V && Web_Buf[ICMP_TYPE_P]==ICMP_TYPE_ECHOREQUEST_V)
      {
          make_echo_reply_from_request(Web_Buf, plen);          
          continue;
      }

      if (Web_Buf[IP_PROTO_P]==IP_PROTO_TCP_V&&Web_Buf[TCP_DST_PORT_H_P]==0&&Web_Buf[TCP_DST_PORT_L_P]==mywwwport)
      {
          if (Web_Buf[TCP_FLAGS_P] & TCP_FLAGS_SYN_V)
          {
              make_tcp_synack_from_syn(Web_Buf);
              continue;
          }
          if (Web_Buf[TCP_FLAGS_P] & TCP_FLAGS_ACK_V)
          {
              init_len_info(Web_Buf);
              dat_p=get_tcp_data_pointer();
              if (dat_p==0)
              {
                  if (Web_Buf[TCP_FLAGS_P] & TCP_FLAGS_FIN_V)
                  {
                      make_tcp_ack_from_any(Web_Buf);
                  }
                  continue;
              }
              if (strncmp("GET ",(char *)&(Web_Buf[dat_p]),4)!=0)
              {
								ch=BytesFindString((char *)Web_Buf,"Text1=",plen);
								if(ch!=Null)
								{
								Text1=(char *)(Web_Buf+ch);
								USART_WriteDatas(USART2,(uint8 *)Web_Buf,plen);
								StringReplaceChar((uint8 *)Text1,(uint8)'&',0);
								HtmlGetGB2312((uint8 *)Text1);
								Web_Enter(CurGetMenu,Text1);
								Web_SendKey(0);
								}
                  goto DRAWMENU;
              }
              if (strncmp("/ ",(char *)&(Web_Buf[dat_p+4]),2)==0)
              {
                  plen=fill_tcp_data_p(Web_Buf,0,PSTR("HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n"));
                  plen=fill_tcp_data_p(Web_Buf,plen,PSTR("<p>Usage: "));
                  plen=fill_tcp_data(Web_Buf,plen,baseurl);
                  plen=fill_tcp_data_p(Web_Buf,plen,PSTR("password</p>"));
                  goto SENDTCP;
              }
              cmd=analyse_get_Index((char *)&(Web_Buf[dat_p+5]));
              if(cmd>=0)
							{
								CurM=MenuValue->CurMenu+cmd;
								switch(CurM->Type)
								{
									case VT_Uint8:plen=GetInPutHtml(Web_Buf,CurM);CurGetMenu=CurM;goto SENDTCP;
									case VT_Int8:plen=GetInPutHtml(Web_Buf,CurM);CurGetMenu=CurM;goto SENDTCP;
									case VT_Uint16:plen=GetInPutHtml(Web_Buf,CurM);CurGetMenu=CurM;goto SENDTCP;
									case VT_Float:plen=GetInPutHtml(Web_Buf,CurM);CurGetMenu=CurM;goto SENDTCP;
									case VT_Str:plen=GetInPutHtml(Web_Buf,CurM);CurGetMenu=CurM;goto SENDTCP;
									case VT_IP:plen=GetInPutHtml(Web_Buf,CurM);CurGetMenu=CurM;goto SENDTCP;
								}
								Web_Enter(MenuValue->CurMenu+cmd,Null);
								Web_SendKey(0);
							}
DRAWMENU:
              plen=GetMenuHtml(Web_Buf,MenuValue->CurMenu);
SENDTCP:
              make_tcp_ack_from_any(Web_Buf);      
							make_tcp_ack_with_data(Web_Buf,plen); 
              continue;
          }
      }
/*-------------------------------------- tcp port www end ---------------------------------------*/

/*--------------------- udp start, we listen on udp port 1200=0x4B0 -----------------------------*/
      if (Web_Buf[IP_PROTO_P]==IP_PROTO_UDP_V&&Web_Buf[UDP_DST_PORT_H_P]==4&&Web_Buf[UDP_DST_PORT_L_P]==0xb0)
      {
          payloadlen=	  Web_Buf[UDP_LEN_H_P];
          payloadlen=payloadlen<<8;
          payloadlen=(payloadlen+Web_Buf[UDP_LEN_L_P])-UDP_HEADER_LEN;
          //payloadlen=Web_Buf[UDP_LEN_L_P]-UDP_HEADER_LEN;
          
         // ANSWER:
          //while(1){
          for(i1=0; i1<payloadlen; i1++) Web_Buf1[i1]=Web_Buf[UDP_DATA_P+i1];
          
          //make_udp_reply_from_request(Web_Buf,str,strlen(str),myudpport);
          make_udp_reply_from_request(Web_Buf,(char *)Web_Buf1,payloadlen,myudpport);          
          //}
      }
/*----------------------------------------udp end -----------------------------------------------*/
  }
        //  return (0);
}
