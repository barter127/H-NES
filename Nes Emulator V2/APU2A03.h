#pragma once

#include <cstdint>
#include <functional>

class APU2A03 // Still a bit off but dats alright.
{
public:
	APU2A03();
	~APU2A03();

	void CpuWrite(uint16_t addr, uint8_t data);
	uint8_t CpuRead(uint16_t addr, bool readOnly);
	void Reset();
	void Clock();

	double GetOutputSample();

private:
	static uint8_t lengthTable[];

	uint32_t clockCounter = 0;
	uint32_t frameClockCounter = 0;

	struct Sequencer
	{
		uint32_t sequence = 0x00000000;
		uint32_t newSequence = 0x00000000;
		uint16_t timer = 0x0000;
		uint16_t reload = 0x0000;
		uint8_t output = 0x00;

		uint8_t Clock(bool enable, std::function<void(uint32_t& s)> functionManip)
		{
			if (enable)
			{
				timer--;
				if (timer == 0xFFFF)
				{
					timer = reload + 1;
					functionManip(sequence);
					output = sequence & 0x00000001;
				}
			}

			return output;
		}
	};

	struct Oscpulse
	{
		double frequency = 0.0;
		double dutyCycle = 0.0;
		double amplitude = 1;
		double pi = 3.14159;
		double harmonics = 20;

		double Sample(double t)
		{
			double a = 0;
			double b = 0;
			double p = dutyCycle * 2.0 * pi;

			auto approxsin = [](double t)
				{
					double j = t * 0.15915;
					j = j - (int)j;
					return 20.785 * j * (j - 0.5) * (j - 1.0);
				};

			for (int n = 1; n < harmonics; n++)
			{
				double c = n + frequency * 2.0 * pi * t;
				a += -approxsin(c) / n;
				b += -approxsin(c - p * n) / n;
			}

			return (2.0 * amplitude / pi) * (a - b);
		}
	};

	struct LengthCounter
	{
		uint8_t counter = 0x00;
		uint8_t clock(bool bEnable, bool bHalt)
		{
			if (!bEnable)
				counter = 0;
			else
				if (counter > 0 && !bHalt)
					counter--;
			return counter;
		}
	};

	struct Envelope
	{
		void clock(bool bLoop)
		{
			if (!start)
			{
				if (decayCount == 0)
				{
					decayCount = volume;

					if (decayCount == 0)
					{
						if (bLoop)
						{
							decayCount = 15;
						}

					}
					else
						decayCount--;
				}
				else
					decayCount--;
			}
			else
			{
				start = false;
				decayCount = 15;
				decayCount = volume;
			}

			if (disable)
			{
				output = volume;
			}
			else
			{
				output = decayCount;
			}
		}

		bool start = false;
		bool disable = false;
		uint16_t dividerCount = 0;
		uint16_t volume = 0;
		uint16_t output = 0;
		uint16_t decayCount = 0;
	};


	struct Sweeper
	{
		bool enabled = false;
		bool down = false;
		bool reload = false;
		uint8_t shift = 0x00;
		uint8_t timer = 0x00;
		uint8_t period = 0x00;
		uint16_t change = 0;
		bool mute = false;

		void track(uint16_t& target)
		{
			if (enabled)
			{
				change = target >> shift;
				mute = (target < 8) || (target > 0x7FF);
			}
		}

		bool clock(uint16_t& target, bool channel)
		{
			bool changed = false;
			if (timer == 0 && enabled && shift > 0 && !mute)
			{
				if (target >= 8 && change < 0x07FF)
				{
					if (down)
					{
						target -= change - channel;
					}
					else
					{
						target += change;
					}
					changed = true;
				}
			}

			if (timer == 0 || reload)
			{
				timer = period;
				reload = false;
			}
			else
				timer--;

			mute = (target < 8) || (target > 0x7FF);

			return changed;
		}
	};

	bool pulse1Enable = false;
	bool pulse1Halt = false;
	double pulse1Sample = 0.0;
	double pulse1Output = 0.0;
	Sequencer pulse1Seq;
	Oscpulse pulse1Osc;
	Envelope pulse1Env;
	LengthCounter pulse1LC;
	Sweeper pulse1Sweep;

	bool pulse2Enable = false;
	bool pulse2Halt = false;
	double pulse2Sample = 0.0;
	double pulse2Output = 0.0;
	Sequencer pulse2Seq;
	Oscpulse pulse2Osc;
	Envelope pulse2Env;
	LengthCounter pulse2LC;
	Sweeper pulse2Sweep;

	bool noiseEnable = false;
	bool noiseHalt = false;
	Envelope noiseEnv;
	LengthCounter noise_lc;
	Sequencer noiseSeq;
	double noiseSample = 0;
	double noiseOutput = 0;

	double globalTime = 0.0;
};

