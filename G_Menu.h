void MM()
{
	int cl;
	char strn[64];
	colorStuff(frame,2);
	FDrawRect(200,tRGB(cclr[0][0],cclr[0][1],cclr[0][2]),0,0,xmax,ymax); 
	renderBG(frame/2,2,quality);
	renderFG(frame/2,2);
	DrawRect(200,ARGB(64,0,0,0),0,0,xmax,ymax);
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
		LS();
		pg = 1;
		gs = 2;
		gamereset();
		LS();
		inmenu=11;
		LS();
		scrn=1;
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
		DrawRect(510,tRGB(127,127,127),0,0,32,96);
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
		DrawRect(510,tRGB(127,127,127),0,0,32,96);
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
		DrawRect(510,tRGB(127,127,127),0,0,32,96);
		MascedLayer(17,510,0,0,ttcss[0],ttcss[1],ttcss[2]);
		ZoomRender(510,200,180,40,120,120,(16*((frame >> 2) & 1)),16*(sel-1),16,16,0);
	}
	if (inmenu>=1241 && inmenu<=1246)
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
		PrintText(200,1,"SDL port by DeaDDomMER",inmenu_color,60,10+60);
		PrintText(200,1,"Using OpenGL",inmenu_color,60,10+40);
	}
	if (inmenu==1421)
	{
		inmenu=14;
	}
	if (inmenu==143) //extras
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
		frame = 0;
		inmenu = 143;
		modif = 3;
	}
	if (inmenu==1434)
	{
		frame = 0;
		inmenu = 143;
		modif = 4;
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