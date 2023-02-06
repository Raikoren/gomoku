#include "visual.hpp"

/* Constructor */
Visual::Visual() {
	_window_.create(sf::VideoMode(WIN_X, WIN_Y), "gomoku");
	if (!titleBg.loadFromFile("ressources/background.png")) {
		std::cout << "Error while loading background.png" << std::endl;
	}
	if (!sbTextureI.loadFromFile("ressources/go_button_small_idle.png")) {
		std::cout << "Error while loading button texture" << std::endl;
	}
	if (!sbTextureM.loadFromFile("ressources/go_button_small_mouseover.png")) {
		std::cout << "Error while loading button texture" << std::endl;
	}
	if (!sbTextureC.loadFromFile("ressources/go_button_small_selected.png")) {
		std::cout << "Error while loading button texture" << std::endl;
	}
	if (!goTextI.loadFromFile("ressources/gogame_idle.png")) {
		std::cout << "Error while loading button texture" << std::endl;
	}
	if (!goTextM.loadFromFile("ressources/gogame_mouseover.png")) {
		std::cout << "Error while loading button texture" << std::endl;
	}
	if (!mokuTextI.loadFromFile("ressources/gomoku_idle.png")) {
		std::cout << "Error while loading button texture" << std::endl;
	}
	if (!mokuTextM.loadFromFile("ressources/gomoku_mouseover.png")) {
		std::cout << "Error while loading button texture" << std::endl;
	}
	titleBg.setRepeated(true);
	wPound.setFillColor(sf::Color::White);
	wPound.setOutlineThickness(3);
	wPound.setOutlineColor(sf::Color(50, 50, 50));
	bPound.setFillColor(sf::Color::Black);
	bPound.setOutlineThickness(3);
	bPound.setOutlineColor(sf::Color(50, 50, 50));
	territory.setRadius(5);
	b1 = Button("9 x 9", { 200, 55 }, sf::Color::Black);
	b2 = Button("13 x 13", { 200, 55 }, sf::Color::Black);
	b3 = Button("19 x 19", { 200, 55 }, sf::Color::Black);
	b4 = Button("", { 400, 250 }, sf::Color::Black);
	b5 = Button("", { 400, 250 }, sf::Color::Black);
	b6 = Button("PASS", { 200, 55 }, sf::Color::Black);
	b7 = Button("HINT", { 200, 55 }, sf::Color::Black);
	b8 = Button("previews", { 200, 55 }, sf::Color::Black);
	b9 = Button("show score", { 200, 55 }, sf::Color::Black);
	bReturn = Button("Title Screen", { 200, 55 }, sf::Color::Black);
	if (!f.loadFromFile("ressources/arial.ttf")) {
		std::cout << "ERROR while loading font" << std::endl;
	}
	b1.setFont(f);
	b1.setButtonTexture(&sbTextureC);
	b2.setFont(f);
	b2.setButtonTexture(&sbTextureI);
	b3.setFont(f);
	b3.setButtonTexture(&sbTextureI);
	b4.setFont(f);
	b4.setButtonTexture(&mokuTextI);
	b5.setFont(f);
	b5.setButtonTexture(&goTextI);
	b6.setFont(f);
	b6.setButtonTexture(&sbTextureI);
	b7.setFont(f);
	b7.setButtonTexture(&sbTextureI);
	b8.setFont(f);
	b8.setButtonTexture(&sbTextureI);
	b9.setFont(f);
	b9.setButtonTexture(&sbTextureI);
	bReturn.setFont(f);
	bReturn.setButtonTexture(&sbTextureI);
	b1.setPosition({ WIN_X / 2 - 400, 700 });
	b2.setPosition({ WIN_X / 2 - 100, 700 });
	b3.setPosition({ WIN_X / 2 + 200, 700 });
	b4.setPosition({ WIN_X / 2 + 100, 200 });
	b5.setPosition({ WIN_X / 2 - 500, 200 });
	b6.setPosition({ WIN_X / 2 + BOARD / 2 + 50, WIN_Y / 2 + BOARD / 2 - 55 });
	b7.setPosition({ WIN_X / 2 + BOARD / 2 + 50, WIN_Y / 2 - BOARD / 2 });
	b8.setPosition({ WIN_X / 2 + BOARD / 2 + 50, WIN_Y / 2 - BOARD / 2 + 150 });
	b9.setPosition({ WIN_X / 2 + BOARD / 2 + 50, WIN_Y / 2 - BOARD / 2 });
	bReturn.setPosition({ WIN_X / 2 + BOARD / 2 + 50, WIN_Y / 2 + BOARD / 2 - 205 });
}

void Visual::draw(visual_data visualData) {
	size = visualData.size;
	sf::Sprite bg(titleBg);
	bg.setTextureRect(sf::IntRect(0, 0, WIN_X, WIN_Y));
	bg.setColor(sf::Color(104, 80, 50));
	_window_.draw(bg);
	if (!visualData.gameOn) {
		drawButtons();
	}
	else if (!visualData.victoryScreen) {
		drawBoard(visualData);
	}
	else {
		sf::Text t;
		sf::RectangleShape r;
		t.setFont(f);
		t.setCharacterSize(30);
		r.setSize(sf::Vector2f(WIN_X, WIN_Y));
		if (visualData.bScore < visualData.wScore) {
			r.setFillColor(sf::Color::White);
			t.setFillColor(sf::Color::Black);
			t.setString("BLANC GAGNED");
		}
		else if (visualData.bScore > visualData.wScore) {
			r.setFillColor(sf::Color::Black);
			t.setFillColor(sf::Color::White);
			t.setString("NOIR GAGNED");
		}
		else {
			r.setFillColor(sf::Color::Black);
			t.setFillColor(sf::Color::White);
			t.setString("EGALITED");
		}
		_window_.draw(r);
		_window_.draw(t);
	}
}

void Visual::drawButtons() {
	b1.drawTo(_window_);
	b2.drawTo(_window_);
	b3.drawTo(_window_);
	b4.drawTo(_window_);
	b5.drawTo(_window_);
}

void Visual::drawBoard(visual_data v) {
	sf::RectangleShape rect;
	sf::Vector2f pos(WIN_X / 2 - BOARD / 2, WIN_Y / 2 - BOARD / 2);
	rect.setPosition(pos);
	rect.setSize(sf::Vector2f(BOARD, BOARD));
	rect.setTexture(&goban);
	_window_.draw(rect);
	double pad = (BOARD - (MIN_MARGIN * 2)) / (size - 1);
	double margin = (BOARD - (pad * (size - 1))) / 2;
	for (int i = 0; i < size; i++) {
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
	drawPounds(v, margin, pad);
	b6.drawTo(_window_);
	b8.drawTo(_window_);
	bReturn.drawTo(_window_);
	if (v.gomoku)
		b7.drawTo(_window_);
	else
		b9.drawTo(_window_);
}

void Visual::drawPounds(visual_data v, double margin, double pad) {
	int x = 0;
	int y = 0;
	
	for (int i = 0; i < size * size; i++) {
		sf::Vector2f pos((WIN_X / 2 - BOARD / 2) + margin + pad * x - bPound.getRadius(),
			(WIN_Y / 2 - BOARD / 2) + margin + pad * y - bPound.getRadius());
		x++;
		if (x == size) {
			x = 0;
			y++;
		}
		if (v.map[i] != '0') {
			if (v.map[i] == '2') {
				bPound.setPosition(pos);
				_window_.draw(bPound);
			}
			else if (v.map[i] == '1') {
				wPound.setPosition(pos);
				_window_.draw(wPound);
			}
		}
		if (!v.gomoku && (v.endGame || v.scoreState) && v.result[i] != '0') {
			territory.setPosition(sf::Vector2f(pos.x + (bPound.getRadius() - 5), pos.y + (bPound.getRadius() - 5)));
			if (v.result[i] == '2') {
				territory.setFillColor(sf::Color::Black);
			}
			else if (v.result[i] == '1') {
				territory.setFillColor(sf::Color::White);
			}
			_window_.draw(territory);
		}
	}
	if (v.endGame || v.scoreState || v.gomoku) {
		sf::RectangleShape rb(sf::Vector2f(250, 100));
		sf::RectangleShape rw(sf::Vector2f(250, 100));
		sf::Text score;
		score.setFont(f);
		score.setCharacterSize(80);
		score.setFillColor(sf::Color::White);
		rb.setPosition(sf::Vector2f((WIN_X / 2 - BOARD / 2) - 300, (WIN_Y / 2 - BOARD / 2)));
		rw.setPosition(sf::Vector2f((WIN_X / 2 - BOARD / 2) - 300, (WIN_Y / 2 + BOARD / 2) - 100));
		rb.setFillColor(sf::Color(20,20,20));
		rw.setFillColor(sf::Color(230,230,230));
		if (v.endGame && v.gomoku && v.bScore == 5)
			rb.setFillColor(sf::Color::Green);
		if (v.endGame && v.gomoku && v.wScore == 5)
			rw.setFillColor(sf::Color::Green);
		_window_.draw(rw);
		_window_.draw(rb);
		score.setPosition(sf::Vector2f((WIN_X / 2 - BOARD / 2) - 200, (WIN_Y / 2 - BOARD / 2)));
		score.setString(std::to_string(v.bScore));
		_window_.draw(score);
		score.setPosition(sf::Vector2f((WIN_X / 2 - BOARD / 2) - 200, (WIN_Y / 2 + BOARD / 2) - 100));
		score.setString(std::to_string(v.wScore));
		score.setFillColor(sf::Color::Black);
		_window_.draw(score);
		if (v.endGame && v.gomoku && v.bScore != 5 && v.wScore != 5) {
			_window_.draw(v.line, 2, sf::Lines);
		}
	}
	if (v.previewEnable) {
		_window_.draw(v.preview);
	}
}