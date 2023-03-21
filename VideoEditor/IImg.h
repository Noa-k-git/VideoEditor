#pragma once

class IImg {
public:
	int resolution[2];
	IImg() { resolution[0] = 0; resolution[1] = 0; }
	virtual ~IImg() {}
	void virtual Show() = 0;
};