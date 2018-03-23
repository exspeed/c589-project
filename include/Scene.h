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
    Scene(Camera* cam );
    void AddGeometry( Geometry* g );
    void ClearGeometries();
    int GetGeometriesSize();
    Geometry* GetGeometry( int i );
    void ToggleSelectedGeometry( int i );
    bool HasAnyGeometrySelected();
    void Render() const;

private:
    Camera* camera;

    std::vector<Geometry*> geometries;
    void InitializeVAO( Geometry& g ) const;
    void RenderStencil( Geometry* geometry ) const;
};
