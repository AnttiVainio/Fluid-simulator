/** fluid.cpp **/

#include <GL/gl.h>
#include <cmath>
#include "fluid.hpp"
#include "settings.hpp"

#define clamp(x,y,z) ((x) < (y) ? (y) : ((x) > (z) ? (z) : (x)))

/*
	The fluid particles only have this one act function
	That means the particles are moved to a new position as soon as it has been calculated
	This is why the simulation is not accurate but an approximation
*/
void fluid_c::act(const std::list<fluid_c> &fluid) {
	//Initial movement damping and gravity
	speedx*= damp;
	speedy+= gravity;
	speedy*= damp;
	//Offset is applied directly to the particle and the offset doesn't affect the speed of the particle
	float x_off = 0.0f, y_off = 0.0f;
	//This is the sum of inversed distances; it is only used for extra_damp later
	float dist_sum = 0.0f;
	//Iterate through all the fluid particles
	for(std::list<fluid_c>::const_iterator i = fluid.begin(); i != fluid.end(); i++) {
		const float dx = x - i->x;
		const float dy = y - i->y;
		float dist = dx * dx + dy * dy; //No need to calculate the REAL distance
		//Only if the other fluid particle is further away than one pixel it actually affects this particle
		if(dist > 1.0f) {
			//Instead of the real delta y we use this one
			//This makes the fluid particles to slightly float upwards
			const float dy1 = dy - 1.0f;
			dist*= dist;
			dist_sum+= 1.0f / dist;
			//Instead of the speed affecting the fluid's speed directly
			//  a higher speed will make the distance between particles smaller
			//  and that will make the particles move faster
			dist/= speed;
			const float mult = dist * 0.01f; //mult is inversed multiplier
			//Calculate direct offset here
			const float offset_x = dx / mult;
			x_off+= clamp(offset_x, -0.4f, 0.4f);
			const float offset_y = dy1 / mult;
			y_off+= clamp(offset_y, -0.4f, 0.4f);
			dist*= 0.0002f; //dist is inversed multiplier
			//Calculate the speed change here
			const float speed_x = dx / dist;
			speedx+= clamp(speed_x, -0.2f, 0.2f);
			const float speed_y = dy1 / dist;
			speedy+= clamp(speed_y, -0.2f, 0.2f);
		}
	}
	//The speed is damped even more when there are a lot of fluid particles near
	//This is done to prevent the particles from gaining really high speeds in these situations
	float extra_damp = 1.0f + (damp - 1.0f) * dist_sum * 20000.0f;
	//Limit the damping so that the particles don't suddenly stop if there is another particle really close
	if(extra_damp < extra_damp_limit) extra_damp = extra_damp_limit;
	speedx*= extra_damp;
	speedy*= extra_damp;
	//Actually move the particle
	x+= x_off + speedx;
	y+= y_off + speedy;
	//Collisions with the screen edges
	if(x < 0.0f) {
		speedx = -speedx * 0.5f;
		x = -x;
	}
	else if(x > SCREEN_WIDTH) {
		speedx = -speedx * 0.5f;
		x = (float)SCREEN_WIDTH * 2.0f - x;
	}
	if(y > SCREEN_HEIGHT) {
		speedy = -speedy * 0.5f;
		y = (float)SCREEN_HEIGHT * 2.0f - y;
	}
}

//Moves the particles towards a coordinate
void fluid_c::drain(const float _x, const float _y) {
	const float dx = x - _x;
	const float dy = y - _y;
	float dist = dx * dx + dy * dy;
	if(dist > 1.0f) {
		dist*= dist * 0.000001f / speed;
		const float speed_x = dx / dist;
		speedx-= clamp(speed_x, -0.8f, 0.8f);
		const float speed_y = dy / dist;
		speedy-= clamp(speed_y, -0.8f, 0.8f);
	}
}

//Moves the particles away from a coordinate
void fluid_c::repel(const float _x, const float _y) {
	const float dx = x - _x;
	const float dy = y - _y;
	float dist = dx * dx + dy * dy;
	if(dist > 1.0f) {
		dist*= dist * 0.000001f / speed;
		const float speed_x = dx / dist;
		speedx+= clamp(speed_x, -0.8f, 0.8f);
		const float speed_y = dy / dist;
		speedy+= clamp(speed_y, -0.8f, 0.8f);
	}
}

#define DRAW_SIZE 160.0f

void fluid_c::draw() const {
	glColor4f(r, g, b, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(x - DRAW_SIZE, y - DRAW_SIZE);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(x + DRAW_SIZE, y - DRAW_SIZE);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(x - DRAW_SIZE, y + DRAW_SIZE);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(x + DRAW_SIZE, y + DRAW_SIZE);
	glEnd();
}
