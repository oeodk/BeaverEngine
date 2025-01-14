#include "BeaverEngine/Platform/Desktop/DesktopRenderer.h"
#include <glm/gtc/type_ptr.hpp>
#include "BeaverEngine/Core/GlobalConstants.h"
#include "BeaverEngine/Core/Debug.h"
#include "BeaverEngine/Utils/View2D.h"
#include "BeaverEngine/Utils/Window.h"

namespace bv
{
	Renderer* Renderer::getInstance()
	{
		static DesktopRenderer renderer;
		return &renderer;
	}

	void DesktopRenderer::setup()
	{
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		loadShader(shader_program_2d_, constants:: SHADERS_PATH + "Desktop/2d_render.vert", constants::SHADERS_PATH + "Desktop/2d_render.frag");

		uniforms_location_2d_.insert({ "u_projection_view", 0 });
		uniforms_location_2d_.at("u_projection_view") = glGetUniformLocation(shader_program_2d_, "u_projection_view");

		uniforms_location_2d_.insert({ "u_color", 0 });
		uniforms_location_2d_.at("u_color") = glGetUniformLocation(shader_program_2d_, "u_color");

		uniforms_location_2d_.insert({ "u_width", 0 });
		uniforms_location_2d_.at("u_width") = glGetUniformLocation(shader_program_2d_, "u_width");

		uniforms_location_2d_.insert({ "u_height", 0 });
		uniforms_location_2d_.at("u_height") = glGetUniformLocation(shader_program_2d_, "u_height");

		uniforms_location_2d_.insert({ "u_texture_sampler", 0 });
		uniforms_location_2d_.at("u_texture_sampler") = glGetUniformLocation(shader_program_2d_, "u_texture_sampler");

		//uniforms_location_2d_.insert({ "has_texture", 0 });
		//uniforms_location_2d_.at("has_texture") = glGetUniformLocation(shader_program_2d_, "has_texture");
	}
	void DesktopRenderer::setData(std::string_view var_name, int data)
	{
		if (constants::RENDER_MODE == E_RenderDimension::RENDER_2D)
		{
			glUniform1i(uniforms_location_2d_.at(std::string(var_name)), data);
		}
		else
		{
			// 3D uniform set
		}
	}

	void DesktopRenderer::setData(std::string_view var_name, float data)
	{
		if(constants::RENDER_MODE == E_RenderDimension::RENDER_2D)
		{
			glUniform1f(uniforms_location_2d_.at(std::string(var_name)), data);
		}
		else
		{
			// 3D uniform set
		}
	}
	void DesktopRenderer::setData(std::string_view var_name, const glm::vec2& data)
	{
		if (constants::RENDER_MODE == E_RenderDimension::RENDER_2D)
		{
			glUniform2fv(uniforms_location_2d_.at(std::string(var_name)), 1, glm::value_ptr(data));
		}
		else
		{
			// 3D uniform set
		}
	}
	void DesktopRenderer::setData(std::string_view var_name, const glm::vec3& data)
	{
		if (constants::RENDER_MODE == E_RenderDimension::RENDER_2D)
		{
			glUniform3fv(uniforms_location_2d_.at(std::string(var_name)), 1, glm::value_ptr(data));
		}
		else
		{
			// 3D uniform set
		}
	}
	void DesktopRenderer::setData(std::string_view var_name, const glm::vec4& data)
	{
		if (constants::RENDER_MODE == E_RenderDimension::RENDER_2D)
		{
			glUniform4fv(uniforms_location_2d_.at(std::string(var_name)), 1, glm::value_ptr(data));
		}
		else
		{
			// 3D uniform set
		}
	}
	void DesktopRenderer::setData(std::string_view var_name, const glm::mat4& data)
	{
		if (constants::RENDER_MODE == E_RenderDimension::RENDER_2D)
		{
			glUniformMatrix4fv(uniforms_location_2d_.at(std::string(var_name)), 1, GL_FALSE, glm::value_ptr(data));
		}
		else
		{
			// 3D uniform set
		}
	}
	void DesktopRenderer::render(const VertexBuffer<Vertex2D>& vertex_buffer, const IndexBuffer& index_buffer, Window* window, View2D* view)
	{
		const unsigned int window_width = window->getWidth();
		const unsigned int window_height = window->getHeight();
		glViewport(window_width * view->getViewPortPos().x, window_height * view->getViewPortPos().y, window_width * view->getViewPortSize().x, window_height * view->getViewPortSize().y);
		setData("u_projection_view", view->getTransform());
		setData("u_width", static_cast<float>(window_width));
		setData("u_height", static_cast<float>(window_height));
		setData("u_texture_sampler", 0);

		printGlError();

		vertex_buffer.bind();
		printGlError();

		index_buffer.bind();
		printGlError();

		//Texture things

		glDrawElements(GL_TRIANGLES, index_buffer.getIndexCount(), GL_UNSIGNED_INT, nullptr);

		printGlError();

		vertex_buffer.release();
		index_buffer.release();



		//Texture things
	}

	void DesktopRenderer::render(const VertexBuffer<Vertex3D>& vertex_buffer, const IndexBuffer& index_buffer, Window* window/*, Camera* camera*/)
	{

	}

	void DesktopRenderer::begin2DRender(Window* window) const
	{
		window->makeCurrent();
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		window->clear();
		window->beginRender2d();

		glUseProgram(shader_program_2d_);

		printGlError();
	}

	void DesktopRenderer::begin3DRender(Window* window) const
	{
		window->makeCurrent();
	}

	void DesktopRenderer::end2DRender()
	{
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void DesktopRenderer::end3DRender()
	{
	}

	void DesktopRenderer::printGlError() const
	{
		int err = glGetError();
		if (err != GL_NO_ERROR)
		{
			DEBUG_PRINT_INT(err);
		}
	}

	std::string DesktopRenderer::loadShaderSource(std::string_view filename)
	{
		std::string name(filename);
		std::ifstream file(name);
		if (!file.is_open())
		{
			DEBUG_PRINT_STRING(("Could not open the file: " + name).c_str());
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	void DesktopRenderer::loadShader(GLuint& program, std::string_view vertex_shader_path, std::string_view frag_shader_path)
	{
		GLint success, vertexShader, fragmentShader;
		char infoLog[512];
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const std::string vertexShaderSource = loadShaderSource(vertex_shader_path);
		const char* vs = vertexShaderSource.c_str();
		glShaderSource(vertexShader, 1, &vs, nullptr);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			DEBUG_PRINT_STRING("Vertex Shader Compilation Failed:");
			DEBUG_PRINT_STRING(infoLog);
		}

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		const std::string fragmentShaderSource = loadShaderSource(frag_shader_path);
		const char* fs = fragmentShaderSource.c_str();
		glShaderSource(fragmentShader, 1, &fs, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
			DEBUG_PRINT_STRING("frag Shader Compilation Failed:");
			DEBUG_PRINT_STRING(infoLog);
		}

		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, nullptr, infoLog);
			DEBUG_PRINT_STRING("Shader Program Linking Failed:");
			DEBUG_PRINT_STRING(infoLog);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glUseProgram(0);
	}
}