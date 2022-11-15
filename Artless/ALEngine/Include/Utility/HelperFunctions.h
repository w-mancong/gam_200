/*!
file:	HelperFunctions.h
author:	Wong Man Cong
email:	w.mancong@digipen.edu
brief:	This file contain useful functions declarations to help ease with the process of coding

		All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*//*__________________________________________________________________________________*/
#ifndef	HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#include "Utility/Type.h"
namespace ALEngine::Utility
{
	/*!*********************************************************************************
		\brief
		Helper function to check if two floats are equal within an epsilon range

		\param [in] a:
		First float to be check with
		\param [in] b:
		Second float to be check with

		\return
		True if both a and b are equal within the epsilon range, else false
	***********************************************************************************/
	bool IsEqual(f32 a, f32 b);

	/*!*********************************************************************************
		\brief Generic function to read in bytes from binary file

		\param [in] filePath: Path to the file to be read
		\param [out] outSize: Used to store the byte size of the file read

		\return Buffer to the bytes read from file
	***********************************************************************************/
	c8* ReadBytes(std::filesystem::path const& filePath, u32* outSize = nullptr);

	template <typename T>
	using Ref = std::shared_ptr<T>;

	/*!*********************************************************************************
		\brief To create a share pointer of type T
		
		\param [in] args: to construct T with variadic arguments

		\return A shared pointer of type T
	***********************************************************************************/
	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#endif