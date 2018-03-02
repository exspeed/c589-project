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
    std::vector<bool> selected;

    Scene( Shader* prog, Shader* progOutline, Camera* cam );
    void AddGeometry( Geometry* g );
    void ClearGeometries();
    int GetGeometriesSize();
    Geometry* getGeometry( int i );
    void ToggleSelectedGeometry( int i );
    void Render() const;

private:
    Shader* program;
    Shader* programOutline;
    Camera* camera;

    std::vector<Geometry*> geometries;
    void InitializeVAO( Geometry& g ) const;
    void RenderStencil( Geometry* geometry ) const;
};
