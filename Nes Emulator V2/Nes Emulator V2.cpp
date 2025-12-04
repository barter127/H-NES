#include <iostream>
#include <sstream>

#include "Bus.h"
#include "CPU6502.h"

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define OLC_PGEX_SOUND
#include "olcPGEX_Sound.h"

class Demo_olc2C02 : public olc::PixelGameEngine
{
public:
	Demo_olc2C02() { sAppName = "NES Emulator"; }

private:
	// The NES
	Bus nes;
	std::shared_ptr<Cartridge> cart;
	bool bEmulationRun = false;
	float fResidualTime = 0.0f;

	uint8_t nSelectedPalette = 0x00;

private:
	// Support Utilities
	std::map<uint16_t, std::string> mapAsm;

	std::string hex(uint32_t n, uint8_t d)
	{
		std::string s(d, '0');
		for (int i = d - 1; i >= 0; i--, n >>= 4)
			s[i] = "0123456789ABCDEF"[n & 0xF];
		return s;
	};

	void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns)
	{
		int nRamX = x, nRamY = y;
		for (int row = 0; row < nRows; row++)
		{
			std::string sOffset = "$" + hex(nAddr, 4) + ":";
			for (int col = 0; col < nColumns; col++)
			{
				sOffset += " " + hex(nes.CpuRead(nAddr, true), 2);
				nAddr += 1;
			}
			DrawString(nRamX, nRamY, sOffset);
			nRamY += 10;
		}
	}

	void DrawCpu(int x, int y)
	{
		std::string status = "STATUS: ";
		DrawString(x, y, "STATUS:", olc::WHITE);
		DrawString(x + 64, y, "N", nes.m_cpu.status & CPU6502::N ? olc::GREEN : olc::RED);
		DrawString(x + 80, y, "V", nes.m_cpu.status & CPU6502::V ? olc::GREEN : olc::RED);
		DrawString(x + 96, y, "-", nes.m_cpu.status & CPU6502::U ? olc::GREEN : olc::RED);
		DrawString(x + 112, y, "B", nes.m_cpu.status & CPU6502::B ? olc::GREEN : olc::RED);
		DrawString(x + 128, y, "D", nes.m_cpu.status & CPU6502::D ? olc::GREEN : olc::RED);
		DrawString(x + 144, y, "I", nes.m_cpu.status & CPU6502::I ? olc::GREEN : olc::RED);
		DrawString(x + 160, y, "Z", nes.m_cpu.status & CPU6502::Z ? olc::GREEN : olc::RED);
		DrawString(x + 178, y, "C", nes.m_cpu.status & CPU6502::C ? olc::GREEN : olc::RED);
		DrawString(x, y + 10, "PC: $" + hex(nes.m_cpu.pc, 4));
		DrawString(x, y + 20, "A: $" + hex(nes.m_cpu.accumulator, 2) + "  [" + std::to_string(nes.m_cpu.accumulator) + "]");
		DrawString(x, y + 30, "X: $" + hex(nes.m_cpu.xReg, 2) + "  [" + std::to_string(nes.m_cpu.xReg) + "]");
		DrawString(x, y + 40, "Y: $" + hex(nes.m_cpu.yReg, 2) + "  [" + std::to_string(nes.m_cpu.yReg) + "]");
		DrawString(x, y + 50, "Stack P: $" + hex(nes.m_cpu.stkptr, 4));
	}

	void DrawCode(int x, int y, int nLines)
	{
		auto it_a = mapAsm.find(nes.m_cpu.pc);
		int nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			DrawString(x, nLineY, (*it_a).second, olc::CYAN);
			while (nLineY < (nLines * 10) + y)
			{
				nLineY += 10;
				if (++it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}

		it_a = mapAsm.find(nes.m_cpu.pc);
		nLineY = (nLines >> 1) * 10 + y;
		if (it_a != mapAsm.end())
		{
			while (nLineY > y)
			{
				nLineY -= 10;
				if (--it_a != mapAsm.end())
				{
					DrawString(x, nLineY, (*it_a).second);
				}
			}
		}
	}


	static float SoundOut(int channel, float globalTime, float timeStep)
	{
		while (!instancePtr->nes.Clock()) {}
		return static_cast<float>(instancePtr->nes.m_audioSample);
	}

	bool OnUserCreate()
	{
		// Load the cartridge
		cart = std::make_shared<Cartridge>("nestest.nes");

		if (!cart->ImageValid())
			return false;

		// Load cart.
		nes.InsertCartridge(cart);

		// Extract Assembly.
		mapAsm = nes.m_cpu.Disassemble(0x0000, 0xFFFF);


		instancePtr = this;
		nes.SetSampleFrequency(44100);
		olc::SOUND::InitialiseAudio(44100, 1, 8, 512);
		olc::SOUND::SetUserSynthFunction(SoundOut);

		// Reset NES
		nes.Reset();
		return true;
	}
	inline static Demo_olc2C02* instancePtr;

	bool OnUserDestroy()
	{
		olc::SOUND::DestroyAudio();
		return true;
	}

	void EmulatorUpdateWithAudio(float elapsedTime)
	{
		Clear(olc::DARK_BLUE);

		nes.m_controller[0] = 0x00;
		nes.m_controller[0] |= GetKey(olc::Key::X).bHeld ? 0x80 : 0x00;
		nes.m_controller[0] |= GetKey(olc::Key::Z).bHeld ? 0x40 : 0x00;
		nes.m_controller[0] |= GetKey(olc::Key::A).bHeld ? 0x20 : 0x00;
		nes.m_controller[0] |= GetKey(olc::Key::S).bHeld ? 0x10 : 0x00;
		nes.m_controller[0] |= GetKey(olc::Key::UP).bHeld ? 0x08 : 0x00;
		nes.m_controller[0] |= GetKey(olc::Key::DOWN).bHeld ? 0x04 : 0x00;
		nes.m_controller[0] |= GetKey(olc::Key::LEFT).bHeld ? 0x02 : 0x00;
		nes.m_controller[0] |= GetKey(olc::Key::RIGHT).bHeld ? 0x01 : 0x00;

		if (GetKey(olc::Key::SPACE).bPressed) bEmulationRun = !bEmulationRun;
		if (GetKey(olc::Key::R).bPressed) nes.Reset();
		if (GetKey(olc::Key::P).bPressed) (++nSelectedPalette) &= 0x07;

		DrawCpu(516, 2);

		for (int i = 0; i < 26; i++)
		{
			std::string str = hex(i, 2) + ": (" + std::to_string(nes.m_ppu.m_OAMPtr[i * 4])
				+ ", " + std::to_string(nes.m_ppu.m_OAMPtr[i * 4 + 0]) + ") "
				+ "ID: " + hex(nes.m_ppu.m_OAMPtr[i * 4 + 1], 2) +
				+"AT: " + hex(nes.m_ppu.m_OAMPtr[i * 4 + 2], 2);
			DrawString(516, 72 + i * 10, str);
		}

		// Draw Palettes & Pattern Tables
		const int nSwatchSize = 6;
		for (int p = 0; p < 8; p++)
			for (int s = 0; s < 4; s++)
				FillRect(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340,
					nSwatchSize, nSwatchSize, nes.m_ppu.GetColourFromPaletteRam(p, s));

		// Draw selection around selected palette.
		DrawRect(516 + nSelectedPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4), nSwatchSize, olc::WHITE);

		// Generate pattern tables.
		DrawSprite(516, 348, &nes.m_ppu.GetPatternTable(0, nSelectedPalette));
		DrawSprite(648, 348, &nes.m_ppu.GetPatternTable(1, nSelectedPalette));

		// Draw output.
		DrawSprite(0, 0, &nes.m_ppu.GetScreen(), 2);
	}


	bool OnUserUpdate(float elapsedTime)
	{
		EmulatorUpdateWithAudio(elapsedTime);
		return true;
	}
};

int main()
{
	Demo_olc2C02 demo;
	demo.Construct(780, 480, 2, 2);
	demo.Start();
	return 0;
}