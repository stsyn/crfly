#include "Lib.h"
#include "Math.h"

class LPObject: public LParticle
{
private:
	int st_lifetime, st_posx, st_posy;
	bool renewable;

	void LReset()
	{
		posx = st_posx;
		posy = st_posy;
		lifetime = st_lifetime;
	}

protected:
	int lifetime,speed,angle;

public:
	int posx, posy;
	bool alive;
	void Create(int Sposx, int Sposy, int Scolor, int Ssize, int Slife, bool SR)
	{
		posx = Sposx*100;
		posy = Sposy*100;
		st_posx = Sposx*100;
		st_posy = Sposy*100;
		color = Scolor;
		glowcolor = Scolor;
		size = Ssize;
		speed = 200;
		angle = 90;
		st_lifetime = Slife;
		lifetime = Slife;
		renewable = SR;
		alive = true;
	}

	void EChangeSpeed(int s)
	{
		speed = s;
	}

	void ERotate(int a)
	{
		angle = a;
	}

	void ChangeSpeed(int s)
	{
		speed = (speed+s)/2;
	}

	void Rotate(int a)
	{
		angle = (angle+a)/2;
	}

	void SRotate(int a) //MOAR
	{
		angle = angle+a;
	}

	void Update()
	{
		posx = posx+cos(3.14*angle/180)*speed;
		posy = posy-sin(3.14*angle/180)*speed;
		if (lifetime>0)
		{
			lifetime--;
			brightness = sbrightness*lifetime/st_lifetime;
		}
		else if (lifetime == 0 && renewable)
		{
			LReset();
		}
		if (lifetime == 0) alive = false;
	}

	void Draw(int id)
	{
		if (lifetime != 0)
		{
			LDraw(id, posx/100, posy/100);
		}
	}

	void Draw(int id, int sizeup, int brightnessup, int camerax,int cameray)
	{
		if (lifetime != 0)
		{
			LTDraw(id, posx/100+camerax, posy/100+cameray, sizeup, brightnessup);
		}
	}
};