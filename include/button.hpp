#ifndef BUTTON_HPP
# define BUTTON_HPP

#include <iostream>
#include <SFML/Graphics.hpp>


class Button {
public:
	Button() { }

	Button(std::string t, sf::Vector2f size, sf::Color textColor) {
		label.setString(t);
		label.setFillColor(textColor);
		label.setCharacterSize(30);
		button.setSize(size);
	}

	void setFont(sf::Font& font) {
		label.setFont(font);
	}

	void setButtonTexture(sf::Texture* t) {
		button.setTexture(t);
	}

	void setTextColor(sf::Color c) {
		label.setFillColor(c);
	}

	void setText(std::string s) {
		label.setString(s);
	}

	void setPosition(sf::Vector2f pos) {
		button.setPosition(pos);
		float xPos = (pos.x + button.getLocalBounds().width / 2) - (label.getLocalBounds().width / 2);
		float yPos = (pos.y + button.getLocalBounds().height / 2) - (label.getLocalBounds().height / 2) - 8;

		label.setPosition({ xPos, yPos });
	}

	void drawTo(sf::RenderWindow& w) {
		w.draw(button);
		w.draw(label);
	}

	bool isTargeted(sf::RenderWindow& w) {
		float mx = sf::Mouse::getPosition(w).x;
		float my = sf::Mouse::getPosition(w).y;

		float bx = button.getPosition().x;
		float by = button.getPosition().y;

		float bxWidth = button.getPosition().x + button.getLocalBounds().width;
		float byHeight = button.getPosition().y + button.getLocalBounds().height;

		if (mx < bxWidth && mx > bx && my < byHeight && my > by)
			return(true);
		else
			return(false);
	}

private:
	sf::RectangleShape button;
	sf::Text label;
};

#endif
