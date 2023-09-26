#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3[10] light_directions;
uniform vec3[10] light_positions;
uniform vec3 eye_position;
uniform float cut_off_angle;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform int spotlight_enable;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;


float point_light_intensity(vec3 light_position, vec3 light_direction)
{
	vec3 N = normalize(world_normal);
    vec3 L = normalize( light_position - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 R = reflect (-L, N);
    vec3 H = normalize (L+V);

    // (student): Define ambient, diffuse and specular light components
    float diffuse_light = material_kd * max (dot(N,L), 0);
    float specular_light = 0;
    float emisivity = 0;
    float spot_light = dot(-L, light_direction);
    float light_att_factor=0;
    float cut_off = radians(cut_off_angle);
    float spot_light_limit = cos(cut_off);

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);

    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    if(spotlight_enable!=0)
    {
        if (spot_light > spot_light_limit)
        {
            // fragmentul este iluminat de spot, deci se calculeaza valoarea luminii conform  modelului Phong
            // se calculeaza atenuarea luminii
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            light_att_factor = pow(linear_att, 2);
            // light_att_factor=1;
        }
    }
    else
        light_att_factor=1;

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.
    // float atenuation_factor=1/(1+pow(distance(world_position, light_position), 2));

    float light_intensity=(1/pow(distance(world_position, light_position), 2));
	light_intensity*=light_att_factor*(diffuse_light+specular_light);
	// emisivity + ambient_light + light_att_factor * ( diffuse_light + specular_light );

	return light_intensity;
}

void main()
{
    // vec3 world_position = (Model * vec4(world_position,1)).xyz;
	float ambient_light = 0.25;

	float light_intensity1= point_light_intensity(light_positions[0], light_directions[0]);
	float light_intensity2= point_light_intensity(light_positions[1], light_directions[1]);

    // TODO(student): Write pixel out color
    out_color = vec4(object_color * (light_intensity1+light_intensity2+ambient_light), 1);

}
