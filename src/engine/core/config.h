#pragma once
#include <string>
#include <vector>
#include <unordered_map>


#include <nlohmann/json_fwd.hpp>        //提供前向声明，相当于class json;和class nolhman_json;
//因为json和nolhman_json两个类是别名而不是真正的名字，所以要引入官方给的头文件来使得可以做到和我们使用前向声明一样的效果


namespace engine::core
{



    /**
     *@brief 管理应用程序的配置设置
     *
     * 提供配置项的默认值，并支持从json文件加载或保存配置
     * 加载失败或文件不存在将使用默认值
     */
    class Config final
    {
    public:
        //以下是一些默认配置
        //窗口
        std::string window_title_ = "SunnyLand";
        int window_width_ = 1280;
        int window_height_ = 720;
        bool window_resizable_ = true;
        //图形设置
        bool vsync_enabled_ = true;     //是否开启垂直同步
        //性能设置
        int target_fps_ = 144;      //帧率限制，0表示不限制
        //音频设置
        float music_volume_ = 0.5f;
        float sound_volume_ = 0.5f;
        //存储动作名称到SDL Scancode 名称的映射
        std::unordered_map<std::string, std::vector<std::string>> input_mappings_ =
        {
            //提供默认值防止配置文件缺失
            {"move_left", {"A","Left"}},
            {"move_right", {"D","Right"}},
            {"move_up", {"W","Up"}},
            {"move_down", {"S","Down"}},
            {"jump", {"K","Space"}},
            {"attack", {"J","MouseLeft"}},
            {"pause", {"P","Escape"}},
            {"dash", {"L","L"}},
            //也可以继续添加其他动作
        };

        explicit Config(const std::string& filepath);

        // 删除拷贝和移动语义
        Config(const Config&) = delete;
        Config& operator=(const Config&) = delete;
        Config(Config&&) = delete;
        Config& operator=(Config&&) = delete;

        /**
         *@brief 从指定的json加载配置
         *
         * @param filepath 文件路径
         * @return true 加载成功
         * @return false 加载失败
         */
        bool loadFromFile(const std::string& filepath);
        /**
         *@brief 将当前配置保存至指定的json文件
         *
         * @param filepath 文件路径
         * @return true 保存成功
         * @return false 保存失败
         */
        [[nodiscard]] bool saveFile(const std::string& filepath);


    private:
        void fromJson(const nlohmann::json& j);     //从json对象来设置成员变量配置
        nlohmann::ordered_json toJson() const;      //根据当前的成员变量配置来生成配置文件

    };

}

