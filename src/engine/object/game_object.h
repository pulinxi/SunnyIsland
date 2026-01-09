#pragma once
#include "../component/component.h" 
#include <memory>
#include <unordered_map>
#include <typeindex>        // 用于类型索引
#include <utility>          // 用于完美转发
#include <spdlog/spdlog.h>


namespace engine::core
{
    class Context;
}

namespace engine::object
{

    /**
     * @brief 游戏对象类，负责管理游戏对象的组件。
     *
     * 该类管理游戏对象的组件，并提供添加、获取、检查和移除组件的功能。
     * 它还提供更新和渲染游戏对象的方法。
     */
    class GameObject final
    {
    private:
        std::string name_;      //名称
        std::string tag_;       //标签
        std::unordered_map<std::type_index, std::unique_ptr<engine::component::Component>> components_;     //所有组件
        bool need_remove_ = false;


    public:
        //构造函数，名字和标签的默认值为空
        GameObject(const std::string& name = "", const std::string& tag = "");

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        GameObject(const GameObject&&) = delete;
        GameObject& operator=(GameObject&&) = delete;

        //  setters and getters
        void setName(const std::string& name) { name_ = name; }                 ///< @brief 设置名称
        const std::string& getName() const { return name_; }                    ///< @brief 获取名称
        void setTag(const std::string& tag) { tag_ = tag; }                     ///< @brief 设置标签
        const std::string& getTag() const { return tag_; }                      ///< @brief 获取标签
        void setNeedRemove(bool need_remove) { need_remove_ = need_remove; }    ///< @brief 设置是否需要删除
        bool isNeedRemove() const { return need_remove_; }                      ///< @brief 获取是否需要删除


        /**
         *@brief 添加组件（并且每同一种组件每一个对象只会拥有一个），这里会完成组件的init，由于每个组件构造时需要传入的参数不同，所以这里用模板可选参数的形式来达到这一目的
         *
         * @tparam T 组件的类型
         * @tparam Args 组件构造函数所需要的所有参数类型
         * @param args 组件构造函数所需要的所有参数
         * @return T* 组件指针
         */
        template <typename T, typename... Args>
        T* addComponent(Args&&... args)
        {
            //检测组件是否合法
            /*  static_assert(condition, message)：静态断言，在编译期检测，无任何性能影响 */
            /* std::is_base_of<Base, Derived>::value -- 判断 Base 类型是否是 Derived 类型的基类 */
            static_assert(std::is_base_of<engine::component::Component, T>::value, "T必须继承自Component。");

            //获取类型标识
            /* typeid(T) -- 用于获取一个表达式或类型的运行时类型信息 (RTTI), 返回 std::type_info& */
            /* std::type_index -- 针对std::type_info对象的包装器，主要设计用来作为关联容器（如 std::map）的键。*/
            auto type_index = std::type_index(typeid(T));


            //如果组件已经存在
            if (hasComponent<T>())
            {
                return getComponent<T>();
            }

            //不存在则添加组件
            /* std::forward -- 用于实现完美转发。           传递多个参数的时候使用...标识 */
            auto new_component = std::make_unique<T>(std::forward<Args>(args)...);
            T* ptr = new_component.get();             //先获取指针
            new_component->setOwner(this);            //设置拥有者
            components_[type_index] = std::move(new_component);     //移动组件，实际上是把new_component设置为右值，因此这句话之后new_component就不存在了
            ptr->init();                              //初始化组件，不可以用new_component
            spdlog::debug("成功添加component:{} {}", name_, typeid(T).name());
            return ptr;



        }



        template <typename T>
        T* getComponent() const
        {
            static_assert(std::is_base_of<engine::component::Component, T>::value, "T必须继承自Component");
            auto type_index = std::type_index(typeid(T));
            auto it = components_.find(type_index);
            if (it != components_.end())
            {
                return static_cast<T*>(it->second.get());
            }


        }


        /**
         *@brief 检查组件是否已经存在
         *
         * @tparam T 组件类型
         * @return true 存在
         * @return false 不不在
         */
        template <typename T>
        bool hasComponent() const
        {
            static_assert(std::is_base_of<engine::component::Component, T>::value, "T 必须继承自Component");
            return components_.contains(std::type_index(typeid(T)));
        }

        /**
         *@brief 移除组件
         *
         * @tparam T 组件类型
         */
        template <typename T>
        void removeComponent()
        {
            static_assert(std::is_base_of<engine::component::Component, T>::value, "T 必须继承自Component");
            auto type_index = std::type_index(typeid(T));
            auto it = components_.find(type_index);
            if (it != components_.end())
            {
                it->second->clean();
                components_.erase(it);
            }


        }



        // 关键循环函数
        void update(float delta_time, engine::core::Context& context);                ///< @brief 更新所有组件
        void render(engine::core::Context& context);                                ///< @brief 渲染所有组件
        void clean();                                 ///< @brief 清理所有组件
        void handleInput(engine::core::Context& context);                           ///< @brief 处理输入



    };



}