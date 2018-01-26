#include "ShaderTool.h"

// --------------------------------------------------------------------------
// OpenGL shader support functions

// reads a text file with the given name into a std::string
std::string LoadSource(const std::string &filename)
{
	std::string source;

	std::ifstream input(filename.c_str());
	if (input) {
		copy(std::istreambuf_iterator<char>(input),
			std::istreambuf_iterator<char>(),
			back_inserter(source));
		input.close();
	}
	else {
		std::cerr << "ERROR: Could not load shader source from file "
			<< filename << std::endl;
	}

	return source;
}

// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const std::string &source)
{
	// allocate shader object name
	GLuint shaderObject = glCreateShader(shaderType);

	// try compiling the source as a shader of the given type
	const GLchar *source_ptr = source.c_str();
	glShaderSource(shaderObject, 1, &source_ptr, 0);
	glCompileShader(shaderObject);

	// retrieve compile status
	GLint status;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		std::string info(length, ' ');
		glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
		std::cerr << "ERROR compiling shader:" << std::endl << std::endl;
		std::cerr << source << std::endl;
		std::cerr << info << std::endl;
	}

	return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (fragmentShader) glAttachShader(programObject, fragmentShader);

	// try linking the program with given attachments
	glLinkProgram(programObject);

	// retrieve link status
	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
		std::string info(length, ' ');
		glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
		std::cerr << "ERROR linking shader program:" << std::endl;
		std::cerr << info << std::endl;
	}

	return programObject;
}