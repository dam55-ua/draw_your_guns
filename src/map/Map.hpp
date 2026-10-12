#pragma once
#include <raylib.h>
#include <vector>
#include "segment.hpp"
#include "spawn_base.hpp"

// Agrupamos la configuración para mantener limpia la clase
struct MapConfig
{
    int terrainSegmentCount = 40;
    int terrainAnchorCount = 12;
    int minHeightPercent = 28;
    int maxHeightPercent = 52;
    int boundaryVariationPercent = 25;
    int grassHeight = 8;
    int baseWidthPercent = 6;
    int baseHeightPercent = 42;
    int foundationHeightPercent = 5;
};

class Map
{
private:
    int screenWidth;
    int screenHeight;
    MapConfig config;

    std::vector<Segment> segments;
    std::vector<SpawnBase> spawnBases;

    // Gestión del Evento Dinámico: Subida del Nivel del Mar
    float waterLevel;
    float waterRiseSpeed;

public:
    Map(int screenWidth, int screenHeight, MapConfig config = MapConfig());

    // Generación
    void generateTerrain(const std::vector<SpawnBase>& bases);
    void generateSegments();

    // Actualización y Eventos (P. ej. Subida de agua en rondas avanzadas)
    void update(int currentRound);
    void draw() const;

    // Métodos de Física y Colisión para Jugadores y Proyectiles
    bool checkCollision(Rectangle entity) const;
    void destroyTerrainAt(Vector2 impactPoint, float radius);

    // Getters por referencia constante (Optimización de memoria)
    const std::vector<Segment>& getSegments() const;
    const std::vector<SpawnBase>& getSpawnBases() const;
    float getWaterLevel() const;
};