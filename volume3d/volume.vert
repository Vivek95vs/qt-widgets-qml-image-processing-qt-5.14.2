#version 330 core

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 texCoord;

out vec2 vTexCoord;
out vec3 vRayDirection;

uniform vec3 dimensions;
uniform mat4 modelViewProjection;

void main() {
    vTexCoord = texCoord;
    
    // Calculate ray direction for volume rendering
    vec4 worldPos = vec4((vertex * 2.0 - 1.0), 0.0, 1.0);
    vRayDirection = normalize(worldPos.xyz);
    
    gl_Position = vec4(vertex * 2.0 - 1.0, 0.0, 1.0);
}