#include "ObjectCreationRegistry.h"

#include "ObjectCreationRegistry.h"

std::unordered_map<uint32_t, FactoryFn>& GetRegistry()
{
    static std::unordered_map<uint32_t, FactoryFn> registry;
    return registry;
}

void RegisterAllClasses()
{
    GetRegistry()[1] = []() { return std::make_unique<PlayerState>(); };
}

std::unique_ptr<PlayerState> CreateByClassId(uint32_t classId)
{
    auto& registry = GetRegistry();
    auto it = registry.find(classId);
    return (it != registry.end()) ? it->second() : nullptr;
}