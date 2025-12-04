#include "CPU6502.h"
#include "Bus.h"

CPU6502::CPU6502()
{
	// Create translation table for instructions.
	// 16x16 entries with 256 instructions in total.
	// Evil initialiser list of initaialiser lists of death and evil.
	using a = CPU6502;
	m_lookup =
	{
		{ "BRK", &a::BRK, &a::IMM, 7 },{ "ORA", &a::ORA, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::ZP0, 3 },{ "ASL", &a::ASL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHP", &a::PHP, &a::IMP, 3 },{ "ORA", &a::ORA, &a::IMM, 2 },{ "ASL", &a::ASL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABS, 4 },{ "ASL", &a::ASL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BPL", &a::BPL, &a::REL, 2 },{ "ORA", &a::ORA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ZPX, 4 },{ "ASL", &a::ASL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLC", &a::CLC, &a::IMP, 2 },{ "ORA", &a::ORA, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ORA", &a::ORA, &a::ABX, 4 },{ "ASL", &a::ASL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "JSR", &a::JSR, &a::ABS, 6 },{ "AND", &a::AND, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "BIT", &a::BIT, &a::ZP0, 3 },{ "AND", &a::AND, &a::ZP0, 3 },{ "ROL", &a::ROL, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLP", &a::PLP, &a::IMP, 4 },{ "AND", &a::AND, &a::IMM, 2 },{ "ROL", &a::ROL, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "BIT", &a::BIT, &a::ABS, 4 },{ "AND", &a::AND, &a::ABS, 4 },{ "ROL", &a::ROL, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BMI", &a::BMI, &a::REL, 2 },{ "AND", &a::AND, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ZPX, 4 },{ "ROL", &a::ROL, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEC", &a::SEC, &a::IMP, 2 },{ "AND", &a::AND, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "AND", &a::AND, &a::ABX, 4 },{ "ROL", &a::ROL, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTI", &a::RTI, &a::IMP, 6 },{ "EOR", &a::EOR, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "EOR", &a::EOR, &a::ZP0, 3 },{ "LSR", &a::LSR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PHA", &a::PHA, &a::IMP, 3 },{ "EOR", &a::EOR, &a::IMM, 2 },{ "LSR", &a::LSR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::ABS, 3 },{ "EOR", &a::EOR, &a::ABS, 4 },{ "LSR", &a::LSR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVC", &a::BVC, &a::REL, 2 },{ "EOR", &a::EOR, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ZPX, 4 },{ "LSR", &a::LSR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLI", &a::CLI, &a::IMP, 2 },{ "EOR", &a::EOR, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "EOR", &a::EOR, &a::ABX, 4 },{ "LSR", &a::LSR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "RTS", &a::RTS, &a::IMP, 6 },{ "ADC", &a::ADC, &a::IZX, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 3 },{ "ADC", &a::ADC, &a::ZP0, 3 },{ "ROR", &a::ROR, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "PLA", &a::PLA, &a::IMP, 4 },{ "ADC", &a::ADC, &a::IMM, 2 },{ "ROR", &a::ROR, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "JMP", &a::JMP, &a::IND, 5 },{ "ADC", &a::ADC, &a::ABS, 4 },{ "ROR", &a::ROR, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BVS", &a::BVS, &a::REL, 2 },{ "ADC", &a::ADC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ZPX, 4 },{ "ROR", &a::ROR, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SEI", &a::SEI, &a::IMP, 2 },{ "ADC", &a::ADC, &a::ABY, 4 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "ADC", &a::ADC, &a::ABX, 4 },{ "ROR", &a::ROR, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "???", &a::NOP, &a::IMP, 2 },{ "STA", &a::STA, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZP0, 3 },{ "STA", &a::STA, &a::ZP0, 3 },{ "STX", &a::STX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "DEY", &a::DEY, &a::IMP, 2 },{ "???", &a::NOP, &a::IMP, 2 },{ "TXA", &a::TXA, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "STY", &a::STY, &a::ABS, 4 },{ "STA", &a::STA, &a::ABS, 4 },{ "STX", &a::STX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCC", &a::BCC, &a::REL, 2 },{ "STA", &a::STA, &a::IZY, 6 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "STY", &a::STY, &a::ZPX, 4 },{ "STA", &a::STA, &a::ZPX, 4 },{ "STX", &a::STX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "TYA", &a::TYA, &a::IMP, 2 },{ "STA", &a::STA, &a::ABY, 5 },{ "TXS", &a::TXS, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::NOP, &a::IMP, 5 },{ "STA", &a::STA, &a::ABX, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "???", &a::XXX, &a::IMP, 5 },
		{ "LDY", &a::LDY, &a::IMM, 2 },{ "LDA", &a::LDA, &a::IZX, 6 },{ "LDX", &a::LDX, &a::IMM, 2 },{ "???", &a::XXX, &a::IMP, 6 },{ "LDY", &a::LDY, &a::ZP0, 3 },{ "LDA", &a::LDA, &a::ZP0, 3 },{ "LDX", &a::LDX, &a::ZP0, 3 },{ "???", &a::XXX, &a::IMP, 3 },{ "TAY", &a::TAY, &a::IMP, 2 },{ "LDA", &a::LDA, &a::IMM, 2 },{ "TAX", &a::TAX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "LDY", &a::LDY, &a::ABS, 4 },{ "LDA", &a::LDA, &a::ABS, 4 },{ "LDX", &a::LDX, &a::ABS, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "BCS", &a::BCS, &a::REL, 2 },{ "LDA", &a::LDA, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 5 },{ "LDY", &a::LDY, &a::ZPX, 4 },{ "LDA", &a::LDA, &a::ZPX, 4 },{ "LDX", &a::LDX, &a::ZPY, 4 },{ "???", &a::XXX, &a::IMP, 4 },{ "CLV", &a::CLV, &a::IMP, 2 },{ "LDA", &a::LDA, &a::ABY, 4 },{ "TSX", &a::TSX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 4 },{ "LDY", &a::LDY, &a::ABX, 4 },{ "LDA", &a::LDA, &a::ABX, 4 },{ "LDX", &a::LDX, &a::ABY, 4 },{ "???", &a::XXX, &a::IMP, 4 },
		{ "CPY", &a::CPY, &a::IMM, 2 },{ "CMP", &a::CMP, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPY", &a::CPY, &a::ZP0, 3 },{ "CMP", &a::CMP, &a::ZP0, 3 },{ "DEC", &a::DEC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INY", &a::INY, &a::IMP, 2 },{ "CMP", &a::CMP, &a::IMM, 2 },{ "DEX", &a::DEX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 2 },{ "CPY", &a::CPY, &a::ABS, 4 },{ "CMP", &a::CMP, &a::ABS, 4 },{ "DEC", &a::DEC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BNE", &a::BNE, &a::REL, 2 },{ "CMP", &a::CMP, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ZPX, 4 },{ "DEC", &a::DEC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "CLD", &a::CLD, &a::IMP, 2 },{ "CMP", &a::CMP, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "CMP", &a::CMP, &a::ABX, 4 },{ "DEC", &a::DEC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
		{ "CPX", &a::CPX, &a::IMM, 2 },{ "SBC", &a::SBC, &a::IZX, 6 },{ "???", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "CPX", &a::CPX, &a::ZP0, 3 },{ "SBC", &a::SBC, &a::ZP0, 3 },{ "INC", &a::INC, &a::ZP0, 5 },{ "???", &a::XXX, &a::IMP, 5 },{ "INX", &a::INX, &a::IMP, 2 },{ "SBC", &a::SBC, &a::IMM, 2 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::SBC, &a::IMP, 2 },{ "CPX", &a::CPX, &a::ABS, 4 },{ "SBC", &a::SBC, &a::ABS, 4 },{ "INC", &a::INC, &a::ABS, 6 },{ "???", &a::XXX, &a::IMP, 6 },
		{ "BEQ", &a::BEQ, &a::REL, 2 },{ "SBC", &a::SBC, &a::IZY, 5 },{ "???", &a::XXX, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 8 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ZPX, 4 },{ "INC", &a::INC, &a::ZPX, 6 },{ "???", &a::XXX, &a::IMP, 6 },{ "SED", &a::SED, &a::IMP, 2 },{ "SBC", &a::SBC, &a::ABY, 4 },{ "NOP", &a::NOP, &a::IMP, 2 },{ "???", &a::XXX, &a::IMP, 7 },{ "???", &a::NOP, &a::IMP, 4 },{ "SBC", &a::SBC, &a::ABX, 4 },{ "INC", &a::INC, &a::ABX, 7 },{ "???", &a::XXX, &a::IMP, 7 },
	};
}

CPU6502::~CPU6502()
{
}

// Reads an 8-bit byte from the bus, located at the specified 16-bit address
uint8_t CPU6502::Read(uint16_t address)
{
	return m_bus->CpuRead(address, false);
}

// Writes a byte to the bus at the specified address
void CPU6502::Write(uint16_t address, uint8_t data)
{
	m_bus->CpuWrite(address, data);
}

void CPU6502::Reset()
{
	// Get address to set program counter to.
	m_addrAbs = 0xFFFC;
	uint16_t lo = Read(m_addrAbs + 0);
	uint16_t hi = Read(m_addrAbs + 1);

	// Set it
	pc = (hi << 8) | lo;

	// Reset CPU registers.
	accumulator = 0;
	xReg = 0;
	yReg = 0;
	stkptr = 0xFD;
	status = 0x00 | U;

	// Clear CPU helper vars.
	m_addrRel = 0x0000;
	m_addrAbs = 0x0000;
	m_fetched = 0x00;

	// Reset takes time.
	m_cycles = 8;
}


// if the CPU's IRQ input is 0 at the end of an instruction, then the CPU pushes the program counter and the processor status register, 
// sets the I flag to ignore further IRQs, and the Program Counter takes the value read at $fffe and $ffff.

// This behaviour is masked by the CPU's interrupt-disable status flag. The SEI instruction disables IRQs, and the CLI instruction enables them. 
void CPU6502::Irq()
{
	// If interrupts are allowed
	if (GetFlag(I) == 0)
	{
		// Push the program counter to the stack. It's 16-bits dont.
		Write(0x0100 + stkptr, (pc >> 8) & 0x00FF);
		stkptr--;
		Write(0x0100 + stkptr, pc & 0x00FF);
		stkptr--;

		// Then Push the status register to the stack.
		SetFlag(B, 0);
		SetFlag(U, 1);
		SetFlag(I, 1);
		Write(0x0100 + stkptr, status);
		stkptr--;

		// Read new program counter location from fixed address.
		m_addrAbs = 0xFFFE;
		uint16_t lo = Read(m_addrAbs + 0);
		uint16_t hi = Read(m_addrAbs + 1);
		pc = (hi << 8) | lo;

		m_cycles = 7;
	}
}

// The 2A03 and most other 6502 family CPUs are capable of processing a non - maskable interrupt(NMI).This calls a special NMI handler function after the current instruction ends.
//The check for this is edge - sensitive, meaning that if other circuitry on the board pulls the / NMI pin from high to low voltage, this sets a flip - flop in the CPU.
// When the CPU checks for interrupts and find that the flip - flop is set, it pushes the current state onto the stack and jumps to the NMI handler address at $FFFA - $FFFB.
//"Non-maskable" means that no state inside the CPU can prevent the NMI from being processed as an interrupt. 
// However, most boards that use a 6502 CPU's /NMI line allow the CPU to disable the generation of /NMI signals by writing to a memory-mapped I/O device. 
// In the case of the NES, the /NMI line is connected to the NES PPU and is used to detect vertical blanking. 
void CPU6502::Nmi()
{
	Write(0x0100 + stkptr, (pc >> 8) & 0x00FF);
	stkptr--;
	Write(0x0100 + stkptr, pc & 0x00FF);
	stkptr--;

	SetFlag(B, 0);
	SetFlag(U, 1);
	SetFlag(I, 1);
	Write(0x0100 + stkptr, status);
	stkptr--;

	m_addrAbs = 0xFFFA;
	uint16_t lo = Read(m_addrAbs + 0);
	uint16_t hi = Read(m_addrAbs + 1);
	pc = (hi << 8) | lo;

	m_cycles = 8;
}

// Perform one clock cycles worth of emulation/
void CPU6502::Clock()
{
	if (m_cycles == 0)
	{
		m_opcode = Read(pc);

		SetFlag(U, true);

		pc++;

		// Get Starting number of cycles.
		m_cycles = m_lookup[m_opcode].cycles;

		// Perform fetch using the appropriate addressing mode.
		uint8_t additional_cycle1 = (this->*m_lookup[m_opcode].addrmode)();

		// Perform operation.
		uint8_t additional_cycle2 = (this->*m_lookup[m_opcode].operate)();

		// The addressmode and opcode may need more cycles.
		m_cycles += (additional_cycle1 & additional_cycle2);

		SetFlag(U, true);
	}

	m_clockCount++;

	// Decrement cycles needed for instruction.
	m_cycles--;
}

#pragma region Flag Functions

// Get specific flag from status register.
uint8_t CPU6502::GetFlag(FLAGS6502 f)
{
	return ((status & f) > 0) ? 1 : 0;
}

// Sets flag in status register.
void CPU6502::SetFlag(FLAGS6502 f, bool v)
{
	if (v)
		status |= f;
	else
		status &= ~f;
}

#pragma endregion

#pragma region Addressing Modes

uint8_t CPU6502::IMP()
{
	m_fetched = accumulator;
	return 0;
}


// Immediate.
uint8_t CPU6502::IMM()
{
	m_addrAbs = pc++;
	return 0;
}



// Zero Page.
uint8_t CPU6502::ZP0()
{
	m_addrAbs = Read(pc);
	pc++;
	m_addrAbs &= 0x00FF;
	return 0;
}



// Zero Page with X Offset.
uint8_t CPU6502::ZPX()
{
	m_addrAbs = (Read(pc) + xReg);
	pc++;
	m_addrAbs &= 0x00FF;
	return 0;
}


// Zero Page with Y Offset.
uint8_t CPU6502::ZPY()
{
	m_addrAbs = (Read(pc) + yReg);
	pc++;
	m_addrAbs &= 0x00FF;
	return 0;
}


// Relative.
uint8_t CPU6502::REL()
{
	m_addrRel = Read(pc);
	pc++;
	if (m_addrRel & 0x80)
		m_addrRel |= 0xFF00;
	return 0;
}


// Absolute.
uint8_t CPU6502::ABS()
{
	uint16_t lo = Read(pc);
	pc++;
	uint16_t hi = Read(pc);
	pc++;

	m_addrAbs = (hi << 8) | lo;

	return 0;
}


// Absolute with X Offset.
uint8_t CPU6502::ABX()
{
	uint16_t lo = Read(pc);
	pc++;
	uint16_t hi = Read(pc);
	pc++;

	m_addrAbs = (hi << 8) | lo;
	m_addrAbs += xReg;

	if ((m_addrAbs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}


// Absolute with Y Offset.
uint8_t CPU6502::ABY()
{
	uint16_t lo = Read(pc);
	pc++;
	uint16_t hi = Read(pc);
	pc++;

	m_addrAbs = (hi << 8) | lo;
	m_addrAbs += yReg;

	if ((m_addrAbs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

// Indirect.
uint8_t CPU6502::IND()
{
	uint16_t ptr_lo = Read(pc);
	pc++;
	uint16_t ptr_hi = Read(pc);
	pc++;

	uint16_t ptr = (ptr_hi << 8) | ptr_lo;

	if (ptr_lo == 0x00FF) // Simulate page boundary hardware bug.
	{
		m_addrAbs = (Read(ptr & 0xFF00) << 8) | Read(ptr + 0);
	}
	else // Behave normally.
	{
		m_addrAbs = (Read(ptr + 1) << 8) | Read(ptr + 0);
	}

	return 0;
}


// Indirect X.
uint8_t CPU6502::IZX()
{
	uint16_t t = Read(pc);
	pc++;

	uint16_t lo = Read((uint16_t)(t + (uint16_t)xReg) & 0x00FF);
	uint16_t hi = Read((uint16_t)(t + (uint16_t)xReg + 1) & 0x00FF);

	m_addrAbs = (hi << 8) | lo;

	return 0;
}


// Indirect Y.
uint8_t CPU6502::IZY()
{
	uint16_t t = Read(pc);
	pc++;

	uint16_t lo = Read(t & 0x00FF);
	uint16_t hi = Read((t + 1) & 0x00FF);

	m_addrAbs = (hi << 8) | lo;
	m_addrAbs += yReg;

	if ((m_addrAbs & 0xFF00) != (hi << 8))
		return 1;
	else
		return 0;
}

uint8_t CPU6502::Fetch()
{
	if (!(m_lookup[m_opcode].addrmode == &CPU6502::IMP))
		m_fetched = Read(m_addrAbs);
	return m_fetched;
}

#pragma endregion

#pragma region Instructions

// Addition
// Thank you Javid.
// We can see how the above equation calculates V, based on A, M and R. V was chosen
// based on the following hypothesis:
//       Positive Number + Positive Number = Negative Result -> Overflow
//       Negative Number + Negative Number = Positive Result -> Overflow
//       Positive Number + Negative Number = Either Result -> Cannot Overflow
//       Positive Number + Positive Number = Positive Result -> OK! No Overflow
//       Negative Number + Negative Number = Negative Result -> OK! NO Overflow
uint8_t CPU6502::ADC()
{
	Fetch();

	m_temp = (uint16_t)accumulator + (uint16_t)m_fetched + (uint16_t)GetFlag(C);

	// The carry flag out exists in the high byte bit 0.
	SetFlag(C, m_temp > 255);

	// The Zero flag is set if the result is 0.
	SetFlag(Z, (m_temp & 0x00FF) == 0);

	// The signed Overflow flag is set based on previous code.
	SetFlag(V, (~((uint16_t)accumulator ^ (uint16_t)m_fetched) & ((uint16_t)accumulator ^ (uint16_t)m_temp)) & 0x0080);

	// The negative flag is set to the most significant bit of the result.
	SetFlag(N, m_temp & 0x80);

	// Load the result into the accumulator.
	accumulator = m_temp & 0x00FF;

	return 1;
}


// Subtraction
// Again thank you Javid.
// A = A - M - (1 - C)  ->  A = A + -1 * (M - (1 - C))  ->  A = A + (-M + 1 + C)
// To make a signed positive number negative, we can invert the bits and add 1.
//  5 = 00000101
// -5 = 11111010 + 00000001 = 11111011 (or 251 in our 0 to 255 range)
uint8_t CPU6502::SBC()
{
	Fetch();

	// We can invert the bottom 8 bits with bitwise xor.
	uint16_t value = ((uint16_t)m_fetched) ^ 0x00FF;

	// Notice this is exactly the same as addition.
	m_temp = (uint16_t)accumulator + value + (uint16_t)GetFlag(C);
	SetFlag(C, m_temp & 0xFF00);
	SetFlag(Z, ((m_temp & 0x00FF) == 0));
	SetFlag(V, (m_temp ^ (uint16_t)accumulator) & (m_temp ^ value) & 0x0080);
	SetFlag(N, m_temp & 0x0080);
	accumulator = m_temp & 0x00FF;
	return 1;
}

// Instruction: Bitwise Logic AND
// Function:    A = A & M
// Flags Out:   N, Z
uint8_t CPU6502::AND()
{
	Fetch();
	accumulator = accumulator & m_fetched;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 1;
}


// Instruction: Arithmetic Shift Left
// Function:    A = C <- (A << 1) <- 0
// Flags Out:   N, Z, C
uint8_t CPU6502::ASL()
{
	Fetch();
	m_temp = (uint16_t)m_fetched << 1;
	SetFlag(C, (m_temp & 0xFF00) > 0);
	SetFlag(Z, (m_temp & 0x00FF) == 0x00);
	SetFlag(N, m_temp & 0x80);
	if (m_lookup[m_opcode].addrmode == &CPU6502::IMP)
		accumulator = m_temp & 0x00FF;
	else
		Write(m_addrAbs, m_temp & 0x00FF);
	return 0;
}


// Instruction: Branch if Carry Clear
// Function:    if(C == 0) pc = address 
uint8_t CPU6502::BCC()
{
	if (GetFlag(C) == 0)
	{
		m_cycles++;
		m_addrAbs = pc + m_addrRel;

		if ((m_addrAbs & 0xFF00) != (pc & 0xFF00))
			m_cycles++;

		pc = m_addrAbs;
	}
	return 0;
}


// Instruction: Branch if Carry Set
// Function:    if(C == 1) pc = address
uint8_t CPU6502::BCS()
{
	if (GetFlag(C) == 1)
	{
		m_cycles++;
		m_addrAbs = pc + m_addrRel;

		if ((m_addrAbs & 0xFF00) != (pc & 0xFF00))
			m_cycles++;

		pc = m_addrAbs;
	}
	return 0;
}


// Instruction: Branch if Equal
// Function:    if(Z == 1) pc = address
uint8_t CPU6502::BEQ()
{
	if (GetFlag(Z) == 1)
	{
		m_cycles++;
		m_addrAbs = pc + m_addrRel;

		if ((m_addrAbs & 0xFF00) != (pc & 0xFF00))
			m_cycles++;

		pc = m_addrAbs;
	}
	return 0;
}

uint8_t CPU6502::BIT()
{
	Fetch();
	m_temp = accumulator & m_fetched;
	SetFlag(Z, (m_temp & 0x00FF) == 0x00);
	SetFlag(V, m_fetched & (1 << 6));
	SetFlag(N, m_fetched & (1 << 7));
	return 0;
}

// Instruction: Branch if Negative
// Function:    if(N == 1) pc = address
uint8_t CPU6502::BMI()
{
	if (GetFlag(N) == 1)
	{
		m_cycles++;
		m_addrAbs = pc + m_addrRel;

		if ((m_addrAbs & 0xFF00) != (pc & 0xFF00))
			m_cycles++;

		pc = m_addrAbs;
	}
	return 0;
}

// Instruction: Branch if Not Equal
// Function:    if(Z == 0) pc = address
uint8_t CPU6502::BNE()
{
	if (GetFlag(Z) == 0)
	{
		m_cycles++;
		m_addrAbs = pc + m_addrRel;

		if ((m_addrAbs & 0xFF00) != (pc & 0xFF00))
			m_cycles++;

		pc = m_addrAbs;
	}
	return 0;
}

// Instruction: Branch if Positive
// Function:    if(N == 0) pc = address
uint8_t CPU6502::BPL()
{
	if (GetFlag(N) == 0)
	{
		m_cycles++;
		m_addrAbs = pc + m_addrRel;

		if ((m_addrAbs & 0xFF00) != (pc & 0xFF00))
			m_cycles++;

		pc = m_addrAbs;
	}
	return 0;
}

// Instruction: Break
// Function:    Program Sourced Interrupt
uint8_t CPU6502::BRK()
{
	pc++;

	SetFlag(I, 1);
	Write(0x0100 + stkptr, (pc >> 8) & 0x00FF);
	stkptr--;
	Write(0x0100 + stkptr, pc & 0x00FF);
	stkptr--;

	SetFlag(B, 1);
	Write(0x0100 + stkptr, status);
	stkptr--;
	SetFlag(B, 0);

	pc = (uint16_t)Read(0xFFFE) | ((uint16_t)Read(0xFFFF) << 8);
	return 0;
}

// Instruction: Branch if Overflow Clear
// Function:    if(V == 0) pc = address
uint8_t CPU6502::BVC()
{
	if (GetFlag(V) == 0)
	{
		m_cycles++;
		m_addrAbs = pc + m_addrRel;

		if ((m_addrAbs & 0xFF00) != (pc & 0xFF00))
			m_cycles++;

		pc = m_addrAbs;
	}
	return 0;
}

// Instruction: Branch if Overflow Set
// Function:    if(V == 1) pc = address
uint8_t CPU6502::BVS()
{
	if (GetFlag(V) == 1)
	{
		m_cycles++;
		m_addrAbs = pc + m_addrRel;

		if ((m_addrAbs & 0xFF00) != (pc & 0xFF00))
			m_cycles++;

		pc = m_addrAbs;
	}
	return 0;
}

// Instruction: Clear Carry Flag
// Function:    C = 0
uint8_t CPU6502::CLC()
{
	SetFlag(C, false);
	return 0;
}


// Instruction: Clear Decimal Flag
// Function:    D = 0
uint8_t CPU6502::CLD()
{
	SetFlag(D, false);
	return 0;
}

// Instruction: Disable Interrupts / Clear Interrupt Flag
// Function:    I = 0
uint8_t CPU6502::CLI()
{
	SetFlag(I, false);
	return 0;
}

// Instruction: Clear Overflow Flag
// Function:    V = 0
uint8_t CPU6502::CLV()
{
	SetFlag(V, false);
	return 0;
}

// Instruction: Compare Accumulator
// Function:    C <- A >= M      Z <- (A - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU6502::CMP()
{
	Fetch();
	m_temp = (uint16_t)accumulator - (uint16_t)m_fetched;
	SetFlag(C, accumulator >= m_fetched);
	SetFlag(Z, (m_temp & 0x00FF) == 0x0000);
	SetFlag(N, m_temp & 0x0080);
	return 1;
}

// Instruction: Compare X Register
// Function:    C <- X >= M      Z <- (X - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU6502::CPX()
{
	Fetch();
	m_temp = (uint16_t)xReg - (uint16_t)m_fetched;
	SetFlag(C, xReg >= m_fetched);
	SetFlag(Z, (m_temp & 0x00FF) == 0x0000);
	SetFlag(N, m_temp & 0x0080);
	return 0;
}

// Instruction: Compare Y Register
// Function:    C <- Y >= M      Z <- (Y - M) == 0
// Flags Out:   N, C, Z
uint8_t CPU6502::CPY()
{
	Fetch();
	m_temp = (uint16_t)yReg - (uint16_t)m_fetched;
	SetFlag(C, yReg >= m_fetched);
	SetFlag(Z, (m_temp & 0x00FF) == 0x0000);
	SetFlag(N, m_temp & 0x0080);
	return 0;
}

// Instruction: Decrement Value at Memory Location
// Function:    M = M - 1
// Flags Out:   N, Z
uint8_t CPU6502::DEC()
{
	Fetch();
	m_temp = m_fetched - 1;
	Write(m_addrAbs, m_temp & 0x00FF);
	SetFlag(Z, (m_temp & 0x00FF) == 0x0000);
	SetFlag(N, m_temp & 0x0080);
	return 0;
}

// Instruction: Decrement X Register
// Function:    X = X - 1
// Flags Out:   N, Z
uint8_t CPU6502::DEX()
{
	xReg--;
	SetFlag(Z, xReg == 0x00);
	SetFlag(N, xReg & 0x80);
	return 0;
}

// Instruction: Decrement Y Register
// Function:    Y = Y - 1
// Flags Out:   N, Z
uint8_t CPU6502::DEY()
{
	yReg--;
	SetFlag(Z, yReg == 0x00);
	SetFlag(N, yReg & 0x80);
	return 0;
}

// Instruction: Bitwise Logic XOR
// Function:    A = A xor M
// Flags Out:   N, Z
uint8_t CPU6502::EOR()
{
	Fetch();
	accumulator = accumulator ^ m_fetched;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 1;
}

// Instruction: Increment Value at Memory Location
// Function:    M = M + 1
// Flags Out:   N, Z
uint8_t CPU6502::INC()
{
	Fetch();
	m_temp = m_fetched + 1;
	Write(m_addrAbs, m_temp & 0x00FF);
	SetFlag(Z, (m_temp & 0x00FF) == 0x0000);
	SetFlag(N, m_temp & 0x0080);
	return 0;
}

// Instruction: Increment X Register
// Function:    X = X + 1
// Flags Out:   N, Z
uint8_t CPU6502::INX()
{
	xReg++;
	SetFlag(Z, xReg == 0x00);
	SetFlag(N, xReg & 0x80);
	return 0;
}

// Instruction: Increment Y Register
// Function:    Y = Y + 1
// Flags Out:   N, Z
uint8_t CPU6502::INY()
{
	yReg++;
	SetFlag(Z, yReg == 0x00);
	SetFlag(N, yReg & 0x80);
	return 0;
}

// Instruction: Jump To Location
// Function:    pc = address
uint8_t CPU6502::JMP()
{
	pc = m_addrAbs;
	return 0;
}

// Instruction: Jump To Sub-Routine
// Function:    Push current pc to stack, pc = address
uint8_t CPU6502::JSR()
{
	pc--;

	Write(0x0100 + stkptr, (pc >> 8) & 0x00FF);
	stkptr--;
	Write(0x0100 + stkptr, pc & 0x00FF);
	stkptr--;

	pc = m_addrAbs;
	return 0;
}

// Instruction: Load The Accumulator
// Function:    A = M
// Flags Out:   N, Z
uint8_t CPU6502::LDA()
{
	Fetch();
	accumulator = m_fetched;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 1;
}

// Instruction: Load The X Register
// Function:    X = M
// Flags Out:   N, Z
uint8_t CPU6502::LDX()
{
	Fetch();
	xReg = m_fetched;
	SetFlag(Z, xReg == 0x00);
	SetFlag(N, xReg & 0x80);
	return 1;
}

// Instruction: Load The Y Register
// Function:    Y = M
// Flags Out:   N, Z
uint8_t CPU6502::LDY()
{
	Fetch();
	yReg = m_fetched;
	SetFlag(Z, yReg == 0x00);
	SetFlag(N, yReg & 0x80);
	return 1;
}

uint8_t CPU6502::LSR()
{
	Fetch();
	SetFlag(C, m_fetched & 0x0001);
	m_temp = m_fetched >> 1;
	SetFlag(Z, (m_temp & 0x00FF) == 0x0000);
	SetFlag(N, m_temp & 0x0080);
	if (m_lookup[m_opcode].addrmode == &CPU6502::IMP)
		accumulator = m_temp & 0x00FF;
	else
		Write(m_addrAbs, m_temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::NOP()
{
	switch (m_opcode) {
	case 0x1C:
	case 0x3C:
	case 0x5C:
	case 0x7C:
	case 0xDC:
	case 0xFC:
		return 1;
		break;
	}
	return 0;
}


// Instruction: Bitwise Logic OR
// Function:    A = A | M
// Flags Out:   N, Z
uint8_t CPU6502::ORA()
{
	Fetch();
	accumulator = accumulator | m_fetched;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 1;
}


// Instruction: Push Accumulator to Stack
// Function:    A -> stack
uint8_t CPU6502::PHA()
{
	Write(0x0100 + stkptr, accumulator);
	stkptr--;
	return 0;
}


// Instruction: Push Status Register to Stack
// Function:    status -> stack
// Note:        Break flag is set to 1 before push
uint8_t CPU6502::PHP()
{
	Write(0x0100 + stkptr, status | B | U);
	SetFlag(B, 0);
	SetFlag(U, 0);
	stkptr--;
	return 0;
}


// Instruction: Pop Accumulator off Stack
// Function:    A <- stack
// Flags Out:   N, Z
uint8_t CPU6502::PLA()
{
	stkptr++;
	accumulator = Read(0x0100 + stkptr);
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 0;
}


// Instruction: Pop Status Register off Stack
// Function:    Status <- stack
uint8_t CPU6502::PLP()
{
	stkptr++;
	status = Read(0x0100 + stkptr);
	SetFlag(U, 1);
	return 0;
}

uint8_t CPU6502::ROL()
{
	Fetch();
	m_temp = (uint16_t)(m_fetched << 1) | GetFlag(C);
	SetFlag(C, m_temp & 0xFF00);
	SetFlag(Z, (m_temp & 0x00FF) == 0x0000);
	SetFlag(N, m_temp & 0x0080);
	if (m_lookup[m_opcode].addrmode == &CPU6502::IMP)
		accumulator = m_temp & 0x00FF;
	else
		Write(m_addrAbs, m_temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::ROR()
{
	Fetch();
	m_temp = (uint16_t)(GetFlag(C) << 7) | (m_fetched >> 1);
	SetFlag(C, m_fetched & 0x01);
	SetFlag(Z, (m_temp & 0x00FF) == 0x00);
	SetFlag(N, m_temp & 0x0080);
	if (m_lookup[m_opcode].addrmode == &CPU6502::IMP)
		accumulator = m_temp & 0x00FF;
	else
		Write(m_addrAbs, m_temp & 0x00FF);
	return 0;
}

uint8_t CPU6502::RTI()
{
	stkptr++;
	status = Read(0x0100 + stkptr);
	status &= ~B;
	status &= ~U;

	stkptr++;
	pc = (uint16_t)Read(0x0100 + stkptr);
	stkptr++;
	pc |= (uint16_t)Read(0x0100 + stkptr) << 8;
	return 0;
}

uint8_t CPU6502::RTS()
{
	stkptr++;
	pc = (uint16_t)Read(0x0100 + stkptr);
	stkptr++;
	pc |= (uint16_t)Read(0x0100 + stkptr) << 8;

	pc++;
	return 0;
}

// Instruction: Set Carry Flag
// Function:    C = 1
uint8_t CPU6502::SEC()
{
	SetFlag(C, true);
	return 0;
}

// Instruction: Set Decimal Flag
// Function:    D = 1
uint8_t CPU6502::SED()
{
	SetFlag(D, true);
	return 0;
}


// Instruction: Set Interrupt Flag / Enable Interrupts
// Function:    I = 1
uint8_t CPU6502::SEI()
{
	SetFlag(I, true);
	return 0;
}

// Instruction: Store Accumulator at Address
// Function:    M = A
uint8_t CPU6502::STA()
{
	Write(m_addrAbs, accumulator);
	return 0;
}

// Instruction: Store X Register at Address
// Function:    M = X
uint8_t CPU6502::STX()
{
	Write(m_addrAbs, xReg);
	return 0;
}

// Instruction: Store Y Register at Address
// Function:    M = Y
uint8_t CPU6502::STY()
{
	Write(m_addrAbs, yReg);
	return 0;
}

// Instruction: Transfer Accumulator to X Register
// Function:    X = A
// Flags Out:   N, Z
uint8_t CPU6502::TAX()
{
	xReg = accumulator;
	SetFlag(Z, xReg == 0x00);
	SetFlag(N, xReg & 0x80);
	return 0;
}



// Instruction: Transfer Accumulator to Y Register
// Function:    Y = A
// Flags Out:   N, Z
uint8_t CPU6502::TAY()
{
	yReg = accumulator;
	SetFlag(Z, yReg == 0x00);
	SetFlag(N, yReg & 0x80);
	return 0;
}

// Instruction: Transfer Stack Pointer to X Register
// Function:    X = stack pointer
// Flags Out:   N, Z
uint8_t CPU6502::TSX()
{
	xReg = stkptr;
	SetFlag(Z, xReg == 0x00);
	SetFlag(N, xReg & 0x80);
	return 0;
}

// Instruction: Transfer X Register to Accumulator
// Function:    A = X
// Flags Out:   N, Z
uint8_t CPU6502::TXA()
{
	accumulator = xReg;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 0;
}

// Instruction: Transfer X Register to Stack Pointer
// Function:    stack pointer = X
uint8_t CPU6502::TXS()
{
	stkptr = xReg;
	return 0;
}

// Instruction: Transfer Y Register to Accumulator
// Function:    A = Y
// Flags Out:   N, Z
uint8_t CPU6502::TYA()
{
	accumulator = yReg;
	SetFlag(Z, accumulator == 0x00);
	SetFlag(N, accumulator & 0x80);
	return 0;
}

// This function captures illegal opcodes
uint8_t CPU6502::XXX()
{
	return 0;
}

#pragma endregion

#pragma region Helper Funcs

bool CPU6502::Complete()
{
	return m_cycles == 0;
}

// Convert binary instructions to human readable code.
std::map<uint16_t, std::string> CPU6502::Disassemble(uint16_t nStart, uint16_t nStop)
{
	uint32_t addr = nStart;
	uint8_t value = 0x00, lo = 0x00, hi = 0x00;
	std::map<uint16_t, std::string> mapLines;
	uint16_t line_addr = 0;

	// A convenient utility to convert variables into hex strings.
	auto hex = [](uint32_t n, uint8_t d)
		{
			std::string s(d, '0');
			for (int i = d - 1; i >= 0; i--, n >>= 4)
				s[i] = "0123456789ABCDEF"[n & 0xF];
			return s;
		};

	// As the instruction is decoded, a std::string is assembled with the readable output.
	while (addr <= (uint32_t)nStop)
	{
		line_addr = addr;

		std::string sInst = "$" + hex(addr, 4) + ": ";

		// Read instruction, and get its readable name
		uint8_t m_opcode = m_bus->CpuRead(addr, true); addr++;
		sInst += m_lookup[m_opcode].name + " ";

		if (m_lookup[m_opcode].addrmode == &CPU6502::IMP)
		{
			sInst += " {IMP}";
		}
		else if (m_lookup[m_opcode].addrmode == &CPU6502::IMM)
		{
			value = m_bus->CpuRead(addr, true); addr++;
			sInst += "#$" + hex(value, 2) + " {IMM}";
		}
		else if (m_lookup[m_opcode].addrmode == &CPU6502::ZP0)
		{
			lo = m_bus->CpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + " {ZP0}";
		}
		else if (m_lookup[m_opcode].addrmode == &CPU6502::ZPX)
		{
			lo = m_bus->CpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", X {ZPX}";
		}
		else if (m_lookup[m_opcode].addrmode == &CPU6502::ZPY)
		{
			lo = m_bus->CpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "$" + hex(lo, 2) + ", Y {ZPY}";
		}
		else if (m_lookup[m_opcode].addrmode == &CPU6502::IZX)
		{
			lo = m_bus->CpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + ", X) {IZX}";
		}
		else if (m_lookup[m_opcode].addrmode == &CPU6502::IZY)
		{
			lo = m_bus->CpuRead(addr, true); addr++;
			hi = 0x00;
			sInst += "($" + hex(lo, 2) + "), Y {IZY}";
		}
		else if (m_lookup[m_opcode].addrmode == &CPU6502::ABS)
		{
			lo = m_bus->CpuRead(addr, true); addr++;
			hi = m_bus->CpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + " {ABS}";
		}
		else if (m_lookup[m_opcode].addrmode == &CPU6502::ABX)
		{
			lo = m_bus->CpuRead(addr, true); addr++;
			hi = m_bus->CpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", X {ABX}";
		}
		else if (m_lookup[m_opcode].addrmode == &CPU6502::ABY)
		{
			lo = m_bus->CpuRead(addr, true); addr++;
			hi = m_bus->CpuRead(addr, true); addr++;
			sInst += "$" + hex((uint16_t)(hi << 8) | lo, 4) + ", Y {ABY}";
		}
		else if (m_lookup[m_opcode].addrmode == &CPU6502::IND)
		{
			lo = m_bus->CpuRead(addr, true); addr++;
			hi = m_bus->CpuRead(addr, true); addr++;
			sInst += "($" + hex((uint16_t)(hi << 8) | lo, 4) + ") {IND}";
		}
		else if (m_lookup[m_opcode].addrmode == &CPU6502::REL)
		{
			value = m_bus->CpuRead(addr, true); addr++;
			sInst += "$" + hex(value, 2) + " [$" + hex(addr + (int8_t)value, 4) + "] {REL}";
		}

		// Add the formed string to a std::map, using the instructions address as the key.
		mapLines[line_addr] = sInst;
	}

	return mapLines;
}

#pragma endregion