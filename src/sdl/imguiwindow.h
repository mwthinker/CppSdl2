#ifndef CPPSDL2_IMGUIWINDOW_H
#define CPPSDL2_IMGUIWINDOW_H

#include "window.h"
#include "imgui.h"
#include "imguiauxiliary.h"

namespace sdl {

	class ImGuiWindow : public sdl::Window {
	public:
		ImGuiWindow() = default;

		ImGuiWindow(int majorVersionGl, int minorVersionGl);

		~ImGuiWindow() override;

		bool isShowDemoWindow() const;
		void setShowDemoWindow(bool show);

		bool isShowColorWindow() const;
		void setShowColorWindow(bool show);

	protected:
		void initPreLoop() override;

		void eventUpdate(const SDL_Event& windowEvent) override;

	private:
		void update(const DeltaTime& deltaTime) final;
		
		virtual void imGuiEventUpdate(const SDL_Event& windowEvent) {};
		virtual void imGuiPreUpdate(const DeltaTime& deltaTime) {};
		virtual void imGuiUpdate(const DeltaTime& deltaTime) {};
		virtual void imGuiPostUpdate(const DeltaTime& deltaTime) {};
		
		bool showDemoWindow_ = false;
		bool showColorWindow_ = false;
	};

	inline bool ImGuiWindow::isShowDemoWindow() const {
		return showDemoWindow_;
	}

	inline void ImGuiWindow::setShowDemoWindow(bool show) {
		showDemoWindow_ = show;
	}

	inline bool ImGuiWindow::isShowColorWindow() const {
		return showColorWindow_;
	}

	inline void ImGuiWindow::setShowColorWindow(bool show) {
		showColorWindow_ = show;
	}

}

#endif
