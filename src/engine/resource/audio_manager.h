#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <SDL3_mixer/SDL_mixer.h>

namespace engine::resource
{

    class AudioManager
    {
        friend class ResourceManager;


    private:
        //Mix_Chunk的自定义删除器
        struct SDLMixChunkDeleter
        {
            void operator()(Mix_Chunk* chunk) const
            {
                if (chunk)
                {
                    Mix_FreeChunk(chunk);
                }
            }
        };


        //Mix_Music的自定义删除器
        struct SDLMixMusicDeleter
        {
            void operator()(Mix_Music* music) const
            {
                if (music)
                {
                    Mix_FreeMusic(music);
                }
            }
        };


        //音效存储
        std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, SDLMixChunkDeleter>> sounds_;
        //音乐存储
        std::unordered_map<std::string, std::unique_ptr<Mix_Music, SDLMixMusicDeleter>> music_;


    public:

        /**
         *@brief Construct a new Audio Manager object
         *@throws std::runtime_error 如果SDL_Mixer初始化或打开音频失效
         *
         */
        AudioManager();


        ~AudioManager();    //  与TextureManager不同，这里需要手动Quit，所以必须要有析构函数

        // 当前设计中，我们只需要一个AudioManager，所有权不变，所以不需要拷贝、移动相关构造及赋值运算符
        AudioManager(const AudioManager&) = delete;
        AudioManager& operator=(const AudioManager&) = delete;
        AudioManager(AudioManager&&) = delete;
        AudioManager& operator=(AudioManager&&) = delete;


    private:  // 仅供 ResourceManager 访问的方法

        Mix_Chunk* loadSound(const std::string& file_path);     ///< @brief 从文件路径加载音效
        Mix_Chunk* getSound(const std::string& file_path);      ///< @brief 尝试获取已加载音效的指针，如果未加载则尝试加载
        void unloadSound(const std::string& file_path);         ///< @brief 卸载指定的音效资源
        void clearSounds();                                      ///< @brief 清空所有音效资源

        Mix_Music* loadMusic(const std::string& file_path);     ///< @brief 从文件路径加载音乐
        Mix_Music* getMusic(const std::string& file_path);      ///< @brief 尝试获取已加载音乐的指针，如果未加载则尝试加载
        void unloadMusic(const std::string& file_path);         ///< @brief 卸载指定的音乐资源
        void clearMusic();                                      ///< @brief 清空所有音乐资源

        void clearAudio();

    };

}