#pragma once

#include <raylib.h>
#include <string>
#include <unordered_map>

class ResourceManager
{
    public:
        static ResourceManager& Get();

        ResourceManager(const ResourceManager&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;

        Texture2D GetTexture(const std::string& path);
        Font GetFont(const std::string& path, int size);

        void UnloadAll();

    private:
        ResourceManager() = default;
        ~ResourceManager() = default;

        std::unordered_map<std::string, Texture2D> textures; //Clave: path, Valor: Texture 
        std::unordered_map<std::string, Font> fonts; //Clave: path:tamaño, Valor: Font
};