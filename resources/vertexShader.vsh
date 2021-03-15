in vec4 vertex;

uniform mat4 matrix;

varying vec4 fragPos;

void main(void)
{
   gl_Position = matrix * vertex;

   fragPos = vertex;
}
