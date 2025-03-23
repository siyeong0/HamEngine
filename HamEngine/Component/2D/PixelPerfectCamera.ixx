module;

import Common;
import Math;

export module HamEngine.Component.PixelPerfectCamera;

export namespace ham
{
	struct PixelPerfectCamera
	{
		int32 PixelPerUnit;	// Number of pixels per Unit(1m)
		Vec2i RefResoulution;
	};
}