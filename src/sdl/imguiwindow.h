#ifndef CPPSDL2_IMGUIWINDOW_H
#define CPPSDL2_IMGUIWINDOW_H

#include "window.h"
#include "shader.h"
#include "vertexbufferobject.h"
#include "vertexarrayobject.h"

#include <imgui.h>
#include <array>

namespace sdl {

	class ImGuiWindow : public sdl::Window {
	public:
		ImGuiWindow() = default;
		virtual ~ImGuiWindow();

		bool isShowDemoWindow() const;
		void setShowDemoWindow(bool show);

	protected:
		virtual void initOpenGl() override;

		virtual void initPreLoop() override;

		virtual void eventUpdate(const SDL_Event& windowEvent) override;

		const Shader& getShader() const;

	private:
        virtual void update(const std::chrono::high_resolution_clock::duration& deltaTime) override final;

        virtual void imGuiPreUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {};
        virtual void imGuiPostUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {};
        virtual void imGuiUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {};

		struct GLState {
			GLenum lastActiveTexture{};

			GLint lastProgram{};
			GLint lastTexture{};
#ifdef GL_SAMPLER_BINDING
			GLint lastSampler{};
#endif
			GLint lastArrayBuffer{};
			GLint lastVertexArray{};
#ifdef GL_POLYGON_MODE
			GLint lastPolygonMode[2]{};
#endif
			GLint lastViewport[4]{};
			GLint lastScissorBox[4]{};
			GLenum lastBlendSrcRgb{};
			GLenum lastBlendDstRgb{};
			GLenum lastBlendSrcAlpha{};
			GLenum lastBlendDstAlpha{};
			GLenum lastBlendEquationRgb{};
			GLenum lastBlendEquationAlpha{};
			GLboolean lastEnableBlend{GL_FALSE};
			GLboolean lastEnableCullFace{GL_FALSE};
			GLboolean lastEnableDepthTest{GL_FALSE};
			GLboolean lastEnableScissorTest{GL_FALSE};
			bool clipOriginLowerLeft{};
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
			GLenum lastClipOrigin{};
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

		sdl::Shader shader_;
		
		bool showDemoWindow_{};
		bool initiatedOpenGl_{};
		bool initiatedSdl_{};
        
		char* clipboardTextData_{};
		GLuint imGuiFontTexture_{};
	};

	inline bool ImGuiWindow::isShowDemoWindow() const {
		return showDemoWindow_;
	}

	inline void ImGuiWindow::setShowDemoWindow(bool show) {
		showDemoWindow_ = show;
	}

    inline const Shader& ImGuiWindow::getShader() const {
	    return shader_;
	}

} // Namespace sdl.

#endif // CPPSDL2_IMGUIWINDOW_H
