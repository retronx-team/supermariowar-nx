#ifndef GFX_SYS_SCREEN_SETTINGS
#define GFX_SYS_SCREEN_SETTINGS

enum gfxScreenSize {
	gfxScreenSize_AspectRatio,
	gfxScreenSize_Stretch,
	gfxScreenSize_IntegerScale,
	gfxScreenSize_PixelPerfect
};

enum gfxScreenFilter {
	gfxScreenFilter_Nearest,
	gfxScreenFilter_Linear,
	gfxScreenFilter_Best
};

#endif // GFX_SYS_SCREEN_SETTINGS
