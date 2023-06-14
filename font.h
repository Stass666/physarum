#include "fontgra.h"
#include <stdint.h>
double kf=(float)HIG/(float)WID;

unsigned char ss(uint8_t s, float x, float y, float r, float g, float b, float size){
	unsigned char symbol;
	unsigned char  FONTX=font[0];
	unsigned char  FONTY=font[1];
	unsigned char  xpixels=0;
	s-=font[2];
//	s*=2;
	
	glColor3f (r, g, b);
	glBegin (GL_POINTS);

		for(int j=0;j<FONTY;j+=1){
			for(int i=0;i<FONTX;i+=1){
				if(font[4+s*FONTY*FONTX/8+j*FONTX/8+i/8] & 1<<7-i%8){
					if(i>xpixels)xpixels=i;
			
	 		

					glVertex2f (x+size*i*kf, y-size*j);
	
					

				}
			}
		}
		
	glEnd ();
	if(s==0)xpixels+=font[0]/2;
	return xpixels;
}


void prnum(int s,float x,float y, float r, float g, float b, float size){
	unsigned int bias=0;
	glPointSize(HIG*(size+0.001)/2);
	int d=1,t=s;
	while(t>9){
		t/=10;
		d++;
	}
	for(int i=d-1;i>=0;i--){
		t=s%((int)pow(10,i+1));
		t/=(pow(10,i));
//		ss(t+48,x+(float)(d-i-1)*size*8*kf*font[0]/8,y,r,g,b,size);
		bias+=2+ss(t+48,x+(float)bias*size*kf,y,r,g,b,size);
	}
}

void prnstr(const char* s,float x,float y, float r, float g, float b, float size){
	unsigned int bias=0;
	glPointSize(HIG*(size+0.001)/2);
//		glPointSize(size+0.1);
	unsigned char t;
	for(int i=0;i<strlen(s);i++){
		t=s[i];
		bias+=2+ss(t,x+(float)bias*size*kf,y,r,g,b,size);
	}
}


uint8_t sizeofchar(uint8_t s){
	unsigned char symbol;
	unsigned char  FONTX=font[0];
	unsigned char  FONTY=font[1];
	uint8_t  xpixels=0;
	s-=font[2];
		for(int j=0;j<FONTY;j+=1){
			for(int i=0;i<FONTX;i+=1){
				if(font[4+s*FONTY*FONTX/8+j*FONTX/8+i/8] & 1<<7-i%8){
					if(i>xpixels)xpixels=i;
				}
			}
		}
	if(s==0)xpixels+=font[0]/2;
	return xpixels;
}

uint16_t sizeofstr(const char* s){
	uint16_t bias=0;
	for(int i=0;i<strlen(s);i++){
		bias+=2+sizeofchar(s[i]);
	}
	return bias;
}




