#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED

	#ifdef WIN32
		#define SEP "\\"
	#else //UNIX
		#define SEP "/"
	#endif
	
	#define KEY_F1 0x3A
	#define KEY_F2 0x3B
	#define KEY_F3 0x3C
	#define KEY_F4 0x3D
	#define KEY_UP 0x052
	#define KEY_DOWN 0x051
	#define KEY_LEFT 0x050
	#define KEY_RIGHT 0x04F
	#define KEY_ESCAPE 0x029
	#define KEY_RETURN 0x028
	#define KEY_SPACE 0x2C
	#define KEY_1 0x1E
	#define KEY_2 0x1F
	#define KEY_3 0x20
	#define KEY_4 0x21
	#define KEY_5 0x22
	#define KEY_A 0x04
	#define KEY_D 0x07
	#define KEY_W 26
	#define KEY_S 22
	#define KEY_NUMPAD0 0x062
	#define KEY_NUMPAD2 0x05A
	#define KEY_NUMPAD4 0x05C
	#define KEY_NUMPAD5 0x05D
	#define KEY_NUMPAD6 0x05E
	#define KEY_NUMPAD8 0x060

	extern bool	fullscreen;
	extern unsigned long int frame;
	extern int fps, dticks, frameskip, l_targetfps, l_minframeskip, l_maxframeskip;
	extern int rxmax, rymax;

	bool get_key(int id);
	int get_keypressed(int id);
	void reload();
	
	void Quit(int exitcode);
	void SwapBuffers();
	void Sleep(int ms);

	int exec();
	int process();
	
#endif //PLATFORM_H_INCLUDED
