#pragma once
#include "./component.h"
#include "../utils/alignment.h"
#include "../render/sprite.h"
#include <glm/vec2.hpp>
#include <vector>

namespace engine::core
{
    class Context;
}

namespace engine::resource
{
    class ResourceManager;
}

namespace engine::component {
    class TransformComponent;

    /**
     *@brief 拖尾组件,负责管理拖尾的精灵图，并使用verlet积分对拖尾之间的距离进行限制（但事实上我想实现的是celeste中madline的头发漂浮效果）
     *
     */
    class TrailComponent : public Component
    {
        friend engine::object::GameObject;
    private:
        TransformComponent* transform_ = nullptr;             //游戏对象的位置
        engine::resource::ResourceManager* resource_manager_ = nullptr;     //资源管理器

        engine::render::Sprite sprite_node_;        //每一个节点的精灵图
        std::vector<glm::vec2> nodes_position_;     //每个节点的位置
        glm::vec2 sprite_scale_ = glm::vec2(1.0f);      //每个节点的精灵图缩放
        glm::vec2 sprite_offset_ = glm::vec2(0.0f, 0.0f);                          //每个节点的sprite相对于节点的偏移量
        glm::vec2 first_node_position_offset_ = glm::vec2(12.0f, 14.0f);             //因为第一个节点相对于实体的要固定，所以单独设置一个变量存储第一个节点相对于游戏对象的偏移量

        engine::utils::Alignment alignment_ = engine::utils::Alignment::NONE;       //对其方式
        float distance_ = 10.0f;                       //节点间的距离

    public:
        TrailComponent(engine::resource::ResourceManager* resource_manager, engine::render::Sprite sprite_node,
            float distance, glm::vec2 sprite_scale = glm::vec2(1.0f), engine::utils::Alignment alignment = engine::utils::Alignment::NONE);
        ~TrailComponent() = default;

        //删除拷贝和移动构造
        TrailComponent(TrailComponent& t) = delete;
        TrailComponent& operator=(TrailComponent& t) = delete;
        TrailComponent(TrailComponent&& t) = delete;
        TrailComponent& operator=(TrailComponent&& t) = delete;

        void init();                                              ///< @brief 初始化
        void update(float, engine::core::Context&);               ///< @brief 更新
        void render(engine::core::Context&);                      ///< @brief 渲染


        void updateSpriteOffset();        //更新偏移量

        //getter and setter
        void setFirstNodePositionOffset(glm::vec2 offset);
        glm::vec2 getFirstNodePositionOffset() { return first_node_position_offset_; }
        void setDistance(float distance);
        float getDistance() { return distance_; }
        void setAlignment(engine::utils::Alignment alignment) { alignment_ = alignment; updateSpriteOffset(); }
        engine::utils::Alignment getAlignment() { return alignment_; }
        std::vector<glm::vec2>& getNodesPosition() { return nodes_position_; }
        void setSpriteScale(glm::vec2 scale);
        glm::vec2 getSpriteScale() { return sprite_scale_; }

    private:
        void initNodesPosition(int count);    ///< @brief 初始化节点位置
    };

}