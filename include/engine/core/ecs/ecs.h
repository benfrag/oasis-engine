#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <string>
#include "engine/core/ecs/system.h"

/*
namespace {
    std::atomic_uint componentTypeIDCounter{0};

    template<typename T>
    uint32_t get_component_type_id() {
        static uint32_t id = componentTypeIDCounter++;
        return id;
    }
}
*/

class ECSController : public IECSController
{
public:
    void* engine_core;
    EntityManager entity_manager;
    ComponentManager component_manager;
    SystemManager system_manager;

//    ECSController() {system_manager.ecs = (IECSController*)(this);}

    Entity create_entity()
    {
        return entity_manager.create_entity();
    }

    void destroy_entity(Entity entity)
    {
        entity_manager.destroy_entity(entity);
        //tell component manager to delete all of entity's components
        component_manager.entity_destroyed(entity);
    }

    template<typename T>
    void add_component(Entity entity, T component)
    {
        component_manager.add_component(entity, std::move(component));

        auto component_id = get_component_type_id<T>();
        auto signature = entity_manager.get_signature(entity);
        signature.set(component_id);
        entity_manager.set_signature(entity, signature);
        system_manager.entity_signature_changed(entity, signature);
        

        std::cout << "component id : " << component_id;
        std::cout << "added component new signature " << signature.to_string() <<  std::endl;
    }

    template<typename T>
    void remove_component(Entity entity)
    {
        component_manager.remove_component<T>(entity);

        auto component_id = get_component_type_id<T>();
        auto signature = entity_manager.get_signature(entity);
        signature.reset(component_id);
        entity_manager.set_signature(entity, signature);
        system_manager.entity_signature_changed(entity, signature);


        std::cout << "component id : " << component_id;
        std::cout << "removed component new signature " << signature.to_string() <<  std::endl;

    }

    template<typename T>
    T* get_component(Entity entity)
    {
        return component_manager.get_component<T>(entity);
    }

    template<typename T>
    std::shared_ptr<T> register_system()
    {
        //if activeentities != 0 then we need to inform system about all entities
        //as system has been created after creating entities.
        return system_manager.register_system<T>();
    }

    template<typename T>
    void set_system_signature(Signature signature)
    {
        system_manager.set_signature<T>(signature);
    }

    void update_systems(float dt)
    {
        system_manager.update_all(engine_core, dt);
    }

    template<typename T>
    uint32_t get_component_type_id() {
        static uint32_t id = componentTypeIDCounter++;
        return id;
    }

private:
    std::atomic_uint componentTypeIDCounter{0};
};

#endif
