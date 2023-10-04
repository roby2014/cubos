/// @file
/// @brief Component @ref cubos::engine::BoxCollisionShape.
/// @ingroup collisions-plugin

#pragma once

#include <cubos/core/geom/box.hpp>

namespace cubos::engine
{
    /// @brief Component which adds a box collision shape to an entity, used with a collider component.
    /// @ingroup collisions-plugin
    struct [[cubos::component("cubos/box_collision_shape", VecStorage)]] BoxCollisionShape
    {
        cubos::core::geom::Box shape; ///< Box shape.
    };
} // namespace cubos::engine