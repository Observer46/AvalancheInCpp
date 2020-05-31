#include "Button.h"

Button::Button(const std::string& title, const sf::Vector2f& position, const sf::Vector2f& size) :
	_font(), _title(), _unclicked(size), _mouse_on(size), _clicked(size), _mode{ ButtonMode::Unclicked }, _on_screen{false}, _position{position}
{
	if (!this->_font.loadFromFile(Button::default_font_name)) 
		throw std::invalid_argument("There is no such font here!");

	const int* colors = Button::button_colors;
	this->_unclicked.setFillColor(sf::Color(colors[0], colors[1], colors[2], colors[3]));
	this->_unclicked.setPosition(position);
	this->_unclicked.setOutlineThickness(3.f);
	this->_unclicked.setOutlineColor(sf::Color::Black);

	this->_mouse_on.setFillColor(sf::Color::Red);
	this->_mouse_on.setPosition(position);
	this->_mouse_on.setOutlineThickness(3.f);
	this->_mouse_on.setOutlineColor(sf::Color::Black);

	this->_clicked.setFillColor(sf::Color::Magenta);
	this->_clicked.setPosition(position);
	this->_clicked.setOutlineThickness(3.f);
	this->_clicked.setOutlineColor(sf::Color::Black);

	this->_title.setFont(this->_font);
	this->_title.setString(title);
	this->_title.setFillColor(sf::Color::White);
	this->_title.setOutlineColor(sf::Color::Black);
	this->_title.setOutlineThickness(1.f);
	sf::Vector2f title_position(position.x + _unclicked.getSize().x / 2 - _title.getGlobalBounds().width / 2,		// Centered position
		position.y + _unclicked.getSize().y / 2 - 3 *_title.getGlobalBounds().height / 4);
	this->_title.setPosition(title_position);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = NULL;
	if (this->_on_screen) {
		if (this->_mode == ButtonMode::Unclicked)
			target.draw(this->_unclicked, states);
		else if (this->_mode == ButtonMode::MouseOn)
			target.draw(this->_mouse_on, states);
		else
			target.draw(this->_clicked, states);
		target.draw(this->_title);
	}
}

void Button::setVisible(const sf::Vector2f& view_center)
{

	sf::Vector2f new_pos(view_center.x + this->_position.x, view_center.y + this->_position.y);

	sf::Vector2f title_position(view_center.x  - _title.getGlobalBounds().width / 2, // Ceneterd position
		view_center.y + this->_position.y);

	this->_title.setPosition(title_position);
	this->_unclicked.setPosition(new_pos);
	this->_mouse_on.setPosition(new_pos);
	this->_clicked.setPosition(new_pos);
	this->_on_screen = true; 
}
