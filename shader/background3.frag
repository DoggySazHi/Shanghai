// [Buffer A] : SDF texture
// [Buffer B] : Accumulated raytraced image
// [Image]    : Final image

//   [Buffer A]
//      |
//      | +-------+
//      | |       |
//      V V       |
//   [Buffer B]---+
//       |
//       V
//    [Image]
//
vec2 pix(int x, int y)
{
    return vec2(x,y)/iResolution.xy;
}

uniform highp sampler2D iChannel0;
layout(location = 0) out vec4 fragColor;

void main()
{
    vec2 uv = fragCoord.xy/iResolution.xy;
    vec4 image = texture(iChannel0, uv) / float(iFrame + 1);
    fragColor = image;
}