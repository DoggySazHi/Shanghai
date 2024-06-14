// ================
// Pre-render SDF
// ================
layout(location = 0) out vec4 fragColor;

void main()
{
    vec2 uv = (fragCoord.xy-iResolution.xy*0.5)/(iResolution.y*2.0);
    vec2 pos = uv*20.0;
    vec2 dist = getDist(pos);
    fragColor = vec4(pos, dist.x, dist.y);
}