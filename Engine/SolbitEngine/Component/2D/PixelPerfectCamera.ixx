module;

import Common;
import Math;

export module SolbitEngine.Component.PixelPerfectCamera;

export namespace solbit
{
	struct PixelPerfectCamera
	{
		int32 PixelPerUnit;	// Number of pixels per Unit(1m)
		IVector2 RefResoulution;

		PixelPerfectCamera()
			: PixelPerUnit(16)
			, RefResoulution({ 640, 360 })
		{

		}
	};
}