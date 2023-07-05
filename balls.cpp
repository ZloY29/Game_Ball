#include <iostream>
#include <cmath>
#include  <cstdio>
#include <time.h>

#include <OpenGL/gl.h>
#include <GLUT/glut.h>
// Прототипы глобальных функций
void RenderScene(void); // вызывается при перерисовке экрана
void SetupRC(void); //отрисовка сцены
void ChangeSize(int, int);
void TimerFunction(int);
void keyboardFunc(unsigned char, int, int);

// Глобальные переменные
const int COUNTB = 10;
double R = 10, step, alpha;
bool flag = 0;
int i1 = 0;
double ArrBall[COUNTB][6];
double Arr[COUNTB];

// Текущая ширина и высота клиентской области окна приложения
GLfloat windowsWidth = 100;
GLfloat windowsHeigh = 100;

// Функция отрисовывания треугольника
void DrawRect(float x1, float y1, float x2, float y2, int fillflag)
{
    glBegin(fillflag ? GL_QUADS : GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

// Функция отрисовки отрезка
void DrawLine(float x1, float y1, float x2, float y2)
{
    glBegin(GLU_LINE);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();
}

// Функция отображения окружности
void DrawCircle(float x, float y, float R, int fillflag)
{
    int N = 40;
    glBegin(fillflag ? GL_TRIANGLE_FAN : GL_LINE_LOOP);
    for (int i = 0; i < N; ++i) {
        glVertex2f(x+R*cos(2*3.14/N*i),y+R*sin(2*3.14/N*i));
    }
    glEnd();
}


int main(int argc, char* argv[]) {
    srand(time(NULL));
    for (int i = 0; i < COUNTB; ++i) {
        bool f = true;
        while (f) {
            f = false;
            ArrBall[i][0] = -windowsWidth + R + rand() % (2*(int)(windowsWidth - 2 * R));
            ArrBall[i][1] = -windowsHeigh + R + rand() % (2*(int)(windowsHeigh - 2 * R));
            for (int j = 0; j < i; ++j) {
                double d = pow(ArrBall[i][0] - ArrBall[j][0], 2) +
                        pow(ArrBall[i][1] - ArrBall[j][1], 2);
                if (d < 4 * R * R)
                    f = true;
            }
        }
        Arr[i] = rand() % 360;
        ArrBall[i][3] = (double)rand() / RAND_MAX;
        ArrBall[i][4] = (double)rand() / RAND_MAX;
        ArrBall[i][5] = (double)rand() / RAND_MAX;
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Bounce");
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(keyboardFunc);

    glutTimerFunc(10, TimerFunction, 1);
    SetupRC();
    glutMainLoop();
}

void RenderScene(void){
    // очистка окна текущим цветом
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < COUNTB; ++i) {
        glColor3f(ArrBall[i][3], ArrBall[i][4], ArrBall[i][5]);
        DrawCircle(ArrBall[i][0], ArrBall[i][1], R, 1);

    }
    // переключение буффера с очисткой команд
    glutSwapBuffers();
}
// вызывается по таймеру библиотеки глют в спокойном состоянии
// когда не меняются размеры окна приложения и нет перемещений параметра - номер таймера
void TimerFunction(int value){
    if (flag){
        for (int i = 0; i < COUNTB; ++i) {
            double AlphaPi = Arr[i] * 3.14 / 180;
            double xt = ArrBall[i][0];
            double yt = ArrBall[i][1];
            int alpha = Arr[i];

            xt = xt + step * cos(AlphaPi);
            yt = yt + step * sin(AlphaPi);
            if ((yt + R >= 100) || (yt - R <= -100))
                alpha = 360 - alpha;
            if ((xt + R >= 100) || (xt - R <= -100))
                alpha = 180 - alpha;
            if (alpha < 0)
                alpha += 360;
            if (alpha > 360)
                alpha -= 360;
            ArrBall[i][0] = xt;
            ArrBall[i][1] = yt;
            Arr[i] = alpha;
        }
        // столкновения
        for (int i = 0; i < COUNTB; ++i) {
            for (int j = i + 1; j < COUNTB; ++j) {
                double d = pow(ArrBall[i][0] - ArrBall[j][0], 2) +
                           pow(ArrBall[i][1] - ArrBall[j][1], 2);
                if (d <= 4 * R * R){
                    double k = ((ArrBall[i][1] - ArrBall[j][1] /
                            ArrBall[i][0] - ArrBall[j][0]));
                    double ugol = 180 * atan(k) / 3.14;
                    Arr[i] = 180 + 2 * ugol - Arr[i];
                    Arr[j] = 180 + 2 * ugol - Arr[j];
                    if (Arr[i] < 0)
                        Arr[i] += 360;
                    if (Arr[j] < 0)
                        Arr[j] += 360;
                    if (Arr[i] > 360)
                        Arr[i] -= 360;
                    if (Arr[j] > 360)
                        Arr[j] -= 360;
                    ArrBall[i][0] += step*cos(Arr[i] * 3.14 / 180);
                    ArrBall[i][0] += step*sin(Arr[i] * 3.14 / 180);
                    ArrBall[i][0] += step*cos(Arr[j] * 3.14 / 180);
                    ArrBall[i][0] += step*sin(Arr[j] * 3.14 / 180);
                }
            }
        }
    }
    // перерисовка сцены с новыми координатами
    glutPostRedisplay();
    // запуск нового таймера
    glutTimerFunc(10, TimerFunction, 1);
}

// установка состояния редеринга (визуализация)
void SetupRC(void){
    // установка синего цвета очистки
    glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
}

// вызывается библиотекой глют при изменении размеров окна приложения
void ChangeSize(int width, int height){
    GLfloat aspectRatio;
    // предотвращаем деление на ноль
    if (height == 0)
        height = 1;
    if (width == 0)
        width = 1;

    // установка поля просмотра с размерами окна
    glViewport(0, 0, width, height);

    // устанавливаем матрицу в режим проецирования
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // устанавливаем размер перспективы(отсекающего объем
    // (left, right, bottom, top, near, far)
    aspectRatio = (GLfloat)width / (GLfloat)height;
    if (width <= height){
        windowsWidth = 100;
        windowsHeigh = 100/ aspectRatio;
        glOrtho(-100.0, 100.0, -windowsHeigh, windowsHeigh, 1.0, -1.0);
    }
    else {
        windowsWidth = 100 * aspectRatio;
        windowsHeigh = 100;
        glOrtho(-windowsHeigh, windowsHeigh, -100.0, 100.0, 1.0, -1.0);
    }
    // восстановление матрицы преобразования в исходный режим
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void keyboardFunc(unsigned char key, int x, int y){
    if (key == 27)
        exit(0);
    if ((key == ' ') && (!flag)){
        flag = 1;
        step = 0.5;
    }
}

