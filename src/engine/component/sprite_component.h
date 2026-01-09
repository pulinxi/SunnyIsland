#pragma once
#include "../render/sprite.h"
#include "component.h"
#include "../utils/alignment.h"
#include <string>
#include <optional>
#include <SDL3/SDL_rect.h>
#include <glm/vec2.hpp>


namespace engine::core
{
    class Context;
}

namespace engine::resource
{
    class ResourceManager;
}

namespace engine::component
{
    class TransformComponent;



    class SpriteComponent final : public engine::component::Component
    {


        friend class engine::object::GameObject;        //友元无法继承，必须每个子类单独声明

    private:
        engine::resource::ResourceManager* resource_manager_ = nullptr;     //资源管理器
        TransformComponent* transform_ = nullptr;

        engine::render::Sprite sprite_;                //精灵图指针
        engine::utils::Alignment alignment_ = engine::utils::Alignment::NONE;       //对其方式
        glm::vec2 sprite_size_ = { 0.0f,0.0f };         //精灵大小
        glm::vec2 offset_ = { 0.0f,0.0f };              //偏移量（配合对其方式使用）
        bool is_hidden_ = false;                        //是否需要隐藏(不渲染)

    public:

        /**
         * @brief 构造函数
         * @param texture_id 纹理资源的标识符。
         * @param resource_manager 资源管理器指针。
         * @param alignment 初始对齐方式。
         * @param source_rect_opt 可选的源矩形。
         * @param is_flipped 初始翻转状态。
         */
        SpriteComponent(
            const std::string& texture_id,
            engine::resource::ResourceManager& resource_manager,
            engine::utils::Alignment alignment = engine::utils::Alignment::NONE,
            std::optional<SDL_FRect> source_rect_opt = std::nullopt,
            bool is_flipped = false
        );


        ~SpriteComponent() override = default;

        // 禁止拷贝和移动
        SpriteComponent(const SpriteComponent&) = delete;
        SpriteComponent& operator=(const SpriteComponent&) = delete;
        SpriteComponent(SpriteComponent&&) = delete;
        SpriteComponent& operator=(SpriteComponent&&) = delete;


        void updateOffset();        //根据当前的alignment和sprite_size更新offset


        // Getters
        const engine::render::Sprite& getSprite() const { return sprite_; }         ///< @brief 获取精灵对象
        const std::string& getTextureId() const { return sprite_.getTextureId(); }  ///< @brief 获取纹理ID
        bool isFlipped() const { return sprite_.isFlipped(); }                      ///< @brief 获取是否翻转
        bool isHidden() const { return is_hidden_; }                                ///< @brief 获取是否隐藏
        const glm::vec2& getSpriteSize() const { return sprite_size_; }             ///< @brief 获取精灵尺寸
        const glm::vec2& getOffset() const { return offset_; }                      ///< @brief 获取偏移量
        engine::utils::Alignment getAlignment() const { return alignment_; }        ///< @brief 获取对齐方式

        // Setters
        void setSpriteById(const std::string& texture_id, const std::optional<SDL_FRect>& source_rect_opt = std::nullopt); ///< @brief 设置精灵对象
        void setFlipped(bool flipped) { sprite_.setFlipped(flipped); }                                             ///< @brief 设置是否翻转
        void setHidden(bool hidden) { is_hidden_ = hidden; }                                                      ///< @brief 设置是否隐藏
        void setSourceRect(const std::optional<SDL_FRect>& source_rect_opt);                                     ///< @brief 设置源矩形
        void setAlignment(engine::utils::Alignment anchor);


    private:
        void updateSpriteSize();            //根据 sprite_ 的 source_rect_ 更新 sprite_size_

        void init() override;                                               //初始化函数要覆盖
        void update(float, engine::core::Context&) override {}              //更新函数留空
        void render(engine::core::Context& context) override;               //渲染函数要覆盖

    };



}