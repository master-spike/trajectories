#include "orbit_view_window.hpp"

#include <GL/gl.h>
#include <GLES3/gl32.h>
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

#include "load_shaders.hpp"

#include <chrono>
#include <iostream>

namespace trajectories {

static GLuint g_orbitShaderProgram = 0;

void orbit_view_window::draw_orbit(orbit_keplerian const& orbit, glm::vec3 center, glm::mat4 const& view)
{
    if (g_orbitShaderProgram == 0) {
        g_orbitShaderProgram = load_shaders(
            "shaders/orbit_shader.vert",
            "shaders/orbit_shader.frag",
            "shaders/orbit_shader.geom"
        );
    }

    float vertices[] = {
        0.0f, 0.0f, 0.0f
    };

    unsigned int indices[] = {
        0
    };

    static GLuint VBO = 0;
    static GLuint VAO = 0;
    static GLuint EBO = 0;
    if (VBO == 0) {

        glGenVertexArrays(1, &VAO);  
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    
    }

    glUseProgram(g_orbitShaderProgram);

    glm::mat4 projection = glm::perspective(90.0f, 1.0f, 0.001f, 10000.0f);
    glUniformMatrix4fv(glGetUniformLocation(g_orbitShaderProgram, "projection"), 1, false, (float*)&projection);

    glm::mat4 my_view(view);
    glUniformMatrix4fv(glGetUniformLocation(g_orbitShaderProgram, "view"), 1, false, (float*)&my_view);

    glUniform1f(glGetUniformLocation(g_orbitShaderProgram, "eccentricity"), orbit.eccentricity);
    glUniform1f(glGetUniformLocation(g_orbitShaderProgram, "semiMajorAxis"), orbit.semi_major_axis);
    glUniform1f(glGetUniformLocation(g_orbitShaderProgram, "inclination"), orbit.inclination);
    glUniform1f(glGetUniformLocation(g_orbitShaderProgram, "longitudeOfAscendingNode"), orbit.longitude_of_ascending_node);
    glUniform1f(glGetUniformLocation(g_orbitShaderProgram, "argumentOfPeriapsis"), orbit.argument_of_periapsis);
    
    glUniform1f(glGetUniformLocation(g_orbitShaderProgram, "trueAnomalyFirst"), orbit.true_anomaly);
    glUniform1f(glGetUniformLocation(g_orbitShaderProgram, "trueAnomalyLast"), orbit.true_anomaly + M_PI * 2);

    glUniform3fv(glGetUniformLocation(g_orbitShaderProgram,"orbitalCenter"), 1, &center[0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void orbit_view_window::draw_window()
{
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    
    ImGui::SetNextWindowPos(ImVec2(m_x, m_y), ImGuiCond_Appearing);
    ImGui::SetNextWindowSize(ImVec2(m_w + 2, m_h + 1 + ImGui::GetFrameHeight()), ImGuiCond_Always);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    if (!ImGui::Begin("System", &m_open))
    {
        ImGui::End();
        ImGui::PopStyleVar();
        return;
    }

    ImGui::PopStyleVar();

    
    if (m_texture != 0) {
        auto pos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddImage(
            (void*)(intptr_t)m_texture,
            pos,
            ImVec2(pos.x + m_w, pos.y + m_h),
            ImVec2(0,1), ImVec2(1,0)
        );
    }
    
    ImGui::End();
}

void orbit_view_window::render_scene(scene const&) {

    static std::chrono::system_clock::time_point start;
    static bool time_set = false;
    if (!time_set) {
        start = std::chrono::system_clock::now();
        time_set = true;
    }

    static GLuint prog_id = 0;

    if (prog_id == 0) {
        prog_id = load_shaders("shaders/axis_shader.vert", "shaders/axis_shader.frag", "shaders/axis_shader.geom");
    }

    if (m_texture == 0) {

        std::cout << "creating texture" << std::endl;

        GLuint tex;
        // if no texture, generate it
        glGenFramebuffers(1, &m_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        glGenTextures(1, &tex);

        glBindTexture(GL_TEXTURE_2D, tex);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_w, m_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // Set "renderedTexture" as our colour attachement #0
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);

        // Set the list of draw buffers.
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

        m_texture = tex;

    }
    
    float vertices[] = {
        0.0f, 0.0f, 0.0f
    };

    unsigned int indices[] = {
        0
    };

    static GLuint VBO = 0;
    static GLuint VAO = 0;
    static GLuint EBO = 0;
    if (VBO == 0) {

        glGenVertexArrays(1, &VAO);  
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glViewport(0,0,m_w,m_h);

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(prog_id);

    glm::mat4 projection = glm::perspective(90.0f, 1.0f, 0.001f, 10000.0f);
    glUniformMatrix4fv(glGetUniformLocation(prog_id, "projection"), 1, false, (float*)&projection);
    glUniform1f(glGetUniformLocation(prog_id, "axis_size"), GLfloat(1.0f));

    auto current_time = std::chrono::system_clock::now();
    float time = (current_time - start).count() / 1000000000.0f;

    glm::vec3 cameraPos = glm::vec3(4.0f * cos(time), 3.0f, 4.0f * sin(time));
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDir = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDir));
    glm::vec3 cameraUp = glm::cross(cameraDir, cameraRight);
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

    glUniformMatrix4fv(glGetUniformLocation(prog_id, "modelview"), 1, false, (float*)&view);

    glBindVertexArray(VAO);
    glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    orbit_keplerian k;
    k.argument_of_periapsis = 0;
    k.semi_major_axis = 1.0f;
    k.inclination = M_PI / 4;
    k.longitude_of_ascending_node = 0.0f;
    k.true_anomaly = 0.0f;
    k.eccentricity = 0.5f;

    draw_orbit(k, glm::vec3(0,0,0), view);

    k.semi_major_axis = 0.2f;
    k.inclination = 0.0f;
        k.eccentricity = 0.99f;
    draw_orbit(k, glm::vec3(0,0,1.0), view);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

}
