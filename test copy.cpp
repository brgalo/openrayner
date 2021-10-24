#include "3rdParty/glm/glm/glm.hpp"
#include "3rdParty/glm/gtc/type_ptr.hpp"
#include <vector>

class Test {
    public:
    std::vector<glm::vec3> vertices;

    bool loadStuff();
};

bool Test::loadStuff() {
    vertices = glm::make_vec3{1,1,1};
}