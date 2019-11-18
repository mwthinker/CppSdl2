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

	template<class T>
	class Batch;

	template <class Vertex>
	class SubBatch {
	public:
		// ---- Vertexes
		
		template<class InputIterator>
		void insert(InputIterator begin, InputIterator end);
		
		void pushBack(const Vertex& vertex);
		
		template<class ...Vertexes>
		void add(Vertexes&& ...vertexes);

		GLsizei getSize() const noexcept;

		const Vertex* getData() const noexcept;
		
		// ---- Indexes
		
		template<class InputIterator>
		void insertIndexes(InputIterator begin, InputIterator end);

		void pushBackIndex(GLint index);

		template<class ...Indexes>
		void addIndexes(Indexes&& ...vertexes);

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
			mode_{mode}, index_{index}, size_{size} {

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
		GLsizei getSize() const noexcept;

		GLsizei getIndexesSize() const noexcept;

		void uploadToGraphicCard();
		void draw(GLenum mode) const;
		void draw(const BatchView<Vertex>& batchView) const;

		void add(const SubBatch<Vertex>& subBatch);

		template<class InputIterator>
		void insert(InputIterator begin, InputIterator end);

		void pushBack(const Vertex& vertex);

		template<class ...Vertexes>
		void add(Vertexes&& ...vertexes);

		void startBatchView() noexcept;

		void startAdding() noexcept;
		
		BatchView<Vertex> getBatchView(GLenum mode) const noexcept;

		template<class InputIterator>
		void insertIndexes(InputIterator begin, InputIterator end);

		void pushBackIndex(GLint index);

		template<class ...Indexes>
		void addIndexes(Indexes&& ...indexes);

	private:
		void bindAndBufferData();
		void bindAndBufferSubData();

		bool isValidBatchView(const BatchView<Vertex>& batchView) const;

		bool isEveryIndexSizeValid() const;

		void assertIndexSizeIsValid(GLenum mode) const;
		
		SubBatch<Vertex> fullBatch_;
		sdl::VertexBufferObject vbo_;
		sdl::VertexBufferObject vboIndexes_;
		
		GLsizei currentViewIndex_ = 0;
		GLuint currentIndexesIndex_ = 0;
		GLenum usage_ = 0;
	};

	template <class Vertex>
	Batch<Vertex>::Batch(GLenum usage) : usage_{usage} {
		IS_VERTEX_STANDARD_LAYOUT<Vertex>();
	}

	template <class Vertex>
	Batch<Vertex>::Batch(Batch&& other) noexcept :
		fullBatch_{std::move(other.fullBatch_)},
		vbo_{std::move(other.vbo_)},
		vboIndexes_{std::move(other.vboIndexes_)},
		
		currentViewIndex_{std::exchange(other.currentViewIndex_, 0)},
		currentIndexesIndex_{std::exchange(other.currentIndexesIndex_, 0)},
		usage_{std::exchange(other.usage_, 0)}
	{
		IS_VERTEX_STANDARD_LAYOUT<Vertex>();
	}
	
	template <class Vertex>
	Batch<Vertex>& Batch<Vertex>::operator=(Batch&& other) noexcept {
		fullBatch_ = std::move(other.fullBatch_);
		vbo_ = std::move(other.vbo_);
		vboIndexes_ = std::move(other.vboIndexes_);

		currentViewIndex_ = std::exchange(other.currentViewIndex_, 0);
		currentIndexesIndex_ = std::exchange(other.currentIndexesIndex_, 0);
		usage_ = std::exchange(other.usage_, 0);
		return *this;
	}

	template <class Vertex>
	GLenum Batch<Vertex>::getUsage() const noexcept {
		return usage_;
	}

	template <class Vertex>
	float Batch<Vertex>::getVboSizeInMiB() const noexcept {
		return fullBatch_.totalSizeInBytes() * 1.f / 1024 / 1024;
	}

	template <class Vertex>
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

	template <class Vertex>
	void Batch<Vertex>::clear() {
		if (usage_ == GL_STATIC_DRAW) {
			logger()->warn("[Batch] Batch static, vbo failed to cleare");
			return;
		}

		fullBatch_.clear();
		currentIndexesIndex_ = 0;
		currentViewIndex_ = 0;
	}

	template <class Vertex>
	GLsizei Batch<Vertex>::getSize() const noexcept {
		return fullBatch_.getSize();
	}
	
	template <class Vertex>
	GLsizei Batch<Vertex>::getIndexesSize() const noexcept {
		return fullBatch_.getIndexesSize();
	}

	template <class Vertex>
	void Batch<Vertex>::bindAndBufferData() {
		vbo_.bind(GL_ARRAY_BUFFER);
		vbo_.bufferData(fullBatch_.getSize() * sizeof(Vertex), fullBatch_.getData(), usage_);
		if (fullBatch_.isIndexBatch()) {
			vboIndexes_.bind(GL_ELEMENT_ARRAY_BUFFER);
			vboIndexes_.bufferData(fullBatch_.getIndexesSize() * sizeof(GLuint), fullBatch_.getIndexData(), usage_);
		}
	}

	template <class Vertex>
	void Batch<Vertex>::bindAndBufferSubData() {
		vbo_.bind(GL_ARRAY_BUFFER);
		vbo_.bufferSubData(0, fullBatch_.getSize() * sizeof(Vertex), fullBatch_.getData());

		if (fullBatch_.isIndexBatch()) {
			vboIndexes_.bind(GL_ELEMENT_ARRAY_BUFFER);
			vboIndexes_.bufferSubData(0, fullBatch_.getIndexesSize() * sizeof(GLuint), fullBatch_.getIndexData());
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
			if (vbo_.getSize() < static_cast<GLint>(fullBatch_.getSize())) {
				bindAndBufferData();
			} else {
				bindAndBufferSubData();
			}
		}
	}

	template <class Vertex>
	void Batch<Vertex>::draw(GLenum mode) const {
		if (fullBatch_.isIndexBatch()) {
			draw({mode, 0, static_cast<GLsizei>(fullBatch_.getIndexesSize())});
		} else {
			draw({mode, 0, static_cast<GLsizei>(fullBatch_.getSize())});
		}
	}

	template <class Vertex>
	bool Batch<Vertex>::isValidBatchView(const BatchView<Vertex>& batchView) const {
		if (fullBatch_.isIndexBatch()) {
			return batchView.index_ >= 0 && batchView.index_ + static_cast<GLsizei>(batchView.size_) <= fullBatch_.getIndexesSize();			
		} else {
			return batchView.index_ >= 0 && batchView.index_ + static_cast<GLsizei>(batchView.size_) <= fullBatch_.getSize();
		}
	}

	template <class Vertex>
	void Batch<Vertex>::draw(const BatchView<Vertex>& batchView) const {
		if (vbo_.getSize() > 0) {
			if (!isValidBatchView(batchView)) {
				logger()->warn("[Batch] BatchView not valid. start = {}, size = {}", batchView.index_, batchView.size_);
				return;
			}

			if (fullBatch_.isIndexBatch()) {
				assertIndexSizeIsValid(batchView.mode_);
				glDrawElements(batchView.mode_, batchView.size_, GL_UNSIGNED_INT, reinterpret_cast<void*>(batchView.index_ * sizeof(GLint)));
			} else {
				glDrawArrays(batchView.mode_, batchView.index_, batchView.size_);
			}
			assertGlError();
		} else if (!vbo_.isGenerated()) {
			logger()->error("[Batch] Vertex data failed to draw, no vbo binded, i.e. Batch::uploadToGraphicCard never called");
		}
	}

	template <class Vertex>
	void Batch<Vertex>::add(const SubBatch<Vertex>& subBatch) {
		const auto& vertexes = subBatch.getVertexes();
		insert(vertexes.begin(), vertexes.end());

		const auto& indexes = subBatch.getIndexes();
		insertIndexes(indexes.begin(), indexes.end());
	}

	template <class Vertex>
	template<class InputIterator>
	void Batch<Vertex>::insert(InputIterator begin, InputIterator end) {
		IS_RANDOM_ACCESS_ITERATOR<InputIterator>();

		if (usage_ != GL_STATIC_DRAW) {
			fullBatch_.insert(begin, end);
		} else {
			if (vbo_.getSize() == 0) {
				fullBatch_.insert(begin, end);
			} else {
				logger()->error("[] VertexData is static, data can't be modified");
			}
		}
	}

	template <class Vertex>
	void Batch<Vertex>::pushBack(const Vertex& vertex) {
		fullBatch_.pushBack(vertex);
	}

	template <class Vertex>
	template<class ...Vertexes>
	void Batch<Vertex>::add(Vertexes&&... pack) {
		std::array<Vertex, sizeof...(Vertexes)> vertexes = {{ pack ... }};
		insert(vertexes.begin(), vertexes.end());
	}

	template <class Vertex>
	void Batch<Vertex>::startBatchView() noexcept {
		if (fullBatch_.isIndexBatch()) {
			currentViewIndex_ = static_cast<GLsizei>(fullBatch_.getIndexesSize());
		} else {
			currentViewIndex_ = static_cast<GLsizei>(fullBatch_.getSize());
		}
	}

	template <class Vertex>
	void Batch<Vertex>::startAdding() noexcept {
		currentIndexesIndex_ = static_cast<GLuint>(fullBatch_.getSize());
	}

	template <class Vertex>
	BatchView<Vertex> Batch<Vertex>::getBatchView(GLenum mode) const noexcept {
		if (fullBatch_.isIndexBatch()) {
			return {mode, currentViewIndex_, static_cast<GLsizei>(fullBatch_.getIndexesSize()) - currentViewIndex_};
		}
		return {mode, currentViewIndex_, static_cast<GLsizei>(fullBatch_.getSize()) - currentViewIndex_};
	}

	template <class Vertex>
	template<class InputIterator>
	void Batch<Vertex>::insertIndexes(InputIterator begin, InputIterator end) {
		IS_RANDOM_ACCESS_ITERATOR<InputIterator>();
		IS_INDEX_TYPE<typename std::decay<decltype(*begin)>::type>();
		
		if (vbo_.getSize() != 0 && usage_ == GL_STATIC_DRAW) {
			logger()->error("[Batch] Vertex data is static, data index can't be modified");
			return;
		}
		for (auto it = begin; it != end; ++it) {
			*it += currentIndexesIndex_;
		}
		fullBatch_.insertIndexes(begin, end);
	}

	template <class Vertex>
	void Batch<Vertex>::pushBackIndex(GLint index) {
		fullBatch_.pushBackIndex(index + currentIndexesIndex_);
	}

	template <class Vertex>
	template<class ...Indexes>
	void Batch<Vertex>::addIndexes(Indexes&& ...pack) {
		std::array<GLint, sizeof...(Indexes)> indexes = {{ pack ... }};
		insertIndexes(indexes.begin(), indexes.end());
	}

	template <class Vertex>
	bool Batch<Vertex>::isEveryIndexSizeValid() const {
		return fullBatch_.isEveryIndexSizeValid();
	}

	template <class Vertex>
	void Batch<Vertex>::assertIndexSizeIsValid(GLenum mode) const {
		assert(mode != GL_TRIANGLES || (mode == GL_TRIANGLES && fullBatch_.getIndexesSize() % 3 == 0 && isEveryIndexSizeValid()));
	}

	// ---- Vertexes

	template <class Vertex>
	template<class InputIterator>
	void SubBatch<Vertex>::insert(InputIterator begin, InputIterator end) {
		IS_RANDOM_ACCESS_ITERATOR<InputIterator>();

		const auto size = static_cast<int>(end - begin);
		assert(size >= 0);

		vertexes_.insert(vertexes_.end(), begin, end);
	}

	template <class Vertex>
	void SubBatch<Vertex>::pushBack(const Vertex& vertex) {
		vertexes_.push_back(vertex);
	}

	template <class Vertex>
	template<class ...Vertexes>
	void SubBatch<Vertex>::add(Vertexes&& ... pack) {
		std::array<Vertex, sizeof...(Vertexes)> vertexes = {{ pack ... }};
		insert(vertexes.begin(), vertexes.end());
	}

	template <class Vertex>
	GLsizei SubBatch<Vertex>::getSize() const noexcept {
		return static_cast<GLsizei>(vertexes_.size());
	}

	template <class Vertex>
	const Vertex* SubBatch<Vertex>::getData() const noexcept {
		return vertexes_.data();
	}

	// ---- Indexes

	template <class Vertex>
	template<class InputIterator>
	void SubBatch<Vertex>::insertIndexes(InputIterator begin, InputIterator end) {
		IS_RANDOM_ACCESS_ITERATOR<InputIterator>();
		IS_INDEX_TYPE<typename std::decay<decltype(*begin)>::type>();
		
		for (auto it = begin; it != end; ++it) {
			auto index = *it;
			indexes_.push_back(index);
		}
	}

	template <class Vertex>
	void SubBatch<Vertex>::pushBackIndex(GLint index) {
		indexes_.push_back(index);
	}

	template <class Vertex>
	template<class ...Indexes>
	void SubBatch<Vertex>::addIndexes(Indexes&& ...pack) {
		std::array<GLint, sizeof...(Indexes)> indexes = {{ pack ... }};
		insertIndexes(indexes.begin(), indexes.end());
	}

	template <class Vertex>
	GLsizei SubBatch<Vertex>::getIndexesSize() const noexcept {
		return static_cast<GLsizei>(indexes_.size());
	}

	template <class Vertex>
	const GLint* SubBatch<Vertex>::getIndexData() const noexcept {
		return indexes_.data();
	}

	// ---- General

	template <class Vertex>
	void SubBatch<Vertex>::clear() {
		vertexes_.clear();
		indexes_.clear();
	}

	template <class Vertex>
	int SubBatch<Vertex>::totalSizeInBytes() const noexcept {
		return static_cast<int>(sizeof(GLuint) * vertexes_.size() + sizeof(Vertex) * indexes_.size());
	}

	template <class Vertex>
	bool SubBatch<Vertex>::isIndexBatch() const noexcept {
		return !indexes_.empty();
	}

	template <class Vertex>
	bool SubBatch<Vertex>::isEveryIndexSizeValid() const {
		for (auto index : indexes_) {
			if (index < 0 || index >= vertexes_.size()) {
				return false;
			}
		}
		return true;
	}

	template <class Vertex>
	const std::vector<Vertex>& SubBatch<Vertex>::getVertexes() const noexcept {
		return vertexes_;
	}

	template <class Vertex>
	const std::vector<Vertex>& SubBatch<Vertex>::getIndexes() const noexcept {
		return indexes_;
	}

} // Namespace sdl.

#endif // CPPSDL2_SDL_BATCH_H
