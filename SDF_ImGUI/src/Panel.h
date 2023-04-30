#pragma once
#include <iostream>
#include <string>
#include <functional>

class Panel
{
private :
	std::string _panelName;
	bool _state = true;
	std::function<void()> _content;
public :
	Panel(std::string name, std::function<void()> content);
	std::string getPanelName() const;
	bool getState() const;
	void setState(bool newState);
	void renderPanel() const;
};

