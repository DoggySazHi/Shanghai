#version 320 es

precision mediump float;
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D patchTexture;
uniform vec4 color;

void main() {
    vec4 pixel = texture(patchTexture, TexCoord);
    if (pixel.r < 0.1) {
        discard;
    }

    FragColor = color / 255.0f;
}
