#pragma once
#include <memory>
#include <forward_list>
#include <utility>
#include <spdlog/spdlog.h>
#include "../core/context.h"


namespace engine::pool
{
    /**
     *@brief 对象池类
     *
     * @tparam T 对象类型
     */
    template <typename T>
    class ObjectPool
    {
    private:
        std::forward_list<std::unique_ptr<T>> object_container_;

    public:
        ObjectPool() {}
        ~ObjectPool() {}

        std::unique_ptr<T> getObject()
        {
            if (object_container_.empty())
            {
                spdlog::error("内存池为空");
                return nullptr;
            }
            auto p = std::move(object_container_.front());
            object_container_.pop_front();
            spdlog::warn("get目前对象池中共有{}个对象", std::distance(object_container_.begin(), object_container_.end()));
            return std::move(p);
        }

        void releaseObject(std::unique_ptr<T> obj, engine::core::Context& context)
        {
            obj->resetComponent(context);
            object_container_.push_front(std::move(obj));
            spdlog::warn("release目前对象池中共有{}个对象", std::distance(object_container_.begin(), object_container_.end()));
        }

        [[nodiscard]] bool addObject(std::unique_ptr<T> prototype)
        {
            object_container_.push_front(std::move(prototype));
            spdlog::warn("add目前对象池中共有{}个对象", std::distance(object_container_.begin(), object_container_.end()));
            return true;
        }
    };
}