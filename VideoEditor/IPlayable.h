#pragma once

class IPlayable {
public:
	int length;
	int fps;
	IPlayable() { length = 0; fps = 30; }
	virtual ~IPlayable() {}
	void virtual Play() = 0;
};