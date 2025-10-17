/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "meshGen.h"
#include "../renderer/mesh.h"

void Utils::Mesh::generateCube(Renderer::Mesh&mesh, float size) {
  mesh.vertices.clear();
  mesh.indices.clear();

  for (int i=0; i<6; i++) {
    glm::vec3 normal{};
    glm::vec3 tangent{};
    glm::vec3 bitangent{};

    switch(i) {
      case 0: normal = {0, 0, 1}; tangent = {1, 0, 0}; bitangent = {0, 1, 0}; break; // front
      case 1: normal = {0, 0, -1}; tangent = {-1, 0, 0}; bitangent = {0, 1, 0}; break; // back
      case 2: normal = {1, 0, 0}; tangent = {0, 0, -1}; bitangent = {0, 1, 0}; break; // right
      case 3: normal = {-1, 0, 0}; tangent = {0, 0, 1}; bitangent = {0, 1, 0}; break; // left
      case 4: normal = {0, 1, 0}; tangent = {1, 0, 0}; bitangent = {0, 0, -1}; break; // top
      case 5: normal = {0, -1, 0}; tangent = {1, 0, 0}; bitangent = {0, 0, 1}; break; // bottom
    }

    uint16_t startIdx = mesh.vertices.size();

    mesh.vertices.push_back({(normal - tangent - bitangent) * size * 0.5f, normal, {1,1,1,1}, {0,0}});
    mesh.vertices.push_back({(normal + tangent - bitangent) * size * 0.5f, normal, {1,1,1,1}, {1,0}});
    mesh.vertices.push_back({(normal + tangent + bitangent) * size * 0.5f, normal, {1,1,1,1}, {1,1}});
    mesh.vertices.push_back({(normal - tangent + bitangent) * size * 0.5f, normal, {1,1,1,1}, {0,1}});

    mesh.indices.push_back(startIdx + 2);
    mesh.indices.push_back(startIdx + 0);
    mesh.indices.push_back(startIdx + 1);

    mesh.indices.push_back(startIdx + 0);
    mesh.indices.push_back(startIdx + 2);
    mesh.indices.push_back(startIdx + 3);
  }
}
