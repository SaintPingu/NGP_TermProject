#pragma once

template<typename T1, typename T2>
void Safe_Delete_Map(std::map<T1, T2>& _map)
{
	for (auto& pair : _map)
	{
		SAFE_DELETE(pair.second);
	}

	_map.clear();
}

template<typename T, int iSize>
void Safe_Delete_Array(T* (&_arr)[iSize])
{
	for (UINT i = 0; i < iSize; ++i)
	{
		SAFE_DELETE(_arr[i]);
	}
}

template<typename T>
void Safe_Delete_Vector(std::vector<T*> _vec)
{
	for (UINT i = 0; i < _vec.size(); ++i)
	{
		SAFE_DELETE(_vec[i]);
	}
}


