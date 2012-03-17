#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Framework.h"
#include "Shader.h"

#include <vector>
#include <algorithm>

using namespace std;

const float		spreadStep = 0.035f,		// step size to increment spread
				velStep = 0.2f,				// step size to increment velocity
				forceStep = 0.2f,			// step size to increment gravity
				rateStep = 0.1f;			// step size to increment rate

struct Particle {
	float		age,			// current age in seconds
				size;			// particle size
	aiVector3D	pos,			// current position
				vel;			// current velocity

	Particle(aiVector3D _pos, aiVector3D _vel, float _size):
		pos(_pos),
		vel(_vel),
		size(_size),
		age(0.0f)
	{}
};

class ParticleEmitter {
public:
	enum particleType { PARTICLE_FIRE, PARTICLE_SMOKE, PARTICLE_SPARK };
	
	vector<Particle>	mParticles;		// array of particles
	particleType		mType;			// type of particle
	bool				mIsActive;		// toggles functionality
	aiVector3D			mPosition,		// location of the emitter
						mInitDir,		// emission direction
						mGlobalForce;	// forces acting on our particle
	GLsizei				mWindowWidth;	// width of frame window
	float				mRate,			// rate of particle emission, every N frames
						mSpread,		// spread angle of emitter cone
						mInitVelocity,	// initial velocity of new particles
						mVelocityRand,	// random scale for initial velocity
						mLifespan,		// total lifetime in seconds
						mInitSize,		// intial size of particle, before randomizing
						mSizeRand,		// random scale for size
						mAccumulator;	// accumulates time
	Shader				*mShader;		// particle shader
	sf::Image			mTexture;		// texture map to apply
	
	ParticleEmitter(particleType _type, string _filename, GLsizei _width);
	void spawnParticles(float _dTime);
	void renderParticles();
	void updateParticles(float _dTime);

	inline void updatePose(aiVector3D _pos, aiVector3D _dir) {
		mPosition = _pos;
		mInitDir = _dir;
	}
	inline void incrementGravity() { mGlobalForce.y = max(mGlobalForce.y - forceStep, -17.6f); }
	inline void decrementGravity() { mGlobalForce.y = min(mGlobalForce.y + forceStep, 0.0f); }
	inline void incrementRate() { mRate = min(mRate + rateStep, 5.0f); }
	inline void decrementRate() { mRate = max(mRate - rateStep, 0.0f); }
	inline void incrementSpread() { mSpread = min(mSpread + spreadStep, 0.7854f); }
	inline void decrementSpread() { mSpread = max(mSpread - spreadStep, 0.0f); }
	inline void incrementVelocity() { mInitVelocity = min(mInitVelocity + velStep, 10.0f); }
	inline void decrementVelocity() { mInitVelocity = max(mInitVelocity - velStep, 0.2f); }
};

#endif