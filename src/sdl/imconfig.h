#ifndef CPPSDL2_IMCONFIG_H
#define CPPSDL2_IMCONFIG_H

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

#define IM_VEC2_CLASS_EXTRA \
constexpr ImVec2(const glm::vec2& f) : x(f.x), y(f.y) {} \
constexpr operator glm::vec2() const { return {x, y}; }

#define IM_VEC4_CLASS_EXTRA \
constexpr ImVec4(const glm::vec4& f) : x(f.x), y(f.y), z(f.z), w(f.w) {} \
constexpr operator glm::vec4() const { return {x, y, z, w}; }

#endif
