#pragma once
#include <Windows.h>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>


//using ExceptionInfoList = std::vector<ExceptionInfo>;

enum class HwbkpType
{
	Execute,
	ReadWrite,
	Write
};

enum class HwbkpSize
{
	Size_1,
	Size_2,
	Size_8,
	Size_4
};

enum class HandlerType
{
	VectoredExceptionHandler,
	UnhandledExceptionFilter,
	VectoredExceptionHandlerIntercept,
	KiUserExceptionDispatcherHook,
};

enum class BkpType
{
	Hardware,
};

struct BkpInfo
{
	BkpType Type = BkpType::Hardware;
	int Pos = 0;
};
	
struct CatchedDetails
{
	size_t Count		= 0;
	size_t ThreadId		= 0;
	CONTEXT Ctx			= {};
};

struct ExceptionInfo
{
	CatchedDetails Details{};
};

using ExceptionInfoList = std::map<uintptr_t, ExceptionInfo>;

namespace VExDebugger
{

	bool Init( HandlerType Type = HandlerType::VectoredExceptionHandler, bool SpoofHwbkp = false, bool Logs = false );

	std::map<uintptr_t, ExceptionInfoList>& GetAssocExceptionList( );

	std::map<uintptr_t, BkpInfo>& GetBreakpointList( );

	bool StartMonitorAddress( uintptr_t Address, HwbkpType Type, HwbkpSize Size );
	
	void RemoveMonitorAddress( uintptr_t Address );

	template <typename T>
	inline bool StartMonitorAddress( T Address, HwbkpType Type, HwbkpSize Size )
	{
		return StartMonitorAddress( (uintptr_t)Address, Type, Size );
	}
}