#include "ResourceManager.hpp"

//Devuelve una instancia de Manager
ResourceManager& ResourceManager::Get()
{
    static ResourceManager instance;
    return instance;
}

Texture2D ResourceManager::GetTexture(const std::string& path)
{
    auto it = textures.find(path);
    if (it != textures.end())//No ha llegado al final, por lo que ha encontrado
    {
        return it->second;
    }

    //Si no encuentra, lo guarda
    Texture2D texture = LoadTexture(path.c_str());

    if (texture.id == 0)
    {
        TraceLog(LOG_WARNING,
                 "ResourceManager: no se pudo cargar la textura %s",
                 path.c_str());
    }

    textures[path] = texture;
    return texture;
}

//Misma lógica que texture
Font ResourceManager::GetFont(const std::string& path, int size)
{
    const std::string key = path + ":" + std::to_string(size);

    auto it = fonts.find(key);
    if (it != fonts.end())
    {
        return it->second;
    }

    Font font = LoadFontEx(path.c_str(), size, nullptr, 0);

    if (font.texture.id == 0)
    {
        TraceLog(LOG_WARNING,
                 "ResourceManager: no se pudo cargar la fuente %s",
                 path.c_str());
        font = GetFontDefault();
    }
    else
    {
        SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    }

    fonts[key] = font;
    return font;
}

// Libera todas las texturas y fuentes cargadas y vacía las cachés.
void ResourceManager::UnloadAll()
{
    for (auto& entry : textures)
    {
        if (entry.second.id > 0)
        {
            UnloadTexture(entry.second);
        }
    }
    textures.clear();

    const Font defaultFont = GetFontDefault();

    for (auto& entry : fonts)
    {
        if (entry.second.texture.id != 0 &&
            entry.second.texture.id != defaultFont.texture.id)
        {
            UnloadFont(entry.second);
        }
    }
    fonts.clear();
}

ResourceManager::~ResourceManager() = default;