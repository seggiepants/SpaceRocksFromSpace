#define OLC_PGE_APPLICATION
#include "BackendPGE.h"
#include "RendererPGE.h"
#include "KeyCodesPGE.h"

namespace jam
{
    BackendPGE::BackendPGE() : IBackend(), olc::PixelGameEngine()
    {
        this->render = nullptr;
        key::KeyInit_PGE();
    }

    BackendPGE::~BackendPGE()
    {
        if (this->render != nullptr)
        {
            delete this->render;
            this->render = nullptr;
        }
    }

    bool BackendPGE::Construct(std::string title, int screenWidth, int screenHeight)
    {
        olc::rcode ret;
        ret = ((olc::PixelGameEngine*)this)->Construct(screenWidth, screenHeight, 1, 1);
        if (ret == olc::rcode::OK)
        {
            this->sAppName = title;
        }
        return ret == olc::rcode::OK;
    }

    bool BackendPGE::IsKeyDown(uint8_t code)
    {
        return this->Key[code] && !this->oldKey[code];
    }

    bool BackendPGE::IsKeyPressed(uint8_t code)
    {
        return this->Key[code];
    }

    bool BackendPGE::IsKeyReleased(uint8_t code)
    {
        return !this->Key[code] && this->oldKey[code];
    }
    void BackendPGE::Start(IScene* scene)
    {
        if (this->render != nullptr)
        {
            delete this->render;
            this->render = nullptr;
        }
        RendererPGE* pgeRender = new RendererPGE();
        pgeRender->Init(this);
        this->render = (IRenderer*)pgeRender;

        this->currentScene = scene;
        int screenWidth, screenHeight;
        this->render->GetScreenSize(&screenWidth, &screenHeight);
        this->currentScene->Construct(screenWidth, screenHeight);
        ((olc::PixelGameEngine*)this)->Start();
    }

    bool BackendPGE::OnUserCreate() 
    {
        return true;
    }

    bool BackendPGE::OnUserUpdate(float dt) 
    {
        if (this->currentScene != nullptr)
        {
            this->currentScene->Update(dt);
            if (this->render != nullptr)
            {
                this->currentScene->Draw((jam::IRenderer*)this->render);
            }
        }
        return this->currentScene != nullptr;
    }
}