#pragma once
#include "../render/sprite.h"
#include "string"
#include <glm/vec2.hpp>
#include "component.h"

namespace engine::component
{
    class TransformComponent;



    /**
     *@brief 在背景中渲染可滚动纹理的组件，以创建视差效果。
     *
     * 该组件根据相机的位置和滚动因子来移动纹理。
     */
    class ParallaxComponent final : public Component {

        friend class engine::object::GameObject;

    private:
        TransformComponent* transform_ = nullptr;       //缓存变换组件

        engine::render::Sprite sprite_;                 //精灵对象
        glm::vec2 scroll_factor_;                       //滚动速度(0静止，1随相机移动，0<x<1则比相机慢)
        glm::bvec2 repeat_;                             //是否需要沿着x和y轴重复
        bool is_hidden_ = false;                        //是否渲染



    public:
        /**
         *@brief Construct a new Parallax Component object
         *
         * @param texture_id 背景纹理资源
         * @param scroll_factor 控制背景相对于相机移动速度的因子
         *                      (0,0)表示完全静止
         *                      (1,1)表示完全同步
         *                      (0.5,0.5)表示以相机一半的速度运动
         * @param repeat 是否重复
         */
        ParallaxComponent(const std::string& texture_id, const glm::vec2& scroll_factor, const glm::bvec2& repeat);

        // --- 设置器 ---
        void setSprite(const engine::render::Sprite& sprite) { sprite_ = sprite; }  ///< @brief 设置精灵对象
        void setScrollFactor(const glm::vec2& factor) { scroll_factor_ = factor; }  ///< @brief 设置滚动速度因子
        void setRepeat(const glm::bvec2& repeat) { repeat_ = repeat; }              ///< @brief 设置是否重复
        void setHidden(bool hidden) { is_hidden_ = hidden; }                        ///< @brief 设置是否隐藏（不渲染）

        // --- 获取器 ---
        const engine::render::Sprite& getSprite() const { return sprite_; }          ///< @brief 获取精灵对象
        const glm::vec2& getScrollFactor() const { return scroll_factor_; }          ///< @brief 获取滚动速度因子
        const glm::bvec2& getRepeat() const { return repeat_; }                      ///< @brief 获取是否重复
        bool isHidden() const { return is_hidden_; }                                  ///< @brief 获取是否隐藏（不渲染）


    protected:
        // 核心循环函数覆盖
        void update(float, engine::core::Context&) override {}     // 必须实现纯虚函数，留空
        void init() override;
        void render(engine::core::Context& context) override;

    };

}