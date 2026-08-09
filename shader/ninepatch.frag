#version 320 es

precision highp float;

in vec2 QuadUV;
out vec4 FragColor;

uniform sampler2D patchTexture;
uniform vec2 patchSize; // Render size in pixels
uniform vec2 textureSize; // Texture size in pixels
uniform vec4 centerRect; // Texture's center patch coords + size

float slice(const float dest, const float destSize, const float srcSize, const float lead, const float trail) {
    float destLead = lead;
    float destTrail = trail;
    float borders = lead + trail;
    if (borders > destSize) {
        // Destination is too small, just render in the center of the texture
        destLead = lead / borders * destSize;
        destTrail = trail / borders * destSize;
    }

    if (dest < destLead) {
        return dest / destLead * lead;
    }
    if (dest > destSize - destTrail) {
        return srcSize - (destSize - dest) / destTrail * trail;
    }

    // Avoid NaN when destination is too small
    float destCenter = max(destSize - destLead - destTrail, 1e-5);
    return lead + (dest - destLead) / destCenter * (srcSize - lead - trail);
}

void main() {
    // Compensate for stbi_set_flip_vertically_on_load(false) since the texture and patch coordinates are top-left
    vec2 dest = vec2(QuadUV.x, 1.0 - QuadUV.y) * patchSize;

    vec2 lead = centerRect.xy;
    vec2 trail = textureSize - centerRect.xy - centerRect.zw;

    vec2 src = vec2(
        slice(dest.x, patchSize.x, textureSize.x, lead.x, trail.x),
        slice(dest.y, patchSize.y, textureSize.y, lead.y, trail.y)
    );

    FragColor = texture(patchTexture, src / textureSize);
}
