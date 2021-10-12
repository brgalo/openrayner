#include "memory"

class Vertex {
    float pos[3];
};

class Mesh {
    std::shared_ptr<Vertex> vertBuf;
    std::shared_ptr<float> idxBuf;
    
};