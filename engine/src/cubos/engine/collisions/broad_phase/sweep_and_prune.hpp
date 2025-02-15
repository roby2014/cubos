/// @file
/// @brief Resource @ref cubos::engine::BroadPhaseSweepAndPrune.
/// @ingroup broad-phase-collisions-plugin

#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <cubos/core/ecs/entity/hash.hpp>
#include <cubos/core/ecs/entity/manager.hpp>

namespace cubos::engine
{
    /// @brief Resource which stores sweep and prune data.
    /// @ingroup broad-phase-collisions-plugin
    struct BroadPhaseSweepAndPrune
    {
        /// @brief Marker used for sweep and prune.
        struct SweepMarker
        {
            core::ecs::Entity entity; ///< Entity referenced by the marker.
            bool isMin;               ///< Whether the marker is a min or max marker.
        };

        /// @brief List of ordered sweep markers for each axis. Stores the index of the marker in mMarkers.
        std::vector<SweepMarker> markersPerAxis[3];

        /// @brief Maps of overlapping entities for each axis calculated by sweep and prune.
        ///
        /// For each each map, the key is an entity and the value is a list of entities that
        /// overlap with the key. Symmetrical pairs are not stored.
        std::unordered_map<core::ecs::Entity, std::vector<core::ecs::Entity>, core::ecs::EntityHash>
            sweepOverlapMaps[3];

        /// @brief Set of active entities during sweep for each axis.
        std::unordered_set<core::ecs::Entity, core::ecs::EntityHash> activePerAxis[3];

        /// @brief Adds an entity to the list of entities tracked by sweep and prune.
        /// @param entity Entity to add.
        void addEntity(core::ecs::Entity entity);

        /// @brief Removes an entity from the list of entities tracked by sweep and prune.
        /// @param entity Entity to remove.
        void removeEntity(core::ecs::Entity entity);

        /// @brief Clears the list of entities tracked by sweep and prune.
        void clearEntities();
    };
} // namespace cubos::engine
