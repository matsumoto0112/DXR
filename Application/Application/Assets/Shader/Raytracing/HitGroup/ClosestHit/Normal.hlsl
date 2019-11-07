#ifndef SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_NORMAL_HLSL
#define SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_NORMAL_HLSL

#define HLSL
#include "../../Util/Global.hlsli"

[shader("closesthit")]
void Normal(inout RayPayload payload, in MyAttr attr) {
    payload.color = g_sceneCB.lightAmbient;
}

#endif //! SHADER_RAYTRACING_HITGROUP_CLOSESTHIT_NORMAL_HLSL