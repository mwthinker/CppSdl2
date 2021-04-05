#include "music.h"

#include <spdlog/spdlog.h>

#include <memory>
#include <algorithm>

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
		volume_ = std::clamp(volume, 0.f, 1.f);
		Mix_VolumeMusic(static_cast<float>(volume_ * MIX_MAX_VOLUME));
	}

	float Music::getVolume() const noexcept {
		return volume_;
	}

	bool Music::isValid() const noexcept {
		return musicBuffer_ ? musicBuffer_->valid : false;
	}

	Music::MusicBuffer::MusicBuffer(const std::string& filename) {
		mixMusic = Mix_LoadMUS(filename.c_str());
		if (mixMusic == nullptr) {
			valid = false;
			spdlog::warn("[sdl::Music] {} failed to load {}: {}", filename, Mix_GetError());
		}
	}

	Music::MusicBuffer::~MusicBuffer() {
		if (mixMusic != nullptr) {
			Mix_FreeMusic(mixMusic);
		}
	}

}
