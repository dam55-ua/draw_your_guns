#include "Map.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>

Map::Map(int screenWidth, int screenHeight, MapConfig config){
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->config = config;
}

bool Map::isInsideSpawnBase(float x) const
{
    for (const SpawnBase& base : spawnBases)
    {
        const Rectangle rect = base.getBase();
        if (x >= rect.x && x <= rect.x + rect.width)
            return true;
    }
    return false;
}

std::vector<int> Map::buildColumnEdges() const
{
    std::vector<int> columnEdges(config.terrainSegmentCount + 1);

    for (int i = 0; i <= config.terrainSegmentCount; ++i)
    {
        columnEdges[i] = i * screenWidth / config.terrainSegmentCount;
    }

    return columnEdges;
}

TerrainProfile Map::buildTerrainProfile() const
{
    TerrainProfile profile;

    //SpawnBases
    for (const SpawnBase& base : spawnBases)
    {
        const Rectangle rect = base.getBase();
        profile.addFlatSection(rect.x, rect.x + rect.width, config.baseHeightPercent);
    }

    // Anclas aleatorias repartidas por el ancho, fuera de las bases.
    for (int i = 1; i < config.terrainAnchorCount - 1; ++i)
    {
        const float x = static_cast<float>(i * screenWidth / (config.terrainAnchorCount - 1));
        if (!isInsideSpawnBase(x))
        {
            profile.addAnchor(x, GetRandomValue(config.minHeightPercent, config.maxHeightPercent));
        }
    }

    profile.sortAnchors();
    return profile;
}


Segment Map::makeSegment(int leftX, int rightX, const TerrainProfile& profile) const
{
    const float sampleX = (leftX + rightX) * 0.5f;   // centro de la columna

    // Altura en el centro, interpolada entre las anclas vecinas.
    int heightPercent = profile.samplePercent(sampleX);

    // Ruido pequeño, pero solo fuera de las plataformas de spawn.
    if (!isInsideSpawnBase(sampleX))
    {
        heightPercent += GetRandomValue(-2, 2);
    }
    heightPercent = std::clamp(heightPercent, config.minHeightPercent, config.maxHeightPercent);

    const int width = rightX - leftX;
    const int height = screenHeight * heightPercent / 100;

    // En pantalla, más Y = más abajo: la columna se ancla al fondo.
    return Segment(
        width, height, BROWN,
        Rectangle{
            static_cast<float>(leftX),
            static_cast<float>(screenHeight - height),
            static_cast<float>(width),
            static_cast<float>(height)
        });
}

void Map::generateSpawnBases()
{
    spawnBases.clear();

    const float baseWidth = screenWidth * config.baseWidthPercent / 100;
    const float baseSurfaceY = screenHeight - screenHeight * config.baseHeightPercent / 100;
    const float grassHeight = static_cast<float>(config.grassHeight);   // ← evita el warning

    SpawnBase base1;
    base1.setBase({ 0.0f, baseSurfaceY, baseWidth, grassHeight });
    SpawnBase base2;
    base2.setBase({ screenWidth - baseWidth, baseSurfaceY, baseWidth, grassHeight });

    spawnBases.push_back(base1);
    spawnBases.push_back(base2);
}


void Map::generateSegments()
{
    segments.clear();
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    const std::vector<int> columnEdges = buildColumnEdges();
    const TerrainProfile profile = buildTerrainProfile();

    segments.reserve(columnEdges.size() - 1);
    for (int i = 0; i + 1 < columnEdges.size(); ++i)
    {
        segments.push_back(makeSegment(columnEdges[i], columnEdges[i + 1], profile));
    }
}


// Generación del Mapa
void Map::generateTerrain()
{
    generateSpawnBases();
    generateSegments();
    buildTerrainMask();
    updateTerrainTexture();
}   

// Actualización y Eventos 
void Map::update(int currentRound){

}

void Map::draw() const
{
    // Dibujar el terreno destructible
    if (terrainTexture.id != 0) {
        DrawTexture(terrainTexture, 0, 0, WHITE);
    }

    // Dibujar la roca indestructible del fondo
    const int bedrockHeight =
        screenHeight * config.bedrockHeightPercent / 100;

    DrawRectangle(
        0,
        screenHeight - bedrockHeight,
        screenWidth,
        bedrockHeight,
        GRAY
    );

    // Dibujar las bases de los jugadores
    for (const SpawnBase& base : spawnBases) {
        base.draw();
    }
}

int Map::getGroundYAt(float x) const
{
    const int pixelX = static_cast<int>(std::floor(x));

    if (pixelX < 0 || pixelX >= screenWidth) {
        return screenHeight;
    }

    for (int y = 0; y < screenHeight; ++y)
    {
        if (isSolid(pixelX, y)) {
            return y;
        }
    }

    return screenHeight;
}

//Colisión
bool Map::checkCollision(Rectangle entity) const
{
    if (terrainMask.empty()) {
        return false;
    }

    const int minX = std::max(
        0,
        static_cast<int>(std::floor(entity.x))
    );

    const int maxX = std::min(
        screenWidth - 1,
        static_cast<int>(std::ceil(entity.x + entity.width)) - 1
    );

    const int minY = std::max(
        0,
        static_cast<int>(std::floor(entity.y))
    );

    const int maxY = std::min(
        screenHeight - 1,
        static_cast<int>(std::ceil(entity.y + entity.height)) - 1
    );

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            if (isSolid(x, y)) {
                return true;
            }
        }
    }

    return false;
}

bool Map::checkCollisionProjectile(Vector2 center, float radius) const
{
    if (terrainMask.empty() || radius <= 0.0f) {
        return false;
    }

    const int minX = std::max(
        0,
        static_cast<int>(std::floor(center.x - radius))
    );

    const int maxX = std::min(
        screenWidth - 1,
        static_cast<int>(std::ceil(center.x + radius))
    );

    const int minY = std::max(
        0,
        static_cast<int>(std::floor(center.y - radius))
    );

    const int maxY = std::min(
        screenHeight - 1,
        static_cast<int>(std::ceil(center.y + radius))
    );

    const float radiusSquared = radius * radius;

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            if (!isSolid(x, y)) {
                continue;
            }

            const float dx = x - center.x;
            const float dy = y - center.y;

            if (dx * dx + dy * dy <= radiusSquared) {
                return true;
            }
        }
    }

    return false;
}

void Map::destroyTerrainAt(Vector2 impactPoint, float radius)
{
    if (radius <= 0.0f || terrainMask.empty()) {
        return;
    }

    const int bedrockHeight =
        screenHeight * config.bedrockHeightPercent / 100;

    const int bedrockY = screenHeight - bedrockHeight;

    // Limites del area afectada
    const int minX = std::max(
        0,
        static_cast<int>(std::floor(impactPoint.x - radius))
    );

    const int maxX = std::min(
        screenWidth - 1,
        static_cast<int>(std::ceil(impactPoint.x + radius))
    );

    const int minY = std::max(
        0,
        static_cast<int>(std::floor(impactPoint.y - radius))
    );

    const int maxY = std::min(
        bedrockY - 1,
        static_cast<int>(std::ceil(impactPoint.y + radius))
    );

    const float radiusSquared = radius * radius;

    // Eliminar los pixeles dentro del circulo
    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            const float dx = x - impactPoint.x;
            const float dy = y - impactPoint.y;

            if (dx * dx + dy * dy <= radiusSquared)
            {
                terrainMask[y * screenWidth + x] = 0;
            }
        }
    }

    // Actualizar la representacion visual
    updateTerrainTexture();
}


const std::vector<Segment>& Map::getSegments() const {
    return segments;
}
const std::vector<SpawnBase>& Map::getSpawnBases() const {
    return spawnBases;
}
//float Map::getWaterLevel() const;

bool Map::isSolid(int x, int y) const
{
    if (x < 0 || x >= screenWidth ||
        y < 0 || y >= screenHeight) {
        return false;
    }

    if (terrainMask.empty()) {
        return false;
    }

    return terrainMask[y * screenWidth + x] != 0;
}


// converts the terrain into a grid of solid and empty pixels: 1s and 0s
void Map::buildTerrainMask()
{
    terrainMask.assign(screenWidth * screenHeight, 0);

    for (const Segment& segment : segments)
    {
        Rectangle bounds = segment.getBounds();

        int startX = std::max(0, static_cast<int>(bounds.x));
        int endX = std::min(
            screenWidth,
            static_cast<int>(bounds.x + bounds.width)
        );

        int startY = std::max(0, static_cast<int>(bounds.y));

        for (int x = startX; x < endX; ++x)
        {
            for (int y = startY; y < screenHeight; ++y)
            {
                terrainMask[y * screenWidth + x] = 1;
            }
        }
    }
}

//generates a transparent image and fills its solid pixels with brown. 

void Map::updateTerrainTexture()
{
    if (terrainImage.data != nullptr) {
        UnloadImage(terrainImage);
        terrainImage = {};
    }

    terrainImage = GenImageColor(screenWidth, screenHeight, BLANK);

    Color* pixels = static_cast<Color*>(terrainImage.data);

    for (int y = 0; y < screenHeight; ++y)
    {
        for (int x = 0; x < screenWidth; ++x)
        {
            if (isSolid(x, y))
            {
                bool grass = false;

                // Comprobar si estamos cerca de la superficie
                for (int offset = 1; offset <= config.grassHeight; ++offset)
                {
                    if (!isSolid(x, y - offset)) {
                        grass = true;
                        break;
                    }
                }

                pixels[y * screenWidth + x] = grass ? GREEN : BROWN;
            }
        }
    }

    if (terrainTexture.id != 0) {
        UnloadTexture(terrainTexture);
    }

    terrainTexture = LoadTextureFromImage(terrainImage);
}


//cleanup
void Map::unloadTerrain()
{
    if (terrainTexture.id != 0) {
        UnloadTexture(terrainTexture);
        terrainTexture = {};
    }

    if (terrainImage.data != nullptr) {
        UnloadImage(terrainImage);
        terrainImage = {};
    }
}