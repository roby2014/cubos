#include <cubos/core/ecs/component/registry.hpp>

using namespace cubos::core;
using namespace cubos::core::ecs;

bool Registry::create(std::string_view name, data::old::Deserializer& des, Blueprint& blueprint, Entity id)
{
    auto& creators = Registry::entriesByName();
    if (auto it = creators.find(std::string(name)); it != creators.end())
    {
        return it->second->componentCreator(des, blueprint, id);
    }

    return false;
}

std::unique_ptr<IStorage> Registry::createStorage(const reflection::Type& type)
{
    auto& creators = Registry::entriesByType();
    if (creators.contains(type))
    {
        return creators.at(type)->storageCreator();
    }

    return nullptr;
}

std::optional<std::string_view> Registry::name(const reflection::Type& type)
{
    auto& entries = Registry::entriesByType();
    if (entries.contains(type))
    {
        return entries.at(type)->name;
    }

    return std::nullopt;
}

const reflection::Type* Registry::type(std::string_view name)
{
    auto& entries = Registry::entriesByName();
    if (auto it = entries.find(std::string(name)); it != entries.end())
    {
        return it->second->type;
    }

    return nullptr;
}

memory::TypeMap<std::shared_ptr<Registry::Entry>>& Registry::entriesByType()
{
    static memory::TypeMap<std::shared_ptr<Entry>> entries;
    return entries;
}

std::unordered_map<std::string, std::shared_ptr<Registry::Entry>>& Registry::entriesByName()
{
    static std::unordered_map<std::string, std::shared_ptr<Entry>> entries;
    return entries;
}
