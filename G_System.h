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
	else if (scores[0][0] < 20000) maxships = 4;
	else maxships = 5;
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

	str_file = fopen(PATTERNS_DAT, "r" ); 
	for (i=0; i<5; i++) fread(&evecounts[i], 1, 1, str_file);
	fclose(str_file);
}

/////////////////////////////////////////////////

void close()
{
	delete[] mas[0];
	delete[] mas[1];
}


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

void SolotoSP()
{
	scores[0][4]=0;
	scores[0][5]=2;
	for (int i=0; i<6; i++) 
	{
		for (int j=0; j<20; j++) 
			{
				map[0][j][i]=0;
			}
	}
	for (i=0; i<maxabonuses; i++) bonuses[0][i]=0;
	tmult = 1;
	buildLayer(499,32,20);
	scores[0][3] = 1;
}

void gamereset()
{
	eveflag = false;
	//for UI
	buildLayer(201,xmax,ymax);
	//creating tiles
	buildLayer(511, 12, 200);
	buildLayer(509, 12, 200);
	HGradient(511,tRGB(64,64,64),tRGB(196,196,196),0,0,4,200,0);
	HGradient(511,tRGB(196,196,196),tRGB(64,64,64),4,0,8,200,0);
	int i;
	//i wanna yo ship

	buildLayer(510, 32, 96);
	MascedLayer(17,510,0,0,css[0][0],css[0][1],css[0][2]);	
	buildLayer(506, 24, 14);
	MascedLayer(16+8,506,0,0,tRGB(255,255,255),tRGB(255,255,255),css[0][2]);
	buildLayer(508, 18, 18);
	MascedLayer(16+7,508,0,0,css[0][0],tRGB(255,255,255),tRGB(255,255,255));

	//player 2 u too
	if (pg>1)
	{
		buildLayer(410, 32, 96);
		MascedLayer(17,410,0,0,css[1][0],css[1][1],css[1][2]);	
		buildLayer(406, 24, 14);
		MascedLayer(16+8,406,0,0,tRGB(255,255,255),tRGB(255,255,255),css[1][2]);
		buildLayer(408, 18, 18);
		MascedLayer(16+7,408,0,0,css[1][0],tRGB(255,255,255),tRGB(255,255,255));
	}

	//reloaded stuff
	if (gs == 2)
	{
		buildLayer(450, 32, 4);
		HGradient(450,tRGB(0,64,0),tRGB(0,255,0),0,0,32,4,0);
		buildLayer(451, 32, 4);
		HGradient(451,tRGB(64,0,0),tRGB(255,0,0),0,0,32,4,0);
		buildLayer(452, 32, 4);
		HGradient(452,tRGB(0,0,64),tRGB(0,0,255),0,0,32,4,0);
		buildLayer(453, 32, 4);
		HGradient(453,tRGB(255,0,0),tRGB(255,255,0),0,0,16,4,0);
		HGradient(453,tRGB(255,255,0),tRGB(0,255,0),16,0,16,4,0);
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
	//top bar
	if (shipstyle[0] == 5)
	{
		buildLayer(504,20,200);
		buildLayer(0,12,36);
		TransformLayer(507,0,0,0,6);
		ZoomRender(0,504,0,0,20,200,0,0,12,36,0);
		delLayer(0);
	}
	//particles
	EP.Destroy();
	EP.randangle = 22;
	EP.randspawnx = 4;
	EP.randsize = 2;
	AP.Destroy();
	AP.randangle = 360;
	AP.defspeed = 200;
	BUL.Destroy();
	//for reloaded
	xp = 0;
	w_equip(0);
	//another shit
	hold = 0;
	tbg = 0;
	cbg = 0;
	tfg = 0;
	cfg = 0;
	frame=0;
	gframe=0;
	ac5=false;
	dc5=false;
	nobonuses=false;
	tways=3;
	for (i=0; i<6; i++) 
	{
		scores[0][i]=0;
		scores[1][i]=0;
		for (int j=0; j<20; j++) 
		{
			map[0][j][i]=0;
			map[1][j][i]=0;
			metamap[j][i]=0;
		}
	}
	for (i=0; i<maxabonuses; i++) 
	{
		bonuses[0][i]=0;
		bonuses[1][i]=0;
	}
	for (i=0; i<5; i++) 
	{
		wavail[i] = false;
		smoothstuff[0][i]=0;
		smoothstuff[1][i]=0;
	}
	wavail[0] = true;
	for (i=0; i<maxbg; i++)
	{
		for (int j=0; j<3; j++) bg[i][j]=0;
	}
	for (i=0; i<maxfg; i++)
	{
		for (int j=0; j<3; j++) fg[i][j]=0;
	}
	shipsmooth[0] = 0;
	shipsmooth[1] = 0;
	scores[0][5] = 3;
	scores[0][3] = 3;
	scores[1][3] = 3;
	if (gs == 2) scores[0][3] = 30;
	hpmax = 50;
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
	if (eveflag) e_close();
	if (scores[0][0]<=scores[0][1] && pg == 1) 
	{
		hsc_write();
		hsc_read();
	}
	scrn=0;
	delLayer(511);
	delLayer(509);
	delLayer(508);
	delLayer(507);
	delLayer(506);
	delLayer(505);
	if (pg >1)
	{
		delLayer(408);
		delLayer(406);
	}
	if (gs == 2)
	{
		delLayer(450);
		delLayer(451);
		delLayer(452);
		delLayer(453);
	}
}

void spgameover()
{
	scores[0][1]+=scores[0][4]*9;
	gameover();
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