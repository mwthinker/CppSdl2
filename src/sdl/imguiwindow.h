#ifndef CPPSDL2_IMGUIWINDOW_H
#define CPPSDL2_IMGUIWINDOW_H

#include "window.h"
#include "imguishader.h"
#include "vertexbufferobject.h"
#include "vertexarrayobject.h"

#include <imgui.h>
#include <array>

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

		virtual void eventUpdate(const SDL_Event& windowEvent) override;

		const ImGuiShader& getImGuiShader() const;

	private:
        virtual void update(const std::chrono::high_resolution_clock::duration& deltaTime) override final;

        virtual void imGuiPreUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {};
        virtual void imGuiPostUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {};
        virtual void imGuiUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {};

		struct GLState {
			GLenum lastActiveTexture = 0;

			GLint lastProgram = 0;
			GLint lastTexture = 0;
#ifdef GL_SAMPLER_BINDING
			GLint lastSampler = 0;
#endif
			GLint lastArrayBuffer = 0;
			GLint lastVertexArray = 0;
#ifdef GL_POLYGON_MODE
			GLint lastPolygonMode[2] = {0, 0};
#endif
			GLint lastViewport[4] = {0, 0, 0, 0};
			GLint lastScissorBox[4] = {0, 0, 0, 0};
			GLenum lastBlendSrcRgb = 0;
			GLenum lastBlendDstRgb = 0;
			GLenum lastBlendSrcAlpha = 0;
			GLenum lastBlendDstAlpha = 0;
			GLenum lastBlendEquationRgb = 0;
			GLenum lastBlendEquationAlpha = 0;
			GLboolean lastEnableBlend = GL_FALSE;
			GLboolean lastEnableCullFace = GL_FALSE;
			GLboolean lastEnableDepthTest = GL_FALSE;
			GLboolean lastEnableScissorTest = GL_FALSE;
			bool clipOriginLowerLeft = false;
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
			GLenum lastClipOrigin = 0;
#endif
		};

		// ImGui SDL2 specific code.
		bool ImGui_ImplSDL2_ProcessEvent(const SDL_Event& sdlEvent);
		void ImGui_ImplSDL2_Init();
		void ImGui_ImplSDL2_Shutdown();
		void ImGui_ImplSDL2_NewFrame(const std::chrono::nanoseconds& deltaTime);

		// ImGui OpenGL3 specific code.
		void ImGui_ImplOpenGL3_Init();
		void ImGui_ImplOpenGL3_Shutdown();
		void ImGui_ImplOpenGL3_NewFrame();
		void ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* drawData);
		void ImGui_ImplSDL2_UpdateMousePosAndButtons();
		const char* ImGui_ImplSDL2_GetClipboardText();
		void ImGui_ImplSDL2_UpdateMouseCursor();

		// ImGui specific code.
		void ImGui_ImplOpenGL3_CreateFontsTexture();
		void ImGui_ImplOpenGL3_DestroyFontsTexture();
		void ImGui_ImplOpenGL3_CreateDeviceObjects();
		void ImGui_ImplOpenGL3_DestroyDeviceObjects();
		void backupGlState();
		void restoreGlState();

		void setupVao();

		GLState glState_;
		std::array<bool, 3> mousePressed_{};
		std::array<SDL_Cursor*, ImGuiMouseCursor_COUNT> mouseCursors_{}; // Initiate in constructor.

		sdl::VertexArrayObject vao_;
		VertexBufferObject imGuiVbo_;
		VertexBufferObject imGuiElementsVbo_;

		sdl::ImGuiShader shader_;
		
		bool showDemoWindow_ = true;
		bool initiatedOpenGl_ = false;
		bool initiatedSdl_ = false;
        
		char* clipboardTextData_ = nullptr;
		GLuint imGuiFontTexture_ = 0;
	};

	inline bool ImGuiWindow::isShowDemoWindow() const {
		return showDemoWindow_;
	}

	inline void ImGuiWindow::setShowDemoWindow(bool show) {
		showDemoWindow_ = show;
	}

    inline const ImGuiShader& ImGuiWindow::getImGuiShader() const {
	    return shader_;
	}

} // Namespace sdl.

#endif // CPPSDL2_IMGUIWINDOW_H
