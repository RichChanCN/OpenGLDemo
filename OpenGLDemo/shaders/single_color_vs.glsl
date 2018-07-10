#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos.x*1.1,aPos.y*1.1,aPos.z*1.1, 1.0);
}