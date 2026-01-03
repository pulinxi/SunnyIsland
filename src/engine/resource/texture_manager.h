#pragma once
#include <memory>
#include <stdexcept>        //用于std:runtime_error
#include <string>
#include <unordered_map>    
#include <SDL3/SDL_render.h>        //用于SDL_Renderer和SDL_Texture
#include <glm/glm.hpp>


namespace engine::resource
{


    /**
     *@brief 管理SDL_Texture资源的加载存储检索
     *
     * 在构建时初始化，使用文件路径作为键，确保纹理只加载一次正确释放
     * 依赖与一个有效的SDL_Renderer
     */
    class TextureManager {
        friend class ResourceManager;

    private:


        //自定义删除器，用于智能指针管理
        struct SDLTextureDeleter {
            void operator()(SDL_Texture* texture) const {
                if (texture)
                {
                    //也可以放一些其他操作
                    SDL_DestroyTexture(texture);
                }
            }
        };

        //存储文件路径和指向管理纹理的unique_ptr的映射
        std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> textures_;

        SDL_Renderer* renderer_ = nullptr;





    public:

        /**
         *@brief Construct a new Texture Manager object
         *
         * @param renderer 不能为空
         *
         *@throws std::runtime_error 如果renderer为空则初始化失败
         */
        explicit TextureManager(SDL_Renderer* renderer);

        //和ResourceManger一样删除
        TextureManager(const TextureManager&) = delete;
        TextureManager operator=(const TextureManager&) = delete;
        TextureManager(TextureManager&&) = delete;
        TextureManager& operator=(TextureManager&&) = delete;


    private://只允许本类函数以及友元访问以下方法

        SDL_Texture* loadTexture(const std::string& file_path);
        SDL_Texture* getTexture(const std::string& file_path);
        void unloadTexture(const std::string& file_path);
        glm::vec2 getTextureSize(const std::string& file_path);
        void clearTextures();





    };



}