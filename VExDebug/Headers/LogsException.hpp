#pragma once
#include "Header.h"

inline void DisplayContextLogs(PCONTEXT ContextRecord, PEXCEPTION_RECORD pExceptionRecord )
{
	log_file( "===> ExceptionAddress 0x%p, Code: %X\n", pExceptionRecord->ExceptionAddress, pExceptionRecord->ExceptionCode );
#ifdef _M_X64
	log_file(" r8[%I64X]\n r9[%I64X]\nr10[%I64X]\nr11[%I64X]\nr12[%I64X]\nr13[%I64X]\nr14[%I64X]\nr15[%I64X]\n"
		"rax[%I64X]\nrbx[%I64X]\nrcx[%I64X]\nrdx[%I64X]\nrbp[%I64X]\nrsi[%I64X]\nrsp[%I64X]\nrdi[%I64X]\n",
		ContextRecord->R8,
		ContextRecord->R9,
		ContextRecord->R10,
		ContextRecord->R11,
		ContextRecord->R12,
		ContextRecord->R13,
		ContextRecord->R14,
		ContextRecord->R15,
		ContextRecord->Rax,
		ContextRecord->Rbx,
		ContextRecord->Rcx,
		ContextRecord->Rdx,
		ContextRecord->Rbp,
		ContextRecord->Rsi,
		ContextRecord->Rsp,
		ContextRecord->Rdi
	);

	log_file("\nP1Home 0x%I64X\nP2Home 0x%I64X\nP3Home 0x%I64X\nP4Home 0x%I64X\nP5Home 0x%I64X\nP6Home 0x%I64X\n",
		ContextRecord->P1Home,
		ContextRecord->P2Home,
		ContextRecord->P3Home,
		ContextRecord->P4Home,
		ContextRecord->P5Home,
		ContextRecord->P6Home
	);

	log_file("\nDr0 0x%I64X\nDr1 0x%I64X\nDr2 0x%I64X\nDr3 0x%I64X\nDr6 0x%I64X\nDr7 0x%I64X\n",
		ContextRecord->Dr0,
		ContextRecord->Dr1,
		ContextRecord->Dr2,
		ContextRecord->Dr3,
		ContextRecord->Dr6,
		ContextRecord->Dr7
	);
	log_file("\nContextFlags 0x%X\nVectorControl 0x%I64X\n",
		ContextRecord->ContextFlags,
		ContextRecord->VectorControl
	);

	log_file("\nDebugControl 0x%I64X\nLastBranchToRip 0x%I64X\nLastBranchFromRip 0x%I64X\nLastExceptionToRip 0x%I64X\nLastExceptionFromRip 0x%I64X\n",
		ContextRecord->DebugControl,
		ContextRecord->LastBranchToRip,
		ContextRecord->LastBranchFromRip,
		ContextRecord->LastExceptionToRip,
		ContextRecord->LastExceptionFromRip
	);
	for (uint32_t i = 0; i < pExceptionRecord->NumberParameters; ++i)
	{
		log_file("ExceptionInformation[%d] == %I64X\n", i, pExceptionRecord->ExceptionInformation[i]);
	}
#else
	log_file(" Edi[%lX]\nEsi[%lX]\nEbx[%lX]\nEdx[%lX]\nEcx[%lX]"
		"\nEax[%lX]\nEbp[%lX]\nEip[%lX]\nEsp[%lX]\n",
		ContextRecord->Edi,
		ContextRecord->Esi,
		ContextRecord->Ebx,
		ContextRecord->Edx,
		ContextRecord->Ecx,
		ContextRecord->Eax,
		ContextRecord->Ebp,
		ContextRecord->Eip,
		ContextRecord->Esp
	);
	log_file("\nDr0 0x%lX\nDr1 0x%lX\nDr2 0x%lX\nDr3 0x%lX\nDr6 0x%lX\nDr7 0x%lX\n",
		ContextRecord->Dr0,
		ContextRecord->Dr1,
		ContextRecord->Dr2,
		ContextRecord->Dr3,
		ContextRecord->Dr6,
		ContextRecord->Dr7
	);
	for (uint32_t i = 0; i < pExceptionRecord->NumberParameters; ++i)
	{
		log_file("ExceptionInformation[%d] == %luX\n", i, pExceptionRecord->ExceptionInformation[i]);
	}
#endif
	log_file("\nSegCs %lu \nSegDs %lu\nSegEs %lu\nSegFs %lu\nSegGs %lu\nSegSs %lu\nEFlags %lu\n",
		ContextRecord->SegCs,
		ContextRecord->SegDs,
		ContextRecord->SegEs,
		ContextRecord->SegFs,
		ContextRecord->SegGs,
		ContextRecord->SegSs,
		ContextRecord->EFlags
	);
	log_file("\nContextFlags 0x%lX\n",
		ContextRecord->ContextFlags
	);
	log_file(
		"======================================================="
		"\n");

}
