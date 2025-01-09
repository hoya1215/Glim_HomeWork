#pragma once

struct CFPoint
{
	float x;
	float y;

	bool operator==(CFPoint point)
	{
		if (this->x == point.x && this->y == point.y)
			return true;
		else
			return false;
	}

	void operator+=(CFPoint point)
	{
		this->x += point.x;
		this->y += point.y;
	}
};