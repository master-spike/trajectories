#include "load_shaders.hpp"

#include <GLES2/gl2.h>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>

static GLuint load_shader(const char* file_path, GLenum type) {
	GLuint shader_id = glCreateShader(type);
	
	std::ifstream shader_stream(file_path, std::ios::in);
	if (!shader_stream.is_open()) return 0;
	std::stringstream sstr;	std::noskipws(sstr);
	sstr << shader_stream.rdbuf();
	std::string shader_code(std::istream_iterator<char>(sstr), std::istream_iterator<char>{});
	shader_stream.close();
	printf("Compiling shader : %s\n", file_path);
	const char* source_ptr = shader_code.c_str();
	glShaderSource(shader_id, 1, &source_ptr, nullptr);
	glCompileShader(shader_id);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0){
		std::string error_message(InfoLogLength, '0');
		glGetShaderInfoLog(shader_id, InfoLogLength, nullptr, error_message.data());
		printf("%s\n", error_message.data());
	}

	return shader_id;

}

GLuint load_shaders(const char* vertex_file_path, const char * fragment_file_path, const char* geometry_file_path){

	GLuint VertShaderID = (vertex_file_path) ? load_shader(vertex_file_path, GL_VERTEX_SHADER): 0;
	GLuint FragShaderID = (fragment_file_path) ? load_shader(fragment_file_path, GL_FRAGMENT_SHADER): 0;
	GLuint GeomShaderID = (geometry_file_path) ? load_shader(geometry_file_path, GL_GEOMETRY_SHADER): 0;

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	if (vertex_file_path) glAttachShader(ProgramID, VertShaderID);
	if (fragment_file_path) glAttachShader(ProgramID, FragShaderID);
	if (geometry_file_path) glAttachShader(ProgramID, GeomShaderID);
	glLinkProgram(ProgramID);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

	if (InfoLogLength > 0){
		std::string error_message(InfoLogLength, '0');
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, error_message.data());
		printf("%s\n", error_message.data());
	}
	

	// delete shaders once program is linked
	if (vertex_file_path) glDetachShader(ProgramID, VertShaderID);
	if (fragment_file_path) glDetachShader(ProgramID, FragShaderID);
	if (geometry_file_path) glDetachShader(ProgramID, GeomShaderID);
	
	if (vertex_file_path) glDeleteShader(VertShaderID);
	if (fragment_file_path) glDeleteShader(FragShaderID);
	if (geometry_file_path) glDeleteShader(GeomShaderID);

	return ProgramID;
}

