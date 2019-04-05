/*
 * Copyright (c) 2015, 2019, Мира Странная <miraityan2004@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#define BOMB 9

#include <GL/glut.h>
#include <ctime>

enum COLOR {E, W, B, R};

struct Point {
    int x;
    int y;
};

Point windowSize;
Point mousePos;

int mouseState;
int mouseButton;
int losex;
int losey;
int gameWidth = 30;
int gameHeight = 30;
int lastGameWidth = 0;
int lastGameHeight = 0;
int lastFieldx;
int lastFieldy;
int bombs;
int mainWindow;
int settingsWindow;
char field[256][128];
bool lose;
bool win;
bool firstClick;

const COLOR bombd[16][16] =
{
    E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, B, E, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, B, E, E, E, E, E, E, E,
    E, E, E, E, B, E, B, B, B, B, B, E, B, E, E, E,
    E, E, E, E, E, B, B, B, B, B, B, B, E, E, E, E,
    E, E, E, E, B, B, W, W, B, B, B, B, B, E, E, E,
    E, E, E, E, B, B, W, W, B, B, B, B, B, E, E, E,
    E, E, B, B, B, B, B, B, B, B, B, B, B, B, B, E,
    E, E, E, E, B, B, B, B, B, B, B, B, B, E, E, E,
    E, E, E, E, B, B, B, B, B, B, B, B, B, E, E, E,
    E, E, E, E, E, B, B, B, B, B, B, B, E, E, E, E,
    E, E, E, E, B, E, B, B, B, B, B, E, B, E, E, E,
    E, E, E, E, E, E, E, E, B, E, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, B, E, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
};

const COLOR flagd[16][16] =
{
    E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, R, R, E, E, E, E, E, E,
    E, E, E, E, E, E, R, R,	R, R, E, E, E, E, E, E,
    E, E, E, E, E, R, R, R, R, R, E, E, E, E, E, E,
    E, E, E, E, E, E, R, R, R, R, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, R, R, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, E, B, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, E, B, E, E, E, E, E, E,
    E, E, E, E, E, E, E, B, B, B, B, E, E, E, E, E,
    E, E, E, E, E, B, B, B, B, B, B, B, B, E, E, E,
    E, E, E, E, E, B, B, B, B, B, B, B, B, E, E, E,
    E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
    E, E, E, E, E, E, E, E, E, E, E, E, E, E, E, E,
};

void initGame(int x, int y);

struct Button;

extern Button buttons[][128];

struct Button
{
    int x1;
    int y1;
    int x2;
    int y2;
    bool clickable;
    bool visible;
    bool pressed;
    bool flag;

    Button()
    {
        init();
    }

    void init()
    {
        x1 = 0;
        y1 = 0;
        x2 = 0;
        y2 = 0;
        clickable = true;
        visible = true;
        pressed = false;
        flag = false;
    }

    void draw()
    {
        if (visible)
        {
            glColor3f(0.75f, 0.75f, 0.75f);

            glBegin(GL_QUADS);
                glVertex2f(x1, y1);
                glVertex2f(x1, y2);
                glVertex2f(x2, y2);
                glVertex2f(x2, y1);

            pressed ? glColor3f(0.5f, 0.5f, 0.5f) : glColor3f(1.0f, 1.0f, 1.0f);

                glVertex2f(x1, y1);
                glVertex2f(x1, y2);
                glVertex2f(x1 + 2, y2 - 2);
                glVertex2f(x1 + 2, y1 + 2);

                glVertex2f(x1 + 2, y2 - 2);
                glVertex2f(x1, y2);
                glVertex2f(x2, y2);
                glVertex2f(x2 - 2, y2 - 2);

            pressed ? glColor3f(1.0f, 1.0f, 1.0f) : glColor3f(0.5f, 0.5f, 0.5f);

                glVertex2f(x2 - 2, y1 + 2);
                glVertex2f(x2 - 2, y2 - 2);
                glVertex2f(x2, y2);
                glVertex2f(x2, y1);

                glVertex2f(x1, y1);
                glVertex2f(x1 + 2, y1 + 2);
                glVertex2f(x2 - 2, y1 + 2);
                glVertex2f(x2, y1);
            glEnd();

            if(flag)
            {
                glBegin(GL_POINTS);
                    for(int k = 0; k < 16; k++)
                        for(int l = 0; l < 16; l++)
                        {
                            if(flagd[15 - l][k] != E)
                            {
                                switch(flagd[15 - l][k])
                                {
                                case W:
                                    glColor3f(1.0f, 1.0f, 1.0f);
                                    break;

                                case B:
                                    glColor3f(0.0f, 0.0f, 0.0f);
                                    break;

                                case R:
                                    glColor3f(1.0f, 0.0f, 0.0f);
                                    break;
                                }
                                glVertex2f(k + x1 - 0.5f, l + y1 + 1.5f);
                            }
                        }
                    glEnd();
            }
        }
    }

    void tick(int i, int j)
    {
        if(clickable)
        {
            if (mouseState == GLUT_DOWN && mousePos.x > x1 && mousePos.x < x2 && mousePos.y > y1 && mousePos.y < y2)
            {
                if(mouseButton == GLUT_LEFT_BUTTON)
                    pressed = true;

                flag = (mouseButton == GLUT_RIGHT_BUTTON) && !flag;
            }

            if(mouseState == GLUT_UP && pressed)
            {
                pressed = false;

                if(mousePos.x > x1 && mousePos.x < x2 && mousePos.y > y1 && mousePos.y < y2)
                    clickUp(i, j);
            }
        }
        else
        {
            if(mouseButton == GLUT_MIDDLE_BUTTON && mousePos.x > x1 && mousePos.x < x2 && mousePos.y > y1 && mousePos.y < y2)
            {
                buttons[i + 1][j + 1].pressed = mouseState == GLUT_DOWN && !buttons[i + 1][j + 1].flag;
                buttons[i]	  [j + 1].pressed = mouseState == GLUT_DOWN && !buttons[i]	  [j + 1].flag;
                buttons[i - 1][j + 1].pressed = mouseState == GLUT_DOWN && !buttons[i - 1][j + 1].flag;
                buttons[i + 1][j	].pressed = mouseState == GLUT_DOWN && !buttons[i + 1][j	].flag;
                buttons[i - 1][j	].pressed = mouseState == GLUT_DOWN && !buttons[i - 1][j	].flag;
                buttons[i + 1][j - 1].pressed = mouseState == GLUT_DOWN && !buttons[i + 1][j - 1].flag;
                buttons[i]	  [j - 1].pressed = mouseState == GLUT_DOWN && !buttons[i]	  [j - 1].flag;
                buttons[i - 1][j - 1].pressed = mouseState == GLUT_DOWN && !buttons[i - 1][j - 1].flag;

                if((buttons[i + 1][j + 1].flag +
                    buttons[i    ][j + 1].flag +
                    buttons[i - 1][j + 1].flag +
                    buttons[i + 1][j	].flag +
                    buttons[i - 1][j    ].flag +
                    buttons[i + 1][j - 1].flag +
                    buttons[i    ][j - 1].flag +
                    buttons[i - 1][j - 1].flag) == field[i][j] && mouseState == GLUT_UP)
                {
                    if(!buttons[i + 1][j + 1].flag)
                        buttons[i + 1][j + 1].clickUp(i + 1, j + 1);

                    if(!buttons[i    ][j + 1].flag)
                        buttons[i    ][j + 1].clickUp(i,     j + 1);

                    if(!buttons[i - 1][j + 1].flag)
                        buttons[i - 1][j + 1].clickUp(i - 1, j + 1);

                    if(!buttons[i + 1][j    ].flag)
                        buttons[i + 1][j    ].clickUp(i + 1, j	  );

                    if(!buttons[i - 1][j    ].flag)
                        buttons[i - 1][j    ].clickUp(i - 1, j	  );

                    if(!buttons[i + 1][j - 1].flag)
                        buttons[i + 1][j - 1].clickUp(i + 1, j - 1);

                    if(!buttons[i    ][j - 1].flag)
                        buttons[i    ][j - 1].clickUp(i,     j - 1);

                    if(!buttons[i - 1][j - 1].flag)
                        buttons[i - 1][j - 1].clickUp(i - 1, j - 1);
                }
            }
        }
    }

    void clickUp(int i, int j)
    {
        if(firstClick)
        {
            bombs = (gameWidth * gameHeight) / 8;
            initGame(i, j);
            firstClick = false;
        }

        visible = false;
        clickable = false;

        if(field[i][j] == BOMB)
        {
            for(int k = 0; k < gameWidth; k++)
                for(int l = 0; l < gameHeight; l++)
                {
                    buttons[k][l].visible = (field[k][l] == BOMB && buttons[k][l].flag) || (field[k][l] != BOMB && !buttons[k][l].flag) && buttons[k][l].visible;
                    buttons[k][l].clickable = (field[k][l] == BOMB && buttons[k][l].flag) || (field[k][l] != BOMB && !buttons[k][l].flag) && buttons[k][l].clickable;
                }

            losex = i;
            losey = j;
            lose = true;
        }
        else if(field[i][j] == 0 && j < gameHeight && i < gameWidth && j >= 0 && i >= 0)
        {
            if(buttons[i][j + 1].clickable && !buttons[i][j + 1].flag)
                buttons[i][j + 1].clickUp(i, j + 1);

            if(buttons[i][j - 1].clickable && !buttons[i][j - 1].flag)
                buttons[i][j - 1].clickUp(i, j - 1);

            if(buttons[i + 1][j + 1].clickable && !buttons[i + 1][j + 1].flag)
                buttons[i + 1][j + 1].clickUp(i + 1, j + 1);

            if(buttons[i - 1][j - 1].clickable && !buttons[i - 1][j - 1].flag)
                buttons[i - 1][j - 1].clickUp(i - 1, j - 1);

            if(buttons[i + 1][j - 1].clickable && !buttons[i + 1][j - 1].flag)
                buttons[i + 1][j - 1].clickUp(i + 1, j - 1);

            if(buttons[i - 1][j + 1].clickable && !buttons[i - 1][j + 1].flag)
                buttons[i - 1][j + 1].clickUp(i - 1, j + 1);

            if(buttons[i + 1][j].clickable && !buttons[i + 1][j].flag)
                buttons[i + 1][j].clickUp(i + 1, j);

            if(buttons[i - 1][j].clickable && !buttons[i - 1][j].flag)
                buttons[i - 1][j].clickUp(i - 1, j);
        }
    }
} buttons[256][128];

void initGame(int x, int y)
{
    int i;
    int j;

    //srand(GetTickCount());
    lose = false;

    if(bombs >= gameWidth * gameHeight)
        bombs = (gameWidth * gameHeight) - 1;

    for(int k = 0; k < bombs; 0)
    {
        i = rand() % gameWidth;
        j = rand() % gameHeight;

        if(!(x == i && y == j) && field[i][j] != BOMB)
        {
            field[i][j] = BOMB;
            k++;
        }
    }

    for(int i = 0; i <= gameWidth; i++)
        for(int j = 0; j <= gameHeight; j++)
        {
            field[i][j] == 0 ? field[i][j] = (field[i + 1][j]     == BOMB) +
                                             (field[i - 1][j]	  == BOMB) +
                                             (field[i + 1][j + 1] == BOMB) +
                                             (field[i - 1][j + 1] == BOMB) +
                                             (field[i + 1][j - 1] == BOMB) +
                                             (field[i - 1][j - 1] == BOMB) +
                                             (field[i]	  [j + 1] == BOMB) +
                                             (field[i]    [j - 1] == BOMB) : 0;
        }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glViewport(0, 0, windowSize.x, windowSize.y);
    glOrtho(0, windowSize.x, 0, windowSize.y, -1.0, 1.0);

    glColor3f(0.75f, 0.75f, 0.75f);

    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(0, gameHeight * 16 + 32);
        glVertex2f(gameWidth * 16 + 32, gameHeight * 16 + 32);
        glVertex2f(gameWidth * 16 + 32, 0);
    glEnd();

    if(lose)
    {
        glColor3f(1.0f, 0.0f, 0.0f);

        glBegin(GL_QUADS);
            glVertex2f(losex * 16 + 16, losey * 16 + 16);
            glVertex2f(losex * 16 + 16, losey * 16 + 32);
            glVertex2f(losex * 16 + 32, losey * 16 + 32);
            glVertex2f(losex * 16 + 32, losey * 16 + 16);
        glEnd();
    }

    glColor3f(0.5f, 0.5f, 0.5f);


    glBegin(GL_LINES);
        for(int i = 1; i < gameWidth; i++)
        {
            glVertex2f(i * 16 + 16.5f, 15.5f);
            glVertex2f(i * 16 + 16.5f, gameHeight * 16 + 16.5f);
        }

        for(int i = 1; i < gameHeight; i++)
        {
            glVertex2f(15.5f, i * 16 + 15.5f);
            glVertex2f(gameWidth * 16 + 16.5f, i * 16 + 15.5f);
        }
    glEnd();

    glColor3f(0.5f, 0.5f, 0.5f);

    glBegin(GL_QUADS);
        glVertex2f(13, 13);
        glVertex2f(13, gameHeight * 16 + 19);
        glVertex2f(16, gameHeight * 16 + 16);
        glVertex2f(16, 16);

        glVertex2f(16, gameHeight * 16 + 16);
        glVertex2f(13, gameHeight * 16 + 19);
        glVertex2f(gameWidth * 16 + 19, gameHeight * 16 + 19);
        glVertex2f(gameWidth * 16 + 16, gameHeight * 16 + 16);

    glColor3f(1.0f, 1.0f, 1.0f);

        glVertex2f(gameWidth * 16 + 16, gameHeight * 16 + 16);
        glVertex2f(gameWidth * 16 + 19, gameHeight * 16 + 19);
        glVertex2f(gameWidth * 16 + 19, 13);
        glVertex2f(gameWidth * 16 + 16, 16);

        glVertex2f(16, 16);
        glVertex2f(13, 13);
        glVertex2f(gameWidth * 16 + 19, 13);
        glVertex2f(gameWidth * 16 + 16, 16);
    glEnd();

    lastFieldx = 255;
    lastFieldy = 127;

    if(lose)
        for(int i = 0; i < gameWidth; i++)
            for(int j = 0; j < gameHeight; j++)
            {
                glPushMatrix();
                glTranslatef(16.0f * i + 16, 16.0f * j + 16, 0.0f);
                if(field[i][j] == BOMB)
                {
                    glBegin(GL_POINTS);
                        for(int k = 0; k < 16; k++)
                            for(int l = 0; l < 16; l++)
                            {
                                if(bombd[15 - l][k] != E)
                                {
                                    switch(bombd[15 - l][k])
                                    {
                                    case W:
                                        glColor3f(1.0f, 1.0f, 1.0f);
                                        break;

                                    case B:
                                        glColor3f(0.0f, 0.0f, 0.0f);
                                        break;

                                    case R:
                                        glColor3f(1.0f, 0.0f, 0.0f);
                                        break;
                                    }

                                    glVertex2f(k + 0.5f, l + 0.5f);
                                }
                            }
                    glEnd();
                }

                if(field[i][j] != BOMB && buttons[i][j].flag)
                {
                    glBegin(GL_POINTS);
                        for(int k = 0; k < 16; k++)
                            for(int l = 0; l < 16; l++)
                            {
                                if(bombd[15 - l][k] != E)
                                {
                                    switch(bombd[15 - l][k])
                                    {
                                    case W:
                                        glColor3f(1.0f, 1.0f, 1.0f);
                                        break;

                                    case B:
                                        glColor3f(0.0f, 0.0f, 0.0f);
                                        break;

                                    case R:
                                        glColor3f(1.0f, 0.0f, 0.0f);
                                        break;
                                    }

                                    glVertex2f(k + 0.5f, l + 0.5f);
                                }
                            }
                    glEnd();

                    glColor3f(1.0f, 0.0f, 0.0f);

                    glBegin(GL_LINES);
                        glVertex2i(3, 13);
                        glVertex2i(15, 1);

                        glVertex2i(2, 13);
                        glVertex2i(14, 1);

                        glVertex2i(14, 13);
                        glVertex2i(2, 1);

                        glVertex2i(15, 13);
                        glVertex2i(3, 1);
                    glEnd();
                }

                glPopMatrix();
        }

    for(int i = 0; i < gameWidth; i++)
        for(int j = 0; j < gameHeight; j++)
        {
            if(field[i][j] != BOMB && field[i][j] != 0 && !buttons[i][j].visible && !buttons[i][j].flag)
            {
                if(field[i][j] != field[lastFieldx][lastFieldy])
                    switch (field[i][j])
                    {
                    case 1:
                        glColor3f(0.0f, 0.0f, 1.0f);
                        break;

                    case 2:
                        glColor3f(0.0f, 0.5f, 0.0f);
                        break;

                    case 3:
                        glColor3f(1.0f, 0.0f, 0.0f);
                        break;

                    case 4:
                        glColor3f(0.0f, 0.0f, 0.5f);
                        break;

                    case 5:
                        glColor3f(0.5f, 0.0f, 0.0f);
                        break;

                    case 6:
                        glColor3f(0.0f, 0.5f, 0.5f);
                        break;

                    case 7:
                        glColor3f(1.0f, 0.0f, 0.0f);
                        break;

                    case 8:
                        glColor3f(0.4f, 0.4f, 0.4f);
                        break;

                    default:
                        glColor3f(0.0f, 0.0f, 0.0f);
                        break;
                    }

                lastFieldx = i;
                lastFieldy = j;

                glRasterPos2f(i * 16 + 21.5f, j * 16 + 19.5f);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, field[i][j] + '0');

                glRasterPos2f(i * 16 + 21.5f, j * 16 + 18.5f);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, field[i][j] + '0');

                glRasterPos2f(i * 16 + 20.5f, j * 16 + 19.5f);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, field[i][j] + '0');

                glRasterPos2f(i * 16 + 20.5f, j * 16 + 18.5f);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, field[i][j] + '0');

                glRasterPos2f(i * 16 + 19.5f, j * 16 + 19.5f);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, field[i][j] + '0');

                glRasterPos2f(i * 16 + 19.5f, j * 16 + 18.5f);
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, field[i][j] + '0');
            }
        }

        for(int i = 0; i < gameWidth; i++)
            for(int j = 0; j < gameHeight; j++)
                buttons[i][j].draw();

    glutSwapBuffers();
}

void mouse(int button, int state, int ax, int ay)
{
    mousePos.x = ax;
    mousePos.y = windowSize.y - ay;
    mouseButton = button;
    mouseState = state;

    for(int i = 0; i < gameWidth; i++)
        for(int j = 0; j < gameHeight; j++)
            if (!(lose || win))
                buttons[i][j].tick(i, j);
}

void initButtons()
{
    firstClick = true;
    lose = false;

    for(int i = 0; i < 256; i++)
        for(int j = 0; j < 128; j++)
            field[i][j] = 0;

    for(int i = 0; i < gameWidth; i++)
        for(int j = 0; j < gameHeight; j++)
        {
            buttons[i][j].init();
            buttons[i][j].x1 = (i + 1) * 16;
            buttons[i][j].y1 = (j + 1) * 16;
            buttons[i][j].x2 = ((i + 1) * 16) + 16;
            buttons[i][j].y2 = ((j + 1) * 16) + 16;
        }
}

void timer(int = 0)
{
    int openCells = 0;

    for(int i = 0; i < gameWidth; i++)
        for(int j = 0; j < gameHeight; j++)
            if(!buttons[i][j].visible && !buttons[i][j].clickable)
                openCells++;

    win = openCells == (gameWidth * gameHeight) - bombs;

    if(win)
        for(int i = 0; i < gameWidth; i++)
            for(int j = 0; j < gameHeight; j++)
                buttons[i][j].flag = buttons[i][j].visible;

    display();
}

void windowResize(int x, int y)
{
    x = (int)(x / 16) * 16;
    y = (int)(y / 16) * 16;

    if(x < 128)
        x = windowSize.x;
    if(y < 64)
        y = windowSize.y;

    windowSize.x = (x / 16) * 16;
    windowSize.y = (y / 16) * 16;

    gameWidth = windowSize.x / 16 - 2;
    gameHeight = windowSize.y / 16 - 2;

    if(gameWidth != lastGameWidth || gameHeight != lastGameHeight)
    {
        initButtons();
        bombs = 0;

        for(int i = 0; i <= gameWidth; i++)
            for(int j = 0; j <= gameHeight; j++)
                field[i][j] = 0;
    }

    lastGameWidth = gameWidth;
    lastGameHeight = gameHeight;

    glutReshapeWindow(x, y);
}

int main(int argc, char **argv)
{
    windowSize.x = 512;
    windowSize.y = 512;
    srand(std::time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(windowSize.x, windowSize.y);
    mainWindow = glutCreateWindow("Minesweeper");
    glutReshapeFunc(windowResize);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glutMouseFunc(mouse);
    glutDisplayFunc(display);
    glutTimerFunc(1, timer, 0);
    glutMainLoop();

    return 0;
}
