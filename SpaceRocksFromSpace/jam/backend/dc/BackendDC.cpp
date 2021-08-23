#include "BackendDC.h"
#ifdef __DREAMCAST__
#include <kos.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

namespace jam
{
    BackendDC::BackendDC()
    {

    }

    BackendDC::~BackendDC()
    {
    }

    bool BackendDC::Construct(std::string title, int screenWidth, int screenHeight)
    {
        return false;
    }

    void BackendDC::Start(IScene* scene)
    {

    }

    bool BackendDC::OnUserCreate()
    {

    }

    bool BackendDC::OnUserUpdate(float dt)
    {

    }

    IResourceManager* BackendDC::ResourceManager()
    {

    }
}
#endif