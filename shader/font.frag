#version 320 es

precision mediump float;
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D fontTexture;
uniform vec4 color;

void main() {
    vec4 pixel = texture(fontTexture, TexCoord);
    if (pixel.r < 0.1) {
        discard;
    }

    FragColor = color / 255.0f;
}
