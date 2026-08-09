#version 320 es

precision highp float;

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform vec2 screenGeometry;
uniform vec2 position;

out vec2 QuadUV;

void main() {
    // Transform from cartesian to normalized device coordinates
    vec2 pos = aPos + position;
    vec2 ndc = (pos - screenGeometry / 2.0) / screenGeometry * 2.0;
    gl_Position = vec4(ndc.x, ndc.y, 0.0, 1.0);

    // Propagate UV coordinates to fragment for math
    QuadUV = aTexCoord;
}
