#ifndef SHADER_RAYTRACING_RAYGENSHADER_HLSL
#define SHADER_RAYTRACING_RAYGENSHADER_HLSL

#define HLSL
#include "Util/Global.hlsli"
#include "Util/Helper.hlsli"

[shader("raygeneration")] void RayGenShader() {
    //カメラからのレイを生成
    Ray ray = GenerateCameraRay(
        DispatchRaysIndex().xy, g_sceneCB.cameraPosition.xyz, g_sceneCB.projectionToWorld);

    //飛ばした先の色を取得
    float4 color = RayCast(ray, 0);
    g_renderTarget[DispatchRaysIndex().xy] = color;
}

#endif //! SHADER_RAYTRACING_RAYGENSHADER_HLSL
