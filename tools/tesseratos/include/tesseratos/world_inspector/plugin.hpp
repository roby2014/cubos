/// @dir
/// @brief @ref tesseratos-world-inspector-plugin plugin directory.

/// @file
/// @brief Plugin entry point.
/// @ingroup tesseratos-world-inspector-plugin

#pragma once

#include <cubos/engine/cubos.hpp>

namespace tesseratos
{
    /// @defgroup tesseratos-world-inspector-plugin World inspector
    /// @ingroup tesseratos
    /// @brief Shows all of the entities in the world through a ImGui window, and allows selecting
    /// them.
    ///
    /// @note Selected entities are identified by the @ref EntitySelector resource.
    ///
    /// ## Dependencies
    /// - @ref imgui-plugin
    /// - @ref tesseratos-entity-selector-plugin

    /// @brief Plugin entry function.
    /// @param cubos @b CUBOS. main class
    /// @ingroup tesseratos-world-inspector-plugin
    void worldInspectorPlugin(cubos::engine::Cubos& cubos);
} // namespace tesseratos
