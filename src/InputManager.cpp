#define GLM_ENABLE_EXPERIMENTAL
#include "InputManager.h"

namespace {
    const float MOUSE_SENSITIVITY = 0.005f;
    const float GEOMETRY_ROTATION_SPEED = 0.05f;
    const float GEOMETRY_SCALE_UP = 1.1f;
    const float GEOMETRY_SCALE_DOWN = 0.9f;
}

InputManager::InputManager( GLFWwindow* w, Camera* cam, Scene* s )
    : window( w )
    , camera( cam )
    , scene( s )
    , sketchCursor( glfwCreateStandardCursor( GLFW_CROSSHAIR_CURSOR ) )
    , standardCursor( glfwCreateStandardCursor( GLFW_ARROW_CURSOR ) ) {
}

void InputManager::CheckInput() {
    MouseInput();
}

void InputManager::MouseInput() {
    double xpos, ypos;

    if ( sketching ) {

        if ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS ) {
            glfwGetCursorPos( window, &xpos, &ypos );
            glm::vec2 screenRes = camera->GetScreenResolution();
            float MVPX = 2.0f * ( xpos / screenRes.x ) - 1;
            float MVPY = -2.0f * ( ypos / screenRes.y ) + 1;

            if ( xpos != cursorX || ypos != cursorY ) {
                Geometry* g = scene->GetSketch();
                g->vertices.push_back( glm::vec3( MVPX, MVPY, 0.0f ) );
                g->colours.push_back( glm::vec3( 1.0f, 0.0f, 0.0f ) );
                g->normals.push_back( glm::vec3( 0.0f, 0.0f, 0.0f ) );
                g->Load();
            }
        }
    } else {
        if ( shiftKey && glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS ) {
            glfwGetCursorPos( window, &xpos, &ypos );
            float deltaX = ( xpos - cursorX ) * MOUSE_SENSITIVITY;
            float deltaY = ( ypos - cursorY ) * MOUSE_SENSITIVITY;

            if ( !( deltaX || deltaY ) ) {
                return;
            }

            camera->Panning( deltaX, deltaY );
        } else if ( glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT ) == GLFW_PRESS ) {
            // Get mouse position
            glfwGetCursorPos( window, &xpos, &ypos );
            float deltaX = ( xpos - cursorX ) * MOUSE_SENSITIVITY;
            float deltaY = ( ypos - cursorY ) * MOUSE_SENSITIVITY;

            if ( !( deltaX || deltaY ) ) {
                return;
            }

            camera->RotateAround( deltaX, deltaY );
        }
    }

    glfwGetCursorPos( window, &xpos, &ypos );
    cursorX = xpos;
    cursorY = ypos;
}

void InputManager::KeyInput( const int key, const int action ) {
    if ( action == GLFW_PRESS ) {
        switch ( key ) {
            // Camera
            case GLFW_KEY_UP:
                camera->Zoom( 1 );
                break;

            case GLFW_KEY_DOWN:
                camera->Zoom( -1 );
                break;

            case GLFW_KEY_LEFT_SHIFT:
                shiftKey = true;
                break;

            // Geometry Select toggle (TODO)
            case GLFW_KEY_1:
                scene->ToggleSelectedGeometry( 0 );
                break;

            case GLFW_KEY_2:
                scene->ToggleSelectedGeometry( 1 );
                break;

            case GLFW_KEY_3:
                scene->ToggleSelectedGeometry( 2 );
                break;

            case GLFW_KEY_4:
                scene->ToggleSelectedGeometry( 3 );
                break;

            case GLFW_KEY_5:
                scene->ToggleSelectedGeometry( 4 );
                break;

            case GLFW_KEY_6:
                scene->ToggleSelectedGeometry( 5 );
                break;

            case GLFW_KEY_7:
                scene->ToggleSelectedGeometry( 6 );
                break;

            case GLFW_KEY_8:
                scene->ToggleSelectedGeometry( 7 );
                break;

            case GLFW_KEY_9:
                scene->ToggleSelectedGeometry( 8 );
                break;

            case GLFW_KEY_0:
                scene->ToggleSelectedGeometry( 9 );
                break;

            // Sketching toggle
            case GLFW_KEY_X:
                if ( !scene->IsSketchConfirmed() ) {
                    sketching = !sketching;
                    ( sketching )
                    ? glfwSetCursor( window, sketchCursor )
                    : glfwSetCursor( window, standardCursor );
                }

                break;

            case GLFW_KEY_DELETE:
                scene->ClearSketch();

                for ( int i = 0; i < scene->GetGeometriesSize(); i++ ) {
                    Geometry* g = scene->GetGeometry( i );

                    if ( g->IsSelectedGeometry() ) {
                        scene->DeleteGeometry( i );
                    }
                }

                break;

            case GLFW_KEY_K:
                scene->SmoothSketch();
                break;

            case GLFW_KEY_R:
                // Clear Geometries and reload cube model
                scene->ResetGeometry();
                break;

            // Wireframe toggle
            case GLFW_KEY_Z:
                wireframe = !wireframe;
                ( wireframe )
                ? glPolygonMode( GL_FRONT_AND_BACK, GL_LINE )
                : glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                break;

            case GLFW_KEY_ENTER:
                if ( !scene->IsSketchConfirmed() ) {
                    scene->SmoothSketch();
                    scene->Carve( scene->GetGeometry( 0 ) );
                    sketching = false;
                    glfwSetCursor( window, standardCursor );
                    scene->ClearSketch();
                }

                break;

            // Misc
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose( window, GL_TRUE );
                break;

            default:
                break;
        }
    }

    if ( action == GLFW_RELEASE ) {
        // Camera
        switch ( key ) {
            case GLFW_KEY_LEFT_SHIFT:
                shiftKey = false;

            default:
                break;
        }
    }

    if ( ( action == GLFW_PRESS ) || ( action == GLFW_REPEAT ) ) {
        for ( int i = 0; i < scene->GetGeometriesSize(); i++ ) {
            Geometry* geometry = scene->GetGeometry( i );

            if ( !geometry->IsSelectedGeometry() ) {
                continue;
            }

            switch ( key ) {
                case GLFW_KEY_A:
                    geometry->Rotate( glm::vec3( 0.0f, 1.0f, 0.0f ), -GEOMETRY_ROTATION_SPEED );
                    break;

                case GLFW_KEY_D:
                    geometry->Rotate( glm::vec3( 0.0f, 1.0f, 0.0f ), GEOMETRY_ROTATION_SPEED );
                    break;

                case GLFW_KEY_W:
                    geometry->Rotate( glm::vec3( 1.0f, 0.0f, 0.0f ), GEOMETRY_ROTATION_SPEED );
                    break;

                case GLFW_KEY_S:
                    geometry->Rotate( glm::vec3( 1.0f, 0.0f, 0.0f ), -GEOMETRY_ROTATION_SPEED );
                    break;

                case GLFW_KEY_Q:
                    geometry->Rotate( glm::vec3( 0.0f, 0.0f, 1.0f ), GEOMETRY_ROTATION_SPEED );
                    break;

                case GLFW_KEY_E:
                    geometry->Rotate( glm::vec3( 0.0f, 0.0f, 1.0f ), -GEOMETRY_ROTATION_SPEED );
                    break;

                case GLFW_KEY_P:
                    scene->ExportSelectedGeometries();
                    break;

                default:
                    break;
            }
        }
    }
}

void InputManager::ScrollWheel( double xoffset, double yoffset ) {
    if ( scene->HasAnyGeometrySelected() ) {
        for ( int i = 0; i < scene->GetGeometriesSize(); i++ ) {
            Geometry* geometry = scene->GetGeometry( i );

            if ( !geometry->IsSelectedGeometry() ) {
                continue;
            }

            if ( yoffset == 1 ) {
                geometry->Scale( glm::vec3( GEOMETRY_SCALE_UP ) );
            } else if ( yoffset == -1 ) {
                geometry->Scale( glm::vec3( GEOMETRY_SCALE_DOWN ) );
            }
        }
    } else {
        camera->Zoom( yoffset );
    }
}
