#pragma once
#ifndef __IMAGE_DC_H__
#define __IMAGE_DC_H__
#include "../../IImage.h"

#ifdef __DREAMCAST__
namespace jam
{
    class ImageDC :
        public IImage
    {
    public:
        ImageDC();
        ~ImageDC() override;
        virtual int GetWidth();
        virtual int GetHeight();
    protected:
    };
}
#endif
#endif

