#ifndef _TGUI_CONFIG_H_
#define _TGUI_CONFIG_H_

/*-------------------------------------------------------------------------
 TGUI 控件当前状态
-------------------------------------------------------------------------*/
#define TGUI_State_UnLoad		0x01					//控件未加载状态
#define TGUI_State_ReLoad     	0x02					//控件重加载到界面
#define TGUI_State_UnFocus		0x03					//控件失去焦点状态
#define TGUI_State_OnFocus		0x04					//控件获得焦点状态
#define TGUI_State_Deleted		0x05					//控件从界面被删除状态
#define TGUI_State_EntryEdit	0x06					//控件进入编辑模式  
#define TGUI_State_ExitEdit		0x07					//控件退出编辑模式
#define TGUI_State_Hidden		0x08					//控件进入隐藏状态

/*-------------------------------------------------------------------------
 TGUI 控件呈现样式
-------------------------------------------------------------------------*/			  
#define TGUI_Style_NoBcolor		0x01					//控件无背景色显示
#define TGUI_Style_NoFrame		0x02					//控件无边框显示
#define TGUI_Style_NoTitle		0x04					//控件无标题显示
#define TGUI_Style_NoDirH		0x08					//控件垂直方向,传90度
#define TGUI_Style_NoDirInc		0x10					//控件向反序方向 水平右->左 垂直下->上
#define TGUI_Style_NoVbar		0x20					//控件无垂直滚动条
#define TGUI_Style_NoHvar		0x40					//控件无水平滚动条

#define TGUI_Style_Default		0x00					//控件默认样式

/*-------------------------------------------------------------------------							
 TGUI 控件行为样式
-------------------------------------------------------------------------*/
#define TGUI_Action_EnOnFucus	0x01					//控件能响应焦点事件
#define TGUI_Action_EnEdit		0x02					//控件能响应编辑事件
#define TGUI_Action_TabDirV		0x04					//控件能响应垂直TAB(Up/Down)
#define TGUI_Action_TabDirH		0x08					//控件能响应水平TAB(Left/Right)
#define TGUI_Action_Enable		0x10					//控件事件响应激活

/*-------------------------------------------------------------------------
 TGUI 方向定义
-------------------------------------------------------------------------*/
#define TGUI_Dir_Up	  		  0x01
#define TGUI_Dir_Down		  0x02
#define TGUI_Dir_Left		  0x03
#define TGUI_Dir_Right		  0x04

/*-------------------------------------------------------------------------
 TGUI 线样式定义
-------------------------------------------------------------------------*/
#define TGUI_Line_Light       0xFF
#define TGUI_Line_Dark        0x49
#define TGUI_Line_Face        242
#define TGUI_Line_Back        0xFF
#define TGUI_DotLine_Face     0x00


/*-------------------------------------------------------------------------
 TGUI 定义元素类型
-------------------------------------------------------------------------*/
#define TGUI_Type_Window	  0x01
#define TGUI_Type_Button	  0x02
#define TGUI_Type_Textbox	  0x03
#define TGUI_Type_TextArea	  0x04
#define TGUI_Type_ComBox	  0x05
#define TGUI_Type_Radio		  0x06
#define TGUI_Type_CheckBox	  0x07
#define TGUI_Type_LineChart	  0x08
#define TGUI_Type_Label		  0x09
#define TGUI_Type_ImageBox	  0x0A
#define TGUI_Type_ProcessBar  0x0B

/*-------------------------------------------------------------------------
 TGUI 定义消息类型
-------------------------------------------------------------------------*/
#define TGUI_MsgType_KeyBoard 	  0x01				//键盘消息
#define TGUI_MsgType_SoftEvent	  0x02				//软消息
#define TGUI_MsgType_SysInt		  0x03				//系统中断
#define TGUI_MsgType_TouchScreen  0x04				//触模屏消息
#define TGUI_MsgType_SysTimer	  0x05				//时间消息


/*-------------------------------------------------------------------------
 TGUI 定义事件类型
-------------------------------------------------------------------------*/
#define TGUI_Event_KeyClickDown	  0x01
#define TGUI_Event_KeyClickUp	  0x02
#define TGUI_Event_KeyHolding	  0x03
#define TGUI_Event_TouchDown   	  0x05
#define TGUI_Event_TouchUp		  0x06
#define TGUI_Event_TouchHolding	  0x07
#define TGUI_Event_ControlOnFocus 0x10
#define TGUI_Event_ControlUnFocus 0x11

/*-------------------------------------------------------------------------
 TGUI 定义控件事件处理状态
-------------------------------------------------------------------------*/
#define TGUI_Result_BackSpace	  0x01				//控件要求到上一个索引
#define TGUI_Result_NextSpace	  0x02				//控件要求到下一个索引
#define TGUI_Result_Ok			  0x03				//控件返回正确
#define TGUI_Result_Yes			  0x04				//控件返回确认
#define TGUI_Result_No			  0x05				//控件返回否认
#define TGUI_Result_Cancel		  0x06				//控件返回取消
#define TGUI_Result_Null		  0x07				//控件返回空
#define TGUI_Result_Ignore		  0x08				//控件将当前事件忽略
#define TGUI_Result_Error		  0x09				//控件返回错误
#define TGUI_Result_Success		  0x0A				//控件返回成功

/*-------------------------------------------------------------------------
 TGUI 定义编辑类型
-------------------------------------------------------------------------*/
#define TGUI_EditType_Num		  0x01				//允许输入数字
#define TGUI_EditType_Dot		  0x02				//允许输入点
#define TGUI_EditType_Sig		  0x04				//允许输入符号
#define TGUI_EditType_UpDown	  0x08				//允许上下键加减
#define TGUI_EditType_OkEdit	  0x10				//先确定再编辑

#define TGUI_EditType_EnEdit	  0x80				//当前编辑使能状态
#define TGUI_EditType_Default     0x17

/*-------------------------------------------------------------------------
 TGUI 定义按键值
-------------------------------------------------------------------------*/
#define TGUI_Key_Num0	  0x01
#define TGUI_Key_Num1	  0x02
#define TGUI_Key_Num2	  0x03
#define TGUI_Key_Num3	  0x04
#define TGUI_Key_Num4	  0x05
#define TGUI_Key_Num5	  0x06
#define TGUI_Key_Num6	  0x07
#define TGUI_Key_Num7	  0x08
#define TGUI_Key_Num8	  0x09
#define TGUI_Key_Num9	  0x0A
#define TGUI_Key_Dot	  0x0B				//.符号
#define TGUI_Key_Add	  0x0C				//+符号
#define TGUI_Key_Sub	  0x0D				//-符号
#define TGUI_Key_Mul	  0x0E				//*符号
#define TGUI_Key_Div	  0x0F				///符号
#define TGUI_Key_Up		  0x30
#define TGUI_Key_Down	  0x31
#define TGUI_Key_Left	  0x32
#define TGUI_Key_Right	  0x33
#define TGUI_Key_Ok		  0x34
#define TGUI_Key_Esc	  0x35
#define TGUI_Key_Cannel	  0x36
#define TGUI_Key_F1		  0x50	
#define TGUI_Key_F2		  0x51
#define TGUI_Key_F3		  0x52
#define TGUI_Key_F4		  0x53
#define TGUI_Key_F5		  0x54




/*-------------------------------------------------------------------------
 TGUI全局属性
--------------------------------------------------------------------------*/
#define TGUI_Image_Null		0xFFFFFFFF		//设置图片为空
#define TGUI_Object_Null	0x00			//空元素
#define TGUI_Cursor_Width	0x02			//定义光标宽度
#define TGUI_Cursor_Height  0x10			//定义光标高度

#define TGUI_OnFocus_ForeColor	0x0000		//选中状态下的前景色
#define TGUI_OnFocus_BackColor	0xFFFF		//选中状态下的背景色
#define TGUI_OnEdit_ForeColor	0xFFFF		//编辑状态下的前景色
#define TGUI_OnEdit_BackColor	0x001F		//编辑状态下的背景色


/*-------------------------------------------------------------------------
 TGUI窗口控件定义
-------------------------------------------------------------------------*/

#define TGUI_Window_TitleHeight	    30				//定义窗口标题栏高度像素
#define TGUI_Window_TitleBackColor 	Color_Blue		//定义窗口标题栏背景色
#define TGUI_Window_TitleForeColor	Color_White		//定义窗口标题栏前景色
#define TGUI_Window_TitleLenth		11				//定义窗口标题长度
#define TGUI_Window_BackColor		Color_White		//定义窗口默认背景色
#define TGUI_Window_ForeColor		Color_Black		//定义窗口默认前景色
#define TGUI_Window_Buffer_Lenth	20
#define TGUI_Window_CursorEnable	0x01			//窗口光标开启
#define TGUI_Window_CursorState		0x02			//窗口光标状态


/*-------------------------------------------------------------------------
 TGUI按钮控件定义
-------------------------------------------------------------------------*/
#define TGUI_Button_ForeColor    0x0000						//按钮表面前景色
#define TGUI_Button_BackColor    0xCDff						//按钮背景色
#define TGUI_Button_TextColor	 0xFFFF						//按钮表面文本
#define TGUI_Button_Lenth		 9							//按钮文本串长度

/*-------------------------------------------------------------------------
TGUI文本框控件定义
-------------------------------------------------------------------------*/
#define Textbox_Enable     0x01
#define Textbox_Password   0x02
#define Textbox_Multiline  0x03

#define TGUI_Textbox_FaceColor   0x0000
#define TGUI_Textbox_BackColor   0xFFFF
#define TGUI_Textbox_BDcolor	 0x0000
#define TGUI_Textbox_Lenth  8
#define TGUI_Textbox_AlignMent	Align_CenterCenter

/*-------------------------------------------------------------------------
TGUI文本域控件定义
-------------------------------------------------------------------------*/
#define TextArea_Enable     0x01
#define TextArea_Password   0x02
#define TextArea_Multiline  0x03 

#define TGUI_TextArea_Face   0xFF  
#define TGUI_TextArea_Back   0x49
#define TGUI_TextArea_Lenth  100
#define TGUI_TextArea_ScrollBarWidth 16


/*-------------------------------------------------------------------------
TGUI下拉菜单域控件定义
-------------------------------------------------------------------------*/
#define TGUI_ComBox_ForeColor   0xFFFF  
#define TGUI_ComBox_BackColor   0xCDB4
#define TGUI_ComBox_ForeColorH	0xFFFF		//高亮选中项前景色
#define TGUI_ComBox_BackColorH	0x4228		//高亮选中项背景色
#define TGUI_ComBox_Lenth  14 
#define TGUI_ComBox_ScrollBarWidth 20		//ComBox控件滚动条宽度

#define TGUI_ComBox_ListLines   15
#define TGUI_ComBox_ItemLenth   15
#define TGUI_ComBox_ItemsMax    16
#define TGUI_ComBox_LineFirst	10			//下拉窗体首行元素间距
#define TGUI_ComBox_LineHeight  23			//下拉元素显示行距

#define TGUI_ComBox_Flag_Expand 0x01		//ComBox控件下拉窗口已展开


/*-------------------------------------------------------------------------
TGUI下拉菜单域控件定义
-------------------------------------------------------------------------*/
#define TGUI_Object_FaceColor 242			//控件表面色彩	
#define TGUI_Object_DarkColor 0x49  		//控件阴影色彩


/*-------------------------------------------------------------------------
TGUI 单选按钮控件定义
-------------------------------------------------------------------------*/
#define TGUI_Radio_Face   0xCDB4  
#define TGUI_Radio_Back   0xFFFF
#define TGUI_Radio_Lenth  14 

#define TGUI_Radio_ItemLenth   11
#define TGUI_Radio_ItemsMax    10

#define TGUI_Radio_ChooseMode  0x04			//单选控件处于选择状态

/*-------------------------------------------------------------------------
 TGUI 多选按钮定义
-------------------------------------------------------------------------*/
#define TGUI_CheckBox_Face   0xFF  
#define TGUI_CheckBox_Back   0x49
#define TGUI_CheckBox_Lenth  14 

#define TGUI_CheckBox_ItemLenth   11
#define TGUI_CheckBox_ItemsMax    10

#define TGUI_CheckBox_ChooseMode  0x04

/*-------------------------------------------------------------------------
 TGUI Label标签控件定义
-------------------------------------------------------------------------*/
#define TGUI_Label_Lenth	20
#define TGUI_Label_Align	Align_CenterCenter
#define TGUI_Label_Fcolor	Color_Black
#define TGUI_Label_Bcolor	Color_White
#define TGUI_Label_BDcolor	Color_Black					//标签边框色彩

/*-------------------------------------------------------------------------
 TGUI ImageBox标签控件定义
-------------------------------------------------------------------------*/
#define TGUI_ImageBox_Bcolor Color_White
#define TGUI_ImageBox_BDcolor Color_Black

/*-------------------------------------------------------------------------
 TGUI ProcessBar进度条控件定义
-------------------------------------------------------------------------*/
#define TGUI_ProcessBar_Fcolor  0xBB88
#define TGUI_ProcessBar_Bcolor  0x94B2
#define TGUI_ProcessBar_BDcolor Color_Black;

#endif
