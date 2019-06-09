#ifndef CPPSDL2_SDL_TEXTUREATLAS_H
#define CPPSDL2_SDL_TEXTUREATLAS_H

#include "opengl.h"
#include "texture.h"
#include "sprite.h"

#include <string>
#include <memory>
#include <map>

namespace sdl {

	// The packing algortihm is from http://www.blackpawn.com/texts/lightmaps/default.html.
	class TextureAtlas {
	public:
		TextureAtlas(int width, int height, const std::function<void()>& filter = []() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		});
		
		TextureAtlas(const TextureAtlas&) = delete;
		TextureAtlas& operator=(const TextureAtlas&) = delete;

		TextureAtlas(TextureAtlas&&) noexcept;
		TextureAtlas& operator=(TextureAtlas&&) noexcept;

		Sprite add(const std::string& filename, int border = 0, const std::string& uniqueKey = "");

		// Add the image to the texture atlas. Return true if successful, 
		// else it return false.
		Sprite add(SDL_Surface* image, int border = 0, const std::string& key = "");

		const Texture& getTexture() const {
			return texture_;
		}

		Sprite get(const std::string& key) const;

	private:
		static void uploadSdlSurfaceToTexture(SDL_Surface* image, SDL_Rect dstRec, Texture& texture);

		class Node : public std::enable_shared_from_this<Node> {
		public:
			// Create a new root node with plane, dimension defined by width and height, and 
			// a first image added at the top left corner of the defined plane.
			// Return the node containing the image if sucsessfull, else null
			// is returned.
			static std::shared_ptr<Node> createRoot(std::shared_ptr<Node>& root, int width, int height, SDL_Surface* image, int border);

			// Insert an image on the plane, dimensions defined by the root node.
			// Should only be called by the root node.
			// Return the node containing the image if sucsessfull, else null
			// is returned.
			std::shared_ptr<Node> insert(SDL_Surface* image, int border);

			Node(int x, int y, int w, int h);

			// Return the coordinate for the node. (x,y) represents the
			// up-left coordinate of the rectangle. Including the border.
			SDL_Rect getRect() const {
				return rect_;
			}

		private:
			SDL_Surface* image_;
			std::shared_ptr<Node> left_;
			std::shared_ptr<Node> right_;
			SDL_Rect rect_;
		};

		Texture texture_;
		std::map<std::string, Sprite> images_;
		std::shared_ptr<Node> root_;
		int width_, height_;
	};

} // Namespace sdl.

#endif // CPPSDL2_SDL_TEXTUREATLAS_H
