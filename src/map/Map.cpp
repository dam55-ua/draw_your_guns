#include "Map.hpp"
#include <algorithm>


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
}   

// Actualización y Eventos 
void Map::update(int currentRound){

}

void Map::draw() const
{
    // Terreno + franja de césped.
    for (const Segment& segment : segments) {
        const Rectangle bounds = segment.getBounds();
        segment.draw();
        DrawRectangle(bounds.x, bounds.y, bounds.width, config.grassHeight, GREEN);
    }

    // Franja indestructible en el fondo.
    const int bedrockHeight = screenHeight * config.bedrockHeightPercent / 100;
    DrawRectangle(0, screenHeight - bedrockHeight, screenWidth, bedrockHeight, GRAY);

    for (const SpawnBase& base : spawnBases){
        base.draw();
    }
}

int Map::getGroundYAt(float x) const
{
    for (const Segment& segment : segments)
    {
        const Rectangle bounds = segment.getBounds();
        if (x >= bounds.x && x <= bounds.x + bounds.width)
        {
            return bounds.y; // Retorna la coordenada Y del suelo en esa posición X
        }
    }
    return screenHeight; // Si no hay segmento, retorna el fondo de la pantalla
}

//Colisión
bool Map::checkCollision(Rectangle entity) const
{
    for (const Segment& segment : segments){
        if (segment.checkCollision(entity)){
            return true;
        }
    }
    return false;
}

bool Map::checkCollisionProjectile(Vector2 center, float radius) const
{
    for (const Segment& segment : segments){
        if (CheckCollisionCircleRec(center, radius, segment.getBounds())){return true;}
    }
    return false;
}

void Map::destroyTerrainAt(Vector2 impactPoint, float radius) {

}


const std::vector<Segment>& Map::getSegments() const {
    return segments;
}
const std::vector<SpawnBase>& Map::getSpawnBases() const {
    return spawnBases;
}
//float Map::getWaterLevel() const;