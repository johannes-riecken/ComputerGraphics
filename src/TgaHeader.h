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
    std::byte    idLength;
    std::byte    colormapType;
    std::byte    imageType;
    uint16  colormapIndex;
    uint16  colormapLength;
    std::byte    colormapEntrySize;
    uint16  xOrigin;
    uint16  yOrigin;
    uint16  width;
    uint16  height;
    std::byte    pixelSize;
    std::byte    imageDesc;
};

#pragma pack (pop)
#endif
