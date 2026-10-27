#pragma once
#include <raylib.h>
#include <vector>
#include "Segment.hpp"
#include "SpawnBase.hpp"
#include "TerrainProfile.hpp"
#include <cstdint>

// Configuración del Mapa predeterminada
struct MapConfig
{
    int terrainSegmentCount = 80;
    int terrainAnchorCount = 20;
    int minHeightPercent = 25;
    int maxHeightPercent = 80;
    int columnEdgeVariationPercent = 25;
    int grassHeight = 8;
    int baseWidthPercent = 6;
    int baseHeightPercent = 40;
    int bedrockHeightPercent = 1;
};

class Map
{
private:

    int screenWidth;
    int screenHeight;
    MapConfig config;

    std::vector<Segment> segments;
    std::vector<SpawnBase> spawnBases;

    std::vector<int> buildColumnEdges() const;
    TerrainProfile buildTerrainProfile() const;
    Segment makeSegment(int leftX, int rightX, const TerrainProfile& profile) const;
    bool isInsideSpawnBase(float x) const;
    // 1 = terreno sólido, 0 = espacio vacío
    std::vector<std::uint8_t> terrainMask;

    // Representación visual del terreno
    Image terrainImage = {};
    Texture2D terrainTexture = {};

    // Acceso al terreno por píxeles
    bool isSolid(int x, int y) const;
    void buildTerrainMask();
    void updateTerrainTexture();



   /*// Gestión del Evento Dinámico: Subida del Nivel del Mar
    float waterLevel;
    float waterRiseSpeed;*/

public:
    Map(int screenWidth, int screenHeight, MapConfig config = MapConfig());
    ~Map();

    void generateSpawnBases();

    // Generación
    void generateTerrain();
    void generateSegments();

    // Actualización y Eventos 
    void update(int currentRound);
    void draw() const;

    // Métodos de Física y Colisión para Jugadores y Proyectiles
    bool checkCollision(Rectangle entity) const;
    int getGroundYAt(float x, float feetY) const;
    void destroyTerrainAt(Vector2 impactPoint, float radius);
    bool checkCollisionProjectile(Vector2 center, float radius) const;

    // Getters por referencia constante (Optimización de memoria)
    const std::vector<Segment>& getSegments() const;
    const std::vector<SpawnBase>& getSpawnBases() const;
    //float getWaterLevel() const;

    void unloadTerrain();
};