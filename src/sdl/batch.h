#ifndef CPPSDL2_SDL_BATCH_H
#define CPPSDL2_SDL_BATCH_H

#include "vertexbufferobject.h"
#include "vertexarrayobject.h"
#include "window.h"
#include "logger.h"

#include <algorithm>
#include <vector>

namespace sdl {

	template <class Shader>
	class Batch : public VertexArrayObject {
	public:
		using iterator = typename std::vector<typename Shader::Vertex>::iterator;
		using const_iterator = typename std::vector<typename Shader::Vertex>::const_iterator;

		Batch() : mode_(0), usage_(0), shader_(nullptr), index_(0),
			uploadedIndex_(0) {
		}

		Batch(GLenum mode, GLenum usage, const std::shared_ptr<Shader>& shader, unsigned int maxVertexes) :
			mode_(mode), usage_(usage), index_(0),
			uploadedIndex_(0), shader_(shader), data_(usage == GL_STATIC_DRAW ? 0 : maxVertexes) {
		}

		Batch(GLenum mode, const std::shared_ptr<Shader>& shader) :
			mode_(mode), usage_(GL_STATIC_DRAW), index_(0),
			uploadedIndex_(0), shader_(shader) {
		}

		virtual ~Batch() = default;

		Batch(const Batch&& other) {
			*this = std::move(other);
		}

		Batch& operator=(const Batch&& other) {
			mode_ = other.mode_;
			usage_ = other.usage_;
			shader_ = other.shader_;
			index_ = other.index_;
			vbo_ = other.vbo_;
			data_ = std::move(other.data_);

			other.mode_ = 0;
			other.usage_ = 0;
			other.shader_ = nullptr;
			other.index_ = 0;
			other.vbo_ = VertexBufferObject();
			return *this;
		}

		Batch(const Batch& batch) = delete;
		Batch &operator=(const Batch& batch) = delete;

		GLenum getMode() const {
			return mode_;
		}

		GLenum getUsage() const {
			return usage_;
		}

		int getMaxVertexes() const {
			return data_.size();
		}

		float getVboSizeInMiB() const {
			return sizeof(typename Shader::Vertex) * data_.size() * 1.f / 1024 / 1024;
		}

		void useProgram() const override {
			shader_->useProgram();
		}

		void setVertexAttribPointer() const override {
			shader_->setVertexAttribPointer();
		}

		void bindBuffer() const override {
			vbo_.bindBuffer();
		}

		void clear() {
			index_ = 0;
			if (usage_ != GL_STATIC_DRAW) {
				// Can only clear data if the vbo is not updated. 
				uploadedIndex_ = 0;
			}
		}

		unsigned int getSize() const {
			return index_;
		}

		iterator begin() {
			return data_.begin();
		}

		iterator end() {
			return data_.end();
		}

		const_iterator begin() const {
			return data_.begin();
		}

		const_iterator end() const {
			return data_.end();
		}

		void uploadToGraphicCard() {
			if (Window::getOpenGlMajorVersion() >= 2) {
				if (vbo_.getSize() > 0) {
					if (usage_ != GL_STATIC_DRAW) {
						vbo_.bindBufferSubData(0, index_ * sizeof(typename Shader::Vertex), data_.data());
						uploadedIndex_ = index_;
					}
				} else {
					if (usage_ == GL_STATIC_DRAW) {
						vbo_.bindBufferData(GL_ARRAY_BUFFER, index_ * sizeof(typename Shader::Vertex), data_.data(), usage_);
					} else {
						vbo_.bindBufferData(GL_ARRAY_BUFFER, data_.size() * sizeof(typename Shader::Vertex), data_.data(), usage_);
					}
					uploadedIndex_ = index_;
				}
			}
		}

		virtual void draw() const {
			if (vbo_.getSize() > 0) {
				if (uploadedIndex_ > 0) { // Data is avaiable to be drawn.
					useProgram();
					bind();
					glDrawArrays(mode_, 0, index_);
					sdl::checkGlError();
				}
			} else {
				logger()->error("[Batch] VertexData failed to draw, no vbo binded, i.e. Batch::uploadToGraphicCard never called.");
			}
		}

		void add(const typename Shader::Vertex& v) {
			if (usage_ != GL_STATIC_DRAW) {
				data_[index_++] = v;
			} else {
				if (vbo_.getSize() == 0) {
					data_.push_back(v);
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

		void add(const std::vector<typename Shader::Vertex>& data) {
			add(data.begin(), data.end());
		}

	private:
		template<class InputIterator>
		constexpr void IS_RANDOM_ACCESS_ITERATOR() {
			static_assert(std::is_same<std::random_access_iterator_tag,
				typename std::iterator_traits<InputIterator>::iterator_category>::value,
				"The function only accepts random access iterator.\n");
		}

		const GLenum usage_;
		const GLenum mode_;
		unsigned int index_;
		unsigned int uploadedIndex_;
		std::vector<typename Shader::Vertex> data_;
		std::shared_ptr<Shader> shader_;
		sdl::VertexBufferObject vbo_;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_BATCH_H
