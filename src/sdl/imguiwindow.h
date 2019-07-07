#ifndef CPPSDL2_IMGUIWINDOW_H
#define CPPSDL2_IMGUIWINDOW_H

#include <sdl/window.h>
#include <sdl/sprite.h>
#include <sdl/textureatlas.h>

#include <imgui.h>

namespace sdl {

	class ImGuiWindow : public sdl::Window {
	public:
		ImGuiWindow();

		virtual ~ImGuiWindow();
	protected:		

		void initOpenGl() override;

		void initPreLoop() override;			

	private:
		void update(double deltaTime);

		void eventUpdate(const SDL_Event& windowEvent);

		void resize(int width, int height);

		bool show_demo_window;
		bool show_another_window;
		float menuHeight_;

		bool initiatedOpenGl_;
		bool initiatedSdl_;
	};

} // Namespace tetris.

#endif // CPPSDL2_IMGUIWINDOW_H
