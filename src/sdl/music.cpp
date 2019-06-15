#include "music.h"
#include "logger.h"

#include <memory>

namespace sdl {

	Music::Music() {
		volume_ = 1;
	}

	Music::Music(const std::string& filename) {
		musicBuffer_ = std::make_shared<MusicBuffer>(filename);
		volume_ = 1;
	}

	void Music::play(int loops) {
		if (musicBuffer_ && musicBuffer_->valid_) {
			setVolume(volume_);
			Mix_PlayMusic(musicBuffer_->mixMusic_, loops);
		}
	}

	void Music::pause() {
		Mix_PausedMusic();
	}

	void Music::resume() {
		Mix_ResumeMusic();
	}

	bool Music::isPaused() const {
		return Mix_PausedMusic() == 1;
	}
	
	void Music::stopPlaying() {
		Mix_HaltMusic();
	}

	void Music::setVolume(float volume) {
		volume_ = volume;
		Mix_VolumeMusic((int) (volume_ * MIX_MAX_VOLUME));
	}

	float Music::getVolume() const {
		return volume_;
	}

	bool Music::isValid() const {
		return musicBuffer_ ? musicBuffer_->valid_ : false;
	}

	Music::MusicBuffer::MusicBuffer(const std::string& filename) : valid_(true) {
		mixMusic_ = Mix_LoadMUS(filename.c_str());
		if (mixMusic_ == nullptr) {
			valid_ = false;
			logger()->warn("[Music] {} failed to load!", filename);
		}
	}

	Music::MusicBuffer::~MusicBuffer() {
		if (mixMusic_ != 0) {
			Mix_FreeMusic(mixMusic_);
		}
	}

} // Namespace sdl.
