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

	protected:
		void initPreLoop() override;

		void eventUpdate(const SDL_Event& windowEvent) override;

	private:
		void update(const DeltaTime& deltaTime) final;

		virtual void imGuiPreUpdate(const DeltaTime& deltaTime) {};
		virtual void imGuiUpdate(const DeltaTime& deltaTime) {};
		virtual void imGuiPostUpdate(const DeltaTime& deltaTime) {};
		
		bool showDemoWindow_{};
	};

	inline bool ImGuiWindow::isShowDemoWindow() const {
		return showDemoWindow_;
	}

	inline void ImGuiWindow::setShowDemoWindow(bool show) {
		showDemoWindow_ = show;
	}

}

#endif
