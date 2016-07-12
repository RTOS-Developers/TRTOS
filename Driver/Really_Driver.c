#include <Really_Driver.h>
#include <text.h>
#ifdef BOARD_REALLY

uint8 Really_TrunIndex=0;
uint8 *Readlly_P;
void ReallyCreate_Init(uint8 *P)
{
	Readlly_P=P;
	Really_TrunIndex=0;
}
void send(uint8 Data)
{
	USART1_SendByte(Data);
}
uint8 ReallyGetLength()
{
	return Really_TrunIndex;
}
void display_ID()      
{
    send(0xaa);
    send(0x00);
}
void reset()
{
    send(0xaa);
    send(0x01);
}

void modify_ID(uint8 oid,uint8 nid)
{
    send(0xaa);
    send(oid);
    send(0x15);
    send(nid);
}

void clear_display(uint8 ID)
{
    send(0xaa);
	if(ID!=0)
	{
        send(ID);
	}
    send(0x13);
}

void display_on(uint8 ID)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x10);
}

void display_off(uint8 ID)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x11);
}


void baud(uint8 ID,uint32 n)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x14);
    if(n==9600)
    {
        send(0);
    }
    else if(n==19200)
    {
        send(1);
    }

    else if(n==38400)
    {
        send(2);
    }

    else if(n==57600)
    {
        send(3);
    }
    else if(n==115200)
    {
        send(4);
    }else send(0);


}

void foreground_color(uint8 ID,uint8 r,uint8 g,uint8 b)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x21);
    send(r);
    send(g);
    send(b);
}

void background_color(uint8 ID,uint8 r,uint8 g,uint8 b)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x20);
    send(r);
    send(g);
    send(b);
}

void background_transparen(uint8 ID)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x22);
    send(0x01);
}

void background_nor_transparen(uint8 ID)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x22);
    send(0x00);
}

void cusor_xy(uint8 ID,uint16 x,uint16 y)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x23);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
}

void simplified_chinese(uint8 ID)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x24);
    send(0);
}

void traditional_chinese(uint8 ID)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x24);
    send(1);
}

void japanese(uint8 ID)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x24);
    send(2);
}

void korean(uint8 ID)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x24);
    send(3);
}

void font_size_16(uint8 ID)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x25);
    send(0);
}

void font_size_24(uint8 ID)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x25);
    send(1);
}

void font_size_32(uint8 ID)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x25);
    send(2);
}

void line_spacing(uint8 ID,uint8 n)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x26);
    send(n);
}

void row_spacing(uint8 ID,uint8 n)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x27);
    send(n);
}

void print_string(uint8 ID,uint8 *ptr)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x28);
    while(*ptr!='\0')
    {
        send(*ptr++);             
    }
    send(0X0d);
}

void print_string_xy(uint8 ID,uint16 x,uint16 y,uint8 *ptr)
{

    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x23);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(0xaa);	
	if(ID!=0)
	{
        send(ID);
	}
    send(0x28);
    while(*ptr!='\0')
    {
        send(*ptr++);             
    }
    send(0X0d);
}

void print_uint32(uint8 ID,uint32 n)
{
    uint32 i,temp;
    uint8 t=0;
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x28);
	if(n==0)
	{
        send(0x30);
	}
	else
	{
        for(i=1000000000;i>=1;i=i/10)
        {
            temp=n/i;
            if(t==0&&temp)
            {
                send(temp%10+0x30);
                t=1;
            }
            else if(t==1)
            {
                send(temp%10+0x30);
            }
		}
    }
    send(0x0d);
}


void print_uint32_xy(uint8 ID,uint16 x,uint16 y,uint32 n)
{
    uint32 i,temp;
    uint8 t=0;
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x23);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x28);
	if(n==0)
	{
        send(0x30);
	}
	else
	{
        for(i=1000000000;i>=1;i=i/10)
        {
            temp=n/i;
            if(t==0&&temp)
            {
                send(temp%10+0x30);
                t=1;
            }
            else if(t==1)
            {
                send(temp%10+0x30);
            }
		}
    }
    send(0x0d);
}

void draw_pixel(uint8 ID,uint16 x,uint16 y,uint8 r,uint8 g,uint8 b)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x30);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(r);
    send(g);
    send(b);
}

void draw_line(uint8 ID,uint16 x1,uint16 y1,uint16 x2,uint16 y2,uint8 r,uint8 g,uint8 b)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x31);
    send(x1>>8);
    send(x1);
    send(y1>>8);
    send(y1);
    send(x2>>8);
    send(x2);
    send(y2>>8);
    send(y2);
    send(r);
    send(g);
    send(b);
}

void draw_rectangle(uint8 ID,uint16 x,uint16 y,uint16 w,uint16 h,uint8 r,uint8 g,uint8 b)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x36);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(w>>8);
    send(w);
    send(h>>8);
    send(h);
    send(r);
    send(g);
    send(b);
}

void draw_rectangle_fill(uint8 ID,uint16 x,uint16 y,uint16 w,uint16 h,uint8 r,uint8 g,uint8 b)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x37);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(w>>8);
    send(w);
    send(h>>8);
    send(h);
    send(r);
    send(g);
    send(b);
}

void draw_circle_square(uint8 ID,uint16 x,uint16 y,uint16 w,uint16 h,uint16 ra,uint8 r,uint8 g,uint8 b)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x38);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(w>>8);
    send(w);
    send(h>>8);
    send(h);
	send(ra>>8);
    send(ra);
    send(r);
    send(g);
    send(b);
}

void draw_circle_square_fill(uint8 ID,uint16 x,uint16 y,uint16 w,uint16 h,uint16 ra,uint8 r,uint8 g,uint8 b)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x39);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(w>>8);
    send(w);
    send(h>>8);
    send(h);
	send(ra>>8);
    send(ra);
    send(r);
    send(g);
    send(b);
}


void draw_circle(uint8 ID,uint16 x,uint16 y,uint16 ra,uint8 r,uint8 g,uint8 b)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x32);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(ra>>8);
    send(ra);
    send(r);
    send(g);
    send(b);
}

void draw_circle_fill(uint8 ID,uint16 x,uint16 y,uint16 ra,uint8 r,uint8 g,uint8 b)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x33);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(ra>>8);
    send(ra);
    send(r);
    send(g);
    send(b);
}

void draw_ellipse(uint8 ID,uint16 x,uint16 y,uint16 xra,uint16 yra,uint8 r,uint8 g,uint8 b)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x34);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(xra>>8);
    send(xra);
	send(yra>>8);
    send(yra);
    send(r);
    send(g);
    send(b);
}
void draw_ellipse_fill(uint8 ID,uint16 x,uint16 y,uint16 xra,uint16 yra,uint8 r,uint8 g,uint8 b)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x35);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(xra>>8);
    send(xra);
	send(yra>>8);
    send(yra);
    send(r);
    send(g);
    send(b);
}

void picture_xy(uint8 ID,uint8 *filename,uint16 x,uint16 y)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x40);
	while(*filename!='\0')
    {
        send(*filename++);             
    }
    send(0X0d);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);

}

void pitrure_spread(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x41);
	while(*filename!='\0')
    {
        send(*filename++);             
    }
    send(0X0d);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(0);
	send(s);

}
void picture_right_to_left(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x41);
	while(*filename!='\0')
    {
        send(*filename++);             
    }
    send(0X0d);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(2);
	send(s);

}

void picture_left_to_right(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x41);
	while(*filename!='\0')
    {
        send(*filename++);             
    }
    send(0X0d);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(1);
	send(s);

}

void picture_up_to_down(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x41);
	while(*filename!='\0')
    {
        send(*filename++);             
    }
    send(0X0d);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(3);
	send(s);

}

void picture_down_to_up(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x41);
	while(*filename!='\0')
    {
        send(*filename++);             
    }
    send(0X0d);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(4);
	send(s);

}

void picture_horizontally(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x41);
	while(*filename!='\0')
    {
        send(*filename++);             
    }
    send(0X0d);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(5);
	send(s);

}

void picture_vertical(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x41);
	while(*filename!='\0')
    {
        send(*filename++);             
    }
    send(0X0d);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(6);
	send(s);

}

void picture_clip(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint16 lx,uint16 ly,uint16 w,uint16 h)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x42);
	while(*filename!='\0')
    {
        send(*filename++);             
    }
    send(0X0d);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(lx>>8);
    send(lx);
    send(ly>>8);
    send(ly);
	send(w>>8);
    send(w);
    send(h>>8);
    send(h);
}

void picture_zom(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint16 w,uint16 h)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x43);
	while(*filename!='\0')
    {
        send(*filename++);             
    }
    send(0X0d);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(w>>8);
    send(w);
    send(h>>8);
    send(h);
}
void picture_best_show(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint16 w,uint16 h)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x44);
	while(*filename!='\0')
    {
        send(*filename++);             
    }
    send(0X0d);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(w>>8);
    send(w);
    send(h>>8);
    send(h);
}

void load_buffer(uint8 ID,uint8 *filename,uint8 n)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x45);
	while(*filename!='\0')
    {
        send(*filename++);             
    }
    send(0X0d);
    send(n);
}

void unload_buffer(uint8 ID,uint8 n)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x46);
    send(n);
}

void buffer_xy(uint8 ID,uint8 n,uint16 x,uint16 y)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x47);
	send(n);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);

}

void buffer_spread(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x48);
	send(n);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(0);
	send(s);

}
void buffer_right_to_left(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x48);
	send(n);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(2);
	send(s);

}

void buffer_left_to_right(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x48);
	send(n);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(1);
	send(s);

}

void buffer_up_to_down(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x48);
	send(n);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(3);
	send(s);

}

void buffer_down_to_up(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x48);
	send(n);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(4);
	send(s);

}

void buffer_horizontally(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x48);
	send(n);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(5);
	send(s);

}

void buffer_vertical(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x48);
	send(n);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(6);
	send(s);

}

void buffer_clip(uint8 ID,uint8 n,uint16 x,uint16 y,uint16 lx,uint16 ly,uint16 w,uint16 h)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x49);
	send(n);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
    send(lx>>8);
    send(lx);
    send(ly>>8);
    send(ly);
	send(w>>8);
    send(w);
    send(h>>8);
    send(h);
}

void buffer_zom(uint8 ID,uint8 n,uint16 x,uint16 y,uint16 w,uint16 h)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x4A);
	send(n);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(w>>8);
    send(w);
    send(h>>8);
    send(h);
}
void buffer_best_show(uint8 ID,uint8 n,uint16 x,uint16 y,uint16 w,uint16 h)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x4B);
	send(n);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(w>>8);
    send(w);
    send(h>>8);
    send(h);
}

void buffer_autodisplay(uint8 ID,uint8 n1,uint8 n2,uint16 x,uint16 y)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
	send(0X4C);
    send(n1);
	send(n2);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
}

void buffer_autodisplay_speed(uint8 ID,uint8 n)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
	send(0X4D);
    send(n);
}

void buffer_autodisplay_stop(uint8 ID)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
	send(0X4E);
}

void windows(uint8 ID, uint16 x,uint16 y,uint16 w,uint16 h,uint8 *name)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x50);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(w>>8);
    send(w);
    send(h>>8);
    send(h);
	while(*name!='\0')
    {
        send(*name++);             
    }
    send(0X0d);
}

void windows_click(uint8 ID, uint16 x,uint16 y,uint16 w,uint16 h)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x51);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(w>>8);
    send(w);
    send(h>>8);
    send(h);
}

void button(uint8 ID, uint16 x,uint16 y,uint16 w,uint16 h)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x52);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(w>>8);
    send(w);
    send(h>>8);
    send(h);
}

void button_click(uint8 ID, uint16 x,uint16 y,uint16 w,uint16 h)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x53);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(w>>8);
    send(w);
    send(h>>8);
    send(h);
}

void textbox(uint8 ID, uint16 x,uint16 y,uint16 w,uint16 h)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x54);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(w>>8);
    send(w);
    send(h>>8);
    send(h);
}

void frame(uint8 ID, uint16 x,uint16 y,uint16 w,uint16 h,uint8 *name)
{
    send(0xaa);
    if(ID!=0)
	{
        send(ID);
	}
    send(0x55);
    send(x>>8);
    send(x);
    send(y>>8);
    send(y);
	send(w>>8);
    send(w);
    send(h>>8);
    send(h);
	while(*name!='\0')
    {
        send(*name++);             
    }
    send(0X0d);
}

void touch_reset()
{
    send(0xaa);
    send(0x72);
}

void touch_press_down()
{
    send(0xaa);
    send(0x70);
}

void touch_loosen()
{
    send(0xaa);
    send(0x1);
}


#endif

