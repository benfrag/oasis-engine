#ifndef SYSTEM_H
#define SYSTEM_H

#include "engine/core/ecs/component.h"
#include "engine/core/ecs/interface.h"
#include <set>

class System
{
public:
//    explicit System(IECSController* _ecs) : ecs(_ecs) {}
    // Add an entity to the system
    void add_entity(Entity entity) {
        entities.insert(entity);
    }

    // Remove an entity from the system
    void remove_entity(Entity entity) {
        entities.erase(entity);
    }

    // Called when an entity is destroyed
    void on_entity_destroyed(Entity entity) {
        remove_entity(entity);
    }

    // Update function to be overridden by derived systems
    virtual void update(void* engine_core, float dt) = 0;

    // Set of entities this system is interested in
    std::set<Entity> entities;

 //   IECSController* ecs;
};

class SystemManager
{
public:
//    IECSController* ecs;


    template<typename T>
    std::shared_ptr<T> register_system() {
        const char* typeName = typeid(T).name();
        //assert(systems.find(typeName) == systems.end() && "Registering system more than once.");

        // Create a new system and return its pointer
        auto system = std::make_shared<T>();
        systems.insert({typeName, system});
        return system;
    }

    template<typename T>
    void set_signature(Signature signature) {
        const char* typeName = typeid(T).name();

        //assert(systems.find(typeName) != systems.end() && "System used before registered.");
        signatures.insert({typeName, signature});
    }

    void entity_destroyed(Entity entity) {
        // Notify each system that an entity was destroyed
        // If the system is tracking this entity, it will remove it
        for (auto const& pair : systems) {
            auto const& system = pair.second;
            system->on_entity_destroyed(entity);
        }
    }

    void entity_signature_changed(Entity entity, Signature entitySignature) {
        // Notify each system that an entity's signature changed
        for (auto const& pair : systems) {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = signatures[type];

            // Entity signature matches system signature - add to system's queue
            if ((entitySignature & systemSignature) == systemSignature) {
                system->entities.insert(entity);
            } else {
                system->entities.erase(entity);
            }
        }
    }

    void update_all(void* engine_core, float dt)
    {
        for (auto const& pair : systems) {
            auto const& system = pair.second;
            system->update(engine_core, dt);
        }
    }

private:
    // Map from system type string pointer to a signature
    std::unordered_map<const char*, Signature> signatures;

    // Map from system type string pointer to a system pointer
    std::unordered_map<const char*, std::shared_ptr<System>> systems;

};

#endif
