#pragma once
#include "SyncObject.h"
#include <string>
/// <summary>
/// An anbstruct class that defines a playable properties
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class IPlayable {
public:
	IPlayable(const std::string& name) : name(name) {}
	virtual SyncObject<T>* GetChunk(int) = 0;
	virtual int GetSize() = 0;
	virtual ~IPlayable() {};
//	virtual void Play() = 0;

	const std::string& GetName() const { return name; }
	void SetName( std::string name) { this->name = name; }

private:
	std::string name;

};
