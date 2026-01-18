#pragma once
#include <vector>
#include <memory>
#include <string>

namespace  engine::core
{
    class Context;
}

namespace engine::render
{
    class Renderer;
    class Camera;
}

namespace engine::input
{
    class InputManager;
}

namespace engine::object
{
    class GameObject;
}

namespace engine::scene
{

    class SceneManager;


    /**
     *@brief 场景基类，负责管理场景中的游戏对象和场景声明周期
     *
     * 包含一组游戏对象，并提供更新、渲染、处理输入和清理的接口
     * 派生类应实现具体的场景逻辑
     */
    class Scene
    {
    protected:
        std::string scene_name_;        //场景名称
        engine::core::Context& context_;        //上下文引用（隐式、构造时传入）
        engine::scene::SceneManager& scene_manager_;        //场景管理器
        bool is_initialized_ = false;           //场景是否已经初始化
        std::vector<std::unique_ptr<engine::object::GameObject>> game_objects_;     //存放当前场景的所有游戏对象
        std::vector<std::unique_ptr<engine::object::GameObject>> pending_additions_;     //存放当前场景需要添加的的所有游戏对象


    public:
        /**
         *@brief Construct a new Scene object
         *
         * @param name 场景名称
         * @param context 场景上下文
         * @param scene_manager 场景管理
         */
        Scene(std::string name, engine::core::Context& context, engine::scene::SceneManager& scene_manager);


        /**
         *@brief Destroy the Scene object
         *
         * 1.基类必须声明虚析构函数才能让派生类的析构函数被正确调用
         * 2.析构函数的定义i必须写在cpp中，不然要引入GameObject头文件
         * 2.1（因为这个类内部有GameObject的智能指针，析构函数调用时需要知道其内部实现以找到删除的方法，所以要不就引入gameobject头文件让这个类的析构函数可以在头文件中找到gameobject的定义，要不就把析构函数放在cpp文件中，在cpp文件中引入gameobject头文件
         * 2.2显然第二种情况更加复合我们的最小引入原则
         */
        virtual ~Scene();

        //禁止拷贝和移动构造
        Scene(const Scene&) = delete;
        Scene& operator=(const Scene&) = delete;
        Scene(Scene&&) = delete;
        Scene& operator=(const Scene&&) = delete;

        virtual void init();                        ///< @brief 初始化场景。
        virtual void update(float delta_time);      ///< @brief 更新场景。
        virtual void render();                      ///< @brief 渲染场景。
        virtual void handleInput();                 ///< @brief 处理输入。
        virtual void clean();                       ///< @brief 清理场景。


        //直接向场景中添加一个游戏对象。（初始化可用，游戏进行中使用则不安全）（&&代表右值，配合std::move()使用来移动对象而非拷贝，可以节省空间）
        virtual void addGameObject(std::unique_ptr<engine::object::GameObject>&& game_object);

        //安全的添加游戏对象（添加到pending_addtions_中）
        virtual void safeAddGameObject(std::unique_ptr<engine::object::GameObject>&& game_object);

        //直接从一个场景中移除对象（一般不使用，但是保留实现逻辑）
        virtual void removeGameObject(engine::object::GameObject* game_object_ptr);

        //安全的移除游戏对象（实际上是把对象的need_remove设置为true）
        virtual void safeRemoveGameObject(engine::object::GameObject* game_object_ptr);

        //获取场景中的游戏对象容器
        const std::vector<std::unique_ptr<engine::object::GameObject>>& getGameObjects() const { return game_objects_; }

        //根据名称找到第一个匹配的对象
        engine::object::GameObject* findGameObjectByName(const std::string& name) const;

        // getters and setters
        void setName(const std::string& name) { scene_name_ = name; }               ///< @brief 设置场景名称
        const std::string& getName() const { return scene_name_; }                  ///< @brief 获取场景名称
        void setInitialized(bool initialized) { is_initialized_ = initialized; }    ///< @brief 设置场景是否已初始化
        bool isInitialized() const { return is_initialized_; }                      ///< @brief 获取场景是否已初始化

        engine::core::Context& getContext() const { return context_; }                  ///< @brief 获取上下文引用
        engine::scene::SceneManager& getSceneManager() const { return scene_manager_; } ///< @brief 获取场景管理器引用


    protected:
        //处理待添加的游戏对象，每轮更新的最后添加
        void processPendintAdditions();

    };


}