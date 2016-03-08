#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef WIN32
	#include <windows.h>
	#include <GL\gl.h>
	#include <GL\glu.h>
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else //UNIX
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include "Platform.h"
#include "Lib.h"
#include "GameFiles.h"

const int texmax=24, fontmax=3;
int **cnv[512], fontdata[fontmax][3], cnvd[512][2];
const int xmax=320, ymax=200;
int	ocnv[ymax][xmax];

char tex_list[texmax][64], font_list[fontmax][64];
char tex_path[] = TEXLIST_DAT;
char font_path[] = FONLIST_DAT;
char tex_mpath[] = TEXTURE_PATH;

/******************************
 ***********SYS LEVEL**********
 ******************************/
void reset(int id)
{
	for (int i=0; i<cnvd[id][0]; i++)
		for (int j=0; j<cnvd[id][1]; j++)
			cnv[id][i][j]=0;
}

void delLayer(int id)
{
	if (cnvd[id][0] == 0) return;
    for(int i = 0; i < cnvd[id][0]; i++)
        delete[] cnv[id][i];
    delete[] cnv[id];
	cnvd[id][0] = 0;
	cnvd[id][1] = 0;
}

void buildLayer(int id,const int x,const int y)
{
	if (cnvd[id][0] != 0) delLayer(id);
    cnv[id] = new int*[x];
    for(int i = 0; i < x; ++i)
        cnv[id][i] = new int[y];
	cnvd[id][0] = x;
	cnvd[id][1] = y;
	reset(id);
}

void destroy()
{
	for (int i=0; i<512; i++) delLayer(i);
}

int getWidth(int id)
{
	return cnvd[id][0];
}

int getHeight(int id)
{
	return cnvd[id][1];
}

unsigned int ARGB(unsigned int a,unsigned int r,unsigned int g,unsigned int b)
{
	return (a << 24) | (b << 16) | (g << 8) | r;
}

unsigned int tRGB(unsigned int r,unsigned int g,unsigned int b)
{
	return (0xFF << 24) | (b << 16) | (g << 8) | r;
}

void RGBt(unsigned int color, unsigned int *r,unsigned int *g,unsigned int *b)
{
	*b=((color >> 16) & 255);
	*g=((color >> 8) & 255);
	*r=(color & 255);
}

void ARGBt(unsigned int color, unsigned int *a, unsigned int *r,unsigned int *g,unsigned int *b)
{
	*a=((color >> 24) & 255);
	*b=((color >> 16) & 255);
	*g=((color >> 8) & 255);
	*r=(color & 255);
}
void RGBt(unsigned int color, int *r, int *g, int *b)
{
	*b=((color >> 16) & 255);
	*g=((color >> 8) & 255);
	*r=(color & 255);
}

void ARGBt(unsigned int color, int *a, int *r, int *g, int *b)
{
	*a=((color >> 24) & 255);
	*b=((color >> 16) & 255);
	*g=((color >> 8) & 255);
	*r=(color & 255);
}

/******************************
 ***********LOW LEVEL**********
 ******************************/


/*

  Low level ToDo:
	
	  +Grayscalling

*/

void invert(int id, int x, int y)
{
	if (x>=cnvd[id][0] || x<0 || y>=cnvd[id][1] || y<0) return;
	
	unsigned int da,dr,dg,db;

	ARGBt(cnv[id][x][y],&da,&dr,&dg,&db);

	dr = 255-dr;
	dg = 255-dg;
	db = 255-db;

	cnv[id][x][y]= ARGB(da,dr,dg,db);
}
	
void add(int id, int x,int y,int r,int g,int b)
{
	if (x>=cnvd[id][0] || x<0 || y>=cnvd[id][1] || y<0) return;
	
	unsigned int dr,dg,db,da;
	ARGBt(cnv[id][x][y],&da,&dr,&dg,&db);
	if (da==0) return;
	
	dr+=r;
	if (dr>255) dr=255;
	dg+=g;
	if (dg>255) dg=255;
	db+=b;
	if (db>255) db=255;
	
	cnv[id][x][y]= ARGB(da,dr,dg,db);
}
	
void add(int id, int x,int y,int a,int r,int g,int b)
{
	if (x>=cnvd[id][0] || x<0 || y>=cnvd[id][1] || y<0) return;
	
	unsigned int dr,dg,db,da;
	ARGBt(cnv[id][x][y],&da,&dr,&dg,&db);
	if (da==0) return;
	
	dr=dr + ((r * a) >> 8);
	if (dr>255) dr=255;
	dg=dg + ((g * a) >> 8);
	if (dg>255) dg=255;
	db=db + ((b * a) >> 8);
	if (db>255) db=255;
	
	cnv[id][x][y]= ARGB(da,dr,dg,db);
}
	
void mult(int id, int x,int y,int r,int g,int b)
{
	if (x>=cnvd[id][0] || x<0 || y>=cnvd[id][1] || y<0) return;
	
	unsigned int dr,dg,db,da;
	ARGBt(cnv[id][x][y],&da,&dr,&dg,&db);
	if (da==0) return;
	
	dr=(( dr * r) >> 8) & 255;
	dg=(( dg * g) >> 8) & 255;
	db=(( db * b) >> 8) & 255;
	
	cnv[id][x][y]= ARGB(da,dr,dg,db);
}
	
void mult(int id, int x,int y,int a,int r,int g,int b)
{
	if (x>=cnvd[id][0] || x<0 || y>=cnvd[id][1] || y<0) return;

	unsigned int dr=255 - (((255 - r) * a) >> 8);
	unsigned int dg=255 - (((255 - g) * a) >> 8);
	unsigned int db=255 - (((255 - b) * a) >> 8);
	mult(id,x,y,dr,dg,db);
}
	
void screening(int id, int x,int y,int r,int g,int b)
{
	if (x>=cnvd[id][0] || x<0 || y>=cnvd[id][1] || y<0) return;
	
	int dr,dg,db,da;
	ARGBt(cnv[id][x][y],&da,&dr,&dg,&db);
	if (da==0) return;
	
	if (dr<r) dr=r;
	if (dg<g) dg=g;
	if (db<b) db=b;
	
	cnv[id][x][y]= ARGB(da,dr,dg,db);
}


void plot(int id, int x,int y,int r,int g,int b)
{
	if (x>=cnvd[id][0] || x<0 || y>=cnvd[id][1] || y<0) return;
	cnv[id][x][y]= tRGB(r,g,b);
}

void plot(int id, int x,int y,unsigned int color)
{
	if (x>=cnvd[id][0] || x<0 || y>=cnvd[id][1] || y<0) return;
	cnv[id][x][y]= color;
}
	
void plot(int id,int x,int y, int a, int r,int g,int b)
{
	if (x>=cnvd[id][0] || x<0 || y>=cnvd[id][1] || y<0) return;
	
	unsigned int dr,dg,db,da;
	ARGBt(cnv[id][x][y],&da,&dr,&dg,&db);
	
	dr=( (dr * (255 - a) ) + (r * a)) >> 8;
	dg=( (dg * (255 - a) ) + (g * a)) >> 8;
	db=( (db * (255 - a) ) + (b * a)) >> 8;
	da+=a;
	if (da>255) da=255;
	
	cnv[id][x][y]= ARGB(da,dr,dg,db);
}
	
void transparency(int id,int x,int y, int a)
{
	if (x>=cnvd[id][0] || x<0 || y>=cnvd[id][1] || y<0) return;
	
	unsigned int dr,dg,db;
	RGBt(cnv[id][x][y],&dr,&dg,&db);
	cnv[id][x][y]= ARGB(a,dr,dg,db);
}
	
	
void grayer(int id, int tid, int x,int y,int x1,int y1,int a)
{
	if (x1>=cnvd[id][0] || x<0 || y1>=cnvd[id][1] || y<0) return;
	
	int ddg,ddb,ddr, dr,dg,db,da,de;
	ARGBt(cnv[id][x1][y1],&da,&dr,&dg,&db);
	if (da==0) return;
	
	de = (dg+dr+db)/3;
	ddb = db-de;
	ddg = dg-de;
	ddr = dr-de;
	dr = (dr*127-(ddr*(a-127)))/127;
	db = (db*127-(ddb*(a-127)))/127;
	dg = (dg*127-(ddg*(a-127)))/127;
	
	if (dr>255) dr=255;
	if (dg>255) dg=255;
	if (db>255) db=255;
	
	if (dr<0) dr=0;
	if (dg<0) dg=0;
	if (db<0) db=0;
	
	cnv[tid][x][y]= ARGB(da,dr,dg,db);
	
}
	
/******************************
 **********HIGH LEVEL**********
 ******************************/
	
/*
	
  High level ToDo:
	
	  +Saturation Level
	  +Render Layer With Masc
	  -Render Text Field
	  -Zoom Rendering
	
*/
	
void DrawVLine(int id, unsigned int color, int x1, int y1, int k)
{
	unsigned int a,r,g,b;
	int i;
	ARGBt(color,&a,&r,&g,&b);
	for (i = 0; i<k; i++)
			plot(id,x1,y1+i,a,r,g,b);
}

void FDrawVLine(int id, unsigned int color, int x1, int y1, int k)
{
	int i;
	for (i = 0; i<k; i++)
			plot(id,x1,y1+i,color);
}
	
void DrawHLine(int id, unsigned int color, int x1, int y1, int k)
{
	unsigned int a,r,g,b;
	int i;
	ARGBt(color,&a,&r,&g,&b);
	for (i = 0; i<k; i++)
			plot(id,x1+i,y1,a,r,g,b);
}

void FDrawHLine(int id, unsigned int color, int x1, int y1, int k)
{
	int i;
	for (i = 0; i<k; i++)
			plot(id,x1+i,y1,color);
}
	
void DrawRect(int id, unsigned int color, int x1, int y1, int xk, int yk)
{
	unsigned int a,r,g,b;
	int i,j;
	ARGBt(color,&a,&r,&g,&b);
	for (i = 0; i<xk; i++)
		for (j = 0; j<yk; j++)
			plot(id,x1+i,y1+j,a,r,g,b);
}

void FDrawRect(int id, unsigned int color, int x1, int y1, int xk, int yk)
{
	int i,j;
	for (i = 0; i<xk; i++)
		for (j = 0; j<yk; j++)
			plot(id,x1+i,y1+j,color);
}
	
void DrawRect(int id, unsigned int color, int x1, int y1, int xk, int yk, int mode)
{
	unsigned int a,r,g,b;
	int i,j;
	ARGBt(color,&a,&r,&g,&b);
	for (i = 0; i<xk; i++)
		for (j = 0; j<yk; j++)
		{	
			if (a != 255) {
				if (mode == 0) plot(id,x1+i,y1+j,a,r,g,b);
				else if (mode == 1) add(id,x1+i,y1+j,a,r,g,b);
				else if (mode == 2) mult(id,x1+i,y1+j,a,r,g,b);
				else if (mode == 3) transparency(id,x1+i,y1+j,a);
				else if (mode == 4) invert(id,x1+i,y1+j);
			}
			else
				{
				if (mode == 0) plot(id,x1+i,y1+j,r,g,b);
				else if (mode == 1) add(id,x1+i,y1+j,r,g,b);
				else if (mode == 2) mult(id,x1+i,y1+j,r,g,b);
				else if (mode == 3) transparency(id,x1+i,y1+j,a);
				else if (mode == 4) invert(id,x1+i,y1+j);
			}
		}
}

void DoGray(int id, int tid, unsigned int value, int x1, int y1)
{
	unsigned int a,r,g,b;
	int i,j;
	for (i = 0; i<cnvd[id][0]; i++)
		for (j = 0; j<cnvd[id][1]; j++)
		{	
			ARGBt(cnv[id][i][j],&a,&r,&g,&b);
			if (a == 0) continue;
			grayer(id,tid,x1+i,y1+j,i,j,value);
		}
}
	
void VGradient(int id, unsigned int colorA, unsigned int colorB, int x1, int y1, int xk, int yk, int mode)
{
	int a1,r1,g1,b1,a2,r2,g2,b2,da,dr,dg,db;
	int i,j;
	ARGBt(colorA,&a1,&r1,&g1,&b1);
	ARGBt(colorB,&a2,&r2,&g2,&b2);
	
	for (j=0; j<yk; j++)
	{
		da = a1 + ((a2 - a1) * j / yk);
		dg = g1 + ((g2 - g1) * j / yk);
		db = b1 + ((b2 - b1) * j / yk);
		dr = r1 + ((r2 - r1) * j / yk);
		for (i=0; i<xk; i++)
		{
			if (mode == 0) plot(id, i+x1, j+y1, da, dr, dg, db);
			if (mode == 1) add(id, i+x1, j+y1, da, dr, dg, db);
			if (mode == 2) mult(id, i+x1, j+y1, da, dr, dg, db);
			if (mode == 3) transparency(id, i+x1, j+y1, da);
		}
	}
}
	
void HGradient(int id, unsigned int colorA, unsigned int colorB, int x1, int y1, int xk, int yk, int mode)
{
	int a1,r1,g1,b1,a2,r2,g2,b2,da,dr,dg,db;
	int i,j;
	ARGBt(colorA,&a1,&r1,&g1,&b1);
	ARGBt(colorB,&a2,&r2,&g2,&b2);
	
	for (i=0; i<xk; i++)
	{
		da = a1 + ((a2 - a1) * i / xk);
		dg = g1 + ((g2 - g1) * i / xk);
		db = b1 + ((b2 - b1) * i / xk);
		dr = r1 + ((r2 - r1) * i / xk);
		for (j=0; j<yk; j++)
		{
			if (mode == 0) plot(id, i+x1, j+y1, da, dr, dg, db);
			else if (mode == 1) add(id, i+x1, j+y1, da, dr, dg, db);
			else if (mode == 2) mult(id, i+x1, j+y1, da, dr, dg, db);
			else if (mode == 3) transparency(id, i+x1, j+y1, da);
		}
	}
}

/*

  Da, stasyan, pridetsya tebe podnyat' svoyu lenivuyu gopu i optimizirovat' normal'no kod.
  

*/
	
void IncludeLayer(int id, int tid, int x1, int y1, int mode)
{
	unsigned int a,r,g,b;
	int i,j,mx,my,nx,ny;

	if (x1<0) nx = -x1;
	else nx = 0;
	if (y1<0) ny = -y1;
	else ny = 0;
	
	if (cnvd[id][0]+x1>cnvd[tid][0]) mx = cnvd[tid][0]-x1;
	else mx = cnvd[id][0];
	if (cnvd[id][1]+y1>cnvd[tid][1]) my = cnvd[tid][1]-y1;
	else my = cnvd[id][1];

	for (i = nx; i<mx; i++)
		for (j = ny; j<my; j++)
		{	
			ARGBt(cnv[id][i][j],&a,&r,&g,&b);
			if (a == 0) continue;
			if (a != 255) 
			{
				if (mode == 0) plot(tid,x1+i,y1+j,a,r,g,b);
				else if (mode == 1) add(tid,x1+i,y1+j,a,r,g,b);
				else if (mode == 2) mult(tid,x1+i,y1+j,a,r,g,b);
			}
			else
			{
				if (mode == 0) plot(tid,x1+i,y1+j,cnv[id][i][j]);
				else if (mode == 1) add(tid,x1+i,y1+j,r,g,b);
				else if (mode == 2) mult(tid,x1+i,y1+j,r,g,b);
			}
		}
}
	
void FragmentLayer(int id, int tid, int tx, int ty, int x2, int y2, int xk, int yk, int mode)
{
	unsigned int a,r,g,b;
	int i,j,mx,my,nx,ny;

	if (tx<0) nx = -tx;
	else nx = 0;
	if (ty<0) ny = -ty;
	else ny = 0;

	if (xk+tx>cnvd[tid][0]) mx = cnvd[tid][0]-tx;
	else mx = xk;
	if (yk+ty>cnvd[tid][1]) my = cnvd[tid][1]-ty;
	else my = yk;

	for (i = nx; i<mx; i++)
		for (j = ny; j<my; j++)
		{	
			ARGBt(cnv[id][x2 + i][y2 + j],&a,&r,&g,&b);
			if (a == 0) continue;
			if (a != 255) 
			{
				if (mode == 0) plot(tid,tx+i,ty+j,a,r,g,b);
				else if (mode == 1) add(tid,tx+i,ty+j,a,r,g,b);
				else if (mode == 2) mult(tid,tx+i,ty+j,a,r,g,b);
			}
			else
			{
				if (mode == 0) plot(tid,tx+i,ty+j,cnv[id][x2 + i][y2 + j]);
				else if (mode == 1) add(tid,tx+i,ty+j,r,g,b);
				else if (mode == 2) mult(tid,tx+i,ty+j,r,g,b);
			}
		}
}

void TransformLayer(int id, int tid, int tx, int ty, int x2, int y2, int xk, int yk, int mode)
{
	/*

  modes:
  0 - none
  1 - invert vertical
  2 - invert horizontal
  3 - rotate 180
  4 - transponation //later
	*/
	unsigned int a,r,g,b;
	int i,j,mx,my,nx,ny,tdx, tdy, ax, ay;

	if (tx<0) nx = -tx;
	else nx = 0;
	if (ty<0) ny = -ty;
	else ny = 0;

	if ((mode & 4) == 4)
	{
		ax = yk;
		ay = xk;
	}
	else
	{
		ax = xk;
		ay = yk;
	}

	int bx = x2, by = y2;
	if ((mode & 4) == 4)
	{
		bx = y2;
		by = x2;
	}

	if (ax+tx>cnvd[tid][0]) mx = cnvd[tid][0]-tx;
	else mx = ax;
	if (ay+ty>cnvd[tid][1]) my = cnvd[tid][1]-ty;
	else my = ay;

	for (i = nx; i<mx; i++)
		for (j = ny; j<my; j++)
		{	
			if ((mode & 1) == 1) tdy=ay-j+by-1;
			else tdy=by + j;
			if ((mode & 2) == 2) tdx=ax-i+bx-1;
			else tdx=bx + i;
			if ((mode & 4) == 4) ARGBt(cnv[id][tdy][tdx],&a,&r,&g,&b);
			else ARGBt(cnv[id][tdx][tdy],&a,&r,&g,&b);
			if (a == 0) continue;
			if (a != 255) 
			{
				plot(tid,tx+i,ty+j,a,r,g,b);
			}
			else
			{
				if ((mode & 4) == 4) plot(tid,tx+i,ty+j,cnv[id][tdy][tdx]);
				else plot(tid,tx+i,ty+j,cnv[id][tdx][tdy]);
			}
		}
}

void TransformLayer(int id, int tid, int tx, int ty, int mode)
{
	/*
 0 6 3 5
  modes:
  0 - none
  1 - invert vertical
  2 - invert horizontal
  3 - rotate 180
  4 - invert diagonal
  5 - rotate left
  6 - rotate right
  7 - i dunno
	*/
	unsigned int a,r,g,b;
	int i,j,mx,my,nx,ny,tdx, tdy,ax,ay;

	if (tx<0) nx = -tx;
	else nx = 0;
	if (ty<0) ny = -ty;
	else ny = 0;

	if ((mode & 4) == 4)
	{
		ax = cnvd[id][1];
		ay = cnvd[id][0];
	}
	else
	{
		ax = cnvd[id][0];
		ay = cnvd[id][1];
	}
	
	if (ax+tx>cnvd[tid][0]) mx = cnvd[tid][0]-tx;
	else mx = ax;
	if (ay+ty>cnvd[tid][1]) my = cnvd[tid][1]-ty;
	else my = ay;

	for (i = nx; i<mx; i++)
		for (j = ny; j<my; j++)
		{	
			if ((mode & 1) == 1) tdy=ay-j-1;
			else tdy=j;
			if ((mode & 2) == 2) tdx=ax-i-1;
			else tdx=i;
			if ((mode & 4) == 4) ARGBt(cnv[id][tdy][tdx],&a,&r,&g,&b);
			else ARGBt(cnv[id][tdx][tdy],&a,&r,&g,&b);
			if (a == 0) continue;
			if (a != 255) 
			{
				plot(tid,tx+i,ty+j,a,r,g,b);
			}
			else
			{
				if ((mode & 4) == 4) plot(tid,tx+i,ty+j,cnv[id][tdy][tdx]);
				else plot(tid,tx+i,ty+j,cnv[id][tdx][tdy]);
			}
		}
}
	
void PrintText(int id, int fontn, char *str, int color, int x, int y)
{
	int a2,r2,g2,b2,tch,l,i,j,k;
	k = strlen(str);
	buildLayer(0,k*fontdata[fontn][0],fontdata[fontn][1]);
	reset(0);

	for (l=0; l<k; l++)
	{
		tch = str[l];
		for (i = 0; i<fontdata[fontn][0]; i++)
			for (j = 0; j<fontdata[fontn][1]; j++)
			{
				ARGBt(cnv[fontn+1][fontdata[fontn][0]*tch+i][j],&a2,&r2,&g2,&b2);
				if (a2 == 0) continue;
				if (a2 != 255) 
				{
					plot(0,i+fontdata[fontn][0]*l,j,a2,r2,g2,b2);
				}
				else
				{
					plot(0,i+fontdata[fontn][0]*l,j,cnv[fontn+1][fontdata[fontn][0]*tch+i][j]);
				}
			}
	}
	DrawRect(0,color,0,0,cnvd[0][0],cnvd[0][1],2);
	IncludeLayer(0,id,x,y,0);
	delLayer(0);
}

int TextWidth(int fontn, char *str)
{
	int k = strlen(str);
	return k*fontdata[fontn][0];
}

int TextHeight(int fontn, char *str)
{
	return fontdata[fontn][1];
}

void PrintNum(int id, int fontn, int subj, int color, int x, int y, int k)
{
	int a2,r2,g2,b2,tch,l,i,j;
	char str[9];
	sprintf(str,"%09i",subj);
	for (i = 0; i<9; i++) if (str[i] == ' ') str[i] = '0';
	buildLayer(0,k*fontdata[fontn][0],fontdata[fontn][1]);
	reset(0);

	for (l=0; l<k; l++)
	{
		tch = str[l+9-k]-48;
		for (i = 0; i<fontdata[fontn][0]; i++)
			for (j = 0; j<fontdata[fontn][1]; j++)
			{
				ARGBt(cnv[fontn+1][fontdata[fontn][0]*tch+i][j],&a2,&r2,&g2,&b2);
				if (a2 == 0) continue;
				if (a2 != 255) 
				{
					plot(0,i+fontdata[fontn][0]*l,j,a2,r2,g2,b2);
				}
				else
				{
					plot(0,i+fontdata[fontn][0]*l,j,cnv[fontn+1][fontdata[fontn][0]*tch+i][j]);
				}
			}
	}
	DrawRect(0,color,0,0,cnvd[0][0],cnvd[0][1],2);
	IncludeLayer(0,id,x,y,0);
	delLayer(0);
}

void MascedLayer(int id, int tid, int tx, int ty, int ColorR, int ColorG, int ColorB)
{
	unsigned int colors[4][4];
	RGBt(ColorR,&colors[1][1],&colors[1][2],&colors[1][3]);
	RGBt(ColorG,&colors[2][1],&colors[2][2],&colors[2][3]);
	RGBt(ColorB,&colors[3][1],&colors[3][2],&colors[3][3]);

	int i,j,k;
	for (i = 0; i<cnvd[id][0]; i++)
		for (j = 0; j<cnvd[id][1]; j++)
		{	
			ARGBt(cnv[id][i][j],&colors[0][0],&colors[0][1],&colors[0][2],&colors[0][3]);
			if (colors[0][0] == 0) continue;
			plot(tid,tx+i,ty+j,colors[0][0],0,0,0); //Creating Darkness
			for (k=1; k<4; k++)
			{
				add(tid,tx+i,ty+j,colors[0][k],colors[k][1],colors[k][2],colors[k][3]); 
			}
		}

}

void ZoomRender(int id, int tid, int tx, int ty, int x1, int y1, int cx, int cy, int x2, int y2, int mode)
//                             |starting coords|  final size   | start at layer|   piece size  |
{
	unsigned int a,r,g,b;
	int i,j,mx,my,nx,ny;
	float xt,yt;

	if (tx<0) nx = -tx;
	else nx = 0;
	if (ty<0) ny = -ty;
	else ny = 0;

	if (x1+tx>cnvd[tid][0]) mx = cnvd[tid][0]-tx;
	else mx = x1;
	if (y1+ty>cnvd[tid][1]) my = cnvd[tid][1]-ty;
	else my = y1;

	for (i = nx; i<mx; i++)
		for (j = ny; j<my; j++)
		{	
			xt = (float)x2/x1;
			yt = (float)y2/y1;
			ARGBt(cnv[id][cx + (int)(i*xt)][cy + (int)(j*yt)],&a,&r,&g,&b);
			if (a == 0) continue;
			if (a != 255) 
			{
				if (mode == 0) plot(tid,tx+i,ty+j,a,r,g,b);
				else if (mode == 1) add(tid,tx+i,ty+j,a,r,g,b);
				else if (mode == 2) mult(tid,tx+i,ty+j,a,r,g,b);
			}
			else
			{
				if (mode == 0) plot(tid,tx+i,ty+j,cnv[id][cx + (int)(i*xt)][cy + (int)(j*yt)]);
				else if (mode == 1) add(tid,tx+i,ty+j,r,g,b);
				else if (mode == 2) mult(tid,tx+i,ty+j,r,g,b);
			}
		}

}

void DrawFrame(int id, unsigned int color, int x, int y, int xk, int yk)
{
	FDrawHLine(id,color,x,y,xk);
	FDrawHLine(id,color,x,y+yk-1,xk);
	FDrawVLine(id,color,x,y,yk);
	FDrawVLine(id,color,x+xk-1,y,yk);
}
	
/*************************/
/**********DATA***********/
/*************************/
	
/*
	
  Data level ToDo:
	
	  +Pure Texture Loading
	  +Pure Font Loading
	  -Cloud Textures
	
*/
	
int gener(int max)
{
	return max?(rand()%max):0;
}

void renderer(int id)
{
	//char str[64];
	for (int i=0; i<xmax; i++)
		for (int j=0; j<ymax; j++)
		{
			ocnv[ymax-j-1][i] = cnv[id][i][j];
		}
	float zoom;
	if ((float)rymax/ymax > (float)rxmax/xmax)
	{
		zoom = (float)rxmax/xmax;
	}
	else
	{
		zoom = (float)rymax/ymax;
	}
	
	/*sprintf(str,"%f", zoom);
	MessageBox(NULL,str,"ERROR",MB_OK|MB_ICONEXCLAMATION);*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRasterPos2d( (rxmax - (xmax * zoom)) / 2,rymax - ( (rymax - (ymax * zoom) ) / 2));
	glPixelZoom(zoom,zoom);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glDrawPixels(xmax, ymax, GL_RGBA, GL_UNSIGNED_BYTE, ocnv);
	SwapBuffers();	
}
	
bool tex_read(int fname)
{
	unsigned short int chr[9], w, h, bit;
	unsigned char b[4];
	char  tl[128];

	tl[0] = '\0';
	strcat(tl, tex_mpath);
	strcat(tl, tex_list[fname]);
	
	FILE *tex_file;
	tex_file = fopen(tl, "rb" ); 
	fread(&chr, 1, 18, tex_file);
	
	delLayer(fname+16);
	w = chr[6];
	h = chr[5];
	bit = (chr[8] % 256) / 8;
	if (cnvd[fname+16][0]!=0)
		delLayer(fname+16);
	buildLayer(fname+16,w,h);
	
	/*sprintf(tl,"%i %i %i", w,h,bit);
	MessageBox(NULL,tl,"ERROR",MB_OK|MB_ICONEXCLAMATION);*/

	for (int j = 0; j<h; j++)
		for (int i = 0; i<w; i++)
		{
			fread(&b, 1, bit, tex_file);
			if (bit == 4) cnv[fname+16][i][j] = ARGB(b[3], b[2], b[1], b[0]);
			else cnv[fname+16][i][j] = tRGB(b[2], b[1], b[0]);
		}
	fclose(tex_file);
	return true;
}

bool fnt_read(int fname)
{
	unsigned short int chr[9], w, h, bit;
	unsigned char b[4];
	char  tl[128];
	//char str[64];

	tl[0] = '\0';
	strcat(tl, tex_mpath);
	strcat(tl, font_list[fname]);
	
	printf("fnt_read: fname=%i '%s'\n", fname, tl);
	
	FILE *tex_file;
	tex_file = fopen(tl, "rb" ); 
	fread(&chr, 1, 18, tex_file);
	
	w = chr[6];
	h = chr[5];
	bit = (chr[8] % 256) / 8;
	buildLayer(fname+1,w,h);
	
	/*sprintf(str,"%i %i %i", w,h,bit);
	MessageBox(NULL,str,"ERROR",MB_OK|MB_ICONEXCLAMATION);*/

	for (int j = 0; j<h; j++)
		for (int i = 0; i<w; i++)
		{
			fread(&b, 1, bit, tex_file);
			if (bit == 4) cnv[fname+1][i][j] = ARGB(b[3], b[2], b[1], b[0]);
			else cnv[fname+1][i][j] = tRGB(b[2], b[1], b[0]);
		}
	fclose(tex_file);
	return true;
}

void tex_init()
{
	FILE *tex_file;
	tex_file = fopen(tex_path, "r"); 
	for (int i=0; i<texmax; i++)
	{
		fscanf(tex_file, "%[^\r^\n]\n", tex_list[i]); // WTF,  why uppercase?
		for(char *p = (char*)&tex_list[i]; *p; ++p) *p = tolower(*p); // String to lowcase
		
		printf("tex_init: tex_list[%i]='%s'\n", i, tex_list[i]);
	}
	fclose(tex_file);
}

void font_init()
{
	FILE *tex_file;
	tex_file = fopen(font_path, "r");
	for (int i=0; i<fontmax; i++)
	{
		fscanf(tex_file, "%[^\r^\n]\n", font_list[i]); // WTF,  why uppercase?
		for(char *p = (char*)&font_list[i]; *p; ++p) *p = tolower(*p); // String to lowcase
		
		printf("font_init: font_list[%i]='%s'\n", i, font_list[i]);
		fnt_read(i);
	}
	fclose(tex_file);

	fontdata[0][0]=7;
	fontdata[1][0]=8;
	fontdata[2][0]=5;
	fontdata[0][1]=14;
	fontdata[1][1]=10;
	fontdata[2][1]=6;
	fontdata[0][2]=0;
	fontdata[1][2]=1;
	fontdata[2][2]=0;
}
