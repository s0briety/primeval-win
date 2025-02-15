#include "../features.h"

bool EntityHandler::process() {
    std::vector<globals::EntityData> newStorage;

    for (int i = 1; i <= interfaces::m_entity_list->get_max_entities(); ++i) {
        i_client_entity* entity = interfaces::m_entity_list->get_client_entity(i);
        if (!entity)
            continue;

        c_base_entity* baseEntity = entity->get_base_entity();
        if (!baseEntity)
            continue;

        globals::EntityData data{};
        data.entity = entity;
        data.type = baseEntity->is_player() ? globals::EntityType::Player : globals::EntityType::NonPlayer;

        newStorage.push_back(data);
    }

    EntityStorage = std::move(newStorage);

    return true;
}
