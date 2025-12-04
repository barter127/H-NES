#pragma once

#include <vector>
#include <string>
#include <map>

class Bus;

class CPU6502
{
public:
	CPU6502();
	~CPU6502();

public:
	uint8_t  accumulator = 0x00;		// Accumulator
	uint8_t  xReg = 0x00;		// X Register
	uint8_t  yReg = 0x00;		// Y Register
	uint8_t  stkptr = 0x00;		// Stack Pointer (points to location on bus)
	uint16_t pc = 0x0000;	// Program Counter
	uint8_t  status = 0x00;		// Status Register

	void Reset();	// Reset Interrupt
	void Irq();		// Interrupt Request
	void Nmi();		// Non-Maskable Interrupt Request
	void Clock();	// Perform one clock cycle's worth of update

	bool Complete();

	void ConnectBus(Bus* n) { m_bus = n; }

	// Returns a map of keys to instruction memory locations.
	std::map<uint16_t, std::string> Disassemble(uint16_t nStart, uint16_t nStop);

	enum FLAGS6502
	{
		C = (1 << 0),	// Carry Bit.
		Z = (1 << 1),	// Zero.
		I = (1 << 2),	// Disable Interrupts.
		D = (1 << 3),	// Decimal Mode (unused in mine).
		B = (1 << 4),	// Break.
		U = (1 << 5),	// Unused.
		V = (1 << 6),	// Overflow.
		N = (1 << 7),	// Negative.
	};

private:
	// Convenience functions to access status register
	uint8_t GetFlag(FLAGS6502 f);
	void    SetFlag(FLAGS6502 f, bool v);

	// Assisstive variables to facilitate emulation
	uint8_t  m_fetched = 0x00;
	uint16_t m_temp = 0x0000;
	uint16_t m_addrAbs = 0x0000;
	uint16_t m_addrRel = 0x00; 
	uint8_t  m_opcode = 0x00;  
	uint8_t  m_cycles = 0;	   
	uint32_t m_clockCount = 0;	

	Bus* m_bus = nullptr;
	uint8_t Read(uint16_t address);
	void    Write(uint16_t address, uint8_t data);

	uint8_t Fetch();

	struct INSTRUCTION
	{
		std::string name;
		uint8_t(CPU6502::* operate)(void) = nullptr; // FuncPtr to Opcode Function.
		uint8_t(CPU6502::* addrmode)(void) = nullptr; // FuncPtr To Addressing mode function.
		uint8_t     cycles = 0; // Cycles need to perform.
	};

	std::vector<INSTRUCTION> m_lookup;

private:
	// Addressing Modes.
	// Opcodes contain info about which addressing mode should be used.
	// Implement addressing mode before executing opcode.

	uint8_t IMP();	uint8_t IMM();
	uint8_t ZP0();	uint8_t ZPX();
	uint8_t ZPY();	uint8_t REL();
	uint8_t ABS();	uint8_t ABX();
	uint8_t ABY();	uint8_t IND();
	uint8_t IZX();	uint8_t IZY();

private:
	// Opcodes (CPU Logic).
	// Most functions return 0 but if a function needs more clock cycles it'll return 1.

	uint8_t ADC();	uint8_t AND();	uint8_t ASL();	uint8_t BCC();
	uint8_t BCS();	uint8_t BEQ();	uint8_t BIT();	uint8_t BMI();
	uint8_t BNE();	uint8_t BPL();	uint8_t BRK();	uint8_t BVC();
	uint8_t BVS();	uint8_t CLC();	uint8_t CLD();	uint8_t CLI();
	uint8_t CLV();	uint8_t CMP();	uint8_t CPX();	uint8_t CPY();
	uint8_t DEC();	uint8_t DEX();	uint8_t DEY();	uint8_t EOR();
	uint8_t INC();	uint8_t INX();	uint8_t INY();	uint8_t JMP();
	uint8_t JSR();	uint8_t LDA();	uint8_t LDX();	uint8_t LDY();
	uint8_t LSR();	uint8_t NOP();	uint8_t ORA();	uint8_t PHA();
	uint8_t PHP();	uint8_t PLA();	uint8_t PLP();	uint8_t ROL();
	uint8_t ROR();	uint8_t RTI();	uint8_t RTS();	uint8_t SBC();
	uint8_t SEC();	uint8_t SED();	uint8_t SEI();	uint8_t STA();
	uint8_t STX();	uint8_t STY();	uint8_t TAX();	uint8_t TAY();
	uint8_t TSX();	uint8_t TXA();	uint8_t TXS();	uint8_t TYA();

	// Equivalent to NOP (For unofficial Opcodes).
	uint8_t XXX();
};