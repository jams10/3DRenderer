#include "Mouse.h"

#include <algorithm>

namespace NS
{
	void Mouse::Move(WPARAM buttonState, int x, int y, int screenWidth, int screenHeight)
	{
        // 마우스 커서의 위치를 NDC로 변환함.
        // 마우스 커서는 좌측 상단 (0, 0), 우측 하단(width-1, height-1)
        // NDC는 좌측 하단이 (-1, -1), 우측 상단(1, 1)
        float ndcX = x * 2.0f / screenWidth - 1.0f;
        float ndcY = -y * 2.0f / screenHeight + 1.0f;

        // 커서가 화면 밖으로 나갔을 경우 범위 조절.
        posX = std::clamp(ndcX, -1.0f, 1.0f);
        posY = std::clamp(ndcY, -1.0f, 1.0f);
	}
}