#include <glm/glm.hpp>

#include <cubos/core/ecs/system/query.hpp>

#include <cubos/engine/physics/plugin.hpp>
#include <cubos/engine/settings/plugin.hpp>
#include <cubos/engine/transform/plugin.hpp>

#include "physics_accumulator.hpp"

using cubos::core::ecs::Query;
using cubos::core::ecs::Read;
using cubos::core::ecs::Write;

using namespace cubos::engine;

CUBOS_REFLECT_IMPL(AccumulatedCorrection)
{
    return cubos::core::ecs::ComponentTypeBuilder<AccumulatedCorrection>("cubos::engine::AccumulatedCorrection")
        .withField("vec", &AccumulatedCorrection::vec)
        .build();
}

CUBOS_REFLECT_IMPL(PhysicsMass)
{
    return cubos::core::ecs::ComponentTypeBuilder<PhysicsMass>("cubos::engine::PhysicsMass")
        .withField("mass", &PhysicsMass::mass)
        .withField("inverseMass", &PhysicsMass::inverseMass)
        .build();
}

CUBOS_REFLECT_IMPL(PhysicsVelocity)
{
    return cubos::core::ecs::ComponentTypeBuilder<PhysicsVelocity>("cubos::engine::PhysicsVelocity")
        .withField("velocity", &PhysicsVelocity::velocity)
        .withField("force", &PhysicsVelocity::force)
        .withField("impulse", &PhysicsVelocity::impulse)
        .build();
}

CUBOS_REFLECT_IMPL(PreviousPosition)
{
    return cubos::core::ecs::ComponentTypeBuilder<PreviousPosition>("cubos::engine::PreviousPosition")
        .withField("vec", &PreviousPosition::vec)
        .build();
}

static void increaseAccumulator(Write<PhysicsAccumulator> accumulator, Read<DeltaTime> deltaTime)
{
    accumulator->value += deltaTime->value;
}

static void decreaseAccumulator(Write<PhysicsAccumulator> accumulator, Read<FixedDeltaTime> fixedDeltaTime)
{
    accumulator->value -= fixedDeltaTime->value;
}

static bool simulatePhysicsStep(Write<PhysicsAccumulator> accumulator, Read<FixedDeltaTime> fixedDeltaTime)
{
    return accumulator->value >= fixedDeltaTime->value;
}

static void integratePositionSystem(
    Query<Write<Position>, Write<PreviousPosition>, Write<PhysicsVelocity>, Read<PhysicsMass>> query,
    Read<Damping> damping, Read<FixedDeltaTime> fixedDeltaTime, Read<Substeps> substeps)
{
    float subDeltaTime = fixedDeltaTime->value / (float)substeps->value;

    for (auto [entity, position, prevPosition, velocity, mass] : query)
    {
        prevPosition->vec = position->vec;

        if (mass->inverseMass <= 0.0F)
        {
            return;
        }

        // Apply damping
        velocity->velocity *= glm::pow(damping->value, subDeltaTime);

        // Apply external forces
        glm::vec3 deltaLinearVelocity = velocity->force * mass->inverseMass * subDeltaTime;

        velocity->velocity += deltaLinearVelocity;
        position->vec += velocity->velocity * subDeltaTime;
    }
}

static void applyCorrectionSystem(Query<Write<Position>, Write<AccumulatedCorrection>> query)
{
    for (auto [entity, position, correction] : query)
    {
        position->vec += correction->vec;
        correction->vec = glm::vec3(0, 0, 0);
    }
}

static void updateVelocitySystem(Query<Read<Position>, Read<PreviousPosition>, Write<PhysicsVelocity>> query,
                                 Read<FixedDeltaTime> fixedDeltaTime, Read<Substeps> substeps)
{
    float subDeltaTime = fixedDeltaTime->value / (float)substeps->value;

    for (auto [entity, position, prevPosition, velocity] : query)
    {
        velocity->velocity = (position->vec - prevPosition->vec) / subDeltaTime;
    }
}

static void applyImpulseSystem(Query<Write<PhysicsVelocity>, Read<PhysicsMass>> query)
{
    for (auto [entity, velocity, mass] : query)
    {
        velocity->velocity += velocity->impulse * mass->inverseMass;
    }
}

static void clearForcesSystem(Query<Write<PhysicsVelocity>> query)
{
    for (auto [entity, velocity] : query)
    {
        velocity->force = glm::vec3(0, 0, 0);
        velocity->impulse = glm::vec3(0, 0, 0);
    }
}

void cubos::engine::physicsPlugin(Cubos& cubos)
{
    cubos.addPlugin(gravityPlugin);

    cubos.addResource<FixedDeltaTime>();
    cubos.addResource<Substeps>();
    cubos.addResource<PhysicsAccumulator>();
    cubos.addResource<Damping>();

    cubos.addComponent<PhysicsVelocity>();
    cubos.addComponent<PhysicsMass>();
    cubos.addComponent<AccumulatedCorrection>();
    cubos.addComponent<PreviousPosition>();

    // executed every frame
    cubos.system(increaseAccumulator).tagged("cubos.physics.simulation.prepare");

    cubos.tag("cubos.physics.apply_forces")
        .after("cubos.physics.simulation.prepare")
        .before("cubos.physics.simulation.substeps.integrate")
        .runIf(simulatePhysicsStep);

    cubos.system(applyImpulseSystem)
        .tagged("cubos.physics.simulation.apply_impulses")
        .after("cubos.physics.apply_forces")
        .before("cubos.physics.simulation.substeps.integrate")
        .runIf(simulatePhysicsStep);
    cubos.system(integratePositionSystem)
        .tagged("cubos.physics.simulation.substeps.integrate")
        .after("cubos.physics.simulation.prepare")
        .runIf(simulatePhysicsStep);
    cubos.system(applyCorrectionSystem)
        .tagged("cubos.physics.simulation.substeps.correct_position")
        .after("cubos.physics.simulation.substeps.integrate")
        .runIf(simulatePhysicsStep);
    cubos.system(updateVelocitySystem)
        .tagged("cubos.physics.simulation.substeps.update_velocity")
        .after("cubos.physics.simulation.substeps.correct_position")
        .runIf(simulatePhysicsStep);
    cubos.system(clearForcesSystem)
        .tagged("cubos.physics.simulation.clear_forces")
        .after("cubos.physics.simulation.substeps.update_velocity")
        .runIf(simulatePhysicsStep);
    cubos.system(decreaseAccumulator)
        .tagged("cubos.physics.simulation.decrease_accumulator")
        .after("cubos.physics.simulation.clear_forces")
        .runIf(simulatePhysicsStep); // at the end of the step
}
