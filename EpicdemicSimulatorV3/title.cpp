#include "title.h"

Title::Title() : _title_font(), _title_letters(), _rotate_cw{true}
{
	if (!this->_title_font.loadFromFile(Button::default_font_name))
		throw std::invalid_argument("There is no such font here!");

	////////////// Good idea for future
	//const char title[] = { 'A','v','a','l','a','n','c','h','e' };
	//
	//for (auto& chr : title) {
	//	sf::Text letter;
	//	letter.setFont(this->_title_font);
	//	letter.setCharacterSize(25);
	//	letter.setFillColor(sf::Color::Red/*sf::Color(i * 5, 0, 0, 255)*/);
	//	letter.setString(chr);
	//	
	//	this->_title_letters.push_back(letter);
	//}
	////////////
	
	this->_title_letters.setFont(this->_title_font);
	this->_title_letters.setCharacterSize(75);
	this->_title_letters.setFillColor(sf::Color::Red/*sf::Color(i * 5, 0, 0, 255)*/);
	this->_title_letters.setString("A V A L A N C H E");
	sf::FloatRect title_shape = this->_title_letters.getGlobalBounds();
	this->_title_letters.setOrigin(sf::Vector2f(title_shape.width / 2, title_shape.height / 2));	// Center

}


void Title::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture = NULL;

	target.draw(this->_title_letters);

	////////////// Good idea
	//for (auto& letter : this->_title_letters)
	//	target.draw(letter);
	//////////////

	
	
}

void Title::update(const sf::Time& delta, const sf::Vector2f view_center)
{
	int color_change = (int) (Title::color_change_speed * delta.asSeconds());

	// Rotation
	if (this->_rotate_cw) {
		if (this->_title_letters.getRotation() < Title::max_rotation || this->_title_letters.getRotation() > 360 -  Title::max_rotation)
			this->_title_letters.rotate(Title::rotation_speed * delta.asSeconds());
		else {
			this->_rotate_cw = false;
			this->_title_letters.rotate(-Title::rotation_speed * delta.asSeconds());
		}
	}
	else {
		if (this->_title_letters.getRotation() < Title::max_rotation || this->_title_letters.getRotation() > 360 - Title::max_rotation)
			this->_title_letters.rotate(-Title::rotation_speed * delta.asSeconds());
		else {
			this->_rotate_cw = true;
			this->_title_letters.rotate(Title::rotation_speed * delta.asSeconds());
		}
	}
	// Color
	this->updateColor(_title_letters, color_change);

	// Position
	sf::Vector2f letters_pos(view_center.x, view_center.y - GameLogic::initial_window_height / 4);
	this->_title_letters.setPosition(letters_pos);
	//////////////////// Good idea
	//int i = -4;
	//for (auto& letter : this->_title_letters) {
	//	// Rotation
	//	if (letter.getRotation() < Title::max_rotation || letter.getRotation() < -Title::max_rotation) 
	//		letter.rotate(Title::rotation_speed * delta.asSeconds());
	//	else
	//		letter.rotate(-Title::rotation_speed * delta.asSeconds());
	//	// Color
	//	this->updateColor(letter, color_change);

	//	sf::Vector2f letter_pos(view_center.x + (i * GameLogic::initial_window_width / 11), view_center.y - GameLogic::initial_window_height / 4);
	//}
	////////////////////////
}

void Title::updateColor( sf::Text& letter, int color_change)
{
	sf::Color prev_color = letter.getFillColor();
	bool increase_color = this->checkColorChangeDirection(letter);
	sf::Color updated_color = prev_color;

	if (increase_color) 
	{
		this->increaseColor(prev_color, updated_color, color_change);
		if (color_change > 0)
			this->decreaseColor(prev_color, updated_color, color_change);
	}
	else 
	{
		this->decreaseColor(prev_color, updated_color, color_change);
		if (color_change > 0)
			this->increaseColor(prev_color, updated_color, color_change);
	}
	letter.setFillColor(updated_color);
}

void Title::increaseColor(const sf::Color& prev_color, sf::Color& updated_color, int& color_change) const
{
	if (prev_color.r < 255) {
		int diff = (int)(255 - prev_color.r);
		updated_color.r += (color_change - diff >= 0) ? diff : color_change;
		color_change -= updated_color.r - prev_color.r;
	}
	if (prev_color.g < 255 && color_change > 0) {
		int diff = (int)(255 - prev_color.g);
		updated_color.g += (color_change - diff >= 0) ? diff : color_change;
		color_change -= updated_color.g - prev_color.g;
	}
	if (prev_color.b < 255 && color_change > 0) {
		int diff = (int)(255 - prev_color.b);
		updated_color.b += (color_change - diff >= 0) ? diff : color_change;
		color_change -= updated_color.b - prev_color.b;
	}
}

void Title::decreaseColor(const sf::Color& prev_color, sf::Color& updated_color, int& color_change) const
{
	if (prev_color.r > 0) {
		int diff = (int) prev_color.r;
		updated_color.r -= (color_change - diff >= 0) ? diff : color_change;
		color_change -= prev_color.r - updated_color.r;
	}
	if (prev_color.g > 0 && color_change > 0) {
		int diff = (int)prev_color.g;
		updated_color.g -= (color_change - diff >= 0) ? diff : color_change;
		color_change -= prev_color.g - updated_color.g;
	}
	if (prev_color.b > 0 && color_change > 0) {
		int diff = (int)prev_color.b;
		updated_color.b -= (color_change - diff >= 0) ? diff : color_change;
		color_change -= prev_color.b - updated_color.b;
	}
}

bool Title::checkColorChangeDirection(const sf::Text& letter) const
{
	sf::Color letter_color = letter.getFillColor();
	if (letter_color.r == 0 && letter_color.g == 0 && letter_color.b == 0)
		return true;
	if (letter_color.b < 255)
		return !(letter_color.r == 0);
	return false;

}
