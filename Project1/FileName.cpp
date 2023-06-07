#include <iostream>
#include <windows.h>
#include <vector>
#include <stack>
#include <ctime>
#include <conio.h>
using namespace std;

//ƒелает заставку по середине
void gotoxy(int x, int y)
{
    COORD p = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

enum ConsolColor {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGNETA = 5,
    BROWN = 6,
    LIGH_GRAY = 7,
    DARKGRAY = 8,
    LIGH_BLUE = 9,
    LIGH_GREEN = 10,
    LIGH_CYAN = 11,
    LIGH_RED = 12,
    LIGH_MAGNETA = 13,
    YELLOW = 14,
    WHITE = 15
};


void setColor(int background, int text) {

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

//константы, чтобы ничего не забыть в значени€х
const int BORDER = 100; //граница пол€
const int EMPTY_CELL = 0; //пуста€ €чейка
const int MINE = 10; //мина

//»гровое поле
class Map {
private:
    int size;
    //размер пол€ включа€ границы
    vector<vector<int>> map;
    vector<vector<int>> mask;
public:

    Map() {
        size = 10;
    }

    //открытие €чеек, и вывод завершающего сообщени€
    int openCell(int x, int y) {
        int result = 1;
        mask[x][y] = 1;
        if (map[x][y] == MINE) {
            result = MINE;
        }
        else if (map[x][y] == EMPTY_CELL) {
            result = EMPTY_CELL;
        }
        show();

        return result;
    }

    bool isBorder(int x, int y) {

        //защита в случае выхода вектора за пределы пол€
        if (x < 0 || x >= size)
            return false;
        if (y < 0 || y >= size)
            return false;

        if (map[x][y] == BORDER || y == 0 || y == size - 1) {
            return true;
        }
        return false;
    }
    // сокращение кода дл€ пол€ и маски
    void initVec(vector<vector<int>> & vec) {
        for (int i = 0; i < size; i++) {
            vector<int> temp;
            for (int j = 0; j < size; j++) {
                //делаем границы пол€
                if (i == 0 || j == 0 || i == size - 1 || j == size - 1)
                    temp.push_back(BORDER);
                else
                    temp.push_back(EMPTY_CELL);
            }
            vec.push_back(temp);
        }
    }

    //заполнение пол€
    void initMap() {
        initVec(map);
    }
    //создание маски закрывающее рабочее пространство пол€
    void initMask() {
        initVec(mask);
    }

    // оптимизаци€ кода, который мен€ет цвет символов
    void coutColor(char ch, int color) {
        setColor(BLACK, color);
        cout << ch;
        setColor(BLACK, WHITE);
    }

    //становлени€ уловий дл€ мин и цифр
    void show() {
        gotoxy(0, 0);
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (mask[j][i] == EMPTY_CELL) {
                    //изменение символов внутри пол€!!!
                    cout << "/";
                    continue;
                }
                if (map[j][i] == BORDER) {
                    coutColor('#', DARKGRAY);
                }
                else if (map[j][i] == EMPTY_CELL)
                    cout << " ";
                else if (map[j][i] == MINE)
                    coutColor('*', RED);
                else if (map[j][i] == 1)
                    coutColor('1', WHITE);
                else if (map[j][i] == 2)
                    coutColor('2', BROWN);
                else if (map[j][i] == 3)
                    coutColor('3', LIGH_BLUE);
                else if (map[j][i] == 4)
                    coutColor('4', YELLOW);
                else if (map[j][i] == 5)
                    coutColor('5', LIGH_CYAN);
                else if (map[j][i] == 6)
                    coutColor('6', LIGH_RED);
                else if (map[j][i] == 7)
                    coutColor('7', LIGH_MAGNETA);
                else if (map[j][i] == 8)
                    coutColor('8', LIGH_GREEN);
                //cout << map[j][i] << " ";
            }
            cout << endl;
        }
    }

    //расположение мин в случайных местах
    void setRandomMines(int numMines) {
        //≈сли мин слишком много, выводим сообщение
        if (numMines >= (size - 2) * (size - 2)) {
            cout << "Brat, postav menshe min" << endl;
            return;
        }
        for (int i = 0; i < numMines; i++) {
            int x = 0;
            int y = 0;
            // поиск пустой €чейки, котора€ не зан€та€ миной
            do {
                x = rand() % (size - 2) + 1;
                y = rand() % (size - 2) + 1;
            } while (map[x][y] == MINE);

            //мина обозначаетс€ значением 10
            map[x][y] = MINE;
        }
    }

    //расставление чисел на поле
    void setDigits() {
        int d = 0;
        for (int i = 1; i < size - 1; i++) {
            for (int j = 1; j < size - 1; j++) {
                if (map[i][j] == MINE)
                    continue;
                if (map[i][j + 1] == MINE)
                    d++;
                if (map[i][j - 1] == MINE)
                    d++;
                if (map[i + 1][j + 1] == MINE)
                    d++;
                if (map[i + 1][j - 1] == MINE)
                    d++;
                if (map[i - 1][j + 1] == MINE)
                    d++;
                if (map[i - 1][j - 1] == MINE)
                    d++;
                if (map[i + 1][j] == MINE)
                    d++;
                if (map[i - 1][j] == MINE)
                    d++;
                map[i][j] = d;
                d = 0;
            }
        }
    }

    //открытие соседних €чеек
    void fill(int px, int py) {
        stack <int> stk;
        stk.push(px);
        stk.push(py);

        int x = 0, y = 0;

        while (true) {
            y = stk.top();  stk.pop();
            x = stk.top();  stk.pop();

            if (map[x][y - 1] == EMPTY_CELL && mask[x][y + 1] == 0) {
                    stk.push(x);
                    stk.push(y + 1);
            }
            mask[x][y + 1] = 1;
            if (map[x + 1][y - 1] == EMPTY_CELL && mask[x][y - 1] == 0) {
                stk.push(x);
                stk.push(y - 1);
            }
            mask[x][y - 1] = 1;
            if (map[x + 1][y + 1] == EMPTY_CELL && mask[x + 1][y + 1] == 0) {
                stk.push(x + 1);
                stk.push(y + 1);
            }
            mask[x + 1][y + 1] = 1;
            if (map[x + 1][y - 1] == EMPTY_CELL && mask[x + 1][y - 1] == 0) {
                stk.push(x + 1);
                stk.push(y - 1);
            }
            mask[x + 1][y - 1] = 1;
            if (map[x - 1][y + 1] == EMPTY_CELL && mask[x - 1][y + 1] == 0) {
                stk.push(x - 1);
                stk.push(y + 1);
            }
            mask[x - 1][y + 1] = 1;
            if (map[x - 1][y - 1] == EMPTY_CELL && mask[x - 1][y - 1] == 0) {
                stk.push(x - 1);
                stk.push(y - 1);
            }
            mask[x - 1][y - 1] = 1;
            mask[x - 1][y] = 1;
            if (map[x - 1][y] == EMPTY_CELL && mask[x - 1][y] == 0) {
                stk.push(x - 1);
                stk.push(y);
            }
            mask[x - 1][y] = 1;
            if (map[x + 1][y] == EMPTY_CELL && mask[x + 1][y] == 0 ) {
                stk.push(x + 1);
                stk.push(y);
            }
            mask[x + 1][y] = 1;

            if (stk.empty())
                break;
        }
    }
};


class Keyboard {
private:
    int ch = 0;
public:
    Keyboard() {
        ch = 0;
    }
    void waitkey() {
        ch = _getch();
    }
    int getKey() {
        return ch;
    }
};


class Cursor {
private:
    //переменные, которые содержат текущие координаты курсора
    int x = 1;
    int y = 1;
    int tx = 1;
    int ty = 1;
public:
    void save() {
        tx = x;
        ty = y;
    }
    void undo() {
        x = tx;
        y = ty;
    }
    void incX() {
        x++;
    }
    void decX() {
        x--;
    }
    void incY() {
        y++;
    }
    void decY() {
        y--;
    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    void move() {
        gotoxy(x, y);
    }
};


class Game {
private:
    void showLogo() {
        //ƒелает заставку по середине
        gotoxy(50, 9);
        cout << "Minesweeper" << endl;
        //ƒлительность заставки —апЄра
        //Sleep(3000);
        system("cls");
    }

public:
    void gameOver() {
        gotoxy(50, 9);
        cout << "Vot i vse...";
        Sleep(2000);
        gotoxy(0, 11);
        system("pause");
    }

    void run() {
        showLogo();
        gotoxy(50, 9);
        cout << "Game started" << endl;
        Sleep(2000);
        system("cls");
        Map map;
        map.initMap();
        map.initMask();
        map.setRandomMines(9);
        map.setDigits();
        map.show();

        Keyboard kb;
        Cursor cs;

        cs.move();

        bool exit = false;

        while (!exit) {
            kb.waitkey();
            cs.save();

            switch (kb.getKey()) {
            case 77: cs.incX(); break;// вправо
            case 80: cs.incY(); break;// вниз
            case 75: cs.decX(); break;// влево
            case 72: cs.decY(); break;// вверх
            case 13:
                int result = map.openCell(cs.getX(), cs.getY());
                if (result == MINE) {
                    gameOver();
                    exit = true;
                }
                if (result == EMPTY_CELL) {
                    map.fill(cs.getX(), cs.getY());
                    map.show();
                }
                break;
            }

            if (map.isBorder(cs.getX(), cs.getY())) {
                cs.undo();
            }
            cs.move();
        }
    }
};

int main()
{
    srand(time(0));
    Game game;
    game.run();
    return 0;
}


