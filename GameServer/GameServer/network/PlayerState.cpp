#include "PlayerState.h"

PlayerState::PlayerState(float x, float y, int health) : mX(x), mY(y), mHealth(health) {}

void PlayerState::Serialize(OutputMemoryStream& out) const
{
	out.Write(mX);
	out.Write(mY);
	out.Write(mHealth);
}

void PlayerState::Deserialize(InputMemoryStream& in)
{
	in.Read(mX);
	in.Read(mY);
	in.Read(mHealth);
}
