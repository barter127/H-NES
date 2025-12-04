#include "Bus.h"

Bus::Bus()
{
	m_cpu.ConnectBus(this);
}


Bus::~Bus()
{
}

void Bus::CpuWrite(uint16_t addr, uint8_t data)
{
	if (m_cartridge->CpuWrite(addr, data))
	{

	}
	// RAM.
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		m_cpuRam[addr & 0x07FF] = data;

	}
	// PPU address.
	else if (addr >= 0x2000 && addr <= 0x3FFF)
	{
		m_ppu.CpuWrite(addr & 0x0007, data);
	}  
	// APU address.
	else if ((addr >= 0x4000 && addr <= 0x4013) || addr == 0x4015 || addr == 0x4017)
	{
		m_apu.CpuWrite(addr, data);
	}
	// dma address.
	else if (addr == 0x4014)
	{
		dmaPage = data;
		dmaAddress = 0x00;
		dmaTransfer = true;
	}
	// Controller address.
	else if (addr >= 0x4016 && addr <= 0x4017)
	{
		m_controllerState[addr & 0x0001] = m_controller[addr & 0x0001];
	}
}

uint8_t Bus::CpuRead(uint16_t addr, bool bReadOnly)
{
	uint8_t data = 0x00;
	if (m_cartridge->CpuRead(addr, data))
	{
		// Cartridge Address Range
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		// System RAM Address Range, mirrored every 2048.
		data = m_cpuRam[addr & 0x07FF];
	}
	else if (addr >= 0x2000 && addr <= 0x3FFF)
	{
		// PPU Address range, mirrored every 8
		data = m_ppu.CpuRead(addr & 0x0007, bReadOnly);
	}
	else if (addr >= 0x4016 && addr <= 0x4017)
	{
		data = (m_controllerState[addr & 0x0001] & 0x80) > 0;
		m_controllerState[addr & 0x0001] <<= 1; // Shift the shift register.
	}

	return data;
}

void Bus::InsertCartridge(const std::shared_ptr<Cartridge>& cartridge)
{
	// Connects cartridge to both Main Bus and CPU Bus
	this->m_cartridge = cartridge;
	m_ppu.ConnectCartridge(cartridge);
}

void Bus::Reset()
{
	m_cartridge->Reset();
	m_cpu.Reset();
	m_ppu.Reset();
	m_apu.Reset();
	m_systemClockCounter = 0;
}

bool Bus::Clock()
{
	m_ppu.Clock();

	m_apu.Clock();

	if (m_systemClockCounter % 3 == 0)
	{
		if (dmaTransfer)
		{
			// Waiting to synchronise with clock cycle.
			if (dmaDummy)
			{
				// Synchronise on odd clock cycle.
				if (m_systemClockCounter % 2 == 1)
				{
					dmaDummy = false;
				}
				else
				{

				}
			}
			else
			{
				if (m_systemClockCounter % 2 == 0)
				{
					dmaData = CpuRead(dmaPage << 8 | dmaAddress);
				}
				else
				{
					m_ppu.m_OAMPtr[dmaAddress] = dmaData;
					dmaAddress++;

					// When address wraps around we've finished.
					if (dmaAddress == 0x00)
					{
						dmaTransfer = false;
						dmaDummy = true;
					}
				}
			}
		}
		else
		{
			m_cpu.Clock();
		}
	}

	bool audioSampleReady = false;
	audioTime += audioTimePerNESClock;

	if (audioTime >= audioTimePerSystemSample)
	{
		audioTime -= audioTimePerSystemSample;
		m_audioSample = m_apu.GetOutputSample();
		audioSampleReady = true;
	}

	// The ppu can interrupt during vertical blank period. If done, send the interrupt to the CPU.
	if (m_ppu.m_nmi)
	{
		m_ppu.m_nmi = false;
		m_cpu.Nmi();
	}

	m_systemClockCounter++;

	return audioSampleReady;
}

void Bus::SetSampleFrequency(uint32_t sampleRate)
{
	static float ppuClockFrequency = 5369318.0;

	audioTimePerSystemSample = 1.0 / (double)sampleRate;
	audioTimePerNESClock = 1.0 / 5369318.0;
}