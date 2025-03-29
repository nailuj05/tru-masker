# version 330

out vec4 FragColor;

void main() {
	vec2 uv = gl_FragCoord.xy / 1000;

	float checker = mod(floor(uv.x * 25) + floor(uv.y * 25), 2);
	FragColor = vec4(mix(0.75, 1.0, checker));
}
