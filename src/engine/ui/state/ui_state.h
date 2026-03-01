#pragma once
#include <memory>

namespace engine::core {
    class Context;
}

namespace engine::ui {
    class UIInteractive;
}


namespace engine::ui::state
{
    /**
     *@brief 可交互UI元素在特定状态下的行为接口
     *
     * 该接口定义了所有具体UI状态必须实现的通用操作
     * 例如处理输入事件、更新状态逻辑以及去确定视觉表现
     */
    class UIState
    {
        friend engine::ui::UIInteractive;

    protected:
        engine::ui::UIInteractive* owner_ = nullptr;        //指向父节点

    public:
        /**
         *@brief Construct a new UIState object
         *
         * @param owner 父节点指针
         */
        UIState(engine::ui::UIInteractive* owner) : owner_(owner) {}
        virtual ~UIState() = default;

        // 删除拷贝和移动构造函数/赋值运算符
        UIState(const UIState&) = delete;
        UIState& operator=(const UIState&) = delete;
        UIState(UIState&&) = delete;
        UIState& operator=(UIState&&) = delete;

    protected:
        virtual void enter() {}
        virtual std::unique_ptr<UIState> handleInput(engine::core::Context& context) = 0;
    };
}