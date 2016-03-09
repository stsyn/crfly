#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "Lib.h"
#include "Updater.h"
#include "Platform.h"
#include "Game.h"
#include "GameFiles.h"
#include "Updater.h"

int clr, nclr[3], quality=0;
const int strmax = 46, scrmax = 38;
unsigned int sz=100;

char str[strmax+1][64];
int scrs[scrmax][2];

int sel=1, selmax=4, dmode=-1,dsel=0,tmax=0;
int lc = 0;
int f=0;
int r=5;
int *mas[2];

int modif=0, scrn=0, c=0, gs, pg;
bool dbg=false;
int inmenu=1;

unsigned long int gframe=0;

//LOGIC
const int mplayers = 2, maxbg=2, maxfg=2, maxabonuses=17;
int short unsigned tways, pos[mplayers], map[mplayers][22][6], delay, gamestate, anim[mplayers], doloykamni[mplayers],tbg,tfg,cbg,cfg,tcfix;
long unsigned int  smoothstuff[mplayers][5], scores[mplayers][6], tmult=1,xmult[mplayers],tdelay,shipmove[mplayers];
long int bonuses[mplayers][maxabonuses];
int bg[maxbg][3], fg[maxfg][3], level;
unsigned int css[mplayers][3], tcss, ttcss[3], cclr[2][3], shipstyle[2],tshipstyle, maxships, ccss=0;
void LS();
void ingame();

bool set_read()
{
	FILE *set_file;
	char set_path[] = SETTINGS_DAT;
	int tmp;
	printf("set_read: %s\n", set_path);
	set_file = fopen(set_path, "r"); 
	fscanf(set_file, "%i %i", &rxmax, &rymax);
	fscanf(set_file, "%i %i", &tmp, &quality);
	if (tmp == 1) 
	{
		fullscreen=true;
	}
	else
	{
		fullscreen=false;
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
	fullscreen ? tmp=1 : tmp=0;
	fprintf(set_file, "%i %i", tmp, quality);
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
		LS();
	}
	fclose(set_file);
	return true;
}

void css_write()
{
	FILE *css_file;
	char css_path[] = STYLEDAT_DAT;
	css_file = fopen(css_path, "w" ); 
	fprintf(css_file, "%x %x %x %i ", css[0][0], css[0][1], css[0][2], shipstyle[0]);
	fprintf(css_file, "%x %x %x %i", css[1][0], css[1][1], css[1][2], shipstyle[1]);
	fclose(css_file);
}

void hsc_read()
{
	FILE *css_file;
	long unsigned int tmp;
	char css_path[] = HIGSCORE_DAT;
	css_file = fopen(css_path, "r" ); 
	fscanf(css_file, "%li", &tmp);
	scores[0][0] = (~tmp & 0x7FFFFFFF)/2-78;
	fclose(css_file);

	if (scores[0][0] < 1000) maxships = 0;
	else if (scores[0][0] < 2500) maxships = 1;
	else if (scores[0][0] < 5000) maxships = 2;
	else if (scores[0][0] < 10000) maxships = 3;
	else /*if (scores[0][0] < 20000)*/ maxships = 4;
	//else maxships = 5;
}

void hsc_write()
{
	FILE *css_file;
	long unsigned int tmp;
	char css_path[] = HIGSCORE_DAT;
	css_file = fopen(css_path, "w" ); 
	tmp = ~((scores[0][0]+78)*2) & 0x7FFFFFFF;
	fprintf(css_file, "%li", tmp);
	fclose(css_file);
}

void hsc_reset()
{
	FILE *css_file;
	char css_path[] = HIGSCORE_DAT;
	css_file = fopen(css_path, "w" ); 
	fprintf(css_file, "%li",~(1078*2) & 0x7FFFFFFF);
	scores[0][0] = 0;
	shipstyle[0] = 0;
	shipstyle[1] = 0;
	maxships = 0;
	css_write();
	fclose(css_file);
}

void css_read()
{
	FILE *css_file;
	char css_path[] = STYLEDAT_DAT;
	css_file = fopen(css_path, "r" ); 
	fscanf(css_file, "%x %x %x %i", &css[0][0], &css[0][1], &css[0][2], &shipstyle[0]);
	fscanf(css_file, "%x %x %x %i", &css[1][0], &css[1][1], &css[1][2], &shipstyle[1]);
	fclose(css_file);
	if ((shipstyle[0] > maxships) || (shipstyle[1] > maxships)) hsc_reset();
}

void str_read()
{
	FILE *str_file;
	str_file = fopen(LANGDATA_DAT, "r" ); 
	for (int i=0; i<strmax; i++)
	{
		LS();
		fscanf(str_file,"%[^\n]\n",str[i]);
	}
	fclose(str_file);
}

/////////////////////////////////////////////////

int init()
{
	buildLayer(128,xmax,ymax);
	buildLayer(129,xmax,ymax);

	mas[0] = new int[sz];
	mas[1] = new int[sz];
	srand(unsigned(time(NULL)));
	

	DrawRect(128,tRGB(0,0,0),0,0,xmax,ymax,0);
	DrawRect(129,tRGB(0,0,0),0,0,xmax,ymax,0);

	printf("init: time=%u xmax=%i ymax=%i\n", unsigned(time(NULL)), xmax, ymax);

	for (unsigned j=0; j<sz; j++) 
	{
		mas[0][j]=gener(xmax);
		mas[1][j]=gener(ymax);
	}
	font_init();

	c=0;
	
	////////////////////////////////////////////////////
	buildLayer(200,xmax,ymax);
	buildLayer(510,32,80);
	LS();
	scr_read();

	if (Updater() != 0)
	{
		Sleep(4000);
		return 1;
	}

	hsc_read();

	css_read();

	tex_init();
	LS();
	tex_read(0);
	tex_read(1);

	str_read();
	return 0;
}

void close()
{
	delete[] mas[0];
	delete[] mas[1];
}

void gamereset()
{
	//for UI
	buildLayer(201,xmax,ymax);
	//creating tiles
	buildLayer(511, 12, 200);
	buildLayer(509, 12, 200);
	HGradient(511,tRGB(64,64,64),tRGB(196,196,196),0,0,4,200,0);
	HGradient(511,tRGB(196,196,196),tRGB(64,64,64),4,0,8,200,0);
	int i;
	for (i=1; i<=23; i++) 
	{
		if ((i != 9 || quality > 0) && (i < 10 || quality > 1)) tex_read(i);
	}
	//i wanna yo ship
	buildLayer(510, 32, 80);
	MascedLayer(17,510,0,0,css[0][0],css[0][1],css[0][2]);	
	buildLayer(506, 24, 14);
	MascedLayer(16+8,506,0,0,tRGB(255,255,255),tRGB(255,255,255),css[0][2]);
	buildLayer(508, 18, 18);
	MascedLayer(16+7,508,0,0,css[0][0],tRGB(255,255,255),tRGB(255,255,255));
	//player 2 u too
	if (pg>0)
	{
	buildLayer(410, 32, 80);
	MascedLayer(17,410,0,0,css[1][0],css[1][1],css[1][2]);	
	buildLayer(406, 24, 14);
	MascedLayer(16+8,406,0,0,tRGB(255,255,255),tRGB(255,255,255),css[1][2]);
	buildLayer(408, 18, 18);
	MascedLayer(16+7,408,0,0,css[1][0],tRGB(255,255,255),tRGB(255,255,255));
	}
	//do it for texts
	buildLayer(505, 1, 1);
	//progressbar
	buildLayer(507, 36, 12);
	for (i=0; i<12; i++) 
	{
		FDrawRect(507,tRGB(255,255,255),i*3,0,2,12);
	}
	HGradient(507,tRGB(255,0,0),tRGB(255,255,0),0,0,18,12,2);
	HGradient(507,tRGB(255,255,0),tRGB(0,255,0),18,0,18,12,2);
	//another shit
	tbg = 0;
	cbg = 0;
	tfg = 0;
	cfg = 0;
	frame=0;
	gframe=0;
	tways=3;
	for (i=0; i<6; i++) 
	{
		scores[0][i]=0;
		scores[1][i]=0;
		for (int j=0; j<20; j++) 
		{
			map[0][j][i]=0;
			map[1][j][i]=0;
		}
	}
	for (i=0; i<maxabonuses; i++) 
	{
		bonuses[0][i]=0;
		bonuses[1][i]=0;
	}
	for (i=0; i<5; i++) 
	{
		smoothstuff[0][i]=0;
		smoothstuff[1][i]=0;
	}
	for (i=0; i<maxbg; i++)
	{
		for (int j=0; j<3; j++) bg[i][j]=0;
	}
	for (i=0; i<maxfg; i++)
	{
		for (int j=0; j<3; j++) fg[i][j]=0;
	}

	scores[0][5] = 3;
	scores[0][3] = 3;
	scores[1][3] = 3;
	pos[0]=1;
	pos[1]=1;
	gamestate = 0;
	tcfix = 0;
	smoothstuff[0][0]= 3;
	level = 0;
	hsc_read();
	smoothstuff[0][1] = scores[0][0];
	cclr[0][0] = 0;
	cclr[1][0] = 0;
	cclr[0][1] = 0;
	cclr[1][1] = 192;
	cclr[0][2] = 32;
	cclr[1][2] = 192;
	if (quality == 0) 
	{
		IncludeLayer(511,509,0,0,0);
		DrawRect(509,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),0,0,12,180,2);
	}
}

void gameover()
{
	if (scores[0][0]<=scores[0][1] && pg == 0) 
	{
		hsc_write();
		hsc_read();
	}
	scrn=0;
	delLayer(511);
	delLayer(510);
	delLayer(509);
	delLayer(508);
	delLayer(507);
	delLayer(506);
	delLayer(505);
	if (pg >0)
	{
	delLayer(410);
	delLayer(408);
	delLayer(406);
	}
}

/*

  Hey there, you need to know what scores[x] are mean!
  0 - HiScore
  1 - Total score
  2 - Only score
  3 - Lives counter
  4 - Distance
  5 - Delay

*/

void LS(char *str)
{

	DrawRect(200,tRGB(0,0,0),0,0,xmax,ymax);
	PrintText(200,1,"Loading...",tRGB(127,127,127),125,130);
	PrintText(200,1,str,tRGB(127,127,127),0,144);
	DrawRect(200,tRGB(127,127,127),20,160,280,20);
	DrawRect(200,tRGB(0,0,0),21,161,278,18);
	DrawRect(200,tRGB(127,127,127),lc-20,162,40,16);
	DrawRect(200,tRGB(0,0,0),0,162,20,16);
	DrawRect(200,tRGB(0,0,0),300,162,20,16);
	lc+=10;
	if (lc == 320) lc = 0;
	renderer(200);
}

void LS()
{

	DrawRect(200,tRGB(0,0,0),0,0,xmax,ymax);
	PrintText(200,1,"Loading...",tRGB(127,127,127),125,140);
	DrawRect(200,tRGB(127,127,127),20,160,280,20);
	DrawRect(200,tRGB(0,0,0),21,161,278,18);
	DrawRect(200,tRGB(127,127,127),lc-20,162,40,16);
	DrawRect(200,tRGB(0,0,0),0,162,20,16);
	DrawRect(200,tRGB(0,0,0),300,162,20,16);
	lc+=10;
	if (lc == 320) lc = 0;
	renderer(200);
}

const int inmenu_color=tRGB(196,196,196), inmenu_selected=tRGB(224,224,0);

void Debug()
{
	FDrawRect(200,tRGB(0,0,0),0,0,xmax,ymax);
	char strn[64];
	int tc,cl;
	tmax=0;
	if (dmode == -1) 
	{
		tc=0;
		for (int i=0; i<=512; i++) if (getWidth(i) !=0) tmax++;
		for (i=0; i<tmax; i++)
		{
			for (; tc<=512; tc++) 
			{
				if (getWidth(tc) !=0) break;
			}

			if (dsel == i) 
			{
				cl = inmenu_selected;
			}
			else 
			{
				cl = inmenu_color;
			}
			sprintf(strn,"%i (%ix%i)",tc,getWidth(tc),getHeight(tc));
			PrintText(200,1,strn,cl,100,95-(dsel*11)+i*11);
			tc++;
		}
		sprintf(strn,"Loaded: %i",tmax);
		PrintText(200,1,strn,inmenu_color,0,0);
	}
	else
	{
		IncludeLayer(dmode,200,0,0,0);
	}
	renderer(200);
}

void MM()
{
	int cl;
	char strn[64];
	DrawRect(200,tRGB(0,0,0),0,0,xmax,ymax);
	if (inmenu==1) //Main menu
	{
		selmax = 5;
		int i;
		for (i=1; i<=selmax; i++)
		{
			if (sel == i) 
			{
				cl = inmenu_selected;
			}
			else 
			{
				cl = inmenu_color;
			}

			if (sel != 2 || i!=2) PrintText(200,1,str[i],cl,100,80+i*20);
			else
			{
				sprintf(strn,"%s P%i",str[i],ccss+1);
				PrintText(200,1,strn,cl,100,80+i*20);
			}
		}

		for (i = 0; i<3; i++) ttcss[i] = 0;
		tshipstyle = 255;
	}
	if (inmenu==11) //StartGame menu
	{
		selmax=3;
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
			PrintText(200,1,str[i+17],cl,100,80+i*20);
		}
	}
	if (inmenu==111) //launching
	{
		LS();
		pg = 1;
		gs = 0;
		gamereset();
		LS();
		inmenu=11;
		LS();
		scrn=1;
	}
	if (inmenu==112) //launching
	{
		inmenu=11;
	}
	if (inmenu==113) //launching
	{
		LS();
		pg = 2;
		gs = 0;
		gamereset();
		LS();
		inmenu=11;
		LS();
		scrn=1;
	}
	if (inmenu==12) //Customisation
	{
		if (ttcss[0] == 0) for (int i = 0; i<3; i++) ttcss[i] = css[ccss][i];
		selmax=5;
		tcss = 0;
		if (tshipstyle == 255) tshipstyle = shipstyle[ccss];
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
			if (i < 4) sprintf(strn,"%s (%x)",str[i+34],ttcss[i-1]);
			else sprintf(strn,str[i+34]);
			PrintText(200,1,strn,cl,36,80+i*15);
		}
		sprintf(strn,"Player %i",ccss+1);
		PrintText(200,1,strn,inmenu_color,40,60);
		DrawRect(510,tRGB(127,127,127),0,0,32,80);
		MascedLayer(17,510,0,0,ttcss[0],ttcss[1],ttcss[2]);
		ZoomRender(510,200,180,40,120,120,(16*((frame >> 2) & 1)),16*tshipstyle,16,16,0);
	}
	if (inmenu>=121 && inmenu<=123)
	{
		if (tcss == 0) tcss = ttcss[(inmenu % 10) - 1];
		int dr,dg,db;
		RGBt(tcss,&dr,&dg,&db);
		selmax=3;
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
			if (i == 1) sprintf(strn,"Red (%i)",dr);
			else if (i == 2) sprintf(strn,"Green (%i)",dg);
			else sprintf(strn,"Blue (%i)",db);
			PrintText(200,1,strn,cl,36,80+i*15);
		}
		DrawRect(510,tRGB(127,127,127),0,0,32,80);
		if (inmenu == 121) MascedLayer(17,510,0,0,tcss,ttcss[1],ttcss[2]);
		else if (inmenu == 122) MascedLayer(17,510,0,0,ttcss[0],tcss,ttcss[2]);
		else MascedLayer(17,510,0,0,ttcss[0],ttcss[1],tcss);
		ZoomRender(510,200,180,40,120,120,(16*((frame >> 2) & 1)),16*tshipstyle,16,16,0);
	}
	if (inmenu>=1211 && inmenu<=1233)
	{
		ttcss[((inmenu/10) % 10) - 1] = tcss;
		inmenu = 12;
	}
	if (inmenu==124)
	{	
		selmax=maxships+1;
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
			PrintText(200,1,str[i+39],cl,40,80+i*15);
		}
		DrawRect(510,tRGB(127,127,127),0,0,32,80);
		MascedLayer(17,510,0,0,ttcss[0],ttcss[1],ttcss[2]);
		ZoomRender(510,200,180,40,120,120,(16*((frame >> 2) & 1)),16*(sel-1),16,16,0);
	}
	if (inmenu>=1241 && inmenu<=1245)
	{
		tshipstyle = (inmenu % 10) -1;
		inmenu = 12;
	}
	if (inmenu==125)
	{	
		for (int i = 0; i<3; i++) 
		{
			css[ccss][i] = ttcss[i];
			ttcss[i] = 0;
		}
		shipstyle[ccss] = tshipstyle;
		tshipstyle = 255;
		css_write();
		inmenu = 1;
		css_read();
	}
	if (inmenu==13) //Options menu
	{
		selmax=3;
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
			PrintText(200,1,str[i+6],cl,100,80+i*20);
		}
		if (sel == 3) PrintText(200,1,str[quality+31],tRGB(128,128,0),180,80+3*20);
		else PrintText(200,1,str[quality+31],tRGB(127,127,127),180,80+3*20);
	}
	if (inmenu==131) //goto resolution
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
	if (inmenu==132) //Change window/fullscreen
	{
		inmenu=13;
		fullscreen=!fullscreen;
		reload();
		set_write();
	}
	if (inmenu==133) //Quality
	{
		selmax=4;
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
			PrintText(200,1,str[i+30],cl,100,80+i*20);
		}
	}
	if (inmenu>=1331 && inmenu<=1334) //Quality
	{
		quality = (inmenu % 10)-1;
		set_write();
		inmenu=13;
	}
	if (inmenu==14) //help
	{
		selmax=3;
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

			PrintText(200,1,str[i+21],cl,100,80+i*20);
		}
		//PrintText(200,0,str[21],cl,80,80);
	}
	if (inmenu==141) //help
	{
		inmenu=14;
	}
	if (inmenu==142) //about
	{
		sel = 1;
		sprintf(strn, "CRFly %s", version_name);
		PrintText(200,1,strn,inmenu_color,60,10+20);
		PrintText(200,1,"Based on CRF(ly) v0.94",inmenu_color,60,10+30);
		PrintText(200,1,"Developed by St@SyaN 2016",inmenu_color,60,10+50);
		PrintText(200,1,"Using OpenGL",inmenu_color,60,10+40);
	}
	if (inmenu==1421)
	{
		inmenu=14;
	}
	if (inmenu==143) //extras
	{
		selmax=3;
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

			PrintText(200,1,str[i+24],cl,100,80+i*20);
		}
	}
	if (inmenu==1431)
	{
		DrawRect(129,tRGB(0,0,0),0,0,xmax,ymax,0);
		inmenu = 143;
		modif = 1;
	}
	if (inmenu==1432)
	{
		frame = 0;
		inmenu = 143;
		modif = 2;
	}
	if (inmenu==1433)
	{
		inmenu = 143;
		modif = 3;
	}
	if (inmenu==15) //exit
	{
		printf("MM: exit");
		Quit(0);
	}
	if (inmenu==20) //return fron resolution
	{
		inmenu=13;
	}
	if (inmenu==200) //change resolution
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
			PrintText(200,1,strn,cl,100,95-(sel*11)+i*11);
		}
	}
	if (inmenu>2000 && inmenu<2100) //enable resolution
	{
		rxmax = scrs[inmenu-2001][0];
		rymax = scrs[inmenu-2001][1];
		reload();
		set_write();
		inmenu=13;
	}
	sprintf(strn,"%i | %i | %i",fps,frameskip, dticks);
	PrintText(200,1,strn,inmenu_color,0,0);
	if (frame % (frameskip+1)== 0) renderer(200);

}

//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////

void renderBG()
{
	int i, j;
	if (quality>0) //stars
		{
			c = (scores[0][4]*2 +(delay*2/smoothstuff[0][0]))% ymax;
			for (i = 0; i<sz; i++)
			{
				if (mas[1][i]+c<ymax) plot(200, mas[0][i],mas[1][i]+c, 4282400832);
				else plot(200, mas[0][i],mas[1][i]+c-ymax, 4282400832);
			}
			c = (scores[0][4]*3 +(delay*3/smoothstuff[0][0]))% ymax;
			for (i = 0; i<sz; i++)
			{
				if (mas[1][i]+c<ymax) plot(200, xmax-mas[0][i],mas[1][i]+c, 4286611584);
				else plot(200, xmax-mas[0][i],mas[1][i]+c-ymax, 4286611584);
			}
			c = (scores[0][4]*6 +(delay*6/smoothstuff[0][0]))% ymax;
			for (i = 0; i<sz; i++)
			{
				if (-mas[1][i]+c<0) plot(200, mas[0][i],ymax-mas[1][i]+c, 4291085508);
				else plot(200, mas[0][i],-mas[1][i]+c, 4291085508);
			}
		}

		if (quality>1) //bg objects
		{
			int atbg = -1;
			c = (scores[0][4]*8 +(delay*8/smoothstuff[0][0]));
			for (i = 0; i<tbg; i++)
			{
				bg[i][2]+=c-cbg;
				TransformLayer(bg[i][0],200,bg[i][1],bg[i][2],0,0,getWidth(bg[i][0]),getHeight(bg[i][0]),3);
				if (bg[i][2] > ymax) atbg = i;
			}
			cbg = c;
			if (atbg != -1)
			{
				for (i = atbg; i<tbg-1; i++)
				{
					for (j=0;j<3;j++) bg[i][j] = bg[i+1][j];
				}
				tbg--;
			}
			if (tbg < maxbg)
			{
				int tt;
				if (tbg == 0) tt = ymax;
				else tt = bg[tbg-1][2];
				tt = gener(6*(smoothstuff[0][0]/4+ymax-tt));
				if (tt == 0)
				{
					int t2;
					t2 = 16+9+gener(14);
					bg[tbg][0] = t2;
					bg[tbg][2] = -getHeight(t2);
					bg[tbg][1] = -getWidth(t2)/2 + gener(xmax+getWidth(t2));
					tbg++;
				}
			}
		}
}

void renderFG()
{
	int i, j;
	if (quality>1) //objects
		{
			int atfg = -1;
			c = (scores[0][4]*36 +(delay*36/smoothstuff[0][0]));
			for (i = 0; i<tfg; i++)
			{
				fg[i][2]+=c-cfg;
				ZoomRender(fg[i][0],200,fg[i][1],fg[i][2],getWidth(fg[i][0])*4,getHeight(fg[i][0])*4,0,0,getWidth(fg[i][0]),getHeight(fg[i][0]),0);
				if (fg[i][2] > ymax) atfg = i;
			}
			cfg = c;
			if (atfg != -1)
			{
				for (i = atfg; i<tfg-1; i++)
				{
					for (j=0;j<3;j++) fg[i][j] = fg[i+1][j];
				}
				tfg--;
			}
			if (tfg < maxfg)
			{
				int tt;
				if (tfg == 0) tt = ymax;
				else tt = fg[tfg-1][2];
				tt = gener(4*(smoothstuff[0][0]/5+ymax-tt));
				if (tt == 0)
				{
					int t2;
					t2 = 16+9+gener(2);
					fg[tfg][0] = t2;
					fg[tfg][2] = -4*getHeight(t2);
					fg[tfg][1] = -getWidth(t2)*2 + gener(xmax+getWidth(t2)*4);
					tfg++;
				}
			}
		}
}

void delayCalc()
{
	if (bonuses[0][7] == 0)
	{
		if (bonuses[0][6] != 0) tdelay = scores[0][5] >> 1;
		else if (bonuses[0][4] != 0) tdelay = scores[0][5] << 1;
		else if (bonuses[0][5] != 0) tdelay = scores[0][5] >> 1;
		else tdelay = scores[0][5];
	}
	else 
	{
		if (bonuses[0][6] != 0) tdelay = scores[0][5] << 1;
		else if (bonuses[0][4] != 0) tdelay = scores[0][5] >> 1;
		else if (bonuses[0][5] != 0) tdelay = scores[0][5] << 1;
		else tdelay = scores[0][5];
	}

	if (tdelay>40) tdelay = 40;
}

void Smoothing(int ole)
{
int i;
for (unsigned int k = 0; k<tmult; k++)
{
	for (i = 0; i<3; i++)
	{
		if (smoothstuff[ole][i+1] < scores[ole][i]) smoothstuff[ole][i+1]++;
		else if (smoothstuff[ole][i+1] > scores[ole][i]) smoothstuff[ole][i+1]--;
	}
	for (i = 0; i<3; i++)
	{
		if (smoothstuff[ole][i+1] < scores[ole][i]) smoothstuff[ole][i+1]++;
		else if (smoothstuff[ole][i+1] > scores[ole][i]) smoothstuff[ole][i+1]--;
	}
	if (frame % 4 == 0)
	{
		if (smoothstuff[ole][0] < tdelay) smoothstuff[ole][0]++;
		else if (smoothstuff[ole][0] > tdelay) smoothstuff[ole][0]--;
	}	
	shipmove[ole] = smoothstuff[ole][0] >> 2; 
	if (frame % 8 == 0)
	{
		if (smoothstuff[ole][4] < shipmove[ole]) smoothstuff[ole][4]++;
		else if (smoothstuff[ole][4] > shipmove[ole]) smoothstuff[ole][4]--;
	}
}
}

void multCalc(int ole)
{
int	mult=8;
	if (bonuses[ole][7] == 0)
	{
		if (bonuses[ole][0] != 0) mult = mult << 1;
		if (bonuses[ole][1] != 0) mult = mult << 2;
		if (bonuses[ole][2] != 0) mult = mult >> 1;
	}
	else 
	{
		if (bonuses[ole][0] != 0) mult = mult >> 1;
		if (bonuses[ole][1] != 0) mult = mult >> 2;
		if (bonuses[ole][2] != 0) mult = mult << 1;
	}
	xmult[ole] = mult * tmult;
}

void generation(int ole)
{
unsigned long int j,t;
	for (int k=20; k>=0; k--)
		{
			for (j=0; j<6; j++) map[ole][k+1][j] = map[ole][k][j];
		}
		for (j=0; j<6; j++) map[ole][0][j] = 0;
		t = gener(50);
		if (t % 4 == 0) 
		{
			t = gener(tways);
			map[ole][0][t] = 1; //coin
			doloykamni[ole] = 0;
		}
		else if (t % 4 == 1) 
		{
			if ((doloykamni[ole] < tways-1) || (tmult > 7)) 
			{
				t = gener(tways);
				map[ole][0][t] = 2; //meteor
				doloykamni[ole]++;
			}
			else
			{
				doloykamni[ole] = 0;
			}

		}
		else if (t == 2 || (bonuses[ole][11] !=0 && (t == 3 || t == 6 || t == 7))) 
		{
			t = gener(tways);
			int t2 = gener(maxabonuses);
			map[ole][0][t] = 100+t2; //bonus
			doloykamni[ole] = 0;
		}
		else
		{
			doloykamni[ole] = 0;
		}
}

void speedUpd()
{
if (scores[0][5] < 2) scores[0][5] = 2;

		if (scores[0][4] <= 18 && scores[0][4] >= 10) scores[0][5]+=3;

		if (scores[0][4] % 25 == 0)
		{
			if (scores[0][5] > 20 || scores[0][4] % 50 == 0)
			{
				if (scores[0][5] > 15 || scores[0][4] % 100 == 0)
				{
					if (scores[0][5] >= 24) scores[0][5]-=5;
					else if (scores[0][5] >= 11) scores[0][5]-=2;
					else if (scores[0][5] > 2) scores[0][5]--;
				}
			}
		}
}

void bonusing(int ole)
{
	int i;
	if (bonuses[0][8] == 0) for (i=0; i<maxabonuses; i++)
		{
			if (bonuses[ole][i] != 0) bonuses[ole][i]--;
			if (i != 10 && bonuses[0][10] != 0 && bonuses[ole][i] != 0 && bonuses[0][7] == 0) bonuses[ole][i]--; 
			if (i != 10 && bonuses[0][10] != 0 && bonuses[ole][i] != 0 && bonuses[0][7] != 0 && (scores[ole][4] & 1) == 0) bonuses[ole][i]++;

		}
	if (ole == 1) return;
		if (bonuses[0][8] != 0) 
		{
			bonuses[ole][8]--;
			if (bonuses[0][10] != 0 && bonuses[0][7] == 0) bonuses[0][8]--;
			if (bonuses[0][10] != 0 && bonuses[0][7] != 0 && (scores[ole][4] & 1) == 0) bonuses[0][8]++;
		}
}

void magnetizm(int ole)
{
	int i;
	if (bonuses[ole][9] != 0)
	{
		for (i = 0; i<tways; i++)
		{
			if (map[ole][17][i] == 1)
			{
				map[ole][17][i] = 0;
				if (i>pos[ole]) map[ole][17][i-1] = 1;
				else if (i<pos[ole]) map[ole][17][i+1] = 1;
				else map[ole][17][i] = 1;
				break;
			}
			else if (map[ole][17][i] == 3 && bonuses[ole][7] != 0)
			{
				map[ole][17][i] = 0;
				if (i>pos[ole]) map[ole][17][i-1] = 3;
				else if (i<pos[ole]) map[ole][17][i+1] = 3;
				else map[ole][17][i] = 3;
				break;
			}
		}
	}
}

void triggers(int ole)
{
if (map[ole][17][pos[ole]] != 0)
	{
		if (map[ole][17][pos[ole]] == 1) 
		{
			scores[ole][2]+=2.5*xmult[ole];
		}
		else if (map[ole][17][pos[ole]] == 2) 
		{
			if (bonuses[ole][3] == 0 && bonuses[ole][6] == 0)
			{
				scores[ole][3]--;
				anim[0] = 3;
				anim[1] = 10;
				bonuses[ole][3]+=7;
			}
			else scores[ole][2]+=5;
		}
		else if ((map[ole][17][pos[ole]] >= 100) && (map[ole][17][pos[ole]] < 200)) 
		{
			//ooohhhh yeeeeaaaahh bonuses
			int t = map[ole][17][pos[ole]] - 100;
			if (t == 0) //x2 coins
			{
				bonuses[ole][0]+=75;
			}
			else if (t == 1) //x4 coins 
			{
				bonuses[ole][1]+=40;
			}
			else if (t == 2) //x0.5 coins 
			{
				bonuses[ole][2]+=75;
			}
			else if (t == 3) //shield 
			{
				bonuses[ole][3]+=75;
			}
			else if (t == 4) //slowmode 
			{
				bonuses[0][4]+=(41-scores[0][5])*2;
				if (bonuses[0][4] < 0) bonuses[0][4] = 0;
			}
			else if (t == 5) //fastmode 
			{
				bonuses[0][5]+=75;
			}
			else if (t == 6) //extreme 
			{
				if (pg == 1) bonuses[ole][6]+=125;
				else 
				{
					bonuses[0][5]+=125;
					bonuses[ole][3]+=125;
				}
			}
			else if (t == 7) //inverse 
			{
				bonuses[0][7]+=150;
			}
			else if (t == 8) //freeze
			{
				bonuses[0][8]+=70;
			}
			else if (t == 9) //magnet 
			{
				bonuses[ole][9]+=75;
			}
			else if (t == 10) //time killer 
			{
				bonuses[0][10]+=50;
			}
			else if (t == 11) //more bonuses
			{
				bonuses[ole][11]+=20;
			}
			else if (t == 12) //life up +
			{
				anim[0] = 2;
				anim[1] = 20;
				if (bonuses[ole][7] == 0) scores[ole][3]++;
				else scores[ole][3]--;
			}
			else if (t == 13) //life down +
			{
				anim[0] = 3;
				anim[1] = 10;
				if (bonuses[ole][7] == 0 && bonuses[ole][6] == 0 && bonuses[ole][3] ==0) scores[ole][3]--;
				else scores[ole][3]++;
				bonuses[ole][3]+=10;
			}
			else if (t == 14) //jackpot +
			{
				anim[0] = 4;
				anim[1] = 20;
				if (bonuses[ole][7] == 0) scores[ole][2]+=500;
				else scores[ole][2]-=500;
			}
			else if (t == 15) //add slow +
			{
				anim[0] = 5;
				anim[1] = 20;
				if (bonuses[ole][7] == 0) scores[ole][5]+=2;
				else scores[ole][5]-=2;
			}
			else if (t == 16) //viewer
			{
				bonuses[ole][16]+=100;
			}

		}
		map[ole][17][pos[ole]] = 0;
	}

	if (scores[ole][3]>0) scores[ole][1] = scores[ole][2] + scores[ole][4];
	if (scores[ole][0]<scores[ole][1]) scores[ole][0] = scores[ole][1];
}

void colorStuff()
{
	if (anim[1] != 0) anim[1]--;
	else anim[0] = 0;

	//smooth colors
	if (gframe % smoothstuff[0][0]*2 == 0 && quality>0)
		{
		if (cclr[1][0] > 0 && cclr[1][1] == 192)
		{
			cclr[1][0]--;
			cclr[1][2]++;
		}
		if (cclr[1][1] > 0 && cclr[1][2] == 192)
		{
			cclr[1][1]--;
			cclr[1][0]++;
		}
		if (cclr[1][2] > 0 && cclr[1][0] == 192)
		{
			cclr[1][2]--;
			cclr[1][1]++;
		}
		tcfix++;

		if (tcfix == 6)
		{
			if (cclr[0][0] > 0 && cclr[0][2] == 0)
			{
				cclr[0][0]--;
				cclr[0][1]++;
			}
			if (cclr[0][1] > 0 && cclr[0][0] == 0)
			{
				cclr[0][1]--;
				cclr[0][2]++;
			}
			if (cclr[0][2] > 0 && cclr[0][1] == 0)
			{
				cclr[0][2]--;
				cclr[0][0]++;
			}
			tcfix = 0;
		}
	}
}

void menus()
{
	//pause UI
		if (gamestate == 1)
		{
			DrawRect(200,tRGB(196,196,196),40,40,206,40,0);
			DrawRect(200,tRGB(0,0,0),41,41,204,38,0);
			PrintText(200,1,"Game Paused",tRGB(196,196,196),46,47);
			PrintText(200,1,"Press Enter to continue",tRGB(196,196,196),46,57);
			PrintText(200,1,"Press Esc to quit",tRGB(196,196,196),46,67);
		}
		//gameover UI
		if (gamestate == 3)
		{
			DrawRect(200,tRGB(196,196,196),40,40,206,40,0);
			DrawRect(200,tRGB(0,0,0),41,41,204,38,0);
			PrintText(200,1,"Game Over",tRGB(196,196,196),46,47);
			PrintText(200,1,"Press Esc to quit",tRGB(196,196,196),46,67);
		}
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////


void ingameSolo()
{
char strn[64];
unsigned long int i,j,t;

	//logic
	if (gamestate == 0) delay++;

	delayCalc();
	Smoothing(0);
	multCalc(0);

	if (delay >= smoothstuff[0][0])
	{
		delay = 0;
		scores[0][4]++;
		//generation
		generation(0);
	// ultra multiplier
		if (scores[0][4] % 100 == 0 && scores[0][5] == 2)
		{
			anim[0] = 6;
			anim[1] = 20;
			tmult = tmult << 1;

			sprintf(strn,"x%li",tmult);
			buildLayer(505,TextWidth(1,strn),TextHeight(1,strn));
			PrintText(505,1,strn,tRGB(48,48,24),0,0);
		}
		
	//speed modifiers
		speedUpd();

		if (level == 0 && scores[0][1] >1000)
		{
			anim[0] = 2;
			anim[1] = 20;
			scores[0][3]++;
			level++;
		}
		else if (level == 1 && scores[0][1] >2500)
		{
			anim[0] = 2;
			anim[1] = 20;
			scores[0][3]++;
			level++;
		}
		else if (level == 2 && scores[0][1] >5000)
		{
			anim[0] = 2;
			anim[1] = 20;
			scores[0][3]++;
			level++;
		}
		else if (level == 3 && scores[0][1] >10000)
		{
			anim[0] = 2;
			anim[1] = 20;
			scores[0][3]++;
			level++;
		}
		else if (level == 4 && scores[0][1] >20000)
		{
			anim[0] = 2;
			anim[1] = 20;
			scores[0][3]++;
			level++;
		}

	// bonuses updates
		bonusing(0);
	}

	magnetizm(0);
	triggers(0);

	//animations update
	colorStuff();

	//gameover
	if (scores[0][3] <= 0 && gamestate != 2)
	{
		gamestate = 3;
	}

	//tgs
	if (gamestate == 2) 
	{
		gameover();
		return;
	}

	//game frames
	if (gamestate == 0) gframe++;

	if (frame % (frameskip+1)== 0) 
	{
		//rendering
		FDrawRect(200,tRGB(cclr[0][0],cclr[0][1],cclr[0][2]),0,0,xmax,ymax);
		//background
		
		renderBG();

		//hey, do you know that this game has multipliers.
		//oh, fuck, really, we have it!

		if (tmult > 1)
		{
			ZoomRender(505,200,xmax/4,(ymax - getHeight(505)*xmax/(2*getWidth(505)))/2,xmax/2,getHeight(505)*xmax/(2*getWidth(505)),0,0,getWidth(505),getHeight(505),1);
		}

		//ways
		if (quality > 0) 
		{
			reset(509);
			IncludeLayer(511,509,0,0,0);
			DrawRect(509,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),0,0,12,200,2);
			VGradient(509,tRGB(0,0,0),tRGB(128,128,128),0,200-((frame << 2) & 4095),xmax,10,1);
			VGradient(509,tRGB(128,128,128),tRGB(0,0,0),0,210-((frame << 2) & 4095),xmax,10,1);
		}
		for (i=0; i<tways; i++)
		{
			IncludeLayer(509,200,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,0,1);
		}
		for (i=0; i<=tways; i++)
		{
			FDrawVLine(200, tRGB(196,196,196),160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5,0,200);
		}
		//objects
		for (i=0; i<tways; i++)
		{
			for (j=2; j<22; j++)
			{
				if (map[0][j][i] == 1)
				{
					if ((frame >> 2) % 8 < 4) FragmentLayer(18,200,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],(12*((frame >> 2) & 3)),0,12,12,0);
					else TransformLayer(18,200,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],36-(12*((frame >> 2) & 3)),0,12,12,1);
				}
				else if (map[0][j][i] == 2)
				{
					switch ((frame >> 3) % 4)
					{
					case 0: 
						{
							IncludeLayer(19,200,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],0);
							break;
						}
					case 1: 
						{
							TransformLayer(19,200,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],0,0,12,12,6);
							break;
						}
					case 2: 
						{
							TransformLayer(19,200,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],0,0,12,12,3);
							break;
						}
					case 3: 
						{
							TransformLayer(19,200,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],0,0,12,12,5);
							break;
						}
					}
				}
				else if ((map[0][j][i] >= 100) && (map[0][j][i] < 200))
				{
					FragmentLayer(20,200,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],(12*((frame >> 1) % 6)),0,12,12,0);
					if (bonuses[0][16]!=0)
					{
						DrawFrame(200,tRGB(0,196,0),154-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-8+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],14,14);
						FDrawRect(200,tRGB(0,0,0),155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],12,12);
						FragmentLayer(16+6,200,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],(map[0][j][i]-100)*12,0,12,12,0);
					}
				}
			}
		}
		//spaceship
		if (smoothstuff[0][0]<=30)FragmentLayer(506,200,162-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+1,165-smoothstuff[0][4],(12*((frame >> 2) & 1)),(smoothstuff[0][0]-2)/2,12,14-((smoothstuff[0][0]-2)/2),1);
		FragmentLayer(510,200,160-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+1,154-smoothstuff[0][4],(16*((frame >> 2) & 1)),16*shipstyle[0],16,16,0);
		if (bonuses[0][3] != 0 || bonuses[0][6] != 0) 
		{
			IncludeLayer(508,200,159-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+1,153-smoothstuff[0][4],1);
		}

		//fg effects
		
		renderFG();

		//animations time!
		if (anim[1] != 0)
		{
			if (anim[0] == 2) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,0,255,0),0,0,xmax,ymax,1);
			else if (anim[0] == 3) DrawRect(200,ARGB(255*(10-anim[1])/10,255,0,0),0,0,xmax,ymax,1);
			else if (anim[0] == 4) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,196,196,0),0,0,xmax,ymax,1);
			else if (anim[0] == 5) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,0,196,196),0,0,xmax,ymax,1);
			else if (anim[0] == 6) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,255,255,255),0,0,xmax,ymax,1);
		}

		//bonuses UI
		t = 0;
		for (i = 0; i<maxabonuses; i++)
		{
			if (bonuses[0][i] != 0)
			{
				int tt;
				if (bonuses[0][i] > 100) 
				{
					DrawFrame(200,tRGB(0,196,0),50,20+t*20,54,18);
					tt = 100;
				}
				else 
				{
					DrawFrame(200,tRGB(196,196,196),50,20+t*20,54,18);
					tt = bonuses[0][i];
				}
				FDrawRect(200,tRGB(0,0,0),51,21+t*20,52,16);
				FDrawRect(200,tRGB(0,196,0),52,22+t*20,14,14);
				FDrawRect(200,tRGB(0,0,0),53,23+t*20,12,12);
				FragmentLayer(16+6,200,53,23+t*20,i*12,0,12,12,0);
				FragmentLayer(507,200,67,23+t*20,0,0,36*tt/100,12,0);
				t++;
			}
		}

		//UI
		reset(201);
		if (quality == 0) FDrawRect(201,tRGB(0,0,0),0,180,320,20);
		else if (quality == 3) VGradient(201,ARGB(224,0,0,0),ARGB(64,0,0,0),0,180,320,20,0);
		else DrawRect(201,ARGB(127,0,0,0),0,180,320,20);
		IncludeLayer(21,201,57,183,0);
		PrintNum(201,2,smoothstuff[0][1],tRGB(196,196,196),85,191,6);
		if (scores[0][0] == scores[0][1]) DrawRect(201,tRGB(0,196,0),57,191,57,6,2);
		PrintNum(201,2,smoothstuff[0][2],tRGB(196,196,196),166,191,6);
		PrintNum(201,2,smoothstuff[0][3],tRGB(196,196,196),85,183,6);
		PrintNum(201,2,scores[0][3],tRGB(196,196,196),234,183,6);
		PrintNum(201,2,scores[0][4],tRGB(196,196,196),166,183,6);
		PrintNum(201,2,smoothstuff[0][0],tRGB(196,196,196),234,191,6);
		FDrawHLine(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),0,180,320);
		IncludeLayer(201,200,0,0,0);

		if (bonuses[0][7] != 0) DrawRect(200,0,0,0,xmax,ymax,4);
		if (bonuses[0][10] != 0) 
		{
			DoGray(200,200,0,0,0);
		}
		else if (bonuses[0][8] != 0) 
		{
			DoGray(200,200,254,0,0);
			if (quality > 1) DrawRect(200,tRGB(192,255,255),0,0,xmax,ymax,2);
		}

		menus();

		sprintf(strn,"%i | %i",fps,frameskip);
		PrintText(200,1,strn,tRGB(255,255,255),0,0);
		renderer(200);
	}
}

////////////////////////////////////////////////

////////////////////////////////////////////////

////////////////////////////////////////////////

////////////////////////////////////////////////

void ingame2P()
{
char strn[64];
unsigned long int i,j,t;

	//logic
	if (gamestate == 0) delay++;

	delayCalc();
	Smoothing(0);
	Smoothing(1);
	multCalc(0);
	multCalc(1);

	if (delay >= smoothstuff[0][0])
	{
		delay = 0;
		scores[0][4]++;
		scores[1][4]++;
		//generation
		if (scores[0][3] > 0) generation(0);
		if (scores[1][3] > 0) generation(1);
	
	//speed modifiers
		speedUpd();

	// bonuses updates
		if (scores[0][3] > 0) bonusing(0);
		if (scores[1][3] > 0) bonusing(1);
	}

	
	if (scores[0][3] > 0) 
	{
		triggers(0);
		magnetizm(0);
	}
	if (scores[1][3] > 0) 
	{
		triggers(1);
		magnetizm(1);
	}

	//animations update
	colorStuff();

	//gameover
	if (scores[0][3] <= 0 && scores[1][3] <= 0 && gamestate != 2)
	{
		gamestate = 3;
	}

	//tgs
	if (gamestate == 2) 
	{
		gameover();
		return;
	}

	//game frames
	if (gamestate == 0) gframe++;

	if (frame % (frameskip+1)== 0) 
	{
		//rendering
		FDrawRect(200,tRGB(cclr[0][0],cclr[0][1],cclr[0][2]),0,0,xmax,ymax);
		//background
		
		renderBG();

		//ways
		if (quality > 0) 
		{
			reset(509);
			IncludeLayer(511,509,0,0,0);
			DrawRect(509,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),0,0,12,215,2);
			VGradient(509,tRGB(0,0,0),tRGB(128,128,128),0,215-((frame << 2) & 4095),xmax,10,1);
			VGradient(509,tRGB(128,128,128),tRGB(0,0,0),0,225-((frame << 2) & 4095),xmax,10,1);
		}
		for (i=0; i<tways; i++)
		{
			IncludeLayer(509,200,100-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,0,1);
			IncludeLayer(509,200,215-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,0,1);
		}
		for (i=0; i<=tways; i++)
		{
			FDrawVLine(200, tRGB(196,196,196),100-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5,0,200);
			FDrawVLine(200, tRGB(196,196,196),215-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5,0,200);
		}
		//objects
		int ole;
		for (ole=0; ole<pg; ole++)
		{
			if (scores[ole][3] <= 0) continue;
		for (i=0; i<tways; i++)
		{
			for (j=2; j<22; j++)
			{
				if (map[ole][j][i] == 1)
				{
					if ((frame >> 2) % 8 < 4) FragmentLayer(18,200,100+115*ole-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],(12*((frame >> 2) & 3)),0,12,12,0);
					else TransformLayer(18,200,100+115*ole-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],36-(12*((frame >> 2) & 3)),0,12,12,1);
				}
				else if (map[ole][j][i] == 2)
				{
					switch ((frame >> 3) % 4)
					{
					case 0: 
						{
							IncludeLayer(19,200,100+115*ole-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],0);
							break;
						}
					case 1: 
						{
							TransformLayer(19,200,100+115*ole-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],0,0,12,12,6);
							break;
						}
					case 2: 
						{
							TransformLayer(19,200,100+115*ole-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],0,0,12,12,3);
							break;
						}
					case 3: 
						{
							TransformLayer(19,200,100+115*ole-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],0,0,12,12,5);
							break;
						}
					}
				}
				else if ((map[ole][j][i] >= 100) && (map[ole][j][i] < 200))
				{
					FragmentLayer(20,200,100+115*ole-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],(12*((frame >> 1) % 6)),0,12,12,0);
					if (bonuses[ole][16]!=0)
					{
						DrawFrame(200,tRGB(0,196,0),22+186*ole-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-8+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],14,14);
						FDrawRect(200,tRGB(0,0,0),23+186*ole-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],12,12);
						FragmentLayer(16+6,200,23+186*ole-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],(map[ole][j][i]-100)*12,0,12,12,0);
					}
				}
			}
		}
		}
		//spaceship
		for (ole=0; ole<pg; ole++)
		{
		if (smoothstuff[0][0]<=30 && scores[ole][3] > 0) FragmentLayer(506-ole*100,200,102+115*ole-(tways*12+(tways+1)*5)/2+pos[ole]*12+(pos[ole]+1)*5+1,165-smoothstuff[ole][4],(12*((frame >> 2) & 1)),(smoothstuff[0][0]-2)/2,12,14-((smoothstuff[ole][0]-2)/2),1);
		FragmentLayer(510-ole*100,200,100+115*ole-(tways*12+(tways+1)*5)/2+pos[ole]*12+(pos[ole]+1)*5+1,154-smoothstuff[ole][4],(16*((frame >> 2) & 1)),16*shipstyle[ole],16,16,0);
		if ((bonuses[ole][3] != 0 || bonuses[ole][6] != 0) && scores[ole][3] > 0) 
		{
			IncludeLayer(508-ole*100,200,99+115*ole-(tways*12+(tways+1)*5)/2+pos[ole]*12+(pos[ole]+1)*5+1,153-smoothstuff[ole][4],1);
		}
		}

		//fg effects
		
		renderFG();

		//animations time!
		if (anim[1] != 0)
		{
			if (anim[0] == 2) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,0,255,0),0,0,xmax,ymax,1);
			else if (anim[0] == 3) DrawRect(200,ARGB(255*(10-anim[1])/10,255,0,0),0,0,xmax,ymax,1);
			else if (anim[0] == 4) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,196,196,0),0,0,xmax,ymax,1);
			else if (anim[0] == 5) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,0,196,196),0,0,xmax,ymax,1);
			else if (anim[0] == 6) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,255,255,255),0,0,xmax,ymax,1);
		}
		
		FDrawRect(200,tRGB(192,192,192),159,0,2,200);

		//bonuses UI
		for (ole = 0; ole<pg; ole++) 
		{
		if (scores[ole][3] <= 0) continue;
		int ot = 0;
		int et = 0;
		for (i = 0; i<maxabonuses; i++)
		{
			if (bonuses[ole][i] != 0)
			{
				int xa;
				if (i == 4 || i == 5 || i == 7 || i == 8 || i == 10) 
				{
					t = ot;
					ot++;
					xa = 133;
				}
				else 
				{	
					t = et;
					et++;
					xa = 10+236*ole;
				}
				int tt;
				if (bonuses[ole][i] > 100) 
				{
					DrawFrame(200,tRGB(0,196,0),xa,20+t*20,54,18);
					tt = 100;
				}
				else 
				{
					DrawFrame(200,tRGB(196,196,196),xa,20+t*20,54,18);
					tt = bonuses[ole][i];
				}
				FDrawRect(200,tRGB(0,0,0),xa+1,21+t*20,52,16);
				FDrawRect(200,tRGB(0,196,0),xa+2,22+t*20,14,14);
				FDrawRect(200,tRGB(0,0,0),xa+3,23+t*20,12,12);
				FragmentLayer(16+6,200,xa+3,23+t*20,i*12,0,12,12,0);
				FragmentLayer(507,200,xa+17,23+t*20,0,0,36*tt/100,12,0);
			}
		}
		}

		//UI
		reset(201);
		if (quality == 0) 
		{
			FDrawRect(201,tRGB(0,0,0),0,152,72,48);
			FDrawRect(201,tRGB(0,0,0),249,152,72,48);
		}
		else 
		{
			DrawRect(201,ARGB(127,0,0,0),0,152,72,48);
			DrawRect(201,ARGB(127,0,0,0),249,152,72,48);
		}
		IncludeLayer(16+23,201,0,153,0);
		if (scores[0][1]>scores[1][1]) PrintNum(201,2,smoothstuff[0][2],tRGB(128,224,128),42,153,6);
		else PrintNum(201,2,smoothstuff[0][2],tRGB(224,224,192),42,153,6);
		if (scores[1][1]>scores[0][1]) PrintNum(201,2,smoothstuff[1][2],tRGB(128,224,128),250,153,6);
		else PrintNum(201,2,smoothstuff[1][2],tRGB(224,224,192),250,153,6);
		if (scores[0][3] > 0) 
		{
			PrintNum(201,2,smoothstuff[0][3],tRGB(224,224,192),42,163,6);
			PrintNum(201,2,scores[0][3],tRGB(224,224,192),42,183,6);
			PrintNum(201,2,scores[0][4],tRGB(224,224,192),42,173,6);
			PrintNum(201,2,smoothstuff[0][0],tRGB(224,224,192),42,193,6);
		}
		if (scores[1][3] > 0) 
		{
			PrintNum(201,2,smoothstuff[1][3],tRGB(224,224,192),250,163,6);
			PrintNum(201,2,scores[1][3],tRGB(224,224,192),250,183,6);
			PrintNum(201,2,scores[1][4],tRGB(224,224,192),250,173,6);
			PrintNum(201,2,smoothstuff[1][0],tRGB(224,224,192),250,193,6);
		}
		DrawFrame(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),-1,151,74,50);
		DrawFrame(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),248,151,74,50);
		IncludeLayer(201,200,0,0,0);

		if (bonuses[0][7] != 0) DrawRect(200,0,0,0,xmax,ymax,4);
		if (bonuses[0][10] != 0) 
		{
			DoGray(200,200,0,0,0);
		}
		else if (bonuses[0][8] != 0) 
		{
			DoGray(200,200,254,0,0);
			if (quality > 1) DrawRect(200,tRGB(192,255,255),0,0,xmax,ymax,2);
		}

		menus();

		sprintf(strn,"%i | %i",fps,frameskip);
		PrintText(200,1,strn,tRGB(255,255,255),0,0);
		renderer(200);
	}
}


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void Demo1()
{	
char strn[64];
	if ((f & 2) == 0) r=(rand() & 3)+4;
		
		//reset(cnv[0],xmax,ymax);
		DrawRect(128,tRGB(0,0,0),0,0,xmax,ymax,0);
		for (unsigned int i=0; i<sz; i++)
		{
			nclr[0]=f & 127;
			nclr[1]=(f / 2) & 127;
			nclr[2]=(f / 3) & 127;
			
			for (int j=-r; j<=r * 1; j++)
			{
				for (int k=-r; k<=r * 1; k++)
				{
					//if (sqrt(abs(j)*abs(j)+abs(k)*abs(k))<=r)
					if ((abs(j)+abs(k))<=r)
					{
						clr=255*(r+1-(abs(j)+abs(k)))/r;
						//clr=255*(r+1-sqrt(abs(j)*abs(j)+abs(k)*abs(k)))/r;
						if (mas[1][i]+c+k<ymax)
						{
							plot(128,mas[0][i]+j,mas[1][i]+k+c,clr, nclr[0],nclr[1],nclr[2]);
						}
						else 
						{
							plot(128,mas[0][i]+j,mas[1][i]+c-ymax+k,clr, nclr[0],nclr[1],nclr[2]);
						}
					}
				}
			}
	
			if (mas[1][i]+c<ymax)
			{
				plot(128, mas[0][i],mas[1][i]+c, nclr[0]+127,nclr[1]+127,nclr[2]+127);
			}
			else
			{
				plot(128, mas[0][i],mas[1][i]+c-ymax, nclr[0]+127,nclr[1]+127,nclr[2]+127);
			}
		}
		c++;
		f++;
	
		DrawRect(129,ARGB(4,0,0,0),0,0,xmax,ymax,2);
		IncludeLayer(128, 129, 0, 0, 1);
	
		DrawRect(129,tRGB(0,0,0),0,0,80,14,2);
		sprintf(strn,"%i | %i | %i",fps,frameskip, dticks);
		PrintText(129,1,strn,tRGB(255,255,255),0,0);
		//PrintText(129,1,"Demo1",tRGB(255,255,255),0,0);
		if (frame % (frameskip+1)== 0) renderer(129);
	
		if (c>ymax) c-=ymax;
}


void Demo2()
{
char strn[64], str2[32];
	if (((f >> 4) & 1) == 0) r=(rand() & 3)+4;
		
		reset(128);
		DrawRect(129, tRGB(0,0,0),0,0,xmax,ymax, 0);
		for (unsigned int i=0; i<sz; i++)
		{
			nclr[0]=127;
			nclr[1]=127;
			nclr[2]=127;
			
			for (int j=-r; j<=r; j++)
			{
				for (int k=-r; k<=r; k++)
				{
					if ((abs(j)+abs(k))<=r)
					{
						clr=255*(r+1-(abs(j)+abs(k)))/r;
						add(129,mas[0][i]+j,mas[1][i]+k,clr, nclr[0],nclr[1],nclr[2]);
						
					}
				}	
	
				plot(129, mas[0][i],mas[1][i], 255,255,255);
			}
		}
		
		printf("Demo2: mas[0][0]=%i mas[1][0]=%i\n", mas[0][0], mas[1][0]);

		f++;
		VGradient(129, tRGB(255,255,255),tRGB(0,0,0), 0,0,xmax,ymax*3/4, 2);	//Stars
		DrawRect(129, tRGB(0,0,0),0,ymax*3/4,xmax,ymax/4, 2);					//Invisible at Bottom
		VGradient(129, tRGB(0,0,32), tRGB(32,0,8),0,0,xmax,ymax, 1);			//BG Gradient
		 
		if (frame < 90*5)
		{ 
			IncludeLayer(16, 128, 0, ymax-150, 0);
		}
		if (frame < 90)
		{ 
			VGradient(128,tRGB(127,127,127), tRGB(32,32,32), 0,0,xmax,ymax, 2);
			sprintf(str2,"%s","Default");
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*2)
		{ 
			sprintf(str2,"%s","Without Darkness");
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*3)
		{ 
			sprintf(str2,"%s","Inverted");
			DrawRect(128, tRGB(0,0,0),0,0,xmax/2,ymax, 4);
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*4)
		{ 
			sprintf(str2,"%s","Addition");
			IncludeLayer(128, 129, 0, 0, 1);
		}
		else if (frame < 90*5)
		{ 
			sprintf(str2,"%s","Multipicy");
			IncludeLayer(128, 129, 0, 0, 2);
		}
		else if (frame < 90*6)
		{ 
			sprintf(str2,"%s","Fragment");
			FragmentLayer(16, 128, 0, ymax-150,0,0,xmax/2,75,0);
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*7)
		{ 
			sprintf(str2,"%s","Zoomed");
			ZoomRender(16, 128, 0, ymax-150,xmax,150,0,0,xmax/2,75,0);
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*8)
		{ 
			sprintf(str2,"%s","Grayscale(254)");
			DoGray(16,128,254,0,ymax-150);
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*9)
		{ 
			sprintf(str2,"%s","Grayscale(0)");
			DoGray(16,128,0,0,ymax-150);
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*10)
		{ 
			sprintf(str2,"%s","Custom Channels 1");
			MascedLayer(16,128,0,50,tRGB(128,0,0),tRGB(0,128,0),tRGB(0,128,128));
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*11)
		{ 
			sprintf(str2,"%s","Custom Channels 2");
			MascedLayer(16,128,0,50,tRGB(0,255,0),tRGB(0,0,255),tRGB(255,0,0));
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*12)
		{ 
			sprintf(str2,"%s","Transformation");
			TransformLayer(16, 128, 0, ymax-150,0,0,320,150, 3);
			IncludeLayer(128, 129, 0, 0, 0);
		}
		if (frame == 90*12-1) frame = 0;

		sprintf(strn,"%i | %i | %i (%s)",fps,frameskip, dticks,str2);
		PrintText(129,1,strn,tRGB(255,255,255),0,0);
		//PrintText(129,1,"Demo2",tRGB(255,255,255),0,0);

		if (frame % (frameskip+1)== 0) renderer(129);
}


void Demo3()
{
	reset(128);
	PrintText(128,0,"Test",tRGB(255,255,0),0,0);
	renderer(128);
}

void OP()
{
	if (dbg)
	{
		Debug();
		return;
	}
	if (modif == 0)
	{
		if (scrn == 0)
		{
			MM();
		}
		else if (scrn == 1)
		{
			if (gs == 0 && pg == 1) ingameSolo();
			else if (gs == 0 && pg == 2) ingame2P();
		}
	}
	else if (modif == 1) 
	{
		Demo1();
	}
	else if (modif == 2) 
	{
		Demo2();
	}
	else if (modif == 3) 
	{
		Demo3();
	}
}

void KOP()
{
	if (get_key(KEY_F3) && (get_keypressed(KEY_F3) == 0))
	{
		dbg=!dbg;
		return;
	}
	if (dbg)
	{
		if (get_key(KEY_UP) && (get_keypressed(KEY_UP) % 20 == 0 || (get_keypressed(KEY_UP) > 60 && get_keypressed(KEY_UP) % 2 == 0)))
		{
			if (dmode==-1)
			{
				dsel--;
				if (dsel < 0) dsel=tmax-1;
			}
		}
		if (get_key(KEY_DOWN) && (get_keypressed(KEY_DOWN) % 20 == 0 || (get_keypressed(KEY_DOWN) > 60 && get_keypressed(KEY_DOWN) % 2 == 0)))
		{
			if (dmode==-1)
			{
				dsel++;
				if (dsel+1 > tmax) dsel=0;
			}
		}
		if (get_key(KEY_RETURN) && get_keypressed(KEY_RETURN) == 0)
		{
			if (dmode==-1)
			{
				int tc = 0;
				for (int i=0; i<dsel; i++)
				{
					for (; tc<=512; tc++) 
					{
						if (getWidth(tc) !=0) break;
					}
					tc++;
				}
				dmode = tc;
			}
			else dmode = -1;
		}
		return;
	}
	
	if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0 && inmenu==1)
	{
		Quit(0);
	}
	
	if (modif == 0 && scrn == 0)
	{
		if (inmenu>=121 && inmenu<=123) //colors
		{	
			int d[3];
			RGBt(tcss,&d[0],&d[1],&d[2]);
			if (get_key(KEY_LEFT)) 
			{
				if (d[sel-1] > 0) d[sel-1]--;
			}
			
			if (get_key(KEY_RIGHT)) 
			{
				if (d[sel-1] < 255) d[sel-1]++;
			}
			tcss=tRGB(d[0],d[1],d[2]);
		}
		
		if (inmenu == 1 && sel == 2) //customize select
		{
			if (get_key(KEY_LEFT)) 
			{
				ccss=0;
			}
			if (get_key(KEY_RIGHT)) 
			{
				ccss=1;
			}
		}
		if (get_key(KEY_UP) && (get_keypressed(KEY_UP) % 20 == 0 || (get_keypressed(KEY_UP) > 60 && get_keypressed(KEY_UP) % 2 == 0)))
		{
			sel--;
			if (sel < 1) sel=selmax;
		}
		if (get_key(KEY_DOWN) && (get_keypressed(KEY_DOWN) % 20 == 0 || (get_keypressed(KEY_DOWN) > 60 && get_keypressed(KEY_DOWN) % 2 == 0)))
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
			printf("KOP: inmenu=%i\n", inmenu);
		}
	}
	else if (modif == 0 && scrn == 1)
	{
		if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0)
		{
			if (gamestate == 0) gamestate = 1;
			else gamestate = 2;
			printf("KOP: gamestate=%i\n", gamestate);
		}
		if (get_key(KEY_RETURN) && get_keypressed(KEY_RETURN) == 0)
		{
			if (gamestate == 1) gamestate = 0;
		}
		if ((get_key(KEY_LEFT) && get_keypressed(KEY_LEFT) == 0) || (get_key(KEY_A) && get_keypressed(KEY_A) == 0) && gamestate == 0)
		{
			if (pos[0] !=0) pos[0]--;
		}
		if ((get_key(KEY_RIGHT) && get_keypressed(KEY_RIGHT) == 0) || (get_key(KEY_D) && get_keypressed(KEY_D) == 0) && gamestate == 0)
		{
			if (pos[0] !=tways-1) pos[0]++;
		}
		if (get_key(KEY_NUMPAD4) && get_keypressed(KEY_NUMPAD4) == 0 && gamestate == 0)
		{
			if (pos[1] !=0) pos[1]--;
		}
		if (get_key(KEY_NUMPAD6) && get_keypressed(KEY_NUMPAD6) == 0 && gamestate == 0)
		{
			if (pos[1] !=tways-1) pos[1]++;
		}
	}
	else
	{
		if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0)
		{
			printf("KOP: modif=0\n");
			modif=0;
		}
	}
}
