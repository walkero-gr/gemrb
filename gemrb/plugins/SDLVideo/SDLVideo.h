/* GemRB - Infinity Engine Emulator
 * Copyright (C) 2003 The GemRB Project
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 */

#ifndef SDLVIDEODRIVER_H
#define SDLVIDEODRIVER_H

#include "Video.h"

#include "GUI/EventMgr.h"
#include "win32def.h"

#include "SDLSurfaceSprite2D.h"

#include <vector>
#include <SDL.h>

typedef unsigned char
#ifdef __GNUC__
	__attribute__((aligned(4)))
#endif
	Pixel;

namespace GemRB {

#if SDL_VERSION_ATLEAST(1,3,0)
#define SDL_SRCCOLORKEY SDL_TRUE
#define SDL_SRCALPHA 0
#define SDLKey SDL_Keycode
#define SDLK_SCROLLOCK SDLK_SCROLLLOCK
#define SDLK_KP1 SDLK_KP_1
#define SDLK_KP2 SDLK_KP_2
#define SDLK_KP3 SDLK_KP_3
#define SDLK_KP4 SDLK_KP_4
#define SDLK_KP6 SDLK_KP_6
#define SDLK_KP7 SDLK_KP_7
#define SDLK_KP8 SDLK_KP_8
#define SDLK_KP9 SDLK_KP_9
#else
	typedef Sint32 SDL_Keycode;
#endif

inline int GetModState(int modstate)
{
	int value = 0;
	if (modstate&KMOD_SHIFT) value |= GEM_MOD_SHIFT;
	if (modstate&KMOD_CTRL) value |= GEM_MOD_CTRL;
	if (modstate&KMOD_ALT) value |= GEM_MOD_ALT;
	return value;
}
	
inline SDL_Rect RectFromRegion(const Region& rgn)
{
	SDL_Rect rect = {Sint16(rgn.x), Sint16(rgn.y), Uint16(rgn.w), Uint16(rgn.h)};
	return rect;
}

class SDLVideoDriver : public Video {
public:
	SDLVideoDriver(void);
	virtual ~SDLVideoDriver(void);
	int Init(void);

	virtual bool SetFullscreenMode(bool set)=0;

	virtual bool ToggleGrabInput()=0;
	virtual void ShowSoftKeyboard()=0;
	virtual void HideSoftKeyboard()=0;
	virtual bool TouchInputEnabled()=0;

	virtual Sprite2D* CreateSprite(int w, int h, int bpp, ieDword rMask,
		ieDword gMask, ieDword bMask, ieDword aMask, void* pixels,
		bool cK = false, int index = 0);
	virtual Sprite2D* CreateSprite8(int w, int h, void* pixels,
							Palette* palette, bool cK, int index);
	virtual Sprite2D* CreatePalettedSprite(int w, int h, int bpp, void* pixels,
								   Color* palette, bool cK = false, int index = 0);

	virtual void BlitTile(const Sprite2D* spr, const Sprite2D* mask, int x, int y,
						  const Region* clip, unsigned int flags);
	virtual void BlitSprite(const Sprite2D* spr, const Region& src, const Region& dst);
	virtual void BlitGameSprite(const Sprite2D* spr, int x, int y, unsigned int flags, Color tint,
								SpriteCover* cover, const Region* clip = NULL);

	/** This function Draws the Border of a Rectangle as described by the Region parameter. The Color used to draw the rectangle is passes via the Color parameter. */
	virtual void DrawRect(const Region& rgn, const Color& color, bool fill = true);
	/** This functions Draws a Circle */
	void SetPixel(const Point&, const Color& color);
	/** Gets the pixel of the backbuffer surface */
	void GetPixel(short x, short y, Color& color);
	virtual void DrawCircle(short cx, short cy, unsigned short r, const Color& color);
	/** This functions Draws an Ellipse Segment */
	void DrawEllipseSegment(short cx, short cy, unsigned short xr, unsigned short yr, const Color& color,
		double anglefrom, double angleto, bool drawlines = true);
	/** This functions Draws an Ellipse */
	virtual void DrawEllipse(short cx, short cy, unsigned short xr, unsigned short yr, const Color& color);
	/** This function Draws a Polygon on the Screen */
	virtual void DrawPolyline(Gem_Polygon* poly, const Point& origin, const Color& color, bool fill = false);
	virtual void DrawHLine(short x1, short y, short x2, const Color& color);
	virtual void DrawVLine(short x, short y1, short y2, const Color& color);
	virtual void DrawLine(short x1, short y1, short x2, short y2, const Color& color);
	/** Blits a Sprite filling the Region */
	void BlitTiled(Region rgn, const Sprite2D* img);

	void SetFadeColor(int r, int g, int b);
	void SetFadePercent(int percent);

protected:
#if SDL_VERSION_ATLEAST(1,3,0)
	typedef SDL_Texture vid_buf_t;
	typedef SDLTextureSprite2D sprite_t;
#else
	typedef SDL_Surface vid_buf_t;
	typedef SDLSurfaceSprite2D sprite_t;
#endif

	virtual inline vid_buf_t* CurrentRenderBuffer()=0;

	void BlitSurfaceClipped(SDL_Surface*, const Region& src, const Region& dst);
	virtual bool SetSurfaceAlpha(SDL_Surface* surface, unsigned short alpha)=0;
	void SetPixel(short x, short y, const Color& color);
	int PollEvents();
	/* used to process the SDL events dequeued by PollEvents or an arbitraty event from another source.*/
	virtual int ProcessEvent(const SDL_Event & event);
	void Wait(unsigned long w) { SDL_Delay(w); }

public:
	// static functions for manipulating surfaces
	static void SetSurfacePalette(SDL_Surface* surf, SDL_Color* pal, int numcolors = 256);
	static void SetSurfacePixel(SDL_Surface* surf, short x, short y, const Color& color);
	static Color GetSurfacePixel(SDL_Surface* surf, short x, short y);
};

}

#endif
