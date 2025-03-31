# version 330

uniform float size;
uniform float sharp;
uniform vec2 tres;

in vec2 fragTexCoord;
out vec4 finalColor;
uniform sampler2D texture0;


void main() {
	vec4 texColor = texture(texture0, fragTexCoord);
	vec2 uv = gl_FragCoord.xy / tres;
	float aspect = tres.x / tres.y;
	uv.x *= aspect;

	vec2 center = vec2(0.5 * aspect, 0.5);
	float mask = 1 - sharp/size * (distance(center, uv) - size);

	finalColor = vec4(texColor.xyz, clamp(mask,0,1) * texColor.w);
}
