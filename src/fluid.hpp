/** fluid.hpp **/

#ifndef FLUID_HPP
#define FLUID_HPP

#include <list>

/*
	This is a single fluid particle
	The fluid becomes fluid when there are hundreds of particles
	The simulation basically works so that the particles try to get away from each other
*/
class fluid_c {
	private:
		/*
			damp and gravity is applied to the speed (speedx and speedy) every frame
			The constant variable speed determines how fast the particle moves
			extra_damp is dynamic that becomes larger when fluid particles are near each other
			  extra_damp_limit is used to limit this so that it doesn't become too damping
		*/
		float x, y, speedx, speedy;
		const float damp, speed, gravity;
		const float extra_damp_limit;
		const float r, g, b;

	public:
		/*
			x, y, speedx and speedy are the initial position and speed
			Rest of the variables are constant attributes for the fluid
			Higher damp_mult value will make the fluid less stiff
			Higher speed_mult value will make the fluid take more space and become less dense
		*/
		fluid_c(const float _x, const float _y,
			const float _speedx, const float _speedy,
			const float damp_mult, const float speed_mult, const float _gravity,
			const float _r, const float _g, const float _b):
				x(_x), y(_y),
				speedx(_speedx), speedy(_speedy),
				damp(damp_mult), speed(speed_mult), gravity(_gravity),
				extra_damp_limit(damp * 0.4),
				r(_r), g(_g), b(_b) {}

		void act(const std::list<fluid_c> &fluid);
		void drain(const float _x, const float _y);
		void repel(const float _x, const float _y);
		void draw() const;
};

#endif
