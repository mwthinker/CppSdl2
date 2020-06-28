#ifndef CPPSDL2_SDL_MUSIC_H
#define CPPSDL2_SDL_MUSIC_H

#include <string>
#include <memory>

#include <SDL_mixer.h>

namespace sdl {

	// A music class.
	class Music {
	public:
		// Creates a empty music object. Does nothing, but is safe to use as
		// a regular music object, but no music will be heared.
		Music() = default;

		// Loads a musicfile and creates a music object.
		// Only one music file can be played.
		explicit Music(const std::string& filename);

		bool operator==(const Music& music) const;

		bool operator!=(const Music& other) const;

		// Play the music one time plus the "loops" variable.
		// Will stop any other music object already playing.
		// Set the volume defined by this object.
		void play(int loops = 0);

		// Pauses the music.
		// Will stop any other music object playing.
		void pause();

		// The music is resumed playing, i.e. if the music is paused.
		// Will resume any other music object playing.
		void resume();

		// Return true if a music object is playing.
		// Will resume any other music object playing.
		bool isPaused() const;

		// Stop the playing of the music.
		// Will stop any other music object playing.
		void stopPlaying();

		// Set the volume. A value between [0,1].
		// Will set the volume on any music currently playing.
		void setVolume(float volume);

		// Get the volume. A value between [0,1].
		// Return the volume of the music object, may however
		// not be the volume on other music currently playing.
		float getVolume() const;

		// Return true if the music points to a soundfile in memory.
		// Else false.
		bool isValid() const;

		// Use with care! Return the pointer to the Mix_Music data.
		Mix_Music* getMixMusic() const {
			return musicBuffer_ ? musicBuffer_->mixMusic : nullptr;
		}

	private:
		struct MusicBuffer {
			explicit MusicBuffer(const std::string& filename);
			~MusicBuffer();

			Mix_Music* mixMusic{};
			bool valid{true};
			std::string error;
		};
				
		float volume_{1.f};
		std::shared_ptr<MusicBuffer> musicBuffer_;
	};

	inline bool Music::operator==(const Music& music) const {
		return musicBuffer_ != nullptr && musicBuffer_ == music.musicBuffer_;
	}

	inline bool Music::operator!=(const Music& other) const {
		return !(*this == other);
	}

}

#endif
