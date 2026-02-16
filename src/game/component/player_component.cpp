#include "player_component.h"
#include "../../engine/object/game_object.h"
#include "../../engine/component/physics_component.h"
#include "../../engine/component/transform_component.h"
#include "../../engine/component/sprite_component.h"
#include "../../engine/component/animation_component.h"
#include "../../engine/component/health_component.h"
#include "../../engine/input/input_manager.h"
#include "state/idle_state.h"
#include "state/hurt_state.h"
#include "state/dead_state.h"
#include <utility>
#include <typeinfo>
#include <spdlog/spdlog.h>

namespace game::component
{
    bool PlayerComponent::takeDamage(int damage)
    {
        if (is_dead_ || !health_component_ || damage <= 0)
        {
            spdlog::warn("玩家已死亡或却少必要组件，并未造成伤害.");
            return false;
        }

        bool success = health_component_->takeDamage(damage);
        if (!success) return false;

        //成功造成伤害，根据是否存活来决定状态切换
        // --- 成功造成伤害了，根据是否存活决定状态切换
        if (health_component_->isAlive()) {
            spdlog::debug("玩家受到了 {} 点伤害，当前生命值: {}/{}。",
                damage, health_component_->getCurrentHealth(), health_component_->getMaxHealth());
            // 切换到受伤状态
            setState(std::make_unique<state::HurtState>(this));
        }
        else {
            spdlog::debug("玩家死亡。");
            is_dead_ = true;
            // 切换到死亡状态
            setState(std::make_unique<state::DeadState>(this));
        }
        return true;
    }

    void PlayerComponent::setState(std::unique_ptr<state::PlayerState> new_state)
    {
        if (!new_state)
        {
            spdlog::error("尝试设置空的状态");
            return;
        }
        if (current_state_)
        {
            current_state_->exit();
        }

        current_state_ = std::move(new_state);
        spdlog::debug("玩家组件正在切换到状态: {}", typeid(*current_state_).name());
        current_state_->enter();
    }

    bool PlayerComponent::is_on_ground() const
    {
        return coyote_timer_ <= coyote_time_ || physics_component_->hasCollidedBelow();
    }

    void PlayerComponent::init()
    {
        if (!owner_)
        {
            spdlog::error("PlayerComponent 没有所属游戏对象!");
            return;
        }

        //获取必要的组件
        transform_component_ = owner_->getComponent<engine::component::TransformComponent>();
        physics_component_ = owner_->getComponent<engine::component::PhysicsComponent>();
        sprite_component_ = owner_->getComponent<engine::component::SpriteComponent>();
        animation_component_ = owner_->getComponent<engine::component::AnimationComponent>();
        health_component_ = owner_->getComponent<engine::component::HealthComponent>();

        //检查组件是否存在
        if (!transform_component_ || !physics_component_ || !sprite_component_ || !animation_component_ || !health_component_) {
            spdlog::error("Player 对象缺少必要组件！");
        }

        //初始化状态机
        current_state_ = std::make_unique<state::IdleState>(this);
        if (current_state_)
        {
            setState(std::move(current_state_));
        }
        else
        {
            spdlog::error("初始化玩家状态失败（make_unique 返回空指针）！");
        }
        spdlog::debug("PlayerComponent 初始化完成。");
    }

    void PlayerComponent::handleInput(engine::core::Context& context)
    {
        if (!current_state_) return;

        auto next_state = current_state_->handleInput(context);
        if (next_state)
        {
            setState(std::move(next_state));
        }
    }

    void PlayerComponent::update(float delta_time, engine::core::Context& context)
    {
        if (!current_state_) return;

        //一旦离地，开始计时土狼时间
        if (!physics_component_->hasCollidedBelow())
        {
            coyote_timer_ += delta_time;
        }
        else//碰到地面就重置土狼时间
        {
            coyote_timer_ = 0.0f;
        }

        if (health_component_->isInvincible())
        {
            flash_timer_ += delta_time;
            if (flash_timer_ >= 2 * flash_interval_)    //让闪烁计时器在0~2倍的flash_interval_之间，如果大于flash_interval_就隐藏精灵图，反之则显示
            {
                flash_timer_ -= 2 * flash_interval_;
            }
            if (flash_timer_ < flash_interval_)
            {
                sprite_component_->setHidden(true);
            }
            else
            {
                sprite_component_->setHidden(false);
            }
        }
        else if (sprite_component_->isHidden()) //非无敌状态要确保精灵图设置为可见
        {
            sprite_component_->setHidden(false);
        }

        auto next_state = current_state_->update(delta_time, context);
        if (next_state) {
            setState(std::move(next_state));
        }
    }
}