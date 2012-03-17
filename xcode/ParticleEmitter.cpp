#include "ParticleEmitter.h"
#include "VectorMath.h"

#define GRAVITY		4.9f

ParticleEmitter::ParticleEmitter(particleType _type, string _filename, GLsizei _width) {
	mType = _type;
	mAccumulator = -1.0f;

	// Fire is created from fast, rising particles with short lifespans.
	if(mType == PARTICLE_FIRE) {
		mIsActive = true;
		mRate = 2.8;
		mLifespan = 0.4f;
		mInitVelocity = 3.0f;
		mVelocityRand = 0.5f;
		mInitDir.Set(0.0f, 1.0f, 0.0f);
		mInitSize = 0.5f;
		mSizeRand = 0.1f;

	// Smoke particles rise slower than fire but stick around for much longer.
	} else if(mType == PARTICLE_SMOKE) {
		mIsActive = true;
		mRate = 1;
		mLifespan = 6.0f;
		mInitVelocity = 0.4f;
		mVelocityRand = 0.4f;
		mInitDir.Set(0.0f, 1.0f, 0.0f);
		mInitSize = 0.7f;
		mSizeRand = 0.4f;
		mGlobalForce.Set(0.0f, 0.05f*GRAVITY, 0.0f);

	// Sparks have tweakable settings and can simulate pretty much anything!
	} else if(mType == PARTICLE_SPARK) {
		mIsActive = false;
		mRate = 1;
		mSpread = 0.1f;
		mLifespan = 6.0f;
		mInitVelocity = 5.0f;
		mVelocityRand = 0.0f;
		mInitDir.Set(0.0f, 1.0f, 0.0f);
		mInitSize = 0.5f;
		mSizeRand = 0.1f;
		mGlobalForce.Set(0.0f, -GRAVITY, 0.0f);
	}

	// Loads particle image and sets up shader
	mTexture.LoadFromFile(_filename);
	mTexture.Bind();
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	mShader = new Shader("shaders/particle");
	if(!mShader->loaded()) {
		std::cerr << "Particle shader failed to load" << std::endl;
		std::cerr << mShader->errors() << std::endl;
		exit(-1);
	}

	mWindowWidth = _width;
}

void ParticleEmitter::spawnParticles(float _dTime) {
	if (!mIsActive) return;

	// Create (rate) particles each frame (supports floating point values)
	for(mAccumulator += mRate * _dTime; mAccumulator > _dTime; mAccumulator -= _dTime) {
		// Randomize position +- 0.2 units
		aiVector3D pos(mPosition);
		const float OFFSET = 0.2f;
		if(mType == PARTICLE_FIRE || mType == PARTICLE_SMOKE)
			pos.Set(pos.x + getRand(-OFFSET, OFFSET), pos.y, pos.z + getRand(-OFFSET, OFFSET));
		else if(mType == PARTICLE_SPARK) {
			pos += 0.2f*mInitDir;
			pos.y += 0.2f;
		}

		// Randomize velocity
		float vel = mInitVelocity + getRand(-mVelocityRand, mVelocityRand);

		// Randomize spread within cone
		aiVector3D dir = mInitDir;
		if(mType == PARTICLE_SPARK) {
			float angle = getRand(0.0f, mSpread);
			aiVector3D p(getRand(-1.0f, 1.0f), getRand(-1.0f, 1.0f), getRand(-1.0f, 1.0f));
			float pDotDir = dot(p,mInitDir);
			p -= pDotDir*mInitDir;
			p = p.Normalize() * tan(angle);
			dir += p;
			dir.Normalize();
		}

		// Randomize size
		float size = mInitSize + getRand(-mSizeRand, mSizeRand);

		// Add a new particle to the vector
		mParticles.push_back(Particle(pos, vel * dir, size));
	}
}

void ParticleEmitter::updateParticles(float _dTime) {
	for(int i = 0; i < mParticles.size(); ++i) {
		Particle *p = &mParticles[i];
		p->age += _dTime;

		// kill off old particles
		if(p->age > mLifespan) {
			mParticles.erase(mParticles.begin() + i);
			continue;
		}

		// update velocity
		p->vel += mGlobalForce * _dTime;

		// update position
		p->pos += p->vel * _dTime;
	}
}

void ParticleEmitter::renderParticles() {
	GLuint shaderId = mShader->programID();
	glUseProgram(shaderId);

	// prevents from writing to the depth buffer
	glDepthMask(GL_FALSE);
	// allows points to be drawn as billboards
	glEnable(GL_POINT_SPRITE);
	// enables blending of fragment and color buffer values
	glEnable(GL_BLEND);
	// sets point size from within shader
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	// specifies how fragment and color buffer pixel colors interact
	// What glBlendFunc parameters do I want?
	// Check out this neat, interactive web visualizer:
	// http://www.andersriggelsen.dk/glblendfunc.php
	if(mType == PARTICLE_FIRE || mType == PARTICLE_SPARK)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	else if(mType == PARTICLE_SMOKE)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// sets initial color to WHITE to modulate with the texture
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	// pass in transparency and point size parameters
	GLint	alphaId = glGetAttribLocation(shaderId, "alphaIn"),
			sizeId = glGetAttribLocation(shaderId, "sizeIn");
	glEnableVertexAttribArray(alphaId);
	glEnableVertexAttribArray(sizeId);

	// pass in screen width to rescale the point sprite size
	GLint widthId = glGetUniformLocation(shaderId, "widthIn");
	glUniform1f(widthId, mWindowWidth);

	// Set texture
	GLint diffuseId = glGetUniformLocation(shaderId, "diffuse");
	glUniform1i(diffuseId, 0);
	glActiveTexture(GL_TEXTURE0);
	mTexture.Bind();

	// Draw point sprites
	glBegin(GL_POINTS);
	for(int i = 0; i < mParticles.size(); ++i) {
		glVertexAttrib1f(sizeId, mParticles[i].size);

		// Calculate alpha fade based on the particle's age
		// Allows for fade-in and fade-out effects
		float	normAge = mParticles[i].age / mLifespan,	// age, normalized to [0,1]
				alpha = 1.0f;
		if(mType == PARTICLE_FIRE || mType == PARTICLE_SPARK) {
			if(normAge < 0.1f)			alpha = normAge * 10;
			else if(normAge > 0.6f)		alpha = 2.5f - 2.5f*normAge;
		} else if(mType == PARTICLE_SMOKE) {
			if(normAge < 0.2f)			alpha = normAge * 5;
			else if(normAge > 0.4f)		alpha = 1.6667f - 1.6667f*normAge;
		}
		glVertexAttrib1f(alphaId, alpha);
		
		glVertex3f(	mParticles[i].pos.x,
					mParticles[i].pos.y, 
					mParticles[i].pos.z );
	}
	glEnd();

	// Clean up!  Anything that was set or enabled should be un-set
    glDisableVertexAttribArray(alphaId);
    glDisableVertexAttribArray(sizeId);
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glUseProgram(0);
}