#ifndef _REALLY_H_
#define _REALLY_H_
#include <Include.h>

#ifdef BOARD_REALLY

//#define send USART1_SendByte 
extern void ReallyCreate_Init(uint8 *P);
uint8 ReallyGetLength(void);
extern void display_ID(void);
extern void reset(void);
extern void modify_ID(uint8 oid,uint8 nid);
extern void clear_display(uint8 ID);
extern void display_on(uint8 ID);
extern void display_off(uint8 ID);
extern void baud(uint8 ID,uint32 n);

extern void foreground_color(uint8 ID,uint8 r,uint8 g,uint8 b);
extern void background_color(uint8 ID,uint8 r,uint8 g,uint8 b);
extern void background_transparen(uint8 ID);
extern void background_nor_transparen(uint8 ID);
extern void cusor_xy(uint8 ID,uint16 x,uint16 y);
extern void simplified_chinese(uint8 ID);
extern void traditional_chinese(uint8 ID);
extern void japanese(uint8 ID);
extern void korean(uint8 ID);
extern void font_size_16(uint8 ID);
extern void font_size_24(uint8 ID);
extern void font_size_32(uint8 ID);
extern void line_spacing(uint8 ID,uint8 n);
extern void row_spacing(uint8 ID,uint8 n);
extern void print_string(uint8 ID,uint8 *ptr);
extern void print_string_xy(uint8 ID,uint16 x,uint16 y,uint8 *ptr);
extern void print_uint32(uint8 ID,uint32 n);
extern void print_uint32_xy(uint8 ID,uint16 x,uint16 y,uint32 n);

extern void draw_pixel(uint8 ID,uint16 x,uint16 y,uint8 r,uint8 g,uint8 b);
extern void draw_line(uint8 ID,uint16 x1,uint16 y1,uint16 x2,uint16 y2,uint8 r,uint8 g,uint8 b);
extern void draw_rectangle(uint8 ID,uint16 x,uint16 y,uint16 w,uint16 h,uint8 r,uint8 g,uint8 b);
extern void draw_rectangle_fill(uint8 ID,uint16 x,uint16 y,uint16 w,uint16 h,uint8 r,uint8 g,uint8 b);
extern void draw_circle_square(uint8 ID,uint16 x,uint16 y,uint16 w,uint16 h,uint16 ra,uint8 r,uint8 g,uint8 b);
extern void draw_circle_square_fill(uint8 ID,uint16 x,uint16 y,uint16 w,uint16 h,uint16 ra,uint8 r,uint8 g,uint8 b);
extern void draw_circle(uint8 ID,uint16 x,uint16 y,uint16 ra,uint8 r,uint8 g,uint8 b);
extern void draw_circle_fill(uint8 ID,uint16 x,uint16 y,uint16 ra,uint8 r,uint8 g,uint8 b);
extern void draw_ellipse(uint8 ID,uint16 x,uint16 y,uint16 xra,uint16 yra,uint8 r,uint8 g,uint8 b);
extern void draw_ellipse_fill(uint8 ID,uint16 x,uint16 y,uint16 xra,uint16 yra,uint8 r,uint8 g,uint8 b);

extern void picture_xy(uint8 ID,uint8 *filename,uint16 x,uint16 y);
extern void pitrure_spread(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s);
extern void picture_right_to_left(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s);
extern void picture_left_to_right(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s);
extern void picture_up_to_down(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s);
extern void picture_down_to_up(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s);
extern void picture_horizontally(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s);
extern void picture_vertical(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint8 s);
extern void picture_clip(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint16 lx,uint16 ly,uint16 w,uint16 h);
extern void picture_zom(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint16 w,uint16 h);
extern void picture_best_show(uint8 ID,uint8 *filename,uint16 x,uint16 y,uint16 w,uint16 h);

extern void load_buffer(uint8 ID,uint8 *filename,uint8 n);
extern void unload_buffer(uint8 ID,uint8 n);
extern void buffer_xy(uint8 ID,uint8 n,uint16 x,uint16 y);
extern void buffer_spread(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s);
extern void buffer_right_to_left(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s);
extern void buffer_left_to_right(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s);
extern void buffer_up_to_down(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s);
extern void buffer_down_to_up(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s);
extern void buffer_horizontally(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s);
extern void buffer_vertical(uint8 ID,uint8 n,uint16 x,uint16 y,uint8 s);
extern void buffer_clip(uint8 ID,uint8 n,uint16 x,uint16 y,uint16 lx,uint16 ly,uint16 w,uint16 h);
extern void buffer_zom(uint8 ID,uint8 n,uint16 x,uint16 y,uint16 w,uint16 h);
extern void buffer_best_show(uint8 ID,uint8 n,uint16 x,uint16 y,uint16 w,uint16 h);
extern void buffer_autodisplay(uint8 ID,uint8 n1,uint8 n2,uint16 x,uint16 y);
extern void buffer_autodisplay_speed(uint8 ID,uint8 n);
extern void buffer_autodisplay_stop(uint8 ID);

extern void windows(uint8 ID, uint16 x,uint16 y,uint16 w,uint16 h,uint8 *name);
extern void windows_click(uint8 ID, uint16 x,uint16 y,uint16 w,uint16 h);
extern void button(uint8 ID, uint16 x,uint16 y,uint16 w,uint16 h);
extern void button_click(uint8 ID, uint16 x,uint16 y,uint16 w,uint16 h);
extern void textbox(uint8 ID, uint16 x,uint16 y,uint16 w,uint16 h);
extern void frame(uint8 ID, uint16 x,uint16 y,uint16 w,uint16 h,uint8 *name);

extern void touch_reset(void);
extern void touch_press_down(void);
extern void touch_loosen(void);











#endif


#endif
