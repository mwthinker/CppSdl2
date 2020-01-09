#include "imguiwindow.h"
#include "logger.h"
#include "vertexarrayobject.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <SDL_syswm.h>

namespace sdl {

	namespace {		

		constexpr int getGlslVersion3(int minorVersion) {
			switch (minorVersion) {
				case 0:
					return 130;
				case 1:
					return 140;
				case 2:
					return 150;
				case 3:
					return 330;
			}
			return -1;
		}

		constexpr int getGlslVersion(int majorVersion, int minorVersion) {
			switch (majorVersion) {
				case 3:
					return getGlslVersion3(minorVersion);
			}
			return -1;
		}

		constexpr std::pair<const GLchar*, const GLchar*> getShaderGlsl(int glslVersion) {
			if (glslVersion == 330) {
				return {getImGuiVertexShaderGlsl_330(), getImGuiFragmentShaderGlsl_330()};
			}
			return {"", ""};
		}
		
	}

	ImGuiWindow::ImGuiWindow(int majorVersionGl, int minorVersionGl)
		: Window{majorVersionGl, minorVersionGl} {
	}

	ImGuiWindow::~ImGuiWindow() {
        if (clipboardTextData_) {
            SDL_free(clipboardTextData_);
        }

		if (initiatedOpenGl_) {
			ImGui_ImplOpenGL3_Shutdown();
		}
		if (initiatedSdl_) {
			ImGui_ImplSDL2_Shutdown();
			ImGui::DestroyContext();
		}
	}

	void ImGuiWindow::initOpenGl() {
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		Window::initOpenGl();
	}

	void ImGuiWindow::initPreLoop() {
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		ImGui_ImplSDL2_Init();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplOpenGL3_NewFrame();
	}

	void ImGuiWindow::update(const std::chrono::nanoseconds& deltaTime) {
        imGuiPreUpdate(deltaTime);

		ImGui_ImplSDL2_NewFrame(deltaTime);
		ImGui::NewFrame();
	
		if (showDemoWindow_) {
            ImGui::ShowDemoWindow(&showDemoWindow_);
        }

        imGuiUpdate(deltaTime);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        imGuiPostUpdate(deltaTime);
	}

	void ImGuiWindow::eventUpdate(const SDL_Event& windowEvent) {
		ImGui_ImplSDL2_ProcessEvent(windowEvent);

		switch (windowEvent.type) {
			case SDL_WINDOWEVENT:
				switch (windowEvent.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
                        glViewport(0, 0, windowEvent.window.data1, windowEvent.window.data2);
						break;
				}
				break;
		}
	}

	// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
	// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
	// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
	// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	// If you have multiple SDL events and some of them are not meant to be used by dear imgui, you may need to filter events based on their windowID field.
	bool ImGuiWindow::ImGui_ImplSDL2_ProcessEvent(const SDL_Event& sdlEvent) {
		ImGuiIO& io = ImGui::GetIO();
		switch (sdlEvent.type) {
			case SDL_MOUSEWHEEL: {
				if (sdlEvent.wheel.x > 0) io.MouseWheelH += 1;
				if (sdlEvent.wheel.x < 0) io.MouseWheelH -= 1;
				if (sdlEvent.wheel.y > 0) io.MouseWheel += 1;
				if (sdlEvent.wheel.y < 0) io.MouseWheel -= 1;
				return true;
			}
			case SDL_MOUSEBUTTONDOWN: {
				if (sdlEvent.button.button == SDL_BUTTON_LEFT) mousePressed_[0] = true;
				if (sdlEvent.button.button == SDL_BUTTON_RIGHT) mousePressed_[1] = true;
				if (sdlEvent.button.button == SDL_BUTTON_MIDDLE) mousePressed_[2] = true;
				return true;
			}
			case SDL_TEXTINPUT: {
				io.AddInputCharactersUTF8(sdlEvent.text.text);
				return true;
			}
			case SDL_KEYDOWN:
				// Fall through.
			case SDL_KEYUP: {
				int key = sdlEvent.key.keysym.scancode;
				IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
				io.KeysDown[key] = (sdlEvent.type == SDL_KEYDOWN);
				io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
				io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
				io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
				io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
				return true;
			}
		}
		return false;
	}

	void ImGuiWindow::ImGui_ImplSDL2_Init() {
		initiatedSdl_ = true;
		// Setup back-end capabilities flags
		auto& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)
		io.BackendPlatformName = "Platform: ImGui, Sdl and OpenGl.";

		// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
		io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
		io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
		io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
		io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
		io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
		io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
		io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
		io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
		io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
		io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
		io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
		io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
		io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
		io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
		io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

		io.SetClipboardTextFn = [](void* thisPointer, const char* text) {
            SDL_SetClipboardText(text);
		};
		io.GetClipboardTextFn = [](void* thisPointer) -> const char* {
			auto window = static_cast<ImGuiWindow*>(thisPointer);
			return window->ImGui_ImplSDL2_GetClipboardText();
		};
		
		io.ClipboardUserData = this;

		mouseCursors_[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		mouseCursors_[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
		mouseCursors_[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
		mouseCursors_[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
		mouseCursors_[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
		mouseCursors_[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
		mouseCursors_[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
		mouseCursors_[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

#ifdef _WIN32
		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(getSdlWindow(), &wmInfo);
		io.ImeWindowHandle = wmInfo.info.win.window;
#endif
	}

	void ImGuiWindow::ImGui_ImplSDL2_Shutdown() {
		// Destroy SDL mouse cursors.
		for (auto& cursor : mouseCursors_) {
			SDL_FreeCursor(cursor);
			cursor = nullptr;
		}
	}

	void ImGuiWindow::ImGui_ImplSDL2_NewFrame(const std::chrono::high_resolution_clock::duration& deltaTime) {
		auto& io = ImGui::GetIO();
		IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

		// Setup display size (every frame to accommodate for window resizing)
		auto [w, h] = getSize();
		auto [displayW, displayH] = getDrawableSize();		
		io.DisplaySize = { static_cast<float>(w), static_cast<float>(h) };
		if (w > 0 && h > 0) {
			io.DisplayFramebufferScale = {static_cast<float>(displayW) / w, static_cast<float>(displayH) / h };
		}
		io.DeltaTime = std::chrono::duration<float>(deltaTime).count();
		ImGui_ImplSDL2_UpdateMousePosAndButtons();
		ImGui_ImplSDL2_UpdateMouseCursor();
	}

	void ImGuiWindow::ImGui_ImplSDL2_UpdateMousePosAndButtons() {
		auto& io = ImGui::GetIO();

		// Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
		if (io.WantSetMousePos) {
			SDL_WarpMouseInWindow(getSdlWindow(), static_cast<int>(io.MousePos.x), static_cast<int>(io.MousePos.y));
		} else {
			io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
		}

		int mx, my;
		auto mouseButtons = SDL_GetMouseState(&mx, &my);
		io.MouseDown[0] = mousePressed_[0] || (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;  // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		io.MouseDown[1] = mousePressed_[1] || (mouseButtons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
		io.MouseDown[2] = mousePressed_[2] || (mouseButtons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
		mousePressed_[0] = mousePressed_[1] = mousePressed_[2] = false;

#if !defined(__EMSCRIPTEN__) && !defined(__ANDROID__) && !(defined(__APPLE__) && TARGET_OS_IOS)
		// SDL_GetMouseState() gives mouse position seemingly based on the last window entered/focused(?)
		// The creation of a new windows at runtime and SDL_CaptureMouse both seems to severely mess up with that, so we retrieve that position globally.
			
		auto [wx, wy] = getWindowPosition();
		SDL_GetGlobalMouseState(&mx, &my);
		mx -= wx;
		my -= wy;
		io.MousePos = {static_cast<float>(mx), static_cast<float>(my)};

		// SDL_CaptureMouse() let the OS know e.g. that our imgui drag outside the SDL window boundaries shouldn't e.g. trigger the OS window resize cursor.
		SDL_CaptureMouse(ImGui::IsAnyMouseDown()? SDL_TRUE : SDL_FALSE);
#else
		if (SDL_GetWindowFlags(g_Window) & SDL_WINDOW_INPUT_FOCUS) {
			io.MousePos = {static_cast<float>(mx), static_cast<float>(my)};
		}
#endif
	}

	void ImGuiWindow::ImGui_ImplOpenGL3_Init() {
		initiatedOpenGl_ = true;
		auto& io = ImGui::GetIO();
		io.BackendRendererName = "imgui_impl_opengl3";
	}

	void ImGuiWindow::ImGui_ImplOpenGL3_Shutdown() {
		ImGui_ImplOpenGL3_DestroyDeviceObjects();
	}

	void ImGuiWindow::ImGui_ImplOpenGL3_NewFrame() {
		if (!imGuiFontTexture_) {
			ImGui_ImplOpenGL3_CreateDeviceObjects();
		}
	}

	void ImGuiWindow::setupVao() {
		vao_.generate();
		vao_.bind();

		imGuiVbo_.bind(GL_ARRAY_BUFFER);
		imGuiElementsVbo_.bind(GL_ELEMENT_ARRAY_BUFFER);

		shader_.setVertexAttribPointer();
	}

	// OpenGL3 Render function.
	// (this used to be set in io.RenderDrawListsFn and called by ImGui::Render(), but you can now call this directly from your main loop)
	// Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly, in order to be able to run within any OpenGL engine that doesn't do so.
	void ImGuiWindow::ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* drawData) {
		// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
		auto fbWidth = static_cast<int>(drawData->DisplaySize.x * drawData->FramebufferScale.x);
		auto fbHeight = static_cast<int>(drawData->DisplaySize.y * drawData->FramebufferScale.y);
		if (fbWidth <= 0 || fbHeight <= 0) {
			return;
		}

		backupGlState();

		// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
#ifdef GL_POLYGON_MODE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

		// Setup viewport, orthographic projection matrix
		// Our visible imgui space lies from drawData->DisplayPos (top left) to drawData->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
		glViewport(0, 0, static_cast<GLsizei>(fbWidth), static_cast<GLsizei>(fbHeight));

		const auto projMatrix = glm::ortho(drawData->DisplayPos.x, drawData->DisplayPos.x + drawData->DisplaySize.x,
			drawData->DisplayPos.y + drawData->DisplaySize.y, drawData->DisplayPos.y);

        shader_.useProgram();
        shader_.setMatrix(projMatrix);
        shader_.setTextureId(0);

#ifdef GL_SAMPLER_BINDING
		glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
#endif

		vao_.bind();
		imGuiVbo_.bind(GL_ARRAY_BUFFER);
		imGuiElementsVbo_.bind(GL_ELEMENT_ARRAY_BUFFER);

		// Will project scissor/clipping rectangles into framebuffer space
		ImVec2 clipOff = drawData->DisplayPos;         // (0,0) unless using multi-viewports
		ImVec2 clipScale = drawData->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

		// Render command lists
		for (int n = 0; n < drawData->CmdListsCount; n++) {
			const ImDrawList* cmd_list = drawData->CmdLists[n];
			size_t idxBufferOffset = 0;

            imGuiVbo_.bufferData((GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);
            imGuiElementsVbo_.bufferData((GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback) {
					// User callback (registered via ImDrawList::AddCallback)
					pcmd->UserCallback(cmd_list, pcmd);
				} else {
					// Project scissor/clipping rectangles into framebuffer space
					ImVec4 clipRect;
					clipRect.x = (pcmd->ClipRect.x - clipOff.x) * clipScale.x;
					clipRect.y = (pcmd->ClipRect.y - clipOff.y) * clipScale.y;
					clipRect.z = (pcmd->ClipRect.z - clipOff.x) * clipScale.x;
					clipRect.w = (pcmd->ClipRect.w - clipOff.y) * clipScale.y;

					if (clipRect.x < fbWidth && clipRect.y < fbHeight && clipRect.z >= 0.0f && clipRect.w >= 0.0f) {
						// Apply scissor/clipping rectangle
						if (glState_.clipOriginLowerLeft) {
							glScissor((int) clipRect.x, (int) (fbHeight - clipRect.w), (int) (clipRect.z - clipRect.x), (int) (clipRect.w - clipRect.y));
						} else {
							glScissor((int) clipRect.x, (int) clipRect.y, (int) clipRect.z, (int) clipRect.w); // Support for GL 4.5's glClipControl(GL_UPPER_LEFT)
						}
						// Bind texture, Draw
						glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t) pcmd->TextureId);
						glDrawElements(GL_TRIANGLES, (GLsizei) pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*) idxBufferOffset);
					}
				}
				idxBufferOffset += pcmd->ElemCount * sizeof(ImDrawIdx);
			}
		}
		restoreGlState();
	}

	void ImGuiWindow::ImGui_ImplOpenGL3_CreateFontsTexture() {
		// Build texture atlas
		auto& io = ImGui::GetIO();
		unsigned char* pixels = nullptr;
		int width = 0, height = 0;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height); // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

		// Upload texture to graphics system
		GLint lastTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
		glGenTextures(1, &imGuiFontTexture_);
		glBindTexture(GL_TEXTURE_2D, imGuiFontTexture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef GL_UNPACK_ROW_LENGTH
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		// Store our identifier
		io.Fonts->TexID = (ImTextureID)(intptr_t) imGuiFontTexture_;

		// Restore state
		glBindTexture(GL_TEXTURE_2D, lastTexture);
	}

	void ImGuiWindow::ImGui_ImplOpenGL3_DestroyFontsTexture() {
		if (imGuiFontTexture_) {
			ImGuiIO& io = ImGui::GetIO();
			glDeleteTextures(1, &imGuiFontTexture_);
			io.Fonts->TexID = 0;
			imGuiFontTexture_ = 0;
		}
	}

	void ImGuiWindow::ImGui_ImplOpenGL3_CreateDeviceObjects() {
		// Backup GL state
		GLint lastTexture, lastArrayBuffer;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastArrayBuffer);

		// Parse GLSL version string
		auto glslVersion = getGlslVersion(sdl::Window::getOpenGlMajorVersion(), sdl::Window::getOpenGlMinorVersion());
		auto [vertexShader, fragmentShader] = getShaderGlsl(glslVersion);
		shader_ = sdl::Shader{vertexShader, fragmentShader};

		// Create buffers
        imGuiVbo_.generate();
        imGuiElementsVbo_.generate();

		setupVao();

		ImGui_ImplOpenGL3_CreateFontsTexture();

		// Restore modified GL state
		glBindTexture(GL_TEXTURE_2D, lastTexture);
		glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
	}

	void ImGuiWindow::ImGui_ImplOpenGL3_DestroyDeviceObjects() {
		shader_ = sdl::Shader{};
		ImGui_ImplOpenGL3_DestroyFontsTexture();
	}

	void ImGuiWindow::backupGlState() {
		// Backup GL state
		glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*) &glState_.lastActiveTexture);
		glActiveTexture(GL_TEXTURE0);
		glGetIntegerv(GL_CURRENT_PROGRAM, &glState_.lastProgram);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &glState_.lastTexture);
#ifdef GL_SAMPLER_BINDING
		glGetIntegerv(GL_SAMPLER_BINDING, &glState_.lastSampler);
#endif
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &glState_.lastArrayBuffer);
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &glState_.lastVertexArray);
#ifdef GL_POLYGON_MODE
		glGetIntegerv(GL_POLYGON_MODE, glState_.lastPolygonMode);
#endif
		glGetIntegerv(GL_VIEWPORT, glState_.lastViewport);
		glGetIntegerv(GL_SCISSOR_BOX, glState_.lastScissorBox);
		glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*) &glState_.lastBlendSrcRgb);
		glGetIntegerv(GL_BLEND_DST_RGB, (GLint*) &glState_.lastBlendDstRgb);
		glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*) &glState_.lastBlendSrcAlpha);
		glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*) &glState_.lastBlendDstAlpha);
		glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*) &glState_.lastBlendEquationRgb);
		glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*) &glState_.lastBlendEquationAlpha);
		glState_.lastEnableBlend = glIsEnabled(GL_BLEND);
		glState_.lastEnableCullFace = glIsEnabled(GL_CULL_FACE);
		glState_.lastEnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
		glState_.lastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);
		glState_.clipOriginLowerLeft = true;
#if defined(GL_CLIP_ORIGIN) && !defined(__APPLE__)
		glState_.lastClipOrigin = 0; glGetIntegerv(GL_CLIP_ORIGIN, (GLint*) &glState_.lastClipOrigin); // Support for GL 4.5's glClipControl(GL_UPPER_LEFT)
		if (glState_.lastClipOrigin == GL_UPPER_LEFT) {
			glState_.clipOriginLowerLeft = false;
		}
#endif
	}

	void ImGuiWindow::restoreGlState() {
		// Restore modified GL state
		glUseProgram(glState_.lastProgram);
		glBindTexture(GL_TEXTURE_2D, glState_.lastTexture);
#ifdef GL_SAMPLER_BINDING
		glBindSampler(0, glState_.lastSampler);
#endif
		glActiveTexture(glState_.lastActiveTexture);

		glBindVertexArray(glState_.lastVertexArray);

		glBindBuffer(GL_ARRAY_BUFFER, glState_.lastArrayBuffer);
		glBlendEquationSeparate(glState_.lastBlendEquationRgb, glState_.lastBlendEquationAlpha);
		glBlendFuncSeparate(glState_.lastBlendSrcRgb, glState_.lastBlendDstRgb, glState_.lastBlendSrcAlpha, glState_.lastBlendDstAlpha);
		if (glState_.lastEnableBlend) {
			glEnable(GL_BLEND);
		} else {
			glDisable(GL_BLEND);
		}

		if (glState_.lastEnableCullFace) {
			glEnable(GL_CULL_FACE);
		} else {
			glDisable(GL_CULL_FACE);
		}
		if (glState_.lastEnableDepthTest) {
			glEnable(GL_DEPTH_TEST);
		} else {
			glDisable(GL_DEPTH_TEST);
		}
		if (glState_.lastEnableScissorTest) {
			glEnable(GL_SCISSOR_TEST);
		} else {
			glDisable(GL_SCISSOR_TEST);
		}

#ifdef GL_POLYGON_MODE
		glPolygonMode(GL_FRONT_AND_BACK, (GLenum) glState_.lastPolygonMode[0]);
#endif

		glViewport(glState_.lastViewport[0], glState_.lastViewport[1], (GLsizei) glState_.lastViewport[2], (GLsizei) glState_.lastViewport[3]);
		glScissor(glState_.lastScissorBox[0], glState_.lastScissorBox[1], (GLsizei) glState_.lastScissorBox[2], (GLsizei) glState_.lastScissorBox[3]);
	}

	const char* ImGuiWindow::ImGui_ImplSDL2_GetClipboardText() {
		if (clipboardTextData_) {
			SDL_free(clipboardTextData_);
		}
		clipboardTextData_ = SDL_GetClipboardText();
		return clipboardTextData_;
	}

	void ImGuiWindow::ImGui_ImplSDL2_UpdateMouseCursor() {
		auto& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) {
			return;
		}

		auto imGuiCursor = ImGui::GetMouseCursor();
		if (io.MouseDrawCursor || imGuiCursor == ImGuiMouseCursor_None) {
			// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
			SDL_ShowCursor(SDL_FALSE);
		} else {
			// Show OS mouse cursor
			SDL_SetCursor(mouseCursors_[imGuiCursor] ? mouseCursors_[imGuiCursor] : mouseCursors_[ImGuiMouseCursor_Arrow]);
			SDL_ShowCursor(SDL_TRUE);
		}
	}

} // Namespace sdl.
