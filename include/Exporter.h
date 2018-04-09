#pragma once

#include <string>
#include <vector>

#include "Geometry.h"

#include "assimp/Exporter.hpp"
#include "assimp/scene.h"
#include "assimp/mesh.h"

class Exporter {
public:
    void Export( Geometry& geometry, const std::string filename ) const;
    void ExportGeometries( const std::vector<Geometry*>& geometries, const std::string filename ) const;

private:
    aiScene* CreateExportableScene( const std::vector<Geometry*>& geometries ) const;
    aiMesh* CreateExportableMesh( Geometry* geometry ) const;
    void Export( aiScene* scene, const std::string filename ) const;
};
