#include <iostream>

#include "Exporter.h"

namespace {
    const std::string EXPORT_DIR = "exports/";
    const std::string FORMAT_ID = "obj";
    const unsigned int VERTEX_COUNT_PER_FACE = 3;
}

void Exporter::ExportGeometries( const std::vector<Geometry*>& geometries, const std::string filename ) const {
    aiScene* scene   = CreateExportableScene( geometries );

    Export( scene, filename );

    delete scene;
}

// Create aiScene, ref: https://github.com/assimp/assimp/issues/1454
aiScene* Exporter::CreateExportableScene( const std::vector<Geometry*>& geometries ) const {
    aiScene* scene   = new aiScene();
    scene->mRootNode = new aiNode();

    // Add materials (Currently not supported)
    scene->mMaterials    = new aiMaterial *[1];
    scene->mMaterials[0] = nullptr;
    scene->mNumMaterials = 1;

    scene->mMaterials[0] = new aiMaterial();

    // Create mesh
    scene->mMeshes    = new aiMesh *[1];
    scene->mMeshes[0] = nullptr;
    scene->mNumMeshes = geometries.size();

    scene->mRootNode->mMeshes    = new unsigned int[1];
    scene->mRootNode->mMeshes[0] = 0;
    scene->mRootNode->mNumMeshes = 1;

    for ( int i = 0; i < geometries.size(); ++i ) {
        Geometry* geometry = geometries[i];
        scene->mMeshes[i] = CreateExportableMesh( geometry );
        scene->mMeshes[i]->mMaterialIndex = 0;
    }

    return scene;
}

aiMesh* Exporter::CreateExportableMesh( Geometry* geometry ) const {
    aiMesh* pMesh = new aiMesh();

    pMesh->mVertices    = new aiVector3D[geometry->vertices.size()];
    pMesh->mNumVertices = geometry->vertices.size();

    // Fill mesh with verticecs
    for ( int i = 0; i < geometry->vertices.size(); ++i ) {
        pMesh->mVertices[i] = aiVector3D( geometry->vertices[i].x, geometry->vertices[i].y, geometry->vertices[i].z );
    }

    // Fill face data
    pMesh->mNumFaces = geometry->faces.size() / VERTEX_COUNT_PER_FACE;
    pMesh->mFaces    = new aiFace[pMesh->mNumFaces];

    for ( int i = 0; i < pMesh->mNumFaces; ++i ) {
        aiFace& face     = pMesh->mFaces[i];
        face.mIndices    = new unsigned int[VERTEX_COUNT_PER_FACE];
        face.mNumIndices = VERTEX_COUNT_PER_FACE;

        for ( int j = 0; j < VERTEX_COUNT_PER_FACE; ++j ) {
            int idx = ( i * VERTEX_COUNT_PER_FACE ) + j;
            face.mIndices[j] = geometry->faces[idx];
        }
    }

    return pMesh;
}

void Exporter::Export( aiScene* scene, const std::string filename ) const {
    std::string out_path = EXPORT_DIR + filename + "." + FORMAT_ID;

    // Run AssImp Exporter
    Assimp::Exporter exporter;
    auto ret = exporter.Export( scene, FORMAT_ID, out_path );

    if ( ret != AI_SUCCESS ) {
        std::cerr << "Export failed" << std::endl;
    } else {
        std::cerr << "Export successful" << std::endl;
    }
}
