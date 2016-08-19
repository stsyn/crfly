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
#include "LParticles.h"
#include "LPObjects.h"
#include "LPController.h"
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

int clr, nclr[3], quality=0;
const int strmax = 46, scrmax = 38, maxparticles=360;
unsigned int sz=100;

char str[strmax+1][64];
int scrs[scrmax][2];

int sel=1, selmax=4, dmode=-1,dsel=0,tmax=0;
int lc = 0;
int f=0;
int r=5;
int *mas[2];
int o;

int modif=0, scrn=0, c=0, gs, pg;
bool dbg=false,d3enabled=false,ac5=false,dc5=false,nobonuses=false;
int inmenu=1;

unsigned long int gframe=0;

//LOGIC
const int mplayers = 2, maxbg=2, maxfg=2, maxabonuses=17, maxbbonuses=6;
int short unsigned tways, pos[mplayers], map[mplayers][22][6], delay, gamestate, anim[mplayers], doloykamni[mplayers],tbg,tfg,cbg,cfg,tcfix;
long unsigned int  tmult=1,xmult[mplayers],tdelay,shipmove[mplayers],smoothstuff[mplayers][5];
long int scores[mplayers][6], bonuses[mplayers][maxabonuses];
int bg[maxbg][3], fg[maxfg][3], level,act[2], shipsmooth[2];
unsigned int css[mplayers][3], tcss, ttcss[3], cclr[3][3], shipstyle[2],tshipstyle, maxships, ccss=0;
void LS();
void ingame();

unsigned short int evecounts[5];
int hold,metamap[22][6], xp,wtype,fsthold,bullets,hpmax,evelasts,evecounter;
bool wavail[5], eveflag;
const int lvlmax = 20;

const int levels[lvlmax+1] = {0,   30,   80,  150,  240,  350,  480,  630,  800,  990, 1200
						       , 1430, 1680, 1950, 2240, 2550, 2880, 3230, 3600, 3990, 4400};

struct weapon
{                  
    int power;
	int speed;
	int capacity;
	int color;
	int delay;
	int reload;
	int lifetime;
	int minlevel;
};

weapon W[5], CW;

FILE *event_file;

LPController LP1(256,0,0,90,7,false,160,100);
LPController LP(256,0,0,90,7,false,160,100);
LPController BUL(256,0,0,300,3,false,160,100);
LPController EP(64,0,0,10,4,false,0,0);
LPController AP(256,0,0,10,6,false,0,0);
LSprite testing;

int generateMeteor()
{
	int t2 = gener(20);
	return t2<3?201+t2:200;
}

#include "G_RPG.H"
#include "G_System.h"
/*

  Hey there, you need to know what scores[x] is mean!
  0 - HiScore
  1 - Total score
  2 - Only score
  3 - Lives counter
  4 - Distance
  5 - Delay

*/

//////////////////////////////////////////////////////////

void renderBG(int phs, int spd, int quality)
{
	int i, j;
	if (quality>0) //stars
		{
			c = (phs*2 +(delay*2/spd))% ymax;
			for (i = 0; i<sz; i++)
			{
				if (mas[1][i]+c<ymax) plot(200, mas[0][i],mas[1][i]+c, 4282400832);
				else plot(200, mas[0][i],mas[1][i]+c-ymax, 4282400832);
			}
			c = (phs*3 +(delay*3/spd))% ymax;
			for (i = 0; i<sz; i++)
			{
				if (mas[1][i]+c<ymax) plot(200, xmax-mas[0][i],mas[1][i]+c, 4286611584);
				else plot(200, xmax-mas[0][i],mas[1][i]+c-ymax, 4286611584);
			}
			c = (phs*6 +(delay*6/spd))% ymax;
			for (i = 0; i<sz; i++)
			{
				if (-mas[1][i]+c<0) plot(200, mas[0][i],ymax-mas[1][i]+c, 4291085508);
				else plot(200, mas[0][i],-mas[1][i]+c, 4291085508);
			}
		}

		if (quality>1) //bg objects
		{
			int atbg = -1;
			c = (phs*8 +(delay*8/spd));
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
				tt = gener(6*(spd/4+ymax-tt));
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

void renderFG(int phs, int spd)
{
	int i, j;
	if (quality>1) //objects
		{
			int atfg = -1;
			c = (phs*36 +(delay*36/spd));
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
				tt = gener(4*(spd/5+ymax-tt));
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
	if (ac5) tdelay = tdelay >> 1;
	if (dc5) tdelay = tdelay << 1;

	if (tdelay>40) tdelay = 40;
	if (tdelay==0) tdelay = 1;
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

	shipsmooth[ole] ? shipsmooth[ole]<0 ? shipsmooth[ole]+=2 : shipsmooth[ole]-=2 : shipsmooth[ole];
}
}

void multCalc(int ole)
{
int	mult=8;
	if (bonuses[0][7] == 0)
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

void mve(int ole)
{
		for (int k=20; k>=0; k--)
		{
			for (int j=0; j<6; j++) 
			{
				map[ole][k+1][j] = map[ole][k][j];
				metamap[k+1][j] = metamap[k][j];
			}
		}
		for (int j=0; j<6; j++) 
		{
			map[ole][0][j] = 0;
			metamap[0][j] = 0;
		}
}

void generation(int ole)
{
/*

How T

   00 01 02 03 04 05 06 07 08 09
00 cn mt bn BN cn mt BN BN cn mt
10 PW RN cn mt       cn mt      
20 cn mt       cn mt       cn mt
30       cn mt       cn mt      
40 cn mt       cn mt       cn mt

*/
if (gs == 2) doloykamni[ole] = 0;
unsigned long int t;
		t = gener(50);
		if (t % 4 == 0) 
		{
			t = gener(tways);
			map[ole][0][t] = 1; //coin
			metamap[0][t] = 1; 
			doloykamni[ole] = 0;
		}
		else if (t % 4 == 1) 
		{
			if ((doloykamni[ole] < tways-1) || (tmult > 7)) 
			{
				t = gener(tways);
				map[ole][0][t] = generateMeteor(); //meteor
				metamap[0][t] = 10;
				doloykamni[ole]++;

			}
			else
			{
				doloykamni[ole] = 0;
			}

		}
		else if ((t == 2 || (bonuses[ole][11] !=0 && (t == 3 || t == 6 || t == 7))) && !nobonuses) 
		{
			t = gener(tways);
			int t2 = gener(maxabonuses);
			map[ole][0][t] = 100+t2; //bonus
			metamap[0][t] = 1;
			doloykamni[ole] = 0;
		}
		else if (gs== 2)
		{
			if (t == 10)
			{
				t = gener(tways);
				int t2 = gener(20);
				if (t2<maxbbonuses)
				{
					if ((t2 == 0) && (tways == 6)) return;
					if ((t2 == 1) && (tways == 2)) return;
					if (t2 == 2) return;
					if ((t2 == 3) && (wavail[1])) return;
					if ((t2 == 4) && (wavail[2])) return;
					if ((t2 == 5) && (wavail[3])) return;
					if ((t2 == 6) && (wavail[4]) && !((wavail[1]) && (wavail[2]) && (wavail[3]))) return;
					map[ole][0][t] = 300+t2+maxabonuses; //powerbonus
					metamap[0][t] = 1; 
				}
			}
			else if (t == 11)
			{
				t = gener(tways);
				int t2 = gener(50);
				if (t2<7)
				{
					map[ole][0][t] = 400+t2; //rune
					metamap[0][t] = 0; 
				}
			}
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

void speedUpdRLD()
{
if (scores[0][5] < 2) scores[0][5] = 2;

		if (scores[0][4] <= 18 && scores[0][4] >= 10) scores[0][5]+=3;

		if (scores[0][4] % 25 == 0)
		{
			if (scores[0][5] > 20 || scores[0][4] % 50 == 0)
			{
				if (scores[0][5] > 15 || scores[0][4] % 100 == 0)
				{
					if (scores[0][5] >= 24) scores[0][5]-=2;
					else if (scores[0][5] >= 11) scores[0][5]-=1;
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
	for (i=0; i<maxabonuses; i++) if (bonuses[ole][i] < 0) bonuses[ole][i] = 0;
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

int retX(LPObject P)
{
	return (P.posx/100-168+(tways*12+(tways+1)*5)/2)/17;
}

int retY(LPObject P)
{
	return (P.posy/100+55-(delay*12/smoothstuff[0][0])+smoothstuff[0][4])/12;
}

int setX(int x)
{
	return 160-(tways*12+(tways+1)*5)/2+x*12+(x+1)*5+3;
}

int setY(int y)
{
	return 12*(y-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4];
}

void aParticles(int t, int x, int y)
{
	if (t == 0) return;
	int tt = 5*(quality==3?2:1);
	AP.defcolor = tRGB(t!=3?192:0,t!=2?192:0,t==3?192:0);
	AP.defincolor = tRGB(t!=3?255:0,t!=2?255:0,t==3?255:0);
	if (t == 4)
	{
		AP.defcolor = tRGB(192,192,192);
		AP.defincolor = tRGB(255,255,255);
	}
	AP.spawnx = x;
	AP.spawny = y;
	for (int i = 0; i<tt; i++) AP.Spawn();
}


int triggers(int ole, int ox, int oy, bool self)
{
int oep;
if (ox<0 || oy<0) return 0;
bool stand = false;
if (map[ole][oy][ox] != 0)
	{
		if (map[ole][oy][ox] == 1) 
		{
			if (self) scores[ole][2]+=3*xmult[ole];
			oep = 1;
		}
		else if ((map[ole][oy][ox] >= 200) && (map[ole][oy][ox] < 300)) 
		{
			oep = 2;
			if ((gs == 2) && (bonuses[ole][3] !=0) && self) 
			{
				bonuses[ole][3]-=10;
				if (bonuses[ole][3]<0)
				{
					anim[0] = 3;
					anim[1] = 10;
					scores[ole][3] += bonuses[ole][3];
					bonuses[ole][3] = 0;
				}
			}
			else if (bonuses[ole][3] == 0 && bonuses[ole][6] == 0 && self)
			{
				scores[ole][3]--;
				if (gs == 2) scores[ole][3]-=9;
				anim[0] = 3;
				anim[1] = 10;
				bonuses[ole][3]+=7;
				if (gs == 2) bonuses[ole][3]+=8;
			}
			if (!self) 
			{
				scores[ole][2]+=5;
				if (level < lvlmax) xp+=5;
			}
			if (map[ole][oy][ox] != 200)
			{
				stand = true;
				switch (map[ole][oy][ox] - 200)
				{
				case 1: 
					{
						map[ole][oy][ox] = 1;
						metamap[oy][ox] = 1;
						break;
					}
				case 2: 
					{
						int t2 = gener(20);
						map[ole][oy][ox] = t2<3?201+t2:200;
						metamap[oy][ox] = 10;
						break;
					}
				case 3:
					{
						int t2 = gener(maxabonuses);
						map[ole][oy][ox] = 100+t2;
						metamap[oy][ox] = 1;
						break;
					}
				}
			}
		}
		else if ((map[ole][oy][ox] >= 100) && (map[ole][oy][ox] < 200)) 
		{
			oep = 3;
			//ooohhhh yeeeeaaaahh bonuses
			int t = map[ole][oy][ox] - 100;
			if (!self)
			{
				map[0][oy][ox] = 0;
				aParticles(triggers(0,ox,oy-1,false),setX(ox),setY(oy-1));
				aParticles(triggers(0,ox,oy-2,false),setX(ox),setY(oy-2));
				aParticles(triggers(0,ox,oy+1,false),setX(ox),setY(oy+1));
				aParticles(triggers(0,ox,oy+2,false),setX(ox),setY(oy+2));
				aParticles(triggers(0,ox-1,oy-1,false),setX(ox-1),setY(oy-1));
				aParticles(triggers(0,ox-1,oy,false),setX(ox-1),setY(oy));
				aParticles(triggers(0,ox-1,oy+1,false),setX(ox-1),setY(oy+1));
				aParticles(triggers(0,ox+1,oy-1,false),setX(ox+1),setY(oy-1));
				aParticles(triggers(0,ox+1,oy,false),setX(ox+1),setY(oy));
				aParticles(triggers(0,ox+1,oy+1,false),setX(ox+1),setY(oy+1));
				aParticles(triggers(0,ox-2,oy,false),setX(ox-2),setY(oy));
				aParticles(triggers(0,ox+2,oy,false),setX(ox+2),setY(oy));
			}
			else if (t == 0) //x2 coins
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
				if (pg == 1 && gs == 0) bonuses[ole][6]+=125;
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
				if (bonuses[0][7] == 0) scores[ole][3]++;
				else scores[0][3]--;
			}
			else if (t == 13) //life down +
			{
				anim[0] = 3;
				anim[1] = 10;
				if (bonuses[0][7] == 0 && bonuses[0][6] == 0 && bonuses[ole][3] ==0) scores[ole][3]--;
				else scores[ole][3]++;
				bonuses[ole][3]+=10;
			}
			else if (t == 14) //jackpot +
			{
				anim[0] = 4;
				anim[1] = 20;
				if (bonuses[0][7] == 0) scores[ole][2]+=500;
				else scores[ole][2]-=500;
			}
			else if (t == 15) //add slow +
			{
				anim[0] = 5;
				anim[1] = 20;
				if (bonuses[0][7] == 0) scores[ole][5]+=2;
				else scores[ole][5]-=2;
			}
			else if (t == 16) //viewer
			{
				bonuses[ole][16]+=100;
			}
		}
		else if ((map[ole][oy][ox] >= 300) && (map[ole][oy][ox] < 400) && (self)) 
		{
			oep = 3;
			//ooohhhh yeeeeaaaahh bonuses 2
			int t = map[ole][oy][ox] - 300;
			if (t == 17) //wider
			{
				tways++;
				for (int i=0; i<6;i++)
					map[0][16][i] = 406;

			}
			else if (t == 18) //narrow
			{
				tways--;
				if (pos[0]>=tways) pos[0] = tways-1;
				for (int i=0; i<6;i++)
					map[0][16][i] = 406;
			}
			else if (t == 20) //unlock
			{
				wavail[1] = true;
			}
			else if (t == 21) //unlock
			{
				wavail[2] = true;
			}
			else if (t == 22) //unlock
			{
				wavail[3] = true;
			}
			else if (t == 23) //unlock
			{
				wavail[4] = true;
			}
		}
		else if (map[ole][oy][ox] == 400 && self) // rune coin collect
		{
			for (int i=0; i<6; i++) 
			{
				for (int j=0; j<20; j++) 
				{
					if (map[0][j][i]==1) triggers(0,i,j,true);
				}
			}
			oep = 3;
		}
		else if (map[ole][oy][ox] == 401 && self) // rune event
		{
			for (int i=0; i<6; i++) 
			{
				for (int j=0; j<8; j++) 
				{
					map[0][j][i]=0;
					metamap[j][i]=0;
				}
			}
			anim[0] = 6;
			anim[1] = 20;
			oep = 3;
			e_start();
		}
		else if (map[ole][oy][ox] == 402 && self) // rune coin <> meteor
		{
			for (int i=0; i<6; i++) 
			{
				for (int j=0; j<20; j++) 
				{
					if (map[0][j][i]==1) 
					{
						int t2 = gener(20);
						map[0][j][i] = t2<3?201+t2:200; //meteor
						metamap[j][i] = 10;
					}
					else if (map[0][j][i]>=200 && map[0][j][i]<300)
					{
						metamap[j][i] = 1;
						map[0][j][i] = 1;
					}
				}
			}
			oep = 3;
		}
		else if (map[ole][oy][ox] == 403 && self) // rune destroy meteors
		{
			for (int i=0; i<6; i++) 
			{
				for (int j=0; j<20; j++) 
				{
					if (map[0][j][i]>=200 && map[0][j][i]<300) triggers(0,i,j,false);
				}
			}
			oep = 3;

		}
		else if (map[ole][oy][ox] == 404 && self) // rune extend bonuses
		{
			for (int i=0; i<maxabonuses; i++) 
			{
				if (bonuses[0][i]!=0)  bonuses[0][i]+=20;
			}
			oep = 3;
		}
		else if (map[ole][oy][ox] == 405 && self) // rune recovery
		{
			anim[0] = 2;
			anim[1] = 20;
			scores[0][3] = hpmax;
		}
		else if (map[ole][oy][ox] == 406 && self) // rune reset
		{
			for (int i=0; i<6; i++) 
			{
				for (int j=0; j<20; j++) 
				{
					map[0][j][i]=0;
					metamap[j][i]=0;
				}
			}
			anim[0] = 6;
			anim[1] = 20;
			oep = 3;
		}
		if (!stand) map[ole][oy][ox] = 0;
	}
else
{
	oep = 0;
}

	if (scores[ole][3]>0) scores[ole][1] = scores[ole][2] + scores[ole][4];
	if (scores[ole][0]<scores[ole][1]) scores[ole][0] = scores[ole][1];
	if (self) act[ole] = oep;
	return oep;
}

void colorStuff(int frm, int spd)
{
	if (anim[1] != 0) anim[1]--;
	else anim[0] = 0;

	//smooth colors
	if (frm % spd == 0 && quality>0)
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

void renderCoin(int f, int x, int y)
{
	if ((f >> 2) % 8 < 4) FragmentLayer(18,200,x,y,(12*((frame >> 2) & 3)),0,12,12,0);
	else TransformLayer(18,200,x,y,36-(12*((frame >> 2) & 3)),0,12,12,1);
}

void renderPW(int f, int x, int y)
{
	IncludeLayer(40,200,x,y,0);
}

void renderRune(int f, int x, int y)
{
	FragmentLayer(41,200,x,y,f*12,0,12,12,0);
}

void renderMeteor(int f, int x, int y)
{
	switch ((f >> 3) % 4)
	{
		case 0: 
		{
			FragmentLayer(19,200,x,y,((f >> 1) % 4)*12,0,12,12,0);
			break;
		}
		case 1: 
		{
			TransformLayer(19,200,x,y,((f >> 1) % 4)*12,0,12,12,6);
			break;
		}
		case 2: 
		{
			TransformLayer(19,200,x,y,((f >> 1) % 4)*12,0,12,12,3);
			break;
		}
		case 3: 
		{
			TransformLayer(19,200,x,y,((f >> 1) % 4)*12,0,12,12,5);
			break;
		}
	}
}

void renderBonus(int f, int x, int y)
{
	FragmentLayer(20,200,x,y,(12*((f >> 1) % 6)),0,12,12,0);
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

#include "G_Core.h"
#include "G_Menu.h"
#include "G_OpUnit.h"
