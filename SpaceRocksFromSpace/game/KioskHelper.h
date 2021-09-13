#pragma once
#ifndef __KIOSK_HELPER__
#define __KIOSK_HELPER__

#include <string>

#ifdef KIOSK_MODE
namespace game
{
	class KioskHelper
	{
	public:
		std::string GetHighScores();
		void SetHighScores(std::string value);
		static KioskHelper* Instance() {
			if (!instance) {
				instance = new KioskHelper();
			}
			return instance;
		}
	protected:
		KioskHelper();
		static KioskHelper* instance;
		std::string high_score_json;
	};
}
#endif

#endif