#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

// --- 光源数组声明 ---
#define NR_LIGHTS 4 // 定义光源数量，可以根据需要调整
uniform vec3 lightPositions[NR_LIGHTS];
uniform vec3 lightColors[NR_LIGHTS];
// --------------------

uniform vec3 viewPos;
uniform vec3 albedo;

// --- PBR 材质参数 ---
uniform float metallic;
uniform float roughness;
uniform float ao;

const float PI = 3.14159265359;

// --- D, G, F 函数 (保持不变) ---
float DistributionGGX(float NdotH, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float numerator = a2;
    float denominator = (NdotH * NdotH * (a2 - 1.0) + 1.0);
    denominator = PI * denominator * denominator;
    return numerator / denominator;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(float NdotV, float NdotL, float roughness) {
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    // clamp(1.0 - cosTheta, 0.0, 1.0) 是为了确保 pow 的输入在 [0, 1] 范围内
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {
    // --- 准备基础向量 ---
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    // --- PBR 基础反射率 F0 ---
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // --- 总出射光初始化 ---
    vec3 totalLo = vec3(0.0);

    // --- 遍历所有光源并累加光照贡献 ---
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        // --- 为当前光源计算向量和点积 ---
        vec3 L = normalize(lightPositions[i] - FragPos);
        vec3 H = normalize(V + L);

        float NdotV = max(dot(N, V), 0.001); // NdotV 已经在循环外计算，但为保持代码结构，这里也放着
        float NdotL = max(dot(N, L), 0.0);

        // 如果背面朝向光源，则无贡献
        if (NdotL <= 0.0) continue;

        float NdotH = max(dot(N, H), 0.0);
        float HdotV = max(dot(H, V), 0.0);

        // --- 计算辐射度 (Radiance) ---
        // 辐射度 = 光源颜色 * 强度衰减（这里省略距离衰减）
        vec3 radiance = lightColors[i];

        // --- Cook-Torrance BRDF ---
        float D = DistributionGGX(NdotH, roughness);
        float G = GeometrySmith(NdotV, NdotL, roughness);
        vec3 F = FresnelSchlick(HdotV, F0);

        // 镜面反射 BRDF: D * G * F / (4 * NdotV * NdotL)
        vec3 numerator = D * G * F;
        float denominator = 4.0 * NdotV * NdotL;
        vec3 specular = numerator / max(denominator, 0.001);

        // 能量守恒的漫反射比例
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= (1.0 - metallic); // 金属没有漫反射

        // Lambertian 漫反射项
        vec3 diffuse = kD * albedo / PI;

        // 完整 BRDF Lo = (漫反射项 + 镜面反射项) * radiance * NdotL
        vec3 Lo = (diffuse + specular) * radiance * NdotL;

        // 累加所有光源的贡献
        totalLo += Lo;
    }
//    totalLo /= 4;

    // --- 环境光 (Ambient Light) ---
    // 为配合目标效果，环境光值可以适当提高，比如从 0.03 提高到 0.15 左右
    vec3 ambientStrength = vec3(0.15);
    vec3 ambient = ambientStrength * albedo * ao;

    vec3 color = ambient + totalLo;

    // --- HDR 色调映射和 Gamma 校正 (保持不变) ---
//    color = color / (color + vec3(1.0));
    float gamma = 1.00;
    color = pow(color, vec3(1.0/gamma));

    FragColor = vec4(color, 1.0);
}