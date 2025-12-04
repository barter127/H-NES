#include "APU2A03.h"

// Lookup table used by the NES APU to convert length index to length counter value.
uint8_t APU2A03::lengthTable[] = {
    10, 254, 20, 2, 40, 4, 80, 6,
    160, 8, 60, 10, 14, 12, 26, 14,
     12, 16, 24, 18, 48, 20, 96, 22,
    192, 24, 72, 26, 16, 28, 32, 30
};

APU2A03::APU2A03()
{
}

APU2A03::~APU2A03()
{
}

void APU2A03::CpuWrite(uint16_t addr, uint8_t data)
{
    // CPU writes to $4000-$4017 to configure the APU.
    switch (addr)
    {
    case 0x4000: // Pulse 1 duty.
        switch ((data & 0xC0) >> 6)
        {
        case 0: pulse1Seq.sequence = 0b00000001; pulse1Osc.dutyCycle = 0.125; break;
        case 1: pulse1Seq.sequence = 0b00000011; pulse1Osc.dutyCycle = 0.25;  break;
        case 2: pulse1Seq.sequence = 0b00001111; pulse1Osc.dutyCycle = 0.5;   break;
        case 3: pulse1Seq.sequence = 0b11111100; pulse1Osc.dutyCycle = 0.75;  break;
        }

        pulse1Seq.sequence = pulse1Seq.newSequence;

        pulse1Halt = (data & 0x20);
        pulse1Env.disable = (data & 0x10);
        pulse1Env.volume = (data & 0x0F); 
        break;

    case 0x4001: // Pulse 1 sweep.
        pulse1Sweep.enabled = data & 0x80;
        pulse1Sweep.period = (data & 0x70) >> 4;
        pulse1Sweep.down = data & 0x08;
        pulse1Sweep.shift = data & 0x07;
        pulse1Sweep.reload = true;
        break;

    case 0x4002: // Pulse 1 timer low.
        pulse1Seq.reload = (pulse1Seq.reload & 0xFF00) | data;
        break;

    case 0x4003: // Pulse 1 timer high + length load.
        pulse1Seq.reload = ((data & 0x07) << 8) | (pulse1Seq.reload & 0x00FF);
        pulse1Seq.timer = pulse1Seq.reload;

        pulse1Seq.sequence = pulse1Seq.newSequence;
        pulse1LC.counter = lengthTable[(data >> 3) & 0x1F];

        pulse1Env.start = true;
        break;

    case 0x4004: // Pulse 2 duty.
        switch ((data & 0xC0) >> 6)
        {
        case 0: pulse2Seq.newSequence = 0b01000000; pulse2Osc.dutyCycle = 0.125; break;
        case 1: pulse2Seq.newSequence = 0b01100000; pulse2Osc.dutyCycle = 0.25;  break;
        case 2: pulse2Seq.newSequence = 0b01111000; pulse2Osc.dutyCycle = 0.5;   break;
        case 3: pulse2Seq.newSequence = 0b10011111; pulse2Osc.dutyCycle = 0.75;  break;
        }

        pulse2Seq.sequence = pulse2Seq.newSequence;

        pulse2Halt = (data & 0x20);
        pulse2Env.disable = (data & 0x10);
        pulse2Env.volume = (data & 0x0F);
        break;

    case 0x4005: // Pulse 2 sweep.
        pulse2Sweep.enabled = data & 0x80;
        pulse2Sweep.period = (data & 0x70) >> 4;
        pulse2Sweep.down = data & 0x08;
        pulse2Sweep.shift = data & 0x07;
        pulse2Sweep.reload = true;
        break;

    case 0x4006: // Pulse 2 timer low;
        pulse2Seq.reload = (pulse2Seq.reload & 0xFF00) | data;
        break;

    case 0x4007: // Pulse 1 timer high + length load.
        pulse2Seq.reload = ((data & 0x07) << 8) | (pulse2Seq.reload & 0x00FF);
        pulse2Seq.timer = pulse2Seq.reload;

        pulse2Seq.sequence = pulse2Seq.newSequence;
        pulse2LC.counter = lengthTable[(data >> 3) & 0x1F];

        pulse2Env.start = true;
        break;

    case 0x400C: // Noise envelope.
        noiseEnv.volume = (data & 0x0F);
        noiseEnv.disable = (data & 0x10);
        noiseHalt = (data & 0x20);
        break;

    case 0x400E: // Noise period lookup.
        switch (data & 0x0F)
        {
        case 0x0: noiseSeq.reload = 0;    break;
        case 0x1: noiseSeq.reload = 4;    break;
        case 0x2: noiseSeq.reload = 8;    break;
        case 0x3: noiseSeq.reload = 16;   break;
        case 0x4: noiseSeq.reload = 32;   break;
        case 0x5: noiseSeq.reload = 64;   break;
        case 0x6: noiseSeq.reload = 96;   break;
        case 0x7: noiseSeq.reload = 128;  break;
        case 0x8: noiseSeq.reload = 160;  break;
        case 0x9: noiseSeq.reload = 202;  break;
        case 0xA: noiseSeq.reload = 254;  break;
        case 0xB: noiseSeq.reload = 380;  break;
        case 0xC: noiseSeq.reload = 508;  break;
        case 0xD: noiseSeq.reload = 1016; break;
        case 0xE: noiseSeq.reload = 2034; break;
        case 0xF: noiseSeq.reload = 4068; break;
        }
        break;

    case 0x400F: // Noise length load.
        pulse1Env.start = true;
        pulse2Env.start = true;
        noiseEnv.start = true;

        noise_lc.counter = lengthTable[(data >> 3) & 0x1F];
        break;


    case 0x4015: // Pulse 1 enabled.
        pulse1Enable = (data & 0x01);
        break;
    }
}

uint8_t APU2A03::CpuRead(uint16_t addr, bool readOnly)
{
    return 0x00;
}

void APU2A03::Clock()
{
    bool quarterFrameClock = false;
    bool halfFrameClock = false;

    // Advance time used by oscillators.
    globalTime += (0.3333333333 / 1789773.0);

    // Frame sequencer runs at CPU/240.
    if (clockCounter % 6 == 0)
    {
        frameClockCounter++;

        // Quarter frame passed.
        if (frameClockCounter == 3729)
            quarterFrameClock = true;

        // Half frame passed.
        if (frameClockCounter == 7547)
        {
            quarterFrameClock = true;
            halfFrameClock = true;
        }

        // Third quarter passed.
        if (frameClockCounter == 11189)
        {
            quarterFrameClock = true;
            halfFrameClock = true;
            frameClockCounter = 0;
        }

        // Envelope units.
        if (quarterFrameClock)
        {
            pulse1Env.clock(pulse1Halt);
            pulse2Env.clock(pulse2Halt);
            noiseEnv.clock(noiseHalt);
        }

        // Sweep and length counters
        if (halfFrameClock)
        {
            pulse1LC.clock(pulse1Enable, pulse1Halt);
            pulse2LC.clock(pulse2Enable, pulse2Halt);
            noise_lc.clock(noiseEnable, noiseHalt);

            pulse1Sweep.clock(pulse1Seq.reload, 0);
            pulse2Sweep.clock(pulse2Seq.reload, 1);
        }

        // Pulse 1 sequencer shift
        pulse1Seq.Clock(pulse1Enable, [](uint32_t& s)
        {
            // Rotates bits for duty.
            s = ((s & 0x01) << 7) | ((s & 0xFE) >> 1);
        });

        pulse1Osc.frequency = 1789773.0 / (16.0 * (pulse1Seq.reload + 1));
        pulse1Osc.amplitude = (pulse1Env.output - 1) / 16.0;
        pulse1Sample = pulse1Osc.Sample(globalTime);

        if (pulse1LC.counter > 0 && pulse1Seq.timer >= 8 && !pulse1Sweep.mute && pulse1Env.output > 2)
            pulse1Output += (pulse1Sample - pulse1Output) * 0.5;
        else
            pulse1Output = 0;

        // Pulse 2 sequencer shift
        pulse2Seq.Clock(pulse2Enable, [](uint32_t& s)
        {
            // Rotates bits for duty.
            s = ((s & 0x01) << 7) | ((s & 0xFE) >> 1);
        });

        pulse2Osc.frequency = 1789773.0 / (16.0 * (pulse2Seq.reload + 1));
        pulse2Osc.amplitude = (pulse2Env.output - 1) / 16.0;
        pulse2Sample = pulse2Osc.Sample(globalTime);

        if (pulse2LC.counter > 0 && pulse2Seq.timer >= 8 && !pulse2Sweep.mute && pulse2Env.output > 2)
            pulse2Output += (pulse2Sample - pulse2Output) * 0.5;
        else
            pulse2Output = 0;

        // Noise LFSR
        noiseSeq.Clock(noiseEnable, [](uint32_t& s)
        {
            uint32_t feedback = (s & 1) ^ ((s >> 1) & 1);
            s = (feedback << 14) | (s >> 1);
        });

        if (noise_lc.counter > 0 && noiseSeq.timer >= 8)
            noiseOutput = noiseSeq.output * ((noiseEnv.output - 1) / 16.0);

        if (!pulse1Enable) pulse1Output = 0;
        if (!pulse2Enable) pulse2Output = 0;
        if (!noiseEnable) noiseOutput = 0;
    }

    // Sweep tracking
    pulse1Sweep.track(pulse1Seq.reload);
    pulse2Sweep.track(pulse2Seq.reload);

    pulse1Osc.frequency = 1789773.0 / (16.0 * (pulse1Seq.reload + 1));
    pulse1Sample = pulse1Osc.Sample(globalTime);

    clockCounter++;
}

void APU2A03::Reset()
{
    clockCounter = 0;
    frameClockCounter = 0;

    pulse1Enable = false;
    pulse1Halt = false;
    pulse1Sample = 0.0;
    pulse1Output = 0.0;

    pulse2Enable = false;
    pulse2Halt = false;
    pulse2Sample = 0.0;
    pulse2Output = 0.0;

    noiseEnable = false;
    noiseHalt = false;

    noiseSample = 0;
    noiseOutput = 0;

    globalTime = 0.0;
}

double APU2A03::GetOutputSample()
{
    // Only pulse1 for now.
    return pulse1Sample;
}