#pragma once
#include "ui_element.h"
#include "state/ui_state.h"
#include "../render/sprite.h"   // 需要引入头文件而不是前置声明（map容器创建时可能会检查内部元素是否有析构定义）
#include <memory>
#include <string>
#include <unordered_map>


namespace engine::core {
    class Context;
}

namespace engine::ui
{
    class UIInteractive : public UIElement
    {
    protected:
        engine::core::Context& context_;            //可交互元素可能需要其他引擎组件
        std::unique_ptr<engine::ui::state::UIState> state_;     //当前状态
        std::unordered_map<std::string, std::unique_ptr<engine::render::Sprite>> sprites_;      //精灵集合
        std::unordered_map<std::string, std::string> sounds_;   //音效集合，key时音效名称，value是音效路径
        engine::render::Sprite* current_sprite_ = nullptr;        //当前显示的精灵
        bool interactive_ = true;                     //是否可交互

    public:
        UIInteractive(engine::core::Context& context, const glm::vec2& position = { 0.0f, 0.0f }, const glm::vec2& size = { 0.0f, 0.0f });
        ~UIInteractive() override;

        virtual void clicked() {}       //子类如果有点击事件就重写这个方法


        void addSprite(const std::string& name, std::unique_ptr<engine::render::Sprite> sprite);///< @brief 添加精灵
        void setSprite(const std::string& name);                                                ///< @brief 设置当前显示的精灵
        void addSound(const std::string& name, const std::string& path);                        ///< @brief 添加音效
        void playSound(const std::string& name);                                                ///< @brief 播放音效

        // --- Getters and Setters ---
        void setState(std::unique_ptr<engine::ui::state::UIState> state);       ///< @brief 设置当前状态
        engine::ui::state::UIState* getState() const { return state_.get(); }   ///< @brief 获取当前状态

        void setInteractive(bool interactive) { interactive_ = interactive; }   ///< @brief 设置是否可交互
        bool isInteractive() const { return interactive_; }                     ///< @brief 获取是否可交互

        // --- 核心方法 ---
        bool handleInput(engine::core::Context& context) override;
        void render(engine::core::Context& context) override;


    };
}