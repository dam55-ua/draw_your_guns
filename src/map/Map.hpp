#pragma once
#include <raylib.h>
#include <vector>
#include "Segment.hpp"
#include "SpawnBase.hpp"
#include "TerrainProfile.hpp"

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

   /*// Gestión del Evento Dinámico: Subida del Nivel del Mar
    float waterLevel;
    float waterRiseSpeed;*/

public:
    Map(int screenWidth, int screenHeight, MapConfig config = MapConfig());

    void generateSpawnBases();

    // Generación
    void generateTerrain();
    void generateSegments();

    // Actualización y Eventos 
    void update(int currentRound);
    void draw() const;

    // Métodos de Física y Colisión para Jugadores y Proyectiles
    bool checkCollision(Rectangle entity) const;
    int getGroundYAt(float x) const;
    void destroyTerrainAt(Vector2 impactPoint, float radius);

    // Getters por referencia constante (Optimización de memoria)
    const std::vector<Segment>& getSegments() const;
    const std::vector<SpawnBase>& getSpawnBases() const;
    //float getWaterLevel() const;
};