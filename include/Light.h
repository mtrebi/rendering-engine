#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {
protected:
  glm::vec3 m_ambient;
  glm::vec3 m_diffuse;
  glm::vec3 m_specular;
public:
  Light(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular) {
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
  }

  inline const glm::vec3 ambient(const glm::vec3 position) const {
    return m_ambient * LightIntensity(position);
  };
  inline const glm::vec3 diffuse(const glm::vec3 position) const { 
    return m_diffuse * LightIntensity(position);
  };
  inline const glm::vec3 specular(const glm::vec3 position) const { 
    return m_specular * LightIntensity(position);
  };


  // Returns outgoing light vector at given position
  virtual const glm::vec3 LightVector(const glm::vec3 position) const = 0;
  
  // Returns light intensity at given position
  virtual const float LightIntensity(const glm::vec3 position) const = 0;

};