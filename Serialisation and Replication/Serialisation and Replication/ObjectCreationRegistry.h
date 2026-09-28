#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include "PlayerState.h"


using FactoryFn = std::function<std::unique_ptr<PlayerState>()>;

std::unordered_map<uint32_t, FactoryFn>& GetRegistry() 
{
	static std::unordered_map<uint32_t, FactoryFn> registry;
	return registry;
}

void RegisterAllClasses() 
{
	GetRegistry()[1] = []() { return std::make_unique<PlayerState>(); };
	// a second class would just add one more line here
}

std::unique_ptr<PlayerState> CreateByClassId(uint32_t classId) 
{
	auto& registry = GetRegistry();
	auto it = registry.find(classId);
	return (it != registry.end()) ? it->second() : nullptr;
}

