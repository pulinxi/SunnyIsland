#pragma once
#include <SDL3/SDL_rect.h>
#include <optional>
#include <string>


namespace engine::render
{


    /**
     *
     *@brief 表示要绘制的精灵图数据
     *
     * 包含纹理标识符、要绘制的纹理部分、以及旋转状态
     * 位置、缩放、旋转有外部标识
     * 渲染工作由Renderer类完成，传入Sprite类对象
     *
     */
    class Sprite final
    {
    private:
        std::string texture_id_;    //纹理资源标识符
        std::optional<SDL_FRect> source_rect_;       //记录纹理要渲染的部分（用std::optional可以在使用时指定矩形区域或者不指定，分别代表渲染特定区域和渲染整个图片，如果不用std::optional就需要自己设定规则来区分渲染区域，比如规定全部是0的SDL_Rect是全部渲染,具体使用方法可以看“知识点.md"的06部分，打个比方如果这个source_rect是std::nullopt就可以用来代表渲染整个纹理）
        bool is_flipped_ = false;       //是否水平翻转




    public:

        /**
         *@brief Construct a new Sprite object
         *
         * @param texture_id 纹理资源标识符，不应为空
         * @param source_rect 可选的矩形，表示要渲染纹理的哪一部分,如果是std::nullopt,代表渲染整个纹理
         * @param is_flipped 是否水平旋转
         */
        Sprite(const std::string& texture_id, const std::optional<SDL_FRect>& source_rect = std::nullopt, bool is_flipped = false)
            :texture_id_(texture_id),
            source_rect_(source_rect),
            is_flipped_(is_flipped)
        {
        }



        // --- getters and setters ---
        const std::string& getTextureId() const { return texture_id_; }                                     ///< @brief 获取纹理 ID
        const std::optional<SDL_FRect>& getSourceRect() const { return source_rect_; }                      ///< @brief 获取源矩形 (如果使用整个纹理则为 std::nullopt)
        bool isFlipped() const { return is_flipped_; }                                                      ///< @brief 获取是否水平翻转

        void setTextureId(const std::string& texture_id) { texture_id_ = texture_id; }                      ///< @brief 设置纹理 ID
        void setSourceRect(const std::optional<SDL_FRect>& source_rect) { source_rect_ = source_rect; }     ///< @brief 设置源矩形 (如果使用整个纹理则为 std::nullopt)
        void setFlipped(bool flipped) { is_flipped_ = flipped; }                                            ///< @brief 设置是否水平翻转



    };



}