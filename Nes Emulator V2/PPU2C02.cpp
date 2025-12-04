#include "PPU2C02.h"

PPU2C02::PPU2C02()
{
	palScreen[0x00] = olc::Pixel(84, 84, 84);
	palScreen[0x01] = olc::Pixel(0, 30, 116);
	palScreen[0x02] = olc::Pixel(8, 16, 144);
	palScreen[0x03] = olc::Pixel(48, 0, 136);
	palScreen[0x04] = olc::Pixel(68, 0, 100);
	palScreen[0x05] = olc::Pixel(92, 0, 48);
	palScreen[0x06] = olc::Pixel(84, 4, 0);
	palScreen[0x07] = olc::Pixel(60, 24, 0);
	palScreen[0x08] = olc::Pixel(32, 42, 0);
	palScreen[0x09] = olc::Pixel(8, 58, 0);
	palScreen[0x0A] = olc::Pixel(0, 64, 0);
	palScreen[0x0B] = olc::Pixel(0, 60, 0);
	palScreen[0x0C] = olc::Pixel(0, 50, 60);
	palScreen[0x0D] = olc::Pixel(0, 0, 0);
	palScreen[0x0E] = olc::Pixel(0, 0, 0);
	palScreen[0x0F] = olc::Pixel(0, 0, 0);

	palScreen[0x10] = olc::Pixel(152, 150, 152);
	palScreen[0x11] = olc::Pixel(8, 76, 196);
	palScreen[0x12] = olc::Pixel(48, 50, 236);
	palScreen[0x13] = olc::Pixel(92, 30, 228);
	palScreen[0x14] = olc::Pixel(136, 20, 176);
	palScreen[0x15] = olc::Pixel(160, 20, 100);
	palScreen[0x16] = olc::Pixel(152, 34, 32);
	palScreen[0x17] = olc::Pixel(120, 60, 0);
	palScreen[0x18] = olc::Pixel(84, 90, 0);
	palScreen[0x19] = olc::Pixel(40, 114, 0);
	palScreen[0x1A] = olc::Pixel(8, 124, 0);
	palScreen[0x1B] = olc::Pixel(0, 118, 40);
	palScreen[0x1C] = olc::Pixel(0, 102, 120);
	palScreen[0x1D] = olc::Pixel(0, 0, 0);
	palScreen[0x1E] = olc::Pixel(0, 0, 0);
	palScreen[0x1F] = olc::Pixel(0, 0, 0);

	palScreen[0x20] = olc::Pixel(236, 238, 236);
	palScreen[0x21] = olc::Pixel(76, 154, 236);
	palScreen[0x22] = olc::Pixel(120, 124, 236);
	palScreen[0x23] = olc::Pixel(176, 98, 236);
	palScreen[0x24] = olc::Pixel(228, 84, 236);
	palScreen[0x25] = olc::Pixel(236, 88, 180);
	palScreen[0x26] = olc::Pixel(236, 106, 100);
	palScreen[0x27] = olc::Pixel(212, 136, 32);
	palScreen[0x28] = olc::Pixel(160, 170, 0);
	palScreen[0x29] = olc::Pixel(116, 196, 0);
	palScreen[0x2A] = olc::Pixel(76, 208, 32);
	palScreen[0x2B] = olc::Pixel(56, 204, 108);
	palScreen[0x2C] = olc::Pixel(56, 180, 204);
	palScreen[0x2D] = olc::Pixel(60, 60, 60);
	palScreen[0x2E] = olc::Pixel(0, 0, 0);
	palScreen[0x2F] = olc::Pixel(0, 0, 0);

	palScreen[0x30] = olc::Pixel(236, 238, 236);
	palScreen[0x31] = olc::Pixel(168, 204, 236);
	palScreen[0x32] = olc::Pixel(188, 188, 236);
	palScreen[0x33] = olc::Pixel(212, 178, 236);
	palScreen[0x34] = olc::Pixel(236, 174, 236);
	palScreen[0x35] = olc::Pixel(236, 174, 212);
	palScreen[0x36] = olc::Pixel(236, 180, 176);
	palScreen[0x37] = olc::Pixel(228, 196, 144);
	palScreen[0x38] = olc::Pixel(204, 210, 120);
	palScreen[0x39] = olc::Pixel(180, 222, 120);
	palScreen[0x3A] = olc::Pixel(168, 226, 144);
	palScreen[0x3B] = olc::Pixel(152, 226, 180);
	palScreen[0x3C] = olc::Pixel(160, 214, 228);
	palScreen[0x3D] = olc::Pixel(160, 162, 160);
	palScreen[0x3E] = olc::Pixel(0, 0, 0);
	palScreen[0x3F] = olc::Pixel(0, 0, 0);

	sprScreen = new olc::Sprite(256, 240);
	sprNameTable[0] = new olc::Sprite(256, 240);
	sprNameTable[1] = new olc::Sprite(256, 240);
	sprPatternTable[0] = new olc::Sprite(128, 128);
	sprPatternTable[1] = new olc::Sprite(128, 128);
}


PPU2C02::~PPU2C02()
{
	delete sprScreen;
	delete sprNameTable[0];
	delete sprNameTable[1];
	delete sprPatternTable[0];
	delete sprPatternTable[1];
}

olc::Sprite& PPU2C02::GetScreen()
{
	// Simply returns the current sprite holding the rendered screen
	return *sprScreen;
}



olc::Sprite& PPU2C02::GetPatternTable(uint8_t i, uint8_t palette)
{
	// Loop through all 16x16 tiles.
	for (uint16_t nTileY = 0; nTileY < 16; nTileY++)
	{
		for (uint16_t nTileX = 0; nTileX < 16; nTileX++)
		{
			// Convert the 2D tile coordinate into a 1D offset into the pattern table memory.
			uint16_t nOffset = nTileY * 256 + nTileX * 16;

			// Loop through 8 rows of 8 pixels.
			for (uint16_t row = 0; row < 8; row++)
			{
				// Read both bit planes of the character in order to extract the least significant and most significant bits.
				// Corresponding rows are 8 bits apart.
				uint8_t tile_lsb = PpuRead(i * 0x1000 + nOffset + row + 0x0000);
				uint8_t tile_msb = PpuRead(i * 0x1000 + nOffset + row + 0x0008);

				for (uint16_t col = 0; col < 8; col++)
				{
					// We can get the index value by simply adding the bits together but we're only interested in the lsb.
					uint8_t pixel = ((tile_lsb & 0x01) << 1)| (tile_msb & 0x01);

					// Shift the row words 1 bit right for each column of the character.
					tile_lsb >>= 1; tile_msb >>= 1;

					// Translate pattern table location to screen colour and X,Y pos.
					sprPatternTable[i]->SetPixel
					(
						nTileX * 8 + (7 - col),
						nTileY * 8 + row,
						GetColourFromPaletteRam(palette, pixel)
					);
				}
			}
		}
	}

	return *sprPatternTable[i];
}


olc::Pixel& PPU2C02::GetColourFromPaletteRam(uint8_t palette, uint8_t pixel)
{
	// This is a convenience function that takes a specified palette and pixel
	// index and returns the appropriate screen colour.
	// "0x3F00"       - Offset into PPU addressable range where palettes are stored
	// "palette << 2" - Each palette is 4 bytes in size
	// "pixel"        - Each pixel index is either 0, 1, 2 or 3
	// "& 0x3F"       - Stops us reading beyond the bounds of the palScreen array
	return palScreen[PpuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

olc::Sprite& PPU2C02::GetNameTable(uint8_t i)
{
	return *sprNameTable[i];
}


uint8_t PPU2C02::CpuRead(uint16_t addr, bool rdonly)
{
	uint8_t data = 0x00;

	if (rdonly)
	{
		switch (addr)
		{
		case 0x0000: // Control.
			data = control.reg;
			break;
		case 0x0001: // Mask.
			data = mask.reg;
			break;
		case 0x0002: // Status.
			data = status.reg;
			break;
		case 0x0003: // OAM Address.
			break;
		case 0x0004: // OAM Data.
			break;
		case 0x0005: // Scroll.
			break;
		case 0x0006: // PPU Address.
			break;
		case 0x0007: // PPU Data.
			break;
		}
	}
	else
	{
		switch (addr)
		{
			// Control - Not readable.
		case 0x0000: break;

			// Mask - Not Readable.
		case 0x0001: break;

			// Status.
		case 0x0002:
			data = (status.reg & 0xE0) | (m_ppuDataBuffer & 0x1F);

			// Clear the vertical blanking flag.
			status.verticalBlank = 0;

			// Reset Loopy's Address latch flag.
			m_addressLatch = 0;
			break;

			// OAM Address.
		case 0x0003: break;

			// OAM Data.
		case 0x0004: break;
			data = m_OAMPtr[m_oamAddress];

			// Scroll - Not Readable.
		case 0x0005: break;

			// PPU Address - Not Readable.
		case 0x0006: break;

			// PPU Data.
		case 0x0007:
			// Reads from the NameTable ram get delayed one m_cycle, 
			// so output buffer which contains the data from the 
			// previous read request
			data = m_ppuDataBuffer;
			// then update the buffer for next time
			m_ppuDataBuffer = PpuRead(m_vramAddr.reg);
			// However, if the address was in the palette range, the
			// data is not delayed, so it returns immediately
			if (m_vramAddr.reg >= 0x3F00) data = m_ppuDataBuffer;
			// All reads from PPU data automatically increment the nametable
			// address depending upon the mode set in the control register.
			// If set to vertical mode, the increment is 32, so it skips
			// one whole nametable row; in horizontal mode it just increments
			// by 1, moving to the next column
			m_vramAddr.reg += (control.incrementMode ? 32 : 1);
			break;
		}
	}

	return data;
}

void PPU2C02::CpuWrite(uint16_t addr, uint8_t data)
{
	switch (addr)
	{
	case 0x0000: // Control.
		control.reg = data;
		m_tramAddr.nametableX = control.nametableX;
		m_tramAddr.nametableY = control.nametableY;
		break;
	case 0x0001: // Mask.
		mask.reg = data;
		break;
	case 0x0002: // Status.
		break;
	case 0x0003: // OAM Address.
		m_oamAddress = data;
		break;
	case 0x0004: // OAM Data.
		m_OAMPtr[m_oamAddress] = data;
		break;
	case 0x0005: // Scroll.
		if (m_addressLatch == 0)
		{
			// First write to scroll register contains X offset in pixel space which get split into coarse and fine x values.
			m_fineX = data & 0x07;
			m_tramAddr.coarseX = data >> 3;
			m_addressLatch = 1;
		}
		else
		{
			// First write to scroll register contains Y offset in pixel space which we split into coarse and fine Y values.
			m_tramAddr.fineY = data & 0x07;
			m_tramAddr.coarseY = data >> 3;
			m_addressLatch = 0;
		}
		break;
	case 0x0006: // PPU Address.
		if (m_addressLatch == 0)
		{
			// PPU address bus can be accessed by CPU via the ADDR and DATA registers.
			m_tramAddr.reg = (uint16_t)((data & 0x3F) << 8) | (m_tramAddr.reg & 0x00FF);
			m_addressLatch = 1;
		}
		else
		{
			// When a whole address has been written, the internal vram address buffer is updated.
			m_tramAddr.reg = (m_tramAddr.reg & 0xFF00) | data;
			m_vramAddr = m_tramAddr;
			m_addressLatch = 0;
		}
		break;
	case 0x0007: // PPU Data.
		PpuWrite(m_vramAddr.reg, data);
		// All writes from PPU data automatically increment the nametable address depending upon the mode set in the control register.
		// If vertical mode skip a nametable row.
		// If horizontal just go to next column.
		m_vramAddr.reg += (control.incrementMode ? 32 : 1);
		break;
	}
}

uint8_t PPU2C02::PpuRead(uint16_t addr, bool rdonly)
{
	uint8_t data = 0x00;
	addr &= 0x3FFF;

	if (m_cartridge->PpuRead(addr, data))
	{

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		// If the cartridge cant map the address, have a physical location ready.
		data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF)
	{
		addr &= 0x0FFF;

		if (m_cartridge->m_mirror == Cartridge::MIRROR::VERTICAL)
		{
			// Vertical.
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = tblName[1][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = tblName[1][addr & 0x03FF];
		}
		else if (m_cartridge->m_mirror == Cartridge::MIRROR::HORIZONTAL)
		{
			// Horizontal.
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = tblName[1][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = tblName[1][addr & 0x03FF];
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
	{
		addr &= 0x001F;
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		data = tblPalette[addr] & (mask.grayscale ? 0x30 : 0x3F);
	}

	return data;
}

void PPU2C02::PpuWrite(uint16_t addr, uint8_t data)
{
	addr &= 0x3FFF;

	if (m_cartridge->PpuWrite(addr, data))
	{

	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
	}
	else if (addr >= 0x2000 && addr <= 0x3EFF)
	{
		addr &= 0x0FFF;
		if (m_cartridge->m_mirror == Cartridge::MIRROR::VERTICAL)
		{
			// Vertical.
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = data;
		}
		else if (m_cartridge->m_mirror == Cartridge::MIRROR::HORIZONTAL)
		{
			// Horizontal.
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = data;
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
	{
		addr &= 0x001F;
		if (addr == 0x0010) addr = 0x0000;
		if (addr == 0x0014) addr = 0x0004;
		if (addr == 0x0018) addr = 0x0008;
		if (addr == 0x001C) addr = 0x000C;
		tblPalette[addr] = data;
	}
}

void PPU2C02::ConnectCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	this->m_cartridge = cartridge;
}

void PPU2C02::Reset()
{
	m_fineX = 0x00;
	m_addressLatch = 0x00;
	m_ppuDataBuffer = 0x00;
	m_scanline = 0;
	m_cycle = 0;
	bgNextTileId = 0x00;
	bgNextTileAttrib = 0x00;
	bgNextTileLsb = 0x00;
	bgNextTileMsb = 0x00;
	bgShifterPatternLo = 0x0000;
	bgShifterPatternHi = 0x0000;
	bgShifterAttribLo = 0x0000;
	bgShifterAttribHi = 0x0000;
	status.reg = 0x00;
	mask.reg = 0x00;
	control.reg = 0x00;
	m_vramAddr.reg = 0x0000;
	m_tramAddr.reg = 0x0000;
}

void PPU2C02::Clock()
{
	// Increment the background tile "pointer" one tile/column horizontally.
	auto IncrementScrollX = [&]()
	{
		// If rendering is enabled
		if (mask.renderBackground || mask.renderSprites)
		{
			// A single name table is 32x30 tiles. We may cross into a neighbour nametable so handle wrap around.
			if (m_vramAddr.coarseX == 31)
			{
				// Leaving nametable so wrap address round.
				m_vramAddr.coarseX = 0;
				// Flip target nametable bit.
				m_vramAddr.nametableX = ~m_vramAddr.nametableX;
			}
			else
			{
				m_vramAddr.coarseX++;
			}
		}
	};

	// Increment the background tile "pointer" one m_scanline vertically.
	auto IncrementScrollY = [&]()
	{
		// If rendering is enabled.
		if (mask.renderBackground || mask.renderSprites)
		{
			// If possible, just increment the fine y offset.
			if (m_vramAddr.fineY < 7)
			{
				m_vramAddr.fineY++;
			}
			else
			{
				// Coarse Y - row of nametable.
				// Fine Y - specific scanline.

				// Reset fine y offset
				m_vramAddr.fineY = 0;

				// If we need to swap vertical nametable targets.
				if (m_vramAddr.coarseY == 29)
				{
					m_vramAddr.coarseY = 0;
					m_vramAddr.nametableY = ~m_vramAddr.nametableY;
				}
				else if (m_vramAddr.coarseY == 31)
				{
					// In case the pointer is in the attribute memory, we just wrap around the current nametable.
					m_vramAddr.coarseY = 0;
				}
				else
				{
					m_vramAddr.coarseY++;
				}
			}
		}
	};

	// Transfer the temporarily stored horizontal nametable access information into the "pointer".
	auto TransferAddressX = [&]()
	{
		// Ony if rendering is enabled
		if (mask.renderBackground || mask.renderSprites)
		{
			m_vramAddr.nametableX = m_tramAddr.nametableX;
			m_vramAddr.coarseX = m_tramAddr.coarseX;
		}
	};

	// Transfer the temporarily stored vertical nametable access information into the "pointer".
	auto TransferAddressY = [&]()
	{
		// Ony if rendering is enabled
		if (mask.renderBackground || mask.renderSprites)
		{
			m_vramAddr.fineY = m_tramAddr.fineY;
			m_vramAddr.nametableY = m_tramAddr.nametableY;
			m_vramAddr.coarseY = m_tramAddr.coarseY;
		}
	};


	// Setup the "in-effect" background tile shifters ready for outputting next 8 pixels in m_scanline.
	auto LoadBackgroundShifters = [&]()
	{
		// Move shift register by 1.
		// Top 8 bits are the pixels being drawn. next 8 bits are pixels to be drawn.
		// The required bit is always MSB and LSB
		bgShifterPatternLo = (bgShifterPatternLo & 0xFF00) | bgNextTileLsb;
		bgShifterPatternHi = (bgShifterPatternHi & 0xFF00) | bgNextTileMsb;

		// Attribute bits change every 8 pixels.
		// Take the bottomw two bits of the word (representing the palette).
		bgShifterAttribLo = (bgShifterAttribLo & 0xFF00) | ((bgNextTileAttrib & 0b01) ? 0xFF : 0x00);
		bgShifterAttribHi = (bgShifterAttribHi & 0xFF00) | ((bgNextTileAttrib & 0b10) ? 0xFF : 0x00);
	};

	// Every cycle the pattern and attribute shifters shift by 1 bit as each cycle a new pixel is drawn.
	auto UpdateShifters = [&]()
	{
		if (mask.renderBackground)
		{
			// Shifting background tile pattern row.
			bgShifterPatternLo <<= 1;
			bgShifterPatternHi <<= 1;

			// Shifting palette attributes by 1.
			bgShifterAttribLo <<= 1;
			bgShifterAttribHi <<= 1;
		}

		if (mask.renderSprites && m_cycle >= 1 && m_cycle < 258)
		{
			for (int i = 0; i < m_spriteCount; i++)
			{
				if (m_spriteScanline[i].x > 0)
				{
					m_spriteScanline[i].x--;
				}
				else
				{
					spriteShifterPatternLo[i] <<= 1;
					spriteShifterPatternHi[i] <<= 1;
				}
			}
		}
	};

	// The pre-render scanline at -1, is used to configure the "shifters" for the first visible m_scanline, 0.
	// It is not visible.
	if (m_scanline >= -1 && m_scanline < 240)
	{
		if (m_scanline == 0 && m_cycle == 0)
		{
			// "Odd Frame" m_cycle skip.
			m_cycle = 1;
		}

		if (m_scanline == -1 && m_cycle == 1)
		{
			// Effectively start of new frame, so clear vertical blank flag.
			status.verticalBlank = 0;
			status.spriteZeroHit = 0;
			status.spriteOverflow = 0;

			for (int i = 0; i < 8; i++)
			{
				spriteShifterPatternLo[i] = 0;
				spriteShifterPatternHi[i] = 0;
			}
		}


		if ((m_cycle >= 2 && m_cycle < 258) || (m_cycle >= 321 && m_cycle < 338))
		{
			UpdateShifters();

			// The shifters at this point should have been loaded with data to draw this scanline.
			switch ((m_cycle - 1) % 8)
			{
			case 0: // Each scanline.
				// Load the current background tile pattern and attributes into the shift register.
				LoadBackgroundShifters();

				// Fetch the next background tile ID.
				// "(m_vramAddr.reg & 0x0FFF)" : Mask to 12 bits that are relevant.
				// "| 0x2000"                 : Offset into nametable space on PPU address bus.
				bgNextTileId = PpuRead(0x2000 | (m_vramAddr.reg & 0x0FFF));

				break;
			case 2:
				// Can assume a single palette is applied to a 2x2 tile combination of the 4x4 tile zone.
				// To find the attribute byte for a group of 4x4 tiles divide the 32x32 address by 8 and offset into the target nametable.

				// All attribute memory begins at 0x03C0 within a nametable OR with result to select the target nametable, and attribute byte offset.
				// Then OR with 0x2000 to offset into nametable address space on PPU bus.				
				bgNextTileAttrib = PpuRead(0x23C0 | (m_vramAddr.nametableY << 11)
					| (m_vramAddr.nametableX << 10)
					| ((m_vramAddr.coarseY >> 2) << 3)
					| (m_vramAddr.coarseX >> 2));

				// Need to break the attribute byte into 2x2 tile groups.
				// We can analyse the bottom bits of coarse coords to provide us with the correct offset into the 8 bit word.
				// If coarse x % 4 < 2 we are in the left half else in the right.
				// We wabt tge bottom two bits of the word so shift.			
				if (m_vramAddr.coarseY & 0x02) bgNextTileAttrib >>= 4;
				if (m_vramAddr.coarseX & 0x02) bgNextTileAttrib >>= 2;
				bgNextTileAttrib &= 0x03;
				break;

			case 4:
				// Fetch the next background tile LSB bit plane from the pattern memory
				// The Tile ID has been read from the nametable. We will use this id to 
				// index into the pattern memory to find the correct sprite (assuming
				// the sprites lie on 8x8 pixel boundaries in that memory, which they do
				// even though 8x16 sprites exist, as background tiles are always 8x8).
				//
				// Since the sprites are effectively 1 bit deep, but 8 pixels wide, we 
				// can represent a whole sprite row as a single byte, so offsetting
				// into the pattern memory is easy. In total there is 8KB so we need a 
				// 13 bit address.

				// Get the next BG tile LSB bit from pattern memory.
				// The tileID has already been read so use the id to index the pattern memory to find the correct sprite.
				// A sprite can be represented as a single byte so offsetting is easy.

				// "(control.pattern_background << 12)"  : the pattern memory selector 
				//                                         from control register, either 0K
				//                                         or 4K offset
				// "((uint16_t)bg_next_tile_id << 4)"    : the tile id multiplied by 16, as
				//                                         2 lots of 8 rows of 8 bit pixels
				// "(m_vramAddr.fineY)"                  : Offset into which row based on
				//                                         vertical scroll offset
				bgNextTileLsb = PpuRead((control.patternBackground << 12)
					+ ((uint16_t)bgNextTileId << 4)
					+ (m_vramAddr.fineY));

				break;
			case 6:
				// Fetch the next background tile MSB bit plane from the pattern memory.
				// This is the same as above, but has a +8 offset to select the next bit plane.
				bgNextTileMsb = PpuRead((control.patternBackground << 12)
					+ ((uint16_t)bgNextTileId << 4)
					+ (m_vramAddr.fineY) + 8);
				break;
			case 7:
				// Increment the background tile "pointer" to the next tile horizontally in the nametable memory. 
				IncrementScrollX();
				break;
			}
		}

		// End of a scanline so move downwards.
		if (m_cycle == 256)
		{
			IncrementScrollY();
		}

		// Reset the x position.
		if (m_cycle == 257)
		{
			LoadBackgroundShifters();
			TransferAddressX();
		}

		if (m_cycle == 338 || m_cycle == 340)
		{
			bgNextTileId = PpuRead(0x2000 | (m_vramAddr.reg & 0x0FFF));
		}

		// Sprite evaluation phase.
		if (m_cycle == 257 && m_scanline >= 0)
		{
			std::memset(m_spriteScanline, 0xFF, 8 * sizeof(ObjectAttributeEntry));
			m_spriteCount = 0;

			m_spriteZeroHitPossible = false;

			uint8_t OAMEntry = 0;
			while (OAMEntry < 64 && m_spriteCount < 9)
			{
				int16_t diff = ((uint16_t)m_scanline - (uint16_t)m_OAM[OAMEntry].y);

				if (diff >= 0 && diff < (control.spriteSize ? 16 : 8))
				{
					if (OAMEntry == 0)
					{
						m_spriteZeroHitPossible = true;
					}

					std::memcpy(&m_spriteScanline[m_spriteCount], &m_OAM[OAMEntry], sizeof(ObjectAttributeEntry));
					m_spriteCount++;
				}
				OAMEntry++;
			}
			status.spriteOverflow - (m_spriteCount > 8);
		}

		if (m_cycle == 340)
		{
			for (uint8_t i = 0; i < m_spriteCount; i++)
			{
				uint8_t spritePatternBitsLo, spritePatternBitsHi;
				uint16_t spritePatternAddrLo, spritePatternAddrHi;

				if (!control.spriteSize) // 8x8 sprite mode.
				{
					if (!(m_spriteScanline[i].attribute & 0x80)) // Sprite is not flipped vertically.
					{
						spritePatternAddrLo =
							(control.patternSprite << 12)
							| (m_spriteScanline[i].id << 4)
							| (m_scanline - m_spriteScanline[i].y);

					}
					else
					{
						spritePatternAddrLo =
							(control.patternSprite << 12)
							| (m_spriteScanline[i].id << 4)
							| (7 - (m_scanline - m_spriteScanline[i].y) );
					}
				}
				else // 8 x 16 sprite mode.
				{
					if (!(m_spriteScanline[i].attribute & 0x80)) // Sprite is not flipped vertically.
					{
						if (m_scanline - m_spriteScanline[i].y < 8)
						{
							spritePatternAddrLo =
								((m_spriteScanline[i].id & 0x01) << 12)
								| ((m_spriteScanline[i].id & 0xFE) << 4)
								| ((m_scanline - m_spriteScanline[i].y) & 0x07);
						}
						else
						{
							spritePatternAddrLo =
								((m_spriteScanline[i].id & 0x01) << 12)
								| (((m_spriteScanline[i].id & 0xFE) + 1) << 4)
								| ((m_scanline - m_spriteScanline[i].y) & 0x07);
						}
					}
					else
					{
						if (m_scanline - m_spriteScanline[i].y < 8)
						{
							spritePatternAddrLo =
								((m_spriteScanline[i].id & 0x01) << 12)
								| (((m_spriteScanline[i].id & 0xFE) + 1) << 4)
								| (7 - (m_scanline - m_spriteScanline[i].y) & 0x07);
						}
						{
							// Reading bottom half of tile.
							spritePatternAddrLo =
								((m_spriteScanline[i].id & 0x01) << 12)
								| ((m_spriteScanline[i].id & 0xFE) << 4)
								| (7 - (m_scanline - m_spriteScanline[i].y) & 0x07);
						}
					}
				}

				spritePatternAddrHi = spritePatternAddrLo + 8;
				spritePatternBitsLo = PpuRead(spritePatternAddrLo);
				spritePatternBitsHi = PpuRead(spritePatternAddrHi);

				if (m_spriteScanline[i].attribute & 0x40)
				{
					auto flipbyte = [](uint8_t b)
					{
						b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
						b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
						b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
						return b;
					};

					spritePatternBitsLo = flipbyte(spritePatternBitsLo);
					spritePatternBitsHi = flipbyte(spritePatternBitsHi);
				}

				spriteShifterPatternLo[i] = spritePatternBitsLo;
				spriteShifterPatternHi[i] = spritePatternBitsHi;
			}
		}

		if (m_scanline == -1 && m_cycle >= 280 && m_cycle < 305)
		{
			// End of vertical blank period so reset the Y address ready for rendering
			TransferAddressY();
		}
	}

	if (m_scanline == 240)
	{
		// End of scanline.
	}

	if (m_scanline >= 241 && m_scanline < 261)
	{
		if (m_scanline == 241 && m_cycle == 1)
		{
			// Set vertical blank flag.
			status.verticalBlank = 1;

			// Emit an NMI during vertical blank if CPU asks for it.
			// NMI makes it possible to write to VRAM without visible tearing or artifact.
				m_nmi = true;
		}
	}

	uint8_t bgPixel = 0x00;   // The 2-bit pixel to be rendered
	uint8_t bgPalette = 0x00; // The 3-bit index of the palette.

	if (mask.renderBackground)
	{
		// Select the relevant bit based on fineX.
		uint16_t bit_mux = 0x8000 >> m_fineX;

		// Select pixels by extracting from the shifter at the required location. 
		uint8_t p0Pixel = (bgShifterPatternLo & bit_mux) > 0;
		uint8_t p1Pixel = (bgShifterPatternHi & bit_mux) > 0;

		// Combine to form pixel index.
		bgPixel = (p1Pixel << 1) | p0Pixel;

		// Get palette.
		uint8_t bgPal0 = (bgShifterAttribLo & bit_mux) > 0;
		uint8_t bgPal1 = (bgShifterAttribHi & bit_mux) > 0;
		bgPalette = (bgPal1 << 1) | bgPal0;
	}

	uint8_t fgPixel = 0x00;
	uint8_t fgPalette = 0x00;
	uint8_t fgPriority = 0x00;

	if (mask.renderSprites)
	{
		for (uint8_t i = 0; i < m_spriteCount; i++)
		{
			m_spriteZeroBeingRendered = false;

			if (m_spriteScanline[i].x == 0)
			{
				uint8_t fgPixelLo = (spriteShifterPatternLo[i] & 0x80) > 0;
				uint8_t fgPixelHi = (spriteShifterPatternHi[i] & 0x80) > 0;
				fgPixel = (fgPixelHi << 1) | fgPixelLo;

				fgPalette = (m_spriteScanline[i].attribute & 0x03) + 0x04;
				fgPriority = (m_spriteScanline[i].attribute & 0x20) == 0;

				if (fgPixel != 0)
				{
					if (i == 0)
					{
						m_spriteZeroBeingRendered = true;
					}

					break;
				}
			}
		}
	}

	uint8_t pixel = 0x00;
	uint8_t palette = 0x00;

	if (bgPixel == 0 && fgPixel == 0)
	{
		pixel = 0x00;
		palette = 0x00;
	}
	else if (bgPixel == 0 && fgPixel > 0)
	{
		pixel = fgPixel;
		palette = fgPalette;
	}
	else if (bgPixel > 0 && fgPixel == 0)
	{
		pixel = bgPixel;
		palette = bgPalette;
	}
	else if (bgPixel > 0 && fgPixel > 0)
	{
		if (fgPriority)
		{
			pixel = fgPixel;
			palette = fgPalette;
		}
		else
		{
			pixel = bgPixel;
			palette = bgPalette;
		}

		if (m_spriteZeroHitPossible && m_spriteZeroBeingRendered)
		{
			if (~(mask.renderBackgroundLeft | mask.renderSpritesLeft))
			{
				status.spriteZeroHit = 1;
			}
		}
	}

	// Draw to screen.
	sprScreen->SetPixel(m_cycle - 1, m_scanline, GetColourFromPaletteRam(palette, pixel));

	m_cycle++;
	if (m_cycle >= 341)
	{
		m_cycle = 0;
		m_scanline++;
		if (m_scanline >= 261)
		{
			m_scanline = -1;
			m_isFrameComplete = true;
		}
	}
}
