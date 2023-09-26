#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_color;
in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture_coordinate;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float TimePassed;


// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(frag_color, 1);
    // out_color = vec4(frag_normal, 1);
    // out_color[0]*=sin(TimePassed);
    // out_color[2]*=sin(TimePassed);
    // out_color[0]*=sin(TimePassed);


}
