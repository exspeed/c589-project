#include "Shader.h"

// --------------------------------------------------------------------------
// OpenGL shader support functions

Shader::Shader( std::string vertex_shader, std::string fragment_shader ) {
    // load shader source from files
    std::string vertexSource = LoadSource( vertex_shader );
    std::string fragmentSource = LoadSource( fragment_shader );

    if ( vertexSource.empty() || fragmentSource.empty() ) {
        std::cerr << "Program could not initialize shaders, TERMINATING" << std::endl;
        std::exit( EXIT_FAILURE );
    }

    // compile shader source into shader objects
    GLuint vertex = CompileShader( GL_VERTEX_SHADER, vertexSource );
    GLuint fragment = CompileShader( GL_FRAGMENT_SHADER, fragmentSource );

    // link shader program
    programID = LinkProgram( vertex, fragment );

    glDeleteShader( vertex );
    glDeleteShader( fragment );
}

void Shader::use() {
    glUseProgram( programID );
}

void Shader::setInt( const std::string& name, int value ) const {
    glUniform1i( glGetUniformLocation( programID, name.c_str() ), value );
}

void Shader::setMat4( const std::string& name, const glm::mat4& mat ) const {
    glUniformMatrix4fv( glGetUniformLocation( programID, name.c_str() ), 1, GL_FALSE, &mat[0][0] );
}

// reads a text file with the given name into a std::string
std::string Shader::LoadSource( const std::string& filename ) {
    std::string source;

    std::ifstream input( filename.c_str() );

    if ( input ) {
        copy( std::istreambuf_iterator<char>( input ),
              std::istreambuf_iterator<char>(),
              back_inserter( source ) );
        input.close();
    } else {
        std::cerr << "ERROR: Could not load shader source from file "
                  << filename << std::endl;
    }

    return source;
}

// creates and returns a shader object compiled from the given source
GLuint Shader::CompileShader( GLenum shaderType, const std::string& source ) {
    // allocate shader object name
    GLuint shaderObject = glCreateShader( shaderType );

    // try compiling the source as a shader of the given type
    const GLchar* source_ptr = source.c_str();
    glShaderSource( shaderObject, 1, &source_ptr, 0 );
    glCompileShader( shaderObject );

    // retrieve compile status
    GLint status;
    glGetShaderiv( shaderObject, GL_COMPILE_STATUS, &status );

    if ( status == GL_FALSE ) {
        GLint length;
        glGetShaderiv( shaderObject, GL_INFO_LOG_LENGTH, &length );
        std::string info( length, ' ' );
        glGetShaderInfoLog( shaderObject, info.length(), &length, &info[0] );
        std::cerr << "ERROR compiling shader:" << std::endl << std::endl;
        std::cerr << source << std::endl;
        std::cerr << info << std::endl;
    }

    return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint Shader::LinkProgram( GLuint vertexShader, GLuint fragmentShader ) {
    // allocate program object name
    GLuint programObject = glCreateProgram();

    // attach provided shader objects to this program
    if ( vertexShader ) {
        glAttachShader( programObject, vertexShader );
    }

    if ( fragmentShader ) {
        glAttachShader( programObject, fragmentShader );
    }

    // try linking the program with given attachments
    glLinkProgram( programObject );

    // retrieve link status
    GLint status;
    glGetProgramiv( programObject, GL_LINK_STATUS, &status );

    if ( status == GL_FALSE ) {
        GLint length;
        glGetProgramiv( programObject, GL_INFO_LOG_LENGTH, &length );
        std::string info( length, ' ' );
        glGetProgramInfoLog( programObject, info.length(), &length, &info[0] );
        std::cerr << "ERROR linking shader program:" << std::endl;
        std::cerr << info << std::endl;
    }

    return programObject;
}
