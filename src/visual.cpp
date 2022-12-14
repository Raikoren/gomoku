#include "visual.hpp"

/* Constructor */
Visual::Visual() {
	_window_.create(sf::VideoMode(WIN_X, WIN_Y), "gomoku");
	if (!titleBg.loadFromFile("ressources/background.png")) {
		std::cout << "Error" << std::endl;
	}
	titleBg.setRepeated(true);
	wPound.setFillColor(sf::Color::White);
	wPound.setOutlineThickness(3);
	wPound.setOutlineColor(sf::Color(50, 50, 50));
	wPound.setRadius(10);
	bPound.setFillColor(sf::Color::Black);
	bPound.setOutlineThickness(3);
	bPound.setOutlineColor(sf::Color(50, 50, 50));
	bPound.setRadius(10);
	b1 = Button("9 x 9", { 200, 50 }, sf::Color::Green, sf::Color::Black);
	b2 = Button("13 x 13", { 200, 50 }, sf::Color::Green, sf::Color::Black);
	b3 = Button("19 x 19", { 200, 50 }, sf::Color::Green, sf::Color::Black);
	b4 = Button("lesgong", { 200, 50 }, sf::Color::Green, sf::Color::Black);
	if (!f.loadFromFile("ressources/arial.ttf")) {
		std::cout << "ERROR" << std::endl;
	}
	b1.setFont(f);
	b2.setFont(f);
	b3.setFont(f);
	b4.setFont(f);
	b1.setPosition({ (WIN_X / 2) - 500, 550 });
	b2.setPosition({ (WIN_X / 2) - 100, 550 });
	b3.setPosition({ (WIN_X / 2) + 300, 550 });
	b4.setPosition({ (WIN_X / 2) - 100, 400 });

}

void Visual::draw(visual_data visualData) {
	size = visualData.size;
	if (visualData.titleScreen) {
		drawTitleScreen();
	}
	else {
		if (wPound.getRadius() == 10) {
			wPound.setRadius((BOARD - (MIN_MARGIN * 2)) / (size + 1) / 2 - 7);
			bPound.setRadius((BOARD - (MIN_MARGIN * 2)) / (size + 1) / 2 - 7);
		}
		drawBoard(visualData);
	}
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
	b4.drawTo(_window_);
}

void Visual::drawBoard(visual_data v) {
	sf::RectangleShape rect;
	sf::Vector2f pos(WIN_X / 2 - BOARD / 2, WIN_Y / 2 - BOARD / 2);
	rect.setPosition(pos);
	rect.setSize(sf::Vector2f(BOARD, BOARD));
	rect.setFillColor(sf::Color(194, 110, 0));
	_window_.draw(rect);
	double pad = (BOARD - (MIN_MARGIN * 2)) / (size + 1);
	double margin = (BOARD - pad * (size + 1)) / 2;
	for (int i = 0; i <= size + 1; i++) {
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
	drawPounds(v, margin + 7 + pad / 2, pad);
}

void Visual::drawPounds(visual_data v, double margin, double pad) {
	int x = 0;
	int y = 0;
	
	for (int i = 0; i < size * size; i++) {
		sf::Vector2f pos((WIN_X / 2 - BOARD / 2) + margin + pad * x, (WIN_Y / 2 - BOARD / 2) + margin + pad * y);
		x++;
		if (x == size) {
			x = 0;
			y++;
		}
		if (v.map[i] == '2' || v.map[i] == '1') {
			if (v.map[i] == '2') {
				bPound.setPosition(pos);
				_window_.draw(bPound);
			}
			else if (v.map[i] == '1') {
				wPound.setPosition(pos);
				_window_.draw(wPound);
			}
		}
	}
	if (v.previewEnable) {
		v.preview.setRadius(bPound.getRadius());
		_window_.draw(v.preview);
	}
}