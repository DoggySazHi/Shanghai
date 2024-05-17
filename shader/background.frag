#version 320 es

precision mediump float;

uniform float time;
uniform vec2 resolution;

out vec4 FragColor;

#define PI 3.14159265359

void main(void) {
    // Colors of the sine waves
    vec3 rColor = vec3(0.9, 0.0, 0.3);
    vec3 gColor = vec3(0.0, 0.9, 0.3);
    vec3 bColor = vec3(0.0, 0.3, 0.9);
    vec3 yColor = vec3(0.9, 0.9, 0.3);

    // Normalized coordinates
    vec2 p = (2.0 * gl_FragCoord.xy - resolution) / min(resolution.x, resolution.y);

    // Offset the sine waves
    float a = sin(0.5 * p.y - time * 0.1) / 1.0;
    float b = sin(0.5 * p.y - time * 0.2) / 1.0;
    float c = sin(0.5 * p.y - time * 0.3 + PI) / 1.0;
    float d = sin(0.5 * p.y - time * 0.5 + PI) / 1.0;

    // Calculate the color
    float e = 0.051 / abs(p.x + a);
    float f = 0.051 / abs(p.x + b);
    float g = 0.051 / abs(p.x + c);
    float h = 0.051 / abs(p.x + d);

    // Blend the colors
    vec3 color = rColor * e + gColor * f + bColor * g + yColor * h;
    FragColor = vec4(color, 1.0);
}