#pragma once

namespace game::component {
    class AIComponent;
}

namespace game::component::ai
{

    class AIBehavior
    {
        friend class game::component::AIComponent;
    public:
        AIBehavior() = default;
        virtual ~AIBehavior() = default;

        //禁止移动和拷贝
        AIBehavior(const AIBehavior&) = delete;
        AIBehavior& operator=(const AIBehavior&) = delete;
        AIBehavior(AIBehavior&&) = delete;
        AIBehavior& operator=(AIBehavior&&) = delete;

    protected:
        //没有owner指针，需要传入AIComponent引用
        virtual void enter(AIComponent&) {}      //enter函数可选是否实现，默认为空
        virtual void update(float, AIComponent&) = 0;   //更新 AI 行为逻辑(具体策略)，必须实现
    };
}