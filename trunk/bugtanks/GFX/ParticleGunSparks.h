/**
* @file
* @author Benjamin Golba <benjii.g@gmail.com>
* @version 1.0
* Copyright (�) A-Team.
*
* @section LICENSE
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details at
* http://www.gnu.org/copyleft/gpl.html
* 
* @section DESCRIPTION
*
* Handles particles
*/

#include "Particle.h"

#ifndef PARTICLEGUNSPARKS_H
#define PARTICLEGUNSPARKS_H

namespace gfx
{

	class ParticleGunSparks : public Particle
	{
	public:
		/**
		 * Constructor
		 */
		ParticleGunSparks(ID3D10Device *pDevice, Effect *pEffect, char* name, int passIndex, int layoutIndex, int initParticles, int maxChildParticles, int offset);

		/**
		 * Destructor
		 */
		~ParticleGunSparks();

		/**
		 * Updates particle storage texture with information about emitters
		 * @param pParticleStorage Texture to save data in
		 */
		virtual void updateParticleStorage(ID3D10Resource* pParticleStorage);

		/**
		 * Renders particle
		 */
		virtual void render();

	private:

	};

};

#endif