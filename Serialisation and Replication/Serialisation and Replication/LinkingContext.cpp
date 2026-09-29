#include "LinkingContext.h"
#include "PlayerState.h"

PlayerState* LinkingContext::GetEntity(uint32_t id) const
{
	auto it = mIDToEntity.find(id);
	return (it != mIDToEntity.end()) ? it->second.get() : nullptr;
}

PlayerState* LinkingContext::AddEntity(uint32_t id, std::unique_ptr<PlayerState> entity)
{
	PlayerState* rawPtr = entity.get();
	mIDToEntity[id] = std::move(entity);
	return rawPtr;
}

void LinkingContext::RemoveEntity(uint32_t id) { mIDToEntity.erase(id); }

size_t LinkingContext::Count() const { return mIDToEntity.size(); }