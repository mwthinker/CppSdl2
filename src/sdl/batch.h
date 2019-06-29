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

		Batch() : mode_(0), usage_(0), index_(0),
			uploadedIndex_(0) {

			IS_VERTEX_POD<Vertex>();
		}

		Batch(GLenum mode, GLenum usage, size_t maxVertexes) :
			mode_(mode), usage_(usage), index_(0),
			uploadedIndex_(0), data_(usage == GL_STATIC_DRAW ? 0 : maxVertexes) {

			IS_VERTEX_POD<Vertex>();
		}

		Batch(GLenum mode) :
			mode_(mode), usage_(GL_STATIC_DRAW), index_(0),
			uploadedIndex_(0) {

			IS_VERTEX_POD<Vertex>();
		}

		~Batch() = default;

		Batch(const Batch&& other) noexcept {
			IS_VERTEX_POD<Vertex>();

			*this = std::move(other);
		}

		Batch& operator=(const Batch&& other) noexcept {
			mode_ = other.mode_;
			usage_ = other.usage_;
			index_ = other.index_;
			vbo_ = std::move(other.vbo_);
			vao_ = std::move(other.vao_);
			data_ = std::move(other.data_);

			other.mode_ = 0;
			other.usage_ = 0;
			other.index_ = 0;
			other.vbo_ = VertexBufferObject();
			other.vao_ = VertexArrayObject();
			return *this;
		}

		Batch(const Batch& batch) = delete;
		Batch &operator=(const Batch& batch) = delete;

		GLenum getMode() const noexcept {
			return mode_;
		}

		GLenum getUsage() const noexcept {
			return usage_;
		}

		size_t getMaxVertexes() const noexcept {
			return data_.size();
		}

		float getVboSizeInMiB() const noexcept {
			return sizeof(Vertex) * data_.size() * 1.f / 1024 / 1024;
		}

		void bindBuffer() const {
			vbo_.bind();
		}

		void clear() {
			index_ = 0;
			if (usage_ != GL_STATIC_DRAW) {
				// Can only clear data if the vbo is not updated. 
				uploadedIndex_ = 0;
			} else {
				logger()->error("[Batch] VertexData failed to clear, vbo is static.");
			}
		}

		GLsizei getSize() const noexcept {
			return index_;
		}

		iterator begin() noexcept {
			return data_.begin();
		}

		iterator end() noexcept {
			return data_.end();
		}

		const_iterator begin() const noexcept {
			return data_.begin();
		}

		const_iterator end() const noexcept {
			return data_.end();
		}

		void uploadToGraphicCard() {
			if (vbo_.getSize() > 0) {
				if (usage_ != GL_STATIC_DRAW) {
					vbo_.bindSubData(0, index_ * sizeof(Vertex), data_.data());
					uploadedIndex_ = index_;
				}
			} else {
				if (usage_ == GL_STATIC_DRAW) {
					vbo_.bindData(GL_ARRAY_BUFFER, index_ * sizeof(Vertex), data_.data(), usage_);
				} else {
					vbo_.bindData(GL_ARRAY_BUFFER, data_.size() * sizeof(Vertex), data_.data(), usage_);
				}
				uploadedIndex_ = index_;
			}
		}

		void draw() const {
			if (vbo_.getSize() > 0) {
				if (uploadedIndex_ > 0) { // Data is avaiable to be drawn.
					glDrawArrays(mode_, 0, index_);
					sdl::checkGlError();
				}
			} else {
				logger()->error("[Batch] VertexData failed to draw, no vbo binded, i.e. Batch::uploadToGraphicCard never called.");
			}
		}

		void add(const Vertex& vertex) {
			if (usage_ != GL_STATIC_DRAW) {
				data_[index_++] = vertex;
			} else {
				if (vbo_.getSize() == 0) {
					data_.push_back(vertex);
					++index_;
				} else {
					logger()->error("[Batch] VertexData is static, data can't be modified.");
				}
			}
		}

		template<class InputIterator>
		void add(InputIterator begin, InputIterator end) {
			IS_RANDOM_ACCESS_ITERATOR<InputIterator>();

			if (usage_ != GL_STATIC_DRAW) {
				std::copy(begin, end, data_.begin() + index_);
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

		void add(const std::vector<Vertex>& data) {
			add(data.begin(), data.end());
		}

	private:
		template<class InputIterator>
		constexpr void IS_RANDOM_ACCESS_ITERATOR() {
			static_assert(std::is_same<std::random_access_iterator_tag,
				typename std::iterator_traits<InputIterator>::iterator_category>::value,
				"The function only accepts random access iterator.");
		}

		template<class Vertex>
		constexpr void IS_VERTEX_POD() {
			static_assert(std::is_pod<Vertex>(),
				"Vertex type must be a POD type.");
		}

		const GLenum usage_;
		const GLenum mode_;
		GLsizei index_;
		size_t uploadedIndex_;
		std::vector<Vertex> data_;		
		sdl::VertexBufferObject vbo_;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_BATCH_H
