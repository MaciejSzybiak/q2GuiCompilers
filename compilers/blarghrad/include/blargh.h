#pragma once

namespace blarghrad
{

	extern "C" {
		struct BlarghData
		{
			int   dump = false;
			int   bounce = 0;
			int   verbose = false;
			int   extra = false;
			int   update = false;
			int   threads = 0;
			int   chop = 0;
			int   chopsky = 0;
			int   chopwarp = 0;
			int   choplight = 0;
			int   chopcurve = 0;
			float scale = 1;
			float direct = 1;
			float entity = 1;
			float texscale = 1;
			int   glView = false;
			int   nopvs = false;
			int   nocolor = false;
			float saturation = 1;
			float ambient = 0;
			float minlight = 0;
			float maxlight = 0;
			float gamma = 1;
			float stylemin = 0;
			float radmin = 0;
			int   nobmodlight = false;
			int   nobouncefix = false;
			int   nobrightsurf = false;
			int   nostopbleed = false;
			int   nocurve = false;
			int   nosplotchfix = false;
			int   noradorigin = false;
			int   lightwarp = false;

			std::string gamedir;
			std::string moddir;
			std::string bspName;
		};

		int exec_blarghrad(BlarghData data);
	}
}