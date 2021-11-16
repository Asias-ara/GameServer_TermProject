// stdafx.cpp 표준 포함 파일만 들어있는 소스 파일입니다.
#include "stdafx.h"

bool collide(const RECT& rect1, const RECT& rect2)
{
	if (rect1.left > rect2.right) return false;
	if (rect1.top > rect2.bottom) return false;
	if (rect1.right < rect2.left) return false;
	if (rect1.bottom < rect2.top) return false;

	return true;
}

void CharToWChar(const char* pstrSrc, wchar_t pwstrDest[])
{
	int nLen = (int)strlen(pstrSrc) + 1;
	mbstowcs(pwstrDest, pstrSrc, nLen);
}

void Normalize(const POINT& start, const POINT& end, float& dx, float& dy)
{
	POINT dir{ end.x - start.x,end.y - start.y };
	
	float temp = sqrt(dir.x * dir.x + dir.y * dir.y);
	dx = dir.x / temp;
	dy = dir.y / temp;
	
}
