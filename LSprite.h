#include "Lib.h"
#define LSPRITE_FRAMES_MAX 65536
#define LSPRITE_ANIMS_MAX 32
#define LSPRITE_MAX_ANIMSIZE 65536
#define LSPRITE_MAX_METASIZE 16

class LSprite
{
private:
	int animcounter,framecounter,nextanim;

bool getBit(int i, int pos)
{
	return (i >> pos) && 1;
}
public:
	int frames,anims,contentlayer, frameOffsets[2][128],frameSizes[2][128],frameFix[2][128],anim[24][3][128], advancedflags;
	char animsize[24];
	int meta[16], pos_x, pos_y, speed, angle,cam_x,cam_y,animtype;

void Load(char *str, int targetLayer)
{
#define ALWAYS_SHOW_FIRST 1
	unsigned char oe;
	int i,j;
	char strn[128], fname[128];
	FILE *spr_file;
	sprintf(fname,"%s%s",SPRITES_PATH,str);
	spr_file = fopen(fname, "r"); 
/*

  FILE CONTENT:
  x1 - name-lenght
  x2~ - texturename
  x3 - frames counter
  x4:
	x1 - offSetX
	x2 - offSetY
	x3 - sizeX
	x4 - sizeY
  x5 - anims counter
  ; 0 - static
  ; 1 - active
  ; 2 - die
  x6:
	x1 - anim size
	x2:
	  x1 - frame num
	  x2 - frame time
	  x3 - allow changing
  x7 - meta content (16)

*/
	fread(&oe, 1, 1, spr_file);

	for (i=0; i<oe; i++) fread(&fname[i], 1, 1, spr_file);
	fname[i]='\0';

	sprintf(strn,"%s%s",TEXTURE_PATH,fname);
	tex_read(strn,targetLayer);
	contentlayer = targetLayer;

	fread(&frames, 1, 2, spr_file);

	for (i=0; i<frames; i++)
	{
		fread(&frameOffsets[0][i], 1, 2, spr_file);		
		fread(&frameOffsets[1][i], 1, 2, spr_file);		
		fread(&frameSizes[0][i], 1, 2, spr_file);		
		fread(&frameSizes[1][i], 1, 2, spr_file);		
		fread(&frameFix[0][i], 1, 2, spr_file);		
		fread(&frameFix[1][i], 1, 2, spr_file);	
		frameFix[0][i] -= 32768;
		frameFix[1][i] -= 32768;
	}

	fread(&anims, 1, 2, spr_file);
	
	for (i=0; i<anims; i++)
	{
		fread(&animsize[i], 1, 2, spr_file);	
		for (j=0; j<animsize[i]; j++)
		{
			fread(&anim[i][0][j], 1, 2, spr_file);
			fread(&anim[i][1][j], 1, 2, spr_file);	
			fread(&anim[i][2][j], 1, 4, spr_file);
		}
		
	}

	for (i=0; i<16; i++)
	{
		fread(&meta[i], 1, 2, spr_file);				
	}
	fread(&advancedflags, 1, 2, spr_file);
	fclose(spr_file); 

	animtype=0;
	nextanim = 0;
	framecounter = 0;
	animcounter=0;
	pos_x=xmax*100/2;
	pos_y=ymax*100/2;
	cam_x = 0;
	cam_y = 0;
}

void ChangeAnim(int n, bool force)
{
	nextanim = n;
	if (force)
	{
		animtype = n;
		framecounter = 0;
		animcounter = 0;
	}
}

void Update()
{
	pos_x = pos_x+cos(3.14*angle/180)*speed;
	pos_y = pos_y-sin(3.14*angle/180)*speed;
	framecounter++;
	if (framecounter == anim[animtype][1][animcounter]) 
	{
		if (nextanim != animtype)
		{
			if (getBit(anim[animtype][2][animcounter], nextanim))
			{
				animcounter = 0;
				animtype = nextanim;
			}
			else animcounter++;
		}
		else animcounter++;
		if (animcounter == animsize[animtype]) animcounter = 0;
		framecounter = 0;
	}
}

void Draw(int id)
{
	int t = anim[animtype][0][animcounter];
	if (getBit(advancedflags,ALWAYS_SHOW_FIRST)) FragmentLayer(contentlayer,id,cam_x-frameSizes[0][0]/2+pos_x/100+frameFix[0][0],cam_y-frameSizes[1][0]/2+pos_y/100+frameFix[1][0],frameOffsets[0][0],frameOffsets[1][0],frameSizes[0][0],frameSizes[1][0],0);
	FragmentLayer(contentlayer,id,cam_x-frameSizes[0][t]/2+pos_x/100+frameFix[0][t],cam_y-frameSizes[1][t]/2+pos_y/100+frameFix[1][t],frameOffsets[0][t],frameOffsets[1][t],frameSizes[0][t],frameSizes[1][t],0);
}
};
