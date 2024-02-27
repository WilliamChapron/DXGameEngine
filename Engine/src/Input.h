#pragma once

#include <unordered_map>
class Input
{
private:
	std::unordered_map<int, bool> currentKeys;
	std::unordered_map<int, bool> previousKeys;

public: 
	Input();
	~Input();

	void update();

	bool isKeyDown(int keyCode) const;
	bool isKeyUp(int keyCode) const;
	bool isKeyPressed(int keyCode) const;


};

