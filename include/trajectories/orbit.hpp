#pragma once

#include <glm/glm.hpp>

namespace trajectories {

struct orbit_keplerian {
    // ellipse shape and size
    float eccentricity;
    float semi_major_axis;
    // orbital plane
    float inclination;
    float longitude_of_ascending_node;
    // orientation of ellipse on orbital plane
    float argument_of_periapsis;
    // current location of body along orbit
    float true_anomaly;
    // orbital period
    float period;

    glm::vec3 position_at_time(float t) const;
    float true_anomaly_at_time(float t) const;
    float mean_anomaly_at_time(float t) const;
    float eccentric_anomaly_at_time(float t) const;
};




}