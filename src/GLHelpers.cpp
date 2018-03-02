#include <glad/glad.h>
#include "GLHelpers.h"

void Initialize( GLFWwindow*& window ) {
    // initialize the GLFW windowing system
    if ( !glfwInit() ) {
        std::cerr << "ERROR: GLFW failed to initialize, TERMINATING" << std::endl;
        std::exit( EXIT_FAILURE );
    }

    glfwSetErrorCallback( ErrorCallback );

    // Attempt to create a window with an OpenGL 4.1 core profile context
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    int width = 512, height = 512;
    window = glfwCreateWindow( width, height, "CPSC 589 Modelling Project", 0, 0 );

    if ( !window ) {
        std::cerr << "Program failed to create GLFW window, TERMINATING" << std::endl;
        glfwTerminate();
        std::exit( EXIT_FAILURE );
    }

    glfwMakeContextCurrent( window );

    //Intialize GLAD
    if ( !gladLoadGL() ) {
        std::cerr << "GLAD init failed" << std::endl;
        std::exit( EXIT_FAILURE );
    }

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_STENCIL_TEST );
    glStencilOp( GL_KEEP, GL_KEEP, GL_REPLACE );
    // Query and print out information about our OpenGL environment
    QueryGLVersion();
}

// OpenGL utility functions

// Reports OpenGL version and renderer information
void QueryGLVersion() {
    std::string version = reinterpret_cast<const char*>( glGetString( GL_VERSION ) );
    std::string glslver = reinterpret_cast<const char*>( glGetString( GL_SHADING_LANGUAGE_VERSION ) );
    std::string renderer = reinterpret_cast<const char*>( glGetString( GL_RENDERER ) );

    std::cout << "OpenGL [ " << version << " ] "
              << "with GLSL [ " << glslver << " ] "
              << "on renderer [ " << renderer << " ]" << std::endl;
}

// Reports OpenGL errors
bool CheckGLErrors() {
    bool error = false;

    for ( GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError() ) {
        std::cout << "OpenGL ERROR:  ";

        switch ( flag ) {
            case GL_INVALID_ENUM:
                std::cout << "GL_INVALID_ENUM" << std::endl;
                break;

            case GL_INVALID_VALUE:
                std::cout << "GL_INVALID_VALUE" << std::endl;
                break;

            case GL_INVALID_OPERATION:
                std::cout << "GL_INVALID_OPERATION" << std::endl;
                break;

            case GL_INVALID_FRAMEBUFFER_OPERATION:
                std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
                break;

            case GL_OUT_OF_MEMORY:
                std::cout << "GL_OUT_OF_MEMORY" << std::endl;
                break;

            default:
                std::cout << "[unknown error code]" << std::endl;
        }

        error = true;
    }

    return error;
}

// GLFW CALLBACK FUNCTIONS

// Reports GLFW errors
void ErrorCallback( int error, const char* description ) {
    std::cerr << "GLFW ERROR " << error << ":" << std::endl;
    std::cerr << description << std::endl;
}



// GLM DEBUG
void PrintMat4( glm::mat4 const& m ) {
    for ( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            std::cout << m[i][j] << " ";
        }

        std::cout << std::endl;
    }
}

void PrintVec3( glm::vec3 const& v ) {
    std::cout << v[0] << " " << v[1] << " " << v[2] << std::endl;
}
