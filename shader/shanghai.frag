#version 320 es

precision mediump float;
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture2;

void main()
{
   FragColor = texture(texture2, TexCoord);
}
