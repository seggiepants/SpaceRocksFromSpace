#include <iostream>
#include <SDL2/SDL_image.h>
#include "RendererSDL2.h"
#include "ImageSDL2.h"

namespace jam
{
    void RendererSDL2::Init(SDL_Window* window, SDL_GLContext* context)
    {
        this->window = window;
        this->context = context;
    }
    void RendererSDL2::Clear(rgb bg)
    {
        Uint8 r, g, b, a;
	glClearColor(bg.r / 255.0, bg.g / 255.0, bg.b / 255.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
    }

    void RendererSDL2::DrawImage(IImage* img, int x, int y)
    {
      GLuint texture = ((ImageSDL2*)img)->GetHandle();
      glColor3f(1.0, 1.0, 1.0); // Set color to white so it isn't tinted.
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texture);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Allow transparency;
      glEnable(GL_BLEND);
      glBegin(GL_QUADS);
      glTexCoord2f(0.0, 0.0); glVertex3i(x, y, 0);
      glTexCoord2f(1.0, 0.0); glVertex3i(x + img->GetWidth(), y, 0);
      glTexCoord2f(1.0, 1.0); glVertex3i(x + img->GetWidth(), y + img->GetHeight(), 0);
      glTexCoord2f(0.0, 1.0); glVertex3i(x, y + img->GetHeight(), 0);
      glEnd();
      glDisable(GL_TEXTURE_2D);
    }

    void RendererSDL2::DrawLine(int x1, int y1, int x2, int y2, rgb color)
    {
      glColor3f(color.r/255.0, color.g/255.0, color.b/255.0);
      glBegin(GL_LINES);
      glVertex3i(x1, y1, 0);
      glVertex3i(x2, y2, 0);
      glEnd();
    }

    void RendererSDL2::DrawPoint(int x, int y, rgb color)
    {
      glColor3f(color.r/255.0, color.g/255.0, color.b/255.0);
      glBegin(GL_POINTS);
      glVertex3i(x, y, 0);
      glEnd();
    }

    void RendererSDL2::DrawPolygon(float x, float y, std::vector<jam::Point2Df>* points, jam::rgb color)
    {
      glColor3f(color.r/255.0, color.g/255.0, color.b/255.0);
      glBegin(GL_LINE_LOOP);
      for(int i = 0; i < points->size(); i++)
	{
	  glVertex3i(points->at(i).x + x, points->at(i).y + y, 0);
	}
      glEnd();
    }

    void RendererSDL2::DrawSubImage(IImage* img, int screenX, int screenY, int x, int y, int w, int h)
    {
      GLfloat tx, ty, tw, th;

      tx = (GLfloat)x / (GLfloat)img->GetWidth();
      ty = (GLfloat)y / (GLfloat)img->GetHeight();
      tw = (GLfloat)w / (GLfloat)img->GetWidth();
      th = (GLfloat)h / (GLfloat)img->GetHeight();
      
      GLuint texture = ((ImageSDL2*)img)->GetHandle();
      glColor3f(1.0, 1.0, 1.0); // Set color to white so it isn't tinted.
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, texture);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Allow transparency;
      glEnable(GL_BLEND);
      glBegin(GL_QUADS);
      glTexCoord2f(tx, ty); glVertex3i(screenX, screenY, 0);
      glTexCoord2f(tx + tw, ty); glVertex3i(screenX + w, screenY, 0);
      glTexCoord2f(tx + tw, ty + th); glVertex3i(screenX + w, screenY + h, 0);
      glTexCoord2f(tx, ty + th); glVertex3i(screenX, screenY + h, 0);
      glEnd();
    }

    void RendererSDL2::FillRect(int x1, int y1, int x2, int y2, rgb color)
    {
      int x, y, w, h;
      x = x1 > x2 ? x2 : x1;
      y = y1 > y2 ? y2 : y1;
      w = abs(x2 - x1);
      h = abs(y2 - y1);

      glColor3f(color.r/255.0, color.g/255.0, color.b/255.0);
      glBegin(GL_QUADS);
      glVertex3i(x, y, 0);
      glVertex3i(x + w, y, 0);
      glVertex3i(x + w, y + h, 0);
      glVertex3i(x, y + h, 0);
      glEnd();
    }

    void RendererSDL2::GetScreenSize(int* width, int* height)
    {
        SDL_GetWindowSize(this->window, width, height);
    }
}
