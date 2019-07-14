#ifndef CPPSDL2_SDL_BATCH_H
#define CPPSDL2_SDL_BATCH_H

#include "vertexbufferobject.h"
#include "vertexarrayobject.h"
#include "logger.h"

#include <algorithm>
#include <vector>

namespace sdl {

	template <class Vertex>
	class Batch {
	public:
		using iterator = typename std::vector<Vertex>::iterator;
		using const_iterator = typename std::vector<Vertex>::const_iterator;
		using reverse_iterator = typename std::vector<Vertex>::iterator;
		using const_reverse_iterator = typename std::vector<Vertex>::const_iterator;

		Batch();

		Batch(GLenum mode, GLenum usage, size_t maxVertexes = 1000);

		~Batch() = default;

		Batch(const Batch&& other) noexcept;
		Batch& operator=(const Batch&& other) noexcept;

		Batch(const Batch& batch) = delete;
		Batch &operator=(const Batch& batch) = delete;

		GLenum getMode() const noexcept;
		GLenum getUsage() const noexcept;

		size_t getMaxVertexes() const noexcept;

		float getVboSizeInMiB() const noexcept;
		void bindBuffer() const;

		void clear();
		GLsizei getSize() const noexcept;

		iterator begin() noexcept;
		iterator end() noexcept;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

		iterator rbegin() noexcept;
		iterator rend() noexcept;
		const_iterator rbegin() const noexcept;
		const_iterator rend() const noexcept;
		const_iterator crbegin() const noexcept;
		const_iterator crend() const noexcept;

		void uploadToGraphicCard();
		void draw() const;
		
		void add(const Vertex& vertex);

		template<class InputIterator>
		void add(InputIterator begin, InputIterator end);

		void add(const std::vector<Vertex>& data);

	private:
		template<class InputIterator>
		constexpr void IS_RANDOM_ACCESS_ITERATOR() {
			static_assert(std::is_same<std::random_access_iterator_tag,
				typename std::iterator_traits<InputIterator>::iterator_category>::value,
				"The function only accepts random access iterator.");
		}

		template<class VertexType>
		constexpr void IS_VERTEX_STANDARD_LAYOUT() {
			static_assert(std::is_standard_layout<VertexType>(),
				"Vertex type must be a type of standard layout.");
		}

		GLenum usage_;
		GLenum mode_;
		GLsizei index_;
		size_t uploadedIndex_;
		std::vector<Vertex> data_;		
		sdl::VertexBufferObject vbo_;
	};

	template <class Vertex>
	Batch<Vertex>::Batch() : mode_(0), usage_(0), index_(0),
		uploadedIndex_(0) {

		IS_VERTEX_STANDARD_LAYOUT<Vertex>();
	}

	template <class Vertex>
	Batch<Vertex>::Batch(GLenum mode, GLenum usage, size_t maxVertexes) :
		mode_(mode), usage_(usage), index_(0),
		uploadedIndex_(0), data_(usage == GL_STATIC_DRAW ? 0 : maxVertexes) {

		IS_VERTEX_STANDARD_LAYOUT<Vertex>();
	}

	template <class Vertex>
	Batch<Vertex>::Batch(const Batch&& other) noexcept : mode_(other.mode_), usage_(other.usage_),
		index_(other.index_), vbo_(std::move(other.vbo_)), data_(std::move(other.data_)) {

		IS_VERTEX_STANDARD_LAYOUT<Vertex>();

		other.mode_ = 0;
		other.usage_ = 0;
		other.index_ = 0;
		other.vbo_ = VertexBufferObject();
	}
	
	template <class Vertex>
	Batch<Vertex>& Batch<Vertex>::operator=(const Batch&& other) noexcept {
		mode_ = other.mode_;
		usage_ = other.usage_;
		index_ = other.index_;
		vbo_ = std::move(other.vbo_);
		data_ = std::move(other.data_);

		other.mode_ = 0;
		other.usage_ = 0;
		other.index_ = 0;
		other.vbo_ = VertexBufferObject();
		return *this;
	}

	template <class Vertex>
	GLenum Batch<Vertex>::getMode() const noexcept {
		return mode_;
	}

	template <class Vertex>
	GLenum Batch<Vertex>::getUsage() const noexcept {
		return usage_;
	}

	template <class Vertex>
	size_t Batch<Vertex>::getMaxVertexes() const noexcept {
		return data_.size();
	}

	template <class Vertex>
	float Batch<Vertex>::getVboSizeInMiB() const noexcept {
		return sizeof(Vertex) * data_.size() * 1.f / 1024 / 1024;
	}

	template <class Vertex>
	void Batch<Vertex>::bindBuffer() const {
		vbo_.bind();
	}

	template <class Vertex>
	void Batch<Vertex>::clear() {
		index_ = 0;
		if (usage_ != GL_STATIC_DRAW) {
			// Can only clear data if the vbo is not updated. 
			uploadedIndex_ = 0;
		} else {
			logger()->error("[Batch] VertexData failed to clear, vbo is static.");
		}
	}

	// Iterator
	template <class Vertex>
	GLsizei Batch<Vertex>::getSize() const noexcept {
		return index_;
	}

	template <class Vertex>
	typename Batch<Vertex>::iterator Batch<Vertex>::begin() noexcept {
		return data_.begin();
	}

	template <class Vertex>
	typename Batch<Vertex>::iterator Batch<Vertex>::end() noexcept {
		return data_.end();
	}

	template <class Vertex>
	typename Batch<Vertex>::const_iterator Batch<Vertex>::begin() const noexcept {
		return data_.begin();
	}

	template <class Vertex>
	typename Batch<Vertex>::const_iterator Batch<Vertex>::end() const noexcept {
		return data_.end();
	}

	template <class Vertex>
	typename Batch<Vertex>::const_iterator Batch<Vertex>::cbegin() const noexcept {
		return data_.cbegin();
	}

	template <class Vertex>
	typename Batch<Vertex>::const_iterator Batch<Vertex>::cend() const noexcept {
		return data_.cend();
	}

	// Reverse iterator
	template <class Vertex>
	typename Batch<Vertex>::iterator Batch<Vertex>::rbegin() noexcept {
		return data_.rbegin();
	}

	template <class Vertex>
	typename Batch<Vertex>::iterator Batch<Vertex>::rend() noexcept {
		return data_.rend();
	}

	template <class Vertex>
	typename Batch<Vertex>::const_iterator Batch<Vertex>::rbegin() const noexcept {
		return data_.rbegin();
	}

	template <class Vertex>
	typename Batch<Vertex>::const_iterator Batch<Vertex>::rend() const noexcept {
		return data_.rend();
	}

	template <class Vertex>
	typename Batch<Vertex>::const_iterator Batch<Vertex>::crbegin() const noexcept {
		return data_.crbegin();
	}

	template <class Vertex>
	typename Batch<Vertex>::const_iterator Batch<Vertex>::crend() const noexcept {
		return data_.crend();
	}

	template <class Vertex>
	void Batch<Vertex>::uploadToGraphicCard() {
		if (vbo_.getSize() > index_) {
			if (usage_ != GL_STATIC_DRAW) {
				vbo_.bindSubData(0, index_ * sizeof(Vertex), data_.data());
				uploadedIndex_ = index_;
			}
		} else {
			vbo_ = sdl::VertexBufferObject();
			if (usage_ == GL_STATIC_DRAW) {
				vbo_.bindData(GL_ARRAY_BUFFER, index_ * sizeof(Vertex), data_.data(), usage_);
			} else {
				vbo_.bindData(GL_ARRAY_BUFFER, data_.size() * sizeof(Vertex), data_.data(), usage_);
			}
			uploadedIndex_ = index_;
		}
	}

	template <class Vertex>
	void Batch<Vertex>::draw() const {
		if (vbo_.getSize() > 0) {
			if (uploadedIndex_ > 0) { // Data is avaiable to be drawn.
				glDrawArrays(mode_, 0, index_);
				sdl::checkGlError();
			}
		} else {
			logger()->error("[Batch] VertexData failed to draw, no vbo binded, i.e. Batch::uploadToGraphicCard never called.");
		}
	}

	template <class Vertex>
	void Batch<Vertex>::add(const Vertex& vertex) {
		if (usage_ != GL_STATIC_DRAW) {
			if (index_ < data_.size()) {
				data_[index_++] = vertex;
			} else {
				data_.push_back(vertex);
				++index_;
			}
		} else {
			if (vbo_.getSize() == 0) {
				data_.push_back(vertex);
				++index_;
			} else {
				logger()->error("[Batch] VertexData is static, data can't be modified.");
			}
		}
	}

	template <class Vertex>
	template<class InputIterator>
	void Batch<Vertex>::add(InputIterator begin, InputIterator end) {
		IS_RANDOM_ACCESS_ITERATOR<InputIterator>();

		if (usage_ != GL_STATIC_DRAW) {
			if (index_ + end - begin < data_.size()) {
				std::copy(begin, end, data_.begin() + index_);
			} else {
				data_.insert(data_.begin() + index_, begin, end);
			}
			index_ += end - begin;
		} else {
			if (vbo_.getSize() == 0) {
				// Only add data before the vbo is created.
				data_.insert(data_.end(), begin, end);
				index_ += end - begin;
			} else {
				logger()->error("[Batch] VertexData is static, data can't be modified.");
			}
		}
	}

	template <class Vertex>
	void Batch<Vertex>::add(const std::vector<Vertex>& data) {
		add(data.begin(), data.end());
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_BATCH_H
