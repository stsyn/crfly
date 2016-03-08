#ifndef GAMEFILES_H_INCLUDED
#define GAMEFILES_H_INCLUDED

	#include "Platform.h"
	
	#define DATAPATH(file) ("data" SEP file)
	#define TEXTUREPATH(file) (DATAPATH("textures" SEP file))
	#define DECORATIONPATH(file) (TEXTUREPATH("decoration" SEP file))
	
	#define SETTINGS_DAT DATAPATH("settings.dat")
	#define SCRSIZES_DAT DATAPATH("screensizes.dat")
	#define STYLEDAT_DAT DATAPATH("style.dat")
	#define HIGSCORE_DAT DATAPATH("hiscore.dat")
	#define LANGDATA_DAT DATAPATH("lang.dat")
	#define VERSIOND_DAT DATAPATH("version.dat")
	
	#define TEXLIST_DAT TEXTUREPATH("texlist.dat")
	#define FONLIST_DAT TEXTUREPATH("fontlist.dat")
	#define TEXTURE_PATH DATAPATH("textures" SEP)

#endif //GAMEFILES_H_INCLUDED
