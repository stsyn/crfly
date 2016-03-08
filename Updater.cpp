#ifdef WIN32
	#include <io.h>
	#define F_OK 0
#else //UNIX
	#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include "Platform.h"
#include "Game.h"
#include "GameFiles.h"
#include "Updater.h"

FILE *version_file;
char version_path[] = VERSIOND_DAT;
const char version_name[] = "ver. 1.4\0";
const int curent_version = 7;

bool FileExists(const char *fname)
{
	return access(fname, F_OK) != -1;
}

int GetGVersion()
{
	int i=0;
	version_file = fopen(version_path, "r" ); 
	fscanf(version_file,"%i",&i);
	fclose(version_file);
	return i;
}

void ValidVersion()
{
	version_file = fopen(version_path, "w" ); 
	fprintf(version_file,"%i",curent_version);
	fclose(version_file);
}

/////////

int U1_2()
{
	char upd_telling[] = "Upgrading to 1.1\0";
	LS(upd_telling);

	//get old settings
	FILE *set_file;
	char set_path[] = SETTINGS_DAT;
	int tmp[3];
	LS(upd_telling);

	set_file = fopen(set_path, "r" ); 
	fscanf(set_file, "%i %i", &tmp[0], &tmp[1]);
	fscanf(set_file, "%i", &tmp[2]);
	fclose(set_file);
	set_file = fopen(set_path, "w" ); 
	fprintf(set_file, "%i %i ", tmp[0], tmp[1]);
	fprintf(set_file, "%i", &tmp[2]);
	LS(upd_telling);

	//add quality option
	fprintf(set_file, " 3 ");
	LS(upd_telling);
	//add keys options
	fprintf(set_file, "13 27 37 38 39 40");
	LS(upd_telling);

	fclose(set_file);

	//check files
	const int tfiles = 3;
	char files[tfiles][64];
	sprintf(files[0],"%s",STYLEDAT_DAT);
	sprintf(files[1],"%s",TEXTUREPATH("bgtex1.tga"));
	sprintf(files[2],"%s",HIGSCORE_DAT);
	LS(upd_telling);

	for (int i=0; i<tfiles; i++)
	{
		LS(upd_telling);
		if (!FileExists(files[i])) 
		{
			char strn[64];
			sprintf(strn, "File not found: %s", files[i]);
			LS(strn);
			return 2;
		}
	}
	return 0;
}

int U2_3()
{
	char upd_telling[] = "Upgrading to 1.2\0";
	LS(upd_telling);

	
	//check files
	const int tfiles = 15;
	char files[tfiles][64];
	sprintf(files[0],"%s",TEXTUREPATH("engine.tga"));
	sprintf(files[1],"%s",DECORATIONPATH("bgtex2.tga"));
	sprintf(files[2],"%s",DECORATIONPATH("planet1.tga"));
	sprintf(files[3],"%s",DECORATIONPATH("planet2.tga"));
	sprintf(files[4],"%s",DECORATIONPATH("planet3.tga"));
	sprintf(files[5],"%s",DECORATIONPATH("planet4.tga"));
	sprintf(files[6],"%s",DECORATIONPATH("planet5.tga"));
	sprintf(files[7],"%s",DECORATIONPATH("planet6.tga"));
	sprintf(files[8],"%s",DECORATIONPATH("planet7.tga"));
	sprintf(files[9],"%s",DECORATIONPATH("planet8.tga"));
	sprintf(files[10],"%s",DECORATIONPATH("planet9.tga"));
	sprintf(files[11],"%s",DECORATIONPATH("planet10.tga"));
	sprintf(files[12],"%s",DECORATIONPATH("planet11.tga"));
	sprintf(files[13],"%s",DECORATIONPATH("planet12.tga"));
	sprintf(files[14],"%s",DECORATIONPATH("bgtex1.tga"));
	LS(upd_telling);

	for (int i=0; i<tfiles; i++)
	{
		LS(upd_telling);
		if (!FileExists(files[i])) 
		{
			char strn[64];
			sprintf(strn, "File not found: %s", files[i]);
			LS(strn);
			return 2;
		}
	}

	//add shipstyle option
	FILE *css_file;
	int css[2][3],tmp1,tmp2;
	char css_path[] = STYLEDAT_DAT;
	css_file = fopen(css_path, "r" ); 

	LS(upd_telling);

	fscanf(css_file, "%x %x %x %i", &css[0][0], &css[0][1], &css[0][2], &tmp1, &tmp2);
	fclose(css_file);

	LS(upd_telling);

	if (tmp2 == 18)
	{
		LS("Unknow error during updating");
		return 4;
	}

	css_file = fopen(css_path, "w" ); 
	fprintf(css_file, "%x %x %x 0 18", css[0][0], css[0][1], css[0][2]);
	fclose(css_file);

	LS(upd_telling);
	LS(upd_telling);

	//reload scores
	long unsigned int tmp;
	char css2_path[] = HIGSCORE_DAT;
	css_file = fopen(css2_path, "r" ); 

	LS(upd_telling);

	fscanf(css_file, "%li", &tmp);
	tmp = ~((tmp+78)*2) & 0x7FFFFFFF;
	fclose(css_file);

	LS(upd_telling);

	css_file = fopen(css2_path, "w" ); 
	fprintf(css_file, "%li", tmp);
	fclose(css_file);

	LS(upd_telling);
	return 0;

}

int U5_6()
{
	char upd_telling[] = "Upgrading to 1.3\0";
	//lol
	for (int i=0; i<100; i++) LS(upd_telling);
	int css[2][3], shipstyle[2];

	FILE *css_file;
	char css_path[] = STYLEDAT_DAT;
	css_file = fopen(css_path, "r" ); 
	fscanf(css_file, "%x %x %x %i", &css[0][0], &css[0][1], &css[0][2], &shipstyle[0]);
	fclose(css_file);
	css_file = fopen(css_path, "w" ); 
	fprintf(css_file, "%x %x %x %i", css[0][0], css[0][1], css[0][2], shipstyle[0]);
	fprintf(css_file, " ff00bd00 ff855151 ff005cba 0");
	fclose(css_file);

	return 0;
}

/////////

int Updater()
{
	int v, stat=0;
	if (FileExists(version_path)) 
	{
		v = GetGVersion();
	}
	else v = 1;
	if (v>curent_version)
	{
		LS("Please install actual version");
		return 3;
	}
	for (;v<curent_version; v++)
	{
		if (v == 1) 
		{
			stat = U1_2();
		}
		if (v == 2) 
		{
			stat = U2_3();
		}
		if (v == 5) 
		{
			stat = U5_6();
		}
		if (stat !=0) break;
	}
	if (stat == 0) ValidVersion();
	return stat;
}
