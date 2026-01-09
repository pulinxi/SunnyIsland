#pragma once

namespace engine::object
{
    class GameObject;
}

namespace engine::component
{

    /**
     *@brief 组件类的抽象基类
     *
     * 所有具体的组件都应该继承这个类
     * 定义了组件生命周期可能调用的通用方法
     */
    class Component
    {
        friend class engine::object::GameObject;        //应为GameObejct需要调用init方法，所以设为友元


    protected:
        engine::object::GameObject* onwer_ = nullptr;       //指向拥有这个组件的GameObject，并且每个组件不可以没有这个指针

    public:
        Component() = default;
        virtual ~Component() = default;         //虚析构函数确保派生类的析构函数调用时正确清理派生类，当通过基类指针删除派生类对象时，如果基类析构函数不是虚函数，只会调用基类的析构函数，导致派生类的资源无法正确释放。


        //禁止拷贝和移动
        Component(const Component&) = delete;
        Component& operator=(const Component&) = delete;
        Component(Component&&) = delete;
        Component& operator=(Component&&) = delete;

        void setOnwer(engine::object::GameObject* onwer) { onwer_ = onwer; }
        engine::object::GameObject* getOnwer() const { return onwer_; }


    protected:

        //关键训话函数，全部设为保护，只有GameObject可以调用（未来把其中一个改为=0实现抽象类）
        virtual void init() {}                      ///< @brief 保留两段初始化的机制，GameObject 添加组件时自动调用，不需要外部调用
        virtual void handleInput() {}               ///< @brief 处理输入
        virtual void update(float) {}               ///< @brief 更新
        virtual void render() {}                    ///< @brief 渲染
        virtual void clean() {}                     ///< @brief 清理




    };



}