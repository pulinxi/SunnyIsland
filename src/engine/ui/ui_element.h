#pragma once
#include <SDL3/SDL_rect.h>
#include <memory>
#include <vector>
#include "../utils/math.h"

namespace engine::core
{
    class Context;
}

namespace engine::ui
{
    /**
     *@brief 所有UI元素的基类。采用组合+继承的方式构成整个UI元素
     *
     * 定义了位置、大小、可见性、状态等通用属性
     * 管理子元素的层次结构
     * 提供事件处理、更新何渲染的虚方法
     */
    class UIElement
    {
    protected:
        glm::vec2 position_;            //相对于父元素的位置
        glm::vec2 size_;                //元素大小
        bool visible_ = true;           //是否可见
        bool need_remove_ = false;        //是否需要移除

        UIElement* parent_ = nullptr;   //父节点
        std::vector<std::unique_ptr<UIElement>> children_;      //存储子元素的列表

    public:
        /**
         * @brief 构造UIElement
         * @param position 初始局部位置
         * @param size 初始大小
         */
        explicit UIElement(const glm::vec2& position = { 0.0f, 0.0f }, const glm::vec2& size = { 0.0f, 0.0f });


        /**
         * @brief 虚析构函数，确保派生类正确清理
         */
        virtual ~UIElement() = default;

        virtual bool handleInput(engine::core::Context& context);
        virtual void update(float delta_time, engine::core::Context& context);
        virtual void render(engine::core::Context& context);

        //层次结构管理函数
        void addChild(std::unique_ptr<UIElement> child);        //添加子元素
        std::unique_ptr<UIElement> removeChild(UIElement* child_ptr);       //将指定ui元素从列表中移除，并返回其智能指针
        void removeAllChildren();

        // --- Getters and Setters ---
        const glm::vec2& getSize() const { return size_; }              ///< @brief 获取元素大小
        const glm::vec2& getPosition() const { return position_; }      ///< @brief 获取元素位置(相对于父节点)
        bool isVisible() const { return visible_; }                     ///< @brief 检查元素是否可见
        bool isNeedRemove() const { return need_remove_; }              ///< @brief 检查元素是否需要移除
        UIElement* getParent() const { return parent_; }                ///< @brief 获取父元素
        const std::vector<std::unique_ptr<UIElement>>& getChildren() const { return children_; } ///< @brief 获取子元素列表

        void setSize(const glm::vec2& size) { size_ = size; }           ///< @brief 设置元素大小
        void setVisible(bool visible) { visible_ = visible; }           ///< @brief 设置元素的可见性
        void setParent(UIElement* parent) { parent_ = parent; }         ///< @brief 设置父节点
        void setPosition(const glm::vec2& position) { position_ = position; }   ///< @brief 设置元素位置(相对于父节点)
        void setNeedRemove(bool need_remove) { need_remove_ = need_remove; }    ///< @brief 设置元素是否需要移除

        //辅助方法
        engine::utils::Rect getBounds() const;          //获取UI元素的边界
        glm::vec2 getScreenPosition() const;            //获取ui元素的屏幕坐标
        bool isPointInside(const glm::vec2& point) const;//检查给定的点是否在元素的内部

        // --- 禁用拷贝和移动语义 ---
        UIElement(const UIElement&) = delete;
        UIElement& operator=(const UIElement&) = delete;
        UIElement(UIElement&&) = delete;
        UIElement& operator=(UIElement&&) = delete;
    };
}