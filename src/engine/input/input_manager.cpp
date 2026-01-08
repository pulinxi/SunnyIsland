#include "input_manager.h"
#include "../core/config.h"
#include <stdexcept>
#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>


namespace engine::input
{
    InputManager::InputManager(SDL_Renderer* sdl_renderer, const engine::core::Config* config) : sdl_renderer_(sdl_renderer)
    {
        if (!sdl_renderer_)
        {
            spdlog::error("输入管理器:SDL_Renderer 为空。");
            throw std::runtime_error("输入管理器:SDL_Renderer 为空。");
        }
        initializeMappings(config);
        //设置鼠标初始位置
        float x, y;
        SDL_GetMouseState(&x, &y);
        mouse_position_ = glm::vec2(x, y);
        spdlog::trace("鼠标初始位置:({} {})", mouse_position_.x, mouse_position_.y);

    }

    void InputManager::update()
    {
        //1根据上一帧的值更新默认动作状态
        for (auto& [action_name, state] : action_states_)
        {
            if (state == ActionState::PRESSED_THIS_FRAME)       //上一帧如果处于按下状态，这一帧又还没开始检测是否按下，那这一阵就是处于一直按下的状态
            {
                state = ActionState::HELD_DOWN;
            }
            else if (state == ActionState::RELEASED_THIS_FRAME)     //同理上一帧释放那这一帧就是空闲状态
            {
                state = ActionState::INACTIVE;
            }
        }

        //2更新完之处理sdl事件给下一帧用
        SDL_Event event;
        while (SDL_PollEvent(&event))   // 轮询直到所有事件都被处理！
        {
            processEvent(event);
        }
    }

    bool InputManager::isActionDown(const std::string& action_name) const {
        // C++17 引入的 “带有初始化语句的 if 语句”
        if (auto it = action_states_.find(action_name); it != action_states_.end()) {
            return it->second == ActionState::PRESSED_THIS_FRAME || it->second == ActionState::HELD_DOWN;
        }
        return false;
    }

    bool InputManager::isActionPressed(const std::string& action_name) const {
        if (auto it = action_states_.find(action_name); it != action_states_.end()) {
            return it->second == ActionState::PRESSED_THIS_FRAME;
        }
        return false;
    }

    bool InputManager::isActionReleased(const std::string& action_name) const {
        if (auto it = action_states_.find(action_name); it != action_states_.end()) {
            return it->second == ActionState::RELEASED_THIS_FRAME;
        }
        return false;
    }



    bool InputManager::shouldQuit() const {
        return should_quit_;
    }

    void InputManager::setShouldQuit(bool should_quit)
    {
        should_quit_ = should_quit;
    }

    glm::vec2 InputManager::getMousePosition() const
    {
        return mouse_position_;
    }

    glm::vec2 InputManager::getLogicalMousePosition() const
    {
        glm::vec2 logical_pos;
        //这个函数可以将某个坐标转化为渲染坐标（逻辑坐标）
        SDL_RenderCoordinatesFromWindow(sdl_renderer_, mouse_position_.x, mouse_position_.y, &logical_pos.x, &logical_pos.y);
        return logical_pos;
    }


    void InputManager::processEvent(const SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
        {
            SDL_Scancode scancode = event.key.scancode;
            bool is_down = event.key.down;
            bool is_repeat = event.key.repeat;

            if (auto it = input_to_actions_map_.find(scancode);it != input_to_actions_map_.end())
            {
                const std::vector<std::string>& associated_actions = it->second;
                for (const std::string& action_name : associated_actions)
                {
                    updateActionState(action_name, is_down, is_repeat);//更新动作的状态
                }
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
        {
            Uint32 button = event.button.button;
            bool is_down = event.button.down;//鼠标事件不用考虑repeat
            if (auto it = input_to_actions_map_.find(button);it != input_to_actions_map_.end())
            {
                const std::vector<std::string>& associated_actions = it->second;
                for (const std::string& action_name : associated_actions)
                {
                    updateActionState(action_name, is_down, false);
                }
            }
            //点击时更新鼠标位置
            mouse_position_ = { event.button.x,event.button.y };
            break;
        }
        case SDL_EVENT_MOUSE_MOTION:
            mouse_position_ = { event.button.x,event.button.y };
            break;
        case SDL_EVENT_QUIT:
            should_quit_ = true;
            break;
        default:
            break;
        }


    }

    void InputManager::initializeMappings(const engine::core::Config* config)
    {
        spdlog::trace("初始化输入映射...");
        if (!config)
        {
            spdlog::error("输入管理器：Config为空");
            throw std::runtime_error("输入管理器：Config为空");
        }
        actions_to_keyname_map_ = config->input_mappings_;
        input_to_actions_map_.clear();
        action_states_.clear();

        //如果配置中没有定义鼠标按钮动作就添加默认映射，用于UI
        if (actions_to_keyname_map_.find("MouseLeftClick") == actions_to_keyname_map_.end())
        {
            spdlog::debug("配置中没有定义MouseLeftClick动作，添加默认映射到MouseLeft");
            actions_to_keyname_map_["MouseLeftClick"] = { "MouseLeft" };
        }
        if (actions_to_keyname_map_.find("MouseRightClick") == actions_to_keyname_map_.end())
        {
            spdlog::debug("配置中没有定义MouseRightClick动作，添加默认映射到MouseRight");
            actions_to_keyname_map_["MouseRightClick"] = { "MouseRight" };
        }


        //遍历  动作->按键  映射以初始化action_states_和input_to_actions_map_
        for (const auto& [action_name, key_names] : actions_to_keyname_map_)
        {
            //将动作的状态初始化为INACTIVE
            action_states_[action_name] = ActionState::INACTIVE;
            spdlog::trace("映射动作:{}", action_name);
            //设置  按键->动作名称 的映射
            for (const std::string& key_name : key_names)
            {
                SDL_Scancode scancode = scancodeFromString(key_name);       //尝试根据按键名称获取scancode
                Uint32 mouse_button = mouseButtonFromString(key_name);      // 尝试根据按键名称获取鼠标按钮

                if (scancode != SDL_SCANCODE_UNKNOWN)       //如果scancode有效, 则将action添加到scancode_to_actions_map_中
                {
                    input_to_actions_map_[scancode].push_back(action_name);
                    spdlog::trace("映射按键: {} (Scancode: {}) 到动作: {}", key_name, static_cast<int>(scancode), action_name);

                }
                else if (mouse_button != 0)                 // 如果鼠标按钮有效,则将action添加到mouse_button_to_actions_map_中
                {
                    input_to_actions_map_[mouse_button].push_back(action_name);
                    spdlog::trace("  映射鼠标按钮: {} (Button ID: {}) 到动作: {}", key_name, static_cast<int>(mouse_button), action_name);
                }
                else
                {
                    spdlog::warn("输入映射警告: 未知键或按钮名称 '{}' 用于动作 '{}'.", key_name, action_name);
                }


            }

        }
        spdlog::trace("输入初始化映射完成");

    }

    void InputManager::updateActionState(const std::string& action_name, bool is_input_active, bool is_repeat_event)
    {
        auto it = action_states_.find(action_name);
        if (it == action_states_.end()) {
            spdlog::warn("尝试更新未注册的动作状态: {}", action_name);
            return;
        }

        if (is_input_active) { // 输入被激活 (按下)
            if (is_repeat_event) {
                it->second = ActionState::HELD_DOWN;
            }
            else {            // 非重复的按下事件
                it->second = ActionState::PRESSED_THIS_FRAME;
            }
        }
        else { // 输入被释放 (松开)
            it->second = ActionState::RELEASED_THIS_FRAME;
        }
    }

    SDL_Scancode InputManager::scancodeFromString(const std::string& key_name)
    {
        return SDL_GetScancodeFromName(key_name.c_str());//这个函数可以通过传入的ascii（名字）码来返回对应的按键
    }

    Uint32 InputManager::mouseButtonFromString(const std::string& button_name) {
        if (button_name == "MouseLeft") return SDL_BUTTON_LEFT;
        if (button_name == "MouseMiddle") return SDL_BUTTON_MIDDLE;
        if (button_name == "MouseRight") return SDL_BUTTON_RIGHT;
        // SDL 还定义了 SDL_BUTTON_X1 和 SDL_BUTTON_X2
        if (button_name == "MouseX1") return SDL_BUTTON_X1;
        if (button_name == "MouseX2") return SDL_BUTTON_X2;
        return 0; // 0 不是有效的按钮值，表示无效
    }

}