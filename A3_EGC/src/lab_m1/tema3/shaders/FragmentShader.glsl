#version 330

// Input
in vec2 texcoord;
in vec3 world_position;
in vec3 world_normal;

// Uniform properties
uniform sampler2D texture_1;
// (student): Declare various other uniforms
uniform vec2 texture_offset;
uniform vec3 eye_position;

uniform float spot_light_limit;

uniform float material_kd;
uniform float material_ks;
uniform float material_shininess;

uniform int spotlight_count;
uniform vec3[100] spotlight_positions;
uniform vec3 spotlight_direction;

uniform int lightpoint_count;
uniform vec3[100] lightpoint_positions;
uniform vec3[100] lightpoint_color;


// Output
layout(location = 0) out vec4 out_color;

float global_light_factor()
{
    vec3 world_pos = world_position;
    vec3 N = world_normal;

	vec3 L = normalize(vec3(1, 2, 0));
    vec3 V = normalize(eye_position - world_pos );
    vec3 R = reflect (-L, N);

	float ambient_light = 0.05;



	float diffuse_light = material_kd * max (dot(N, L), 0);

	float specular_light=0;

	if (diffuse_light > 0)
        specular_light = material_ks * diffuse_light * pow(max(dot(V, R), 0), material_shininess);

	return ambient_light+diffuse_light+specular_light;
}


float lightpoint_factor(vec3 light_position)
{
	vec3 N = world_normal;
    vec3 L = normalize( light_position - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 R = reflect (-L, N);
    vec3 H = normalize (L+V);

    // (student): Define ambient, diffuse and specular light components
    float diffuse_light = material_kd * max (dot(N,L), 0);
    float specular_light = 0;

    if (diffuse_light > 0)
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);

    float light_intensity=(1/pow(distance(world_position, light_position), 2));
	light_intensity*=(diffuse_light+specular_light);

	return light_intensity;
}

float spotlight_factor(vec3 light_position)
{
	vec3 light_direction=-normalize(vec3(-1, 2, 0));

	vec3 N = world_normal;
    vec3 L = normalize( light_position - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 R = reflect (-L, N);
    vec3 H = normalize (L+V);

    // (student): Define ambient, diffuse and specular light components
    float diffuse_light = material_kd * max (dot(N,L), 0);
    float specular_light = 0;
    float spot_light = dot(-L, light_direction);
    float light_att_factor=0;

    if (diffuse_light > 0)
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);

    if (spot_light > spot_light_limit)
        light_att_factor = pow(
			(spot_light - spot_light_limit) / (1.0f - spot_light_limit),
			2
		);

    float light_intensity=(1/pow(distance(world_position, light_position)/2, 2));
	light_intensity*=light_att_factor*(diffuse_light+specular_light);

	return light_intensity;
}



void main()
{
    // (student): Calculate the out_color using the texture2D() function.
    out_color = vec4(1);
    // out_color = texture2D(texture_1, texcoord);
    out_color=texture2D(texture_1, texcoord+texture_offset);


	float light_factor=global_light_factor();

	vec3 color_from_light=vec3(0);

	for(int i=0;i<spotlight_count;i++)
		light_factor+=spotlight_factor(spotlight_positions[i]);

	for(int i=0;i<lightpoint_count;i++)
		color_from_light+=lightpoint_factor(lightpoint_positions[i])*lightpoint_color[i];

	// light_factor+=spotlight_factor(vec3(0, 2, -5));
	// light_factor+=spotlight_factor(vec3(2, 1, 5), -vec3(1, 2, 0));

	out_color=out_color*light_factor;

    if(out_color[3]==0)
        discard;

    out_color*=(out_color[3]);
	out_color+=vec4(color_from_light, 0);
    // out_color[3]=1-out_color[3];

}
