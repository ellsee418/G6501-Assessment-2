#pragma once
#include <unordered_map>
#include <memory>
class PlayerState;

class LinkingContext
{
public:
	PlayerState* GetEntity(uint32_t id) const;
	PlayerState* AddEntity(uint32_t id, std::unique_ptr<PlayerState> entity);
	void RemoveEntity(uint32_t id);
	size_t Count() const;
private:
	std::unordered_map<uint32_t, std::unique_ptr<PlayerState>> mIDToEntity;
};

