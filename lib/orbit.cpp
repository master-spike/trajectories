#include "trajectories/orbit.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <cmath>

namespace trajectories {

// position relative to orbited body
glm::vec3 orbit_keplerian::position_at_time(float t) const {
    using glm::vec3;
    using glm::mat3;
    using glm::mat4;

    float ta = true_anomaly_at_time(t);
    float ea = eccentric_anomaly_at_time(t);
    float longitude = ta + argument_of_periapsis + longitude_of_ascending_node;
    float r = semi_major_axis * (1 - eccentricity * cosf(ea));
    
    glm::vec3 ascending_node_axis = glm::rotate(
        glm::vec3{1,0,0},
        longitude_of_ascending_node,
        glm::vec3{0,1.0f,0}
    );
    mat4 transform = glm::rotate(
        glm::rotate(mat4(), longitude, vec3{0,1.0f,0}),
        inclination,
        ascending_node_axis
    );

    return mat3(transform) * glm::vec3{r,0,0};
    
    
}

float orbit_keplerian::true_anomaly_at_time(float t) const {
    float a = eccentric_anomaly_at_time(t);
    float b = eccentricity / (1 + sqrtf(1 - eccentricity * eccentricity));
    return a + 2 * atanf(b * sinf(a) / (1 - b * cosf(a)));
}

float orbit_keplerian::mean_anomaly_at_time(float t) const {
    float a = sqrtf(1 - eccentricity * eccentricity);
    float sf = sinf(true_anomaly);
    float cf = cosf(true_anomaly);

    // source: wikipedia page for mean anomaly
    float mean_anomaly_at_0 = atan2f(-a * sf, -eccentricity - cf) + M_PI
        - eccentricity * a * sf / (1 + eccentricity * cf);

    // chop time to fraction of orbital period
    t = std::fmodf(t, period);
    
    float mean_anomaly_at_t = t / period + mean_anomaly_at_0;
    if (mean_anomaly_at_t >= 2 * M_PI) mean_anomaly_at_t -= 2 * M_PI;
}

float orbit_keplerian::eccentric_anomaly_at_time(float t) const {
    float mean_anomaly = mean_anomaly_at_time(t);

    // Solve  E - e*sin(E) - M = 0, for r = E
    // newton's method
    // use approximation sin(x) = x to obtain an initial solution
    // r - e - M = 0
    // r = e + M
    float r = eccentricity + mean_anomaly;
    for (int i = 0; i < 16; ++i) {
        // derivative of E - e*sin(E) - M is 1 - e*cos(E)
        r -= (r - eccentricity * sinf(r)) / (1 - eccentricity * cosf(r));
    }

    return r;
}

}
