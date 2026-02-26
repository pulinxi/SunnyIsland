#pragma once
#include "../../engine/scene/scene.h"
#include <memory>
#include <glm/vec2.hpp>

// 前置声明
namespace engine::object {
    class GameObject;
}

namespace game::scene {

    /**
     * @brief 主要的游戏场景，包含玩家、敌人、关卡元素等。
     */
    class GameScene final : public engine::scene::Scene {

        engine::object::GameObject* player_ = nullptr;  ///< @brief 保存测试对象的指针，方便访问

    public:
        GameScene(std::string name, engine::core::Context& context, engine::scene::SceneManager& scene_manager);

        // 覆盖场景基类的核心方法
        void init() override;
        void update(float delta_time) override;
        void render() override;
        void handleInput() override;
        void clean() override;

    private:
        [[nodiscard]] bool initLevel();               ///< @brief 初始化关卡
        [[nodiscard]] bool initPlayer();              ///< @brief 初始化玩家
        [[nodiscard]] bool initEnemyAndItem();        ///< @brief 初始化敌人和道具

        void handleObjectCollisions();      //处理游戏对象间的逻辑碰撞
        void PlayerVSEnemyCollision(engine::object::GameObject* player, engine::object::GameObject* enemy); //玩家与敌人的碰撞
        void PlayerVSItemCollision(engine::object::GameObject* player, engine::object::GameObject* item);  //玩家与物体的碰撞
        void handleTileTriggers();                  ///< @brief 处理瓦片触发事件（从PhysicsEngine获取信息）

        void toNextLevel(engine::object::GameObject* trigger);          ///< @brief 进入下一个关卡

        /// @brief 根据关卡名称获取对应的地图文件路径
        std::string levelNameToPath(const std::string& level_name) const { return "assets/maps/" + level_name + ".tmj"; }

        /**
         *@brief Create a Effect object
         *
         * @param center_pos 特效中心位置
         * @param tag 特效标签
         */
        void createEffect(const glm::vec2& center_pos, const std::string& tag);


    };

} // namespace game::scene