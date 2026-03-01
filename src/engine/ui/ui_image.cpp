#include "ui_image.h"
#include "../render/renderer.h"
#include "../render/sprite.h"
#include "../core/context.h"
#include <spdlog/spdlog.h>

namespace engine::ui
{
    UIImage::UIImage(const std::string& texture_id, const glm::vec2& position, const glm::vec2& size, const std::optional<SDL_FRect>& source_rect, bool is_flipped)
        :UIElement(position, size),
        sprite_(texture_id, source_rect, is_flipped)
    {
        if (texture_id.empty())
        {
            spdlog::warn("创建了一个空纹理ID的UIImage。");
        }
        spdlog::trace("UIImage 构造完成");
    }

    void UIImage::render(engine::core::Context& context)
    {
        if (!visible_ || sprite_.getTextureId().empty())
        {
            return; //设置为不可见或者没有分配纹理则不渲染
        }

        //先渲染自身
        auto position = getPosition();
        if (size_.x == 0.0f && size_.y == 0.0f)     //尺寸为0就渲染原始图片的尺寸
        {
            context.getRenderer().drawUISprite(sprite_, position);
        }
        else
        {
            context.getRenderer().drawUISprite(sprite_, position, size_);
        }
        // 渲染子元素（调用基类方法）
        UIElement::render(context);
    }


}