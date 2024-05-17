#version 320 es

layout (location = 0) in vec2 aPos;

precision mediump float;

void main() {
    gl_Position = vec4(aPos.xy, 0.0, 1.0);
}
