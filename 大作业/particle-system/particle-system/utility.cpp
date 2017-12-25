#include "utility.h"
#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>              // 标准输入输出头文件  
#include <olectl.h>              // OLE控制库头文件  
#include <math.h>              // 数学函数头文件  

GLuint LoadTextures3(const char *filename) {
	unsigned int	id = 0;
	unsigned char	*texels = 0;
	int				width, height;
	int				success;

	_AUX_RGBImageRec* Image = NULL;
	if (strstr(filename, ".bmp") || strstr(filename, ".BMP"))
	{
		
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		Image = auxDIBImageLoadA((const char*)filename);
		int Width = Image->sizeX;
		int Height = Image->sizeY;
		gluBuild2DMipmaps(GL_TEXTURE_2D,
			3,
			Image->sizeX,
			Image->sizeY,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			Image->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		delete Image;
		return id;
	}
}

bool BuildTexture(char *szPathName, GLuint &texid, BYTE r, BYTE g, BYTE b)      // 载入图片并转换为纹理  
{
	HDC hdcTemp;     // The DC to hold our bitmap
	HBITMAP hbmpTemp;    // Holds the bitmap temporarily
	IPicture * pPicture;   // IPicture interface
	OLECHAR wszPath[MAX_PATH]; // Full path to picture (WCHAR)
	char szPath[MAX_PATH];   // Full path to picture
	long lWidth;     // Width in logical units
	long lHeight;     // Height in logical units
	long lWidthPixels;    // Width in pixels
	long lHeightPixels;    // Height in pixels
	GLint glMaxTexDim;    // Holds maximum texture size (保存纹理的最大尺寸)

	if (strstr(szPathName, "http://"))
		strcpy(szPath, szPathName);
	else
	{
		GetCurrentDirectory(MAX_PATH, szPath);
		strcat(szPath, "\\");
		strcat(szPath, szPathName);
	}
	MultiByteToWideChar(CP_ACP, 0, szPath, -1, wszPath, MAX_PATH); // Convert from ASCII to Unicode

	HRESULT hr = OleLoadPicturePath(wszPath, 0, 0, 0, IID_IPicture, (void **)&pPicture);

	if (FAILED(hr))
		return FALSE;

	hdcTemp = CreateCompatibleDC(GetDC(0));    // Create the windows compatible device context
	if (!hdcTemp)
	{
		pPicture->Release();       // Decrements IPicture reference count
		return FALSE;
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);// Get maximum texture size supported

	pPicture->get_Width(&lWidth);      // Get IPicture width (convert to pixels)
	lWidthPixels = MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
	pPicture->get_Height(&lHeight);      // Get IPicture height (convert to pixels)
	lHeightPixels = MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

	// Resize image to closest 2^x
	if (lWidthPixels <= glMaxTexDim) // Is image width less than or equal to cards limit
		lWidthPixels = 1 << (int)floor((log((double)lWidthPixels) / log(2.0f)) + 0.5f);
	else // Otherwise set width to "max 2^x" that the card can handle
		lWidthPixels = glMaxTexDim;

	if (lHeightPixels <= glMaxTexDim) // Is image height greater than cards limit
		lHeightPixels = 1 << (int)floor((log((double)lHeightPixels) / log(2.0f)) + 0.5f);
	else // Otherwise set height to "max 2^x" that the card can handle
		lHeightPixels = glMaxTexDim;

	// Create a temporary bitmap
	BITMAPINFO bi = { 0 }; // The type of bitmap we request
	DWORD * pBits = 0; // Pointer to the bitmap bits

	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); // Set structure size
	bi.bmiHeader.biBitCount = 32;       // 32 bit
	bi.bmiHeader.biWidth = lWidthPixels;     // Power of two width
	bi.bmiHeader.biHeight = lHeightPixels;    // Make image top up (positive Y-Axis)
	bi.bmiHeader.biCompression = BI_RGB;     // RGB encoding
	bi.bmiHeader.biPlanes = 1;       // 1 bitplane

									 // Creating a bitmap this way allows us to specify color depth and gives us imediate access to the bits
	hbmpTemp = CreateDIBSection(hdcTemp, &bi, DIB_RGB_COLORS, (void **)&pBits, 0, 0);

	if (!hbmpTemp)
	{
		DeleteDC(hdcTemp);
		pPicture->Release();
		return FALSE;
	}

	SelectObject(hdcTemp, hbmpTemp); // Select handle to our temp DC and our temp bitmap object

									 // Render the IPicture on to the bitmap
	pPicture->Render(hdcTemp, 0, 0, lWidthPixels, lHeightPixels, 0, lHeight, lWidth, -lHeight, 0);

	// Convert from BGR to RGB format and add an alpha value
	for (long i = 0; i<lWidthPixels*lHeightPixels; ++i)
	{
		BYTE * pPixel = (BYTE*)(&pBits[i]);
		BYTE temp = pPixel[0];
		pPixel[0] = pPixel[2];
		pPixel[2] = temp;

		if ((pPixel[0] == r) && (pPixel[1] == g) && (pPixel[2] == b)) // RGB(r,g,b) 设置为全透明
			pPixel[3] = 0;
		else
			pPixel[3] = 255;
	}
	glGenTextures(1, &texid); //Create the texture

							  // Typical texture generation using data from the bitmap
	glBindTexture(GL_TEXTURE_2D, texid);   // Bind to the texture ID

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, lWidthPixels, lHeightPixels, GL_RGBA, GL_UNSIGNED_BYTE, pBits);
	//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,lWidthPixels,lHeightPixels,0,GL_RGBA,GL_UNSIGNED_BYTE,pBits);

	DeleteObject(hbmpTemp);
	DeleteDC(hdcTemp);

	pPicture->Release();

	return TRUE;
}