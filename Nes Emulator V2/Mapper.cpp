#include "Mapper.h"

#include "Mapper.h"

// Mappers take the requested address from the CPU/PPU and transform them into addresses in cartridge mem.
// The CPU/PPU is unaware of the data it's reading and writing. All of it is handled by mappers.

Mapper::Mapper(uint8_t prgBanks, uint8_t chrBanks)
{
	m_PRGBanks = prgBanks;
	m_CHRBanks = chrBanks;

	Reset();
}


Mapper::~Mapper()
{
}

void Mapper::Reset()
{

}