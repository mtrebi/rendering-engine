#pragma once

#include "Light.h"

class PointLight : public Light {
private:
  glm::vec3 m_position;

  float m_constant = 1.0f,
    m_linear = 0.09f,
    m_quadratic = 0.032f;
public:
  PointLight(const glm::vec3 position, const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular)
    : Light(ambient, diffuse, specular) {
    m_position = position;
  }

  // Returns outgoing light vector at given position
  const glm::vec3 LightVector(const glm::vec3 position) const override {
    return glm::normalize(m_position - position);
  }

  // Returns light intensity at given position
  virtual const float LightIntensity(const glm::vec3 position) const override {
    const float distance = glm::distance(position, m_position);
    const float attenuation = 1.0f / (m_constant + m_linear * distance + m_quadratic * (distance * distance));
    
    return (1.0f - attenuation);
  }
};