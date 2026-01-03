#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>  //用于std::pair
#include <functional>  //用于std::hash
#include <SDL3_ttf/SDL_ttf.h>

namespace engine::resource
{

    //定义字体键路径和大小
    using FontKey = std::pair<std::string, int>;

    //FontKey 的自定义哈希函数,这样之后就可以调用FontKeyHash(FontKey& key)了
    struct FontKeyHash
    {
        std::size_t operator()(const FontKey& key) const
        {
            std::hash<std::string> string_hasher;
            std::hash<int> int_hasher;
            return string_hasher(key.first) ^ int_hasher(key.second);       //最常见的合并hash值的方法，按位异或
        }
    };

    /**
     * @brief 管理 SDL_ttf 字体资源（TTF_Font）。
     *
     * 提供字体的加载和缓存功能，通过文件路径和点大小来标识。
     * 构造失败会抛出异常。仅供 ResourceManager 内部使用。
     */
    class FontManager
    {
        friend class ResourceManager;

    private:
        //TTF_Font的自定义删除器
        struct SDLFontDeleter
        {
            void operator()(TTF_Font* font) const
            {
                if (font)
                {
                    TTF_CloseFont(font);
                }
            }
        };


        //字体存储器
        //unorder_map的键需要可以转化为哈希值，并且如果时基础数据类型，系统会自动转化
        //如果是自定义类型，系统无法自动转化，则需要传入第三个模板参数，该参数要记载将键转化为哈希值的逻辑
        std::unordered_map<FontKey, std::unique_ptr<TTF_Font, SDLFontDeleter>, FontKeyHash> fonts_;

    public:
        /**
         * @brief 构造函数。初始化 SDL_ttf。
         * @throws std::runtime_error 如果 SDL_ttf 初始化失败。
         */
        FontManager();

        ~FontManager();            ///< @brief 需要手动添加析构函数，清理资源并关闭 SDL_ttf。

        // 当前设计中，我们只需要一个FontManager，所有权不变，所以不需要拷贝、移动相关构造及赋值运算符
        FontManager(const FontManager&) = delete;
        FontManager& operator=(const FontManager&) = delete;
        FontManager(FontManager&&) = delete;
        FontManager& operator=(FontManager&&) = delete;

    private: // 仅由 ResourceManager（和内部）访问的方法

        TTF_Font* loadFont(const std::string& file_path, int point_size);     ///< @brief 从文件路径加载指定点大小的字体
        TTF_Font* getFont(const std::string& file_path, int point_size);      ///< @brief 尝试获取已加载字体的指针，如果未加载则尝试加载
        void unloadFont(const std::string& file_path, int point_size);        ///< @brief 卸载特定字体（通过路径和大小标识）
        void clearFonts();

    };



}