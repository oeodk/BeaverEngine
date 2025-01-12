#pragma once
#include "BeaverEngine/Core/std.h"
#include <glm/glm.hpp>
#include "BeaverEngine/Enums/E_RenderDimension.h"
#include "BeaverEngine/Utils/Buffer.h"
#include "BeaverEngine/Utils/Vertex.h"

namespace bv
{
	class Window;
	class View2D;
	//class View3D
	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(const Renderer&) = default;
		Renderer(Renderer&&) = default;
		Renderer& operator=(const Renderer&) = default;
		Renderer& operator=(Renderer&&) = default;

		~Renderer() = default;

		static Renderer* getInstance();

		virtual void setup() = 0;
		virtual void setData(std::string_view var_name, int data) = 0;
		virtual void setData(std::string_view var_name, float data) = 0;
		virtual void setData(std::string_view var_name, const glm::vec2& data) = 0;
		virtual void setData(std::string_view var_name, const glm::vec3& data) = 0;
		virtual void setData(std::string_view var_name, const glm::vec4& data) = 0;
		virtual void setData(std::string_view var_name, const glm::mat4& data) = 0;
		//Uniform set for texture to add

		virtual void render(const VertexBuffer<Vertex2D>& vertex_buffer, const IndexBuffer& index_buffer, Window* window, View2D* view) = 0;
		virtual void render(const VertexBuffer<Vertex3D>& vertex_buffer, const IndexBuffer& index_buffer, Window* window/*, Camera* camera*/) = 0;

		virtual void begin2DRender(Window* window) const = 0;
		virtual void begin3DRender(Window* window) const = 0;

		virtual void end2DRender() = 0;
		virtual void end3DRender() = 0;

		virtual void startComponentCreation() const = 0;
		virtual void endComponentCreation() const = 0;

	protected:
	};
}

