#include "Lib.h"

class LPController
{
private:
	int maxcount;

public: 
	int defcolor, defincolor, deflifetime,defsize,defbrightness,spawnx,spawny,defspeed,defangle;
	int randsize,randspawnx,randspawny,randangle,randspeed,randlife,randbright,camerax,cameray;
	bool defrecreate,rendercore;
	bool isalive[256];
	LPObject P[256];
	
	LPController(int max, int defecolor, int defeincolor, int defelifetime,int defesize, bool deferecreate, int spawnx_, int spawny_)
	{
		maxcount = max;
		defcolor = defecolor;
		defincolor = defeincolor;
		deflifetime = defelifetime;
		defrecreate = deferecreate;
		spawnx = spawnx_;
		spawny = spawny_;
		defsize = defesize;
		randsize = 0;
		randspawnx = 0;
		randspawny = 0;
		randangle = 0;
		randspeed = 0;
		randlife = 0;
		randbright = 0;
		rendercore = true;
		camerax = 0;
		cameray = 0;
	}

	//sys

	//operate

	void Spawn(int sp, int ag)
	{
		for (int i=0; i<maxcount; i++)
			if (!isalive[i]) 
			{
				P[i].Create(spawnx+gener(randspawnx)-randspawnx/2,spawny+gener(randspawny)-randspawny/2,defcolor,defsize+gener(randsize)-randsize/2,deflifetime+gener(randlife)-randlife/2,defrecreate);
				P[i].ChangeColors(defincolor,defcolor,defbrightness+gener(randbright)-randbright/2);
				P[i].EChangeSpeed(sp+gener(randspeed)-randspeed/2);
				P[i].ERotate(ag+gener(randangle)-randangle/2);	
				P[i].drawcore = rendercore;
				isalive[i] = true;
				return;
			}
	}

	
	void Spawn()
	{
		Spawn(defspeed,defangle);
	}

	void Draw(int target)
	{
		for (int i=0; i<maxcount; i++)
			if (isalive[i]) 
				P[i].Draw(target,0,0,camerax,cameray);
	}

	void Draw(int target, int sizeup, int brightness)
	{
		for (int i=0; i<maxcount; i++)
			if (isalive[i]) 
				P[i].Draw(target,sizeup,brightness,camerax,cameray);
	}

	void Destroy()
	{
		for (int i=0; i<maxcount; i++) isalive[i] = false;
	}


	void Update()
	{
		for (int i=0; i<maxcount; i++)
			if (isalive[i]) 
				{
					P[i].Update();
					if (!P[i].alive)
						isalive[i] = false;
				}
	}

	
	void RandomizeUpd(int val)
	{
		for (int i=0; i<maxcount; i++)
			if (isalive[i]) 
				P[i].SRotate(gener(val)-val/2);
		Update();
	}
};
