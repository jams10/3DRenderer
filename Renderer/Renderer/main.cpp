
/*
 *	TODO : 사용 라이브러리 기입.
 */

#include "Application/RenderApp.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
 // Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
 // allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif


int main()
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		NS::RenderApp app(1280, 720);

		if (app.Initialize() == false)
			return -1;

		while (app.Run() != 0)
		{}
	}
	//system("PAUSE");

	return 0;
}