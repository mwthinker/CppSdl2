#ifndef CPPSDL2_SDL_BATCH_H
#define CPPSDL2_SDL_BATCH_H

#include "vertexbufferobject.h"
#include "logger.h"

#include <algorithm>
#include <vector>

namespace sdl {

	template<class InputIterator>
	constexpr void IS_RANDOM_ACCESS_ITERATOR() {
		static_assert(std::is_same<std::random_access_iterator_tag,
			typename std::iterator_traits<InputIterator>::iterator_category>::value,
			"The function only accepts random access iterator");
	}

	template<class VertexType>
	constexpr void IS_VERTEX_STANDARD_LAYOUT() {
		static_assert(std::is_standard_layout<VertexType>(),
			"Vertex type must be a type of standard layout");
	}

	template<class IndexType>
	constexpr void IS_INDEX_TYPE() {
		static_assert(std::is_integral<IndexType>(),
			"IndexType must be a integral type ");
	}

	template<class Vertex>
	class Batch;

	template <class Vertex>
	class BatchView {
	public:
		friend class Batch<Vertex>;

		BatchView() noexcept {
			IS_VERTEX_STANDARD_LAYOUT<Vertex>();
		}	

		BatchView(const BatchView&) noexcept = default;
		BatchView& operator=(const BatchView&) noexcept = default;
		BatchView(BatchView&&) noexcept = default;
		BatchView& operator=(BatchView&&) noexcept = default;

	private:
		BatchView(GLenum mode, GLsizei index, GLsizei size) noexcept :
			mode_(mode), index_(index), size_(size) {

			assert(index_ >= 0 && size_ >= 0);
			IS_VERTEX_STANDARD_LAYOUT<Vertex>();
		}

		GLenum mode_ = 0;
		GLsizei index_ = 0;
		GLsizei size_ = 0;
	};

	template <class Vertex>
	class BatchIndex {
	public:

		BatchIndex() noexcept = default;
		BatchIndex(const BatchIndex&) noexcept = default;
		BatchIndex& operator=(const BatchIndex&) noexcept = default;
		BatchIndex(BatchIndex&&) noexcept = default;
		BatchIndex& operator=(BatchIndex&&) noexcept = default;

	private:
		friend class Batch<Vertex>;
		
		BatchIndex(GLsizei index) noexcept
			: index_(index) {
		}

		GLsizei index_ = 0;
	};

	template <class Vertex>
	class Batch {
	public:
		Batch(GLenum usage);

		~Batch() = default;

		Batch(Batch&& other) noexcept;
		Batch& operator=(Batch&& other) noexcept;

		Batch(const Batch& batch) = delete;
		Batch &operator=(const Batch& batch) = delete;

		GLenum getUsage() const noexcept;

		size_t getMaxVertexes() const noexcept;

		float getVboSizeInMiB() const noexcept;

		void bind();

		void clear();
		GLsizei getSize() const noexcept;

		GLsizei getIndexesSize() const noexcept;

		void uploadToGraphicCard();
		void draw(GLenum mode) const;
		void draw(const BatchView<Vertex>& batchView) const;

		template<class InputIterator>
		void insert(InputIterator begin, InputIterator end);

		void add(const std::vector<Vertex>& data);

		template<class ...Vertexes>
		void add(Vertexes&& ...vertexes);

		void startAdding() noexcept;

		BatchIndex<Vertex> getCurrentBatchIndex() const noexcept;
		
		static BatchView<Vertex> getBatchView(GLenum mode, BatchIndex<Vertex> start, BatchIndex<Vertex> end) noexcept;
		BatchView<Vertex> getBatchView(GLenum mode) const noexcept;

		template<class InputIterator>
		void insertIndexes(InputIterator begin, InputIterator end);

		void addIndexes(const std::vector<GLuint>& data);

		template<class ...Indexes>
		void addIndexes(Indexes&& ...vertexes);

	private:
		bool isValidBatchView(const BatchView<Vertex>& batchView) const;

		bool isEveryIndexSizeValid() const;

		void assertIndexSizeIsValid(GLenum mode) const;
		
		std::vector<Vertex> vertexes_;
		std::vector<GLint> indexes_;
		sdl::VertexBufferObject vbo_;
		sdl::VertexBufferObject vboIndexes_;
		
		size_t uploadedIndex_ = 0;
		GLenum usage_ = 0;
		GLsizei index_ = 0;
		GLuint currentIndexesIndex = 0;
	};

	template <class Vertex>
	Batch<Vertex>::Batch(GLenum usage) : usage_(usage) {
		IS_VERTEX_STANDARD_LAYOUT<Vertex>();
	}

	template <class Vertex>
	Batch<Vertex>::Batch(Batch&& other) noexcept :
		uploadedIndex_(other.uploadedIndex_), usage_(other.usage_), index_(other.index_),
		vertexes_(std::move(other.vertexes_)), indexes_(std::move(other.indexes_)), vbo_(std::move(other.vbo_)), vboIndexes_(std::move(other.vboIndexes_)) {

		IS_VERTEX_STANDARD_LAYOUT<Vertex>();

		other.uploadedIndex_ = 0;
		other.usage_ = 0;
		other.index_ = 0;
		other.currentIndexesIndex = false;
	}
	
	template <class Vertex>
	Batch<Vertex>& Batch<Vertex>::operator=(Batch&& other) noexcept {
		vertexes_ = std::move(other.vertexes_);
		indexes_ = std::move(other.indexes_);
		vbo_ = std::move(other.vbo_);
		vboIndexes_ = std::move(other.vboIndexes_);
		usage_ = other.usage_;
		index_ = other.index_;
		currentIndexesIndex = other.currentIndexesIndex;

		other.uploadedIndex_ = uploadedIndex_;
		other.usage_ = 0;
		other.index_ = 0;
		other.currentIndexesIndex = false;
		return *this;
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
			logger()->error("[Batch] VertexData failed to clear, vbo is static");
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
	void Batch<Vertex>::draw(GLenum mode) const {
		if (indexes_.empty()) {
			draw({mode, 0, static_cast<GLsizei>(index_)});
		} else {
			draw({mode, 0, static_cast<GLsizei>(indexes_.size())});
		}
	}

	template <class Vertex>
	bool Batch<Vertex>::isValidBatchView(const BatchView<Vertex>& batchView) const {
		if (indexes_.empty()) {
			return batchView.index_ >= 0 && batchView.index_ + static_cast<GLsizei>(batchView.size_) <= vertexes_.size();
		} else {
			return batchView.index_ >= 0 && batchView.index_ + static_cast<GLsizei>(batchView.size_) <= indexes_.size();
		}
	}

	template <class Vertex>
	void Batch<Vertex>::draw(const BatchView<Vertex>& batchView) const {
		if (vbo_.getSize() > 0) {
			if (uploadedIndex_ <= 0) {
				logger()->warn("[Batch] Failed to draw, no data uploaded to call to uploadToGraphicCard()");
				return;
			}

			if (!isValidBatchView(batchView)) {
				logger()->warn("[Batch] BatchView not valid. start = {}, size = {}", batchView.index_, batchView.size_);
				return;
			}

			if (indexes_.empty()) {
				glDrawArrays(batchView.mode_, batchView.index_, batchView.size_);
			} else {
				assertIndexSizeIsValid(batchView.mode_);
				glDrawElements(batchView.mode_, batchView.size_, GL_UNSIGNED_INT, reinterpret_cast<void*>(batchView.index_ * sizeof(GLint)));
			}
			assertGlError();
		} else if (!vbo_.isGenerated()) {
			logger()->error("[Batch] VertexData failed to draw, no vbo binded, i.e. Batch::uploadToGraphicCard never called");
		}
	}
	

	template <class Vertex>
	template<class InputIterator>
	void Batch<Vertex>::insert(InputIterator begin, InputIterator end) {
		IS_RANDOM_ACCESS_ITERATOR<InputIterator>();
		
		const auto size = static_cast<int>(end - begin);
		assert(size >= 0);

		if (usage_ != GL_STATIC_DRAW) {
			if (size + index_ <= static_cast<int>(vertexes_.size())) {
				std::copy(begin, end, vertexes_.begin() + index_);
				index_ += 3;
			} else {
				vertexes_.insert(vertexes_.begin() + index_, begin, end);
				index_ = static_cast<int>(vertexes_.size());
			}
		} else {
			if (vbo_.getSize() == 0) {
				vertexes_.insert(vertexes_.end(), begin, end);
				index_ += size;
			} else {
				logger()->error("[Batch] VertexData is static, data can't be modified");
			}
		}
	}

	template <class Vertex>
	void Batch<Vertex>::add(const std::vector<Vertex>& data) {
		insert(data.begin(), data.end());
	}

	template <class Vertex>
	template<class ...Vertexes>
	void Batch<Vertex>::add(Vertexes&& ... pack) {
		std::array<Vertex, sizeof...(Vertexes)> vertexes = {{ pack ... }};
		insert(vertexes.begin(), vertexes.end());
	}

	template <class Vertex>
	void Batch<Vertex>::startAdding() noexcept {
		currentIndexesIndex = static_cast<GLuint>(index_);
	}

	template <class Vertex>
	BatchIndex<Vertex> Batch<Vertex>::getCurrentBatchIndex() const noexcept {
		if (indexes_.empty()) {
			return {index_};
		} else {
			return {static_cast<GLsizei>(indexes_.size())};
		}
	}

	template <class Vertex>
	BatchView<Vertex> Batch<Vertex>::getBatchView(GLenum mode, BatchIndex<Vertex> start, BatchIndex<Vertex> end) noexcept {
		return {mode, start.index_, end.index_ - start.index_};
	}

	template <class Vertex>
	BatchView<Vertex> Batch<Vertex>::getBatchView(GLenum mode) const noexcept {
		return getBatchView(mode, BatchIndex<Vertex>(), getCurrentBatchIndex());
	}

	template <class Vertex>
	template<class InputIterator>
	void Batch<Vertex>::insertIndexes(InputIterator begin, InputIterator end) {
		IS_RANDOM_ACCESS_ITERATOR<InputIterator>();
		IS_INDEX_TYPE<typename std::decay<decltype(*begin)>::type>();
		
		const auto size = static_cast<int>(end - begin);
		assert(size >= 0);

		if (vbo_.getSize() != 0 && usage_ == GL_STATIC_DRAW) {
			logger()->error("[Batch] VertexData is static, data index can't be modified");
			return;
		}
		for (auto it = begin; it != end; ++it) {
			auto index = *it;
			indexes_.push_back(index + currentIndexesIndex);
		}
	}

	template <class Vertex>
	void Batch<Vertex>::addIndexes(const std::vector<GLuint>& indexes) {
		insertIndexes(indexes.begin(), indexes.end());
	}

	template <class Vertex>
	template<class ...Indexes>
	void Batch<Vertex>::addIndexes(Indexes&& ...pack) {
		std::array<GLint, sizeof...(Indexes)> indexes = {{ pack ... }};
		insertIndexes(indexes.begin(), indexes.end());
	}

	template <class Vertex>
	bool Batch<Vertex>::isEveryIndexSizeValid() const {
		for (auto index : indexes_) {
			if (index < 0 || index >= vertexes_.size()) {
				return false;
			}
		}
		return true;
	}

	template <class Vertex>
	void Batch<Vertex>::assertIndexSizeIsValid(GLenum mode) const {
		assert(mode != GL_TRIANGLES || (mode == GL_TRIANGLES && indexes_.size() % 3 == 0 && isEveryIndexSizeValid()));
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_BATCH_H
