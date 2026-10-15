#pragma once
#include <vector>

struct TerrainAnchor //Punto por el que el terreno debe pasar
{
    float x;
    int heightPercent;
};

//Clase que se encarga de gestionar el perfil del terreno, incluyendo anclas y muestreo de alturas.

class TerrainProfile
{
    void clear();

    void addAnchor(float x, int heightPercent);

    // Añade dos anclas a la misma altura: genera un tramo horizontal plano.
    void addFlatSection(float fromX, float toX, int heightPercent);

    // Reordena las anclas por X (obligatorio antes de muestrear).
    void sortAnchors();

    // Altura interpolada (en % de pantalla) para una X dada.
    int samplePercent(float x) const;

    bool empty() const { return anchors.empty(); }

private:
    std::vector<TerrainAnchor> anchors;
};