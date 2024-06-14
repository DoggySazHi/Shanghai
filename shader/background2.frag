// ================
// Raymarch
// ================
uniform highp sampler2D iChannel0;
uniform highp sampler2D iChannel1;

vec2 posToUv(vec2 pos)
{
    return ((pos/10.0)*iResolution.y+iResolution.xy*0.5)/iResolution.xy;
}
vec2 uvToPos(vec2 uv)
{
    return ((uv*iResolution.xy)-iResolution.xy*0.5)/(iResolution.y*2.0)*20.0;
}
vec2 getNormal(vec2 p)
{
    return normalize(vec2(
    texture(iChannel0, posToUv(p+vec2(0.01,0.0))).z-texture(iChannel0, posToUv(p+vec2(-0.01,0.0))).z,
    texture(iChannel0, posToUv(p+vec2(0.0,0.01))).z-texture(iChannel0, posToUv(p+vec2(0.0,-0.01))).z
    ));
}
float rand(vec2 co){
    return fract(sin(dot(co.xy+iTime-iDate.w*0.001,vec2(12.9898,78.233))) * 43758.5453);
}
vec2 randDir(vec2 pos)
{
    float r = rand(pos)*3.14159*2.0;
    return vec2(cos(r),sin(r));
}
vec3 march(vec2 pos, vec2 dir)
{
    float prePenetration = 1.0;
    vec2 d = dir;
    vec2 p = pos;
    vec3 addative = vec3(0);
    if (texture(iChannel0, posToUv(pos)).z < 0.0001)
    {
        float depth = -texture(iChannel0, posToUv(pos)).z;
        vec4 mat = getMat(texture(iChannel0, posToUv(pos)).w);
        p += getNormal(pos) * depth;
        prePenetration = ((depth*10.0)*(depth*10.0)+5.0)/(length(mat.xyz)+1.0);
        addative = mat.xyz+vec3(0.03);
    }
    float[] dists = float[](0.0,0.0,0.0,0.0);
    vec3[] hitColors = vec3[](vec3(0.0,0.0,0.0),vec3(0.0,0.0,0.0),vec3(0.0,0.0,0.0),vec3(0.0,0.0,0.0));
    int hits = 0;
    for(int i = 0; i < 200; i++)
    {
        vec4 sdfSample = texture(iChannel0, posToUv(p));
        p += normalize(d) * sdfSample.z;
        dists[hits] += sdfSample.z;

        if (sdfSample.z < 0.001)
        {
            vec4 mat = getMat(sdfSample.w);
            hitColors[hits] = mat.xyz;
            vec2 rd = randDir(p);
            vec2 nrm = getNormal(p);
            vec2 n = mix(reflect(d, nrm), rd*sign(dot(nrm, rd)), mat.w);
            d = n;
            p += d * 0.002 + n * 0.002;
            sdfSample = texture(iChannel0, posToUv(p));
            hits++;
            if (hits == 4)
            {
                break;
            }
        }
    }
    vec3 light = vec3(0);
    for(int i = hits; i >= 0; i--)
    {
        light += hitColors[i] / (pow(dists[i]*0.1+2.0, 2.0)*0.1);
    }
    if (texture(iChannel0, posToUv(pos)).z > 0.03)
    light *= 0.6;
    if (texture(iChannel0, posToUv(pos)).z < 0.03)
    light *= 2.0;
    return light/prePenetration + addative;
}
vec3 getSample(vec2 pos)
{
    return march(pos, randDir(pos))*0.5+march(pos, randDir(pos+1.0))*0.5;
}

layout(location = 0) out vec4 fragColor;

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord.xy/iResolution.xy;
    vec4 prev = texture(iChannel1, uv);
    // Time varying pixel color
    vec4 sdfSample = texture(iChannel0, uv);
    vec2 sdfPos = sdfSample.xy;
    float sdfDst = sdfSample.z;
    fragColor = prev + vec4(getSample(sdfPos + 0.02 * vec2(rand(uv)*2.0-1.0, rand(uv+1.0)*2.0-1.0)), 1.0);
}