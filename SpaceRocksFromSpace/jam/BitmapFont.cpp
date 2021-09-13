#include "../3rdParty/json-c/json.h"
#include "../jam/Configuration.h"
#include "../jam/BitmapFont.h"
#include "../jam/Shared.h"

namespace jam
{
    BitmapFont::BitmapFont()
    {
        this->runes.clear();
        this->texture = nullptr;
    }

    BitmapFont::~BitmapFont()
    {
        this->runes.clear();
        this->texture = nullptr;
    }

    void BitmapFont::Construct(std::string filePath)
    {
        std::string texturePath = "";
        this->runes.clear();
        json_object* ret = jam::Configuration::LoadJsonFile(filePath);
        json_object* image = json_object_object_get(ret, "image");

        texturePath = "assets/image/" + std::string(json_object_get_string(image)); 
        if (!jam::backEnd->ResourceManager()->HasImage(texturePath))
        {
            jam::backEnd->ResourceManager()->PreloadImage(texturePath);
        }
        this->texture = jam::backEnd->ResourceManager()->GetImage(texturePath);
        
        array_list* runes = json_object_get_array(json_object_object_get(ret, "runes"));
        for(size_t i = 0; i < array_list_length(runes); i++)
        {
            json_object* item = (json_object*) array_list_get_idx(runes, i);
            int x, y, w, h;
            char rune;
            rune = json_object_get_string(json_object_object_get(item, "rune"))[0];
            x = json_object_get_int(json_object_object_get(item, "x"));
            y = json_object_get_int(json_object_object_get(item, "y"));
            w = json_object_get_int(json_object_object_get(item, "w"));
            h = json_object_get_int(json_object_object_get(item, "h"));
            this->runes[rune] = Rect{ x, y, w, h };
        }
    }

    void BitmapFont::DrawText(jam::IRenderer* render, std::string message, int x, int y, jam::rgb color)
    {
        int currentX = x;
        int lineW, lineH;
        this->MeasureText(message, &lineW, &lineH);
        for (char c : message)
        {
            std::unordered_map<char, jam::Rect>::const_iterator search = this->runes.find(c);
            if (search != this->runes.end())
            {
                render->DrawSubImage(this->texture, currentX, y - lineH, (*search).second.x, (*search).second.y, (*search).second.w, (*search).second.h);
                currentX += (*search).second.w;
            }
        }
    }

    void BitmapFont::MeasureText(std::string message, int* width, int* height)
    {
        int totalWidth = 0;
        int maxHeight = 0;
        for (char c : message)
        {
            std::unordered_map<char, jam::Rect>::const_iterator search = this->runes.find(c);
            if (search != this->runes.end())
            {
                totalWidth += (*search).second.w;
                if (maxHeight < (*search).second.h)
                    maxHeight = (*search).second.h;
            }
        }
        *width = totalWidth;
        *height = maxHeight;
    }

}