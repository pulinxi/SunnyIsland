#pragma once
#include <optional>
#include <string>
#include <glm/vec2.hpp>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>


namespace engine::core
{
    /**
     *@brief 定义游戏可能处于的宏观状态
     *
     */
    enum class State
    {
        Title,          //标题界面
        Playing,        //正常游戏进行中
        Paused,         //游戏暂停(通常覆盖菜单界面)
        GameOver,       //游戏结束
        //还可以有更多状态
    };



    /**
     *@brief 管理和查询游戏的全局宏观状态
     *
     * 提供一个中心点来确定当前游戏处于哪个主要模式
     * 一遍其他系统（输入、渲染、更新）等可以相应的跳绳行为
     */
    class GameState final
    {
    private:
        SDL_Window* window_ = nullptr;          //用于获取窗口大小
        SDL_Renderer* renderer_ = nullptr;        //用于获取逻辑分辨率
        State current_state_ = State::Title;    //当前游戏状态


    public:
        /**
         *@brief 初始化游戏状态
         *
         * @param window sdl窗口，必须要有效值
         * @param render sdl渲染器，必须要有效值
         * @param initial_state 游戏初始状态，默认为Title
         */
        explicit GameState(SDL_Window* window, SDL_Renderer* render, State initial_state = State::Title);

        State getCurrentState() const { return current_state_; }
        void setState(State new_state);
        glm::vec2 getWindowSize() const;
        void setWindowSize(glm::vec2 new_size);
        glm::vec2 getLogicalSize() const;
        void setLogicalSize(glm::vec2 new_size);

        //便捷查询方法

        bool isInTitle() const { return current_state_ == State::Title; }
        bool isPlaying() const { return current_state_ == State::Playing; }
        bool isPaused() const { return current_state_ == State::Paused; }
        bool isGameOver() const { return current_state_ == State::GameOver; }


    };
}