#pragma once
#include <cstdint>
#include <array>

#include "CPU6502.h"
#include "PPU2C02.h"
#include "APU2A03.h"
#include "Cartridge.h"

class Bus
{
public:
	Bus();
	~Bus();

public: // Devices on Main Bus

	CPU6502 m_cpu;
	PPU2C02 m_ppu;
	APU2A03 m_apu;

	std::shared_ptr<Cartridge> m_cartridge;

	// 2KB of RAM.
	uint8_t m_cpuRam[2048];

	uint8_t m_controller[2];

public: // Main Bus Read and Write.
	void    CpuWrite(uint16_t addr, uint8_t data);
	uint8_t CpuRead(uint16_t addr, bool bReadOnly = false);

	double m_audioSample = 0.0;

private:
	// A count of how many clocks have passed
	uint32_t m_systemClockCounter = 0;
	// Internal cache of controller state
	uint8_t m_controllerState[2];

	uint8_t dmaPage = 0x00;
	uint8_t dmaAddress = 0x00;
	uint8_t dmaData = 0x00;

	bool dmaTransfer = false;
	bool dmaDummy = true;

	double audioTimePerSystemSample = 0.0;
	double audioTimePerNESClock = 0.0;
	double audioTime = 0.0;

public: // System Interface
	// Connects a cartridge object to the internal buses
	void InsertCartridge(const std::shared_ptr<Cartridge>& cartridge);
	// Resets the system
	void Reset();
	// Clocks the system - a single whole systme tick
	bool Clock();

	void SetSampleFrequency(uint32_t sampleRate);
};