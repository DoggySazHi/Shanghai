// ================
// Pre-render SDF
// ================
out vec4 fragColor;

void main()
{
    vec2 uv = (gl_FragCoord.xy-iResolution.xy*0.5)/(iResolution.y*2.0);
    vec2 pos = uv*20.0;
    vec2 dist = getDist(pos);
    fragColor = vec4(pos, dist.x, dist.y);
}