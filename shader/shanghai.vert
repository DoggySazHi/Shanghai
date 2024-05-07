#version 320 es

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform vec2 screenGeometry;

precision mediump float;
out vec2 TexCoord;

void main()
{
   // Transform from cartesian to normalized device coordinates
   gl_Position = vec4((aPos.x - 1.0f) / screenGeometry.x * 2.0f, (aPos.y - 1.0f) / screenGeometry.y * 2.0f, 0.0, 1.0);
   TexCoord = aTexCoord;
}
