#include "trail_component.h"
#include "transform_component.h"
#include "../core/context.h"
#include "../render/renderer.h"
#include "../resource/resource_manager.h"
#include "../object/game_object.h"
#include <spdlog/spdlog.h>

namespace engine::component {
    void TrailComponent::init()
    {
        if (!owner_)
        {
            spdlog::error("SpriteComponent 初始化前没有设置所有者。");
            return;
        }
        transform_ = owner_->getComponent<TransformComponent>();
        if (!transform_)
        {
            spdlog::warn("GameObject '{}' 上的 SpriteComponent 需要一个 TransformComponent，但未找到。", owner_->getName());
            return;
        }
        updateSpriteOffset();
        initNodesPosition(10);
    }

    void TrailComponent::update(float, engine::core::Context&)
    {
        if (transform_) {
            nodes_position_[0] = transform_->getPosition() + first_node_position_offset_ * transform_->getScale();
        }

        const int repeattimes = 7;
        for (int i = 1; i < repeattimes; i++)
        {
            for (int j = 1;j < nodes_position_.size();j++)
            {
                //1.找到两点的坐标
                glm::vec2& A = nodes_position_[j - 1];
                glm::vec2& B = nodes_position_[j];

                //2.计算两点当前的距离
                float dx = B.x - A.x;;
                float dy = B.y - A.y;
                float currdistance = glm::sqrt(dx * dx + dy * dy);

                //3.防止两点重合
                if (currdistance < 1.0f)        //如果重合就从x方向推开
                {
                    dx = 1.0f;
                    dy = 0.0f;
                    currdistance = 1.0f;
                }

                //4.计算当前点应该移动的距离
                float factor = (distance_ - currdistance) / currdistance;

                //5.计算移动的量
                float move_x = dx * factor;
                float move_y = dy * factor;

                //6.移动当前点
                if (i == 1)
                {
                    B.x += move_x;
                    B.y += move_y;
                }
                else
                {
                    B.x += move_x;
                    B.y += move_y;
                    A.x -= move_x;
                    A.y -= move_y;
                }
            }

        }
    }

    void TrailComponent::render(engine::core::Context& context)
    {
        for (int i = 0;i < nodes_position_.size();i++)
        {
            context.getRenderer().drawSprite(context.getCamera(), sprite_node_,
                glm::vec2(nodes_position_[i].x + sprite_offset_.x, nodes_position_[i].y + sprite_offset_.y), sprite_scale_);


        }
    }

    void TrailComponent::updateSpriteOffset()
    {
        glm::vec2 sprite_size = glm::vec2(0.0f);
        if (!resource_manager_) {
            spdlog::error("ResourceManager 为空！无法获取纹理尺寸。");
            return;
        }
        if (sprite_node_.getSourceRect().has_value()) {
            const auto& src_rect = sprite_node_.getSourceRect().value();
            sprite_size = { src_rect.w, src_rect.h };
        }
        else {
            sprite_size = resource_manager_->getTextureSize(sprite_node_.getTextureId());
        }
        auto scale = sprite_scale_;
        switch (alignment_) {
        case engine::utils::Alignment::TOP_LEFT:      sprite_offset_ = glm::vec2{ 0.0f, 0.0f } * scale; break;
        case engine::utils::Alignment::TOP_CENTER:    sprite_offset_ = glm::vec2{ -sprite_size.x / 2.0f, 0.0f } * scale; break;
        case engine::utils::Alignment::TOP_RIGHT:     sprite_offset_ = glm::vec2{ -sprite_size.x, 0.0f } * scale; break;
        case engine::utils::Alignment::CENTER_LEFT:   sprite_offset_ = glm::vec2{ 0.0f, -sprite_size.y / 2.0f } * scale; break;
        case engine::utils::Alignment::CENTER:        sprite_offset_ = glm::vec2{ -sprite_size.x / 2.0f, -sprite_size.y / 2.0f } * scale; break;
        case engine::utils::Alignment::CENTER_RIGHT:  sprite_offset_ = glm::vec2{ -sprite_size.x, -sprite_size.y / 2.0f } * scale; break;
        case engine::utils::Alignment::BOTTOM_LEFT:   sprite_offset_ = glm::vec2{ 0.0f, -sprite_size.y } * scale; break;
        case engine::utils::Alignment::BOTTOM_CENTER: sprite_offset_ = glm::vec2{ -sprite_size.x / 2.0f, -sprite_size.y } * scale; break;
        case engine::utils::Alignment::BOTTOM_RIGHT:  sprite_offset_ = glm::vec2{ -sprite_size.x, -sprite_size.y } * scale; break;
        case engine::utils::Alignment::NONE:
        default:                                      break;
        }
    }

    void TrailComponent::setFirstNodePositionOffset(glm::vec2 offset)
    {
        first_node_position_offset_ = offset;
    }

    void TrailComponent::setDistance(float distance)
    {
        distance_ = distance;
    }

    void TrailComponent::setSpriteScale(glm::vec2 scale)
    {
        sprite_scale_ = scale;
        updateSpriteOffset();
    }

    void TrailComponent::initNodesPosition(int count)
    {
        glm::vec2 first = transform_->getPosition() + first_node_position_offset_ * transform_->getScale();
        for (int i = 0; i < count; ++i)
        {
            nodes_position_.emplace_back(first.x - i * distance_ * 10 * sprite_scale_.x, first.y);
        }
    }


    TrailComponent::TrailComponent(engine::resource::ResourceManager* resource_manager,
        engine::render::Sprite sprite_node, float distance, glm::vec2 sprite_scale, engine::utils::Alignment alignment)
        :
        resource_manager_(resource_manager),
        sprite_node_(sprite_node),
        distance_(distance),
        sprite_scale_(sprite_scale),
        alignment_(alignment)
    {
        if (!resource_manager_)
        {
            spdlog::error("TrailComponent: resource_manager_ 为空");
        }
    }

}