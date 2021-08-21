#include <string>
#include "KioskHelper.h"

#ifdef KIOSK_MODE
namespace game
{
    KioskHelper* KioskHelper::instance = nullptr;

    KioskHelper::KioskHelper()
    {
        this->high_score_json =
            "{\
        \"scores\": [\
            {\
                \"gameTime\": 72.89481353759766,\
                    \"initials\" : \"SEG\",\
                    \"level\" : 5,\
                    \"score\" : 1260\
            },\
            {\
                \"gameTime\": 49.02137756347656,\
                \"initials\" : \"SEG\",\
                \"level\" : 4,\
                \"score\" : 1030\
            },\
            {\
                \"gameTime\": 40.17289352416992,\
                \"initials\" : \"SEG\",\
                \"level\" : 4,\
                \"score\" : 720\
            }\
        ]\
    }";
    }

    std::string KioskHelper::GetHighScores()
    {
        return this->high_score_json;
    }

    void KioskHelper::SetHighScores(std::string value)
    {
        this->high_score_json = value;
    }
}
#endif
