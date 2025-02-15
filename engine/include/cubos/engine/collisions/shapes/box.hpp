/// @file
/// @brief Component @ref cubos::engine::BoxCollisionShape.
/// @ingroup collisions-plugin

#pragma once

#include <cubos/core/geom/box.hpp>
#include <cubos/core/reflection/reflect.hpp>

namespace cubos::engine
{
    /// @brief Component which adds a box collision shape to an entity, used with a @ref Collider component.
    /// @ingroup collisions-plugin
    struct [[cubos::component("cubos/box_collision_shape", VecStorage)]] BoxCollisionShape
    {
        CUBOS_REFLECT;

        cubos::core::geom::Box box; ///< Box shape.
    };
} // namespace cubos::engine
