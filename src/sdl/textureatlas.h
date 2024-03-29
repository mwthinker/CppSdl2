#ifndef CPPSDL2_SDL_TEXTUREATLAS_H
#define CPPSDL2_SDL_TEXTUREATLAS_H

#include "opengl.h"
#include "texture.h"
#include "sprite.h"
#include "surface.h"
#include "textureview.h"

#include <string>
#include <memory>
#include <unordered_map>

namespace sdl {

	// The packing algorithm is from http://www.blackpawn.com/texts/lightmaps/default.html.
	class TextureAtlas {
	public:
		TextureAtlas() = default;

		TextureAtlas(int width, int height, std::function<void()>&& filter = []() {
			gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MIN_FILTER, gl::GL_LINEAR);
			gl::glTexParameteri(gl::GL_TEXTURE_2D, gl::GL_TEXTURE_MAG_FILTER, gl::GL_LINEAR);
		});
		
		TextureAtlas(const TextureAtlas&) = delete;
		TextureAtlas& operator=(const TextureAtlas&) = delete;

		TextureAtlas(TextureAtlas&&) = default;
		TextureAtlas& operator=(TextureAtlas&&) = default;

		const Sprite& add(const std::string& filename, int border = 0, const std::string& uniqueKey = "");
		
		const Sprite& add(const Surface& texture, int border = 0, const std::string& uniqueKey = "");

		const Sprite& get(const std::string& key) const;

		const Sprite& get() const;

		void bind();

		TextureView getTextureView() const;

		TextureView getTextureView(const std::string& key) const;

	private:
		class Node {
		public:
			// Insert an image on the plane, dimensions defined by the root node.
			// Should only be called by the root node.
			// Return the node containing the image if successful, else null
			// is returned.
			Node* insert(const Surface& surface, int border);

			Node(Rect rect);

			// Return the coordinate for the node. (x,y) represents the
			// up-left coordinate of the rectangle. Including the border.
			Rect getRect() const {
				return rect_;
			}

			bool image = false;
		private:
			std::unique_ptr<Node> left_;
			std::unique_ptr<Node> right_;
			Rect rect_{};
		};

		// Create a new root node with plane, dimension defined by width and height, and 
		// a first image added at the top left corner of the defined plane.
		// Return the node containing the image if successful, else null
		// is returned.
		static Node* createRoot(std::unique_ptr<Node>& root,
			int width, int height, const Surface& surface, int border);

		Sprite sprite_;
		mutable std::unordered_map<std::string, Sprite> images_;
		std::unique_ptr<Node> root_;
	};

}

#endif
