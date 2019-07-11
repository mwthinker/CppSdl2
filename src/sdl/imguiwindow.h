#ifndef CPPSDL2_IMGUIWINDOW_H
#define CPPSDL2_IMGUIWINDOW_H

#include "window.h"
#include "imguishader.h"

#include <imgui.h>

namespace sdl {

	class ImGuiWindow : public sdl::Window {
	public:
		ImGuiWindow();
		virtual ~ImGuiWindow();

		bool isShowDemoWindow() const;
		void setShowDemoWindow(bool show);

	protected:
		virtual void initOpenGl() override;

		virtual void initPreLoop() override;

		virtual void update(double deltaTime) override;

		virtual void eventUpdate(const SDL_Event& windowEvent) override;

	private:
		struct GLState {
			GLenum lastActiveTexture;

			GLint lastProgram;
			GLint lastTexture;
#ifdef GL_SAMPLER_BINDING
			GLint lastSampler;
#endif
			GLint lastArrayBuffer;
			GLint lastVertexArray;
#ifdef GL_POLYGON_MODE
			GLint lastPolygonMode[2];
#endif
			GLint lastViewport[4];
			GLint lastScissorBox[4];
			GLenum lastBlendSrcRgb;
			GLenum lastBlendDstRgb;
			GLenum lastBlendSrcAlpha;
			GLenum lastBlendDstAlpha;
			GLenum lastBlendEquationRgb;
			GLenum lastBlendEquationAlpha;
			GLboolean lastEnableBlend;
			GLboolean lastEnableCullFace;
			GLboolean lastEnableDepthTest;
			GLboolean lastEnableScissorTest;
			bool clipOriginLowerLeft;
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
			GLenum lastClipOrigin;
#endif
		};

		// ImGui SDL2 specific code.
		bool ImGui_ImplSDL2_ProcessEvent(const SDL_Event& sdlEvent);
		bool ImGui_ImplSDL2_Init();
		void ImGui_ImplSDL2_Shutdown();
		void ImGui_ImplSDL2_NewFrame(double deltaTime);

		// ImGui OpenGL3 specific code.
		bool ImGui_ImplOpenGL3_Init();
		void ImGui_ImplOpenGL3_Shutdown();
		void ImGui_ImplOpenGL3_NewFrame();
		void ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);
		void ImGui_ImplSDL2_UpdateMousePosAndButtons();
		const char* ImGui_ImplSDL2_GetClipboardText();
		void ImGui_ImplSDL2_SetClipboardText(const char* text);
		void ImGui_ImplSDL2_UpdateMouseCursor();

		// ImGui specific code.
		bool ImGui_ImplOpenGL3_CreateFontsTexture();
		void ImGui_ImplOpenGL3_DestroyFontsTexture();
		bool ImGui_ImplOpenGL3_CreateDeviceObjects();
		void ImGui_ImplOpenGL3_DestroyDeviceObjects();
		void backupGlState();
		void restoreGlState();

		bool showDemoWindow_;
		float menuHeight_;
		bool initiatedOpenGl_;
		bool initiatedSdl_;

		bool mousePressed_[3];
		
		GLState glState_;
		GLuint imGuiFontTexture_;
		GLuint imGuiVboHandle_;
		GLuint imGuiElementsHandle_;
		
		sdl::ImGuiShader shader;
		
		SDL_Cursor* mouseCursors_[ImGuiMouseCursor_COUNT];
		char* clipboardTextData_;
	};

	inline bool ImGuiWindow::isShowDemoWindow() const {
		return showDemoWindow_;
	}

	inline void ImGuiWindow::setShowDemoWindow(bool show) {
		showDemoWindow_ = show;
	}

} // Namespace tetris.

#endif // CPPSDL2_IMGUIWINDOW_H
