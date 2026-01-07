#include "config.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>


namespace engine::core
{

    Config::Config(const std::string& filepath)
    {
        loadFromFile(filepath);
    }




    bool Config::loadFromFile(const std::string& filepath)
    {
        std::ifstream file(filepath);   //打开json文件
        if (!file.is_open())
        {
            spdlog::warn("配置文件 '{}' 找不到,系统将使用默认设置并创建默认配置文件。", filepath);
            if (!saveFile(filepath))
            {
                spdlog::error("无法创建默认配置文件 '{}' 。", filepath);
                return false;
            }
            return false;
        }

        try
        {
            nlohmann::json j;
            file >> j; //通过文件来生成json对象
            fromJson(j);
            spdlog::info("成功从 '{}' 加载配置。", filepath);
            return true;
        }
        catch (const std::exception& e)
        {
            spdlog::error("读取配置文件 '{}' 失败: {}。将使用默认配置。", filepath, e.what());
        }

        return false;
    }

    bool Config::saveFile(const std::string& filepath)
    {
        std::ofstream file(filepath);
        if (!file.is_open())
        {
            spdlog::error("无法打开配置文件'{}'进行写入。", filepath);
            return false;
        }
        try
        {
            nlohmann::ordered_json j = toJson();
            file << j.dump(4);
            spdlog::info("成功将配置保存到 '{}' 。", filepath);
            return true;
        }
        catch (const std::exception& e)
        {
            spdlog::error("写入配置文件'{}' 失败：{}。", filepath, e.what());
        }
        return false;
    }

    void Config::fromJson(const nlohmann::json& j)
    {
        //窗口
        if (j.contains("window"))
        {
            const auto& window_config = j["window"];//因为"window"实际上也是一个json对象，所以先把它取出来单独使用
            window_title_ = window_config.value("title", window_title_);//如果"title"存在则取"title"的值，否则取window_title_的默认值
            window_width_ = window_config.value("width", window_width_);
            window_height_ = window_config.value("height", window_height_);
            window_resizable_ = window_config.value("resizable", window_resizable_);
        }
        //图形
        if (j.contains("graphics")) {
            const auto& graphics_config = j["graphics"];
            vsync_enabled_ = graphics_config.value("vsync", vsync_enabled_);
        }
        //性能
        if (j.contains("performance")) {
            const auto& perf_config = j["performance"];
            target_fps_ = perf_config.value("target_fps", target_fps_);
            if (target_fps_ < 0) {
                spdlog::warn("目标 FPS 不能为负数。设置为 0（无限制）。");
                target_fps_ = 0;
            }
        }
        //音频
        if (j.contains("audio")) {
            const auto& audio_config = j["audio"];
            music_volume_ = audio_config.value("music_volume", music_volume_);
            sound_volume_ = audio_config.value("sound_volume", sound_volume_);
        }

        //input_mappings
        if (j.contains("input_mappings") && j["input_mappings"].is_object())//如果"input_mappings"存在并且是一个json对象，而不是具体的值
        {
            const auto& mappings_json = j["input_mappings"];//单独去除json对象"input_mappings"
            try
            {
                //直接尝试从json对象转化为map<string,vector<string>>
                std::unordered_map<std::string, std::vector<std::string>> input_mappings = mappings_json.get < std::unordered_map<std::string, std::vector<std::string>>>();
                //如果成功，就把input_mappings移动到input_mappings_
                input_mappings_ = std::move(input_mappings);//注意这是移动而不是复制
                spdlog::trace("成功从配置加载输入映射。");
            }
            catch (const std::exception& e)
            {
                spdlog::warn("配置加载警告：解析 'input_mappings' 时发生异常。使用默认映射。错误：{}", e.what());
            }
        }
        else
        {
            spdlog::trace("配置跟踪：未找到 'input_mappings' 部分或不是对象。使用头文件中定义的默认映射。");
        }

    }

    nlohmann::ordered_json Config::toJson() const {
        return nlohmann::ordered_json{
            {"window", {
                {"title", window_title_},
                {"width", window_width_},
                {"height", window_height_},
                {"resizable", window_resizable_}
            }},
            {"graphics", {
                {"vsync", vsync_enabled_}
            }},
            {"performance", {
                {"target_fps", target_fps_}
            }},
            {"audio", {
                {"music_volume", music_volume_},
                {"sound_volume", sound_volume_}
            }},
            {"input_mappings", input_mappings_}
        };
    }



}