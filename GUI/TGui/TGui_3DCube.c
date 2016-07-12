//
// 3D Cube by Jason Wright (C)opyright Pyrofer 2006
//
// This code is free, there is no charge for it nor should anybody else charge
// for the code, its distribution or derivitives of this code.
//
// You may use this code, modify and change and improve upon it on the following conditions,
//
// 1> You send me the modified source code and where possible make it public

#include <include.h>               	// include complex math routines
#include <TGui_3DCube.h>
#include <Gui_include.h>
#include <Driver_Include.h>

#define DOTCOUNT 8
#define LINECOUNT 12
const int16 aa[DOTCOUNT]={10,-10,-10,10,   10,-10,-10,10};	// x data for shape vertex
const int16 bb[DOTCOUNT]={10,10,-10,-10,   10,10,-10,-10};	// y data for shape vertex
const int16 cc[DOTCOUNT]={-10,-10,-10,-10, 10,10,10,10};		// z data for shape vertex

const int16 ff[LINECOUNT]={1,2,3,4,  5,6,7,8, 1,2,3,4};                    // start vertex for lines
const int16 gg[LINECOUNT]={2,3,4,1,  6,7,8,5, 5,6,7,8};                // end vertex for lines

int sx,sy,ex,ey;                	// define global vars for calling graphics subroutines


const Coordinate_3D C3D_Default={LCD_XSIZE-25,25,80,40};
 
void TGui_3DCobe(uint16 OFFSETX,uint16 OFFSETY,uint16 OFFSETZ,uint16 Delay)                     	// routine to draw and calc 3d cube
{
	BOOL Draw=True;
	int newx[(sizeof(aa)/2)];                   	// translated screen x co-ordinates for vertex
	int newy[(sizeof(aa)/2)];                   	// translated screen y co-ordinates for vertex
	int i,loop;                    	// temp variable for loops
	float xt,yt,zt,x,y,z,sinax,cosax,sinay,cosay,sinaz,cosaz,vertex;  // lots of work variables
	float xpos=0;				// position for object in 3d space, in x
	float ypos=0;				// y
	float zpos=0;				// and z values
	float rotx=0;                  	// starting amount of x rotation
	float roty=0;                 		// starting amount of y rotation
	float rotz=0;                		// starting amount of z rotation

	for (loop=0; loop<=201; loop++)	// rotate the cube 100 times
	{
	xpos=xpos+0.0;			// move the object
	ypos=ypos+0.0;			// it would wander off screen
	zpos=zpos+0.0;			// really quick, so leave it centered

if(Draw)
{
	rotx=rotx+0.1;                	// rotate the cube on X axis
	roty=roty+0.1;                	// and on its y axis
	rotz=rotz+0.1;                	// dont bother with z or it gets confusing
}
	sinax=sin(rotx);			// precalculate the sin and cos values
	cosax=cos(rotx);			// for the rotation as this saves a 

	sinay=sin(roty);			// little time when running as we
	cosay=cos(roty);			// call sin and cos less often

	sinaz=sin(rotz);			// they are slow routines
	cosaz=cos(rotz);			// and we dont want slow!

	for (i=0; i<DOTCOUNT; i++)           	// translate 3d vertex position to 2d screen position
	{
		x=aa[i];                	// get x for vertex i
		y=bb[i];                	// get y for vertex i
		z=cc[i];                	// get z for vertex i

		yt = y * cosax - z * sinax;	// rotate around the x axis
		zt = y * sinax + z * cosax;	// using the Y and Z for the rotation
		y = yt;
		z = zt;

		xt = x * cosay - z * sinay;	// rotate around the Y axis
		zt = x * sinay + z * cosay;	// using X and Z
		x = xt;
		z = zt;

		xt = x * cosaz - y * sinaz;	// finaly rotate around the Z axis
		yt = x * sinaz + y * cosaz;	// using X and Y
		x = xt;
		y = yt;

		x=x+xpos;			// add the object position offset
		y=y+ypos;			// for both x and y
		z=z+OFFSETZ-zpos;		// as well as Z

		newx[i]=(x*64/z)+OFFSETX;	// translate 3d to 2d coordinates for screen
		newy[i]=(y*64/z)+OFFSETY;	// drawing so we can see the cube
	}

	for (i=0; i<LINECOUNT; i++)		// draw the lines that make up the object
	{
		vertex=ff[i]-1;         // temp = start vertex for this line
		sx=newx[(int)vertex];        // set line start x to vertex[i] x position
		sy=newy[(int)vertex];        // set line start y to vertex[i] y position
		vertex=gg[i]-1;         // temp = end vertex for this line
		ex=newx[(int)vertex];	// set line end x to vertex[i+1] x position
		ey=newy[(int)vertex];	// set line end y to vertex[i+1] y position
		if(Draw)TGui_Draw_Line(sx, sy, ex, ey,P_TGui_Style->F_Color);
		else TGui_Draw_Line(sx, sy, ex, ey,P_TGui_Style->B_Color);
	}
	if(Draw)Tos_TaskDelay(Delay);
	Draw=(BOOL)!Draw;
	}
}
void TGui_3DCobeTest()
{
	TGui_3DCobe(LCD_XSIZE-25,25,80,40);
}





///* 定义显示位置 */
//#define OFFSETX  200              
//#define OFFSETY  80             
//#define OFFSETZ  20

///////////////////////////////////立方体加文字///////////////////////////////////////////////////////////////////////////
//const signed int aa[23]={8,-8,-8,8,8,-8,-8,8,8,8,8,8,8,0,4,-4,-8,-8,-8,-8,-8,-8,-8}; // x 
//const signed int bb[23]={8,8,-8,-8,8,8,-8,-8,0,-4,4,-2,2,8,8,8,4,4,4,-4,-4,-4,0};    // y 
//const signed int cc[23]={-8,-8,-8,-8,8,8,8,8,6,-6,-6,0,0,-6,6,6,-6,0,6,6,0,-6,0};    // z

//const int ff[22]={1,2,3,4,5,6,7,8,1,2,3,4,9,9,12,14,14,17,19,20,21,22};              // start
//const int gg[22]={2,3,4,1,6,7,8,5,5,6,7,8,10,11,13,15,16,19,20,21,18,23};         // end
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///* 定义LCD图像显示缓冲区 */
//extern unsigned char displaybuffer[8][128];

//int sx,sy,ex,ey;                             // define global vars for calling graphics subroutines
//float rotx=0.0;                               // starting amount of x rotation
//float roty=0.0;                               // starting amount of y rotation
//float rotz=0.0;                               // starting amount of z rotation

//void Disply3D(float xpos,float ypos,float zpos); 

///**************************************************************************
//*                          3D显示主程序
//***************************************************************************/
//void Disply3D(float xpos,float ypos,float zpos)     
//{
//    int newx[23];                               // translated screen x co-ordinates for vertex
//    int newy[23];                               // translated screen y co-ordinates for vertex
//    int i,loop;                                    // temp variable for loops
//    int vertex;
//    float xt,yt,zt,x,y,z,sinax,cosax,sinay,cosay,sinaz,cosaz;     // lots of work variables
//    xpos=xpos+0.0;                      // move the object
//    ypos=ypos+0.0;                     // it would wander off screen
//    zpos=zpos+0.0;                      // really quick, so leave it centered
//    rotx=rotx+0.02;                        // rotate the cube on X axis
//    roty=roty+0.03;                        // and on its y axis
//    rotz=rotz+0.01;                        // dont bother with z or it gets confusing

//   sinax=sin(rotx);                     // precalculate the sin and cos values
//   cosax=cos(rotx);                  // for the rotation as this saves a 
//  
//   sinay=sin(roty);                    // little time when running as we
//   cosay=cos(roty);                  // call sin and cos less often
//  
//   sinaz=sin(rotz);                    // they are slow routines
//   cosaz=cos(rotz);                 // and we dont want slow!

//    for (i=0; i<23; i++)              // translate 3d vertex position to 2d screen position
//    {
//        x=aa[i];                            // get x for vertex i
//        y=bb[i];                            // get y for vertex i
//        z=cc[i];                             // get z for vertex i

//        yt = y * cosax - z * sinax;            // rotate around the x axis
//        zt = y * sinax + z * cosax;           // using the Y and Z for the rotation
//        y = yt;
//        z = zt;

//        xt = x * cosay - z * sinay;           // rotate around the Y axis
//        zt = x * sinay + z * cosay;          // using X and Z
//        x = xt;
//        z = zt;

//        xt = x * cosaz - y * sinaz;            // finaly rotate around the Z axis
//        yt = x * sinaz + y * cosaz;           // using X and Y
//        x = xt;
//        y = yt;

//        x=x+xpos;                                    // add the object position offset
//        y=y+ypos;                                    // for both x and y
//        z=z+OFFSETZ-zpos;                 // as well as Z

//        newx[i]=(x*64/z)+OFFSETX;          // translate 3d to 2d coordinates for screen
//        newy[i]=(y*64/z)+OFFSETY;          // drawing so we can see the cube
//  }
//	Tos_TaskDelay(20);
//	LCD_Clear_Screen(Color_Black);
//    for (i=0; i<22; i++)                // draw the lines that make up the object
//    {
//       vertex=ff[i]-1;                      // temp = start vertex for this line
//       sx=newx[vertex];               // set line start x to vertex[i] x position
//       sy=newy[vertex];               // set line start y to vertex[i] y position
//       vertex=gg[i]-1;                   // temp = end vertex for this line
//       ex=newx[vertex];               // set line end x to vertex[i+1] x position
//       ey=newy[vertex];               // set line end y to vertex[i+1] y position
//       if(sx>255 | sy>255 | ex>255 | ey>255)
//       {
//           sx=255;
//           sy=255;
//           ex=255;
//           ey=255;
//       }
//       TGui_Draw_Line(sy, sx, ey, ex, Color_Red);
//    }
//    for (i=0; i<22; i++)                                    
//    {
//        vertex=ff[i]-1;      
//        sx=newx[vertex];       
//        sy=newy[vertex];  
//        vertex=gg[i]-1;      
//        ex=newx[vertex]; 
//        ey=newy[vertex];
//    if(sx>255 | sy>255 | ex>255 | ey>255)
//    {
//        sx=255;
//        sy=255;
//        ex=255;
//        ey=255;
//    }
//    TGui_Draw_Line(sy, sx, ey, ex, Color_Red);
//		
//    }
// }
