#include "Mouse.h"

#include <algorithm>

namespace NS
{
	void Mouse::Move(WPARAM buttonState, int x, int y, int screenWidth, int screenHeight)
	{
        // ���콺 Ŀ���� ��ġ�� NDC�� ��ȯ��.
        // ���콺 Ŀ���� ���� ��� (0, 0), ���� �ϴ�(width-1, height-1)
        // NDC�� ���� �ϴ��� (-1, -1), ���� ���(1, 1)
        float ndcX = x * 2.0f / screenWidth - 1.0f;
        float ndcY = -y * 2.0f / screenHeight + 1.0f;

        // Ŀ���� ȭ�� ������ ������ ��� ���� ����.
        posX = std::clamp(ndcX, -1.0f, 1.0f);
        posY = std::clamp(ndcY, -1.0f, 1.0f);
	}
}