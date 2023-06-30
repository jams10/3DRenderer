#pragma once

#define NS Renderer

namespace NS
{
	#define __ERRORLINE__ cout << "Line : " << __LINE__ << " / File : " << __FILE__ << '\n';
	#define SAFE_RELEASE(p) if(p) { delete p; p = nullptr; }
	#define THROWFAILED(x) ThrowIfFailed(x)

	/*-----------------
	       CRASH
	-----------------*/
	// ���������� crash�� �߻� ��Ŵ.
	#define CRASH(cause)						\
	{											\
		int* crash = nullptr;				    \
		__analysis_assume(crash != nullptr);	\
	    *crash = 0xDEAFBEEF;				    \
	}

	// ���ڷ� �Ѱ��� ǥ������ �߸��� ��� crash.
	#define ASSERT_CRASH(expr)					\
	{											\
		if(!(expr))								\
		{										\
			CRASH("ASSERT_CRASH");				\
			__analysis_assume(expr);			\
		}										\
	}											\

}