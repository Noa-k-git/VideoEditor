#pragma once
#include "SyncObject.h"
template <typename T>
class IPlayable {
public:
	virtual SyncObject<T>& GetChunk(int) = 0;
	virtual int GetSize() = 0;
	virtual ~IPlayable() {};
	virtual void Play() = 0;
};