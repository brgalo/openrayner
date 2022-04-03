#pragma once

#include "memory"
#include <climits>
#include <cstdint>
#include <memory>
#include <vector>
#include "../../3rdParty/glm/glm/glm.hpp"


#include "../../3rdParty/tinyOBJ/tiny_obj_loader.h"

struct Vertex {
    Vertex(float X, float Y, float Z) : x(X),y(Y),z(Z) {};
    const float x,y,z;
};

struct Pos {
    Pos(Vertex A, Vertex B, Vertex C) : a(A),b(B),c(C) {};
    const Vertex a,b,c;
};

class Mesh {
    public:
    Mesh(const tinyobj::shape_t &shape, uint32_t off, uint32_t ID) : name(shape.name), id(ID),offset(off),n(shape.mesh.indices.size()/3) {};
    const std::string name;
    const uint32_t id;
    const uint32_t offset;
    const uint32_t n;
};

struct PrimitiveData {
    PrimitiveData(Vertex A, Vertex B, Vertex C, Vertex N) : p(Pos(A,B,C)),n(N) {};
    const Pos p;
    const Vertex n;
};

struct Geometry {
    Geometry() {};
    Geometry(std::vector<glm::vec3> &vertices,
             std::vector<glm::vec3> &normals,
             std::vector<glm::uvec3> &idx,
             std::vector<Mesh> &m) : vertexBuf(vertices),normBuf(normals),indices(idx),meshes(m) {};
    std::vector<glm::vec3> vertexBuf;
    std::vector<glm::vec3> normBuf;
    std::vector<glm::uvec3> indices;
    std::vector<Mesh> meshes;
};

glm::vec3 floatTovec3(const float* x);

std::vector<glm::vec3> convertVectorTovec3(const std::vector<tinyobj::real_t>&v);
std::vector<glm::ivec3> convertVectorToivec3(const std::vector<tinyobj::index_t>&v);