#ifndef PLATFORM_H_INCLUDED
#define PLATFORM_H_INCLUDED

	#ifdef WIN32
		#define SEP "\\"
	#else //UNIX
		#define SEP "/"
	#endif
	
	#define KEY_F3 0x3C
	#define KEY_UP 0x052
	#define KEY_DOWN 0x051
	#define KEY_LEFT 0x050
	#define KEY_RIGHT 0x04F
	#define KEY_ESCAPE 0x029
	#define KEY_RETURN 0x028
	#define KEY_A 0x004
	#define KEY_D 0x007
	#define KEY_NUMPAD4 0x05C
	#define KEY_NUMPAD6 0x05E

	extern bool	fullscreen;
	extern unsigned long int frame;
	extern int fps, dticks, frameskip;
	extern int rxmax, rymax;

	bool get_key(int id);
	int get_keypressed(int id);
	void reload();
	
	void Quit(int exitcode);
	void SwapBuffers();
	void Sleep(int ms);
	
#endif //PLATFORM_H_INCLUDED
