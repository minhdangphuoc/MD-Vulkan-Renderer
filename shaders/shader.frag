#version 450

////////////////// VERSION 0.1.0 ////////////////
/*
layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}
*/
////////////////// VERSION 0.1.0 ////////////////

layout( location = 0 ) in vec3 fragColor;
layout( location = 0 ) out vec4 outColor;

void main()
{
    outColor = vec4( fragColor, 1.0 );
}