#version 330 core

out vec4 outColor;

in vec2 vertexTex;

uniform vec4 u_DynamicColor = vec4(1);
uniform sampler2D u_Texture;

const float kernelOffset = 1.0 / 300.0;  

void main()
{
    //TODO: a way to enable them with imgui
	vec4 preprocessColor = u_DynamicColor * texture(u_Texture, vertexTex);
	// Various effects (from learnopengl.com)
	vec4 inverseColor = vec4(1) - preprocessColor;
	vec4 basicGrayscale = vec4((preprocessColor.r + preprocessColor.g + preprocessColor.b) / 3.0);
	vec4 weightedGrayscale = vec4(0.2126 * preprocessColor.r + 0.7152 * preprocessColor.g + 0.0722 * preprocessColor.b);
	vec4 perhapsNightvision = vec4(0.1*preprocessColor.r, 0.8*preprocessColor.g, 0.1*preprocessColor.b, preprocessColor.a)*3; // myself
	// using kernels (from learnopengl.com too)
	vec2 offsets[9] = vec2[](
        vec2(-kernelOffset,  kernelOffset), // top-left
        vec2( 0.0f,    kernelOffset), // top-center
        vec2( kernelOffset,  kernelOffset), // top-right
        vec2(-kernelOffset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( kernelOffset,  0.0f),   // center-right
        vec2(-kernelOffset, -kernelOffset), // bottom-left
        vec2( 0.0f,   -kernelOffset), // bottom-center
        vec2( kernelOffset, -kernelOffset)  // bottom-right    
    );
    float sharpenKernel[9] = float[](
        -1, -1, -1,
        -1,  9, -1,
        -1, -1, -1
    );
    float blurKernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
    float blurKernelNotOne[9] = float[](
        1.0, 2.0, 1.0,
        2.0, 4.0, 2.0,
        1.0, 2.0, 1.0
    );
    float edgeDetectionKernel[9] = float[](
        1,  1, 1,
        1, -8, 1,
        1,  1, 1
    );
    float customKernelTest[9] = float[](
        0, 0, -1,
        0, 0, 1,
        0, 0, 1
    );
    vec3 sampleColor[9];
    for(int i = 0; i < 9; i++) sampleColor[i] = vec3(texture(u_Texture, vertexTex.st + offsets[i]));

    float kernel[9] = blurKernel;
    vec3 postKernelColor = vec3(0.0);
    for(int i = 0; i < 9; i++) postKernelColor += sampleColor[i] * kernel[i];

	vec4 postprocessColor = vec4(postKernelColor, 1);
	postprocessColor.a = preprocessColor.a;
	outColor = preprocessColor;
}