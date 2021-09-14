#pragma once
#ifndef __IMAGE_DC_H__
#define __IMAGE_DC_H__

#include <string>
#include "../../IImage.h"

#ifdef __DREAMCAST__

#include <kos.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <png/png.h>

namespace jam
{
    class ImageDC :
        public IImage
    {
    public:
        ImageDC();
        ~ImageDC() override;
        void Construct(std::string fileName);
        int GetWidth() override;
        int GetHeight() override;
    protected:
        int width;
        int height;
        bool loadPngImage(const char* name, int& w, int& h, bool& alpha, GLubyte** data);
        GLuint texture;
    };
}
#endif
#endif

