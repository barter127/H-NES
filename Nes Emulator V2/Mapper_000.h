#pragma once
#include "Mapper.h"


class Mapper_000 : public Mapper
{
public:
	Mapper_000(uint8_t prgBanks, uint8_t chrBanks);
	~Mapper_000();

public:
	bool CpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
	bool CpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data = 0) override;
	bool PpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
	bool PpuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;
	void Reset() override;
};
