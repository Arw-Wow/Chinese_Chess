#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <easyx.h>
#pragma comment(lib,"MSIMG32.LIB")


class Tools
{
public:
	static void putimage_alpha(int x, int y, IMAGE* img) {
		int w = img->getwidth();
		int h = img->getheight();
		AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	}
};

#endif // !__TOOLS_H__


