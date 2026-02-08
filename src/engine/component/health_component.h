#pragma once
#include "../../engine/component/component.h"


namespace engine::component
{
    class HealthComponent final : public engine::component::Component
    {
        friend class engine::object::GameObject;

    private:
        int max_health_ = 1;        //最大生命值
        int current_health_ = 1;    //当前生命值
        bool is_invincible_ = false;//是否处于无敌状态
        float invincibility_duration_ = 2.0f;      //无敌时间
        float invincibility_timer_ = 0.0f;         //无敌时间计时器


    public:
        /**
         *@brief Construct a new Health Component object
         *
         * @param max_health 最大生命值，默认是1
         * @param incincibility_duration 无敌状态持续时间，默认是2.0秒
         */
        explicit HealthComponent(int max_health = 1, float incincibility_duration = 2.0f);
        ~HealthComponent() override = default;

        // 禁止拷贝和移动
        HealthComponent(const HealthComponent&) = delete;
        HealthComponent& operator=(const HealthComponent&) = delete;
        HealthComponent(HealthComponent&&) = delete;
        HealthComponent& operator=(HealthComponent&&) = delete;

        /**
         *@brief 对Gameobject施加伤害
         *       如果当前处于无敌状态则伤害无效
         *       如果成功造成伤害且设置了无敌时长，则会触发无敌帧
         *
         * @param damage_amount 造成的伤害量（应该为正数）
         * @return true 成功造成伤害
         * @return false 未造成伤害
         */
        bool takeDamage(int damage_amount);
        void heal(int heal_amount);     //增加GameObject当前生命值

        // --- Getters and Setters ---
        bool isAlive() const { return current_health_ > 0; }        ///< @brief 检查 GameObject 是否存活（当前生命值大于 0）。
        bool isInvincible() const { return is_invincible_; }        ///< @brief 检查 GameObject 是否处于无敌状态。
        int getCurrentHealth() const { return current_health_; }    ///< @brief 获取当前生命值。
        int getMaxHealth() const { return max_health_; }            ///< @brief 获取最大生命值。

        void setCurrentHealth(int current_health);                  ///< @brief 设置当前生命值 (确保不超过最大生命值)。
        void setMaxHealth(int max_health);                          ///< @brief 设置最大生命值 (确保不小于 1)。
        void setInvincible(float duration);                         ///< @brief 设置 GameObject 进入无敌状态，持续时间为 duration 秒。
        void setInvincibilityDuration(float duration) { invincibility_duration_ = duration; } ///< @brief 设置无敌状态持续时间。


    protected:
        void update(float delta_time, engine::core::Context&) override;
    };

}