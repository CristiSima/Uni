#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec4 car_pos;

// Output values to fragment shader
out vec3 frag_color;


void main()
{
    // TODO(student): Compute world space vertex position and normal,
    // and send them to the fragment shader
	vec4 world_pos = Model * vec4(v_position, 1);

	world_pos.y-=pow(distance(world_pos, car_pos), 2)* 0.002;

	frag_color=v_color;
    gl_Position = Projection * View  * world_pos;
}
