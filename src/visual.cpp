#include "visual.hpp"

/* Constructor */
Visual::Visual() {
	_window_.create(sf::VideoMode(WIN_X, WIN_Y), "gomoku");
	if (!titleBg.loadFromFile("ressources/background.png")) {
		std::cout << "Error" << std::endl;
	}
	titleBg.setRepeated(true);
	b1 = Button("9 x 9", { 200, 50 }, sf::Color::Green, sf::Color::Black);
	b2 = Button("13 x 13", { 200, 50 }, sf::Color::Green, sf::Color::Black);
	b3 = Button("19 x 19", { 200, 50 }, sf::Color::Green, sf::Color::Black);
	if (!f.loadFromFile("ressources/arial.ttf")) {
		std::cout << "ERROR" << std::endl;
	}
	b1.setFont(f);
	b2.setFont(f);
	b3.setFont(f);
	b1.setPosition({ (WIN_X / 2) - 500, 550 });
	b2.setPosition({ (WIN_X / 2) - 100, 550 });
	b3.setPosition({ (WIN_X / 2) + 300, 550 });

}

void Visual::draw() {

	drawTitleScreen();
	//drawBoard();
}

void Visual::drawTitleScreen() {
	
	sf::Vector2u size = titleBg.getSize();
	sf::Sprite bg(titleBg);
	bg.setTextureRect(sf::IntRect(0, 0, WIN_X, WIN_Y));
	_window_.draw(bg);
	drawButtons();
}

void Visual::drawButtons() {
	b1.drawTo(_window_);
	b2.drawTo(_window_);
	b3.drawTo(_window_);
}

void Visual::drawBoard() {
	sf::RectangleShape rect;
	sf::Vector2f pos(WIN_X / 2 - BOARD / 2, WIN_Y / 2 - BOARD / 2);
	rect.setPosition(pos);
	rect.setSize(sf::Vector2f(BOARD, BOARD));
	rect.setFillColor(sf::Color(194, 110, 0));
	_window_.draw(rect);
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
