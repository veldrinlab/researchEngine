/**
 * File contains definition of Shader class.
 * @file    Shader.hpp
 * @author  Szymon "Veldrin" Jab³oñski
 * @date    2011-08-10
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#include "Shader.hpp"

using namespace std;

namespace AyumiEngine
{
	namespace AyumiResource
	{
		/**
		 * Class default constructor. Set object default values. 
		 */
		Shader::Shader()
		{
			shaderProgram = 0;
			shaderVertex = 0;
			shaderFragment = 0;
		}

		/**
		 * Class destructor, free allocated memory. Detach and delete shader program.
		 */
		Shader::~Shader()
		{
			if(shaderFragment != 0)
			{
				glDetachShader(shaderProgram, shaderFragment);
				glDeleteShader(shaderFragment);
			}
	
			if(shaderVertex != 0)
			{
				glDetachShader(shaderProgram, shaderVertex);
				glDeleteShader(shaderVertex);
			}
			glDeleteProgram(shaderProgram);
		}
	
		/**
		 * Method is used to load shader from external file.
		 * @param	vertPath is vertex shader source file path.
		 * @param	fragPath is fragment shader source file path.
		 */
		void Shader::loadShader(const string& vertPath, const string& fragPath)
		{
			shaderVertex = glCreateShader(GL_VERTEX_SHADER);
			shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
			
			const char* vertexShaderSource = loadShaderSource(vertPath.c_str());
			const char* fragmentShaderSource = loadShaderSource(fragPath.c_str());

			if(vertexShaderSource == nullptr || fragmentShaderSource == nullptr)
				cerr <<  "Shader loading error detected: " << vertPath.c_str() << " " << fragPath.c_str() << endl;
			
			glShaderSource(shaderVertex,1,&vertexShaderSource,0);
			glShaderSource(shaderFragment,1,&fragmentShaderSource,0);	
			glCompileShader(shaderVertex);

			GLint result;
			glGetShaderiv(shaderVertex, GL_COMPILE_STATUS,&result);
		
			if(result == GL_FALSE)
				std::cout << "Could not compile shader: " << shaderVertex << ": " << vertPath.c_str() << std::endl;
    
			glCompileShader(shaderFragment);
			glGetShaderiv(shaderFragment, GL_COMPILE_STATUS,&result);
			if(result == GL_FALSE)
				std::cout << "Could not compile shader: " << shaderFragment << ": " << fragPath.c_str() << std::endl;
        
			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram,shaderVertex);
			glAttachShader(shaderProgram,shaderFragment);
			glLinkProgram(shaderProgram);

			delete vertexShaderSource;
			delete fragmentShaderSource;

		}

		void Shader::loadShader(const string& vertPath, const string& fragPath, const string& commonPath)
		{
			shaderVertex = glCreateShader(GL_VERTEX_SHADER);
			shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
			
			const char* vertexShaderSource = loadShaderSource(vertPath.c_str());
			const char* fragmentShaderSource = loadShaderSource(fragPath.c_str(),commonPath.c_str());

			if(vertexShaderSource == nullptr || fragmentShaderSource == nullptr)
				cerr <<  "Shader loading error detected: " << vertPath.c_str() << " " << fragPath.c_str() << endl;
			
			glShaderSource(shaderVertex,1,&vertexShaderSource,0);
			glShaderSource(shaderFragment,1,&fragmentShaderSource,0);	
			glCompileShader(shaderVertex);

			GLint result;
			glGetShaderiv(shaderVertex, GL_COMPILE_STATUS,&result);
		
			if(result == GL_FALSE)
				std::cout << "Could not compile shader: " << shaderVertex << ": " << vertPath.c_str() << std::endl;
    
			glCompileShader(shaderFragment);
			glGetShaderiv(shaderFragment, GL_COMPILE_STATUS,&result);
			if(result == GL_FALSE)
				std::cout << "Could not compile shader: " << shaderFragment << ": " << fragPath.c_str() << std::endl;
        
			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram,shaderVertex);
			glAttachShader(shaderProgram,shaderFragment);
			glLinkProgram(shaderProgram);

			delete vertexShaderSource;
			delete fragmentShaderSource;
		}

		/**
		 * Method is used to bind shader program.
		 */
		void Shader::bindShader()
		{
			glUseProgram(shaderProgram);
		}

		/**
		 * Method is used to unbind shader program.
		 */
		void Shader::unbindShader()
		{
			glUseProgram(0);
		}

		/**
		 * Method is used to set uniform float value to shader program.
		 * @param	name is uniform name which is used to shader program.
		 * @param	value is uniform value.
		 */
		void Shader::setUniformf(const string& name, const float value)
		{
			glUniform1f(glGetUniformLocation(shaderProgram,name.c_str()),value);
		}

		/**
		 * Method is used to set uniform int value to shader program.
		 * @param	name is uniform name which is used to shader program.
		 * @param	value is uniform value.
		 */
		void Shader::setUniformi(const string& name, const int value)
		{
			glUniform1i(glGetUniformLocation(shaderProgram,name.c_str()),value);
		}

		/**
		 * Method is used to set uniform float vec2 to shader program.
		 * @param	name is uniform name which is used to shader program.
		 * @param	value is uniform vec2 first value.
		 * @param	value is uniform vec2 second value.
		 */
		void Shader::setUniform2f(const string& name, const float value, const float value2)
		{
			glUniform2f(glGetUniformLocation(shaderProgram,name.c_str()),value,value2);
		}

		/**
		 * Method is used to set uniform float vec3 to shader program.
		 * @param	name is uniform name which is used to shader program.
		 * @param	value is uniform vec2 first value.
		 * @param	value is uniform vec2 second value.
		 * @param	value is uniform vec3 third value.
		 */
		void Shader::setUniform3f(const std::string& name, const float value, const float value2, const float value3)
		{
			glUniform3f(glGetUniformLocation(shaderProgram,name.c_str()),value,value2,value3);
		}

		/**
		 * Method is used to set uniform float 3-element array to shader program.
		 * @param	name is uniform name which is used to shader program.
		 * @param	valueArray is pointer to uniform float array.
		 */
		void Shader::setUniform3fv(const string& name, const float* valueArray)
		{
			glUniform3fv(glGetUniformLocation(shaderProgram,name.c_str()),1,valueArray);
		}

		/**
		 * Method is used to set uniform float 4-element array to shader program.
		 * @param	name is uniform name which is used to shader program.
		 * @param	valueArray is pointer to uniform float array.
		 */
		void Shader::setUniform4fv(const string&name, const float* valueArray)
		{
			glUniform4fv(glGetUniformLocation(shaderProgram,name.c_str()),1,valueArray);
		}

		/**
		 * Method is used to set uniform texture unit to shader program.
		 * @param	name is uniform name which is used to shader program.
		 * @param	texture is uniform texture unit.
		 */
		void Shader::setUniformTexture(const string& name, const int unit)
		{
			glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), unit);
		}

		/**
		 * Method is used to set uniform float 3x3 matrix to shader program.
		 * @param	name is uniform name which is used to shader program.
		 * @param	valueArray is pointer to uniform float array.
		 */
		void Shader::setUniformMatrix3fv(const string& name,const float* valueArray)
		{
			glUniformMatrix3fv(glGetUniformLocation(shaderProgram,name.c_str()),1,GL_TRUE,valueArray);
		}

		/**
		 * Method is used to set uniform float 4x4 matrix to shader program.
		 * @param	name is uniform name which is used to shader program.
		 * @param	valueArray is pointer to uniform float array.
		 */
		void Shader::setUniformMatrix4fv(const string& name, const float* valueArray)
		{
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram,name.c_str()),1,GL_TRUE,valueArray);
		}

		/**
		 * Method is used to load Shader source code from file.
		 * @param	shaderFileName is path to shader program source code.
		 * @return	loaded shader source code string.
		 */
		char* Shader::loadShaderSource(const char* shaderFileName)
		{
			char* shaderSource = nullptr;
    
			if(shaderFileName != nullptr)
			{
				ifstream file;
				file.open(shaderFileName, ios::binary);
		
				if(file.is_open())
				{
					file.seekg(0,ios::end);
					int count = static_cast<int>(file.tellg());
					file.seekg(0,ios::beg);
			
					if(count > 0)
					{
						shaderSource = new char[count + 1];
						file.read(shaderSource, count * sizeof(char));
						shaderSource[count] = '\0';
					}
				}
				else
					cerr << "Shader source file loading error detected: " << shaderFileName << endl;
				file.close();
			}

			return shaderSource;
		}

		char* Shader::loadShaderSource(const char* shaderFileName, const char* commonFileName)
		{
			char* commonSource = nullptr;
			char* shaderSource = nullptr;
			char* fullSource = nullptr;
    
			if(shaderFileName != nullptr && commonFileName != nullptr)
			{
				ifstream file;
				file.open(commonFileName, ios::binary);
		
				ifstream file2;
				file2.open(shaderFileName, ios::binary);

				if(file.is_open() && file2.is_open())
				{
					file.seekg(0,ios::end);
					int count = static_cast<int>(file.tellg());
					file.seekg(0,ios::beg);
			
					file2.seekg(0,ios::end);
					int count2 = static_cast<int>(file2.tellg());
					file2.seekg(0,ios::beg);

					if(count > 0)
					{
						commonSource = new char[count];
						file.read(commonSource, count * sizeof(char));
					}

					if(count2 > 0)
					{
						shaderSource = new char[count2];
						file2.read(shaderSource,count2 * sizeof(char));
					}

					fullSource = new char[count+count2+1];

					for(int i = 0; i < count; ++i)
						fullSource[i] = commonSource[i];

					for(int i = 0; i < count2; ++i)
						fullSource[count+i] = shaderSource[i];

					fullSource[count+count2] = '\0';

					delete [] shaderSource;
					delete [] commonSource;
				
				}
				else
					cerr << "Shader source file loading error detected: " << shaderFileName << endl;
				file.close();
			}

			return fullSource;
		}
	}
}
