/// @dir
/// @brief @ref tesseratos-entity-selector-plugin plugin directory.

/// @file
/// @brief Plugin entry point.
/// @ingroup tesseratos-entity-selector-plugin

#pragma once

#include <cubos/core/ecs/entity/entity.hpp>

#include <cubos/engine/cubos.hpp>

namespace tesseratos
{
    /// @defgroup tesseratos-entity-selector-plugin Entity selector
    /// @ingroup tesseratos
    /// @brief Adds a resource used to select an entity.
    ///
    /// This plugins exists to reduce coupling between plugins. For example, a plugin which allows
    /// selecting entities through a ImGui window only needs to depend on this plugin, instead of
    /// having to know about all the plugins which care about it. The same applies in the other
    /// direction.
    ///
    /// ## Resources
    /// - @ref EntitySelector - identifies the currently selected entity.

    /// @brief Resource which identifies the currently selected entity.
    struct EntitySelector
    {
        cubos::core::ecs::Entity selection; ///< Selected entity, or `null` if none.
    };

    /// @brief Plugin entry function.
    /// @param cubos @b CUBOS. main class
    /// @ingroup tesseratos-entity-selector-plugin
    void entitySelectorPlugin(cubos::engine::Cubos& cubos);
} // namespace tesseratos
