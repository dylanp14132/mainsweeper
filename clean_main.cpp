// Cleaner version of main.cpp with select things outside the main function.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <random>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <string>
#include <SFML/Graphics.hpp>

class TextureManager {
public:
    sf::Texture t_digit;
    sf::Texture t_debug;
    sf::Texture t_pause;
    sf::Texture t_play;
    sf::Texture t_leaderboard;
    sf::Texture t_face_happy;
    sf::Texture t_face_lose;
    sf::Texture t_face_win;
    sf::Texture t_flag;
    sf::Texture t_mine;
    sf::Texture t_number_1;
    sf::Texture t_number_2;
    sf::Texture t_number_3;
    sf::Texture t_number_4;
    sf::Texture t_number_5;
    sf::Texture t_number_6;
    sf::Texture t_number_7;
    sf::Texture t_number_8;
    sf::Texture t_tile_revealed;
    sf::Texture t_tile_hidden;

    static TextureManager &getInstance() {
        static TextureManager instance;
        return instance;
    }

private:
    TextureManager() {
        t_digit.loadFromFile("files/images/digits.png");
        t_debug.loadFromFile("files/images/debug.png");
        t_pause.loadFromFile("files/images/pause.png");
        t_play.loadFromFile("files/images/play.png");
        t_leaderboard.loadFromFile("files/images/leaderboard.png");
        t_face_happy.loadFromFile("files/images/face_happy.png");
        t_face_lose.loadFromFile("files/images/face_lose.png");
        t_face_win.loadFromFile("files/images/face_win.png");
        t_flag.loadFromFile("files/images/flag.png");
        t_mine.loadFromFile("files/images/mine.png");
        t_number_1.loadFromFile("files/images/number_1.png");
        t_number_2.loadFromFile("files/images/number_2.png");
        t_number_3.loadFromFile("files/images/number_3.png");
        t_number_4.loadFromFile("files/images/number_4.png");
        t_number_5.loadFromFile("files/images/number_5.png");
        t_number_6.loadFromFile("files/images/number_6.png");
        t_number_7.loadFromFile("files/images/number_7.png");
        t_number_8.loadFromFile("files/images/number_8.png");
        t_tile_revealed.loadFromFile("files/images/tile_revealed.png");
        t_tile_hidden.loadFromFile("files/images/tile_hidden.png");
    }
};

struct Tile {
    unsigned int adjacentMines = 0;

    bool hasMine = false;
    bool isFlagged = false;
    bool isRevealed = false;

    sf::Sprite tileSprite;
    sf::Sprite numberSprite;
    sf::Sprite mineSprite;
    sf::Sprite flagSprite;
    sf::Sprite pausedSprite;

    std::array<Tile *, 8> tiles;

    Tile() {
        tileSprite.setTexture(TextureManager::getInstance().t_tile_hidden);
        flagSprite.setTexture(TextureManager::getInstance().t_flag);
        pausedSprite.setTexture(TextureManager::getInstance().t_tile_revealed);
    }
};

void setText(sf::Text &text, const float x, const float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(x, y);
}

void calculateAdjacentTiles(std::vector<std::vector<Tile> > &board) {
    const int rows = board.size();
    const int cols = board[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j].adjacentMines = 0;

            for (int k = 0; k < 8; k++) {
                int offsetI = i + (k == 0 || k == 1 || k == 2 ? -1 : k == 5 || k == 6 || k == 7 ? 1 : 0);
                int offsetJ = j + (k == 0 || k == 3 || k == 5 ? -1 : k == 2 || k == 4 || k == 7 ? 1 : 0);

                if (offsetI < 0 || offsetI >= rows || offsetJ < 0 || offsetJ >= cols) {
                    board[i][j].tiles[k] = nullptr;
                    continue;
                }

                board[i][j].tiles[k] = &board[offsetI][offsetJ];

                if (board[offsetI][offsetJ].hasMine)
                    board[i][j].adjacentMines++;
            }
        }
    }
}

void addOverlayNumber(std::vector<std::vector<Tile> > &board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            switch (board[i][j].adjacentMines) {
                case 1:
                    board[i][j].numberSprite.setTexture(TextureManager::getInstance().t_number_1);
                    break;
                case 2:
                    board[i][j].numberSprite.setTexture(TextureManager::getInstance().t_number_2);
                    break;
                case 3:
                    board[i][j].numberSprite.setTexture(TextureManager::getInstance().t_number_3);
                    break;
                case 4:
                    board[i][j].numberSprite.setTexture(TextureManager::getInstance().t_number_4);
                    break;
                case 5:
                    board[i][j].numberSprite.setTexture(TextureManager::getInstance().t_number_5);
                    break;
                case 6:
                    board[i][j].numberSprite.setTexture(TextureManager::getInstance().t_number_6);
                    break;
                case 7:
                    board[i][j].numberSprite.setTexture(TextureManager::getInstance().t_number_7);
                    break;
                case 8:
                    board[i][j].numberSprite.setTexture(TextureManager::getInstance().t_number_8);
                    break;
                default:
                    break;
            }
        }
    }
}

std::vector<std::vector<Tile> > createBoard(unsigned int rows, unsigned int cols, unsigned int mines) {
    std::vector<std::vector<Tile> > board;
    std::vector<bool> a;
    int count = 0;

    for (std::size_t i = 0; i < rows * cols; i++) {
        if (count < mines) {
            a.push_back(true);
            count++;
        } else {
            a.push_back(false);
        }
    }

    std::shuffle(a.begin(), a.end(), std::default_random_engine(rand()));

    for (int i = 0; i < rows; i++) {
        board.push_back(std::vector<Tile>());
        for (int j = 0; j < cols; j++) {
            Tile tile;

            tile.tileSprite.setPosition(j * 32, i * 32);
            tile.numberSprite.setPosition(j * 32, i * 32);
            tile.mineSprite.setPosition(j * 32, i * 32);
            tile.flagSprite.setPosition(j * 32, i * 32);
            tile.pausedSprite.setPosition(j * 32, i * 32);

            if (a[i * cols + j] == true) {
                tile.hasMine = true;
                tile.mineSprite.setTexture(TextureManager::getInstance().t_mine);
            }

            board[i].push_back(tile);
        }
    }

    calculateAdjacentTiles(board);
    addOverlayNumber(board);

    return board;
}

void revealTile(Tile &tile) {
    if (tile.isRevealed)
        return;

    tile.isRevealed = true;
    tile.tileSprite.setTexture(TextureManager::getInstance().t_tile_revealed);

    if (tile.adjacentMines == 0 && !tile.hasMine) {
        for (std::size_t i = 0; i < tile.tiles.size(); i++) {
            if (tile.tiles[i] == nullptr || tile.tiles[i]->hasMine || tile.tiles[i]->isFlagged)
                continue;

            revealTile(*(tile.tiles[i]));
        }
    }
}

void flagTile(Tile &tile, int &count) {
    if (tile.isRevealed)
        return;

    tile.isFlagged = !tile.isFlagged;

    count += (tile.isFlagged) ? -1 : 1;
}

void flagAllMines(std::vector<std::vector<Tile> > &board) {
    for (std::size_t i = 0; i < board.size(); i++) {
        for (std::size_t j = 0; j < board[i].size(); j++) {
            if (board[i][j].hasMine) {
                board[i][j].isFlagged = true;
            }
        }
    }
}

void revealAllMines(std::vector<std::vector<Tile> > &board) {
    for (std::size_t i = 0; i < board.size(); i++) {
        for (std::size_t j = 0; j < board[i].size(); j++) {
            if (board[i][j].hasMine) {
                board[i][j].isRevealed = true;
                board[i][j].tileSprite.setTexture(TextureManager::getInstance().t_tile_revealed);
            }
        }
    }
}

bool checkIfGameIsWon(const std::vector<std::vector<Tile> > &board) {
    for (std::size_t i = 0; i < board.size(); i++) {
        for (std::size_t j = 0; j < board[i].size(); j++) {
            if (!board[i][j].hasMine && !board[i][j].isRevealed) {
                return false;
            }
        }
    }

    return true;
}

void updateMineCounter(sf::Sprite (sprite)[], int num) {
    if (num > 999) {
        for (int i = 0; i < 3; i++)
            sprite[i].setTextureRect(sf::IntRect(189, 0, 21, 32));
        return;
    } else if (num < -99) {
        return;
    }


    bool isNegative = false;

    if (num < 0) {
        isNegative = true;
        num *= -1;
    }

    int firstDigit = num % 10;
    int secondDigit = (num / 10) % 10;
    int thirdDigit = (num / 100) % 10;

    sprite[2].setTextureRect(sf::IntRect((firstDigit * 21), 0, 21, 32));
    sprite[1].setTextureRect(sf::IntRect((secondDigit * 21), 0, 21, 32));

    if (isNegative) {
        sprite[0].setTextureRect(sf::IntRect(210, 0, 21, 32));
    } else {
        sprite[0].setTextureRect(sf::IntRect((thirdDigit * 21), 0, 21, 32));
    }
}

int numToTime(int num) {
    int seconds = num % 60;
    int minutes = num / 60;

    return minutes * 100 + seconds;
}

void updateTimer(sf::Sprite (sprite)[], int num) {
    num = numToTime(num);

    if (num > 9959)
        return;

    int firstDigit = num % 10;
    int secondDigit = (num / 10) % 10;
    int thirdDigit = (num / 100) % 10;
    int fourthDigit = (num / 1000) % 10;

    sprite[3].setTextureRect(sf::IntRect((firstDigit * 21), 0, 21, 32));
    sprite[2].setTextureRect(sf::IntRect((secondDigit * 21), 0, 21, 32));
    sprite[1].setTextureRect(sf::IntRect((thirdDigit * 21), 0, 21, 32));
    sprite[0].setTextureRect(sf::IntRect((fourthDigit * 21), 0, 21, 32));
}

int insertIntoLeaderboard(int time, const std::string &name) {
    time = numToTime(time);

    std::ifstream in("files/leaderboard.txt");
    std::stringstream ss;

    int rowCount = 1;
    bool isOnLeaderboard = false;

    std::string line;
    while (std::getline(in, line, ':')) {
        ss << line;
        std::getline(in, line);
        ss << line;

        int timeComparison = std::stoi(ss.str());
        ss.str("");

        if (time < timeComparison) {
            isOnLeaderboard = true;
            break;
        }

        rowCount++;
    }

    in.seekg(0, std::ios::beg);
    ss << in.rdbuf();

    if (isOnLeaderboard) {
        std::ofstream out("files/leaderboard.txt");
        for (std::size_t i = 1; i <= 5; i++) {
            if (i == rowCount) {
                out << (time / 1000) % 10 << (time / 100) % 10 << ":" << (time / 10) % 10 << time % 10 << ", " << name
                        << "\n";
            } else {
                std::getline(ss, line);
                out << line << '\n';
            }
        }
        return rowCount;
    }

    return 0;
}

void openLeaderboardWindow(unsigned int rows, unsigned int cols, const sf::Font &font,
                           bool isAddingToLeaderboard = false, int time = 0, const std::string &name = "") {
    int index = 0;

    if (isAddingToLeaderboard)
        index = insertIntoLeaderboard(time, name);

    unsigned int width = cols * 16, height = rows * 16 + 50;

    std::ifstream leaderboardFile("files/leaderboard.txt");
    std::stringstream lineStream;

    int rowCount = 1;

    std::string line;
    while (std::getline(leaderboardFile, line, ',')) {
        lineStream << rowCount << ". " << line;
        std::getline(leaderboardFile, line);
        if (index == rowCount) {
            lineStream << line << '*' << "\n\n";
        } else {
            lineStream << line << "\n\n";
        }
        rowCount++;
    }

    sf::RenderWindow leaderboard_window(sf::VideoMode(width, height), "Leaderboard", sf::Style::Close);

    sf::Text title;
    title.setFont(font);
    title.setString("LEADERBOARD");
    title.setCharacterSize(20);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Underlined | sf::Text::Bold);
    setText(title, (width / 2), (height / 2) - 120);

    sf::Text scores(title);
    title.setCharacterSize(18);
    title.setString(lineStream.str());
    title.setStyle(sf::Text::Bold);
    setText(title, (width / 2), (height / 2) + 20);


    while (leaderboard_window.isOpen()) {
        sf::Event event;

        while (leaderboard_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                leaderboard_window.close();
            }
        }

        leaderboard_window.clear(sf::Color::Blue);
        leaderboard_window.draw(title);
        leaderboard_window.draw(scores);
        leaderboard_window.display();
    }
}

int main() {
    std::ifstream configFile("files/config.cfg");

    unsigned int cols, rows, mines;

    configFile >> cols >> rows >> mines;

    unsigned int width = cols * 32, height = (rows * 32) + 100;

    sf::RenderWindow welcome_window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);

    sf::Font font;
    font.loadFromFile("files/font.ttf");

    std::string typed_text_string;

    sf::Text welcome_text;
    welcome_text.setFont(font);
    welcome_text.setString("WELCOME TO MINESWEEPER!");
    welcome_text.setCharacterSize(24);
    welcome_text.setFillColor(sf::Color::White);
    welcome_text.setStyle(sf::Text::Underlined | sf::Text::Bold);
    setText(welcome_text, (width / 2), (height / 2) - 150);

    sf::Text name_text(welcome_text);
    name_text.setString("Enter your name:");
    name_text.setStyle(sf::Text::Bold);
    name_text.setCharacterSize(20);
    setText(name_text, (width / 2), (height / 2) - 75);

    sf::Text typed_text(name_text);
    typed_text.setString("|");
    typed_text.setCharacterSize(18);
    typed_text.setFillColor(sf::Color::Yellow);
    setText(typed_text, (width / 2), (height / 2) - 45);


    while (welcome_window.isOpen()) {
        sf::Event event;
        //welcome_window.close();
        while (welcome_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                welcome_window.close();
                return 0;
            }
            if (event.type == sf::Event::TextEntered && typed_text_string.size() < 10) {
                if ((event.text.unicode < 91 && event.text.unicode > 64) || (
                        event.text.unicode > 96 && event.text.unicode < 123)) {
                    if (typed_text_string.size() == 0) {
                        typed_text_string += std::toupper(event.text.unicode);
                    } else {
                        typed_text_string += std::tolower(event.text.unicode);
                    }
                    typed_text.setString(typed_text_string + "|");
                    setText(typed_text, (width / 2), (height / 2) - 45);
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Backspace) {
                    if (!typed_text_string.empty()) {
                        typed_text_string.pop_back();
                        typed_text.setString(typed_text_string + "|");
                        setText(typed_text, (width / 2), (height / 2) - 45);
                    }
                }

                if (event.key.code == sf::Keyboard::Enter) {
                    if (!typed_text_string.empty()) {
                        welcome_window.close();
                    }
                }
            }
        }

        welcome_window.clear(sf::Color::Blue);
        welcome_window.draw(welcome_text);
        welcome_window.draw(name_text);
        welcome_window.draw(typed_text);
        welcome_window.display();
    }

    sf::RenderWindow game_window(sf::VideoMode(width, height), "Minesweeper", sf::Style::Close);

    sf::Sprite faceButton;
    faceButton.setPosition(((cols / 2.0f) * 32) - 32, 32 * (rows + 0.5f));
    faceButton.setTexture(TextureManager::getInstance().t_face_happy);

    sf::Sprite debugButton;
    debugButton.setPosition(cols * 32 - 304, 32 * (rows + 0.5f));
    debugButton.setTexture(TextureManager::getInstance().t_debug);

    sf::Sprite pausePlayButton;
    pausePlayButton.setPosition(cols * 32 - 240, 32 * (rows + 0.5f));
    pausePlayButton.setTexture(TextureManager::getInstance().t_pause);

    sf::Sprite leaderboardButton;
    leaderboardButton.setPosition(cols * 32 - 176, 32 * (rows + 0.5f));
    leaderboardButton.setTexture(TextureManager::getInstance().t_leaderboard);

    sf::Sprite mineCounter[3];

    for (int i = 0; i < 3; i++) {
        mineCounter[i].setPosition(33 + (21 * i), 32 * (rows + 0.5f) + 16);
        mineCounter[i].setTexture(TextureManager::getInstance().t_digit);
        mineCounter[i].setTextureRect(sf::IntRect(189, 0, 21, 32));
    }

    sf::Sprite timer[4];

    for (int i = 0; i < 2; i++) {
        timer[i].setPosition(cols * 32 - 97 + (21 * i), 32 * (rows + 0.5f) + 16);
        timer[i].setTexture(TextureManager::getInstance().t_digit);
        timer[i].setTextureRect(sf::IntRect(0, 0, 21, 32));
    }

    for (int i = 0; i < 2; i++) {
        timer[i + 2].setPosition(cols * 32 - 54 + (21 * i), 32 * (rows + 0.5f) + 16);
        timer[i + 2].setTexture(TextureManager::getInstance().t_digit);
        timer[i + 2].setTextureRect(sf::IntRect(0, 0, 21, 32));
    }


    std::vector<std::vector<Tile> > board = createBoard(rows, cols, mines);

    bool isPaused = false;
    bool isGameWon = false;
    bool isGameOver = false;
    bool isDebugModeOn = false;
    bool isMinesRevealed = false;
    bool isLeaderboardOpen = false;

    int mineCount = mines;

    double time = 0;

    while (game_window.isOpen()) {
        sf::Event event;

        auto start_time = std::chrono::high_resolution_clock::now();

        isLeaderboardOpen = false;

        while (game_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                game_window.close();
                return 0;
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !
                isLeaderboardOpen) {
                if (!isGameOver && !isPaused && debugButton.getGlobalBounds().contains(
                        sf::Mouse::getPosition(game_window).x, sf::Mouse::getPosition(game_window).y)) {
                    isDebugModeOn = !isDebugModeOn;
                }
                if (!isGameOver && pausePlayButton.getGlobalBounds().contains(
                        sf::Mouse::getPosition(game_window).x, sf::Mouse::getPosition(game_window).y)) {
                    isPaused = !isPaused;

                    if (isPaused) {
                        pausePlayButton.setTexture(TextureManager::getInstance().t_play);
                    } else {
                        pausePlayButton.setTexture(TextureManager::getInstance().t_pause);
                    }
                }
                if (leaderboardButton.getGlobalBounds().contains(sf::Mouse::getPosition(game_window).x,
                                                                 sf::Mouse::getPosition(game_window).y)) {
                    isLeaderboardOpen = true;
                }
                if (faceButton.getGlobalBounds().contains(sf::Mouse::getPosition(game_window).x,
                                                          sf::Mouse::getPosition(game_window).y)) {
                    board = createBoard(rows, cols, mines);
                    isGameOver = false;
                    isMinesRevealed = false;
                    mineCount = mines;
                    time = 0;
                    faceButton.setTexture(TextureManager::getInstance().t_face_happy);
                }

                for (std::size_t i = 0; i < rows; i++) {
                    bool tileFound = false;
                    for (std::size_t j = 0; j < cols; j++) {
                        if (!isGameOver && board[i][j].tileSprite.getGlobalBounds().contains(
                                sf::Mouse::getPosition(game_window).x, sf::Mouse::getPosition(game_window).y)) {
                            if (!board[i][j].isFlagged && board[i][j].hasMine) {
                                faceButton.setTexture(TextureManager::getInstance().t_face_lose);
                                revealAllMines(board);
                                isGameOver = true;
                                isMinesRevealed = true;
                            } else if (!board[i][j].isFlagged) {
                                revealTile(board[i][j]);
                                isGameWon = checkIfGameIsWon(board);
                            }

                            tileFound = true;
                            break;
                        }
                    }
                    if (tileFound)
                        break;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                for (std::size_t i = 0; i < rows; i++) {
                    bool tileFound = false;
                    for (std::size_t j = 0; j < cols; j++) {
                        if (!isGameOver && board[i][j].tileSprite.getGlobalBounds().contains(
                                sf::Mouse::getPosition(game_window).x, sf::Mouse::getPosition(game_window).y)) {
                            flagTile(board[i][j], mineCount);
                            tileFound = true;
                            break;
                        }
                    }
                    if (tileFound)
                        break;
                }
            }
        }

        if (isGameWon) {
            isGameOver = true;
            mineCount = 0;
            flagAllMines(board);
            faceButton.setTexture(TextureManager::getInstance().t_face_win);
        }

        updateMineCounter(mineCounter, mineCount);

        updateTimer(timer, time);

        game_window.clear(sf::Color::White);

        for (std::size_t i = 0; i < rows; i++) {
            for (std::size_t j = 0; j < cols; j++) {
                if (isPaused || isLeaderboardOpen) {
                    game_window.draw(board[i][j].pausedSprite);
                    continue;
                }
                game_window.draw(board[i][j].tileSprite);

                if (isDebugModeOn || isMinesRevealed)
                    game_window.draw(board[i][j].mineSprite);

                if (board[i][j].isRevealed && !board[i][j].hasMine)
                    game_window.draw(board[i][j].numberSprite);
                else if (board[i][j].isFlagged)
                    game_window.draw(board[i][j].flagSprite);
            }
        }

        for (const auto &i: mineCounter) {
            game_window.draw(i);
        }

        for (const auto &i: timer) {
            game_window.draw(i);
        }

        game_window.draw(faceButton);
        game_window.draw(debugButton);
        game_window.draw(pausePlayButton);
        game_window.draw(leaderboardButton);
        game_window.display();

        auto end_time = std::chrono::high_resolution_clock::now();

        if (!isGameOver && !isPaused && !isLeaderboardOpen) {
            auto duration = std::chrono::duration_cast<std::chrono::duration<double> >(end_time - start_time);
            time += duration.count();
        }
        if (isGameWon) {
            openLeaderboardWindow(rows, cols, font, true, time, typed_text_string);
            isGameWon = false;
        } else if (isLeaderboardOpen) {
            openLeaderboardWindow(rows, cols, font);
        }
    }
    return 0;
}
