#define RAST 20
#define RES 5
#define MAX 10000

#define WID 800
#define HIG 800
#define BOTSIZE 2
#define SPD 0.003
#define PI 3.14159265
#include <ctime>
#include <windows.h>
#include <cmath>
#include <GL/gl.h> 
#include <stdio.h>  
#include "font.h"



RECT r;

HWND hwnd;
HDC hdc;
HGLRC hrc;
POINT pt;
double ferromons[WID][HIG],fade_speed=0.0070f;	
void EnableOpenGL (HWND hwnd, HDC *hdc, HGLRC *hrc);
void DisableOpenGL (HWND hwnd, HDC hdc, HGLRC hrc);
double col[MAX+1][3],tmpd,tmpd2,BLIZOST=(SPD*RAST)*(SPD*RAST),intensivity,ant_angle=.1,turn_angle=.1;

void Mir();
void Run();


float moux,mouy;
int population,bgr=0,tmpi,ferx,fery;
bool bQuit = false,q=true,save=false,training=true,show_one;
long iter=0;

//Timer t = Timer();
double get_pixel(int x,int y){
	if(x<0)x+=WID;
	if(y<0)y+=HIG;
	if(x>=WID)x-=WID;
	if(y>=HIG)y-=HIG;
	return ferromons[x][y];
}

void set_pixel(int x,int y, double f){
	if(x<0)x+=WID;
	if(y<0)y+=HIG;
	if(x>=WID)x-=WID;
	if(y>=HIG)y-=HIG;
	ferromons[x][y]+=f;
}

double how_ferromons(double x,double y){
	double fer,ix,iy,ku,kd,kl,kr;
	int dx,dy;
	ix=(x+1.0f)*(double)WID/2.0f;
	iy=(y+1.0f)*(double)HIG/2.0f;
	dx=ix;dy=iy;

	kl=ix-dx;kr=1-kl;
	ku=iy-dy;kd=1-ku;
	
	fer =get_pixel(dx+0,dy+0)*kr*kd;
	fer+=get_pixel(dx+0,dy+1)*kr*ku;
	fer+=get_pixel(dx+1,dy+0)*kl*kd;
	fer+=get_pixel(dx+1,dy+1)*kl*ku;
	
	return fer;

}

void set_ferromons(double x,double y){
	double fer,ix,iy,ku,kd,kl,kr;
	int dx,dy;
	ix=(x+1)*(double)WID/2.0f;
	iy=(y+1)*(double)HIG/2.0f;
	dx=ix;dy=iy;

	kl=ix-dx;kr=1-kl;
	ku=iy-dy;kd=1-ku;
	
	set_pixel(dx+0,dy+0,.2f*kr*kd);
	set_pixel(dx+0,dy+1,.2f*kr*ku);
	set_pixel(dx+1,dy+0,.2f*kl*kd);
	set_pixel(dx+1,dy+1,.2f*kl*ku);

}


void sp(double x,double y, int c,double size){
	glPointSize(size);	
	glBegin (GL_POINTS);

	glColor3f (col[c][0], col[c][1], col[c][2]);
	glVertex2f (x, y);
	glEnd ();				
}

double rast(double x1,double y1,double x2,double y2){
	return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}


struct Bot{
		double x,y,vector,turn_always,speed;

};

Bot zm[MAX];


double vec(double x1,double y1,double x2,double y2){
	double angle = atan((y2-y1)/(x2-x1));
	if(x2<x1)angle+=PI;
	return angle;
}



void step(int n){
	double f[3],ferms,mferms;
	byte maxferms,minferms;
			
			maxferms=0;minferms=0;

			f[0]=how_ferromons(cos(zm[n].vector)*zm[n].speed+zm[n].x,sin(zm[n].vector)*zm[n].speed+zm[n].y);
			f[1]=how_ferromons(cos(zm[n].vector+ant_angle)*zm[n].speed+zm[n].x,sin(zm[n].vector+ant_angle)*zm[n].speed+zm[n].y);
			f[2]=how_ferromons(cos(zm[n].vector-ant_angle)*zm[n].speed+zm[n].x,sin(zm[n].vector-ant_angle)*zm[n].speed+zm[n].y);

			ferms=f[0];

			
			if(f[1]>ferms){maxferms=1;ferms=f[1];}
			if(f[2]>ferms){maxferms=2;ferms=f[2];}

			if(ferms>4.0f){
				mferms=f[0];
				if(f[1]<mferms){minferms=1;mferms=f[1];}
				if(f[2]<mferms){minferms=2;mferms=f[2];}
				maxferms=minferms;
			}
			
			

			set_ferromons(zm[n].x,zm[n].y);
			
			if(maxferms==1)zm[n].vector+=turn_angle;
			if(maxferms==2)zm[n].vector-=turn_angle;
			zm[n].vector+=zm[n].turn_always;

			zm[n].x+=cos(zm[n].vector)*zm[n].speed;
			zm[n].y+=sin(zm[n].vector)*zm[n].speed;

					
				
			if(zm[n].x> 1)zm[n].x-=2;
			if(zm[n].x<-1)zm[n].x+=2;
			if(zm[n].y> 1)zm[n].y-=2;
			if(zm[n].y<-1)zm[n].y+=2;
if(show_one)			sp(zm[n].x,zm[n].y,0,BOTSIZE);
			

//			zm[n].vector+=(rand()/327680.0f-0.05)/10000.0f;
			while(zm[n].vector<0)zm[n].vector+=PI*2;
			while(zm[n].vector>PI*2)zm[n].vector-=PI*2;				

}







	







LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {

	case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        switch (wParam){
		case VK_ESCAPE:PostQuitMessage(0);return 0;
		case 32:show_one=!show_one;
		case 'q'-32:ant_angle +=0.1;if(ant_angle>2*PI)ant_angle-=2*PI;return 0;
		case 'a'-32:ant_angle -=0.1;if(ant_angle<0)ant_angle+=2*PI;return 0;
		case 'w'-32:turn_angle+=0.1;if(turn_angle>2*PI)turn_angle-=2*PI;return 0;
		case 's'-32:turn_angle-=0.1;if(turn_angle<0)turn_angle+=2*PI;return 0;
		case 'e'-32:fade_speed+=0.0001;return 0;
		case 'd'-32:fade_speed-=0.0001;if(fade_speed<=0)fade_speed=0.0001;return 0;

        }
        return 0;	
		
			case WM_MOUSEWHEEL:
				for(int i=0;i<MAX;i++){
					zm[i].speed+=((double)GET_WHEEL_DELTA_WPARAM(wParam))*0.000001f;
					if(zm[i].speed<=0)zm[i].speed=SPD;
					
				}

				return 0;	
		
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	MSG msg;

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.lpszClassName = "WindowClass";
	RegisterClassEx(&wc);
	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","Caption", WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		20, 20, WID, HIG, NULL,NULL,NULL,NULL);
	EnableOpenGL (hwnd, &hdc, &hrc);





	Mir();


	EnableOpenGL (hwnd, &hdc, &hrc);		
		

	while (!bQuit)if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)){
	
		if (msg.message == WM_QUIT){
			bQuit = true;
		}else{
			TranslateMessage (&msg);
			DispatchMessage (&msg);
        }
	}else{
		moux=((float)pt.x/(WID/2))-1;
		mouy=1-((float)pt.y/(HIG/2));
//		glClearColor (1.0f, 1.0f, 1.0f, 0.0f);		
		glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
		glClear (GL_COLOR_BUFFER_BIT);
//		glEnable( GL_POINT_SMOOTH );
		glEnable( GL_BLEND );
		glPushMatrix ();
		//	glRotatef (theta, 0.2f, 0.3f, 0.3f);
		
//		glBegin (GL_QUADS);
		
			GetCursorPos(&pt);
			ScreenToClient (hwnd, &pt);
			Run();
			
//		glEnd ();
		glPopMatrix ();
		SwapBuffers (hdc);

        //    theta += 0.05f;

		
		
		
		
		

	}
	
	DisableOpenGL (hwnd, hdc, hrc);
	return msg.wParam;
}




	
void Run(){
//	glPointSize(size);	
	glBegin (GL_POINTS);
		for(fery=0;fery<HIG;fery++)for(ferx=0;ferx<WID;ferx++){
			intensivity=ferromons[ferx][fery];
			glColor3f(intensivity, intensivity, intensivity);
			if(!show_one)			glVertex2f(ferx*2/(double)WID-1.0f, fery*2/(double)HIG-1.0f);
			if(ferromons[ferx][fery]>0)ferromons[ferx][fery]-=fade_speed;
		}
			
			
	glEnd ();	
	
	
  for(int i=0;i<MAX;i++)step(i);
	prnum(zm[0].speed*100000,					-.99, .999, 1, 1, 1, 0.0005);
	prnum(ant_angle*10,								-.99, .950, 1, 1, 1, 0.0005);
	prnum(turn_angle*10,							-.99, .900, 1, 1, 1, 0.0005);
	prnum(fade_speed*10000,						-.99, .850, 1, 1, 1, 0.0005);
	
	
/*	for(int i=0;i<MAX;i++){
		glVertex2f(ferx*2/(double)WID-1.0f, fery*2/(double)HIG-1.0f);
				intensivity=(double)i/(double)WID/2.0f-1.0f;
				glBegin (GL_QUADS);
				glVertex2f (intensivity,					0);
				glVertex2f (intensivity+0.001,		0);
				glVertex2f (intensivity+0.001,		zm[i].vector/6.0f);
				glVertex2f (intensivity,					zm[i].vector/6.0f);
				glEnd ();	
	}*/
	
	
  
}

void Mir(){
	srand(time(0));
	for(int i=0;i<MAX;i++){for(int j=0;j<3;j++)col[i][j]=rand()/65536.0f;}
	for(int j=0;j<3;j++)col[0][j]=0.0f;col[0][0]=1.0f;
	for(int j=0;j<3;j++)col[1][j]=0.0f;col[1][2]=1.0f;
	
	for(int j=0;j<3;j++)col[MAX][j]=0.9;
	for(int i=0;i<MAX;i++){
		zm[i].x=(double)rand()/16384-1;
		zm[i].y=(double)rand()/16384-1;
		zm[i].vector=(double)rand()/16384*PI;
		zm[i].speed=(float)rand()/327680.0f*SPD+SPD;		
		ant_angle=.2;
		turn_angle=.3;
		zm[i].turn_always=(rand()/32768.0f-0.5)/50.0f;
	}
	
	for(fery=48;fery<102;fery++)for(ferx=102;ferx<400;ferx++){
		ferromons[ferx][fery]=.2;
		
		
	}
	
}







void EnableOpenGL (HWND hwnd, HDC *hdc, HGLRC *hrc)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hdc = GetDC (hwnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hdc, &pfd);
    SetPixelFormat (*hdc, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hrc = wglCreateContext( *hdc );
    wglMakeCurrent( *hdc, *hrc );

}


void DisableOpenGL (HWND hwnd, HDC hdc, HGLRC hrc)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hrc);
    ReleaseDC (hwnd, hdc);
}



