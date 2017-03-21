#pragma once

#include "Light.h"

class DirectionalLight : public Light {
private:
  glm::vec3 m_direction;
public:
  DirectionalLight(const glm::vec3 direction, const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular)
    : Light(ambient, diffuse, specular){
    m_direction = direction;
  }

  // Returns outgoing light vector at given position
  const glm::vec3 LightVector(const glm::vec3 position) const override {
    return m_direction;
  }

  // Returns light intensity at given position
  virtual const float LightIntensity(const glm::vec3 position) const override {
    return 1.0f;
  }
};