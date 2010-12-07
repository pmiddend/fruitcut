#version 140

$$$HEADER$$$

out vec4 frag_color;

/* Generated using this code:
#include <boost/math/distributions/normal.hpp>
#include <iostream>

int main()
{
  boost::math::normal_distribution<double> nd(
    0.0,
    0.84089642);

  for (double i = -2.0; i <= 2.0; i += 1.0)
    std::cout << boost::math::pdf(nd,i) << ",";
}
*/

float distributions[5] = float[](0.0280412,0.233924,0.474425,0.233924,0.0280412);

void
main()
{
	vec3 t = 
		vec3(
			texture2D(
				texture,
				gl_FragCoord.xy/vec2(256.0,256.0)));

	vec3 sum = vec3(0.0,0.0,0.0);
	float coord = gl_FragCoord.x - 2.0;
	for (int i = 0; i < 5; ++i)
	{
		sum += 
			distributions[i] * vec3(
				texture2D(
					texture,
					vec2(
						coord,
						gl_FragCoord.y)/vec2(256.0,256.0)));
		coord += 1.0;
	}

	frag_color = 
		vec4(
			sum,
			1.0);
}
