#include "Cartridge.h"

Cartridge::Cartridge(const std::string& sFileName)
{
	// iNES Format Header
	struct sHeader
	{
		char name[4];
		uint8_t prgRomChunks;
		uint8_t chrRomChunks;
		uint8_t mapper1;
		uint8_t mapper2;
		uint8_t prgRamSize;
		uint8_t tvSystem1;
		uint8_t tvSystem2;
		char unused[5];
	} header;

	m_isImageValid = false;

	std::ifstream ifs;
	ifs.open(sFileName, std::ifstream::binary);
	if (ifs.is_open())
	{
		// Read file header
		ifs.read((char*)&header, sizeof(sHeader));

		// If trainer is found skip.
		// I need to understand trainers. I believe they're to do with rom hacks.
		if (header.mapper1 & 0x04)
			ifs.seekg(512, std::ios_base::cur);

		// Get Mapper ID.
		m_mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
		m_mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

		// "Discover" File Format
		uint8_t nFileType = 1;

		if (nFileType == 0)
		{

		}

		if (nFileType == 1)
		{
			m_PRGBanks = header.prgRomChunks;
			m_PRGMemory.resize(m_PRGBanks * 16384);
			ifs.read((char*)m_PRGMemory.data(), m_PRGMemory.size());

			m_CHRBanks = header.chrRomChunks;
			if (m_CHRBanks == 0)
			{
				// Create CHR RAM.
				m_CHRMemory.resize(8192);
			}
			else
			{
				// Allocate for ROM.
				m_CHRMemory.resize(m_CHRBanks * 8192);
			}
			ifs.read((char*)m_CHRMemory.data(), m_CHRMemory.size());
		}

		if (nFileType == 2)
		{

		}

		// I will support other mappers eventually.
		switch (m_mapperID)
		{
		case   0: m_mapper = std::make_shared<Mapper_000>(m_PRGBanks, m_CHRBanks); break;
		}

		m_isImageValid = true;
		ifs.close();
	}

}


Cartridge::~Cartridge()
{
}

bool Cartridge::ImageValid()
{
	return m_isImageValid;
}

bool Cartridge::CpuRead(uint16_t addr, uint8_t& data)
{
	uint32_t mapped_addr = 0;
	if (m_mapper->CpuMapRead(addr, mapped_addr))
	{
		data = m_PRGMemory[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::CpuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (m_mapper->CpuMapWrite(addr, mapped_addr, data))
	{
		m_PRGMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}

bool Cartridge::PpuRead(uint16_t addr, uint8_t& data)
{
	uint32_t mapped_addr = 0;
	if (m_mapper->PpuMapRead(addr, mapped_addr))
	{
		data = m_CHRMemory[mapped_addr];
		return true;
	}
	else
		return false;
}

bool Cartridge::PpuWrite(uint16_t addr, uint8_t data)
{
	uint32_t mapped_addr = 0;
	if (m_mapper->PpuMapWrite(addr, mapped_addr))
	{
		m_CHRMemory[mapped_addr] = data;
		return true;
	}
	else
		return false;
}


void Cartridge::Reset()
{
	if (m_mapper != nullptr)
		m_mapper->Reset();
}