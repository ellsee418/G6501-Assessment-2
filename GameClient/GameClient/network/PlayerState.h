#pragma once
#include "OutputMemoryStream.h"
#include "InputMemoryStream.h"

class PlayerState
{
public:
	PlayerState() = default;
	PlayerState(float x, float y, int health);

	void Serialize(OutputMemoryStream& out) const;
	void Deserialize(InputMemoryStream& in);

	float X() const { return mX; }
	float Y() const { return mY; }
	int Health() const { return mHealth; }

private:
	float mX = 0.0f;
	float mY = 0.0f;
	int mHealth = 100;
};

