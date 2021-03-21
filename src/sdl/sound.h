#ifndef CPPSDL2_SDL_SOUND_H
#define CPPSDL2_SDL_SOUND_H

#include <string>
#include <map>
#include <memory>

#include <SDL_mixer.h>

namespace sdl {

	class Sound {
	public:
		Sound();

		~Sound();

		friend bool operator==(const Sound& left, const Sound& right) noexcept;

		friend bool operator!=(const Sound& sound, const Sound& right) noexcept;

		// Loads a soundfile and creates a sound.
		explicit Sound(const std::string& filename);

		// Copie the sound. Is a fast copy. Will point to the 
		// same Mix_Chunk data.
		Sound(const Sound& sound);
		
		// Copies the sound. Is a fast copy. Will point to the 
		// same Mix_Chunk data.
		Sound& operator=(const Sound& sound);

		// Play the sound one time plus the "loops" variable.
		void play(int loops = 0);

		// Pauses the sound.
		void pause();
		
		// The sound is resumed playing, i.e. if the sound is paused.
		void resume();

		// Return true if the sound is paused else false.
		bool isPaused() const;

		// Stop the playing of the sound.
		void stopPlaying();

		// Set the volume. A value between [0,1].
		void setVolume(float volume);

		// Get the volume. A value between [0,1].
		float getVolume() const noexcept;

		// Use with care! Return the pointer to the Mix_Chunk data.
		Mix_Chunk* getMixChunk() const noexcept;

		// Is sound in memory.
		bool isValid() const noexcept;

	private:
		struct SoundBuffer {
			explicit SoundBuffer(const std::string& filename);
			~SoundBuffer();

			Mix_Chunk* mixChunk{};
			bool valid{true};
			std::string error;

			static std::map<int, int> channelList;// <channel, sound id>
		};

		// Returns the sounds channel. If no channel is own 0 is returned.
		int getChannel() const;

		bool isPlaying() const {
			return !(channel_ != -1 && Mix_Playing(channel_) != 1);
		}

		bool ownChannel() const {
			return (channel_ != -1 && SoundBuffer::channelList[channel_] == id_);
		}

		std::shared_ptr<SoundBuffer> soundBuffer_;

		float volume_{1.f};
		int channel_{-1};
		int id_{};

		static int lastId_;
	};

	inline Mix_Chunk* Sound::getMixChunk() const noexcept {
		return soundBuffer_ ? soundBuffer_->mixChunk : nullptr;
	}

	inline bool operator==(const Sound& left, const Sound& right) noexcept {
		return left.soundBuffer_ == right.soundBuffer_;
	}

	inline bool operator!=(const Sound& left, const Sound& right) noexcept {
		return left.soundBuffer_ != right.soundBuffer_;
	}

}

#endif
