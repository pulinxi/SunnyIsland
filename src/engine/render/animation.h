#pragma once
#include <SDL3/SDL_rect.h>
#include <vector>
#include <string>

namespace engine::render
{
    struct AnimationFrame
    {
        SDL_FRect source_rect;     //纹理图集上此帧的区域
        float duration;            //这一帧的持续时间
    };

    /**
     *@brief 管理一系列动画帧
     *
     * 存储动画的帧、总时长、名称和循环行为
     */
    class Animation final
    {
    private:
        std::string name_;          //动画帧的名称
        std::vector<AnimationFrame> frames_;        //动画帧列表
        float total_duration_ = 0.0f;               //动画的总持续时间
        bool loop_ = true;                          //是否循环（默认是）

    public:
        /**
         *@brief Construct a new Animation object
         *
         * @param name 动画的名称
         * @param loop 动画是否应该循环播放
         */
        Animation(const std::string& name = "default", bool loop = true);
        ~Animation() = default;

        // 禁止拷贝和移动，因为 Animation 通常由管理器持有，不应随意拷贝
        Animation(const Animation&) = delete;
        Animation& operator=(const Animation&) = delete;
        Animation(Animation&&) = delete;
        Animation& operator=(Animation&&) = delete;

        /**
         *@brief 向动画添加一帧
         *
         * @param source_rect 纹理图集上此帧的区域
         * @param duration 此帧应显示的持续时间
         */
        void addFrame(const SDL_FRect& source_rect, float duration);

        /**
         *@brief 获取在给定时间点应该显示的动画帧
         *
         * @param time 当前时间（秒）。如果动画循环，则可以通过总持续时间返回播放到那一帧
         * @return const AnimationFrame& 对应时间点的动画帧
         */
        const AnimationFrame& getFrame(float time) const;

        // --- Setters and Getters ---
        const std::string& getName() const { return name_; }                        ///< @brief 获取动画名称。
        const std::vector<AnimationFrame>& getFrames() const { return frames_; }    ///< @brief 获取动画帧列表。
        size_t getFrameCount() const { return frames_.size(); }                     ///< @brief 获取帧数量。
        float getTotalDuration() const { return total_duration_; }                  ///< @brief 获取动画的总持续时间（秒）。
        bool isLooping() const { return loop_; }                                    ///< @brief 检查动画是否循环播放。
        bool isEmpty() const { return frames_.empty(); }                            ///< @brief 检查动画是否没有帧。

        void setName(const std::string& name) { name_ = name; }                     ///< @brief 设置动画名称。
        void setLooping(bool loop) { loop_ = loop; }


    };
}