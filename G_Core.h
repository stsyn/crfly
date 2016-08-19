
void ingameSP()
{
	char strn[64];
	unsigned long int i,j;
	act[0] = 0;

	nobonuses=true;
	//logic
	if (gamestate == 0) delay++;

	delayCalc();
	Smoothing(0);
	multCalc(0);

	shipstyle[0] = 5;
	if (scores[0][4] == 400) scores[0][5] = 4;

	if (delay >= smoothstuff[0][0])
	{
		delay = 0;
		scores[0][4]++;
		//generation
		mve(0);
		if (scores[0][4] > 400) generation(0);	
	}
	triggers(0, pos[0], 17, true);

	//gameover
	if (scores[0][3] <= 0 && gamestate != 2)
	{
		gamestate = 3;
		spgameover();
	}

	//tgs
	if (gamestate == 2) 
	{
		gameover();
		return;
	}
	if (gamestate == 0) gframe++;

	if (frame % (frameskip+1)== 0) 
	{
		//rendering
		reset(200);
		if (scores[0][4]>=400) 
		{
			FDrawRect(201,tRGB(0,0,0),0,0,xmax,ymax);
			renderBG(scores[0][4],smoothstuff[0][0],1);
		}
		else
		{
			if (gamestate == 0 && (frame % 4 ==0)) BWNoise(499,0,0,32,20);
			ZoomRender(499,201,0,0,320,200,0,0,32,20,0);
		}

		if (scores[0][4]<=25) DrawRect(201,ARGB((25-scores[0][4])*10,0,0,0),0,20,xmax,180,2);
		if (scores[0][4]<390 && scores[0][4]>390-255) DrawRect(201,ARGB(scores[0][4]-390+255,0,0,0),0,20,xmax,180,2);
		if (scores[0][4]<400 && scores[0][4]>=390) FDrawRect(201,tRGB(0,0,0),0,20,xmax,180);
		for (i=0; i<=tways; i++)
		{
			FDrawVLine(201, tRGB(255,255,255),160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5,0,200);
		}

		for (i=0; i<tways; i++)
		{
			for (j=2; j<22; j++)
			{
				if (map[0][j][i] == 1)
				{
					renderCoin(frame,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
				}
				else if ((map[0][j][i] >= 200) && (map[0][j][i] < 300))
				{
					renderMeteor(frame,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
					if (bonuses[0][16]!=0)
					{
						if (map[0][j][i] != 200)
						{
							DrawFrame(200,tRGB(0,196,0),154-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-8+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],14,14);
							FDrawRect(200,tRGB(0,0,0),155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],12,12);
						switch (map[0][j][i]-200)
						{
						case 1: 
							{
								renderCoin(frame,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
								break;
							}
						case 2: 
							{
								renderMeteor(frame,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
								break;
							}
						case 3: 
							{
								renderBonus(frame,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
								break;
							}
						}
						}
					}
				}
				else if ((map[0][j][i] >= 100) && (map[0][j][i] < 200))
				{
					renderBonus(frame,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
					if (bonuses[0][16]!=0)
					{
						DrawFrame(200,tRGB(0,196,0),154-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-8+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],14,14);
						FDrawRect(200,tRGB(0,0,0),155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],12,12);
						FragmentLayer(16+6,200,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],(map[0][j][i]-100)*12,0,12,12,0);
					}
				}
			}
		}

		if (smoothstuff[0][0]<=30)FragmentLayer(506,200,162-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+1+shipsmooth[0],165-smoothstuff[0][4],(12*((frame >> 2) & 1)),(smoothstuff[0][0]-2)/2,12,14-((smoothstuff[0][0]-2)/2),1);
		FragmentLayer(510,200,160-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+1+shipsmooth[0],154-smoothstuff[0][4],(16*((frame >> 2) & 1)),16*shipstyle[0],16,16,0);

		buildLayer(202,xmax,ymax);
		reset(202);
		DrawRect(201,0,0,0,xmax,ymax,4);
		IncludeLayer(200,202,0,0,3);
		IncludeLayer(202,201,0,0,0);
		delLayer(202);
		reset(200);

		ZoomRender(201,201,0,0,320,20,0,21,320,1,0);
		ZoomRender(201,201,0,180,320,20,0,179,320,1,0);

		VGradient(201,tRGB(255,255,255),tRGB(192,192,192),0,0,320,20,5);
		VGradient(201,tRGB(192,192,192),tRGB(255,255,255),0,180,320,20,5);

		menus();

		IncludeLayer(200,201,0,0,0);

		sprintf(strn,"%i | %i",fps,frameskip);
		PrintText(201,1,strn,tRGB(255,255,255),0,0);
		renderer(201);

	}
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////

void ingameRLD()
{
	/*
	RUNES
	1.  1 FEHU		collect all coins
	2. 27 JERA		meteors <-> coins
	3. 25 NAUTHIZ	boss/event
	4.  5 RAIDHO	destroy meteors
	5.  6 KENAZ		all active bonuses +20
	6. 18 SIGEL		restore default HP
	7. 15 EIHWAZ	regeneration
	*/
char strn[64];
unsigned long int i,j,t;
act[0] = 0;
act[1] = 0;

	//logic
	if (gamestate == 0) delay++;

	delayCalc();
	Smoothing(0);
	multCalc(0);

	if (delay >= smoothstuff[0][0])
	{
		delay = 0;
		scores[0][4]++;
		//oh wait, events here!
		if (scores[0][4] % 250 == 228)		//it's not special \_(0_0)_/
		{
			for (i=0; i<tways; i++)
			{
				map[0][0][i] = 401;
				metamap[0][i] = 0;
			}
		}
		mve(0);
		//generation
		if (eveflag) e_exec();
		else generation(0);
		
	//speed modifiers
		speedUpd();

	// bonuses updates
		bonusing(0);
	}

	magnetizm(0);
	triggers(0, pos[0], 17, true);

	//rpg
	if (xp>=levels[level+1] && level < lvlmax) 
	{
		//place cool animations here
		level_up();
	}

	//animations update
	colorStuff(gframe, smoothstuff[0][0]*2);

	//gameover
	if (scores[0][3] <= 0 && gamestate != 2)
	{
		gamestate = 3;
	}

	//tgs
	if (gamestate == 2) 
	{
		gameover();
		return;
	}

	if (gamestate == 0) 
	{
		//game frames
		gframe++;

		//reloading bullets
		if (hold == 1)
		{
			bullets = CW.capacity;
		}


		//bullets

		if (hold>0) hold--;
		if (fsthold>0) fsthold--;
		BUL.Update();

		for (i = 0; i<255; i++)
		{
			if (BUL.isalive[i])
			{
				if (retX(BUL.P[i])<0 || retY(BUL.P[i])<0) continue;
				if (map[0][retY(BUL.P[i])][retX(BUL.P[i])] != 0) 
				{
					metamap[retY(BUL.P[i])][retX(BUL.P[i])] -=CW.power;
					BUL.isalive[i] = false;
				}
				if (metamap[retY(BUL.P[i])][retX(BUL.P[i])]<=0) aParticles(triggers(0,retX(BUL.P[i]),retY(BUL.P[i]),false),BUL.P[i].posx/100,BUL.P[i].posy/100);
			}
		}
	}


	if (frame % (frameskip+1)== 0) 
	{
		//rendering
		FDrawRect(200,tRGB(cclr[0][0],cclr[0][1],cclr[0][2]),0,0,xmax,ymax);
		//background
		
		renderBG(scores[0][4],smoothstuff[0][0],quality);

		//hey, do you know that this game has multipliers.
		//oh, fuck, really, we have it!

		if (tmult > 1)
		{
			ZoomRender(505,200,xmax/4,(ymax - getHeight(505)*xmax/(2*getWidth(505)))/2,xmax/2,getHeight(505)*xmax/(2*getWidth(505)),0,0,getWidth(505),getHeight(505),1);
		}

		//ways
		if (quality > 0) 
		{
			reset(509);
			IncludeLayer(511,509,0,0,0);
			DrawRect(509,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),0,0,12,200,2);
			VGradient(509,tRGB(0,0,0),tRGB(128,128,128),0,200-((frame << 2) & 4095),xmax,10,1);
			VGradient(509,tRGB(128,128,128),tRGB(0,0,0),0,210-((frame << 2) & 4095),xmax,10,1);
		}
		for (i=0; i<tways; i++)
		{
			IncludeLayer(509,200,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,0,1);
		}
		for (i=0; i<=tways; i++)
		{
			FDrawVLine(200, tRGB(196,196,196),160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5,0,200);
		}
		//objects
		for (i=0; i<tways; i++)
		{
			for (j=2; j<22; j++)
			{
				if (map[0][j][i] == 1)
				{
					renderCoin(frame,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
				}
				else if ((map[0][j][i] >= 200) && (map[0][j][i] < 300))
				{
					renderMeteor(frame,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
					if (bonuses[0][16]!=0)
					{
						if (map[0][j][i] != 200)
						{
							DrawFrame(200,tRGB(0,196,0),154-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-8+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],14,14);
							FDrawRect(200,tRGB(0,0,0),155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],12,12);
						switch (map[0][j][i]-200)
						{
						case 1: 
							{
								renderCoin(frame,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
								break;
							}
						case 2: 
							{
								renderMeteor(frame,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
								break;
							}
						case 3: 
							{
								renderBonus(frame,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
								break;
							}
						}
						}
					}
					if (metamap[j][i]!=10)
					{
						DrawHLine(200,tRGB(0,255,0),160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+4,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],metamap[j][i]);		
						DrawHLine(200,tRGB(255,0,0),160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+4+metamap[j][i],12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],10-metamap[j][i]);
					}
				}
				else if ((map[0][j][i] >= 100) && (map[0][j][i] < 200))
				{
					renderBonus(frame,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
					if (bonuses[0][16]!=0)
					{
						DrawFrame(200,tRGB(0,196,0),154-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-8+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],14,14);
						FDrawRect(200,tRGB(0,0,0),155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],12,12);
						FragmentLayer(16+6,200,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],(map[0][j][i]-100)*12+12,0,12,12,0);
					}
				}
				else if ((map[0][j][i] >= 300) && (map[0][j][i] < 400))
				{
					renderPW(frame,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
					if (bonuses[0][16]!=0)
					{
						DrawFrame(200,tRGB(0,196,0),154-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-8+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],14,14);
						FDrawRect(200,tRGB(0,0,0),155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],12,12);
						FragmentLayer(16+6,200,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],(map[0][j][i]-300)*12+12,0,12,12,0);
					}
				}
				else if ((map[0][j][i] >= 400) && (map[0][j][i] < 500))
				{
					renderRune(map[0][j][i]-400,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
					if (bonuses[0][16]!=0)
					{
						DrawFrame(200,tRGB(0,196,0),154-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-8+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],14,14);
						FDrawRect(200,tRGB(0,0,0),155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],12,12);
						FragmentLayer(16+6,200,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],0,0,12,12,0);
					}
				}
			}
		}
		//particles
		if (quality>1)
		{
			EP.defcolor=css[0][2];
			EP.spawnx = 162-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+7+shipsmooth[0];
			EP.spawny = 165-smoothstuff[0][4];
			if (gamestate == 0)
			{
				if (frame % smoothstuff[0][0] == 0) EP.Spawn(500-smoothstuff[0][0]*10,270);
				else if ((frame % (smoothstuff[0][0]/2) == 0) && (quality >= 3)) EP.Spawn(500-smoothstuff[0][0]*10,270);
				EP.Update();
			}
			EP.Draw(200,level,0);

			if (act[0] !=0)
			{
				aParticles(act[0],162-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+7,152-smoothstuff[0][4]);
			}

			AP.Update();
			AP.Draw(200,level,0);
		}

		BUL.Draw(200);

		//spaceship

		if (smoothstuff[0][0]<=30)FragmentLayer(506,200,162-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+1+shipsmooth[0],165-smoothstuff[0][4],(12*((frame >> 2) & 1)),(smoothstuff[0][0]-2)/2,12,14-((smoothstuff[0][0]-2)/2),1);
		FragmentLayer(510,200,160-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+1+shipsmooth[0],154-smoothstuff[0][4],(16*((frame >> 2) & 1)),16*shipstyle[0],16,16,0);
		if (bonuses[0][3] != 0 || bonuses[0][6] != 0) 
		{
			IncludeLayer(508,200,159-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+1+shipsmooth[0],153-smoothstuff[0][4],1);
		}

		//fg effects
		
		renderFG(scores[0][4],smoothstuff[0][0]);

		//animations time!
		if (anim[1] != 0)
		{
			if (anim[0] == 2) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,0,255,0),0,0,xmax,ymax,1);
			else if (anim[0] == 3) DrawRect(200,ARGB(255*(10-anim[1])/10,255,0,0),0,0,xmax,ymax,1);
			else if (anim[0] == 4) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,196,196,0),0,0,xmax,ymax,1);
			else if (anim[0] == 5) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,0,196,196),0,0,xmax,ymax,1);
			else if (anim[0] == 6) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,255,255,255),0,0,xmax,ymax,1);
		}

		//bonuses UI
		t = 0;
		for (i = 0; i<maxabonuses; i++)
		{
			if (bonuses[0][i] != 0)
			{
				int tt;
				if (bonuses[0][i] > 100) 
				{
					DrawFrame(200,tRGB(0,196,0),50,20+t*20,54,18);
					tt = 100;
				}
				else 
				{
					DrawFrame(200,tRGB(196,196,196),50,20+t*20,54,18);
					tt = bonuses[0][i];
				}
				FDrawRect(200,tRGB(0,0,0),51,21+t*20,52,16);
				FDrawRect(200,tRGB(0,196,0),52,22+t*20,14,14);
				FDrawRect(200,tRGB(0,0,0),53,23+t*20,12,12);
				FragmentLayer(16+6,200,53,23+t*20,i*12+12,0,12,12,0);
				FragmentLayer(507,200,67,23+t*20,0,0,36*tt/100,12,0);
				t++;
			}
		}

		//UI
		reset(201);
		if (quality == 0) FDrawRect(201,tRGB(0,0,0),0,180,320,20);
		else if (quality == 3) VGradient(201,ARGB(224,0,0,0),ARGB(64,0,0,0),0,180,320,20,0);
		else DrawRect(201,ARGB(127,0,0,0),0,180,320,20);
		IncludeLayer(21,201,80,183,0);
		PrintNum(201,2,smoothstuff[0][1],tRGB(196,196,196),108,191,6);
		if (scores[0][0] == scores[0][1]) DrawRect(201,tRGB(0,196,0),80,191,57,6,2);
		PrintNum(201,2,smoothstuff[0][2],tRGB(196,196,196),189,191,6);
		PrintNum(201,2,smoothstuff[0][3],tRGB(196,196,196),108,183,6);
		//PrintNum(201,2,scores[0][3],tRGB(196,196,196),257,183,6);
		DrawFrame(201,tRGB(192,192,192),257,183,34,6);
		FragmentLayer(453,201,258,184,0,0,scores[0][3]*32/hpmax,4,0);
		PrintNum(201,2,scores[0][4],tRGB(196,196,196),189,183,6);
		//PrintNum(201,2,smoothstuff[0][0],tRGB(196,196,196),234,191,6);
		if (fsthold!=0 || hold!=0) DrawFrame(201,tRGB(192,192,192),257,191,34,6);
		else DrawFrame(201,tRGB(0,192,0),257,191,34,6);
		FragmentLayer(450,201,258,192,0,0,(CW.delay-hold)*32/CW.delay,4,0);
		FragmentLayer(451,201,258,192,0,0,(CW.reload-fsthold)*32/CW.reload,4,1);
		FragmentLayer(452,201,258,192,0,0,bullets*32/CW.capacity,4,1);
		if (shipstyle[0] == 5)
		{	
			if (quality == 0) FDrawRect(201,tRGB(0,0,0),0,0,20,180);
			else if (quality == 3) DrawRect(201,ARGB(224,0,0,0),0,0,20,180);
			else DrawRect(201,ARGB(127,0,0,0),0,0,20,180);
			if (quality == 0) FDrawRect(201,tRGB(0,0,0),300,0,20,180);
			else if (quality == 3) DrawRect(201,ARGB(224,0,0,0),300,0,20,180);
			else DrawRect(201,ARGB(127,0,0,0),300,0,20,180);
			FDrawHLine(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),20,180,280);
			FDrawVLine(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),20,0,200);
			FDrawVLine(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),299,0,200);
			FragmentLayer(504,201,0,smoothstuff[0][0]*4,0,smoothstuff[0][0]*4,20,200-smoothstuff[0][0]*4,0);
			FragmentLayer(504,201,300,smoothstuff[0][0]*4,0,smoothstuff[0][0]*4,20,200-smoothstuff[0][0]*4,0);
		}
		else FDrawHLine(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),0,180,320);

		//weapons
		for (i = 0; i<5; i++)
		{
			if (wtype == i) DrawFrame(201,tRGB(0,255,0),3+15*i,183,14,14);
			else DrawFrame(201,tRGB(192,192,192),3+15*i,183,14,14);

			if (wavail[i]) FragmentLayer(16+6,201,4+15*i,184,i*12+240,0,12,12,0);
		}
		sprintf(strn,"%i",level+1);
		PrintText(201,1,"LVL",tRGB(255,255,0),294,182);
		PrintText(201,1,strn,tRGB(255,255,0),294,190);

		DrawHLine(201,tRGB(255,255,0),0,199,(xp-levels[level])*200/(levels[level+1]-levels[level]));

		IncludeLayer(201,200,0,0,0);

		if (bonuses[0][7] != 0) DrawRect(200,0,0,0,xmax,ymax,4);
		if (bonuses[0][10] != 0) 
		{
			DoGray(200,200,0,0,0);
		}
		else if (bonuses[0][8] != 0) 
		{
			DoGray(200,200,254,0,0);
			if (quality > 1) DrawRect(200,tRGB(192,255,255),0,0,xmax,ymax,2);
		}

		menus();

		sprintf(strn,"%i | %i",fps,frameskip);
		PrintText(200,1,strn,tRGB(255,255,255),0,0);
		renderer(200);
	}
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////

void ingameSolo()
{
char strn[64];
unsigned long int i,j,t;
act[0] = 0;
act[1] = 0;

	//logic
	if (gamestate == 0) delay++;

	delayCalc();
	Smoothing(0);
	multCalc(0);

	if (delay >= smoothstuff[0][0])
	{
		delay = 0;
		scores[0][4]++;
		//generation
		mve(0);
		generation(0);
		// ultra multiplier
		if (scores[0][4] % 100 == 0 && scores[0][5] == 2)
		{
			anim[0] = 6;
			anim[1] = 20;
			tmult = tmult << 1;

			sprintf(strn,"x%li",tmult);
			buildLayer(505,TextWidth(1,strn),TextHeight(1,strn));
			PrintText(505,1,strn,tRGB(48,48,24),0,0);
		}
		
	//speed modifiers
		speedUpd();

		if (
			(level == 0 && scores[0][1] >1000) ||
			(level == 1 && scores[0][1] >2500) ||
			(level == 2 && scores[0][1] >5000) ||
			(level == 3 && scores[0][1] >10000) ||
			(level == 4 && scores[0][1] >20000)
			)
		{
			anim[0] = 2;
			anim[1] = 20;
			scores[0][3]++;
			level++;
		}
		 

	// bonuses updates
		bonusing(0);
	}
	if (scores[0][1] >20000 && scores[0][0]<=scores[0][1]) 
	{
		gs = 1;
		SolotoSP();
	}

	magnetizm(0);
	triggers(0, pos[0], 17, true);

	//animations update
	colorStuff(gframe, smoothstuff[0][0]*2);

	//gameover
	if (scores[0][3] <= 0 && gamestate != 2)
	{
		gamestate = 3;
	}

	//tgs
	if (gamestate == 2) 
	{
		gameover();
		return;
	}

	//game frames
	if (gamestate == 0) gframe++;

	if (frame % (frameskip+1)== 0) 
	{
		//rendering
		FDrawRect(200,tRGB(cclr[0][0],cclr[0][1],cclr[0][2]),0,0,xmax,ymax);
		//background
		
		renderBG(scores[0][4],smoothstuff[0][0],quality);

		//hey, do you know that this game has multipliers.
		//oh, fuck, really, we have it!

		if (tmult > 1)
		{
			ZoomRender(505,200,xmax/4,(ymax - getHeight(505)*xmax/(2*getWidth(505)))/2,xmax/2,getHeight(505)*xmax/(2*getWidth(505)),0,0,getWidth(505),getHeight(505),1);
		}

		//ways
		if (quality > 0) 
		{
			reset(509);
			IncludeLayer(511,509,0,0,0);
			DrawRect(509,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),0,0,12,200,2);
			VGradient(509,tRGB(0,0,0),tRGB(128,128,128),0,200-((frame << 2) & 4095),xmax,10,1);
			VGradient(509,tRGB(128,128,128),tRGB(0,0,0),0,210-((frame << 2) & 4095),xmax,10,1);
		}
		for (i=0; i<tways; i++)
		{
			IncludeLayer(509,200,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,0,1);
		}
		for (i=0; i<=tways; i++)
		{
			FDrawVLine(200, tRGB(196,196,196),160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5,0,200);
		}
		//objects
		for (i=0; i<tways; i++)
		{
			for (j=2; j<22; j++)
			{
				if (map[0][j][i] == 1)
				{
					renderCoin(frame,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
				}
				else if ((map[0][j][i] >= 200) && (map[0][j][i] < 300))
				{
					renderMeteor(frame,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
					if (bonuses[0][16]!=0)
					{
						if (map[0][j][i] != 200)
						{
							DrawFrame(200,tRGB(0,196,0),154-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-8+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],14,14);
							FDrawRect(200,tRGB(0,0,0),155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],12,12);
						switch (map[0][j][i]-200)
						{
						case 1: 
							{
								renderCoin(frame,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
								break;
							}
						case 2: 
							{
								renderMeteor(frame,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
								break;
							}
						case 3: 
							{
								renderBonus(frame,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
								break;
							}
						}
						}
					}
				}
				else if ((map[0][j][i] >= 100) && (map[0][j][i] < 200))
				{
					renderBonus(frame,160-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
					if (bonuses[0][16]!=0)
					{
						DrawFrame(200,tRGB(0,196,0),154-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-8+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],14,14);
						FDrawRect(200,tRGB(0,0,0),155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],12,12);
						FragmentLayer(16+6,200,155-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4],(map[0][j][i]-100)*12+12,0,12,12,0);
					}
				}
			}
		}
		//particles
		if (quality>1)
		{
			EP.defcolor=css[0][2];
			EP.spawnx = 162-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+7+shipsmooth[0];
			EP.spawny = 165-smoothstuff[0][4];
			if (gamestate == 0)
			{
				if (frame % smoothstuff[0][0] == 0) EP.Spawn(500-smoothstuff[0][0]*10,270);
				else if ((frame % (smoothstuff[0][0]/2) == 0) && (quality >= 3)) EP.Spawn(500-smoothstuff[0][0]*10,270);
				EP.Update();
			}
			EP.Draw(200,level,0);

			if (act[0] !=0)
			{
				aParticles(act[0],162-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+7, 152-smoothstuff[0][4]);
			}

			AP.Update();
			AP.Draw(200,level,0);
		}
		//spaceship

		if (smoothstuff[0][0]<=30)FragmentLayer(506,200,162-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+1+shipsmooth[0],165-smoothstuff[0][4],(12*((frame >> 2) & 1)),(smoothstuff[0][0]-2)/2,12,14-((smoothstuff[0][0]-2)/2),1);
		FragmentLayer(510,200,160-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+1+shipsmooth[0],154-smoothstuff[0][4],(16*((frame >> 2) & 1)),16*shipstyle[0],16,16,0);
		if (bonuses[0][3] != 0 || bonuses[0][6] != 0) 
		{
			IncludeLayer(508,200,159-(tways*12+(tways+1)*5)/2+pos[0]*12+(pos[0]+1)*5+1+shipsmooth[0],153-smoothstuff[0][4],1);
		}

		//fg effects
		
		renderFG(scores[0][4],smoothstuff[0][0]);

		//animations time!
		if (anim[1] != 0)
		{
			if (anim[0] == 2) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,0,255,0),0,0,xmax,ymax,1);
			else if (anim[0] == 3) DrawRect(200,ARGB(255*(10-anim[1])/10,255,0,0),0,0,xmax,ymax,1);
			else if (anim[0] == 4) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,196,196,0),0,0,xmax,ymax,1);
			else if (anim[0] == 5) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,0,196,196),0,0,xmax,ymax,1);
			else if (anim[0] == 6) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,255,255,255),0,0,xmax,ymax,1);
		}

		//bonuses UI
		t = 0;
		for (i = 0; i<maxabonuses; i++)
		{
			if (bonuses[0][i] != 0)
			{
				int tt;
				if (bonuses[0][i] > 100) 
				{
					DrawFrame(200,tRGB(0,196,0),50,20+t*20,54,18);
					tt = 100;
				}
				else 
				{
					DrawFrame(200,tRGB(196,196,196),50,20+t*20,54,18);
					tt = bonuses[0][i];
				}
				FDrawRect(200,tRGB(0,0,0),51,21+t*20,52,16);
				FDrawRect(200,tRGB(0,196,0),52,22+t*20,14,14);
				FDrawRect(200,tRGB(0,0,0),53,23+t*20,12,12);
				FragmentLayer(16+6,200,53,23+t*20,i*12+12,0,12,12,0);
				FragmentLayer(507,200,67,23+t*20,0,0,36*tt/100,12,0);
				t++;
			}
		}

		//UI
		reset(201);
		if (quality == 0) FDrawRect(201,tRGB(0,0,0),0,180,320,20);
		else if (quality == 3) VGradient(201,ARGB(224,0,0,0),ARGB(64,0,0,0),0,180,320,20,0);
		else DrawRect(201,ARGB(127,0,0,0),0,180,320,20);
		IncludeLayer(21,201,57,183,0);
		PrintNum(201,2,smoothstuff[0][1],tRGB(196,196,196),85,191,6);
		if (scores[0][0] == scores[0][1]) DrawRect(201,tRGB(0,196,0),57,191,57,6,2);
		PrintNum(201,2,smoothstuff[0][2],tRGB(196,196,196),166,191,6);
		PrintNum(201,2,smoothstuff[0][3],tRGB(196,196,196),85,183,6);
		PrintNum(201,2,scores[0][3],tRGB(196,196,196),234,183,6);
		PrintNum(201,2,scores[0][4],tRGB(196,196,196),166,183,6);
		PrintNum(201,2,smoothstuff[0][0],tRGB(196,196,196),234,191,6);
		if (shipstyle[0] == 5)
		{	
			if (quality == 0) FDrawRect(201,tRGB(0,0,0),0,0,20,180);
			else if (quality == 3) DrawRect(201,ARGB(224,0,0,0),0,0,20,180);
			else DrawRect(201,ARGB(127,0,0,0),0,0,20,180);
			if (quality == 0) FDrawRect(201,tRGB(0,0,0),300,0,20,180);
			else if (quality == 3) DrawRect(201,ARGB(224,0,0,0),300,0,20,180);
			else DrawRect(201,ARGB(127,0,0,0),300,0,20,180);
			FDrawHLine(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),20,180,280);
			FDrawVLine(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),20,0,200);
			FDrawVLine(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),299,0,200);
			FragmentLayer(504,201,0,smoothstuff[0][0]*4,0,smoothstuff[0][0]*4,20,200-smoothstuff[0][0]*4,0);
			FragmentLayer(504,201,300,smoothstuff[0][0]*4,0,smoothstuff[0][0]*4,20,200-smoothstuff[0][0]*4,0);
		}
		else FDrawHLine(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),0,180,320);

		IncludeLayer(201,200,0,0,0);

		if (bonuses[0][7] != 0) DrawRect(200,0,0,0,xmax,ymax,4);
		if (bonuses[0][10] != 0) 
		{
			DoGray(200,200,0,0,0);
		}
		else if (bonuses[0][8] != 0) 
		{
			DoGray(200,200,254,0,0);
			if (quality > 1) DrawRect(200,tRGB(192,255,255),0,0,xmax,ymax,2);
		}

		menus();

		sprintf(strn,"%i | %i",fps,frameskip);
		PrintText(200,1,strn,tRGB(255,255,255),0,0);
		renderer(200);
	}
}

////////////////////////////////////////////////

////////////////////////////////////////////////

////////////////////////////////////////////////

////////////////////////////////////////////////

void ingame2P()
{
char strn[64];
unsigned long int i,j,t;
act[0] = 0;
act[1] = 0;
	//logic
	if (gamestate == 0) delay++;

	delayCalc();
	Smoothing(0);
	Smoothing(1);
	multCalc(0);
	multCalc(1);

	if (delay >= smoothstuff[0][0])
	{
		delay = 0;
		scores[0][4]++;
		scores[1][4]++;
		//generation
		if (scores[0][3] > 0) 
		{
			mve(0);
			generation(0);
		}
		if (scores[1][3] > 0) 
		{
			mve(1);
			generation(1);
		}
	
	//speed modifiers
		speedUpd();

	// bonuses updates
		bonusing(0);
		bonusing(1);
	}

	
	if (scores[0][3] > 0) 
	{
		triggers(0,pos[0],17, true);
		magnetizm(0);
	}
	if (scores[1][3] > 0) 
	{
		triggers(1,pos[1],17, true);
		magnetizm(1);
	}

	//animations update
	colorStuff(gframe, smoothstuff[0][0]*2);

	//gameover
	if (scores[0][3] <= 0 && scores[1][3] <= 0 && gamestate != 2)
	{
		gamestate = 3;
	}

	//tgs
	if (gamestate == 2) 
	{
		gameover();
		return;
	}

	//game frames
	if (gamestate == 0) gframe++;

	if (frame % (frameskip+1)== 0) 
	{
		//rendering
		FDrawRect(200,tRGB(cclr[0][0],cclr[0][1],cclr[0][2]),0,0,xmax,ymax);
		//background
		
		renderBG(scores[0][4],smoothstuff[0][0],quality);

		//ways
		if (quality > 0) 
		{
			reset(509);
			IncludeLayer(511,509,0,0,0);
			DrawRect(509,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),0,0,12,215,2);
			VGradient(509,tRGB(0,0,0),tRGB(128,128,128),0,215-((frame << 2) & 4095),xmax,10,1);
			VGradient(509,tRGB(128,128,128),tRGB(0,0,0),0,225-((frame << 2) & 4095),xmax,10,1);
		}
		for (i=0; i<tways; i++)
		{
			IncludeLayer(509,200,100-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,0,1);
			IncludeLayer(509,200,215-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,0,1);
		}
		for (i=0; i<=tways; i++)
		{
			FDrawVLine(200, tRGB(196,196,196),100-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5,0,200);
			FDrawVLine(200, tRGB(196,196,196),215-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5,0,200);
		}
		//objects
		int ole;
		for (ole=0; ole<pg; ole++)
		{
			if (scores[ole][3] <= 0) continue;
		for (i=0; i<tways; i++)
		{
			for (j=2; j<22; j++)
			{
				if (map[ole][j][i] == 1)
				{
					renderCoin(frame,100+115*ole-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4]);
				}
				else if ((map[ole][j][i] >= 200) && (map[ole][j][i] < 300))
				{
					renderMeteor(frame,100+115*ole-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4]);
					if (bonuses[ole][16]!=0)
					{
						if (map[ole][j][i] != 200)
						{
							DrawFrame(200,tRGB(0,196,0),22+186*ole-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-8+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],14,14);
							FDrawRect(200,tRGB(0,0,0),23+186*ole-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],12,12);
						switch (map[ole][j][i]-200)
						{
						case 1: 
							{
								renderCoin(frame,23+186*ole-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
								break;
							}
						case 2: 
							{
								renderMeteor(frame,23+186*ole-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
								break;
							}
						case 3: 
							{
								renderBonus(frame,23+186*ole-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[0][0])-smoothstuff[0][4]);
								break;
							}
						}
						}
					}
				}
				else if ((map[ole][j][i] >= 100) && (map[ole][j][i] < 200))
				{
					renderBonus(frame,100+115*ole-(tways*12+(tways+1)*5)/2+i*12+(i+1)*5+3,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4]);
					if (bonuses[ole][16]!=0)
					{
						DrawFrame(200,tRGB(0,196,0),22+186*ole-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-8+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],14,14);
						FDrawRect(200,tRGB(0,0,0),23+186*ole-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],12,12);
						FragmentLayer(16+6,200,23+186*ole-(tways*12+(tways+1)*5)/2+(tways)*12+((tways+2)+1)*5,12*(j-4)-7+(delay*12/smoothstuff[ole][0])-smoothstuff[ole][4],(map[ole][j][i]-100)*12+12,0,12,12,0);
					}
				}
			}
		}
		}
		for (ole=0; ole<pg; ole++)
		{
		//particles
		if (quality>1)
		{
			EP.defcolor=css[ole][2];
			EP.spawnx = 102+115*ole-(tways*12+(tways+1)*5)/2+pos[ole]*12+(pos[ole]+1)*5+7+shipsmooth[ole];
			EP.spawny = 165-smoothstuff[0][4];
			if (gamestate == 0)
			{
				if (frame % smoothstuff[0][0] == 0) EP.Spawn(500-smoothstuff[0][0]*10,270);
				if ((frame % (smoothstuff[0][0]/2) == 0) && (quality >= 3)) EP.Spawn(500-smoothstuff[0][0]*10,270);
				EP.Update();
			}
			EP.Draw(200,level,0);

			if (act[ole] !=0)
			{
				aParticles(act[ole],102+115*ole-(tways*12+(tways+1)*5)/2+pos[ole]*12+(pos[ole]+1)*5+7,152-smoothstuff[0][4]);
			}

			AP.Update();
			AP.Draw(200,level,0);
		}

		//spaceship
		if (smoothstuff[0][0]<=30 && scores[ole][3] > 0) FragmentLayer(506-ole*100,200,102+115*ole-(tways*12+(tways+1)*5)/2+pos[ole]*12+(pos[ole]+1)*5+1+shipsmooth[ole],165-smoothstuff[ole][4],(12*((frame >> 2) & 1)),(smoothstuff[0][0]-2)/2,12,14-((smoothstuff[ole][0]-2)/2),1);
		FragmentLayer(510-ole*100,200,100+115*ole-(tways*12+(tways+1)*5)/2+pos[ole]*12+(pos[ole]+1)*5+1+shipsmooth[ole],154-smoothstuff[ole][4],(16*((frame >> 2) & 1)),16*shipstyle[ole],16,16,0);
		if ((bonuses[ole][3] != 0 || bonuses[ole][6] != 0) && scores[ole][3] > 0) 
		{
			IncludeLayer(508-ole*100,200,99+115*ole-(tways*12+(tways+1)*5)/2+pos[ole]*12+(pos[ole]+1)*5+1+shipsmooth[ole],153-smoothstuff[ole][4],1);
		}
		}

		//fg effects
		
		renderFG(scores[0][4],smoothstuff[0][0]);

		//animations time!
		if (anim[1] != 0)
		{
			if (anim[0] == 2) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,0,255,0),0,0,xmax,ymax,1);
			else if (anim[0] == 3) DrawRect(200,ARGB(255*(10-anim[1])/10,255,0,0),0,0,xmax,ymax,1);
			else if (anim[0] == 4) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,196,196,0),0,0,xmax,ymax,1);
			else if (anim[0] == 5) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,0,196,196),0,0,xmax,ymax,1);
			else if (anim[0] == 6) DrawRect(200,ARGB(255*(10-abs(10-anim[1]))/10,255,255,255),0,0,xmax,ymax,1);
		}
		
		FDrawRect(200,tRGB(192,192,192),159,0,2,200);

		//bonuses UI
		for (ole = 0; ole<pg; ole++) 
		{
		int ot = 0;
		int et = 0;
		for (i = 0; i<maxabonuses; i++)
		{
			if (bonuses[ole][i] != 0)
			{
				int xa;
				if (i == 4 || i == 5 || i == 7 || i == 8 || i == 10) 
				{
					t = ot;
					ot++;
					xa = 133;
				}
				else 
				{	
					t = et;
					et++;
					xa = 10+236*ole;
				}
				int tt;
				if (bonuses[ole][i] > 100) 
				{
					DrawFrame(200,tRGB(0,196,0),xa,20+t*20,54,18);
					tt = 100;
				}
				else 
				{
					DrawFrame(200,tRGB(196,196,196),xa,20+t*20,54,18);
					tt = bonuses[ole][i];
				}
				FDrawRect(200,tRGB(0,0,0),xa+1,21+t*20,52,16);
				FDrawRect(200,tRGB(0,196,0),xa+2,22+t*20,14,14);
				FDrawRect(200,tRGB(0,0,0),xa+3,23+t*20,12,12);
				FragmentLayer(16+6,200,xa+3,23+t*20,i*12+12,0,12,12,0);
				FragmentLayer(507,200,xa+17,23+t*20,0,0,36*tt/100,12,0);
			}
		}
		}

		//UI
		reset(201);
		if (quality == 0) 
		{
			FDrawRect(201,tRGB(0,0,0),0,152,72,48);
			FDrawRect(201,tRGB(0,0,0),249,152,72,48);
		}
		else 
		{
			DrawRect(201,ARGB(127,0,0,0),0,152,72,48);
			DrawRect(201,ARGB(127,0,0,0),249,152,72,48);
		}
		IncludeLayer(16+23,201,0,153,0);
		if (scores[0][1]>scores[1][1]) PrintNum(201,2,smoothstuff[0][2],tRGB(128,224,128),42,153,6);
		else PrintNum(201,2,smoothstuff[0][2],tRGB(224,224,192),42,153,6);
		if (scores[1][1]>scores[0][1]) PrintNum(201,2,smoothstuff[1][2],tRGB(128,224,128),250,153,6);
		else PrintNum(201,2,smoothstuff[1][2],tRGB(224,224,192),250,153,6);
		if (scores[0][3] > 0) 
		{
			PrintNum(201,2,smoothstuff[0][3],tRGB(224,224,192),42,163,6);
			PrintNum(201,2,scores[0][3],tRGB(224,224,192),42,183,6);
			PrintNum(201,2,scores[0][4],tRGB(224,224,192),42,173,6);
			PrintNum(201,2,smoothstuff[0][0],tRGB(224,224,192),42,193,6);
		}
		if (scores[1][3] > 0) 
		{
			PrintNum(201,2,smoothstuff[1][3],tRGB(224,224,192),250,163,6);
			PrintNum(201,2,scores[1][3],tRGB(224,224,192),250,183,6);
			PrintNum(201,2,scores[1][4],tRGB(224,224,192),250,173,6);
			PrintNum(201,2,smoothstuff[1][0],tRGB(224,224,192),250,193,6);
		}
		DrawFrame(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),-1,151,74,50);
		DrawFrame(201,tRGB(cclr[1][0],cclr[1][1],cclr[1][2]),248,151,74,50);
		IncludeLayer(201,200,0,0,0);

		if (bonuses[0][7] != 0) DrawRect(200,0,0,0,xmax,ymax,4);
		if (bonuses[0][10] != 0) 
		{
			DoGray(200,200,0,0,0);
		}
		else if (bonuses[0][8] != 0) 
		{
			DoGray(200,200,254,0,0);
			if (quality > 1) DrawRect(200,tRGB(192,255,255),0,0,xmax,ymax,2);
		}

		menus();

		sprintf(strn,"%i | %i",fps,frameskip);
		PrintText(200,1,strn,tRGB(255,255,255),0,0);
		renderer(200);
	}
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

void Demo1()
{	
char strn[64];
	if ((f & 2) == 0) r=(rand() & 3)+4;
		
		//reset(cnv[0],xmax,ymax);
		DrawRect(128,tRGB(0,0,0),0,0,xmax,ymax,0);
		for (unsigned int i=0; i<sz; i++)
		{
			nclr[0]=f & 127;
			nclr[1]=(f / 2) & 127;
			nclr[2]=(f / 3) & 127;
			
			for (int j=-r; j<=r * 1; j++)
			{
				for (int k=-r; k<=r * 1; k++)
				{
					//if (sqrt(abs(j)*abs(j)+abs(k)*abs(k))<=r)
					if ((abs(j)+abs(k))<=r)
					{
						clr=255*(r+1-(abs(j)+abs(k)))/r;
						//clr=255*(r+1-sqrt(abs(j)*abs(j)+abs(k)*abs(k)))/r;
						if (mas[1][i]+c+k<ymax)
						{
							plot(128,mas[0][i]+j,mas[1][i]+k+c,clr, nclr[0],nclr[1],nclr[2]);
						}
						else 
						{
							plot(128,mas[0][i]+j,mas[1][i]+c-ymax+k,clr, nclr[0],nclr[1],nclr[2]);
						}
					}
				}
			}
	
			if (mas[1][i]+c<ymax)
			{
				plot(128, mas[0][i],mas[1][i]+c, nclr[0]+127,nclr[1]+127,nclr[2]+127);
			}
			else
			{
				plot(128, mas[0][i],mas[1][i]+c-ymax, nclr[0]+127,nclr[1]+127,nclr[2]+127);
			}
		}
		c++;
		f++;
	
		DrawRect(129,ARGB(4,0,0,0),0,0,xmax,ymax,2);
		IncludeLayer(128, 129, 0, 0, 1);
	
		DrawRect(129,tRGB(0,0,0),0,0,80,14,2);
		sprintf(strn,"%i | %i | %i",fps,frameskip, dticks);
		PrintText(129,1,strn,tRGB(255,255,255),0,0);
		//PrintText(129,1,"Demo1",tRGB(255,255,255),0,0);
		if (frame % (frameskip+1)== 0) renderer(129);
	
		if (c>ymax) c-=ymax;
}


void Demo2()
{
char strn[64], str2[32];
	if (((f >> 4) & 1) == 0) r=(rand() & 3)+4;
		
		reset(128);
		DrawRect(129, tRGB(0,0,0),0,0,xmax,ymax, 0);
		for (unsigned int i=0; i<sz; i++)
		{
			nclr[0]=127;
			nclr[1]=127;
			nclr[2]=127;
			
			for (int j=-r; j<=r; j++)
			{
				for (int k=-r; k<=r; k++)
				{
					if ((abs(j)+abs(k))<=r)
					{
						clr=255*(r+1-(abs(j)+abs(k)))/r;
						add(129,mas[0][i]+j,mas[1][i]+k,clr, nclr[0],nclr[1],nclr[2]);
						
					}
				}	
	
				plot(129, mas[0][i],mas[1][i], 255,255,255);
			}
		}
		
		printf("Demo2: mas[0][0]=%i mas[1][0]=%i\n", mas[0][0], mas[1][0]);

		f++;
		VGradient(129, tRGB(255,255,255),tRGB(0,0,0), 0,0,xmax,ymax*3/4, 2);	//Stars
		DrawRect(129, tRGB(0,0,0),0,ymax*3/4,xmax,ymax/4, 2);					//Invisible at Bottom
		VGradient(129, tRGB(0,0,32), tRGB(32,0,8),0,0,xmax,ymax, 1);			//BG Gradient
		 
		if (frame < 90*5)
		{ 
			IncludeLayer(16, 128, 0, ymax-150, 0);
		}
		if (frame < 90)
		{ 
			VGradient(128,tRGB(127,127,127), tRGB(32,32,32), 0,0,xmax,ymax, 2);
			sprintf(str2,"%s","Default");
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*2)
		{ 
			sprintf(str2,"%s","Without Darkness");
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*3)
		{ 
			sprintf(str2,"%s","Inverted");
			DrawRect(128, tRGB(0,0,0),0,0,xmax/2,ymax, 4);
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*4)
		{ 
			sprintf(str2,"%s","Addition");
			IncludeLayer(128, 129, 0, 0, 1);
		}
		else if (frame < 90*5)
		{ 
			sprintf(str2,"%s","Multipicy");
			IncludeLayer(128, 129, 0, 0, 2);
		}
		else if (frame < 90*6)
		{ 
			sprintf(str2,"%s","Fragment");
			FragmentLayer(16, 128, 0, ymax-150,0,0,xmax/2,75,0);
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*7)
		{ 
			sprintf(str2,"%s","Zoomed");
			ZoomRender(16, 128, 0, ymax-150,xmax,150,0,0,xmax/2,75,0);
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*8)
		{ 
			sprintf(str2,"%s","Grayscale(254)");
			DoGray(16,128,254,0,ymax-150);
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*9)
		{ 
			sprintf(str2,"%s","Grayscale(0)");
			DoGray(16,128,0,0,ymax-150);
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*10)
		{ 
			sprintf(str2,"%s","Custom Channels 1");
			MascedLayer(16,128,0,50,tRGB(128,0,0),tRGB(0,128,0),tRGB(0,128,128));
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*11)
		{ 
			sprintf(str2,"%s","Custom Channels 2");
			MascedLayer(16,128,0,50,tRGB(0,255,0),tRGB(0,0,255),tRGB(255,0,0));
			IncludeLayer(128, 129, 0, 0, 0);
		}
		else if (frame < 90*12)
		{ 
			sprintf(str2,"%s","Transformation");
			TransformLayer(16, 128, 0, ymax-150,0,0,320,150, 3);
			IncludeLayer(128, 129, 0, 0, 0);
		}
		if (frame == 90*12-1) frame = 0;

		sprintf(strn,"%i | %i | %i (%s)",fps,frameskip, dticks,str2);
		PrintText(129,1,strn,tRGB(255,255,255),0,0);
		//PrintText(129,1,"Demo2",tRGB(255,255,255),0,0);

		if (frame % (frameskip+1)== 0) renderer(129);
}


void Demo3()
{
if (frame %4 ==0)
{

	if (cclr[2][0] > 0 && cclr[2][1] == 255)
	{
		cclr[2][0]--;
		cclr[2][2]++;
	}
	if (cclr[2][1] > 0 && cclr[2][2] == 255)
	{
		cclr[2][1]--;
		cclr[2][0]++;
	}
	if (cclr[2][2] > 0 && cclr[2][0] == 255)
	{
		cclr[2][2]--;
		cclr[2][1]++;
	}
	o = gener(8);
}

	LP.defbrightness = 128;
	LP1.defbrightness = 128;

	LP.defcolor = tRGB(cclr[2][0],cclr[2][1],cclr[2][2]);
	LP.defincolor = tRGB(cclr[2][0]+32,cclr[2][1]+32,cclr[2][2]+32);
	LP.Spawn(250,(frame/2)%180);
	LP.Spawn(250,(frame/2)%180+180);
	LP1.defcolor = tRGB(cclr[2][2],cclr[2][0],cclr[2][1]);
	LP1.defincolor = tRGB(cclr[2][2]+32,cclr[2][0]+32,cclr[2][1]+32);
	LP1.Spawn(250,-(frame*6/10)%180);
	LP1.Spawn(250,-(frame*6/10)%180+180);

	LP.randsize = 6;
	LP1.randsize = 6;
	LP.rendercore = false;
	LP1.rendercore = false;

	DrawRect(128,ARGB(32,cclr[0][0],cclr[0][1],cclr[0][2]),0,0,xmax,ymax,2); 

	LP.RandomizeUpd(8);
	LP.Draw(128,o,-o*6);
	LP1.RandomizeUpd(8);
	LP1.Draw(128,o,-o*6);

	renderer(128);
}

void Demo4()
{
	reset(128);
	FDrawRect(128,tRGB(0,0,0),0,0,xmax,ymax);

	testing.Update();
	testing.Draw(128);

	if (frame % 600 == 0) 
	{
		if (testing.animtype == 0) testing.ChangeAnim(1,false);
		else testing.ChangeAnim(0,false);
	}

	renderer(128);
}