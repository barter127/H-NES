#pragma once
#include <cstdint>
#include <memory>

#include "olcPixelGameEngine.h"

#include "Cartridge.h"

class PPU2C02
{
public:
	PPU2C02();
	~PPU2C02();

private:
	uint8_t     tblName[2][1024];
	uint8_t     tblPattern[2][4096];
	uint8_t		tblPalette[32];

private:
	olc::Pixel  palScreen[0x40];
	// In Video
	// olc::Sprite sprScreen = olc::Sprite(256, 240);
	// olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256, 240) };
	// olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128), olc::Sprite(128, 128) };

	// Changed To for API breaking subsequent PGE Update
	olc::Sprite* sprScreen;
	olc::Sprite* sprNameTable[2];
	olc::Sprite* sprPatternTable[2];


public:
	// Debugging Utilities
	olc::Sprite& GetScreen();
	olc::Sprite& GetNameTable(uint8_t i);
	olc::Sprite& GetPatternTable(uint8_t i, uint8_t palette);

	olc::Pixel& GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);

	bool m_isFrameComplete = false;

private:

	union
	{
		struct
		{
			uint8_t unused : 5;
			uint8_t spriteOverflow : 1;
			uint8_t spriteZeroHit : 1;
			uint8_t verticalBlank : 1;
		};

		uint8_t reg;
	} status;


	union
	{
		struct
		{
			uint8_t grayscale : 1;
			uint8_t renderBackgroundLeft : 1;
			uint8_t renderSpritesLeft : 1;
			uint8_t renderBackground : 1;
			uint8_t renderSprites : 1;
			uint8_t enhanceRed : 1;
			uint8_t enhanceGreen : 1;
			uint8_t enhanceBlue : 1;
		};

		uint8_t reg;
	} mask;

	union PPUCTRL
	{
		struct
		{
			uint8_t nametableX : 1;
			uint8_t nametableY : 1;
			uint8_t incrementMode : 1;
			uint8_t patternSprite : 1;
			uint8_t patternBackground : 1;
			uint8_t spriteSize : 1;
			uint8_t slaveMode : 1; // unused
			uint8_t enableNmi : 1;
		};

		uint8_t reg;
	} control;

	union loopy_register
	{
		// Credit to Loopy for working this out :D
		struct
		{

			uint16_t coarseX : 5;
			uint16_t coarseY : 5;
			uint16_t nametableX : 1;
			uint16_t nametableY : 1;
			uint16_t fineY : 3;
			uint16_t unused : 1;
		};

		uint16_t reg = 0x0000;
	};


	loopy_register m_vramAddr; // Active "pointer" address into nametable to extract background tile info
	loopy_register m_tramAddr; // Temporary store of information to be "transferred" into "pointer" at various times

	// Pixel offset horizontally
	uint8_t m_fineX = 0x00;

	// Internal communications
	uint8_t m_addressLatch = 0x00;
	uint8_t m_ppuDataBuffer = 0x00;

	// Pixel "dot" position information
	int16_t m_scanline = 0;
	int16_t m_cycle = 0;

	// Background rendering
	uint8_t bgNextTileId = 0x00;
	uint8_t bgNextTileAttrib = 0x00;
	uint8_t bgNextTileLsb = 0x00;
	uint8_t bgNextTileMsb = 0x00;
	uint16_t bgShifterPatternLo = 0x0000;
	uint16_t bgShifterPatternHi = 0x0000;
	uint16_t bgShifterAttribLo = 0x0000;
	uint16_t bgShifterAttribHi = 0x0000;

	struct ObjectAttributeEntry
	{
		uint8_t y;
		uint8_t id;
		uint8_t attribute;
		uint8_t x;
	} m_OAM[64];

	uint8_t m_oamAddress = 0x00;

	ObjectAttributeEntry m_spriteScanline[8];
	uint8_t m_spriteCount;
	uint8_t spriteShifterPatternLo[8];
	uint8_t spriteShifterPatternHi[8];

	bool m_spriteZeroHitPossible = false;
	bool m_spriteZeroBeingRendered = false;

public:
	uint8_t* m_OAMPtr = (uint8_t*)m_OAM;


public:
	// Communications with Main Bus
	uint8_t CpuRead(uint16_t addr, bool rdonly = false);
	void    CpuWrite(uint16_t addr, uint8_t  data);

	// Communications with PPU Bus
	uint8_t PpuRead(uint16_t addr, bool rdonly = false);
	void    PpuWrite(uint16_t addr, uint8_t data);

private:
	// The Cartridge or "GamePak"
	std::shared_ptr<Cartridge> m_cartridge;

public:
	// Interface
	void ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge);
	void Clock();
	void Reset();
	bool m_nmi = false;
};

