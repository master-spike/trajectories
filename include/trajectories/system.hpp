#pragma once

#include "body.hpp"
#include "orbit.hpp"

#include <vector>
#include <unordered_map>

namespace trajectories {

class system {
private:
    std::vector<body> m_bodies;
    std::unordered_map<int, orbit_keplerian> m_body_orbits;
public:
    const orbit_keplerian* get_orbit(int body_id) const;
    orbit_keplerian* get_orbit(int body_id);
};

}
