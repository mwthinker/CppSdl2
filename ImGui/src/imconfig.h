#ifndef IMGUI_IMCONFIG_H
#define IMGUI_IMCONFIG_H

#include <glm/vec4.hpp>
#include <glm/vec2.hpp>

#define IM_VEC2_CLASS_EXTRA \
constexpr ImVec2(const glm::vec2& f) noexcept : x(f.x), y(f.y) {} \
constexpr operator glm::vec2() const noexcept { return {x, y}; }

#define IM_VEC4_CLASS_EXTRA \
constexpr ImVec4(const glm::vec4& f) noexcept : x(f.x), y(f.y), z(f.z), w(f.w) {} \
constexpr operator glm::vec4() const noexcept { return {x, y, z, w}; }

#endif
