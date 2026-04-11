#include "climb_state.h"
#include "jump_state.h"
#include "idle_state.h"
#include "walk_state.h"
#include "fall_state.h"
#include "dash_state.h"
#include "../player_component.h"
#include "../../../engine/core/context.h"
#include "../../../engine/input/input_manager.h"
#include "../../../engine/component/physics_component.h"
#include "../../../engine/component/sprite_component.h"
#include "../../../engine/component/animation_component.h"
#include <glm/common.hpp>
#include <spdlog/spdlog.h>

namespace game::component::state
{
    void ClimbState::enter()
    {
        spdlog::debug("进入攀爬状态");
        playAnimation("climb");
        if (auto* physics = player_component_->getPhysicsComponent();physics)
        {
            physics->setUseGravity(false);
        }
    }

    void ClimbState::exit()
    {
        spdlog::debug("退出攀爬状态");

        if (auto* physics = player_component_->getPhysicsComponent(); physics) {
            physics->setUseGravity(true); // 重新启用重力
        }
    }

    std::unique_ptr<PlayerState> ClimbState::handleInput(engine::core::Context& context)
    {
        auto input_manager = context.getInputManager();
        auto physics_component = player_component_->getPhysicsComponent();
        auto animation_component = player_component_->getAnimationComponent();

        // 如果按下冲刺键
        if (input_manager.isActionPressed("dash") && player_component_->getDashCount() > 0)
        {
            if (input_manager.isActionDown("move_left"))
            {
                player_component_->setDashDirection({ -1.0f, player_component_->getDashDirection().y });
            }
            else if (input_manager.isActionDown("move_right"))
            {
                player_component_->setDashDirection({ 1.0f, player_component_->getDashDirection().y });
            }
            if (input_manager.isActionDown("move_up"))
            {
                player_component_->setDashDirection({ player_component_->getDashDirection().x, -1.0f });
            }
            else if (input_manager.isActionDown("move_down"))
            {
                player_component_->setDashDirection({ player_component_->getDashDirection().x, 1.0f });
            }
            return std::make_unique<DashState>(player_component_);
        }

        //攀爬状态下，按键就移动，不按就静止
        auto is_up = input_manager.isActionDown("move_up");
        auto is_down = input_manager.isActionDown("move_down");
        auto is_left = input_manager.isActionDown("move_left");
        auto is_right = input_manager.isActionDown("move_right");
        auto speed = player_component_->getClimbSpeed();

        //三目运算符嵌套
        physics_component->velocity_.y = is_up ? -speed : (is_down ? speed : 0.0f);
        physics_component->velocity_.x = is_left ? -speed : (is_right ? speed : 0.0f);

        //根据是否右按键决定动画的播放情况
        (is_up || is_down || is_left || is_right) ? animation_component->resumeAnimation() : animation_component->stopAnimation();

        if (input_manager.isActionPressed("jump"))
        {
            return std::make_unique<JumpState>(player_component_);
        }
        return nullptr;

    }

    std::unique_ptr<PlayerState> ClimbState::update(float delta_time, engine::core::Context&)
    {
        auto physics_component = player_component_->getPhysicsComponent();
        player_component_->setDashCount(player_component_->getDashTimes()); //重置冲刺次数

        // 如果着地，则切换到 IdleState
        if (physics_component->hasCollidedBelow()) {
            return std::make_unique<IdleState>(player_component_);
        }
        // 如果离开梯子区域，则切换到 FallState（能走到这里 说明非着地状态）
        if (!physics_component->hasCollidedLadder()) {
            return std::make_unique<FallState>(player_component_);
        }
        return nullptr;
    }



} // namespace game::component::state