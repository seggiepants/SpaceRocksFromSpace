#pragma once
#ifndef __GAME_ASSETS_H__
#define __GAME_ASSETS_H__
#include <string>
namespace game
{
	const std::string IMG_PLASMA_BG = "assets/image/plasma_bg.png";
	const std::string SOUND_EXPLOSION = "assets/sound/explosion.wav";
	const std::string SOUND_GAMEOVER = "assets/sound/game_over.wav";
	const std::string SOUND_MAINMENU = "assets/sound/main_menu.wav";
	const std::string SOUND_NEXTLEVEL = "assets/sound/next_level.wav";
	const std::string SOUND_PAUSE = "assets/sound/pause.wav";
	const std::string SOUND_SELECT = "assets/sound/select.wav";
	const std::string SOUND_TELEPORT =  "assets/sound/teleport.wav";
	const std::string SOUND_UFO = "assets/sound/ufo.wav";
	const std::string SOUND_ZAP = "assets/sound/laser.wav";

	const int MAX_SCORES = 10;
	const std::string HIGHSCORE_FILENAME = "highscore.json";

}
#endif
