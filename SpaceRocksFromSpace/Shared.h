#pragma once
#ifndef __SHARED_H__
#define __SHARED_H__

#include "IBackend.h"
namespace jam
{
	extern IBackend* backEnd;
	const std::string SOUND_EXPLOSION = "assets/sound/explosion.wav";
}
#endif