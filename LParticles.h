#include "Lib.h"

class LParticle
{
	protected:
		int color, glowcolor;
		int size, brightness, sbrightness;
	public:
		bool drawcore;
		LParticle()
		{
			color = tRGB(255,255,255);
			glowcolor = tRGB(255,255,255);
			size = 8;
			brightness = 255;
			sbrightness = 255;
		}

		void LDraw(int id, int x, int y, int size, int brightness)
		{
			brightness = brightness < 256 ? brightness : 255;
			brightness = brightness > -1 ? brightness : 0;
			int r,g,b;
			RGBt(glowcolor,&r,&g,&b);
			for (int j=-size; j<=size; j++)
			{
				for (int k=-size; k<=size; k++)
				{
					if ((abs(j)+abs(k))<=size)
					{
						int aclr=brightness*(size+1-(abs(j)+abs(k)))/size;
						add(id,x+j,y+k,aclr, r,g,b);
					}
				}
			}
			RGBt(color,&r,&g,&b);
			if (drawcore) add(id,x,y,r,g,b);
		}

		void LDraw(int id, int x, int y)
		{
			LDraw(id,x,y,size,brightness);
		}

		void LTDraw(int id, int x, int y, int sizeup, int brightnessup)
		{
			LDraw(id,x,y,size+sizeup,brightness+brightnessup);
		}

		void ChangeColors(int Scolor, int Sglowcolor, int SB)
		{
			color = Scolor;
			glowcolor = Sglowcolor;
			brightness = SB;
		}

		void Resize(int Ssize)
		{
			size = Ssize;
		}
};
