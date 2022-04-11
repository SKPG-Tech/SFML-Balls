#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <SFML/Audio.hpp>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <string>

namespace Game {
	class Music : public sf::SoundStream {
	public:
		void LoadMusic(std::string FileName) {
			sf::SoundBuffer SoundBuffer;
			if (!SoundBuffer.loadFromFile(FileName)) {
				std::cout << "[ERROR]: Failed loading media: " << FileName << "\n";

				return;
			}

			SampleRate = SoundBuffer.getSampleRate();
			SampleCount = SoundBuffer.getSampleCount();
			AudioChannels = SoundBuffer.getChannelCount();

			MusicSamples.assign(SoundBuffer.getSamples(), SoundBuffer.getSamples() + SoundBuffer.getSampleCount());

			initialize(SoundBuffer.getChannelCount(), SoundBuffer.getSampleRate());

			std::cout << "[Media Loaded]: " << FileName << "\n";
		}

		std::vector<float> ComputeBeatValues() {
			std::vector<float> BeatValues;

			float MaxValue = *std::max_element(MusicSamples.begin(), MusicSamples.end());
			float MinValue = *std::min_element(MusicSamples.begin(), MusicSamples.end());
			float AbsMax = std::max(std::abs(MinValue), MaxValue);

			for (sf::Uint64 i = 0; i < SampleCount - SampleRate; i += SampleRate) {
				for (sf::Uint64 j = i; j < i + SampleRate; j += static_cast<int>(SampleRate / 60)) {
					sf::Uint64 SampleSum = 0;

					for (sf::Uint64 k = j; k < j + static_cast<int>(SampleRate / 60); k += 1) {
						SampleSum += MusicSamples[k] * MusicSamples[k];
					}

					BeatValues.push_back(std::sqrtf(SampleSum / static_cast<int>(SampleRate / 60)) / AbsMax);
				}
			}

			return BeatValues;
		}

		float GetCurrentBeatValue(std::vector<float>& BeatValues, sf::Time CurrentTime) {
			if (static_cast<sf::Uint64>(CurrentTime.asSeconds() * 60 * getChannelCount()) <= BeatValues.size() - 60 * getChannelCount()) {
				return BeatValues.at(static_cast<sf::Uint64>(CurrentTime.asSeconds() * 60 * getChannelCount()));
			}
		}

	private:
		virtual bool onGetData(Chunk& ChunkData) {
			ChunkData.samples = &MusicSamples[CurrentMusicSample];

			if (CurrentMusicSample + SampleRate <= MusicSamples.size()) {
				ChunkData.sampleCount = SampleRate;
				CurrentMusicSample += SampleRate;
				return true;
			}
			else {
				ChunkData.sampleCount = MusicSamples.size() - CurrentMusicSample;
				CurrentMusicSample = MusicSamples.size();
				return false;
			}
		}

		virtual void onSeek(sf::Time TimeOffset) {
			CurrentMusicSample = static_cast<sf::Uint64>(TimeOffset.asSeconds() * getSampleRate() * getChannelCount());
		}

		std::vector<sf::Int16> MusicSamples;
		sf::Uint64 CurrentMusicSample = 0;
		sf::Uint64 SampleCount = 0;
		sf::Uint32 SampleRate = 0;
		sf::Uint32 AudioChannels = 0;
	};
}

#endif MUSIC_HPP