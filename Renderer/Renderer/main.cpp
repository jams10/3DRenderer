
/*
 *	TODO : ��� ���̺귯�� ����.
 */

#include "Application/RenderApp.h"

int main()
{
	NS::RenderApp app(1280, 720);

	if (app.Initialize() == false)
		return -1;

	return app.Run();
}