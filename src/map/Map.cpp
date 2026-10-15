#include "Map.hpp"


Map::Map(int screenWidth, int screenHeight, MapConfig config){
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->config = config;
}


void Map::generateSpawnBases()
{
    spawnBases.clear();

    const float baseWidth = screenWidth * config.baseWidthPercent / 100;
    const float baseSurfaceY = screenHeight - screenHeight * config.baseHeightPercent / 100;

    SpawnBase base1;
    base1.setBase({
        0.0f, baseSurfaceY,baseWidth,config.grassHeight
    });
    SpawnBase base2;
    base2.setBase({
        screenWidth - baseWidth, baseSurfaceY, baseWidth, config.grassHeight
    });
    spawnBases.push_back(base1);
    spawnBases.push_back(base2);

    for (SpawnBase& base : spawnBases)
    {
        base.setBase({
            base.getBase().x, base.getBase().y, base.getBase().width, config.grassHeight
        });
    }
    
}


void Map::generateSegments(){
    segments.clear();
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    
    
}


// Generación del Mapa
void Map::generateTerrain(const std::vector<SpawnBase>& bases)
{
    spawnBases = bases;
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
void Map::destroyTerrainAt(Vector2 impactPoint, float radius) {

}


const std::vector<Segment>& Map::getSegments() const {

}
const std::vector<SpawnBase>& Map::getSpawnBases() const {

}
//float Map::getWaterLevel() const;