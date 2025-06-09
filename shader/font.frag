#version 320 es

precision mediump float;
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D atlas;

void main() {
    vec4 pixel = texture(atlas, TexCoord);
    if (pixel.r < 0.1) {
        discard;
    }

    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
