#include "visual.hpp"

/* Constructor */
Visual::Visual() {
	_window_.create(sf::VideoMode(WIN_X, WIN_Y), "gomoku");
}

void Visual::draw() {
	sf::RectangleShape rect;
	sf::Vector2f pos(WIN_X / 2 - BOARD / 2, WIN_Y / 2 - BOARD / 2);
	rect.setPosition(pos);
	rect.setSize(sf::Vector2f(BOARD, BOARD));
	rect.setFillColor(sf::Color(194, 110, 0));
	_window_.draw(rect);
	grid();
}

void Visual::grid() {
	double pad = (BOARD - (MIN_MARGIN * 2)) / size;
	double margin = (BOARD - pad * size) / 2;
	for (int i = 0; i <= size; i++) {
		sf::Vertex line_h[] = {
			sf::Vertex(sf::Vector2f((WIN_X / 2 - BOARD / 2) + margin, (WIN_Y / 2 - BOARD / 2) + margin + pad * i),
				sf::Color::Black),
			sf::Vertex(sf::Vector2f((WIN_X / 2 + BOARD / 2) - margin, (WIN_Y / 2 - BOARD / 2) + margin + pad * i),
				sf::Color::Black)
		};
		sf::Vertex line_v[] = {
			sf::Vertex(sf::Vector2f((WIN_X / 2 - BOARD / 2) + margin + pad * i, (WIN_Y / 2 - BOARD / 2) + margin),
				sf::Color::Black),
			sf::Vertex(sf::Vector2f((WIN_X / 2 - BOARD / 2) + margin + pad * i, (WIN_Y / 2 + BOARD / 2) - margin),
				sf::Color::Black)
		};
		_window_.draw(line_h, 2, sf::Lines);
		_window_.draw(line_v, 2, sf::Lines);
	}
}
