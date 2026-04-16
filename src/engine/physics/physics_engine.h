#pragma once
#include "../utils/math.h"
#include <vector>
#include <utility>  // for std::pair
#include <optional>
#include <glm/vec2.hpp>

namespace engine::component
{
    class PhysicsComponent;
    class TileLayerComponent;
    enum class TileType;
    class TrailComponent;
}
namespace engine::object
{
    class GameObject;
}

namespace engine::physics
{

    /**
     * @brief 负责管理和模拟物理行为及碰撞检测。
     */
    class PhysicsEngine
    {
    private:
        std::vector<engine::component::TrailComponent*> Trails;             //存储注册的拖尾组件
        std::vector<engine::component::PhysicsComponent*> components_;      //存储注册的物理组件
        std::vector<engine::component::TileLayerComponent*> collision_tile_layers_;  //注册的碰撞瓦片容器
        glm::vec2 gravity_ = { 0.0f,980.0f };           // 默认重力值(像素/秒^2,相当于100像素对应1m)
        glm::vec2 trailgravity = glm::vec2(0.0f, 200.0f);//用用来表示头发收到的重力速度（像素/秒），设小一些看上去更像飘起来
        float max_speed_ = 500.9f;                      //最大速度(像素/秒)
        std::optional<engine::utils::Rect> world_bounds_;//世界变价

        // 存储本帧发生碰撞的碰撞对(update时清空)
        std::vector<std::pair<engine::object::GameObject*, engine::object::GameObject*>> collision_pairs_;
        /// @brief 存储本帧发生的瓦片触发事件 (GameObject*, 触发的瓦片类型, 每次 update 开始时清空)
        std::vector<std::pair<engine::object::GameObject*, engine::component::TileType>> tile_trigger_events_;

    public:
        PhysicsEngine() = default;

        //禁止移动和拷贝
        PhysicsEngine(const PhysicsEngine&) = delete;
        PhysicsEngine& operator=(const PhysicsEngine&&) = delete;
        PhysicsEngine(const PhysicsEngine&&) = delete;
        PhysicsEngine& operator=(PhysicsEngine&&) = delete;


        void registerComponent(engine::component::PhysicsComponent* component);     //注销物理组件
        void unregisterComponent(engine::component::PhysicsComponent* component);   //注册物理组件
        void registerCollisionLayer(engine::component::TileLayerComponent* layer);  //注册用于碰撞检测的TileLayerComonent
        void unregisterCollisionLayer(engine::component::TileLayerComponent* layer);//注销用于碰撞检测的TileLayerComonent
        void registerTrail(engine::component::TrailComponent* Trail);              //注册拖尾组件
        void unregisterTrail(engine::component::TrailComponent* Trail);            //注销拖尾组件

        /**
         *@brief 核心循环，会更新所有注册的物理组件的状态，lesson15只考虑重力
         *  并且更新完之后清楚当前所有力，每帧重新考虑所有受到的力
         *
         * @param delta_time 单位时间
         */
        void update(float delta_time);


        // setter and getter
        void setGravity(const glm::vec2& gravity) { gravity_ = gravity; }   ///< @brief 设置全局重力加速度
        const glm::vec2& getGravity() const { return gravity_; }            ///< @brief 获取当前的全局重力加速度
        void setMaxSpeed(float max_speed) { max_speed_ = max_speed; }       ///< @brief 设置最大速度
        float getMaxSpeed() const { return max_speed_; }                    ///< @brief 获取当前的最大速度
        void setWorldBounds(const engine::utils::Rect& world_bounds) { world_bounds_ = world_bounds; }   ///< @brief 设置世界边界
        const std::optional<engine::utils::Rect>& getWorldBounds() const { return world_bounds_; }       ///< @brief 获取世界边界
        /// @brief 获取本帧检测到的所有 GameObject 碰撞对。(此列表在每次 update 开始时清空)
        const std::vector<std::pair<engine::object::GameObject*, engine::object::GameObject*>>& getCollisionPairs() const {
            return collision_pairs_;
        };
        /// @brief 获取本帧检测到的所有瓦片触发事件。(此列表在每次 update 开始时清空)
        const std::vector<std::pair<engine::object::GameObject*, engine::component::TileType>>& getTileTriggerEvents() const {
            return tile_trigger_events_;
        };


    private:
        /**
         *@brief 检测并处理对象之间的碰撞，并记录需要游戏逻辑处理的碰撞对。
         *
         */
        void checkObjectCollisions();
        /**
         *@brief 检测并处理游戏对象和瓦片之间的碰撞,并且对于瓦片层不管是圆形还是矩形都只需要检测其最小AABB包围盒就可以了
         * @note 对包围盒的四个角的点检测而不是整个包围盒，并且逻辑不完善，
         * 比如这里默认认为需要检测的对象长度为1~2个瓦片宽度，当对象长度大于2个瓦片长度时，
         * 如果瓦片从物体正中间产生碰撞，则会出现对象直接穿过瓦片的现象，
         * 目前想到的解决方法是在AABB包围盒的每一条变上添加一定量的点，使得每个相邻点间隔小于或等于瓦片宽度
         * @param pc 物理组件
         * @param delta_time 单位时间
         */
        void resolveTileCollision(engine::component::PhysicsComponent* pc, float delta_time);

        /// @brief 处理可移动物体与SOLID物体的碰撞。
        void resolveSolidObjectCollisions(engine::object::GameObject* move_obj, engine::object::GameObject* solid_obj);
        void applyWorldBounds(engine::component::PhysicsComponent* pc);     //限制世界边界

        /**
         * @brief 根据瓦片类型和指定宽度x坐标，计算瓦片上对应y坐标。
         * @param width 从瓦片左侧起算的宽度。
         * @param type 瓦片类型。
         * @param tile_size 瓦片尺寸。
         * @return 瓦片上对应高度（从瓦片下侧起算）。
         */
        float getTileHeightAtWidth(float width, engine::component::TileType type, glm::vec2 tile_size);

        /**
         * @brief 检测所有游戏对象与瓦片层的触发器类型瓦片碰撞，并记录触发事件。(位移处理完毕后再调用)
         */
        void checkTileTriggers();
    };

}