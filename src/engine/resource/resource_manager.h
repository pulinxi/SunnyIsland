#pragma once
#include <memory>       //用于std::unique_ptr
#include <string>
#include <glm/glm.hpp>



struct SDL_Renderer;
struct SDL_Texture;
struct Mix_Chunk;
struct Mix_Music;
struct TTF_Font;

namespace engine::resource
{


    //向前声明
    class TextureManager;
    class AudioManager;
    class FontManager;


    /**
     *@brief 作为访问各种资源的中央控制节点（外观模式Facade)
     * 在构造时初始化管理的子系统，构造失败会抛出异常
     */
    class ResourceManager
    {
    private:


        //使用unique_ptr 确保所有资源自动清理
        std::unique_ptr<TextureManager> texture_manager_;
        std::unique_ptr<AudioManager> audio_manager_;
        std::unique_ptr<FontManager> font_manager_;



    public:


        /**
         *@brief 构造并初始化
         *
         * @param renderer 不能为空
         */
        explicit ResourceManager(SDL_Renderer* renderer);   //explicit关键字防止隐士转化，对单一参数的构造函数有效


        ~ResourceManager(); //显示声明析构函数，这是为了让只能指针正确管理仅有向前声明的类，因为在析构函数内部需要知道智能指针所属的类的完整实现，以生成删除方式

        void clear();       //清空所有资源


        //当前设计中我们只需要一个ResourceManager,所有权不变所以不需要拷贝、移动、构造、赋值运算符函数
        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;

        //Texture
        SDL_Texture* loadTexture(const std::string& file_path);
        SDL_Texture* getTexture(const std::string& file_path);
        void unloadTexture(const std::string& file_path);
        glm::vec2 getTextureSize(const std::string& file_path);
        void clearTextures();


        //sound
        Mix_Chunk* loadSound(const std::string& file_path);
        Mix_Chunk* getSound(const std::string& file_path);
        void unloadSound(const std::string& file_path);
        void clearSounds();


        //music
        Mix_Music* loadMusic(const std::string& file_path);
        Mix_Music* getMusic(const std::string& file_path);
        void unloadMusic(const std::string& file_path);
        void clearMusic();

        //font
        TTF_Font* loadFont(const std::string& file_path, int point_size);
        TTF_Font* getFont(const std::string& file_path, int point_size);
        void unloadFont(const std::string& file_path, int point_size);
        void clearFonts();

    };



}