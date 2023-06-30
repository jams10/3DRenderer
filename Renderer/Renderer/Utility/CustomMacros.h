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
	// 인위적으로 crash를 발생 시킴.
	#define CRASH(cause)						\
	{											\
		int* crash = nullptr;				    \
		__analysis_assume(crash != nullptr);	\
	    *crash = 0xDEAFBEEF;				    \
	}

	// 인자로 넘겨준 표현식이 잘못된 경우 crash.
	#define ASSERT_CRASH(expr)					\
	{											\
		if(!(expr))								\
		{										\
			CRASH("ASSERT_CRASH");				\
			__analysis_assume(expr);			\
		}										\
	}											\

}