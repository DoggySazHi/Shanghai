// ================
// Pre-render SDF
// ================
layout(location = 0) out highp vec4 fragColor;

void main()
{
    if (iFrame < 5) {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    vec2 uv = (gl_FragCoord.xy-iResolution.xy*0.5)/(iResolution.y*2.0);
    vec2 pos = uv*20.0;
    vec2 dist = getDist(pos);
    fragColor = vec4(pos, dist.x, dist.y);
}