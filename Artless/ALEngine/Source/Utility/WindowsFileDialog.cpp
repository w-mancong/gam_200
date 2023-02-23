/*!
file:	WindowsFileDialog.cpp
author: Lucas Nguyen
email:	l.nguyen\@digipen.edu
brief:	This file contains function definitions for the WindowsFileDialog class.
		The WindowsFileDialog class helps to call the Windows API File Dialogs that
		helps with saving and loading of specific files.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#include "pch.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32		// For GLFW Win32
#include <GLFW/glfw3native.h>

namespace ALEngine::Utility
{
	std::string WindowsFileDialog::LoadFile(const c8* filter)
	{
#if EDITOR
		Engine::FileWatcher::SetPause(true);
#endif
		// A stands for the ASCII OPENFILENAME,
			// Common Dialog box structure
		OPENFILENAMEA openFileName;
		// File
		c8 file[260] = { 0 };		

		// Init to 0
		ZeroMemory(&openFileName, sizeof(OPENFILENAME));

		// Set openFileName variables
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = glfwGetWin32Window(Graphics::OpenGLWindow::Window());
		openFileName.lpstrFile = file;
		openFileName.nMaxFile = sizeof(file);
		openFileName.lpstrFilter = filter;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;		// OFN_NOCHANGEDIR is cause this may change the working directory if this isn't here
		if (GetOpenFileNameA(&openFileName) == TRUE)
		{
#if EDITOR
			Engine::FileWatcher::SetPause(false);
#endif
			return openFileName.lpstrFile;
		}

#if EDITOR
		Engine::FileWatcher::SetPause(false);
#endif
		return std::string();
	}

	std::string WindowsFileDialog::SaveFile(const c8* filter)
	{
#if EDITOR
		Engine::FileWatcher::SetPause(true);
#endif
		// A stands for the ASCII OPENFILENAME,
			// Common Dialog box structure
		OPENFILENAMEA openFileName;
		// File
		c8 file[260] = { 0 };

		// Init to 0
		ZeroMemory(&openFileName, sizeof(OPENFILENAME));

		// Set openFileName variables
		openFileName.lStructSize = sizeof(OPENFILENAME);
		openFileName.hwndOwner = glfwGetWin32Window(Graphics::OpenGLWindow::Window());
		openFileName.lpstrFile = file;
		openFileName.nMaxFile = sizeof(file);
		openFileName.lpstrFilter = filter;
		openFileName.nFilterIndex = 1;
		openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;		// OFN_NOCHANGEDIR is cause this may change the working directory if this isn't here
		if (GetSaveFileNameA(&openFileName) == TRUE)
		{
#if EDITOR
			Engine::FileWatcher::SetPause(false);
#endif
			return openFileName.lpstrFile;
		}

#if EDITOR
		Engine::FileWatcher::SetPause(false);
#endif
		return std::string();
	}
}