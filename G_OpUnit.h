int init()
{

	cclr[0][0] = 0;
	cclr[1][0] = 0;
	cclr[2][0] = 0;
	cclr[0][1] = 0;
	cclr[1][1] = 192;
	cclr[2][1] = 255;
	cclr[0][2] = 32;
	cclr[1][2] = 192;
	cclr[2][2] = 255;


	buildLayer(128,xmax,ymax);
	buildLayer(129,xmax,ymax);

	mas[0] = new int[sz];
	mas[1] = new int[sz];
	srand(unsigned(time(NULL)));

	DrawRect(128,tRGB(0,0,0),0,0,xmax,ymax,0);
	DrawRect(129,tRGB(0,0,0),0,0,xmax,ymax,0);

	printf("init: time=%u xmax=%i ymax=%i\n", unsigned(time(NULL)), xmax, ymax);

	for (int j=0; j<sz; j++) 
	{
		mas[0][j]=gener(xmax);
		mas[1][j]=gener(ymax);
	}
	font_init();
	weapon_init();

	c=0;
	
	////////////////////////////////////////////////////
	buildLayer(200,xmax,ymax);
	buildLayer(510,32,96);
	LS();
	scr_read();

	if (Updater() != 0)
	{
		Sleep(4000);
		return 1;
	}

	hsc_read();

	css_read();

	tex_init();
	LS();
	tex_read(0);
	for (int i=1; i<=25; i++) 
	{
		tex_read(i);
	}

	testing.Load("test.spr", 420);

	str_read();

	return 0;
}

void OP()
{
	if (dbg)
	{
		Debug();
		return;
	}
	if (modif == 0)
	{
		if (scrn == 0)
		{
			MM();
		}
		else if (scrn == 1)
		{
			if (gs == 0 && pg == 1) ingameSolo();
			else if (gs == 0 && pg == 2) ingame2P();
			else if (gs == 1) ingameSP();
			else if (gs == 2) ingameRLD();
		}
	}
	else if (modif == 1) 
	{
		Demo1();
	}
	else if (modif == 2) 
	{
		Demo2();
	}
	else if (modif == 3) 
	{
		Demo3();
	}
	else if (modif == 4) 
	{
		Demo4();
	}
}

void KOP()
{
	if (get_key(KEY_F3) && (get_keypressed(KEY_F3) == 0))
	{
		dbg=!dbg;
		return;
	}
	if (dbg)
	{
		if (get_key(KEY_UP) && (get_keypressed(KEY_UP) % 20 == 0 || (get_keypressed(KEY_UP) > 60 && get_keypressed(KEY_UP) % 2 == 0)))
		{
			if (dmode==-1)
			{
				dsel--;
				if (dsel < 0) dsel=tmax-1;
			}
		}
		if (get_key(KEY_DOWN) && (get_keypressed(KEY_DOWN) % 20 == 0 || (get_keypressed(KEY_DOWN) > 60 && get_keypressed(KEY_DOWN) % 2 == 0)))
		{
			if (dmode==-1)
			{
				dsel++;
				if (dsel+1 > tmax) dsel=0;
			}
		}

		if (get_key(KEY_RETURN) && get_keypressed(KEY_RETURN) == 0)
		{
			if (dmode==-1)
			{
				int tc = 0;
				for (int i=0; i<dsel; i++)
				{
					for (; tc<=512; tc++) 
					{
						if (getWidth(tc) !=0) break;
					}
					tc++;
				}
				dmode = tc;
			}
			else dmode = -1;
		}
		return;
	}
	
	if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0 && inmenu==1)
	{
		Quit(0);
	}
	
	if (modif == 0 && scrn == 0)
	{
		if (inmenu>=121 && inmenu<=123) //colors
		{	
			int d[3];
			RGBt(tcss,&d[0],&d[1],&d[2]);
			if (get_key(KEY_LEFT)) 
			{
				if (d[sel-1] > 0) d[sel-1]--;
			}
			
			if (get_key(KEY_RIGHT)) 
			{
				if (d[sel-1] < 255) d[sel-1]++;
			}
			tcss=tRGB(d[0],d[1],d[2]);
		}
		
		if (inmenu == 1 && sel == 2) //customize select
		{
			if (get_key(KEY_LEFT)) 
			{
				ccss=0;
			}
			if (get_key(KEY_RIGHT)) 
			{
				ccss=1;
			}
		}
		if (get_key(KEY_UP) && (get_keypressed(KEY_UP) % 20 == 0 || (get_keypressed(KEY_UP) > 60 && get_keypressed(KEY_UP) % 2 == 0)))
		{
			sel--;
			if (sel < 1) sel=selmax;
		}
		if (get_key(KEY_DOWN) && (get_keypressed(KEY_DOWN) % 20 == 0 || (get_keypressed(KEY_DOWN) > 60 && get_keypressed(KEY_DOWN) % 2 == 0)))
		{
			sel++;
			if (sel > selmax) sel=1;
		}
		if (get_key(KEY_RETURN) && get_keypressed(KEY_RETURN) == 0)
		{
			inmenu = inmenu*10+sel;
			sel = 1;
		}
		if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0)
		{
			inmenu = inmenu/10;
			sel = 1;
			printf("KOP: inmenu=%i\n", inmenu);
		}
	}
	else if (modif == 0 && scrn == 1)
	{
		if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0)
		{
			if (gamestate == 0) gamestate = 1;
			else gamestate = 2;
			printf("KOP: gamestate=%i\n", gamestate);
		}
		if (get_key(KEY_RETURN) && get_keypressed(KEY_RETURN) == 0)
		{
			if (gamestate == 1) gamestate = 0;
		}
		if ((get_key(KEY_LEFT) && get_keypressed(KEY_LEFT) == 0) || (get_key(KEY_A) && get_keypressed(KEY_A) == 0) && gamestate == 0)
		{
			if (pos[0] !=0) 
			{
				pos[0]--;
				shipsmooth[0]+=16;
			}
		}
		if ((get_key(KEY_RIGHT) && get_keypressed(KEY_RIGHT) == 0) || (get_key(KEY_D) && get_keypressed(KEY_D) == 0) && gamestate == 0)
		{
			if (pos[0] !=tways-1) 
			{
				pos[0]++;
				shipsmooth[0]-=16;
			}
		}
		if (get_key(KEY_NUMPAD4) && get_keypressed(KEY_NUMPAD4) == 0 && gamestate == 0)
		{
			if (pos[1] !=0) 
			{
				pos[1]--;
				shipsmooth[1]+=16;
			}
		}
		if (get_key(KEY_NUMPAD6) && get_keypressed(KEY_NUMPAD6) == 0 && gamestate == 0)
		{
			if (pos[1] !=tways-1) 
			{
				pos[1]++;
				shipsmooth[1]-=16;
			}

		}
		for (int i = 0; i<5; i++)
		{
			if (get_key(KEY_1+i) && (wtype!=i) && get_keypressed(KEY_1+i) == 0)
			{
				if (!(wavail[i])) return;
				wtype = i;
				w_equip(i);
				bullets = 0;
				hold+=CW.delay;
			}
		}

		if (get_key(KEY_SPACE) && gamestate == 0 && gs == 2)
		{
			//bullets
			if ((fsthold > 0) || (bullets == 0) || (hold > 0)) return;
			BUL.spawnx = 162-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+7;
			BUL.spawny = 152-smoothstuff[0][4];
			bullets--;
			fsthold+=CW.reload;
			if (bullets == 0) hold+=CW.delay;

			BUL.Spawn();

		}
		if (get_key(KEY_RETURN) && gamestate == 0 && gs == 2)
		{
			if ((fsthold == 0) && (hold == 0)) hold+=CW.delay;
		}
		if ((pg != 2) && (shipstyle[0] == 5))
		{
			(get_keypressed(KEY_UP)!=0 || get_keypressed(KEY_W)!=0) ? ac5=true : ac5 = false;
			(get_keypressed(KEY_DOWN)!=0 || get_keypressed(KEY_S)!=0) ? dc5=true : dc5 = false;
		}
	}
	else
	{
		if (get_key(KEY_ESCAPE) && get_keypressed(KEY_ESCAPE) == 0)
		{
			printf("KOP: modif=0\n");
			modif=0;
		}
	}
}

int main(int argc, char** argv)
{
	if(!set_read())
	{
		fprintf(stderr, "Failed to load settings\n");
        return 1;							
	}

	if (exec() != 0)
	{
		Quit(1);
	}

	
	/* init() from Game.cpp */
	if (init() != 0)
	{
		Quit(1);
	}
	
	while(1)
	{
		if (process() !=0) Quit(1);
		/* Game.cpp */
		OP();		
		KOP();
		
	}
	
	return 0;
}