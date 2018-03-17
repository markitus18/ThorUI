#ifndef __MTOV_H__
#define __MTOV_H__

template<int Cols>

class MToV
{
	float v[Cols];

public:
	THORUI_API float operator[](int col) const
	{
		return v[col];
	}

	THORUI_API float &operator[](int col)
	{
		return v[col];
	}
};


#endif // !__MTOV_H__

