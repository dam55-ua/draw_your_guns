#include "TerrainProfile.hpp"
#include <algorithm>

void TerrainProfile::clear(){
    anchors.clear();
}

void TerrainProfile::addAnchor(float x, int heightPercent)
{
    anchors.push_back({ x, heightPercent });
}

void TerrainProfile::addFlatSection(float fromX, float toX, int heightPercent)
{
    addAnchor(fromX, heightPercent);
    addAnchor(toX, heightPercent);
}

void TerrainProfile::sortAnchors()
{
    std::sort(anchors.begin(), anchors.end(), [](const TerrainAnchor& a, const TerrainAnchor& b){return a.x < b.x;});
}

// Devuelve la altura del terreno (en % de pantalla) para una posición x.
// Interpola linealmente entre las dos anclas que rodean a x.
int TerrainProfile::samplePercent(float x) const
{
    if (anchors.empty()){return 0;}
    if (anchors.size() == 1) return anchors.front().heightPercent;

    // Avanza hasta la primera ancla con x >= a la consultada (la "derecha").
    int rightIndex = 0;
    while (rightIndex < anchors.size() && anchors[rightIndex].x < x)
    {
        rightIndex++;
    }

    // x está a la izquierda de la primera ancla.
    if (rightIndex == 0){return anchors.front().heightPercent;}

    // x está a la derecha de la última ancla.
    if (rightIndex == anchors.size()){return anchors.back().heightPercent;}

    // Con el índice ya es trivial sacar las dos anclas vecinas.
    const TerrainAnchor& left  = anchors[rightIndex - 1];
    const TerrainAnchor& right = anchors[rightIndex];

    const float anchorGap  = right.x - left.x;
    if (anchorGap <= 0.0f){return left.heightPercent;}
    const float blend = (x - left.x) / anchorGap;

    return static_cast<int>(left.heightPercent * (1.0f - blend) + right.heightPercent * blend + 0.5f /*Redondeo arriba*/);
}


