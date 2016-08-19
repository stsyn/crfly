void weapon_init()
{
	W[0].power = 5;
	W[0].speed = 300;
	W[0].lifetime = 90;
	W[0].capacity = 2;
	W[0].delay = 90;
	W[0].reload = 45;
	W[0].color = tRGB(255,255,0);

	W[1].power = 2;
	W[1].speed = 600;
	W[1].lifetime = 60;
	W[1].capacity = 6;
	W[1].delay = 120;
	W[1].reload = 20;
	W[1].color = tRGB(255,0,0);

	W[2].power = 9;
	W[2].speed = 150;
	W[2].lifetime = 120;
	W[2].capacity = 1;
	W[2].delay = 100;
	W[2].reload = 80;
	W[2].color = tRGB(0,255,255);

	W[3].power = 8;
	W[3].speed = 900;
	W[3].lifetime = 5;
	W[3].capacity = 6;
	W[3].delay = 100;
	W[3].reload = 40;
	W[3].color = tRGB(0,0,255);

	W[4].power = 10;
	W[4].speed = 500;
	W[4].lifetime = 90;
	W[4].capacity = 4;
	W[4].delay = 180;
	W[4].reload = 40;
	W[4].color = tRGB(255,0,255);
}

void w_equip(int type)
{
	CW = W[type];
	CW.power = W[type].power+W[type].power*level/5;
	CW.delay = W[type].delay-W[type].delay*level/40;
	CW.reload = W[type].reload-W[type].reload*level/40;
	BUL.deflifetime = CW.lifetime;
	BUL.defspeed = CW.speed;
	BUL.defangle = 90;
	BUL.defcolor = CW.color;
	BUL.defincolor = CW.color;
}

void e_close()
{
	evecounter = 0;
	eveflag = false;
	fclose(event_file);
}

void e_start()
{
	int t = gener(evecounts[tways-2]);
	char strn[64];

	sprintf(strn, "%s%i.dat",PATTERNS_PATH, tways*100+t);

	evecounter = 0;
	eveflag = true;
	event_file = fopen(strn, "r" ); 
	fread(&evelasts, 1, 1, event_file);

}

void e_exec()
{
	unsigned char t;
	for (int i=0; i<tways; i++) 
	{
		fread(&t, 1, 1, event_file);
		if (t == 2) 
		{
			map[0][0][i] = generateMeteor();
			metamap[0][i] = 10;
		}
		else if (t!=0)
		{
			map[0][0][i] = t;
			metamap[0][i] = 1;
		}
		else 
		{
			map[0][0][i] = 0;
			metamap[0][i] = 0;
		}
	}
	evecounter++;
	if (evecounter == evelasts) e_close();
}

void level_up()
{
	level++;
	if (level == lvlmax) xp = 4400;
	w_equip(wtype);
	scores[0][3] = scores[0][3]*(hpmax+5)/hpmax;
	hpmax+=5;
}