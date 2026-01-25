#pragma once
#include <vector>
#include <utility>  // for std::pair
#include "glm/vec2.hpp"

namespace engine::component
{
    class PhysicsComponent;
}
namespace engine::object
{
    class GameObject;
}

namespace engine::physics
{

    /**
     * @brief 负责管理和模拟物理行为及碰撞检测。
     */
    class PhysicsEngine
    {
    private:
        std::vector<engine::component::PhysicsComponent*> components_;      //存储注册的物理组件
        glm::vec2 gravity_ = { 0.0f,980.0f };           // 默认重力值(像素/秒^2,相当于100像素对应1m)
        float max_speed_ = 500.9f;                      //最大速度(像素/秒)

        // 存储本帧发生碰撞的碰撞对(update时清空)
        std::vector<std::pair<engine::object::GameObject*, engine::object::GameObject*>> collision_pairs_;

    public:
        PhysicsEngine() = default;

        //禁止移动和拷贝
        PhysicsEngine(const PhysicsEngine&) = delete;
        PhysicsEngine& operator=(const PhysicsEngine&&) = delete;
        PhysicsEngine(const PhysicsEngine&&) = delete;
        PhysicsEngine& operator=(PhysicsEngine&&) = delete;


        void registerComponent(engine::component::PhysicsComponent* component);     //注销物理组件
        void unregisterComponent(engine::component::PhysicsComponent* component);   //注册物理组件

        /**
         *@brief 检测并处理对象之间的碰撞，并记录需要游戏逻辑处理的碰撞对。
         *
         */
        void checkObjectCollisions();

        /**
         *@brief 核心循环，会更新所有注册的物理组件的状态，lesson15只考虑重力
         *  并且更新完之后清楚当前所有力，每帧重新考虑所有受到的力
         *
         * @param delta_time 单位时间
         */
        void update(float delta_time);


        // setter and getter
        void setGravity(const glm::vec2& gravity) { gravity_ = gravity; }   ///< @brief 设置全局重力加速度
        const glm::vec2& getGravity() const { return gravity_; }            ///< @brief 获取当前的全局重力加速度
        void setMaxSpeed(float max_speed) { max_speed_ = max_speed; }       ///< @brief 设置最大速度
        float getMaxSpeed() const { return max_speed_; }                    ///< @brief 获取当前的最大速度
        /// @brief 获取本帧检测到的所有 GameObject 碰撞对。(此列表在每次 update 开始时清空)
        const std::vector<std::pair<engine::object::GameObject*, engine::object::GameObject*>>& getCollisionPairs() const {
            return collision_pairs_;
        };

    };

}