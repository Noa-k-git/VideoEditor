#pragma once
#include <shared_mutex>

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
		std::shared_lock<std::shared_mutex> lock(m_objectMutex);
		return m_object;
	}

	std::pair<std::unique_lock<std::shared_mutex>, T> GetObjectForUpdate() {
		std::unique_lock<std::shared_mutex> lock(m_objectMutex);
		return std::make_pair(std::move(lock), std::ref(m_object));
	}
private:
	std::shared_mutex m_objectMutex;
	T m_object;
};

