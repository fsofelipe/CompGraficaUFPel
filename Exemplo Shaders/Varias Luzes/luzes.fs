#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace_1;
in vec3 LightDirection_cameraspace_2;
in vec3 LightDirection_cameraspace_3;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D gooseTexture;
uniform mat4 MV;
uniform vec3 LightPosition_worldspace_1;
uniform vec3 LightPosition_worldspace_2;
uniform vec3 LightPosition_worldspace_3;
uniform vec3 LightColor_1;
uniform vec3 LightColor_2;
uniform vec3 LightColor_3;

void main(){

	// Light emission properties
	// You probably want to put them as uniforms
	float LightPower = 30.0f;

	// Material properties
	vec3 MaterialDiffuseColor = texture2D( gooseTexture, UV ).rgb;
	vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	// Distance to the light
	float distance_1 = length( LightPosition_worldspace_1 - Position_worldspace );
	float distance_2 = length( LightPosition_worldspace_2 - Position_worldspace );
	float distance_3 = length( LightPosition_worldspace_3 - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );

	// Direction of the light (from the fragment to the light)
	vec3 l_1 = normalize( LightDirection_cameraspace_1 );
	vec3 l_2 = normalize( LightDirection_cameraspace_2 );
	vec3 l_3 = normalize( LightDirection_cameraspace_3 );

	// Cosine of the angle between the normal and the light direction,
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta_1 = clamp( dot( n,l_1 ), 1,1 );
  float cosTheta_2 = clamp( dot( n,l_2 ), 1,1 );
  float cosTheta_3 = clamp( dot( n,l_3 ), 1,1 );

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);

	// Direction in which the triangle reflects the light
	vec3 R_1 = reflect(-l_1,n);
	vec3 R_2 = reflect(-l_2,n);
	vec3 R_3 = reflect(-l_3,n);

	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha_1 = clamp( dot( E,R_1 ), 0,1 );
  float cosAlpha_2 = clamp( dot( E,R_2 ), 0,1 );
  float cosAlpha_3 = clamp( dot( E,R_3 ), 0,1 );

	color =
    // Ambient : simulates indirect lighting
    (MaterialAmbientColor +
    // Diffuse : "color" of the object
    MaterialDiffuseColor * LightColor_1 * LightPower * cosTheta_1 / (distance_1*distance_1) +
    // Specular : reflective highlight, like a mirror
    MaterialSpecularColor * LightColor_1 * LightPower * pow(cosAlpha_1,5) / (distance_1*distance_1) )
    +
    // Ambient : simulates indirect lighting
    (MaterialAmbientColor +
    // Diffuse : "color" of the object
    MaterialDiffuseColor * LightColor_2 * LightPower * cosTheta_2 / (distance_2*distance_2) +
    // Specular : reflective highlight, like a mirror
    MaterialSpecularColor * LightColor_2 * LightPower * pow(cosAlpha_2,5) / (distance_2*distance_2))
    +
    // Ambient : simulates indirect lighting
    MaterialAmbientColor +
    // Diffuse : "color" of the object
    MaterialDiffuseColor * LightColor_3 * LightPower * cosTheta_3 / (distance_3*distance_3) +
    // Specular : reflective highlight, like a mirror
    MaterialSpecularColor * LightColor_3 * LightPower * pow(cosAlpha_3,5) / (distance_3*distance_3);

}
