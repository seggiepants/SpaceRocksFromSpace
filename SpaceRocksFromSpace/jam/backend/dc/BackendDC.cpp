#include "BackendDC.h"
#ifdef __DREAMCAST__
#include <iostream>
#include <kos.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

namespace jam
{
    BackendDC::BackendDC()
    {
        this->cont = nullptr;
        this->state = nullptr;
        this->currentScene = nullptr;
    }

    BackendDC::~BackendDC()
    {
    }

    bool BackendDC::Construct(std::string title, int screenWidth, int screenHeight)
    {
        bool success = true;
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
        glKosInit();
        // Setup to have a screen of given width and height with 0, 0 in the top left corner.
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0f, this->screenWidth, this->screenHeight, 0.0f, 0.0f, 1.0f);
        return success;
    }

    void BackendDC::Start(IScene* scene)
    {

    }

    bool BackendDC::OnUserCreate()
    {

    }

    bool BackendDC::OnUserUpdate(float dt)
    {
        // Get controller 1
        this->cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);

        // Get controller 1 button state.
        this->state = (cont_state_t*)maple_dev_status(cont);
        if (!this->state) {
            std::cout << "Error reading controller" << std::endl;
            break;
        }
    }

    IResourceManager* BackendDC::ResourceManager()
    {

    }
}
#endif