#pragma once
#include <shared_mutex>
/// <summary>
/// A template class for synchronizing an object between two threads
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
class SyncObject
{
public:
	SyncObject(T obj) {
		m_object = obj;
		std::shared_mutex m_objectMutex;

	}
	// copy constructor
	SyncObject(const SyncObject& other) {
		this->m_object = other.m_object;
		std::shared_mutex m_objectMutex;
	}
	void SetObject(T obj) {
		std::unique_lock<std::shared_mutex> lock(m_objectMutex);
		m_object = obj;
	}
	T GetObject() {
		// waits for the source to be available
		std::shared_lock<std::shared_mutex> lock(m_objectMutex);
		// returns source
		return m_object;
	}

	std::pair<std::unique_lock<std::shared_mutex>, T> GetObjectForUpdate() {
		// waits for the source to be available
		std::unique_lock<std::shared_mutex> lock(m_objectMutex);
		// returns lock&source
		return std::make_pair(std::move(lock), std::ref(m_object));
	}
private:
	std::shared_mutex m_objectMutex;
	T m_object;
};

