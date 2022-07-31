//
// TGA file header definition
//

#pragma once
#ifndef	__TGA_HEADER__
#define	__TGA_HEADER__

#include	"TypeDefs.h"

#pragma pack (push, 1)
                                        // TGA specific constants and structs
struct  TgaHeader
{
    unsigned char    idLength;
    unsigned char    colormapType;
    unsigned char    imageType;
    uint16  colormapIndex;
    uint16  colormapLength;
    unsigned char    colormapEntrySize;
    uint16  xOrigin;
    uint16  yOrigin;
    uint16  width;
    uint16  height;
    unsigned char    pixelSize;
    unsigned char    imageDesc;
};

#pragma pack (pop)
#endif
