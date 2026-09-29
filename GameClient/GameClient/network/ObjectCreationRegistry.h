#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include "PlayerState.h"

using FactoryFn = std::function<std::unique_ptr<PlayerState>()>;

// Only declarations here
std::unordered_map<uint32_t, FactoryFn>& GetRegistry();
void RegisterAllClasses();
std::unique_ptr<PlayerState> CreateByClassId(uint32_t classId);