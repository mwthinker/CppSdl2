#ifndef CPPSDL2_SDL_BATCH_H
#define CPPSDL2_SDL_BATCH_H

#include "vertexbufferobject.h"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <vector>

namespace sdl {

	inline void assertValidDrawMode(GLenum mode) {
		bool warning =
			GL_TRIANGLES == mode ||
			GL_POINTS == mode ||
			GL_LINE_STRIP == mode ||
			GL_LINE_LOOP == mode ||
			GL_LINES == mode ||
			GL_LINE_STRIP_ADJACENCY == mode ||
			GL_LINES_ADJACENCY == mode ||
#ifdef GL_PATCHES
			GL_PATCHES == mode ||
#endif
			GL_TRIANGLE_FAN == mode ||
			GL_TRIANGLE_STRIP_ADJACENCY == mode ||
			GL_TRIANGLES_ADJACENCY == mode;
		assert(warning);
	}

	template<typename T>
	concept VertexType = std::is_standard_layout_v<T>;

	template<VertexType T>
	class Batch;

	template <VertexType Vertex>
	class SubBatch {
	public:
		// ---- Vertexes
		
		template<typename InputIterator> requires std::random_access_iterator<InputIterator>
		void insert(InputIterator begin, InputIterator end);
		
		void insert(std::initializer_list<Vertex> list);
		
		void pushBack(const Vertex& vertex);

		bool isEmpty() const noexcept;

		GLsizei getSize() const noexcept;

		const Vertex* getData() const noexcept;
		
		// ---- Indexes
		
		template<typename InputIterator> requires std::random_access_iterator<InputIterator>
		void insertIndexes(InputIterator begin, InputIterator end);

		void insertIndexes(std::initializer_list<GLint> list);

		void pushBackIndex(GLint index);

		GLsizei getIndexesSize() const noexcept;

		const GLint* getIndexData() const noexcept;

		// ---- General

		void clear();

		int totalSizeInBytes() const noexcept;

		bool isIndexBatch() const noexcept;

		bool isEveryIndexSizeValid() const;

		const std::vector<Vertex>& getVertexes() const noexcept;

		const std::vector<Vertex>& getIndexes() const noexcept;

	private:
		std::vector<Vertex> vertexes_;
		std::vector<GLint> indexes_;
	};

	template<VertexType Vertex>
	class Batch;

	template <VertexType Vertex>
	class BatchView {
	public:
		friend class Batch<Vertex>;

		BatchView() = default;

		bool isEmpty() const noexcept {
			return size_ == 0;
		}

		bool tryMerge(const BatchView& view) noexcept {
			if (mode_ == view.mode_ && index_ + size_ == view.index_) {
				size_ += view.size_;
				return true;
			}
			return false;
		};

		bool isIndexSizeValid() const noexcept {
			return mode_ != GL_TRIANGLES || (mode_ == GL_TRIANGLES && size_ % 3 == 0);
		}

	private:
		BatchView(GLenum mode, GLsizei index, GLsizei size) noexcept
			: mode_{mode}
			, index_{index}
			, size_{size} {

			assertValidDrawMode(mode);
			assert(index_ >= 0 && size_ >= 0);
		}

		GLenum mode_ = 0;
		GLsizei index_ = 0;
		GLsizei size_ = 0;
	};

	template <VertexType Vertex>
	class Batch {
	public:
		explicit Batch(GLenum usage);

		~Batch() = default;

		Batch(Batch&& other) noexcept;
		Batch& operator=(Batch&& other) noexcept;

		Batch(const Batch& batch) = delete;
		Batch &operator=(const Batch& batch) = delete;

		GLenum getUsage() const noexcept;

		float getVboSizeInMiB() const noexcept;

		void bind();

		void clear();

		bool isEmpty() const noexcept;

		GLsizei getSize() const noexcept;

		GLsizei getIndexesSize() const noexcept;

		void uploadToGraphicCard();
		void draw(GLenum mode) const;
		void draw(const BatchView<Vertex>& batchView) const;

		void add(const SubBatch<Vertex>& subBatch);

		template<typename InputIterator> requires std::random_access_iterator<InputIterator>
		void insert(InputIterator begin, InputIterator end);

		void insert(std::initializer_list<Vertex> list);

		void pushBack(const Vertex& vertex);

		void startBatchView() noexcept;

		void startAdding() noexcept;
		
		BatchView<Vertex> getBatchView(GLenum mode) const noexcept;

		template<typename InputIterator> requires std::random_access_iterator<InputIterator>
		void insertIndexes(InputIterator begin, InputIterator end);

		void insertIndexes(std::initializer_list<GLint> list);

		void pushBackIndex(GLint index);

		bool isEveryIndexSizeValid() const;

	private:
		void bindAndBufferData();
		void bindAndBufferSubData();

		bool isValidBatchView(const BatchView<Vertex>& batchView) const;
		
		SubBatch<Vertex> fullBatch_;
		sdl::VertexBufferObject vbo_;
		sdl::VertexBufferObject vboIndexes_;
		
		GLsizei currentViewIndex_ = 0;
		GLuint currentIndexesIndex_ = 0;
		GLenum usage_ = 0;
	};

	template <VertexType Vertex>
	Batch<Vertex>::Batch(GLenum usage)
		: usage_{usage} {
	}

	template <VertexType Vertex>
	Batch<Vertex>::Batch(Batch&& other) noexcept :
		fullBatch_{std::move(other.fullBatch_)},
		vbo_{std::move(other.vbo_)},
		vboIndexes_{std::move(other.vboIndexes_)},

		currentViewIndex_{std::exchange(other.currentViewIndex_, 0)},
		currentIndexesIndex_{std::exchange(other.currentIndexesIndex_, 0)},
		usage_{std::exchange(other.usage_, 0)}
	{ }

	template <VertexType Vertex>
	Batch<Vertex>& Batch<Vertex>::operator=(Batch&& other) noexcept {
		fullBatch_ = std::move(other.fullBatch_);
		vbo_ = std::move(other.vbo_);
		vboIndexes_ = std::move(other.vboIndexes_);

		currentViewIndex_ = std::exchange(other.currentViewIndex_, 0);
		currentIndexesIndex_ = std::exchange(other.currentIndexesIndex_, 0);
		usage_ = std::exchange(other.usage_, 0);
		return *this;
	}

	template <VertexType Vertex>
	GLenum Batch<Vertex>::getUsage() const noexcept {
		return usage_;
	}

	template <VertexType Vertex>
	float Batch<Vertex>::getVboSizeInMiB() const noexcept {
		return fullBatch_.totalSizeInBytes() * 1.f / 1024 / 1024;
	}

	template <VertexType Vertex>
	void Batch<Vertex>::bind() {
		if (!vbo_.isGenerated()) {
			vbo_.generate();
		}
		vbo_.bind(GL_ARRAY_BUFFER);

		if (fullBatch_.isIndexBatch()) {
			if (!vboIndexes_.isGenerated()) {
				vboIndexes_.generate();
			}
			vboIndexes_.bind(GL_ELEMENT_ARRAY_BUFFER);
		}
	}

	template <VertexType Vertex>
	void Batch<Vertex>::clear() {
		if (usage_ == GL_STATIC_DRAW) {
			spdlog::warn("[sdl::Batch] Batch static, vbo failed to cleare");
			return;
		}

		fullBatch_.clear();
		currentIndexesIndex_ = 0;
		currentViewIndex_ = 0;
	}

	template <VertexType Vertex>
	bool Batch<Vertex>::isEmpty() const noexcept {
		return fullBatch_.isEmpty();
	}

	template <VertexType Vertex>
	GLsizei Batch<Vertex>::getSize() const noexcept {
		return fullBatch_.getSize();
	}

	template <VertexType Vertex>
	GLsizei Batch<Vertex>::getIndexesSize() const noexcept {
		return fullBatch_.getIndexesSize();
	}

	template <VertexType Vertex>
	void Batch<Vertex>::bindAndBufferData() {
		vbo_.bind(GL_ARRAY_BUFFER);
		vbo_.bufferData(fullBatch_.getSize() * sizeof(Vertex), fullBatch_.getData(), usage_);
		if (fullBatch_.isIndexBatch()) {
			vboIndexes_.bind(GL_ELEMENT_ARRAY_BUFFER);
			vboIndexes_.bufferData(fullBatch_.getIndexesSize() * sizeof(GLuint), fullBatch_.getIndexData(), usage_);
		}
	}

	template <VertexType Vertex>
	void Batch<Vertex>::bindAndBufferSubData() {
		vbo_.bind(GL_ARRAY_BUFFER);
		vbo_.bufferSubData(0, fullBatch_.getSize() * sizeof(Vertex), fullBatch_.getData());

		if (fullBatch_.isIndexBatch()) {
			vboIndexes_.bind(GL_ELEMENT_ARRAY_BUFFER);
			vboIndexes_.bufferSubData(0, fullBatch_.getIndexesSize() * sizeof(GLuint), fullBatch_.getIndexData());
		}
	}

	template <VertexType Vertex>
	void Batch<Vertex>::uploadToGraphicCard() {
		if (usage_ == GL_STATIC_DRAW) {
			if (vbo_.getSize() > 0) {
				spdlog::debug("[sdl::Batch] Vbo is static but is replaced anyway");
			}
			bindAndBufferData();
		} else {
			if (vbo_.getSize() < static_cast<GLint>(fullBatch_.getSize())) {
				bindAndBufferData();
			} else {
				bindAndBufferSubData();
			}
		}
	}

	template <VertexType Vertex>
	void Batch<Vertex>::draw(GLenum mode) const {
		if (fullBatch_.isIndexBatch()) {
			draw({mode, 0, static_cast<GLsizei>(fullBatch_.getIndexesSize())});
		} else {
			draw({mode, 0, static_cast<GLsizei>(fullBatch_.getSize())});
		}
	}

	template <VertexType Vertex>
	bool Batch<Vertex>::isValidBatchView(const BatchView<Vertex>& batchView) const {
		if (fullBatch_.isIndexBatch()) {
			return batchView.index_ >= 0 && batchView.index_ + static_cast<GLsizei>(batchView.size_) <= fullBatch_.getIndexesSize();
		} else {
			return batchView.index_ >= 0 && batchView.index_ + static_cast<GLsizei>(batchView.size_) <= fullBatch_.getSize();
		}
	}

	template <VertexType Vertex>
	void Batch<Vertex>::draw(const BatchView<Vertex>& batchView) const {
		if (vbo_.getSize() > 0) {
			if (!isValidBatchView(batchView)) {
				spdlog::warn("[sdl::Batch] BatchView not valid. start = {}, size = {}", batchView.index_, batchView.size_);
				return;
			}

			if (fullBatch_.isIndexBatch()) {
				assert(batchView.isIndexSizeValid());
				glDrawElements(batchView.mode_, batchView.size_, GL_UNSIGNED_INT, reinterpret_cast<void*>(batchView.index_ * sizeof(GLint)));
			} else {
				glDrawArrays(batchView.mode_, batchView.index_, batchView.size_);
			}
			assertGlError();
		} else if (!vbo_.isGenerated()) {
			spdlog::error("[sdl::Batch] Vertex data failed to draw, no vbo binded, i.e. Batch::uploadToGraphicCard never called");
		}
	}

	template <VertexType Vertex>
	void Batch<Vertex>::add(const SubBatch<Vertex>& subBatch) {
		const auto& vertexes = subBatch.getVertexes();
		insert(vertexes.begin(), vertexes.end());

		const auto& indexes = subBatch.getIndexes();
		insertIndexes(indexes.begin(), indexes.end());
	}

	template <VertexType Vertex>
	template<typename InputIterator> requires std::random_access_iterator<InputIterator>
	void Batch<Vertex>::insert(InputIterator begin, InputIterator end) {
		if (usage_ != GL_STATIC_DRAW) {
			fullBatch_.insert(begin, end);
		} else {
			if (vbo_.getSize() == 0) {
				fullBatch_.insert(begin, end);
			} else {
				spdlog::error("[sdl::Batch] VertexData is static, data can't be modified");
			}
		}
	}

	template <VertexType Vertex>
	void Batch<Vertex>::insert(std::initializer_list<Vertex> list) {
		insert(list.begin(), list.end());
	}

	template <VertexType Vertex>
	void Batch<Vertex>::pushBack(const Vertex& vertex) {
		fullBatch_.pushBack(vertex);
	}

	template <VertexType Vertex>
	void Batch<Vertex>::startBatchView() noexcept {
		if (fullBatch_.isIndexBatch()) {
			currentViewIndex_ = static_cast<GLsizei>(fullBatch_.getIndexesSize());
		} else {
			currentViewIndex_ = static_cast<GLsizei>(fullBatch_.getSize());
		}
	}

	template <VertexType Vertex>
	void Batch<Vertex>::startAdding() noexcept {
		currentIndexesIndex_ = static_cast<GLuint>(fullBatch_.getSize());
	}

	template <VertexType Vertex>
	BatchView<Vertex> Batch<Vertex>::getBatchView(GLenum mode) const noexcept {
		if (fullBatch_.isIndexBatch()) {
			return {mode, currentViewIndex_, static_cast<GLsizei>(fullBatch_.getIndexesSize()) - currentViewIndex_};
		}
		return {mode, currentViewIndex_, static_cast<GLsizei>(fullBatch_.getSize()) - currentViewIndex_};
	}

	template <VertexType Vertex>
	template<typename InputIterator> requires std::random_access_iterator<InputIterator>
	void Batch<Vertex>::insertIndexes(InputIterator begin, InputIterator end) {
		if (vbo_.getSize() != 0 && usage_ == GL_STATIC_DRAW) {
			spdlog::error("[sdl::Batch] Vertex data is static, data index can't be modified");
			return;
		}
		for (auto it = begin; it != end; ++it) {
			auto index = *it;
			fullBatch_.pushBackIndex(index + currentIndexesIndex_);
		}
	}

	template <VertexType Vertex>
	void Batch<Vertex>::insertIndexes(std::initializer_list<GLint> list) {
		insertIndexes(list.begin(), list.end());
	}

	template <VertexType Vertex>
	void Batch<Vertex>::pushBackIndex(GLint index) {
		fullBatch_.pushBackIndex(index + currentIndexesIndex_);
	}

	template <VertexType Vertex>
	bool Batch<Vertex>::isEveryIndexSizeValid() const {
		return fullBatch_.isEveryIndexSizeValid();
	}

	// ---- Vertexes

	template <VertexType Vertex>
	template<typename InputIterator> requires std::random_access_iterator<InputIterator>
	void SubBatch<Vertex>::insert(InputIterator begin, InputIterator end) {
		assert(end - begin >= 0);

		vertexes_.insert(vertexes_.end(), begin, end);
	}
	
	template <VertexType Vertex>
	void SubBatch<Vertex>::insert(std::initializer_list<Vertex> list) {
		vertexes_.insert(vertexes_.end(), list);
	}

	template <VertexType Vertex>
	void SubBatch<Vertex>::pushBack(const Vertex& vertex) {
		vertexes_.push_back(vertex);
	}

	template <VertexType Vertex>
	bool SubBatch<Vertex>::isEmpty() const noexcept {
		return vertexes_.empty();
	}

	template <VertexType Vertex>
	GLsizei SubBatch<Vertex>::getSize() const noexcept {
		return static_cast<GLsizei>(vertexes_.size());
	}

	template <VertexType Vertex>
	const Vertex* SubBatch<Vertex>::getData() const noexcept {
		return vertexes_.data();
	}

	// ---- Indexes

	template <VertexType Vertex>
	template<typename InputIterator> requires std::random_access_iterator<InputIterator>
	void SubBatch<Vertex>::insertIndexes(InputIterator begin, InputIterator end) {
		for (auto it = begin; it != end; ++it) {
			indexes_.push_back(*it);
		}
	}

	template <VertexType Vertex>
	void SubBatch<Vertex>::insertIndexes(std::initializer_list<GLint> list) {
		vertexes_.insert(vertexes_.end(), list);
	}

	template <VertexType Vertex>
	void SubBatch<Vertex>::pushBackIndex(GLint index) {
		indexes_.push_back(index);
	}

	template <VertexType Vertex>
	GLsizei SubBatch<Vertex>::getIndexesSize() const noexcept {
		return static_cast<GLsizei>(indexes_.size());
	}

	template <VertexType Vertex>
	const GLint* SubBatch<Vertex>::getIndexData() const noexcept {
		return indexes_.data();
	}

	// ---- General

	template <VertexType Vertex>
	void SubBatch<Vertex>::clear() {
		vertexes_.clear();
		indexes_.clear();
	}

	template <VertexType Vertex>
	int SubBatch<Vertex>::totalSizeInBytes() const noexcept {
		return static_cast<int>(sizeof(GLuint) * vertexes_.size() + sizeof(Vertex) * indexes_.size());
	}

	template <VertexType Vertex>
	bool SubBatch<Vertex>::isIndexBatch() const noexcept {
		return !indexes_.empty();
	}

	template <VertexType Vertex>
	bool SubBatch<Vertex>::isEveryIndexSizeValid() const {
		for (auto index : indexes_) {
			if (index < 0 || index >= vertexes_.size()) {
				return false;
			}
		}
		return true;
	}

	template <VertexType Vertex>
	const std::vector<Vertex>& SubBatch<Vertex>::getVertexes() const noexcept {
		return vertexes_;
	}

	template <VertexType Vertex>
	const std::vector<Vertex>& SubBatch<Vertex>::getIndexes() const noexcept {
		return indexes_;
	}

}

#endif
