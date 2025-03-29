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
	float dist = max(0.0, max(0.5 - uv.x, max(uv.x - 0.5, max(0.5 - uv.y, uv.y - 0.5))));
	float mask = clamp(1 - sharp/size * (dist - size), 0, 1);
	
	finalColor = vec4(texColor.xyz, texColor.w * mask);
}
