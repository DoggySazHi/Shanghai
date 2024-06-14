#version 320 es
precision highp float;

in vec2 fragCoord;

uniform vec2 iResolution;
uniform float iTime;
uniform vec4 iDate;
uniform int iFrame;

// ================
// World Data
// ================
#define TYPE_CIRCLE 0.0
#define TYPE_RECT 1.0
#define WORLD_COUNT 7
#define WORLD_LENGTH WORLD_COUNT*9

// object: { type, pos, (data), color }
// data: { radius, unused } or { halfWidth, halfHeight }
const float[] world = float[]
(
TYPE_CIRCLE,   -4.0, 4.0,   0.25, 0.0,   11.0, 1.0, 1.0,  0.0,
TYPE_CIRCLE,   0.0, 4.0,   0.25, 0.0,   1.0, 11.0, 1.0,  0.0,
TYPE_CIRCLE,   4.0, 4.0,   0.25, 0.0,   1.0, 1.0, 11.0,  0.0,
TYPE_RECT,   0.0, -5.0,   90.0, 2.0,   0.0,0.0,0.0,  1.0,
TYPE_CIRCLE,   -4.0, -1.0,   1.5, 0.0,   0.0, 0.0, 0.0,  1.0,
TYPE_CIRCLE,   0.0, -1.0,   1.5, 0.0,   0.0, 0.0, 0.0,  1.0,
TYPE_CIRCLE,   4.0, -1.0,   1.5, 0.0,   0.0, 0.0, 0.0,  1.0
);
vec4 getMat(float index)
{
    int i = int(floor(index+0.1));
    return vec4(world[i+5], world[i+6], world[i+7], world[i+8]);
}
vec2 getSceneDist(vec2 p)
{
    float d = 99999.0;
    float index = 0.0;
    for(int i = 0; i < WORLD_LENGTH; i += 9)
    {
        float d1 = d;
        if (world[i] == TYPE_CIRCLE)
        {
            vec2 rpos = vec2(world[i+1], world[i+2]);

            d1 = length(p - rpos)-world[i+3];
        }
        if (world[i] == TYPE_RECT)
        {
            vec2 rpos = vec2(world[i+1], world[i+2]);
            vec2 rel1 = (p-rpos);
            float mul = 1.0;
            if (abs(rel1.x) >= world[i+3] || abs(rel1.y) >= world[i+4])
            {
                rel1.x = min(abs(rel1.x), world[i+3]) * sign(rel1.x);
                rel1.y = min(abs(rel1.y), world[i+4]) * sign(rel1.y);
            }
            else
            {
                mul = -1.0;
                vec2 dir = vec2(abs(rel1.x), abs(rel1.y));
                float a = float(world[i+3] > world[i+4]);
                float diff = max(world[i+3], world[i+4])-min(world[i+3], world[i+4]);
                dir.x -= diff * a;
                dir.y -= diff * (1.0-a);
                if (dir.x > dir.y)
                {
                    rel1.x = world[i+3] * sign(rel1.x);
                }
                else
                {
                    rel1.y = world[i+4] * sign(rel1.y);
                }
            }
            d1 = mul * length(p - (rel1+rpos));
        }
        if (d1 < d)
        {
            d = d1;
            index = float(i);
        }
    }
    return vec2(d, index);
}
vec2 getDist(vec2 p)
{
    vec2 pos = p;
    return getSceneDist(pos);
}
