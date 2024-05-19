in vec2 v_coord;
layout(location = 0) out vec4 f_color;

uniform sampler2D u_texture0;
uniform float u_timer;

float random(vec2 st){
    return fract(sin(dot(st.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    float totalWeight = 0.0;

    vec2 uv = v_coord;

    vec3 color;

    vec4 textureColor = texture(u_texture0, v_coord);

    vec2 centerCoord = vec2(0.5, 0.5);

    vec4 centerColor = texture(u_texture0, centerCoord);

    float centerBrightness = dot(centerColor.rgb, vec3(0.2126, 0.7152, 0.0722));

    float glitchX = random(vec2(u_timer, 1.0)) * 0.1 - 0.05;

    vec2 redUV = uv + vec2(0.005, 0.0);
    vec4 redColor = texture(u_texture0, redUV);

    vec2 greenUV = uv - vec2(0.0, 0.0);
    vec4 greenColor = texture(u_texture0, greenUV);

    vec2 blueUV = uv - vec2(-0.005, 0.0);
    vec4 blueColor = texture(u_texture0, blueUV);

    f_color = vec4(redColor.r, greenColor.g, blueColor.b, 1.0);
}