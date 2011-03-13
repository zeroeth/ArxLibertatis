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
//////////////////////////////////////////////////////////////////////////////////////
//   @@        @@@        @@@                @@                           @@@@@     //
//   @@@       @@@@@@     @@@     @@        @@@@                         @@@  @@@   //
//   @@@       @@@@@@@    @@@    @@@@       @@@@      @@                @@@@        //
//   @@@       @@  @@@@   @@@  @@@@@       @@@@@@     @@@               @@@         //
//  @@@@@      @@  @@@@   @@@ @@@@@        @@@@@@@    @@@            @  @@@         //
//  @@@@@      @@  @@@@  @@@@@@@@         @@@@ @@@    @@@@@         @@ @@@@@@@      //
//  @@ @@@     @@  @@@@  @@@@@@@          @@@  @@@    @@@@@@        @@ @@@@         //
// @@@ @@@    @@@ @@@@   @@@@@            @@@@@@@@@   @@@@@@@      @@@ @@@@         //
// @@@ @@@@   @@@@@@@    @@@@@@           @@@  @@@@   @@@ @@@      @@@ @@@@         //
// @@@@@@@@   @@@@@      @@@@@@@@@@      @@@    @@@   @@@  @@@    @@@  @@@@@        //
// @@@  @@@@  @@@@       @@@  @@@@@@@    @@@    @@@   @@@@  @@@  @@@@  @@@@@        //
//@@@   @@@@  @@@@@      @@@      @@@@@@ @@     @@@   @@@@   @@@@@@@    @@@@@ @@@@@ //
//@@@   @@@@@ @@@@@     @@@@        @@@  @@      @@   @@@@   @@@@@@@    @@@@@@@@@   //
//@@@    @@@@ @@@@@@@   @@@@             @@      @@   @@@@    @@@@@      @@@@@      //
//@@@    @@@@ @@@@@@@   @@@@             @@      @@   @@@@    @@@@@       @@        //
//@@@    @@@  @@@ @@@@@                          @@            @@@                  //
//            @@@ @@@                           @@             @@        STUDIOS    //
//////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// THEODATA
///////////////////////////////////////////////////////////////////////////////
//
// Description:
//		THEO file structures.
//
// Updates: (date) (person) (update)
//
// Code: Cyril Meynier
//
// Copyright (c) 1999 ARKANE Studios SA. All rights reserved
///////////////////////////////////////////////////////////////////////////////
#ifndef THEODATA_H
#define THEODATA_H

#include "core/Common.h"

#define CURRENT_THEO_VERSION	3011
#define CURRENT_SCENE_VERSION	3024
#define CURRENT_THEA_VERSION	2015

#define SAVE_MAP_IN_OBJECT		0
#define SAVE_MAP_BMP			1
#define SAVE_MAP_TGA			2


#define SIZE_IDENTITY_OBJECT	17
#define SIZE_NAME				256

#define TBOOL s32

#pragma pack(push,1)
struct THEO_3D
{
	f32			x;
	f32			y;
	f32			z;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct ArxQuat
{
	f32	w;
	f32	x;
	f32	y;
	f32	z;
};
#pragma pack(pop)

//***********************************************************************
//*		BEGIN THEO SCN FILE FORMAT structures							*
//***********************************************************************
#pragma pack(push,1)
struct TSCN_HEADER
{
	u32		version;
	s32		maps_seek;
	s32		object_seek;
	s32		anim_seek;
	s32		path_seek;
	s32		cam_seek;
	s32		light_seek;
	s32		particle_seek;
	s32		groups_seek; //Added version 3007;
	s32		nb_maps;
	u32		type_write;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct TSCN_OBJHEADER
{
	s32				next_obj;
	char				object_name[SIZE_NAME];
	TBOOL				object_state;
	TBOOL				object_state_module;
	TBOOL				object_freeze;
	TBOOL				object_selected;

}; // Aligned 1 2 4
#pragma pack(pop)

#define	LIGHT_DIRECTIONAL	0
#define	LIGHT_OMNI			1
//#define	LIGHT_OMNI			5
#define LIGHT_OMNI_SHADOW	2
#define LIGHT_SPOT			3
//#define LIGHT_SPOT			6
#define LIGHT_SPOT_SHADOW	4

#pragma pack(push,1)
struct TSCN_LIGHT
{
	char				light_name[SIZE_NAME];
	TBOOL				light_state;
	u32		light_type;
	THEO_3D				pos;
	s32				red;
	s32				green;
	s32				blue;
	f32				falloff;
	f32				hotspot;
	THEO_3D				spot_target;
	f32				intensity;
	f32				saturation;
	f32				radiosity;
	TBOOL				shadow;
	f32				factor_size;
	s32				hallow_nummap;
	f32				hallow_ray;
	f32				hallow_zmarge;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct TSCN_LIGHT_3019
{
	char				light_name[SIZE_NAME];
	TBOOL				light_state;
	TBOOL				light_selected;
	u32		light_type;
	THEO_3D				pos;
	s32				red;
	s32				green;
	s32				blue;
	f32				falloff;
	f32				hotspot;
	THEO_3D				spot_target;
	f32				intensity;
	f32				saturation;
	f32				radiosity;
	TBOOL				shadow;
	f32				factor_size;
	s32				hallow_nummap;
	f32				hallow_ray;
	f32				hallow_zmarge;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct TSCN_LIGHT_3024
{
	char				light_name[SIZE_NAME];
	TBOOL				light_state;
	TBOOL				light_selected;
	TBOOL				light_Freeze;
	u32		light_type;
	THEO_3D				pos;
	s32				red;
	s32				green;
	s32				blue;
	f32				falloff;
	f32				hotspot;
	THEO_3D				spot_target;
	f32				intensity;
	f32				saturation;
	f32				radiosity;
	TBOOL				shadow;
	f32				factor_size;
	s32				hallow_nummap;
	f32				hallow_ray;
	f32				hallow_zmarge;
}; // Aligned 1 2 4
#pragma pack(pop)

//***********************************************************************
//*		END THEO SCN FILE FORMAT structures								*
//***********************************************************************


//***********************************************************************
//*		BEGIN THEO TEO FILE FORMAT structures							*
//***********************************************************************

#pragma pack(push,1)
struct THEO_HEADER
{
	char			identity[SIZE_IDENTITY_OBJECT];
	u32	version;
	s32		maps_seek;
	s32		object_seek;
	s32		nb_maps;
	u32	type_write;
}; // NOT ALIGNED (SIZE_IDENTITY_OBJECT)
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_TEXTURE
{
	char			texture_name[SIZE_NAME];
	s32			dx;	//largeur de la texture
	s32			dy;	//hauteur de la texture
	u32	bpp; //nb de bits par pixel
	u32	map_data;  //dx*dy*sizeof(logtype)
	u32	map_type;
	s32			reflect_map;
	f32			water_intensity;
	s32			mipmap_level;
	u32	color_mask;
	TBOOL			animated_map;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_SAVE_MAPS_IN
{
	char			texture_name[SIZE_NAME];
	u32	map_type;
	s32			reflect_map;
	f32			water_intensity;
	s32			mipmap_level;
	TBOOL			animated_map;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_SAVE_MAPS_IN_3019
{
	char			texture_name[SIZE_NAME];
	u32	map_type;
	s32			reflect_map;
	f32			water_intensity;
	s32			mipmap_level;
	u32	color_mask;
	TBOOL			animated_map;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_OFFSETS
{
	s32			vertex_seek;
	s32			action_point_seek;
	s32			lines_seek;
	s32			faces_seek;
	s32			extras_seek;
	s32			groups_seek;
}; // Aligned 1 2 4 8
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_NB
{
	s32	nb_vertex;
	s32	nb_action_point;
	s32	nb_lines;
	s32	nb_faces;
	s32	nb_groups;
	u32	channel;
}; // Aligned 1 2 4 8
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_VERTEX
{
	f32			x;
	f32			y;
	f32			z;
	TBOOL			hide;
	TBOOL			freeze;
	TBOOL			isselected;
}; // Aligned 1 2 4 8
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_ACTION_POINT
{
	char			name[SIZE_NAME];
	s32			vert_index;
	s32			action;
	s32			num_sfx;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_FACE_UV
{
	s32			u1;
	s32			v1;
	s32			u2;
	s32			v2;
	s32			u3;
	s32			v3;
}; // Aligned 1 2 4 8
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_FACE_RGB
{
	s32 r;
	s32 g;
	s32 b;
};  // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_FACES
{
	u32	color;
	s32			index1;
	s32			index2;
	s32			index3;
	TBOOL			ismap;
	THEO_FACE_UV	liste_uv;
	s32			element_uv;
	s32			num_map;
	f32			tile_x;
	f32			tile_y;
	f32			user_tile_x;
	f32			user_tile_y;
	s32			flag;
	s32			collision_type;
	TBOOL		 	rgb; //au vertex
	THEO_FACE_RGB	rgb1;
	THEO_FACE_RGB	rgb2;
	THEO_FACE_RGB	rgb3;
	TBOOL			double_side;
	//TBOOL			transparency;
	u32	transparency;
	f32			trans;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_FACES_3006
{
	u32	color;
	s32			index1;
	s32			index2;
	s32			index3;
	TBOOL			ismap;
	THEO_FACE_UV	liste_uv;
	s32			element_uv;
	s32			num_map;
	f32			tile_x;
	f32			tile_y;
	f32			user_tile_x;
	f32			user_tile_y;
	s32			flag;
	s32			collision_type;
	TBOOL		 	rgb;			//au vertex
	THEO_FACE_RGB	rgb1;
	THEO_FACE_RGB	rgb2;
	THEO_FACE_RGB	rgb3;
	TBOOL			double_side;
	TBOOL			hide;
	TBOOL			transparency;
	f32			trans;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_EXTRA_DATA
{
	s32			alpha;
	s32			beta;
	s32			gamma;
	f32			posx;
	f32			posy;
	f32			posz;
	s32			origin_index;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_EXTRA_DATA_3005
{
	s32			alpha;
	s32			beta;
	s32			gamma;
	ArxQuat			quat;
	f32			posx;
	f32			posy;
	f32			posz;
	s32			origin_index;
}; // Aligned 1 2 4
#pragma pack(pop)

#define THEO_OBJECT_TYPE_3D			0
#define THEO_OBJECT_TYPE_SPRITE		1

#define THEO_RENDER_TYPE_MAP			0
#define THEO_RENDER_TYPE_MAP_LIGHT		1
#define THEO_RENDER_TYPE_MAP_GOURAUD	2
#define THEO_RENDER_TYPE_SOLID			3
#define THEO_RENDER_TYPE_LIGHT			4
#define THEO_RENDER_TYPE_GOURAUD		5
#define THEO_RENDER_TYPE_WIRE			6
#define THEO_RENDER_TYPE_VERTEX			7
#define THEO_RENDER_TYPE_BBOX			8
#define THEO_RENDER_TYPE_PREDATOR		9
#define THEO_RENDER_TYPE_CHROME			10

#pragma pack(push,1)
struct THEO_GROUPS
{
	s32			origin;
	s32			nb_index;
}; // Aligned 1 2 4 8
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_GROUPS_3011
{
	s32			origin;
	s32			symetric;
	TBOOL			lock_alpha;
	TBOOL			lock_beta;
	TBOOL			lock_gamma;
	s32			alphamin;
	s32			betamin;
	s32			gammamin;
	s32			alphamax;
	s32			betamax;
	s32			gammamax;
	s32			nb_index;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_SELECTED
{
	char			name[SIZE_NAME];
	s32			nb_index;
}; // Aligned 1 2 4
#pragma pack(pop)

//***********************************************************************
//*		END THEO TEO FILE FORMAT structures								*
//***********************************************************************

//***********************************************************************
//*		BEGIN THEO TEA Animation FILE FORMAT structures					*
//***********************************************************************
#define		THEO_SIZE_IDENTITY_ANIM 20
#define		STBOOL					s32

#pragma pack(push,1)
struct THEA_HEADER
{
	char			identity[THEO_SIZE_IDENTITY_ANIM];
	u32	version;
	char			anim_name[SIZE_NAME];
	s32			nb_frames;
	s32			nb_groups;
	s32			nb_key_frames;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEA_KEYFRAME
{
	s32			num_frame;
	s32			flag_frame;
	STBOOL			master_key_frame;
	STBOOL			key_frame; //image clef
	STBOOL			key_move;
	STBOOL			key_orient;
	STBOOL			key_morph;
	s32			time_frame;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEA_KEYFRAME_2015
{
	s32			num_frame;
	s32			flag_frame;
	char			info_frame[SIZE_NAME];
	STBOOL			master_key_frame;
	STBOOL			key_frame; //image clef
	STBOOL			key_move;
	STBOOL			key_orient;
	STBOOL			key_morph;
	s32			time_frame;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEA_KEYMOVE
{
	f32			x;
	f32			y;
	f32			z;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEA_MORPH
{
	s32		num_list_morph;
	s32		next_list_morph;
	s32		nb_inter;
	s32		start_frame;
}; // Aligned 1 2 4 8
#pragma pack(pop)

#pragma pack(push,1)
struct THEO_GROUPANIM
{
	TBOOL		key_group;
	char angle[8]; // ignored
	ArxQuat		Quaternion;
	THEO_3D		translate;
	THEO_3D		zoom;
}; // Aligned 1 2 4
#pragma pack(pop)

#pragma pack(push,1)
struct THEA_SAMPLE
{
	char			sample_name[SIZE_NAME];
	s32			sample_size;
}; // Aligned 1 2 4
#pragma pack(pop)

//***********************************************************************
//*            END THEO TEA Animation FILE FORMAT structures            *
//***********************************************************************

#endif // THEODATA_H