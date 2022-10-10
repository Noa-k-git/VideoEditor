#pragma once

class IPlayable {
public:
	int fps;
	int length;
	IPlayable() { length = 0; fps = 30; }
	virtual ~IPlayable() {}
	void virtual Play() = 0;
};