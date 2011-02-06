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

#include "graphics/data/CinematicTexture.h"

#include <iomanip>

#include "animation/Cinematic.h"
#include "core/Application.h"
#include "graphics/data/Texture.h"
#include "io/IO.h"
#include "io/PakManager.h"
#include "io/Logger.h"

/*-----------------------------------------------------------*/
CinematicBitmap	TabBitmap[MAX_BITMAP];
int			MaxW, MaxH;
int			NbBitmap;
bool		Restore;
/*-----------------------------------------------------------*/
extern HWND HwndPere;
extern char DirectoryChoose[];


/*-----------------------------------------------------------*/
void FreeGrille(CinematicGrid * grille);
void ReajustUV(LPDIRECT3DDEVICE7 device, int id);
/*-----------------------------------------------------------*/
void InitMapLoad(Cinematic * c)
{
	CinematicBitmap	* tb;
	int			nb;

	MaxW = MAX_WIDTH_AND_HEIGHT;
	MaxH = MAX_WIDTH_AND_HEIGHT;

	if (MaxW > 256) MaxW = 256;

	if (MaxH > 256) MaxH = 256;

	tb = TabBitmap;
	nb = MAX_BITMAP;

	while (nb)
	{
		memset((void *)tb, 0, sizeof(CinematicBitmap));
		tb++;
		nb--;
	}

	NbBitmap = 0;
	Restore = true;
}
/*-----------------------------------------------------------*/
CinematicBitmap * GetFreeBitmap(int * num)
{
	CinematicBitmap	* tb;
	int			nb;

	tb = TabBitmap;
	nb = MAX_BITMAP;

	while (nb)
	{
		if (!tb->actif)
		{
			*num = MAX_BITMAP - nb;
			return tb;
		}

		tb++;
		nb--;
	}

	return NULL;
}
/*-----------------------------------------------------------*/
bool DeleteFreeBitmap(int num)
{
	CinematicBitmap	* cb;

	cb = &TabBitmap[num];

	if (!cb->actif) return false;

	DeleteObject(cb->hbitmap);
	free((void *)cb->dir);
	free((void *)cb->name);
	FreeGrille(&cb->grid);
	memset((void *)cb, 0, sizeof(CinematicBitmap));

	NbBitmap--;

	return true;
}
/*-----------------------------------------------------------*/
bool KillTexture(LPDIRECT3DDEVICE7 device, int num)
{
	CinematicBitmap	* cb;

	cb = &TabBitmap[num];

	if (!cb->actif) return false;

	int nb = cb->grid.nbmat;

	while (nb--)
	{
		D3DTextr_KillTexture(cb->grid.mats[nb].tex);
	}

	return true;
}
/*-----------------------------------------------------------*/
void DeleteAllBitmap(LPDIRECT3DDEVICE7 device)
{
	int			nb;

	nb = MAX_BITMAP;

	while (nb)
	{
		KillTexture(device, MAX_BITMAP - nb);
		DeleteFreeBitmap(MAX_BITMAP - nb);
		nb--;
	}
}
/*-----------------------------------------------------------*/
bool AllocGrille(CinematicGrid * grille, int nbx, int nby, float tx, float ty, float dx, float dy, int echelle)
{
	grille->echelle = echelle;
	int oldnbx = nbx + 1;
	int oldnby = nby + 1;
	nbx *= echelle;
	nby *= echelle;
	float olddx = dx;
	float olddy = dy;
	dx /= ((float)echelle);
	dy /= ((float)echelle);

	grille->nbuvs = 0;
	grille->nbinds = 0;
	grille->nbuvsmalloc = grille->nbvertexs = (nbx + 1) * (nby + 1);
	grille->nbfaces = (nbx * nby) << 1;
	grille->nbindsmalloc = grille->nbfaces;
	grille->vertexs = (EERIE_3D *)malloc(grille->nbvertexs * sizeof(EERIE_3D));
	grille->uvs = (C_UV *)malloc(grille->nbvertexs * sizeof(C_UV));
	grille->inds = (C_IND *)malloc(grille->nbindsmalloc * sizeof(C_IND));
	grille->nbmat = 0;
	grille->mats = NULL;

	//vertexs
	grille->nbx = nbx;
	grille->nby = nby;
	tx *= .5f;
	ty *= .5f;
	float depx = -tx;
	float depy = -ty;
 
	EERIE_3D * v = grille->vertexs;
	float olddyy = olddy;

	while (oldnby--)
	{
		nby = echelle;

		if (!oldnby) nby = 1;

		while (nby--)
		{
 
			float olddxx = olddx;
			float depxx = depx;
			float dxx = dx;
			int oldnbxx = oldnbx;

			while (oldnbxx--)
			{
				nbx = echelle;

				if (!oldnbxx) nbx = 1;

				while (nbx--)
				{
					v->x = depxx;
					v->y = depy;
					v->z = 0.f;
					depxx += dxx;
					v++;
				}

				olddxx += olddx;

				if (olddxx > (tx * 2.f))
				{
					dxx = tx - depxx;
					dxx /= (float)echelle;
				}
				
			}

			depy += dy;
		}
		
			olddyy += olddy;

			if (olddyy > (ty * 2.f))
			{
				dy = ty - depy;
				dy /= (float)echelle;
			}
		
	}

	return true;
}
/*-----------------------------------------------------------*/
int AddMaterial(CinematicGrid * grille, TextureContainer * tex)
{
	if (!grille->nbmat)
	{
		grille->mats = (C_INDEXED *)malloc(sizeof(C_INDEXED));
	}
	else
	{
		grille->mats = (C_INDEXED *)realloc(grille->mats, sizeof(C_INDEXED) * (grille->nbmat + 1));
	}

	int deb;

	if (!grille->nbmat) deb = 0;
	else
		deb = grille->mats[grille->nbmat-1].startind + grille->mats[grille->nbmat-1].nbind;

	grille->mats[grille->nbmat].tex = tex;
	grille->mats[grille->nbmat].startind = deb;
	grille->mats[grille->nbmat].nbind = 0;

	return grille->nbmat++;
}
/*-----------------------------------------------------------*/
void FreeGrille(CinematicGrid * grille)
{
	if (grille->vertexs)
	{
		free((void *)grille->vertexs);
		grille->vertexs = NULL;
	}

	if (grille->uvs)
	{
		free((void *)grille->uvs);
		grille->uvs = NULL;
	}

	if (grille->inds)
	{
		free((void *)grille->inds);
		grille->inds = NULL;
	}

	if (grille->mats)
	{
		free((void *)grille->mats);
		grille->mats = NULL;
	}
}
/*-----------------------------------------------------------*/
void GetIndNumCube(CinematicGrid * grille, int cx, int cy, int * i1, int * i2, int * i3, int * i4)
{
	*i1 = cy * (grille->nbx + 1) + cx;
	*i2 = *i1 + 1;
	*i3 = *i1 + (grille->nbx + 1);
	*i4 = *i3 + 1;
}
/*-----------------------------------------------------------*/
void AddPoly(CinematicGrid * grille, int nummat, int i0, int i1, int i2)
{

	ARX_CHECK_USHORT(i0);
	ARX_CHECK_USHORT(i1);
	ARX_CHECK_USHORT(i2);


	if (grille->nbinds == grille->nbindsmalloc)
	{
		grille->nbindsmalloc += 100;
		grille->inds = (C_IND *)realloc((void *)grille->inds, grille->nbindsmalloc * sizeof(C_IND));
	}

	grille->inds[grille->nbinds].i1 = ARX_CLEAN_WARN_CAST_USHORT(i0);
	grille->inds[grille->nbinds].i2 = ARX_CLEAN_WARN_CAST_USHORT(i1);
	grille->inds[grille->nbinds++].i3 = ARX_CLEAN_WARN_CAST_USHORT(i2);
	grille->mats[nummat].nbind += 3;

}
/*-----------------------------------------------------------*/
void AddQuadUVs(CinematicGrid * grille, int depcx, int depcy, int tcx, int tcy, int bitmapposx, int bitmapposy, int bitmapwx, int bitmapwy, TextureContainer * tex)
{
	int nummat = AddMaterial(grille, tex);
	grille->mats[nummat].bitmapdepx = bitmapposx;
	grille->mats[nummat].bitmapdepy = bitmapposy;
	grille->mats[nummat].bitmapw = bitmapwx;
	grille->mats[nummat].bitmaph = bitmapwy;
	grille->mats[nummat].nbvertexs = (tcx + 1) * (tcy + 1);

	if ((grille->nbuvs + (4 *(tcx * tcy))) > grille->nbuvsmalloc)
	{
		grille->nbuvsmalloc += 4 * (tcx * tcy);
		grille->uvs = (C_UV *)realloc((void *)grille->uvs, grille->nbuvsmalloc * sizeof(C_UV));
	}

	float v = 0.f;
	float du = 0.999999f / (float)tcx;
	float dv = 0.999999f / (float)tcy;
	tcy++;
	tcx++;
	int tcyy = tcy;
	int depcyy = depcy;

	while (tcyy--)
	{
		float u = 0.f;
		int depcxx = depcx;
		int tcxx = tcx;

		while (tcxx--)
		{
			int i0, i1, i2, i3;
			GetIndNumCube(grille, depcxx, depcyy, &i0, &i1, &i2, &i3);

			//uvs
			grille->uvs[grille->nbuvs].indvertex = i0;
			grille->uvs[grille->nbuvs].uv.x = u;
			grille->uvs[grille->nbuvs++].uv.y = v;
			depcxx++;
			u += du;
		}

		depcyy++;
		v += dv;
	}

	tcx--;
	tcy--;

	while (tcy--)
	{
		int depcxx = depcx;
		int tcxx = tcx;

		while (tcxx--)
		{
			int i0, i1, i2, i3;
			GetIndNumCube(grille, depcxx, depcy, &i0, &i1, &i2, &i3);

			AddPoly(grille, nummat, i0, i1, i2);
			AddPoly(grille, nummat, i1, i2, i3);
			depcxx++;
		}

		depcy++;
	}
}

/*-----------------------------------------------------------*/
HBITMAP LoadTargaFile( const char * strPathname)
{
	if (!PAK_FileExist(strPathname)) return NULL;

	size_t size = 0;
	unsigned char * dat = (unsigned char *)PAK_FileLoadMalloc(strPathname, size);
	// TODO size ignored

	if (NULL == dat) return NULL;

#pragma pack(push,1)
	struct TargaHeader
	{
		BYTE IDLength;
		BYTE ColormapType;
		BYTE ImageType;
 		BYTE ColormapSpecification[5];
		WORD XOrigin;
		WORD YOrigin;
		WORD ImageWidth;
		WORD ImageHeight;
		BYTE PixelDepth;
		BYTE ImageDescriptor;
	} tga;
#pragma pack(pop)

	long pos = 0;
	memcpy(&tga, dat + pos, sizeof(TargaHeader));
	pos += sizeof(TargaHeader);

	// Only true color, non-mapped images are supported
	if ((0 != tga.ColormapType) ||
	        (tga.ImageType != 10 && tga.ImageType != 2))
	{
		free(dat);
		return NULL;
	}

	// Skip the ID field. The first byte of the header is the length of this field
	if (tga.IDLength)
		pos += tga.IDLength;

	DWORD m_dwWidth   = tga.ImageWidth;
	DWORD m_dwHeight  = tga.ImageHeight;
	DWORD m_dwBPP     = tga.PixelDepth;
	DWORD * m_pRGBAData = NULL;

	if ((m_dwBPP == 8) ||
	        (m_dwBPP == 32))
	{
		free(dat);
		return NULL;
	}

	HDC hdc = CreateCompatibleDC(NULL);
	BITMAPINFO bi;
	memset((void *)&bi.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = m_dwWidth;
	bi.bmiHeader.biHeight = m_dwHeight;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	HBITMAP hbitmap = CreateDIBSection(hdc,
	                                   &bi,
	                                   DIB_RGB_COLORS,
	                                   (void **)&m_pRGBAData,
	                                   NULL,
	                                   0);

	if (!hbitmap)
	{
		free(dat);
		return NULL;
	}

	DeleteDC(hdc);

	if (m_pRGBAData == NULL)
	{
		free(dat);
		return NULL;
	}

	for (DWORD y = 0; y < m_dwHeight; y++)
	{
		DWORD dwOffset;

		if (0 == (tga.ImageDescriptor & 0x0010))
			dwOffset = y * m_dwWidth;
		else
			dwOffset = (m_dwHeight - y - 1) * m_dwWidth;

		for (DWORD x = 0; x < m_dwWidth; x)
		{
			BYTE b;
			BYTE g;
			BYTE r;

			if (m_dwBPP == 16)
			{
				int a, bb;
				a = dat[pos++];
				bb = dat[pos++];
				SHORT w = (a & 0xFF) | ((bb & 0xFF) << 8);


				int iR = (w & 0x7C00) >> 7 ;
				int iG = (w & 0x03E0) >> 2 ;
				int iB = (w & 0x001F) << 3 ;
				ARX_CHECK_BYTE(iR);
				ARX_CHECK_BYTE(iG);
				ARX_CHECK_BYTE(iB);


				r	= ARX_CLEAN_WARN_CAST_BYTE(iR);
				g	= ARX_CLEAN_WARN_CAST_BYTE(iG);
				b	= ARX_CLEAN_WARN_CAST_BYTE(iB);


			}
			else
			{
				b = dat[pos++];
				g = dat[pos++];
				r = dat[pos++];
			}

			m_pRGBAData[dwOffset+x] = RGBA_MAKE(r, g, b, 255);
			x++;
		}
	}

	free(dat);
	return hbitmap;
}

HBITMAP LoadBMPImage( const char * strPathname)
{
	HBITMAP m_hbmBitmap = NULL;

	if (!PAK_FileExist(strPathname)) return NULL;

	size_t siz = 0;
	unsigned char * dat = (unsigned char *)PAK_FileLoadMalloc(strPathname, siz);
	// TODO siz ignored

	if (!dat) return NULL;

	// TODO packing of BITMAPINFOHEADER?
	BITMAPINFOHEADER sBitmapH = *((BITMAPINFOHEADER *)(dat + sizeof(BITMAPFILEHEADER)));
	HDC hHDC = CreateCompatibleDC(NULL);

	if (!hHDC)
	{
		free((void *)dat);
		return NULL;
	}

	char * mem = NULL;
	int iUsage;

	if (sBitmapH.biBitCount < 8)
	{
		free((void *)dat);
		DeleteDC(hHDC);
		return NULL;
	}

	if (sBitmapH.biBitCount == 8)
	{
		iUsage = DIB_PAL_COLORS;
	}
	else
	{
		iUsage = DIB_RGB_COLORS;
	}

	sBitmapH.biSize = sizeof(sBitmapH);
	m_hbmBitmap = CreateDIBSection(hHDC, (BITMAPINFO *)&sBitmapH, iUsage, (void **)&mem, NULL, 0);

	if (SelectObject(hHDC, m_hbmBitmap) == NULL)
	{
		DeleteDC(hHDC);
		return NULL;
	}

	if (!mem)
	{
		free((void *)dat);
		return NULL;
	}

	if (!m_hbmBitmap)
	{
		DeleteDC(hHDC);
		free((void *)dat);
		return NULL;
	}


	BITMAPFILEHEADER bh;

	if (iUsage == DIB_PAL_COLORS)
	{
		RGBQUAD col[256];
		memcpy(col, dat + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER), sizeof(RGBQUAD) * 256); //sBitmapH.biSize
		SetDIBColorTable(hHDC,
		                 0,      // color table index of first entry
		                 256,         // number of color table entries
		                 col // array of color table entries
		                );
	}

	bh = *((BITMAPFILEHEADER *)(dat));
	long size = sBitmapH.biSizeImage;

	if (size == 0) size = sBitmapH.biHeight * sBitmapH.biWidth * sBitmapH.biBitCount >> 3;

	memcpy(mem, dat + bh.bfOffBits, size);

	if (sBitmapH.biCompression != BI_RGB)
	{
		memset(mem, 64, size);
	}

	DeleteDC(hHDC);
	free(dat);
	return m_hbmBitmap;

}

int CreateAllMapsForBitmap(const string & path, Cinematic * c) {
	
	int n = -1;
	int pos = 0;
	
	int nbx, nby, w, h, num, id;
	CinematicBitmap	* bi;
	
	string name = GetName(path);
	if(name.empty()) {
		return -1;
	}
	
	if (n >= 0)
	{
		bi = &TabBitmap[n];

		if (!bi->actif) return -1;

		bi->load = 0;

		if (bi->name) free((void *)bi->name);

		if (bi->dir) free((void *)bi->dir);

		DeleteObject(bi->hbitmap);
		KillTexture(GDevice, n);
		FreeGrille(&bi->grid);
		NbBitmap--;

		h = 0;
		id = n;
	}
	else
	{
		bi = GetFreeBitmap(&id);

		if (!bi) return -1;
	}
	
	string dir = path;
	RemoveName(dir);
	if(!dir.empty()) {
		bi->dir = strdup(dir.c_str());
		if(!bi->dir) {
			return -1;
		}
	} else {
		bi->dir = NULL;
	}
	
	bi->name = strdup(name.c_str());
	if(!bi->name) {
		free(bi->dir);
		return -1;
	}
	
	LogDebug << "loading cinematic texture " << path;
	
	string filename = path;
	SetExt(filename, ".bmp");
	if (PAK_FileExist(filename.c_str()))
	{
		bi->hbitmap = (HBITMAP)LoadBMPImage(filename.c_str());

		if (!bi->hbitmap)
		{
			LogError << "error loading " << path;
			h = 0;

			bi->actif = 1;
			NbBitmap++;
			return id;
		}
	}
	else
	{
		SetExt(filename, ".tga");

		if (PAK_FileExist(filename.c_str()))
		{
			bi->hbitmap = LoadTargaFile(filename.c_str());

			if (!bi->hbitmap)
			{
				LogError << "error loading " << path;
				h = 0;

				bi->actif = 1;
				NbBitmap++;
				return id;
			}
		}
		else
		{
			LogError << path << " not found";
			return -1;
		}
	}

	BITMAP bm;
	GetObject(bi->hbitmap, sizeof(BITMAP), &bm);

	nbx = (bm.bmWidth / MaxW);
	nby = (bm.bmHeight / MaxH);

	if (bm.bmWidth % MaxW) nbx++;

	if (bm.bmHeight % MaxH) nby++;

	bi->w = bm.bmWidth;
	bi->h = bm.bmHeight;

	bi->load = 1;

	bi->nbx = nbx;
	bi->nby = nby;

	AllocGrille(&bi->grid, nbx, nby, (float)bi->w, (float)bi->h, (float)((bi->w > MaxW) ? MaxW : bi->w), (float)((bi->h > MaxH) ? MaxH : bi->h), 1);

	num = 0;
	h = bi->h;
	float dy = -(float)(bi->h >> 1);

	while (nby)
	{
		float dx = -(float)(bi->w >> 1);

		int h2;

		if ((h - MaxH) < 0) h2 = h;
		else h2 = MaxH;

		w = bi->w;
		int nbxx = nbx;

		while (nbxx)
		{
			int w2;

			if ((w - MaxW) < 0) w2 = w;
			else w2 = MaxW;

			std::stringstream ss;
			ss << name << '_' << std::setw(4) << num;
			std::string texname = ss.str();
			//AllTxt, "%s_%4d", name, num);
			MakeUpcase(texname);

			if (FAILED(D3DTextr_CreateEmptyTexture(texname.c_str(), w2, h2, 0, D3DTEXTR_NO_MIPMAP, 0)))
			{
				LogError<< "Creating texture #" << num << " -> x: " << (long)dx << " y: " << (long)dy << " w: " << w2 << " h: " << h2;
			}

			TextureContainer * t = FindTexture(texname.c_str());
			AddQuadUVs(&bi->grid, bi->nbx - nbxx, bi->nby - nby, 1, 1, bi->w - w, bi->h - h, w2, h2, t);

			dx += (float)w2;
			w -= MaxW;

			num++;
			nbxx--;
		}

		dy += (float)h2;
		h -= MaxH;
		nby--;
	}

	bi->actif = 1;
	NbBitmap++;

	if (n >= 0)
	{
		h = 0;
	}
	else
	{
		h = 0;
	}

	c->ActiveTexture(id);
	ReajustUV(GDevice, id);

	return id;
}
/*-----------------------------------------------------------*/
bool ReCreateAllMapsForBitmap(int id, int nmax, Cinematic * c, LPDIRECT3DDEVICE7 device)
{
	int			nbx, nby, w, h, num;
	CinematicBitmap	* bi;

	if (!c) return false;

	bi = &TabBitmap[id];

	if (!bi->actif) return false;

	FreeGrille(&bi->grid);

	nbx = (bi->w / MaxW);
	nby = (bi->h / MaxH);

	if (bi->w % MaxW) nbx++;

	if (bi->h % MaxH) nby++;

	bi->nbx = nbx;
	bi->nby = nby;

	AllocGrille(&bi->grid, nbx, nby, (float)bi->w, (float)bi->h, (float)((bi->w > MaxW) ? MaxW : bi->w), (float)((bi->h > MaxH) ? MaxH : bi->h), nmax);

	num = 0;
	h = bi->h;
	float dy = -(float)(bi->h >> 1);

	while (nby)
	{
		float dx = -(float)(bi->w >> 1);

		int h2;

		if ((h - MaxH) < 0) h2 = h;
		else h2 = MaxH;

		w = bi->w;
		int nbxx = nbx;

		while (nbxx)
		{
			int w2;

			if ((w - MaxW) < 0) w2 = w;
			else w2 = MaxW;

			std::stringstream ss;
			ss << bi->name << '_' << std::setw(4) << num;
			std::string texname = ss.str();
			//sprintf(AllTxt, "%s_%4d", bi->name, num);
			MakeUpcase(texname);

			TextureContainer * t = FindTexture(texname);
			AddQuadUVs(&bi->grid, (bi->nbx - nbxx)*nmax, (bi->nby - nby)*nmax, nmax, nmax, bi->w - w, bi->h - h, w2, h2, t);

			dx += (float)w2;
			w -= MaxW;

			num++;
			nbxx--;
		}

		dy += (float)h2;
		h -= MaxH;
		nby--;
	}

	ReajustUV(GDevice, id);

	return true;
}
/*-----------------------------------------------------------*/
bool Cinematic::ActiveTexture(int id)
{

	TextureContainer	* tc;
	CinematicBitmap		*	cb;
	int					nb;

	if (id >= MAX_BITMAP) return false;

	if (id < 0)			return false;

	cb = &TabBitmap[id];

	if (!cb->actif) return false;

	C_INDEXED	* mat = cb->grid.mats;
 
	nb = cb->grid.nbmat;

	while (nb--)
	{
		tc = mat->tex;

		if (!tc) return false;

		if (tc->Restore(GDevice) != S_OK) return false;	

		if (tc->CopyBitmapToSurface2(cb->hbitmap, mat->bitmapdepx, mat->bitmapdepy, mat->bitmapw, mat->bitmaph, 1) != S_OK) return false;

		mat++;
	}

	return true;
}
/*-----------------------------------------------------------*/
void ReajustUV(LPDIRECT3DDEVICE7 device, int id)
{
	TextureContainer	* tc;
	CinematicBitmap		*	cb;
	int					nb;

	cb = &TabBitmap[id];

	if (!cb->actif) return;

	C_INDEXED	* mat = cb->grid.mats;
	C_UV	*	uvs = cb->grid.uvs;

	nb = cb->grid.nbmat;

	while (nb--)
	{
		tc = mat->tex;

		if (!tc) return;

		int w, h;

		w = tc->m_dwWidth;
		h = tc->m_dwHeight;

		if ((w != (int)mat->bitmapw) ||
		        (h != (int)mat->bitmaph))
		{
			float dx = (.999999f * (float)mat->bitmapw) / ((float)w);
			float dy = (.999999f * (float)mat->bitmaph) / ((float)h);

			int nb2 = mat->nbvertexs;

			while (nb2--)
			{
				uvs->uv.x *= dx;
				uvs->uv.y *= dy;
				uvs++;
			}
		}
		else
		{
			uvs += mat->nbvertexs;
		}

		mat++;
	}
}
/*-----------------------------------------------------------*/
bool ActiveAllTexture(Cinematic * c)
{
	int			nb;
	CinematicBitmap	* cb;
	bool		flag = false;

	cb = TabBitmap;
	nb = MAX_BITMAP;

	while (nb)
	{
		if (cb->actif)
		{
			flag |= !c->ActiveTexture(MAX_BITMAP - nb);
		}

		cb++;
		nb--;
	}

	return !flag;
}