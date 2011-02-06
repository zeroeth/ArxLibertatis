/*
===========================================================================
ARX FATALIS GPL Source Code
Copyright (C) 1999-2010 Arkane Studios SA, a ZeniMax Media company.

This file is part of the Arx Fatalis GPL Source Code ('Arx Fatalis Source Code'). 

Arx Fatalis Source Code is free software: you can redistribute it and/or modify it under the terms of the GNU General Public 
License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Arx Fatalis Source Code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Arx Fatalis Source Code.  If not, see 
<http://www.gnu.org/licenses/>.

In addition, the Arx Fatalis Source Code is also subject to certain additional terms. You should have received a copy of these 
additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Arx 
Fatalis Source Code. If not, please request a copy in writing from Arkane Studios at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing Arkane Studios, c/o 
ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/
#ifndef __ATHENA_MIXER_H__
#define __ATHENA_MIXER_H__

#include "AudioTypes.h"
#include "AudioResource.h"

namespace ATHENA
{

	///////////////////////////////////////////////////////////////////////////////
	//                                                                           //
	// Class ATHENA::Mixer                                                       //
	//                                                                           //
	///////////////////////////////////////////////////////////////////////////////
	class Mixer
	{
		public:

			//Constructor and destructor
			Mixer();
			~Mixer();

			//Setup
			aalError SetName(const char * name);
			aalError SetVolume(const aalFloat & volume);
			aalError SetParent(const Mixer * mixer);

			//Status
			aalError GetVolume(aalFloat & volume) const;
			aalError GetParent(Mixer *&mixer) const;
			aalUBool IsPaused() const;

			//Control
			aalError Stop();
			aalError Pause();
			aalError Resume();

			//Data
			char * name;
			aalULong status;
			aalULong flags;
			aalFloat volume;
			aalFloat pitch;
			aalFloat pan;
			const Mixer * parent;
	};

}//ATHENA::

#endif//__ATHENA_MIXER_H__