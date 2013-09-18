/**
 * File contains declaration of Shader class.
 * @file    Shader.hpp
 * @author  Szymon "Veldrin" Jab³oñski
 * @date    2011-07-12
 * 
 * Projekt C-Way
 * Efekty nieba, chmur i s³oñca w grze 2D z widokiem z boku
 * Szymon Jab³oñski
 * pod kierunkiem Tomasza Martyna
 * Instytut Informatyki
 * Politechnika Warszawska
 */

#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <fstream>
#include <iostream>

namespace AyumiEngine
{
	namespace AyumiResource
	{
		/**
		 * Class represents one of basic Engine resource - Shader. Shaders are simple programs in GLSL
		 * language which is used to communicate with GPU. There are three types of shaders: vertex,
		 * and fragment. There are used to render and rasterize geoemtry, create all kind of
		 * spiecial effects, post-processing, lights, material, geoemtry etc. 
		 */
		class Shader
		{
		private:
			unsigned int shaderProgram;
			unsigned int shaderVertex;
			unsigned int shaderFragment;

			char* loadShaderSource(const char* shaderFileName);
			char* loadShaderSource(const char* shaderFileName, const char* commonFileName);
		public:
			
			Shader();
			~Shader();

			void loadShader(const std::string& vertPath, const std::string& fragPath);

			void loadShader(const std::string& vertPath, const std::string& fragPath, const std::string& commonPath);

			void bindShader();
			void unbindShader();
			void setUniformf(const std::string& name, const float value);
			void setUniformi(const std::string& name, const int value);
			void setUniform2f(const std::string& name, const float value, const float value2);
			void setUniform3f(const std::string& name, const float value, const float value2, const float value3);
			void setUniform3fv(const std::string& name, const float* valueArray);
			void setUniform4fv(const std::string&name, const float* valueArray);
			void setUniformTexture(const std::string& name, const int unit);
			void setUniformMatrix3fv(const std::string& name,const float* valueArray);
			void setUniformMatrix4fv(const std::string& name,const float* valueArray);

			unsigned int getProgram() {
				return shaderProgram;
			}
		};
	}
}
#endif
