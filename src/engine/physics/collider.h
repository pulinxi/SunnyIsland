#pragma once
#include <glm/vec2.hpp>

namespace engine::physics
{
    /**
     *@brief 定义不同类型的碰撞器
     *
     */
    enum class ColliderType
    {
        NONE,
        AABB,
        CIRCLE,
        //未来可以添加其他类型
    };

    /**
     *@brief 碰撞器的抽象基类
     * 所有具体的碰撞器都应该继承这个类
     */
    class Collider
    {
    protected:
        glm::vec2 aabb_size_ = { 0.0f,0.0f };   //覆盖collider的最小包围盒的尺寸

    public:
        virtual ~Collider() = default;      //抽象基类需要把析构函数设为虚函数，这样才能在子类调用析构函数时正确删除基类数据
        virtual ColliderType getType() const = 0;       //获取碰撞器类型

        void setAABBSize(const glm::vec2& size) { aabb_size_ = size; }
        const glm::vec2& getAABBSize() const { return aabb_size_; }
    };

    /**
     *@brief 轴对齐包围盒(Axis-Aligned Bounding Box)碰撞器
     *
     */
    class AABBCollider final : public Collider
    {
    private:
        glm::vec2 size_ = { 0.0f,0.0f };

    public:
        /**
         *@brief Construct a new AABBCollider object
         *
         * @param size 包围盒的宽度盒高度
         */
        explicit AABBCollider(const glm::vec2& size) :size_(size) { setAABBSize(size_); }
        ~AABBCollider() override = default;

        // --- Getters and Setters ---
        ColliderType getType() const override { return ColliderType::AABB; }
        const glm::vec2& getSize() const { return size_; }
        void setSize(const glm::vec2& size) { size_ = size; }

    };

    /**
     * @brief 圆形碰撞器。
     */
    class CircleCollider final : public Collider {
    private:
        float radius_ = 0.0f;  ///< @brief 圆的半径。

    public:
        /**
         * @brief 构造函数。
         * @param radius 圆的半径。
         */
        explicit CircleCollider(float radius) : radius_(radius) { setAABBSize(glm::vec2(radius * 2.0f, radius * 2.0f)); }
        ~CircleCollider() override = default;

        // --- Getters and Setters ---
        ColliderType getType() const override { return ColliderType::CIRCLE; }
        float getRadius() const { return radius_; }
        void setRadius(float radius) { radius_ = radius; }
    };

}