#pragma once
#include "./component.h"
#include <string>
#include <unordered_map>
#include <memory>


namespace engine::render
{
    class Animation;
}

namespace engine::component
{
    class SpriteComponent;
}

namespace engine::component
{

    /**
     *@brief GameObject的动画组件
     *
     * 持有一组Animation对象并控制其播放
     * 根据当前帧更新关联的SpriteComponent组件
     */
    class AnimationComponent :public Component
    {
        friend class engine::object::GameObject;
    private:
        //动画名称到Animation对象的映射
        std::unordered_map<std::string, std::unique_ptr<engine::render::Animation>> animations_;
        SpriteComponent* sprite_component_ = nullptr;       //指向必须的spritecomponent组件
        engine::render::Animation* current_animation_ = nullptr;    //指向当前播放的动画

        float animation_timer_ = 0.0f;          //动画播放的计时器
        bool is_playing_ = false;               //当前是否有动画正在播放
        bool is_one_shot_removal_ = false;      //是否在播放完动画之后删除整个GameObject

    public:
        AnimationComponent() = default;
        ~AnimationComponent() override;

        // 删除复制/移动操作
        AnimationComponent(const AnimationComponent&) = delete;
        AnimationComponent& operator=(const AnimationComponent&) = delete;
        AnimationComponent(AnimationComponent&&) = delete;
        AnimationComponent& operator=(AnimationComponent&&) = delete;

        void addAnimation(std::unique_ptr<engine::render::Animation> animation);    //向animations_添加动画
        void playAnimation(const std::string& name);        //根据动画名字播放动画
        void stopAnimation() { is_playing_ = false; }       //停止播放当前动画
        void resumeAnimation() { is_playing_ = true; }   ///< @brief 恢复当前动画播放。

        // --- Getters and Setters ---
        std::string getCurrentAnimationName() const;
        bool isPlaying() const { return is_playing_; }
        bool isAnimationFinished() const;
        bool isOneShotRemoval() const { return is_one_shot_removal_; }
        void setOneShotRemoval(bool is_one_shot_removal) { is_one_shot_removal_ = is_one_shot_removal; }

    protected:
        void init() override;
        void update(float, engine::core::Context&) override;
    };
}