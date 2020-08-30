#include "music.h"

#include <spdlog/spdlog.h>

#include <memory>

namespace sdl {

	Music::Music(const std::string& filename)
		: musicBuffer_{std::make_shared<MusicBuffer>(filename)} {
	}

	void Music::play(int loops) {
		if (musicBuffer_ && musicBuffer_->valid) {
			setVolume(volume_);
			Mix_PlayMusic(musicBuffer_->mixMusic, loops);
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
		return musicBuffer_ ? musicBuffer_->valid : false;
	}

	Music::MusicBuffer::MusicBuffer(const std::string& filename) {
		mixMusic = Mix_LoadMUS(filename.c_str());
		if (mixMusic == nullptr) {
			valid = false;
			spdlog::warn("[sdl::Music] {} failed to load!", filename);
		}
	}

	Music::MusicBuffer::~MusicBuffer() {
		if (mixMusic != nullptr) {
			Mix_FreeMusic(mixMusic);
		}
	}

}
