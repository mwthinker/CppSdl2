#ifndef CPPSDL2_SDL_BATCH_H
#define CPPSDL2_SDL_BATCH_H

#include "vertexbufferobject.h"
#include "logger.h"

#include <algorithm>
#include <vector>

namespace sdl {

	template <class Vertex>
	class Batch {
	public:
		Batch();

		Batch(GLenum mode, GLenum usage);

		~Batch() = default;

		Batch(const Batch&& other) noexcept;
		Batch& operator=(const Batch&& other) noexcept;

		Batch(const Batch& batch) = delete;
		Batch &operator=(const Batch& batch) = delete;

		GLenum getMode() const noexcept;
		GLenum getUsage() const noexcept;

		size_t getMaxVertexes() const noexcept;

		float getVboSizeInMiB() const noexcept;

		void bind();

		void clear();
		GLsizei getSize() const noexcept;

		GLsizei getIndexesSize() const noexcept;

		void uploadToGraphicCard();
		void draw() const;

		template<class InputIterator>
		void add(InputIterator begin, InputIterator end);

		void add(const std::vector<Vertex>& data);

		template<class ...Vertexes>
		void add(Vertexes&& ...vertexes);

		void startIndex();

		template<class InputIterator>
		void addIndexes(InputIterator begin, InputIterator end);

		void addIndexes(const std::vector<GLuint>& data);

		template<class ...Indexes>
		void addIndexes(Indexes&& ...vertexes);

	private:
		void assertIndexSizeIsValid() const;

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

		template<class IndexType>
		constexpr void IS_INDEX_TYPE() {
			static_assert(std::is_integral<IndexType>(),
				"IndexType must be a integral type ");
		}
		
		std::vector<Vertex> vertexes_;
		std::vector<GLint> indexes_;
		sdl::VertexBufferObject vbo_;
		sdl::VertexBufferObject vboIndexes_;
		
		size_t uploadedIndex_ = 0;
		GLenum usage_ = 0;
		GLenum mode_ = 0;
		GLsizei index_ = 0;
		GLuint currentIndexesIndex = 0;
	};

	template <class Vertex>
	Batch<Vertex>::Batch() {
		IS_VERTEX_STANDARD_LAYOUT<Vertex>();
	}

	template <class Vertex>
	Batch<Vertex>::Batch(GLenum mode, GLenum usage) :
		mode_(mode), usage_(usage) {

		IS_VERTEX_STANDARD_LAYOUT<Vertex>();
	}

	template <class Vertex>
	Batch<Vertex>::Batch(const Batch&& other) noexcept :
		uploadedIndex_(other.uploadedIndex_), usage_(other.usage_), mode_(other.mode_), index_(other.index_),
		vertexes_(std::move(other.vertexes_)), indexes_(std::move(other.indexes_)), vbo_(std::move(other.vbo_)), vboIndexes_(std::move(other.vboIndexes_)) {

		IS_VERTEX_STANDARD_LAYOUT<Vertex>();

		other.uploadedIndex_ = 0;
		other.usage_ = 0;
		other.mode_ = 0;
		other.index_ = 0;
		other.currentIndexesIndex = false;
	}
	
	template <class Vertex>
	Batch<Vertex>& Batch<Vertex>::operator=(const Batch&& other) noexcept {
		vertexes_ = std::move(other.vertexes_);
		indexes_ = std::move(other.indexes_);
		vbo_ = std::move(other.vbo_);
		vboIndexes_ = std::move(other.vboIndexes_);
		usage_ = other.usage_;
		mode_ = other.mode_;
		index_ = other.index_;
		currentIndexesIndex = other.currentIndexesIndex;

		other.uploadedIndex_ = uploadedIndex_;
		other.usage_ = 0;
		other.mode_ = 0;
		other.index_ = 0;
		other.currentIndexesIndex = false;
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
		return vertexes_.size();
	}

	template <class Vertex>
	float Batch<Vertex>::getVboSizeInMiB() const noexcept {
		return (sizeof(Vertex) * vertexes_.size() + sizeof(GLuint) * indexes_.size()) * 1.f / 1024 / 1024;
	}

	template <class Vertex>
	void Batch<Vertex>::bind() {
		if (!vbo_.isGenerated()) {
			vbo_.generate();
		}
		vbo_.bind(GL_ARRAY_BUFFER);
		
		if (!indexes_.empty()) {
			if (!vboIndexes_.isGenerated()) {
				vboIndexes_.generate();
			}
			vboIndexes_.bind(GL_ELEMENT_ARRAY_BUFFER);
		}
	}

	template <class Vertex>
	void Batch<Vertex>::clear() {
		index_ = 0;
		if (usage_ != GL_STATIC_DRAW) {
			// Can only clear data if the vbo is not updated. 
			uploadedIndex_ = 0;
			indexes_.clear();
			currentIndexesIndex = 0;
		} else {
			logger()->error("[Batch] VertexData failed to clear, vbo is static.");
		}
	}

	template <class Vertex>
	GLsizei Batch<Vertex>::getSize() const noexcept {
		return index_;
	}
	
	template <class Vertex>
	GLsizei Batch<Vertex>::getIndexesSize() const noexcept {
		return indexes_.size();
	}

	template <class Vertex>
	void Batch<Vertex>::uploadToGraphicCard() {
		if (vbo_.getSize() > index_) {
			if (usage_ != GL_STATIC_DRAW) {
				vbo_.bind(GL_ARRAY_BUFFER);
				vbo_.bufferSubData(0, index_ * sizeof(Vertex), vertexes_.data());
				uploadedIndex_ = index_;
				
				if (!indexes_.empty()) {
					vboIndexes_.bind(GL_ELEMENT_ARRAY_BUFFER);
					vboIndexes_.bufferSubData(0, indexes_.size() * sizeof(GLuint), indexes_.data());
				}
			}
		} else {
			if (usage_ == GL_STATIC_DRAW) {
				vbo_.bind(GL_ARRAY_BUFFER);
				vbo_.bufferData(index_ * sizeof(Vertex), vertexes_.data(), usage_);
				if (!indexes_.empty()) {
					vboIndexes_.bind(GL_ELEMENT_ARRAY_BUFFER);
					vboIndexes_.bufferData(indexes_.size() * sizeof(GLuint), indexes_.data(), usage_);
				}
			} else {
				vbo_.bind(GL_ARRAY_BUFFER);
				vbo_.bufferData(vertexes_.size() * sizeof(Vertex), vertexes_.data(), usage_);
				if (!indexes_.empty()) {
					vboIndexes_.bind(GL_ELEMENT_ARRAY_BUFFER);
					vboIndexes_.bufferData(indexes_.size() * sizeof(GLuint), indexes_.data(), usage_);
				}
			}
			uploadedIndex_ = index_;
		}
	}

	template <class Vertex>
	void Batch<Vertex>::draw() const {
		if (vbo_.getSize() > 0) {
			if (uploadedIndex_ > 0) { // Data is avaiable to be drawn.
				if (!indexes_.empty()) {
					glDrawElements(mode_, static_cast<GLuint>(indexes_.size()), GL_UNSIGNED_INT, nullptr);
				} else {
					glDrawArrays(mode_, 0, index_);
				}
				sdl::checkGlError();
			}
		} else {
			logger()->error("[Batch] VertexData failed to draw, no vbo binded, i.e. Batch::uploadToGraphicCard never called.");
		}
	}

	template <class Vertex>
	template<class InputIterator>
	void Batch<Vertex>::add(InputIterator begin, InputIterator end) {
		IS_RANDOM_ACCESS_ITERATOR<InputIterator>();
		
		const auto size = static_cast<int>(end - begin);
		assert(size >= 0);

		if (usage_ != GL_STATIC_DRAW) {
			if (size + index_ < static_cast<int>(vertexes_.size())) {
				std::copy(begin, end, vertexes_.begin() + index_);
			} else {
				vertexes_.insert(vertexes_.begin() + index_, begin, end);
			}
			index_ += size;
		} else {
			if (vbo_.getSize() == 0) {
				vertexes_.insert(vertexes_.end(), begin, end);
				index_ += size;
			} else {
				logger()->error("[Batch] VertexData is static, data can't be modified.");
			}
		}
	}

	template <class Vertex>
	void Batch<Vertex>::add(const std::vector<Vertex>& data) {
		add(data.begin(), data.end());
	}

	template <class Vertex>
	template<class ...Vertexes>
	void Batch<Vertex>::add(Vertexes&& ... pack) {
		std::array<Vertex, sizeof...(Vertexes)> vertexes = {{ pack ... }};
		add(vertexes.begin(), vertexes.end());
	}

	template <class Vertex>
	void Batch<Vertex>::startIndex() {
		currentIndexesIndex = static_cast<GLuint>(vertexes_.size());
	}

	template <class Vertex>
	template<class InputIterator>
	void Batch<Vertex>::addIndexes(InputIterator begin, InputIterator end) {
		IS_RANDOM_ACCESS_ITERATOR<InputIterator>();
		IS_INDEX_TYPE<InputIterator::value_type>();
		
		const auto size = static_cast<int>(end - begin);
		assert(size >= 0);

		if (vbo_.getSize() != 0 && usage_ == GL_STATIC_DRAW) {
			logger()->error("[Batch] VertexData is static, data index can't be modified.");
			return;
		}
		for (auto it = begin; it != end; ++it) {
			auto index = *it;
			indexes_.push_back(index + currentIndexesIndex);
		}
	}

	template <class Vertex>
	void Batch<Vertex>::addIndexes(const std::vector<GLuint>& indexes) {
		addIndexes(indexes.begin(), indexes.end());
	}

	template <class Vertex>
	template<class ...Indexes>
	void Batch<Vertex>::addIndexes(Indexes&& ...pack) {
		std::array<GLint, sizeof...(Indexes)> indexes = {{ pack ... }};
		addIndexes(indexes.begin(), indexes.end());
	}

	template <class Vertex>
	void Batch<Vertex>::assertIndexSizeIsValid() const {
		assert(mode_ != GL_TRIANGLES || (mode_ == GL_TRIANGLES && indexes_ % 3 == 0));
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_BATCH_H
