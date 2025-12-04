#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>

#include "Mapper_000.h"

class Cartridge
{
public:
	Cartridge(const std::string& sFileName);
	~Cartridge();

public:
	bool ImageValid();

	enum MIRROR
	{
		HORIZONTAL,
		VERTICAL,
		ONESCREEN_LO,
		ONESCREEN_HI,
	} m_mirror = HORIZONTAL;

private:
	bool m_isImageValid = false;

	uint8_t m_mapperID = 0;
	uint8_t m_PRGBanks = 0; // Connected to CPU.
	uint8_t m_CHRBanks = 0; // Connected to PPU.

	std::vector<uint8_t> m_PRGMemory;
	std::vector<uint8_t> m_CHRMemory;

	std::shared_ptr<Mapper> m_mapper;

public:
	// Read from Main Bus.
	bool CpuRead(uint16_t addr, uint8_t& data);
	// Write to Main Bus.
	bool CpuWrite(uint16_t addr, uint8_t data);

	// Read from PPU Bus
	bool PpuRead(uint16_t addr, uint8_t& data);
	// Write to PPU Bus
	bool PpuWrite(uint16_t addr, uint8_t data);

	void Reset();
};