#include <fstream>
#include <string.h>
#include "shader.h"
#include "Debug.h"
#include <assert.h>

/// Vertex and Fragment Shader filenames
Shader::Shader(const char *vsFilename, const char *fsFilename) {
	readCompileAttach(vsFilename,fsFilename);
	link();
	setUniformLocations();
}

void Shader::readCompileAttach(const char *vsFilename, const char *fsFilename){
    GLint status;
	shaderID = 0;
	vertShaderID = 0;
	fragShaderID = 0;
	try { 		
		const char* vsText = readTextFile(vsFilename);
		const char* fsText = readTextFile(fsFilename);
		if (vsText == nullptr || fsText == nullptr) return;

		/// GL_VERTEX_SHADER and GL_FRAGMENT_SHADER are defined in glew.h
		vertShaderID = glCreateShader(GL_VERTEX_SHADER);
		fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		/// Check for errors
		if (vertShaderID == 0 || fragShaderID == 0) {
			std::string errorMsg("Can't create a new shader");
			throw errorMsg;
		}
    
		glShaderSource(vertShaderID, 1, &vsText, 0);   
		glShaderSource(fragShaderID, 1, &fsText, 0);
    
		glCompileShader(vertShaderID);
		/// Check for errors
		glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &status);
			if(status == 0) {
				GLsizei errorLogSize = 0;
				GLsizei titleLength;
				std::string errorLog = vsFilename;
				errorLog +="\nVERT:\n";
				titleLength = errorLog.length();
				glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
				errorLog.resize( titleLength + errorLogSize );
				glGetShaderInfoLog(vertShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
				throw errorLog;
			}

		glCompileShader(fragShaderID);
		/// Check for errors
		glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &status);
			if(status == 0) {
				GLsizei errorLogSize = 0;
				GLsizei titleLength;
				std::string errorLog = fsFilename;
				errorLog += "\nFrag:\n";
				titleLength = errorLog.length();
				glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
				errorLog.resize( titleLength + errorLogSize );
				glGetShaderInfoLog(fragShaderID, errorLogSize, &errorLogSize, &errorLog[titleLength]);
				throw errorLog;
			}
    
		shaderID = glCreateProgram();
		glAttachShader(shaderID, fragShaderID);
		glAttachShader(shaderID, vertShaderID);
		if(vsText) delete[] vsText;
		if(fsText) delete[] fsText;

	}catch(std::string error){
		printf("%s\n",&error[0]);
		Debug::FatalError(error, __FILE__, __LINE__);
	}
	 
}

void Shader::link(){
	GLint status;
	try{
			glLinkProgram(shaderID);
			/// Check for errors
			glGetProgramiv(shaderID, GL_LINK_STATUS, &status);
			if(status == 0) {
				GLsizei errorLogSize = 0;
				std::string errorLog;
				glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &errorLogSize);
				errorLog.resize(errorLogSize);
				glGetProgramInfoLog(shaderID, errorLogSize, &errorLogSize, &errorLog[0]);
				throw errorLog;
			}
		
	}catch(std::string error){
		printf("ERROR:%s\n",&error[0]);
	}

}

Shader::~Shader() {
	glDetachShader(shaderID, fragShaderID);
	glDetachShader(shaderID, vertShaderID);  
	glDeleteShader(fragShaderID);
	glDeleteShader(vertShaderID);
	glDeleteProgram(shaderID);
}



/// Read from a specified file and return a char array from the heap 
/// The memory must be deleted within this class. It may not be the best way 
/// to do this but it is all private and I did delete it! SSF
char* Shader::readTextFile(const char *filename){
	char* buffer = nullptr;
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {								/// Open the file
        file.seekg(0, std::ios::end);					/// goto the end of the file
        int bufferSize = (int) file.tellg();			/// Get the length of the file
		if(bufferSize == 0) {							/// If zero, bad file
			std::string errorMsg("Can't read shader file: ");
			std::string str2(filename);
			errorMsg += str2;
			throw errorMsg;								/// Bail out
		}
		buffer = new char[(int)(bufferSize + 1)];		/// Need to NULL terminate the array
        file.seekg(0, std::ios::beg);
        file.read(buffer, bufferSize);
		buffer[bufferSize] = NULL;						/// Add the NULL
		file.close();
    } else { 
		std::string errorMsg("Can't open shader file: ");
		printf("ERROR:%s:%s\n",filename,&errorMsg[0]);
	}
	return buffer;
}


GLuint Shader::getUniformID(std::string name) { 	
	auto id = uniformMap.find(name);
#ifdef _DEBUG
	if (id == uniformMap.end()) {
		Debug::Warning(name + " :No such uniform name", __FILE__, __LINE__);
		return -1;
	}
#endif 
	return id->second;
}


void Shader::setUniformLocations(){
	int count;
	GLsizei actualLen;
	GLint size;
	GLenum type;
	char *name;
	int maxUniformListLength;
	unsigned int loc;

	glGetProgramiv(shaderID, GL_ACTIVE_UNIFORMS, &count);
	printf("There are %d active Uniforms\n",count);

	/// get the length of the longest named uniform 
	glGetProgramiv(shaderID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformListLength);

	/// Create a little buffer to hold the uniform's name - old C memory call just for fun 
	name = (char *)malloc(sizeof(char) * maxUniformListLength);

	
	for (int i = 0; i < count; ++i) {
		/// Get the name of the ith uniform
		glGetActiveUniform(shaderID, i, maxUniformListLength, &actualLen, &size, &type, name);
		/// Get the (unsigned int) loc for this uniform
		loc = glGetUniformLocation(shaderID, name);
		std::string uniformName = name;
		uniformMap[uniformName] = loc;

		printf("\"%s\" loc:%d\n", uniformName.c_str() , uniformMap[uniformName]);
	}
	free(name);
}

