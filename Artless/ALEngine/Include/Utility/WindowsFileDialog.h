/*!
file:	WindowsFileDialog.h
author: Lucas Nguyen
email:	l.nguyen@digipen.edu
brief:	This file contains function declarations for the WindowsFileDialog class.
		The WindowsFileDialog class helps to call the Windows API File Dialogs that
		helps with saving and loading of specific files.

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef WINDOWS_FILE_DIALOG_H
#define WINDOWS_FILE_DIALOG_H

namespace ALEngine::Utility
{
	/*!*********************************************************************************
		\brief
		This is the WIndowsFileDialog class that contains static functions for Loading
		and Saving files.
	***********************************************************************************/
	class WindowsFileDialog
	{
	public:
		/*!*********************************************************************************
			\brief
			Opens a Windows File Dialog that loads a file
			\param filter
			The extensions that are accepted by this function.
			i.e. .jpg, .png, .wav, etc.
			\return
			Returns the file path to the selected file
			If cancelled, return std::string()
		***********************************************************************************/
		static std::string LoadFile(const c8* filter);

		/*!*********************************************************************************
			\brief
			Opens a Windows File Dialog that saves a file
			\param filter
			The extensions that are accepted by this function.
			i.e. .jpg, .png, .wav, etc.
			\return
			Returns the file path to the selected file
			If cancelled, return std::string()
		***********************************************************************************/
		static std::string SaveFile(const c8* filter);
	};
}

#endif