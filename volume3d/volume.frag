#version 330 core

in vec2 vTexCoord;
in vec3 vRayDirection;

out vec4 fragColor;

uniform sampler3D volumeTexture;
uniform vec3 dimensions;
uniform float stepSize;
uniform float brightness;
uniform float contrast;
uniform float opacity;

// Transfer function (simple grayscale)
vec4 transferFunction(float intensity) {
    // Apply contrast and brightness
    intensity = (intensity - 0.5) * contrast + 0.5;
    intensity *= brightness;
    intensity = clamp(intensity, 0.0, 1.0);
    
    // Return RGBA (grayscale with opacity)
    return vec4(intensity, intensity, intensity, opacity * intensity);
}

// Ray-box intersection
bool intersectBox(vec3 rayOrigin, vec3 rayDir, out float tNear, out float tFar) {
    vec3 invDir = 1.0 / rayDir;
    vec3 tBottom = (-0.5 - rayOrigin) * invDir;
    vec3 tTop = (0.5 - rayOrigin) * invDir;
    
    vec3 tMin = min(tBottom, tTop);
    vec3 tMax = max(tBottom, tTop);
    
    tNear = max(max(tMin.x, tMin.y), tMin.z);
    tFar = min(min(tMax.x, tMax.y), tMax.z);
    
    return tFar > tNear;
}

void main() {
    // Initialize ray
    vec3 rayOrigin = vec3(vTexCoord - 0.5, -0.5);
    vec3 rayDirection = normalize(vRayDirection);
    
    // Intersect with volume bounding box
    float tNear, tFar;
    if (!intersectBox(rayOrigin, rayDirection, tNear, tFar)) {
        discard;
    }
    
    // Clamp to volume
    tNear = max(tNear, 0.0);
    
    // Ray march through volume
    vec3 rayStart = rayOrigin + rayDirection * tNear;
    vec3 rayEnd = rayOrigin + rayDirection * tFar;
    float rayLength = length(rayEnd - rayStart);
    int numSteps = int(rayLength / stepSize);
    
    vec4 accumulatedColor = vec4(0.0);
    float accumulatedAlpha = 0.0;
    
    for (int i = 0; i < numSteps; i++) {
        if (accumulatedAlpha > 0.99) break;
        
        float t = float(i) / float(numSteps);
        vec3 samplePos = mix(rayStart, rayEnd, t);
        
        // Convert to texture coordinates
        vec3 texCoord = samplePos + 0.5;
        
        // Sample volume
        float intensity = texture(volumeTexture, texCoord).r;
        
        // Apply transfer function
        vec4 sampleColor = transferFunction(intensity);
        
        // Front-to-back composition
        accumulatedColor.rgb += (1.0 - accumulatedAlpha) * sampleColor.a * sampleColor.rgb;
        accumulatedAlpha += (1.0 - accumulatedAlpha) * sampleColor.a;
        
        // Early ray termination
        if (accumulatedAlpha > 0.99) {
            accumulatedAlpha = 1.0;
            break;
        }
    }
    
    if (accumulatedAlpha < 0.01) discard;
    
    fragColor = vec4(accumulatedColor.rgb, accumulatedAlpha);
}