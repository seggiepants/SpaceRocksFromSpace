#include <iostream>
#include "ImageDC.h"
#ifdef __DREAMCAST__
namespace jam
{
    ImageDC::ImageDC()
    {
        this->texture = 0;
        this->width = 0;
        this->height = 0;
    }

    ImageDC::~ImageDC()
    {
        if (this->texture != 0)
        {
            glDeleteTextures(1, &this->texture);
            this->texture = 0;
        }
    }

    void ImageDC::Construct(std::string fileName)
    {
        int width, height;
        bool hasAlpha;
        GLubyte* textureImage = NULL;

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        bool success = loadPngImage(fileName.c_str(), width, height, hasAlpha, &textureImage);
        if (!success) {
            std::cout << "Unable to load png file" << std::endl;
            return;
        }
        std::cout << "Image loaded " << width << " " << height << " alpha " << hasAlpha << std::endl;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &this->texture);
        glBindTexture(GL_TEXTURE_2D, this->texture);

        int mode = GL_RGBA;
        this->width = width;
        this->height = height;
        glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, textureImage);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    int ImageDC::GetWidth()
    {
        return this->width;
    }

    int ImageDC::GetHeight()
    {
        return this->height;
    }

    // stolen from:  https://blog.nobel-joergensen.com/2010/11/07/loading-a-png-as-texture-in-opengl-using-libpng/
    bool ImageDC::loadPngImage(const char* name, int& w, int& h, bool& alpha, GLubyte** data) 
    {
        png_structp png_ptr;
        png_infop info_ptr;
        unsigned int sig_read = 0;
        int color_type, interlace_type;
    
        FILE* fp;

        if ((fp = fopen(name, "rb")) == NULL)
            return false;

        /* Create and initialize the png_struct
         * with the desired error handler
         * functions.  If you want to use the
         * default stderr and longjump method,
         * you can supply NULL for the last
         * three parameters.  We also supply the
         * the compiler header file version, so
         * that we know if the application
         * was compiled with a compatible version
         * of the library.  REQUIRED
         */
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
            NULL, NULL, NULL);

        if (png_ptr == NULL) {
            fclose(fp);
            return false;
        }

        /* Allocate/initialize the memory
         * for image information.  REQUIRED. */
        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL) {
            fclose(fp);
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            return false;
        }

        /* Set up the output control if
         * you are using standard C streams */
        png_init_io(png_ptr, fp);

        /* If we have already
         * read some of the signature */
        png_set_sig_bytes(png_ptr, sig_read);

        /*
         * If you have enough memory to read
         * in the entire image at once, and
         * you need to specify only
         * transforms that can be controlled
         * with one of the PNG_TRANSFORM_*
         * bits (this presently excludes
         * dithering, filling, setting
         * background, and doing gamma
         * adjustment), then you can read the
         * entire image (including pixels)
         * into the info structure with this
         * call
         *
         * PNG_TRANSFORM_STRIP_16 |
         * PNG_TRANSFORM_PACKING  forces 8 bit
         * PNG_TRANSFORM_EXPAND forces to
         *  expand a palette into RGB
         */
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

        png_uint_32 width, height;
        int bit_depth;
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
            &interlace_type, NULL, NULL);
        w = width;
        h = height;

        unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        *data = (unsigned char*)malloc(row_bytes * h);

        png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

        for (int i = 0; i < h; i++) {
            // note that png is ordered top to
            // bottom, but OpenGL expect it bottom to top
            // so the order or swapped
            memcpy(*data + (row_bytes * (h - 1 - i)), row_pointers[i], row_bytes);
        }

        /* Clean up after the read,
         * and free any memory allocated */
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

        /* Close the file */
        fclose(fp);

        /* That's it */
        
        return true;
    }
}

#endif
