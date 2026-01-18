#pragma once
#include <memory>
#include <string>
#include <vector>

namespace engine::core
{
    class Context;
}



namespace engine::scene
{
    class Scene;


    /**
     *@brief 管理游戏中的场景栈，处理场景的切换和生命周期
     *
     */
    class SceneManager final
    {

    private:
        engine::core::Context& context_;        //引擎上下文
        std::vector<std::unique_ptr<Scene>> scene_stack_;   //场景栈

        enum class PendingAction { None, Push, Pop, Replace };      //场景所拥有的所有处理可能
        PendingAction pending_action_ = PendingAction::None;        //当前所要执行的场景动作
        std::unique_ptr<Scene> pending_scene_;                      //待处理场景



    public:

        explicit SceneManager(engine::core::Context& context);
        ~SceneManager();


        // 禁止拷贝和移动
        SceneManager(const SceneManager&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;
        SceneManager(SceneManager&&) = delete;
        SceneManager& operator=(SceneManager&&) = delete;


        void requestPushScene(std::unique_ptr<Scene>&& scene);      //请求压入一个场景
        void requestPopScene();                                     //请求弹出栈顶场景
        void requestReplaceScene(std::unique_ptr<Scene>&& scene);   //请求替换栈顶场景


        // getters
        Scene* getCurrentScene() const;                                 ///< @brief 获取当前活动场景（栈顶场景）的指针。
        engine::core::Context& getContext() const { return context_; }  ///< @brief 获取引擎上下文引用。

        // 核心循环函数
        void update(float delta_time);
        void render();
        void handleInput();
        void close();


    private:
        void processPendingActions();               //处理挂起的场景


        //直接切换场景
        void pushScene(std::unique_ptr<Scene>&& scene);     //将一个新场景压入栈
        void popScene();                                    //溢出栈顶场景
        void replaceScene(std::unique_ptr<Scene>&& scene);  //清理场景栈所有场景，将此场景设为栈顶场景。



    };


}



