#pragma once
#include <cstdint>

class Mapper
{
public:
	Mapper(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper();

public:
	// Transform CPU bus address into PRG ROM offset
	virtual bool CpuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
	virtual bool CpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data = 0) = 0;
	// Transform PPU bus address into CHR ROM offset
	virtual bool PpuMapRead(uint16_t addr, uint32_t& mapped_addr) = 0;
	virtual bool PpuMapWrite(uint16_t addr, uint32_t& mapped_addr) = 0;

	virtual void Reset() = 0;

protected:
	// These are stored locally as many of the mappers require this information
	uint8_t m_PRGBanks = 0;
	uint8_t m_CHRBanks = 0;
};