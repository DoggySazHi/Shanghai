#version 320 es

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform vec2 screenGeometry;

uniform vec2 shanghaiPosition;
uniform bool shanghaiHFlip;

precision mediump float;
out vec2 TexCoord;

void main() {
   // Transform from cartesian to normalized device coordinates
   vec2 pos = aPos + shanghaiPosition;
   vec2 ndc = vec2((pos.x - screenGeometry.x / 2.0) / screenGeometry.x * 2.0f, (pos.y - screenGeometry.y / 2.0) / screenGeometry.y * 2.0f);
   gl_Position = vec4(ndc.x, ndc.y, 0.0, 1.0);

   if (shanghaiHFlip) {
      TexCoord = vec2(1.0 - aTexCoord.x, aTexCoord.y);
   }
   else {
      TexCoord = aTexCoord;
   }
}
