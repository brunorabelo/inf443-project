#version 330 core  // OpenGL 3.3 shader

// -- Phong shader for reflectable objects --
// Objects will be rendered once above the z=0 plane,
// and once reflected below the plane. Any part of an
// object below the plane ceases to be rendered as it
// is underwater.

// Code based on the "mesh" shader from the cgp library.

// Inputs coming from tehe vertex shader
in struct fragment_data
{ 
    vec3 position; // position in the world space
    vec3 normal;   // normal in the world space
    vec3 color;    // current color on the fragment
    vec2 uv;       // current uv-texture on the fragment
} fragment;

// Output of the fragment shader - output color
layout(location=0) out vec4 FragColor;

// Texture image
uniform sampler2D image_texture;

// Uniform values - must be send from the C++ code
uniform vec3 light; // position of the light

// Shape color
uniform vec3 color;   // Uniform color of the object
uniform float alpha;  // alpha coefficient

// Phong shading
uniform float Ka;     // Ambient coefficient
uniform float Kd;     // Diffuse coefficient
uniform float Ks;     // Specular coefficient
uniform float specular_exp; // Specular exponent

uniform mat4 view;       // View matrix (rigid transform) of the camera - to compute the camera position

uniform bool reflection;          // Whether we're drawing a reflection or the real object
uniform bool compute_lighting = true; // Whether to do expensive lighting computations

void main()
{
	// Compute the position of the center of the camera
	mat3 O = transpose(mat3(view));                   // get the orientation matrix
	vec3 last_col = vec3(view*vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
	vec3 camera_position = -O*last_col;

//	// If camera above water, clip object and its reflection to water level
//	if(camera_position.z > 0) { 
//		if(reflection && fragment.position.z > 0)
//			discard;
// 		if(!reflection && fragment.position.z < 0)
// 			discard;
//	} else if(reflection) // Below water, don't show reflections
//		discard;
	if(reflection && fragment.position.z > 0)
		discard;
 	if(!reflection && fragment.position.z < 0)
 		discard;

		
	
	// Texture
	// ************************* //
	
	// Current uv coordinates
	// by default inverse the v direction (avoids common image upside-down)
	vec2 uv_image = vec2(fragment.uv.x, 1.0-fragment.uv.y);

	// Get the current texture color
	vec4 color_image_texture = texture(image_texture, uv_image);

	// Compute the base color of the object based on: vertex color, uniform color, and texture
	vec3 color_object  = fragment.color * color * color_image_texture.rgb;

	

	if(compute_lighting) {

		// Re-normalize the normals (interpolated on the triangle)
		vec3 N = normalize(fragment.normal);

		// Inverse the normal if it is viewed from its back
		//  (note: doesn't work on Mac)
		if (gl_FrontFacing == false) {  
			N = -N;
		}

		// Phong coefficient (diffuse, specular)
		// ************************* //

		// Unit direction toward the light
		vec3 L = normalize(light-fragment.position);

		// Diffuse coefficient
		float diffuse = max(dot(N,L),0.0);

		// Specular coefficient
		float specular = 0.0;
		if(diffuse>0.0){
			vec3 R = reflect(-L,N); // symetric of light-direction with respect to the normal
			vec3 V = normalize(camera_position-fragment.position);
			specular = pow( max(dot(R,V),0.0), specular_exp );
		}

		// Compute Shading
		// ************************* //

		// Compute the final shaded color using Phong model
		vec3 color_shading = (Ka + Kd * diffuse) * color_object + Ks * specular * vec3(1.0, 1.0, 1.0);
	
		// Output color, with the alpha component
		FragColor = vec4(color_shading, alpha * color_image_texture.a);
	}
	else
		FragColor = vec4(color_object, alpha * color_image_texture.a);


}