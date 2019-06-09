#ifndef CPPSDL2_SDL_SOUND_H
#define CPPSDL2_SDL_SOUND_H

#include "initmixer.h"

#include <string>
#include <map>
#include <memory>

namespace sdl {

	class Sound {
	public:
		// Creates a empty sound. Does nothing, but is safe to use as
		// a regular sound, but no sound will be heared.
		Sound();

		~Sound();

		bool operator==(const Sound& sound) const;

		bool operator!=(const Sound& sound) const;

		// Loads a soundfile and creates a sound.
		Sound(const std::string& filename);

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
		float getVolume() const;

		// Use with care! Return the pointer to the Mix_Chunk data.
		Mix_Chunk* getMixChunk() const;

		// Is sound in memory.
		bool isValid() const;

	private:
		struct SoundBuffer : public InitMixer {
			SoundBuffer(const std::string& filename);
			~SoundBuffer();

			Mix_Chunk* mixChunk_;
			bool valid_;
			std::string error_;

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

		float volume_;
		int channel_;
		int id_;
		std::shared_ptr<SoundBuffer> soundBuffer_;

		static int lastId_;
	};

	inline Mix_Chunk* Sound::getMixChunk() const {
		return soundBuffer_ ? soundBuffer_->mixChunk_ : nullptr;
	}

	inline bool Sound::operator==(const Sound& sound) const {
		return soundBuffer_ != nullptr && soundBuffer_ == sound.soundBuffer_;
	}

	inline bool Sound::operator!=(const Sound& other) const {
		return !(*this == other);
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_SOUND_H
