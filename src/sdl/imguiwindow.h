#ifndef CPPSDL2_IMGUIWINDOW_H
#define CPPSDL2_IMGUIWINDOW_H

#include "window.h"
#include "shader.h"
#include "texture.h"
#include "vertexbufferobject.h"
#include "vertexarrayobject.h"

#include <imgui.h>
#include <array>

#include <glm/glm.hpp>

namespace ImGui {

	template <class T>
	bool Window(const char* name, bool* p_open, ImGuiWindowFlags flags, T&& t) {
		bool success = ImGui::Begin(name, p_open, flags);
		if (success) {
			t();
		}
		ImGui::End();
		return success;
	}

	template <class T>
	bool Window(const char* name, bool* p_open, T&& t) {
		bool success = ImGui::Begin(name, p_open);
		if (success) {
			t();
		}
		ImGui::End();
		return success;
	}

	template <class T>
	bool Window(const char* name, T&& t) {
		bool success = ImGui::Begin(name);
		if (success) {
			t();
		}
		ImGui::End();
		return success;
	}

}

namespace sdl {

	class ImGuiWindow : public sdl::Window {
	public:
		ImGuiWindow() = default;

		ImGuiWindow(int majorVersionGl, int minorVersionGl);

		virtual ~ImGuiWindow();

		bool isShowDemoWindow() const;
		void setShowDemoWindow(bool show);

	protected:
		using Canvas = std::function<void()>;

		void imGuiCanvas(const glm::vec2& size, Canvas&& canvas);
		void imGuiCanvas(Canvas&& canvas);

		virtual void initPreLoop() override;

		virtual void eventUpdate(const SDL_Event& windowEvent) override;

		const Shader& getShader() const;

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

		virtual void update(const std::chrono::high_resolution_clock::duration& deltaTime) override final;

		virtual void imGuiPreUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {};
		virtual void imGuiUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {};
		virtual void imGuiPostUpdate(const std::chrono::high_resolution_clock::duration& deltaTime) {};

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
		void ImGui_ImplOpenGL3_SetupRenderState(ImDrawData* drawData, int fbWidth, int fbHeight);
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

		struct CanvasData {
			Canvas canvas;
			glm::vec2 pos;
			glm::vec2 size;
		};
		std::vector<CanvasData> imGuiCanvases_;

		GLState glState_{};
		std::array<bool, 3> mousePressed_{};
		std::array<SDL_Cursor*, ImGuiMouseCursor_COUNT> mouseCursors_{};

		sdl::VertexArrayObject vao_;
		VertexBufferObject imGuiVbo_;
		VertexBufferObject imGuiElementsVbo_;

		sdl::Shader shader_;
		
		bool showDemoWindow_{};
		bool initiatedOpenGl_{};
		bool initiatedSdl_{};
        
		char* clipboardTextData_{};
		Texture imGuiFontTexture_;
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
