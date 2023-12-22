#ifndef ENTITY_H
#define ENTITY_H

#include <stdexcept>
#include <windows.h>
#include <cstdint>
#include <queue>
#include <bitset>
#include <array>

const int MAX_COMPONENTS = 32;
using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 10000;
using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager {
public:
    EntityManager() {
        // Initialize the queue with all possible entity IDs
        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
            availableEntities.push(entity);
        }
    }

    Entity create_entity() {
        //assert(livingEntityCount < MAX_ENTITIES, "Too many entities in existence.");
        if (livingEntityCount >= MAX_ENTITIES)
            throw std::runtime_error("Too many entities in existence.");

        Entity id = availableEntities.front();
        availableEntities.pop();
        ++livingEntityCount;

        return id;
    }

    void destroy_entity(Entity entity) {
//        assert(entity < MAX_ENTITIES, "Entity out of range.");
        if (entity >= MAX_ENTITIES)
            throw std::runtime_error("Entity out of range.");

        signatures[entity].reset(); // Clear the signature
        availableEntities.push(entity); // Recycle the entity ID
        --livingEntityCount;
    }

    void set_signature(Entity entity, Signature signature) {
//        assert(entity < MAX_ENTITIES, "Entity out of range.");
        if (entity >= MAX_ENTITIES)
            throw std::runtime_error("Entity out of range.");

        signatures[entity] = signature;
    }

    Signature get_signature(Entity entity) {
//        assert(entity < MAX_ENTITIES, "Entity out of range.");
        if (entity >= MAX_ENTITIES)
            throw std::runtime_error("Entity out of range.");

        return signatures[entity];

    }
private:
    std::queue<Entity> availableEntities; // Pool of reusable entity IDs
    std::array<Signature, MAX_ENTITIES> signatures; // Array of signatures
    uint32_t livingEntityCount = 0; // Total number of active entities

};


#endif
