#version 330 core

// Interpolated values from the vertex shaders
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection1_cameraspace;
in vec3 LightDirection2_cameraspace;
in vec3 LightDirection3_cameraspace;
in vec3 LightDirection4_cameraspace;

// Output data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform vec3 LightPosition1_worldspace;
uniform vec3 LightPosition2_worldspace;
uniform vec3 LightPosition3_worldspace;
uniform vec3 LightPosition4_worldspace;
uniform vec3 MaterialDiffuseColor;
uniform vec3 LightColor;
uniform float Light1Power;
uniform float Light2Power;
uniform float Light3Power;
uniform float Light4Power;
uniform float AmbientBrightness;
uniform float SpecularBrightness;
void main(){
	
	// Material properties
	vec3 MaterialAmbientColor = vec3(AmbientBrightness,AmbientBrightness,AmbientBrightness) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(SpecularBrightness,SpecularBrightness,SpecularBrightness);

	// Distance to the light
	float distance1 = length( LightPosition1_worldspace - Position_worldspace );
	float distance2 = length( LightPosition2_worldspace - Position_worldspace );
	float distance3 = length( LightPosition3_worldspace - Position_worldspace );
	float distance4 = length( LightPosition4_worldspace - Position_worldspace );
	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l1 = normalize( LightDirection1_cameraspace );
	vec3 l2 = normalize( LightDirection2_cameraspace );
	vec3 l3 = normalize( LightDirection3_cameraspace );
	vec3 l4 = normalize( LightDirection4_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta1 = clamp( dot( n,l1 ), 0,1 );
	float cosTheta2 = clamp( dot( n,l2 ), 0,1 );
	float cosTheta3 = clamp( dot( n,l3 ), 0,1 );
	float cosTheta4 = clamp( dot( n,l4 ), 0,1 );

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R1 = reflect(-l1,n);
	vec3 R2 = reflect(-l2,n);
	vec3 R3 = reflect(-l3,n);
	vec3 R4 = reflect(-l4,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha1 = clamp( dot( E,R1 ), 0,1 );
	float cosAlpha2 = clamp( dot( E,R2 ), 0,1 );
	float cosAlpha3 = clamp( dot( E,R3 ), 0,1 );
	float cosAlpha4 = clamp( dot( E,R4 ), 0,1 );
	color = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * Light1Power * cosTheta1 / (distance1*distance1) +
		MaterialDiffuseColor * LightColor * Light2Power * cosTheta2 / (distance2*distance2) +
		MaterialDiffuseColor * LightColor * Light3Power * cosTheta3 / (distance3*distance3) +
		MaterialDiffuseColor * LightColor * Light4Power * cosTheta4 / (distance4*distance4) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * Light1Power * pow(cosAlpha1,5) / (distance1*distance1);
		MaterialSpecularColor * LightColor * Light2Power * pow(cosAlpha2,5) / (distance2*distance2);
		MaterialSpecularColor * LightColor * Light3Power * pow(cosAlpha3,5) / (distance3*distance3);
		MaterialSpecularColor * LightColor * Light4Power * pow(cosAlpha4,5) / (distance4*distance4);
}
