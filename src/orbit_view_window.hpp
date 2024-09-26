#pragma once

#include <GL/gl.h>
#include <glm/glm.hpp>

#include "scene.hpp"



namespace trajectories {

class orbit_view_window {
private:
    int m_x = 0;
    int m_y = 0;
    int m_w = 0;
    int m_h = 0;
    bool m_open = false;
    GLuint m_texture = 0;
    GLuint m_framebuffer = 0;

    glm::mat4 camera;

void draw_orbit(orbit_keplerian const& orbit, glm::vec3 center, glm::mat4 const& view);

public:
    void draw_window();
    void render_scene(scene const& scene);

    void set_open(bool open) {
        m_open = open;
    }

    bool get_open() const {
        return m_open;
    }

    void set_size(int w, int h) {
        m_w = w;
        m_h = h;
    }

    int get_w() const {
        return m_w;
    }

    int get_h() const {
        return m_h;
    }

    void set_pos(int x, int y) {
        m_x = x;
        m_y = y;
    }

    int get_x() const {
        return m_x;
    }

    int get_y() const {
        return m_y;
    }


};

}
