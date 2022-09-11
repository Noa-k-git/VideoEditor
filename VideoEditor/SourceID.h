#pragma once
class SourceID
{
private:
	static int counter;

public:
	const int id = counter;
	SourceID();

};

