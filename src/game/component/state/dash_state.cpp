#include "dash_state.h"
#include <glm/glm.hpp>
#include <math.h>
#include "../../../engine/component/physics_component.h"
#include "../../../engine/component/sprite_component.h"
#include "../player_component.h"
#include "idle_state.h"

namespace game::component::state
{
    void DashState::enter()
    {
        playAnimation("jump");

        auto* physics_component = player_component_->getPhysicsComponent();
        float dash_speed = player_component_->getDashSpeed();
        glm::vec2 dash_direction = player_component_->getDashDirection();

        player_component_->setDashTimer(0.0f);
        player_component_->setDashCount(player_component_->getDashCount() - 1);
        physics_component->setUseGravity(false);
        dash_direction = glm::normalize(dash_direction);        //归一化向量，以保证斜方向和水平方向冲刺速度一致


        if (glm::length(dash_direction) > 0.01f)
        {
            player_component_->setDashSpeedTotal(dash_speed * dash_direction);
        }
        else
        {
            player_component_->setDashSpeedTotal({ dash_speed, 0.0f });
        }
        spdlog::debug("DashState::enter() - Dash speed: {} {}", player_component_->getDashSpeedTotal().x, player_component_->getDashSpeedTotal().y);
    }

    void DashState::exit()
    {
        auto* physics_component = player_component_->getPhysicsComponent();
        physics_component->setUseGravity(true);
        player_component_->resetDashDirection();
    }

    std::unique_ptr<PlayerState> DashState::handleInput(engine::core::Context&)
    {
        return nullptr;
    }

    std::unique_ptr<PlayerState> DashState::update(float delta_time, engine::core::Context&)
    {
        player_component_->setDashTimer(player_component_->getDashTimer() + delta_time);
        if (player_component_->getDashTimer() >= player_component_->getDashInterval())      //冲刺时间递增
        {
            return std::make_unique<IdleState>(player_component_);
        }
        else
        {
            auto physics_component = player_component_->getPhysicsComponent();
            auto speed_total = player_component_->getDashSpeedTotal();
            physics_component->setVelocity(speed_total);

        }
        return nullptr;
    }


}