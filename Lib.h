#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

	extern const int xmax, ymax;

	void buildLayer(int id,const int x,const int y);
	void delLayer(int id);
	void destroy();
	unsigned int ARGB(unsigned int a,unsigned int r,unsigned int g,unsigned int b);
	unsigned int tRGB(unsigned int r,unsigned int g,unsigned int b);
	void RGBt(unsigned int color, unsigned int *r,unsigned int *g,unsigned int *b);
	void ARGBt(unsigned int color, unsigned int *a, unsigned int *r,unsigned int *g,unsigned int *b);
	void RGBt(unsigned int color, int *r, int *g, int *b);
	void ARGBt(unsigned int color, int *a, int *r, int *g, int *b);
	int getHeight(int id);
	int getWidth(int id);
	void reset(int id);
	void invert(int id, int x, int y);
	void add(int id, int x,int y,int r,int g,int b);
	void add(int id, int x,int y,int a,int r,int g,int b);
	void mult(int id, int x,int y,int r,int g,int b);
	void mult(int id, int x,int y,int a,int r,int g,int b);
	void screening(int id, int x,int y,int r,int g,int b);
	void plot(int id, int x,int y,unsigned int color);
	void plot(int id, int x,int y,int r,int g,int b);
	void plot(int id, int x,int y, int a, int r,int g,int b);
	void transparency(int id,int x,int y, int a);
	void grayer(int id, int tid, int x,int y,int x1,int y1,int a);
	void DrawVLine(int id, unsigned int color, int x1, int y1, int k);
	void FDrawVLine(int id, unsigned int color, int x1, int y1, int k);
	void DrawHLine(int id, unsigned int color, int x1, int y1, int k);
	void FDrawHLine(int id, unsigned int color, int x1, int y1, int k);
	void DrawFrame(int id, unsigned int color, int x, int y, int xk, int yk);
	void FDrawRect(int id, unsigned int color, int x1, int y1, int xk, int yk);
	void DrawRect(int id, unsigned int color, int x1, int y1, int xk, int yk);
	void DrawRect(int id, unsigned int color, int x1, int y1, int xk, int yk, int mode);
	void DoGray(int id, int tid, unsigned int value, int x1, int y1);
	void VGradient(int id, unsigned int colorA, unsigned int colorB, int x1, int y1, int xk, int yk, int mode);
	void HGradient(int id, unsigned int colorA, unsigned int colorB, int x1, int y1, int xk, int yk, int mode);
	void IncludeLayer(int id, int tid, int x1, int y1, int mode);
	void FragmentLayer(int id, int tid, int tx, int ty, int x2, int y2, int xk, int yk, int mode);
	void TransformLayer(int id, int tid, int tx, int ty, int x2, int y2, int xk, int yk, int mode);
	void TransformLayer(int id, int tid, int tx, int ty, int mode);
	void PrintText(int id, int fontn, char *str, int color, int x, int y);
	int TextWidth(int fontn, char *str);
	int TextHeight(int fontn, char *str);
	void PrintNum(int id, int fontn, int subj, int color, int x, int y, int k);
	void MascedLayer(int id, int tid, int tx, int ty, int ColorR, int ColorG, int ColorB);
	void ZoomRender(int id, int tid, int tx, int ty, int x1, int y1, int cx, int cy, int x2, int y2, int mode);
	int gener(int max);
	void renderer(int id);
	bool tex_read(int fname);
	bool fnt_read(int fname);
	void tex_init();
	void font_init();
	void BWNoise(int id, int x1, int y1, int xk, int yk);
	
#endif //LIB_H_INCLUDED
