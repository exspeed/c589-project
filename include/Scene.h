#pragma once

#include "PreprocessorSettings.h"

#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>

#include "Geometry.h"
#include "Camera.h"
#include "Shader.h"

class Scene {
public:
    Scene( Camera* cam );
    void AddGeometry( Geometry* g );
    void AddSketch( Geometry* g );
    void ResetGeometry();
    void DeleteGeometry( int i );
    void ClearGeometries();
    void ExportSelectedGeometries() const;
    void ExportAllGeometries() const;
    void ClearSketch();
    void SmoothSketch();
    int GetGeometriesSize();
    Geometry* GetGeometry( int i );
    Geometry* GetSketch();
    void ToggleSelectedGeometry( int i );
    bool HasAnyGeometrySelected();
    std::vector<Geometry*> GetGeometrySelected();
    void Render() const;
    bool IsSketchConfirmed();
    void Carve( Geometry* g );
    void CrackPattern( Geometry* g );
    float DEPTH = 0.005f;
    float WIDTH = 0.005f;

private:
    Camera* camera;

    std::vector<Geometry*> geometries;
    Geometry* sketch;
    void InitializeVAO( Geometry& g ) const;
    void RenderGeometry( Geometry* geometry ) const;
    void RenderSketch( Geometry* geometry ) const;
    void RenderStencil( Geometry* geometry ) const;
    bool SketchConfirmed = false;
};
