#include "physics_engine.h"
#include <spdlog/spdlog.h>
#include "../object/game_object.h"
#include "glm/common.hpp"
#include "../component/physics_component.h"
#include "../component/transform_component.h"
#include <algorithm>



namespace engine::physics
{
    void PhysicsEngine::registerComponent(engine::component::PhysicsComponent* component)
    {
        components_.push_back(component);
        spdlog::trace("物理组件注册完成");
    }

    void PhysicsEngine::unregisterComponent(engine::component::PhysicsComponent* component)
    {
        auto it = std::remove(components_.begin(), components_.end(), component);
        components_.erase(it, components_.end());
        spdlog::trace("物理组件注销完成");
    }

    void PhysicsEngine::update(float delta_time)
    {
        for (auto* pc : components_)
        {
            if (!pc || !pc->isEnabled())// 检查组件是否有效和启用
            {
                continue;
            }

            //应用重力(如果组件受重力影响) : F=g*m
            if (pc->isUseGravity())
            {
                pc->addForce(gravity_ * pc->getMass());
            }
            /* 还可以添加其它力影响，比如风力、摩擦力等，目前不考虑 */

            //更新速度：v+=a*dt,其中a=F/m
            pc->velocity_ += (pc->getForce() / pc->getMass()) * delta_time;
            pc->clearForce();//清楚当前帧的力

            //更新位置
            auto* tc = pc->getTransform();
            if (tc)
            {
                tc->translate(pc->velocity_ * delta_time);
            }

            //限制最大速度
            pc->velocity_ = glm::clamp(pc->velocity_, -max_speed_, max_speed_);

        }
    }





} // namespace engine::physics

