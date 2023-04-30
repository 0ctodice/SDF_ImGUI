#include "Panel.h"

Panel::Panel(std::string name, std::function<void()> content) : _panelName{ name }, _content {content} {}
std::string Panel::getPanelName() const {return _panelName;}
bool Panel::getState() const { return _state; }
void Panel::setState(bool newState) { _state = newState; }
void Panel::renderPanel() const {
	if (_state) {
		_content();
	}
}