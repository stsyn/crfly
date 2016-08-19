#ifndef GAMEFILES_H_INCLUDED
#define GAMEFILES_H_INCLUDED

	#include "Platform.h"
	
	#define SPREDDATAPATH(file) ("spred" SEP file)
	#define DATAPATH(file) ("data" SEP file)
	#define TEXTUREPATH(file) (DATAPATH("textures" SEP file))
	
	#define SETTINGS_DAT SPREDDATAPATH("settings.dat")
	#define SCRSIZES_DAT DATAPATH("screensizes.dat")
	#define RCNTFLES_DAT SPREDDATAPATH("recent.dat")
	#define RCNTTGAS_DAT SPREDDATAPATH("recenttga.dat")
	#define LANGDATA_DAT SPREDDATAPATH("lang.dat")
	#define VERSIOND_DAT SPREDDATAPATH("version.dat")
	
	#define FONLIST_DAT TEXTUREPATH("fontlist.dat")
	#define TEXTURE_PATH DATAPATH("textures" SEP)
	#define SPRITES_PATH DATAPATH("sprites" SEP)

#endif //GAMEFILES_H_INCLUDED
