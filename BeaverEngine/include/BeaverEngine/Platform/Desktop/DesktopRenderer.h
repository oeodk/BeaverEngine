#pragma once
#include "BeaverEngine/Core/Renderer.h"
#include <glad/glad.h>
#include "BeaverEngine/Core/GlobalConstants.h"

namespace bv
{
    class DesktopRenderer :
        public Renderer
    {
    public:
       void setup() override;
       void setData(std::string_view var_name, int data) override;
       void setData(std::string_view var_name, float data) override;
       void setData(std::string_view var_name, const glm::vec2& data) override;
       void setData(std::string_view var_name, const glm::vec3& data) override;
       void setData(std::string_view var_name, const glm::vec4& data) override;
       void setData(std::string_view var_name, const glm::mat4& data) override;

       void render(const VertexBuffer<Vertex2D>& vertex_buffer, const IndexBuffer& index_buffer, Window* window, View2D* view) override;
       void render(const VertexBuffer<Vertex3D>& vertex_buffer, const IndexBuffer& index_buffer, Window* window/*, Camera* camera*/) override;

       void begin2DRender(Window* window) const override;       
       void begin3DRender(Window* window) const override;

       void end2DRender() override; 
       void end3DRender() override;

       void startComponentCreation() const override 
       {
           switch (constants::RENDER_MODE)
           {
           case RenderDimension::RENDER_2D:
               glUseProgram(shader_program_2d_);
               break;
           case RenderDimension::RENDER_3D:
               break;
           }
       }
       void endComponentCreation() const override
       {
           switch (constants::RENDER_MODE)
           {
           case RenderDimension::RENDER_2D:
               glUseProgram(0);
               glBindVertexArray(0);
               break;
           case RenderDimension::RENDER_3D:
               break;
           }
       }

    private:
        void printGlError() const;

        std::string loadShaderSource(std::string_view filename);

        void loadShader(GLuint& program, std::string_view vertex_shader_path, std::string_view frag_shader_path);

        GLuint shader_program_2d_;
        std::map<std::string, GLint> uniforms_location_2d_;
    };
}

