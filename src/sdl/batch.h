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

	inline void assertValidDrawMode(GLenum mode) {
		assert(
			GL_TRIANGLES == mode ||
			GL_POINTS == mode ||
			GL_LINE_STRIP == mode ||
			GL_LINE_LOOP == mode ||
			GL_LINES == mode ||
			GL_LINE_STRIP_ADJACENCY == mode ||
			GL_LINES_ADJACENCY == mode ||
			GL_TRIANGLE_FAN == mode ||
			GL_TRIANGLE_STRIP_ADJACENCY == mode ||
			GL_TRIANGLES_ADJACENCY == mode ||
			GL_PATCHES == mode
		);
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

			assertValidDrawMode(mode);
			assert(index_ >= 0 && size_ >= 0);
			IS_VERTEX_STANDARD_LAYOUT<Vertex>();
		}

		GLenum mode_ = 0;
		GLsizei index_ = 0;
		GLsizei size_ = 0;
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

		void pushBack(const Vertex& vertex);
		void pushBack(Vertex&& vertex);

		template<class ...Vertexes>
		void add(Vertexes&& ...vertexes);

		void startBatchView() noexcept;

		void startAdding() noexcept;
		
		BatchView<Vertex> getBatchView(GLenum mode) const noexcept;

		template<class InputIterator>
		void insertIndexes(InputIterator begin, InputIterator end);

		void pushBackIndex(GLint index);

		template<class ...Indexes>
		void addIndexes(Indexes&& ...vertexes);

	private:
		void bindAndBufferData();
		void bindAndBufferSubData();

		bool isValidBatchView(const BatchView<Vertex>& batchView) const;

		bool isEveryIndexSizeValid() const;

		void assertIndexSizeIsValid(GLenum mode) const;
		
		std::vector<Vertex> vertexes_;
		std::vector<GLint> indexes_;
		sdl::VertexBufferObject vbo_;
		sdl::VertexBufferObject vboIndexes_;
		
		GLsizei currentViewIndex_ = 0;
		GLuint currentIndexesIndex_ = 0;
		GLenum usage_ = 0;
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
		other.currentIndexesIndex_ = false;
	}
	
	template <class Vertex>
	Batch<Vertex>& Batch<Vertex>::operator=(Batch&& other) noexcept {
		vertexes_ = std::move(other.vertexes_);
		indexes_ = std::move(other.indexes_);
		vbo_ = std::move(other.vbo_);
		vboIndexes_ = std::move(other.vboIndexes_);
		usage_ = other.usage_;
		index_ = other.index_;
		currentIndexesIndex_ = other.currentIndexesIndex_;

		other.uploadedIndex_ = uploadedIndex_;
		other.usage_ = 0;
		other.index_ = 0;
		other.currentIndexesIndex_ = false;
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
		if (usage_ == GL_STATIC_DRAW) {
			logger()->warn("[Batch] Batch static, vbo failed to cleare");
			return;
		}

		vertexes_.size();
		indexes_.clear();
		currentIndexesIndex_ = 0;
		currentViewIndex_ = 0;
	}

	template <class Vertex>
	GLsizei Batch<Vertex>::getSize() const noexcept {
		return vertexes_.size();
	}
	
	template <class Vertex>
	GLsizei Batch<Vertex>::getIndexesSize() const noexcept {
		return indexes_.size();
	}

	template <class Vertex>
	void Batch<Vertex>::bindAndBufferData() {
		vbo_.bind(GL_ARRAY_BUFFER);
		vbo_.bufferData(vertexes_.size() * sizeof(Vertex), vertexes_.data(), usage_);
		if (!indexes_.empty()) {
			vboIndexes_.bind(GL_ELEMENT_ARRAY_BUFFER);
			vboIndexes_.bufferData(indexes_.size() * sizeof(GLuint), indexes_.data(), usage_);
		}
	}

	template <class Vertex>
	void Batch<Vertex>::bindAndBufferSubData() {
		vbo_.bind(GL_ARRAY_BUFFER);
		vbo_.bufferSubData(0, vertexes_.size() * sizeof(Vertex), vertexes_.data());

		if (!indexes_.empty()) {
			vboIndexes_.bind(GL_ELEMENT_ARRAY_BUFFER);
			vboIndexes_.bufferSubData(0, indexes_.size() * sizeof(GLuint), indexes_.data());
		}
	}

	template <class Vertex>
	void Batch<Vertex>::uploadToGraphicCard() {
		if (usage_ == GL_STATIC_DRAW) {
			if (vbo_.getSize() > 0) {
				logger()->debug("[Batch] Vbo is static but is replaced anyway");
			}
			bindAndBufferData();
		} else {
			if (vbo_.getSize() < static_cast<GLint>(vertexes_.size())) {
				bindAndBufferData();
			} else {
				bindAndBufferSubData();
			}
		}
	}

	template <class Vertex>
	void Batch<Vertex>::draw(GLenum mode) const {
		if (indexes_.empty()) {
			draw({mode, 0, static_cast<GLsizei>(vertexes_.size())});
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
			vertexes_.insert(vertexes_.end(), begin, end);
		} else {
			if (vbo_.getSize() == 0) {
				vertexes_.insert(vertexes_.end(), begin, end);
			} else {
				logger()->error("[Batch] VertexData is static, data can't be modified");
			}
		}
	}

	template <class Vertex>
	void Batch<Vertex>::pushBack(const Vertex& vertex) {
		vertexes_.push_back(vertex);
	}

	template <class Vertex>
	void Batch<Vertex>::pushBack(Vertex&& vertex) {
		vertexes_.push_back(vertex);
	}

	template <class Vertex>
	template<class ...Vertexes>
	void Batch<Vertex>::add(Vertexes&& ... pack) {
		std::array<Vertex, sizeof...(Vertexes)> vertexes = {{ pack ... }};
		insert(vertexes.begin(), vertexes.end());
	}

	template <class Vertex>
	void Batch<Vertex>::startBatchView() noexcept {
		if (indexes_.empty()) {
			currentViewIndex_ = static_cast<GLsizei>(vertexes_.size());
		} else {
			currentViewIndex_ = static_cast<GLsizei>(indexes_.size());
		}
	}

	template <class Vertex>
	void Batch<Vertex>::startAdding() noexcept {
		currentIndexesIndex_ = static_cast<GLuint>(vertexes_.size());
	}

	template <class Vertex>
	BatchView<Vertex> Batch<Vertex>::getBatchView(GLenum mode) const noexcept {
		if (indexes_.empty()) {
			return {mode, currentViewIndex_, static_cast<GLsizei>(vertexes_.size()) - currentViewIndex_};
		} else {
			return {mode, currentViewIndex_, static_cast<GLsizei>(indexes_.size()) - currentViewIndex_};
		}
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
			indexes_.push_back(index + currentIndexesIndex_);
		}
	}

	template <class Vertex>
	void Batch<Vertex>::pushBackIndex(GLint index) {
		indexes_.push_back(index);
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
