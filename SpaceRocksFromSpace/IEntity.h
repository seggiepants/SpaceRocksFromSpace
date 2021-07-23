#pragma once
#ifndef __IENTITY_H__
#define __IENTITY_H__
#include "IRenderer.h"
#include "IScene.h"

namespace jam
{
	class IEntity
	{
	public:
		virtual void Draw(IRenderer* render) = 0;
		virtual void Update(IScene* scene, float dt) = 0;
		virtual bool IsDeleted() = 0;
		virtual void SetPosition(float x, float y) = 0;
	protected:
		float x;
		float y;
	};
}

#endif