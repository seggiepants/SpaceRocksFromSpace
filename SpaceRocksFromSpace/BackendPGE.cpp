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
        this->oldMousePos.x = this->oldMousePos.y = 0;
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
        olc::HWButton btnState;
        for (int i = 0; i < MAX_KEYS; i++)
        {
            this->oldKey[i] = this->Key[i];
            btnState = this->GetKey((olc::Key)i);
            if (this->currentScene != nullptr)
            {
                if (btnState.bReleased)
                {
                    this->currentScene->KeyUp(i);
                }
                if (btnState.bPressed)
                {
                    this->currentScene->KeyDown(i);
                }
            }
            this->Key[i] = btnState.bHeld;
        }

        olc::HWButton mouseLeft = this->GetMouse(0);
        olc::HWButton mouseRight = this->GetMouse(1);
        olc::vi2d mousePos = this->GetMousePos();
        if (this->oldMousePos.x != mousePos.x || this->oldMousePos.y != mousePos.y)
        {
            this->oldMousePos.x = mousePos.x;
            this->oldMousePos.y = mousePos.y;
            if (this->currentScene != nullptr)
            {
                this->currentScene->MouseMove(mousePos.x, mousePos.y);
            }
        }

        if (mouseLeft.bReleased && this->currentScene != nullptr)
        {
            this->currentScene->MouseClick(jam::MouseButton::LEFT, mousePos.x, mousePos.y);
        }

        if (mouseRight.bReleased && this->currentScene != nullptr)
        {
            this->currentScene->MouseClick(jam::MouseButton::RIGHT, mousePos.x, mousePos.y);
        }

        if (this->currentScene != nullptr)
        {
            this->currentScene->Update(dt);
            if (this->render != nullptr)
            {
                this->currentScene->Draw((jam::IRenderer*)this->render);
            }
        }
        this->currentScene = this->currentScene->NextScene();
        return this->currentScene != nullptr;
    }
}