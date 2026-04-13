#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <queue>
#include <SDL3/SDL_render.h>
#include <glm/vec2.hpp>

namespace engine::core
{
    class Config;
}

namespace engine::input
{

    /**
     *@brief 按键状态的枚举
     *
     */
    enum class ActionState
    {
        INACTIVE,       //动作未激活
        PRESSED_THIS_FRAME,     //在这一帧按下
        HELD_DOWN,      //持续按下
        RELEASED_THIS_FRAME,     //这一帧抬起

    };

    /**
     * @brief 输入管理器类，负责处理输入事件(也包括退出游戏）和动作状态。
     *
     * 该类管理输入事件，将按键转换为动作状态，并提供查询动作状态的功能。
     * 它还处理鼠标位置的逻辑坐标转换。
     */
    class InputManager final
    {
    private:
        SDL_Renderer* sdl_renderer_;        //用于获取逻辑坐标
        std::unordered_map<std::string, std::vector<std::string>> actions_to_keyname_map_;      //存储动作到按键名的映射
        std::unordered_map<std::variant<SDL_Scancode, Uint32>, std::vector<std::string>> input_to_actions_map_;     //从输入到关联的动作名称列表
        std::unordered_map<std::string, ActionState> action_states_;        //存储每个动作的当前状态
        std::queue<std::string> input_buffer_;                  //存储预输入的指令（预输入指令缓存器）

        bool should_quit_ = false;       //退出标志
        glm::vec2 mouse_position_;        //鼠标的位置
        float input_buffer_pop_interval_ = 0.2f;   //预输入指令缓存器的弹出时间
        float input_buffer_pop_timer_ = 0.0f;      //预输入指令缓存器的弹出计时器



    public:

        /**
         *@brief Construct a new Input Manager object
         *
         * @param sdl_renderer 指向渲染器的指针
         * @param config 配置对象
         * @throws std::runtime_error 如果任意一个指针为空
         */
        InputManager(SDL_Renderer* sdl_renderer, const engine::core::Config* config);


        void update(float delta_time);      //更新输入状态

        //动作状态检查
        bool isActionDown(const std::string& action_name) const;        //动作是否触发(持续按下或按下)
        bool isActionPressed(const std::string& action_name) const;      //动作是否在本帧按下
        bool isActionReleased(const std::string& action_name) const;    //动作是否在本帧释放

        //指令缓冲队列操作函数
        std::string getInputBufferFront() const;      //获取输入缓存队列的第一个元素
        void popInputBufferFront() { input_buffer_.pop(); }             //删除输入缓存队列的第一个元素
        void pushInputBufferBack(std::string action_name);          //给缓存队列添加元素
        void clearInputBuffer();        //将输入缓存队列置空
        bool isInputBufferEmpty() { return input_buffer_.empty(); }


        bool shouldQuit() const;                    //查询退出状态
        void setShouldQuit(bool should_quit);       //设置退出状态

        glm::vec2 getMousePosition() const;         //获取鼠标位置（屏幕坐标）
        glm::vec2 getLogicalMousePosition() const;  //获取鼠标位置（逻辑坐标)       
        void setInputBufferPopInterval(float interval) { input_buffer_pop_interval_ = interval; }       //设置缓冲时间
        float getInputBufferPopInterval() const { return input_buffer_pop_interval_; }                  //获取缓冲时间


    private:
        void processEvent(const SDL_Event& event);      //处理SDL事件（将按键转化为动作状态）
        void initializeMappings(const engine::core::Config* config);        //根据Config初始化映射表
        void updateActionState(const std::string& action_name, bool is_input_active, bool is_repeat_event);     //辅助更新动作状态
        SDL_Scancode scancodeFromString(const std::string& key_name);       //将字符串键名转化为SDL_Scancode按键
        Uint32 mouseButtonFromString(const std::string& button_name);   //将字符串键名转化为鼠标的SDL_Button按键

    };



}