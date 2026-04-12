#include "animation_component.h"
#include "sprite_component.h"
#include "../object/game_object.h"
#include "../render/animation.h"
#include <spdlog/spdlog.h>

namespace engine::component
{
    AnimationComponent::~AnimationComponent() = default;

    void AnimationComponent::init()
    {
        if (!owner_) {
            spdlog::error("AnimationComponent 没有所有者 GameObject！");
            return;
        }
        sprite_component_ = owner_->getComponent<SpriteComponent>();
        if (!sprite_component_) {
            spdlog::error("GameObject '{}' 的 AnimationComponent 需要 SpriteComponent，但未找到。", owner_->getName());
            return;
        }
    }

    void AnimationComponent::update(float delta_time, engine::core::Context&)
    {
        // 如果没有正在播放的动画，或者没有当前动画，或者没有精灵组件，或者当前动画没有帧，则直接返回
        if (!is_playing_ || !current_animation_ || !sprite_component_ || current_animation_->isEmpty()) {
            spdlog::trace("AnimationComponent 更新时没有正在播放的动画或精灵组件为空。");
            return;
        }

        //推进计时器
        animation_timer_ += delta_time;

        //根据计时器获取当前帧
        const auto& current_frame = current_animation_->getFrame(animation_timer_);

        //更新精灵组件的源矩形
        sprite_component_->setSourceRect(current_frame.source_rect);

        //检查循环是否结束
        if (!current_animation_->isLooping() && animation_timer_ >= current_animation_->getTotalDuration())
        {
            is_playing_ = false;
            animation_timer_ = current_animation_->getTotalDuration();  //将时间限制在结束的时刻
            if (is_one_shot_removal_)   //如果is_one_shot_removal_是true，则删除对象
            {
                owner_->setNeedRemove(true);
            }
        }
    }

    void AnimationComponent::reset(engine::core::Context& context)
    {
        animation_timer_ = 0.0f;
        is_playing_ = true;
        owner_->setNeedRemove(false);
    }

    void AnimationComponent::addAnimation(std::unique_ptr<engine::render::Animation> animation)
    {
        if (!animation) return;
        std::string name = animation->getName();
        animations_[name] = std::move(animation);
        spdlog::debug("已将动画 '{}' 添加到 GameObject '{}'", name, owner_ ? owner_->getName() : "未知");
    }

    void AnimationComponent::playAnimation(const std::string& name)
    {
        auto it = animations_.find(name);
        if (it == animations_.end() || !it->second) {
            spdlog::warn("未找到 GameObject '{}' 的动画 '{}'", name, owner_ ? owner_->getName() : "未知");
            return;
        }

        //如果正在播放相同的动画就不用重新开始
        if (current_animation_ == it->second.get() && is_playing_)
        {
            return;
        }

        //否则重新开始
        current_animation_ = it->second.get();
        animation_timer_ = 0.0f;
        is_playing_ = true;
        //立即更新到第一帧
        if (sprite_component_ && !current_animation_->isEmpty())
        {
            const auto& first_frame = current_animation_->getFrame(animation_timer_);
            sprite_component_->setSourceRect(first_frame.source_rect);
            spdlog::debug("GameObject '{}' 播放动画 '{}'", owner_ ? owner_->getName() : "未知", name);
        }
    }

    std::string AnimationComponent::getCurrentAnimationName() const
    {
        if (current_animation_) {
            return current_animation_->getName();
        }
        return "";
    }

    bool AnimationComponent::isAnimationFinished() const
    {
        // 如果没有当前动画(说明从未调用过playAnimation)，或者当前动画是循环的，则返回 false
        if (!current_animation_ || current_animation_->isLooping()) {
            return false;
        }
        return animation_timer_ >= current_animation_->getTotalDuration();
    }
}