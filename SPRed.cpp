#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "SPRedFiles.h"
#include "Lib.h"
#include "Platform.h"
#include "LSprite.h"

#ifdef WIN32
	#include <windows.h>
	#include <GL\gl.h>
	#include <GL\glu.h>
	#include "SDL2\SDL.h"
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <SDL2/SDL.h>
#else //UNIX
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <SDL2/SDL.h>
#endif

#define ALWAYS_SHOW_FIRST 1

bool selection=false, working=false, prerendered=false, playing=false;
int inmenu=1;
const strmax = 39, scrmax = 38;
int sel=1, selmax=4, avfiles=0;
int timer=0;
int vselx=0, vsely=0,vstart=0,vfinish=0;
int asel=0,ae=0,aef=0,aemin=0,aemax=1,aefmin=0,aefmax=1;
char str[strmax+1][64], avnames[16][64], current[64], currenttga[64];
int scrs[scrmax][2];

int camx=0,camy=0,selframe=0,tselx,tsely;
LSprite W;
int lc=0,lte=0;

void _L()
{
	unsigned char oe;
	int i;
	char fname[128];
	FILE *spr_file;
	sprintf(fname,"%s%s",SPRITES_PATH,current);
	spr_file = fopen(fname, "r"); 

	fread(&oe, 1, 1, spr_file);

	for (i=0; i<oe; i++) fread(&currenttga[i], 1, 1, spr_file);
	currenttga[i]='\0';
	fclose(spr_file);
}

bool set_read()
{
	FILE *set_file;
	char set_path[] = SETTINGS_DAT;
	int tmp;
	set_file = fopen(set_path, "r"); 
	fscanf(set_file, "%i %i", &rxmax, &rymax);
	fscanf(set_file, "%i %i", &xmax, &ymax);
	fscanf(set_file, "%i", &tmp);
	if (tmp == 1) 
	{
		fullscreen=true;
	}
	else
	{
		fullscreen=false;
	}
	fscanf(set_file, "%i %i", &l_targetfps, &l_minframeskip);
	fclose(set_file);
	return true;
}

bool ave_read(char* FNAME)
{
	FILE *set_file;
	set_file = fopen(FNAME, "r"); 
	fscanf(set_file, "%i", &avfiles);
	if (avfiles != 0) 
	{
		for (int i=0; i<avfiles; i++)
		{
			fscanf(set_file,"%[^\n]\n",avnames[i+1]);
		}
	}
	fclose(set_file);
	return true;
}

void set_write()
{
	FILE *set_file;
	char set_path[] = SETTINGS_DAT;
	int tmp;
	set_file = fopen(set_path, "w" ); 
	fprintf(set_file, "%i %i ", rxmax, rymax);
	fprintf(set_file, "%i %i ", xmax, ymax);
	fullscreen ? tmp=1 : tmp=0;
	fprintf(set_file, "%i ", tmp);
	fprintf(set_file, "%i %i ", l_targetfps, l_minframeskip);
	fclose(set_file);
}

bool scr_read()
{	
	FILE *set_file;
	char scr_path[] = SCRSIZES_DAT;
	set_file = fopen(scr_path, "r" ); 
	for (int i=0; i<scrmax; i++) 
	{
		fscanf(set_file, "%i %i", &scrs[i][0], &scrs[i][1]);
	}
	fclose(set_file);
	return true;
}

void str_read()
{
	FILE *str_file;
	str_file = fopen(LANGDATA_DAT, "r" ); 
	for (int i=0; i<strmax; i++)
	{
		fscanf(str_file,"%[^\n]\n",str[i]);
	}
	fclose(str_file);
}

void save()
{
	unsigned char oe;
	int i,j, oi;
	char fname[128];
	FILE *spr_file;
	sprintf(fname,"%s%s",SPRITES_PATH,current);
	spr_file = fopen(fname, "w");
	oe = strlen(currenttga);
	fwrite(&oe, 1, 1, spr_file);
	for (i=0; i<oe; i++) fwrite(&currenttga[i], 1, 1, spr_file);
	fwrite(&W.frames, 1, 2, spr_file);
	for (i=0; i<W.frames; i++)
	{
		fwrite(&W.frameOffsets[0][i], 1, 2, spr_file);		
		fwrite(&W.frameOffsets[1][i], 1, 2, spr_file);		
		fwrite(&W.frameSizes[0][i], 1, 2, spr_file);		
		fwrite(&W.frameSizes[1][i], 1, 2, spr_file);
		oi = W.frameFix[0][i] + 32768;
		fwrite(&oi, 1, 2, spr_file);	
		oi = W.frameFix[1][i] + 32768;		
		fwrite(&oi, 1, 2, spr_file);	
	}

	fwrite(&W.anims, 1, 2, spr_file);
	
	for (i=0; i<W.anims; i++)
	{
		fwrite(&W.animsize[i], 1, 2, spr_file);	
		for (j=0; j<W.animsize[i]; j++)
		{
			fwrite(&W.anim[i][0][j], 1, 2, spr_file);
			fwrite(&W.anim[i][1][j], 1, 2, spr_file);	
			fwrite(&W.anim[i][2][j], 1, 4, spr_file);
		}
		
	}

	for (i=0; i<16; i++)
	{
		fwrite(&W.meta[i], 1, 2, spr_file);				
	}
	fwrite(&W.advancedflags, 1, 2, spr_file);	
	fclose(spr_file); 
}

void prc()
{
	char strn[64];

	lc++;
	if (lc>=TextWidth(1,str[20+lte]))
	{
		lc = -xmax;
		lte++;
		if (lte>=3) lte=0;
	}
	working=true;

	//interface
	if (frame % (frameskip+1)== 0) 
	{
	int cx=xmax/2,cy=(ymax-11)*3/5;
	////img
	FDrawRect(100,tRGB(8,4,4),0,0,xmax,ymax);
	DrawFrame(100,tRGB(8,128,8),cx-camx-1,cy-camy-1,getWidth(101)+2,getHeight(101)+2);
	IncludeLayer(101,100,cx-camx,cy-camy,0);
	DrawHLine(100,ARGB(128,255,0,0),0,cy,xmax);
	DrawVLine(100,ARGB(128,255,0,0),cx,0,ymax);

	if (selection) DrawFrame(100,ARGB(128,96,96,8),cx-camx+tselx,cy-camy+tsely,-tselx+camx+1,-tsely+camy+1);
	else if (W.frameSizes[0][selframe]!=0 && W.frameSizes[1][selframe]!=0) DrawFrame(100,ARGB(128,8,196,8),cx-camx+W.frameOffsets[0][selframe],cy-camy+W.frameOffsets[1][selframe],W.frameSizes[0][selframe],W.frameSizes[1][selframe]);

	//top
	FDrawRect(100,tRGB(8,4,4),0,0,xmax,ymax/5);
	FDrawHLine(100,tRGB(128,8,8),0,ymax/5,xmax);
	float zoom;
	for (int i=0; i<W.frames; i++)
	{
		if ((float)W.frameSizes[1][i]*5/ymax > (float)W.frameSizes[0][i]*5/xmax) zoom = (float)W.frameSizes[1][i]*5/ymax;
		else zoom = (float)W.frameSizes[0][i]*5/xmax;
		if (W.frameSizes[0][i]!=0 && W.frameSizes[1][i]!=0)
			ZoomRender(101,100,(xmax*(i-selframe)/5)+(xmax*2/5)+(xmax/5-(W.frameSizes[0][i]/zoom))/2,(ymax/5-(W.frameSizes[1][i]/zoom))/2,W.frameSizes[0][i]/zoom,W.frameSizes[1][i]/zoom,W.frameOffsets[0][i],W.frameOffsets[1][i],W.frameSizes[0][i],W.frameSizes[1][i],0);
		sprintf(strn,"%s%i",str[14],i);
		PrintText(100,1,strn,tRGB(0,0,0),(xmax*(i-selframe)/5)+(xmax*2/5)+2,3);
		PrintText(100,1,strn,tRGB(192,192,192),(xmax*(i-selframe)/5)+(xmax*2/5)+1,2);
		FDrawVLine(100,tRGB(128,8,8),((xmax)*(i-selframe)/5)+(xmax*2/5),0,ymax/5);
	}
	FDrawVLine(100,tRGB(128,8,8),((xmax)*(i-selframe)/5)+(xmax*2/5),0,ymax/5);
	DrawFrame(100,tRGB(8,128,8),xmax*2/5,0,xmax/5+1,ymax/5+1);

	
	//topright
	FDrawRect(100,tRGB(8,4,4),xmax*4/5,0,xmax/5,ymax/5);
	if ((float)getHeight(101)*5/ymax > (float)getWidth(101)*5/xmax) zoom = (float)getHeight(101)*5/ymax;
	else zoom = (float)getWidth(101)*5/xmax;
	DrawVLine(100,ARGB(128,255,0,0),xmax*4/5-1,0,ymax/5);
	FDrawRect(102,tRGB(0,0,0),0,0,getWidth(101),getHeight(101));
	IncludeLayer(101,102,0,0,0);
	DrawRect(102,ARGB(64,255,255,255),camx-cx,camy-cy+ymax/5,xmax,ymax-11-ymax/5,0);
	if (!selection)
	{
		if (W.frameSizes[0][selframe]!=0 && W.frameSizes[1][selframe]!=0) DrawRect(102,ARGB(128,0,255,0),W.frameOffsets[0][selframe],W.frameOffsets[1][selframe],W.frameSizes[0][selframe],W.frameSizes[1][selframe]);
	}
	if (selection)
	{
		DrawRect(102,ARGB(128,192,192,0),tselx,tsely,camx-tselx+1,camy-tsely+1);
	}
	ZoomRender(102,100,xmax*4/5+(xmax/5-(getWidth(101)/zoom))/2,(ymax/5-(getHeight(101)/zoom))/2,getWidth(101)/zoom,getHeight(101)/zoom,0,0,getWidth(101),getHeight(101),0);
	
	//statbar
	DrawHLine(100,ARGB(128,255,0,0),0,ymax-11,xmax);
	FDrawRect(100,tRGB(8,4,4),0,ymax-10,xmax,10);
	PrintText(100,1,str[20+lte],tRGB(196,196,196),-lc,ymax-9);

	//info
	sprintf(strn,"X:%i Y:%i",camx,camy);
	PrintText(100,1,strn,tRGB(0,0,0),3,ymax-19);
	PrintText(100,1,strn,tRGB(196,196,196),2,ymax-20);
	if (!selection) sprintf(strn,"%s%i:%i",str[17],W.frameOffsets[0][selframe],W.frameOffsets[1][selframe]);
	else sprintf(strn,"%s%i:%i",str[17],tselx,tsely);
	PrintText(100,1,strn,tRGB(0,0,0),3,ymax-37);
	PrintText(100,1,strn,tRGB(196,196,196),2,ymax-38);
	if (!selection) sprintf(strn,"%s%i:%i",str[19],W.frameSizes[0][selframe],W.frameSizes[1][selframe]);
	else sprintf(strn,"%s%i:%i",str[19],camx-tselx+1,camy-tsely+1);
	PrintText(100,1,strn,tRGB(0,0,0),3,ymax-28);
	PrintText(100,1,strn,tRGB(196,196,196),2,ymax-29);
	
	//corner
	FDrawRect(100,tRGB(8,4,4),xmax*4/5,ymax*4/5-11,xmax/5,ymax/5);
	DrawFrame(100,tRGB(128,8,8),xmax*4/5-1,ymax*4/5-1-11,xmax/5+2,ymax/5+2);	
	FDrawRect(103,tRGB(0,0,0),0,0,xmax/10,ymax/10);
	IncludeLayer(101,103,-camx+xmax/20,-camy+ymax/20,0);
	DrawFrame(103,tRGB(8,128,8),-camx+xmax/20-1,-camy+ymax/20-1,getWidth(101)+2,getHeight(101)+2);
	DrawRect(103,0,xmax/20,ymax/20,1,1,4);
	ZoomRender(103,100,xmax*4/5,ymax*4/5-11,xmax/5,ymax/5,0,0,xmax/10,ymax/10,0);

	renderer(100);
	}
}

void anm_sel()
{
	prerendered=false;
	char strn[64];
	int inmenu_color=tRGB(196,196,196), inmenu_selected=tRGB(224,224,0), cl;
	lc++;
	if (lc>=TextWidth(1,str[30+lte]))
	{
		lc = -xmax;
		lte++;
		if (lte>=1) lte=0;
	}
	if (frame % (frameskip+1)== 0) 
	{
		FDrawRect(100,tRGB(8,4,4),0,0,xmax,ymax);

		selmax = W.anims+1;
		for (int i=1; i<=selmax; i++)
		{
			if (sel == i) 
			{
				cl = inmenu_selected;
			}
			else 
			{
				cl = inmenu_color;
			}

			if (i!=selmax) sprintf(strn,"%s %i",str[16],i-1);
			else sprintf(strn,"%s",str[33]);
			PrintText(100,1,strn,cl,xmax/2-100,ymax/2-(sel*11)+i*11);
		}

		DrawHLine(100,ARGB(128,255,0,0),0,ymax-11,xmax);
		FDrawRect(100,tRGB(8,4,4),0,ymax-10,xmax,10);
		PrintText(100,1,str[30+lte],tRGB(196,196,196),-lc,ymax-9);

		renderer(100);
	}
}

void anm()
{
	char strn[64];
	int right16=xmax*5/6;
	int right26=xmax*2/3;
	int inmenu_color=tRGB(196,196,196), inmenu_selected=tRGB(224,224,0), cl;
	aemax=(aemax-1)?aemax:(ymax-22)/12;
	aefmax=(aefmax-1)?aefmax:(ymax-22)/12;

	lc++;
	if (lc>=TextWidth(1,str[34+lte]))
	{
		lc = -xmax;
		lte++;
		if (lte>=3) lte=0;
	}
	if (frame % (frameskip+1)== 0) 
	{
		FDrawRect(100,tRGB(8,4,4),0,0,xmax,ymax);

		FDrawVLine(100,tRGB(128,8,8),right16,0,ymax-22);
		int t = (aefmax<W.frames)?aefmax:W.frames;
		for (int i=aefmin; i<t; i++) 
		{
			if (aef == i) cl = inmenu_selected;
			else cl = inmenu_color;

			sprintf(strn,"%s%i",str[38],i);
			PrintText(100,1,strn,cl,right16+1,(i-aefmin)*12);
		}
		
		FDrawVLine(100,tRGB(128,8,8),right26,0,ymax-22);

		t = (aemax<W.animsize[asel])?aemax:W.animsize[asel];
		for (i=aemin; i<t; i++) 
		{
			if (ae == i) cl = inmenu_selected;
			else cl = inmenu_color;

			sprintf(strn,"%i:%i",W.anim[asel][0][i],W.anim[asel][1][i]);
			PrintText(100,1,strn,cl,right26+1,(i-aemin)*12);
		}
		

		t = W.anim[asel][0][ae];
		if (W.animsize[asel]>0) FragmentLayer(101,100,right26/2-W.frameSizes[0][t]/2+W.frameFix[0][t],(ymax-22)/2-W.frameSizes[1][t]/2+W.frameFix[1][t],W.frameOffsets[0][t],W.frameOffsets[1][t],W.frameSizes[0][t],W.frameSizes[1][t],0);
		
		FragmentLayer(101,102,0,0,W.frameOffsets[0][aef],W.frameOffsets[1][aef],W.frameSizes[0][aef],W.frameSizes[1][aef],0);
		DrawRect(102,ARGB(128,0,0,0),0,0,getWidth(101),getHeight(101),6);
		FragmentLayer(102,100,right26/2-W.frameSizes[0][aef]/2+W.frameFix[0][aef],(ymax-22)/2-W.frameSizes[1][aef]/2+W.frameFix[1][aef],0,0,W.frameSizes[0][aef],W.frameSizes[1][aef],0);

		DrawHLine(100,ARGB(128,255,0,0),0,ymax-22,xmax);

		t=0;
		if (W.animsize[asel]>0) 
		{
			for (i=0; i<ae; i++) t+=W.anim[asel][1][i];
			int ot=xmax/2-t;
			for (i=0; i<W.animsize[asel]; i++) 
			{
				if (ae != i) DrawRect(100,ARGB(128/(i%2+1),255,255,255),ot,ymax-21,W.anim[asel][1][i],10);
				else 
				{
					FDrawRect(100,tRGB(8,196,8),ot,ymax-21,W.anim[asel][1][i],10);
					sprintf(strn,"%i - %i",ot-xmax/2+t,ot-xmax/2+t+W.anim[asel][1][i]);
					PrintText(100,1,strn,tRGB(0,0,0),2,ymax-30);
					PrintText(100,1,strn,inmenu_color,1,ymax-31);
				}
				ot+=W.anim[asel][1][i];
			}
			ot=ot-xmax/2+t;
			for (i=0; i<=ot/l_targetfps; i++) FDrawVLine(100,tRGB(192,8,8),xmax/2-t+l_targetfps*i,ymax-21,10);
		}

		DrawHLine(100,ARGB(128,255,0,0),0,ymax-11,xmax);
		FDrawRect(100,tRGB(8,4,4),0,ymax-10,xmax,10);
		PrintText(100,1,str[34+lte],tRGB(196,196,196),-lc,ymax-9);
		renderer(100);
	}
}

void player()
{
	char strn[64];
	if (!prerendered)
	{
		int t=0, ot=0;
		for (int i=0; i<W.animsize[asel]; i++) t+=W.anim[asel][1][i];
		if (timer>=t) timer-=t;
		buildLayer(105,t,10);
		for (i=0; i<W.animsize[asel]; i++)
		{
			DrawRect(105,ARGB((i%2)?192:128,255,255,255),ot,0,W.anim[asel][1][i],10);
			ot+=W.anim[asel][1][i];
		}
	}
	if (playing) 
	{
		W.pos_x=xmax*100/2;
		W.pos_y=ymax*100/2;
		timer++;
		W.Update();
	}
	if (frame % (frameskip+1)== 0) 
	{
		FDrawRect(100,tRGB(8,4,4),0,0,xmax,ymax);
		
		W.Draw(100);

		//progress
		DrawHLine(100,ARGB(128,255,0,0),0,ymax-11,xmax);
		IncludeLayer(105,100,xmax/2-timer,ymax-10,0);
		DrawVLine(100,ARGB(128,255,255,0),xmax/2,ymax-11,11);

		int inmenu_color=tRGB(196,196,196);

		sprintf(strn,"%i",timer);
		PrintText(100,1,strn,playing?tRGB(0,192,0):inmenu_color,0,ymax-20);

		sprintf(strn,"%i | %i | %i",fps,frameskip, W.animsize[asel]);
		PrintText(100,1,strn,inmenu_color,0,0);
		renderer(100);
	}
}

void vw()
{
	char strn[64];
	int columns = 6, lines=columns*(ymax-11)/xmax;
	selframe = vselx+vsely*columns;
	vfinish=vfinish?vfinish:lines;
	lc++;
	if (lc>=TextWidth(1,str[27+lte]))
	{
		lc = -xmax;
		lte++;
		if (lte>=1) lte=0;
	}
	if (frame % (frameskip+1)== 0) 
	{
		FDrawRect(100,tRGB(8,4,4),0,0,xmax,ymax);
		float zoom;
		for (int i = 0; i<=columns; i++) FDrawVLine(100,tRGB(128,8,8),i*xmax/columns,0,ymax-11);
		for (i = 0; i<=lines; i++) FDrawHLine(100,tRGB(128,8,8),0,i*(ymax-11)/lines,xmax);
		int vst = vstart*columns;
		int vfn = (vfinish*columns < W.frames)?(vfinish*columns):W.frames;
		for (i=vst; i<vfn; i++)
		{
			FDrawRect(100,tRGB(8,4,4),(i%columns)*xmax/columns+1,(i/columns)*(ymax-11)/lines+1,xmax/columns-1,(ymax-11)/lines-1);
			if ((float)W.frameSizes[1][i]*lines/(ymax-11) > (float)W.frameSizes[0][i]*columns/xmax) zoom = (float)W.frameSizes[1][i]*lines/(ymax-11);
			else zoom = (float)W.frameSizes[0][i]*columns/xmax;
			if (W.frameSizes[0][i]!=0 && W.frameSizes[1][i]!=0)
				ZoomRender(101,100,(i%columns)*xmax/columns+1,(i/columns-vstart)*(ymax-11)/lines+1,W.frameSizes[0][i]/zoom-1,W.frameSizes[1][i]/zoom-1,W.frameOffsets[0][i],W.frameOffsets[1][i],W.frameSizes[0][i],W.frameSizes[1][i],0);
			sprintf(strn,"%i",i);
			PrintText(100,1,strn,tRGB(0,0,0),(i%columns)*xmax/columns+1,(i/columns-vstart)*(ymax-11)/lines+1);
			PrintText(100,1,strn,tRGB(192,192,192),(i%columns)*xmax/columns,(i/columns-vstart)*(ymax-11)/lines);
			if (i == vselx+columns*vsely) DrawFrame(100,tRGB(8,128,8),(i%columns)*xmax/columns,(i/columns-vstart)*(ymax-11)/lines,xmax/columns+1,(ymax-11)/lines+1);

		}

		PrintText(100,1,str[27+lte],tRGB(196,196,196),-lc,ymax-9);
		renderer(100);
	}
}

void initMenu()
{
	char strn[64];
	int inmenu_color=tRGB(196,196,196), inmenu_selected=tRGB(224,224,0);
	int cl;
	FDrawRect(100,tRGB(0,0,0),0,0,xmax,ymax);
	if (inmenu==1) //Main menu
	{
		selmax = 4;
		if (working) selmax=5;
		for (int i=1; i<=selmax; i++)
		{
			if (sel == i) 
			{
				cl = inmenu_selected;
			}
			else 
			{
				cl = inmenu_color;
			}

			if (i==1) PrintText(100,1,current,cl,xmax/2,ymax/2-100+i*20);
			if (i==5) PrintText(100,1,str[25],cl,xmax/2-100,ymax/2-100+i*20);
			else PrintText(100,1,str[i],cl,xmax/2-100,ymax/2-100+i*20);
		}
	}
	if (inmenu==11) //goto files
	{
		inmenu=220;
		ave_read(RCNTFLES_DAT);
	}
	if (inmenu==12) //initator
	{
		ave_read(RCNTTGAS_DAT);
		inmenu=230;
	}
	if (inmenu==13) //loader
	{
		W.Load(current, 101);
		camx=0;					camy=0;
		selframe=0;				selection=false;
		buildLayer(102,getWidth(101),getHeight(101));
		buildLayer(103,xmax/10,ymax/10);
		_L();
		inmenu=3;
	}
	if (inmenu==14) //Options
	{
		selmax = 4;
		for (int i=1; i<=selmax; i++)
		{
			if (sel == i) 
			{
				cl = inmenu_selected;
			}
			else 
			{
				cl = inmenu_color;
			}

			if (i==3) 
			{
				sprintf(strn,"%i",l_targetfps);
				PrintText(100,1,strn,cl,xmax/2,ymax/2-100+i*20);
			}
			
			if (i==4) 
			{
				sprintf(strn,"%i",l_minframeskip);
				PrintText(100,1,strn,cl,xmax/2+20,ymax/2-100+i*20);
			}
			PrintText(100,1,str[i+4],cl,xmax/2-100,ymax/2-100+i*20);
		}
	}
	if (inmenu==141) //goto resolution
	{

		inmenu=200;
		for (int i=0; i<scrmax; i++)
		{
			if (scrs[i][0] == rxmax && scrs[i][1] == rymax)
			{
				sel = i+1;
				break;
			}
		}
	}
	if (inmenu==142) //goto resolution2
	{

		inmenu=210;
		for (int i=0; i<scrmax; i++)
		{
			if (scrs[i][0] == xmax && scrs[i][1] == ymax)
			{
				sel = i+1;
				break;
			}
		}
	}
	if (inmenu==143) //target fps
	{
		selmax = 9;
		for (int i=1; i<=selmax; i++)
		{
			if (sel == i) 
			{
				cl = inmenu_selected;
			}
			else 
			{
				cl = inmenu_color;
			}

			sprintf(strn,"%i",i*10);
			PrintText(100,1,strn,cl,xmax/2-100,ymax/2-100+i*20);
		}
	}
	if (inmenu>1430 && inmenu<1440)
	{
		l_targetfps = (inmenu-1430)*10;
		set_write();
		inmenu=14;
	}
	if (inmenu==144) //frameskip
	{
		selmax = 9;
		for (int i=1; i<=selmax; i++)
		{
			if (sel == i) 
			{
				cl = inmenu_selected;
			}
			else 
			{
				cl = inmenu_color;
			}

			sprintf(strn,"%i",i-1);
			PrintText(100,1,strn,cl,xmax/2-100,ymax/2-100+i*20);
		}
	}
	if (inmenu>1440 && inmenu<1450)
	{
		l_minframeskip = inmenu-1441;
		set_write();
		inmenu=14;
	}
	if (inmenu==15)
	{
		inmenu=3;
	}
	if ((inmenu==20) || (inmenu==21)) //return fron resolution
	{
		inmenu=14;
	}
	if ((inmenu==22) || (inmenu==23))  //return fron filename
	{
		inmenu=1;
	}
	if ((inmenu==200) || (inmenu==210)) //change resolution
	{
		selmax=scrmax;
		for (int i=1; i<=scrmax; i++)
		{
			if (sel == i) 
			{
				cl = inmenu_selected;
			}
			else 
			{
				cl = inmenu_color;
			}
			sprintf(strn,"%ix%i",scrs[i-1][0],scrs[i-1][1]);
			PrintText(100,1,strn,cl,xmax/2-100,ymax/2-(sel*11)+i*11);
		}
	}
	if (inmenu==220) //Filename
	{
		selmax = avfiles+1;
		for (int i=1; i<=selmax; i++)
		{
			if (sel == i) 
			{
				cl = inmenu_selected;
			}
			else 
			{
				cl = inmenu_color;
			}

			if (i>1) PrintText(100,1,avnames[i-1],cl,xmax/2-120,ymax/2-(sel*20)+i*20);
			else PrintText(100,1,str[9],cl,xmax/2-80,ymax/2-(sel*20)+i*20);
		}
	}
	if (inmenu==230) //Filename tga
	{
		selmax = avfiles+1;
		for (int i=1; i<=selmax; i++)
		{
			if (sel == i) 
			{
				cl = inmenu_selected;
			}
			else 
			{
				cl = inmenu_color;
			}

			if (i>1) PrintText(100,1,avnames[i-1],cl,xmax/2-120,ymax/2-(sel*20)+i*20);
			else PrintText(100,1,str[10],cl,xmax/2-80,ymax/2-(sel*20)+i*20);
		}
	}
	if (inmenu>2000 && inmenu<2100) //enable resolution
	{
		rxmax = scrs[inmenu-2001][0];
		rymax = scrs[inmenu-2001][1];
		reload();
		set_write();
		inmenu=14;
	}
	if (inmenu>2100 && inmenu<2200) //enable resolution
	{
		l_resize(scrs[inmenu-2101][0],scrs[inmenu-2101][1]);
		buildLayer(100,xmax,ymax);
		set_write();
		inmenu=14;
	}
	if (inmenu==2201) //Filename
	{
		PrintText(100,1,str[11],inmenu_color,xmax/2-80,ymax/2);
		sprintf(strn,"inputer.exe %s",RCNTFLES_DAT);
		system(strn);
		inmenu=220;
		ave_read(RCNTFLES_DAT);
	}
	if (inmenu>2201 && inmenu<2300) //Filename
	{
		sprintf(current, "%s",avnames[inmenu-2201]);
		inmenu=1;
	}
	if (inmenu==2301) //Filename
	{
		PrintText(100,1,str[11],inmenu_color,xmax/2-80,ymax/2);
		sprintf(strn,"inputer.exe %s",RCNTTGAS_DAT);
		system(strn);
		inmenu=230;
		ave_read(RCNTTGAS_DAT);
	}
	if (inmenu>2301 && inmenu<2400) //Filename
	{
		sprintf(currenttga, "%s",avnames[inmenu-2301]);
		inmenu=3;
		ctex_read(currenttga,101);
		W.advancedflags = 0;
		W.contentlayer = 101;
		W.frames=1;
		W.frameOffsets[0][0]=0;	W.frameOffsets[1][0]=0;
		W.frameSizes[0][0]=0;	W.frameSizes[1][0]=0;
		W.frameFix[0][0]=0;		W.frameFix[1][0]=0;
		camx=0;					camy=0;
		selframe=0;				selection=false;
		buildLayer(102,getWidth(101),getHeight(101));
		buildLayer(103,xmax/10,ymax/10);
	}
	if (inmenu==31) //waiting
	{
		PrintText(100,1,str[11],inmenu_color,xmax/2-80,ymax/2);
	}
	if (inmenu==312) //waiting
	{
		PrintText(100,1,str[11],inmenu_color,xmax/2-80,ymax/2);
	}
	if (inmenu==33) //waiting
	{
		inmenu=3;
	}
	if (inmenu==3301+W.anims)
	{
		inmenu=330;
		if (W.anims == LSPRITE_ANIMS_MAX) return;
		W.anims++;
		W.animsize[W.anims-1] = 0;
	}
	else if (inmenu>3300 && inmenu<3400)
	{
		asel = inmenu-3301;
		ae = 0;
		aef = 0;
		inmenu=34;
	}
	if (inmenu==3121)
	{
		inmenu=34;
	}
	if (inmenu==311)
	{
		inmenu=3;
	}
	sprintf(strn,"%i | %i | %i",fps,frameskip, dticks);
	PrintText(100,1,strn,inmenu_color,0,0);
	if (frame % (frameskip+1)== 0) renderer(100);
	
}

int init()
{
	scr_read();
	str_read();
	font_init();
	buildLayer(100,xmax,ymax);
	return 0;
}

void OP()
{
	if (inmenu==3) prc();
	else if (inmenu==32) vw();
	else if (inmenu==330) anm_sel();
	else if (inmenu==34) anm();
	else if (inmenu==35) player();
	else initMenu();
}

void KOP()
{	
	if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0 && inmenu==1)
	{
		Quit(0);
	}
	if (inmenu==3)
	{
		if (get_key(KEY_UP) && (get_keypressed(KEY_UP) % (l_targetfps/3) == 0 || get_keypressed(KEY_UP) > l_targetfps))
		{
			camy--;
		}
		if (get_key(KEY_DOWN) && (get_keypressed(KEY_DOWN) % (l_targetfps/3) == 0 || get_keypressed(KEY_DOWN) > l_targetfps))
		{
			camy++;
		}
		if (get_key(KEY_LEFT) && (get_keypressed(KEY_LEFT) % (l_targetfps/3) == 0 || get_keypressed(KEY_LEFT) > l_targetfps))
		{
			camx--;
		}
		if (get_key(KEY_RIGHT) && (get_keypressed(KEY_RIGHT) % (l_targetfps/3) == 0 || get_keypressed(KEY_RIGHT) > l_targetfps))
		{
			camx++;
		}
		if (get_key(KEY_NUMPAD4) && (get_keypressed(KEY_NUMPAD4) % (l_targetfps/3) == 0 || get_keypressed(KEY_NUMPAD4) > l_targetfps))
		{
			if (selframe>0)selframe--;
		}
		if (get_key(KEY_NUMPAD6) && (get_keypressed(KEY_NUMPAD6) % (l_targetfps/3) == 0 || get_keypressed(KEY_NUMPAD6) > l_targetfps))
		{
			if (selframe<W.frames-1)selframe++;
		}
		if (get_key(KEY_NUMPAD5) && get_keypressed(KEY_NUMPAD5) == 0 && W.frames < LSPRITE_FRAMES_MAX)
		{
			W.frames++;
			W.frameOffsets[0][W.frames-1]=0;	W.frameOffsets[1][W.frames-1]=0;
			W.frameSizes[0][W.frames-1]=0;		W.frameSizes[1][W.frames-1]=0;
			W.frameFix[0][W.frames-1]=0;		W.frameFix[1][W.frames-1]=0;
		}
		if (get_key(KEY_NUMPAD0) && get_keypressed(KEY_NUMPAD0) == 0)
		{
			if (W.frames!=1)
			{
				W.frames--;
				for (int i=selframe; i<W.frames; i++)
				{
					W.frameOffsets[0][selframe]=W.frameOffsets[0][selframe+1];	W.frameOffsets[1][selframe]=W.frameOffsets[1][selframe+1];
					W.frameSizes[0][selframe]=W.frameSizes[0][selframe+1];		W.frameSizes[1][selframe]=W.frameSizes[1][selframe+1];
					W.frameFix[0][selframe]=W.frameFix[0][selframe+1];			W.frameFix[1][selframe]=W.frameFix[1][selframe+1];
				}
			}
		}
		if (get_key(KEY_F1) && get_keypressed(KEY_F1) == 0)
		{
			save();
			inmenu=31;
		}
		if (get_key(KEY_F2) && get_keypressed(KEY_F2) == 0)
		{
			inmenu=330;
		}
		if (get_key(KEY_F3) && get_keypressed(KEY_F3) == 0)
		{
			inmenu=32;
		}
		if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0)
		{
			inmenu=1;
		}
		if (get_key(KEY_RETURN) && get_keypressed(KEY_RETURN) == 0)
		{
			if (!selection)
			{
				tselx=camx;
				tsely=camy;	
			}
			else
			{	
				if (tselx<camx) 
				{
					W.frameOffsets[0][selframe]=tselx;
					W.frameSizes[0][selframe]=camx-tselx+1;
				}
				else 
				{
					W.frameSizes[0][selframe]=tselx-camx+1;
					W.frameOffsets[0][selframe]=camx;
				}
				if (tsely<camy) 
				{
					W.frameOffsets[1][selframe]=tsely;
					W.frameSizes[1][selframe]=camy-tsely+1;
				}
				else 
				{
					W.frameSizes[1][selframe]=tsely-camy+1;
					W.frameOffsets[1][selframe]=camy;
				}

				
			}
			selection=!selection;
		}
	}
	else if (inmenu==34)
	{
		if (W.animsize[asel]>0)
		{
			if (get_key(KEY_UP) && (get_keypressed(KEY_UP) % (l_targetfps/3) == 0 || get_keypressed(KEY_UP) > l_targetfps))
			{
				ae--;
				if (ae < 0) ae=W.animsize[asel]-1;
			}
			if (get_key(KEY_DOWN) && (get_keypressed(KEY_DOWN) % (l_targetfps/3) == 0 || get_keypressed(KEY_DOWN) > l_targetfps))
			{
				ae++;
				if (ae > W.animsize[asel]-1) ae=0;
			}
			if (ae<aemin)
			{
				aemin--;
				aemax--;
			}
			if (ae>(aemax-1))
			{
				aemin++;
				aemax++;
			}
			if (ae > W.animsize[asel]-1) ae = W.animsize[asel]-1;
			if (get_key(KEY_LEFT) && (get_keypressed(KEY_LEFT) % (l_targetfps/3) == 0 || get_keypressed(KEY_LEFT) > l_targetfps))
			{
				W.anim[asel][1][ae]--;
				if (W.anim[asel][1][ae] < 1) W.anim[asel][1][ae]=1;
			}
			if (get_key(KEY_RIGHT) && (get_keypressed(KEY_RIGHT) % (l_targetfps/3) == 0 || get_keypressed(KEY_RIGHT) > l_targetfps))
			{
				W.anim[asel][1][ae]++;
				if (W.anim[asel][1][ae] > 65535) W.anim[asel][1][ae]=65535;
			}
		}
		if (get_key(KEY_NUMPAD8) && (get_keypressed(KEY_NUMPAD8) % (l_targetfps/3) == 0 || get_keypressed(KEY_NUMPAD8) > l_targetfps))
		{
			aef--;
			if (aef < 0) aef=W.frames-1;
		}
		if (get_key(KEY_NUMPAD2) && (get_keypressed(KEY_NUMPAD2) % (l_targetfps/3) == 0 || get_keypressed(KEY_NUMPAD2) > l_targetfps))
		{
			aef++;
			if (aef > W.frames-1) aef=0;
		}
		if (aef<aefmin)
		{
			aefmin--;
			aefmax--;
		}
		if (aef>(aefmax-1))
		{
			aefmin++;
			aefmax++;
		}

		if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0)
		{
			inmenu = 330;
			sel = 1;
		}
		if (get_key(KEY_F1) && get_keypressed(KEY_F1) == 0)
		{
			save();
			inmenu=312;
		}
		if (get_key(KEY_F3) && get_keypressed(KEY_F3) == 0)
		{
			W.ChangeAnim(asel,true);
			inmenu=35;
		}
		if (get_key(KEY_NUMPAD5) && get_keypressed(KEY_NUMPAD5) == 0 && W.animsize[asel] < LSPRITE_MAX_ANIMSIZE)
		{
			W.animsize[asel]++;
			int adef = 0xFFFFFFFF;
			W.anim[asel][0][W.animsize[asel]-1]=aef;
			W.anim[asel][1][W.animsize[asel]-1]=10;	
			W.anim[asel][2][W.animsize[asel]-1]=adef;	
		}
		if (get_key(KEY_NUMPAD0) && get_keypressed(KEY_NUMPAD0) == 0)
		{
			if (W.animsize[asel]!=1)
			{	
				W.animsize[asel]--;
				for (int i=ae; i<W.animsize[asel]; i++)
				{
					W.anim[asel][0][i]=W.anim[asel][0][i+1];
					W.anim[asel][1][i]=W.anim[asel][1][i+1];
					W.anim[asel][2][i]=W.anim[asel][2][i+1];	
				}
			}
		}
	}
	else if (inmenu==35)
	{
		if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0)
		{
			inmenu = 34;
		}
		if (get_key(KEY_RETURN) && get_keypressed(KEY_RETURN) == 0)
		{
			playing=!playing;
		}
		if (get_key(KEY_NUMPAD0) && get_keypressed(KEY_NUMPAD0) == 0)
		{
			playing=false;
			W.ChangeAnim(asel,true);
			timer=0;
		}
		if (get_key(KEY_RIGHT) && (get_keypressed(KEY_RIGHT) % (l_targetfps/3) == 0 || get_keypressed(KEY_RIGHT) > l_targetfps))
		{
			W.Update();
			timer++;
		}
	}
	else if (inmenu==32)
	{	
		int columns = 6, lines=columns*(ymax-11)/xmax;
		if (get_key(KEY_UP) && (get_keypressed(KEY_UP) % (l_targetfps/3) == 0 || (get_keypressed(KEY_UP) > l_targetfps && get_keypressed(KEY_UP) % 2 == 0)))
		{
			vsely--;
			if (vsely < 0) vsely=W.frames/columns;
		}
		if (get_key(KEY_DOWN) && (get_keypressed(KEY_DOWN) % (l_targetfps/3) == 0 || (get_keypressed(KEY_DOWN) > l_targetfps && get_keypressed(KEY_DOWN) % 2 == 0)))
		{
			vsely++;
			if (vsely > W.frames/columns) vsely=0;
		}
		if (get_key(KEY_LEFT) && (get_keypressed(KEY_LEFT) % (l_targetfps/3) == 0 || (get_keypressed(KEY_LEFT) > l_targetfps && get_keypressed(KEY_LEFT) % 2 == 0)))
		{
			vselx--;
			if (vselx < 0) vselx=columns-1;
		}
		if (get_key(KEY_RIGHT) && (get_keypressed(KEY_RIGHT) % (l_targetfps/3) == 0 || (get_keypressed(KEY_RIGHT) > l_targetfps && get_keypressed(KEY_RIGHT) % 2 == 0)))
		{
			vselx++;
			if (vselx > columns-1) vselx=0;
		}
		if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0)
		{
			inmenu=3;
		}
		if (vsely == W.frames/columns && vselx >= (W.frames%columns)) vselx=0;
		if (vsely<vstart)
		{
			vstart--;
			vfinish--;
		}
		if (vsely>(vfinish-1))
		{
			vstart++;
			vfinish++;
		}
		if (get_key(KEY_NUMPAD5) && get_keypressed(KEY_NUMPAD5) == 0 && W.frames < LSPRITE_FRAMES_MAX)
		{
			int sts = vselx+vsely*columns;
			W.frames++;
			W.frameOffsets[0][W.frames-1]=W.frameOffsets[0][sts];	W.frameOffsets[1][W.frames-1]=W.frameOffsets[1][sts];
			W.frameSizes[0][W.frames-1]=W.frameSizes[0][sts];		W.frameSizes[1][W.frames-1]=W.frameSizes[1][sts];
			W.frameFix[0][W.frames-1]=W.frameFix[0][sts];			W.frameFix[1][W.frames-1]=W.frameFix[1][sts];
		}
		if (get_key(KEY_NUMPAD0) && get_keypressed(KEY_NUMPAD0) == 0)
		{
			if (W.frames!=1)
			{
				W.frames--;
				int sts = vselx+vsely*columns;
				for (int i=sts; i<W.frames; i++)
				{
					W.frameOffsets[0][sts]=W.frameOffsets[0][sts+1];	W.frameOffsets[1][sts]=W.frameOffsets[1][sts+1];
					W.frameSizes[0][sts]=W.frameSizes[0][sts+1];		W.frameSizes[1][sts]=W.frameSizes[1][sts+1];
					W.frameFix[0][sts]=W.frameFix[0][sts+1];			W.frameFix[1][sts]=W.frameFix[1][sts+1];
				}
				if (vsely == W.frames/columns && vselx == ((W.frames)%columns)) vselx--;
				if (vselx<0)
				{
					vsely--;
					vselx=columns-1;
				}
			}
		}
	}
	else 
	{
		if (get_key(KEY_UP) && (get_keypressed(KEY_UP) % (l_targetfps/3) == 0 || (get_keypressed(KEY_UP) > l_targetfps && get_keypressed(KEY_UP) % 2 == 0)))
		{
			sel--;
			if (sel < 1) sel=selmax;
		}
		if (get_key(KEY_DOWN) && (get_keypressed(KEY_DOWN) % (l_targetfps/3) == 0 || (get_keypressed(KEY_DOWN) > l_targetfps && get_keypressed(KEY_DOWN) % 2 == 0)))
		{
			sel++;
			if (sel > selmax) sel=1;
		}
		if (get_key(KEY_RETURN) && get_keypressed(KEY_RETURN) == 0)
		{
			inmenu = inmenu*10+sel;
			sel = 1;
		}
		if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0)
		{
			inmenu = inmenu/10;
			sel = 1;
		}
		if (get_key(KEY_NUMPAD0) && get_keypressed(KEY_NUMPAD0) == 0 && inmenu==330)
		{
			if (W.anims!=1)
			{
				W.anims--;
				for (int i=sel; i<W.anims; i++)
				{
					for (int j=0; j<W.animsize[i]; j++)
					{
						for (int k=0; k<3; k++)
						{
							W.anim[i][k][j] = W.anim[i+1][k][j];
						}
					}
					W.animsize[i] = W.animsize[i+1];
				}
			}
		}
		if (get_key(KEY_F3) && get_keypressed(KEY_F3) == 0 && inmenu==330)
		{
			asel=sel-1;
			playing=false;
			W.ChangeAnim(sel-1,true);
			inmenu=35;
		}
	}
}

int main(int argc, char** argv)
{
	if(!set_read())
	{
		fprintf(stderr, "Failed to load settings\n");
        return 1;							
	}

	if (exec() != 0)
	{
		Quit(1);
	}

	
	/* init() from Game.cpp */
	if (init() != 0)
	{
		Quit(1);
	}
	
	buildLayer(200,320,200);
	while(1)
	{
		if (process() !=0) Quit(1);
		OP();		
		KOP();
		
	}
	
	return 0;
}