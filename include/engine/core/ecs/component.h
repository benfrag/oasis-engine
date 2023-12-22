#ifndef COMPONENT_H
#define COMPONENT_H

#include "engine/core/ecs/entity.h"


#include <unordered_map>
#include <memory>
#include <typeindex>
#include <type_traits>


class ComponentManager {
public:
    template<typename T>
    void add_component(Entity entity, T component) {
        auto& componentArray = get_component_array<T>();
        componentArray[entity] = std::move(component);
    }

    template<typename T>
    void remove_component(Entity entity) {
        auto& componentArray = get_component_array<T>();
        auto it = componentArray.find(entity);
        if (it != componentArray.end()) {
            componentArray.erase(it);
        }
    }

    template<typename T>
    T* get_component(Entity entity) {
        auto& componentArray = get_component_array<T>();
        auto it = componentArray.find(entity);
        if (it != componentArray.end()) {
            return &it->second;
        }
        return nullptr;
    }

    void entity_destroyed(Entity entity) {
        for (auto& [typeIndex, componentArray] : componentArrays) {
            componentArray->entityDestroyed(entity);
        }
    }

private:
    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void entityDestroyed(Entity entity) = 0;
    };

    template<typename T>
    class ComponentArray : public IComponentArray {
    public:
        void entityDestroyed(Entity entity) override {
            auto it = components.find(entity);
            if (it != components.end()) {
                components.erase(it);
            }
        }
        std::unordered_map<Entity, T> components;
    };

    template<typename T>
    std::unordered_map<Entity, T>& get_component_array() {
        std::type_index typeIndex = std::type_index(typeid(T));
        if (componentArrays.find(typeIndex) == componentArrays.end()) {
            auto newComponentArray = std::make_unique<ComponentArray<T>>();
            componentArrays[typeIndex] = std::move(newComponentArray);
        }
        return static_cast<ComponentArray<T>*>(componentArrays[typeIndex].get())->components;
    }

    std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> componentArrays;
};

#endif
