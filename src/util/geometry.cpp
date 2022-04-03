#include "geometry.hpp"
#include <vector>

std::vector<glm::vec3> convertVectorTovec3(const std::vector<tinyobj::real_t>&v) {
    std::vector<glm::vec3> ret(v.size()/3);
    for(size_t n = 0; n < v.size()/3; ++n) {
        ret.at(n) =(glm::vec3(v.at(n*3+0),
                              v.at(n*3+1),
                              v.at(n*3+2)));
    }
    return ret;
}

std::vector<glm::ivec3> convertVectorToivec3(const std::vector<tinyobj::index_t> &v) {
    std::vector<glm::ivec3> ret(v.size()/3);
    for(size_t n = 0; n < v.size()/3; ++n) {

        ret.at(n) =(glm::ivec3(v.at(n*3+0).vertex_index,
                               v.at(n*3+1).vertex_index,
                               v.at(n*3+2).vertex_index));
    }
    return ret;
}
