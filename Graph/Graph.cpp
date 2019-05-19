
/* Chuong trinh co su dung thu vien do hoa OpenGl */

/******************************Nam-Minh-Tai*********************************/

#include <GL/glut.h>		/* OpenGl library, glut.h khong phai thu vien chuan cua C/C++, nen phai tu cai dat vao may */
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <thread>


#define PI 3.14f
#define R 3/3.5f
#define R_S R/16

/******************************Point Definition*********************************/

struct G {
	float x;
	float y;
};

/******************************Global Variable*********************************/
static int si, op[50], ops, pa[50][50], pas, ii, jj;
static G *g;
static bool **a;

/******************************Functions************************************/
void des();
void dis();
void iniSG();
bool check();
void ini();
bool checkType();
void iniG();
void disText();
void disGr();
void disAr();
void disPt();
void re(GLsizei w, GLsizei h);
bool ro(int x, int y);
void del(int *x, int n, int &m);
void tran(int *x, int n, int m = -1);
void drR();
bool con();
void th_a(int *ii, int *jj);
void idle(int x);

/******************************Main function*********************************/

int main(int arg, char **args) {
	bool b;
	std::fstream f;


	f.open("graphinfo.txt", std::ios::in);
	if (f) {
		f >> si;
		ini();
		if (a) {
			for (int i = 0; i < si; i++) {
				for (int j = 0; j < si; j++) {
					f >> a[i][j];
				}
			}
			f.close();
			b = check();

			if (b) {
				iniG();
				if (g) {
					std::cout << "Hop le" << std::endl;
					b = checkType();
					if (b) std::cout << "Do thi huu huong" << std::endl;
					else std::cout << "Do thi vo huong" << std::endl;
					b = con();
					if (b) std::cout << "Do thi lien thong" << std::endl;
					else std::cout << "Do thi khong lien thong" << std::endl;
					for (int i = 0; i < pas; i++) {
						std::cout << "Thanh phan lien thong " << i + 1 << ": ";
						for (int j = 1; j <= pa[i][0]; j++) {
							std::cout << pa[i][j] << " ";
						}
						std::cout << std::endl;
					}
					std::cout << "------------------------------------------------------------------------------------" << std::endl;
					std::cout << "| SAU KHI NHAP XONG, VUI LONG CHUYEN FOCUS SANG CUA SO GRAHP DE THAY DUOC HIEU UNG |" << std::endl;
					std::cout << "------------------------------------------------------------------------------------" << std::endl;
					std::thread t(th_a, &ii, &jj);  /**Create a Thread Object**/
					t.detach();

					/******OpenGl Functions******/
					glutInit(&arg, args);
					glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
					glutInitWindowPosition(0, 0);
					glutInitWindowSize(700, 700);
					glutCreateWindow("GRAPH");
					iniG();
					glutDisplayFunc(dis);
					idle(0);
					atexit(des);
					glutReshapeFunc(re);
					glutMainLoop();
					/****************************/
				}

			}
			else {
				std::cout << "Khong hop le" << std::endl;

				for (int i = 0; i < si; i++) {
					delete[] a[i];
				}
				delete[] a;
			}

		}
		else f.close();
	}
	return 1;
}

/********************Destroy*********************/
void des() {
	delete[] g;
	for (int i = 0; i < si; i++) {
		delete[] a[i];
	}
	delete[] a;
}
/*****************************Displaying Graph**********************************/
void dis() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(1);
	glColor3f(51.0f / 255, 204.0f / 255, 1.0f);
	glPointSize(5);
	glBegin(GL_POINTS);
	disPt();
	glEnd();
	glBegin(GL_LINES);
	disGr();
	glColor3f(102.0f / 255, 1.0f, 51.0f / 255);
	drR();
	glColor3f(1.0f, 1.0f, 1.0f);
	disAr();
	glEnd();
	disText();
	glutSwapBuffers();
}
/******************************Secret*********************************/

void iniSG() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
/******************************Secret*********************************/

void iniG() {
	float f = 2 * PI / si, f_s = PI / 4;
	g = new G[si];
	for (int i = 0; i < si; i++) {
		g[i].x = R*cos(f_s);
		g[i].y = R*sin(f_s);
		f_s = f_s + f;
	}

}

/*****************************Whether valid or not**********************************/

bool check() {
	for (int i = 0; i < si; i++) {
		if (a[i][i] != 0) return false;
	}
	for (int i = 0; i < si; i++) {
		for (int j = 0; j < si; j++) {
			if (a[i][j] != 0 && a[i][j] != 1) return false;
		}
	}
	return true;
}
/******************************Secret*********************************/

void ini() {
	a = new bool *[si];
	for (int i = 0; i < si; i++) {
		a[i] = new bool[si];
	}
}
/*******************************Type of Graph********************************/

bool checkType() {
	for (int i = 0; i < si; i++) {
		for (int j = i + 1; j < si; j++) {
			if ((a[i][j] == 1 && a[j][i] != 1) || (a[i][j] != 1 && a[j][i] == 1)) return true;
		}
	}
	return false;
}
/*******************************Create text********************************/

void disText() {
	char c = '0', b = '0';
	if (si < 10) {
		for (int i = 0; i < si; i++) {
			glRasterPos2f(g[i].x, g[i].y);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c++);
		}
	}
	else {
		for (int i = 0; i < 10; i++) {
			glRasterPos2f(g[i].x, g[i].y);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c++);
		}
		c = '0'; b++;
		for (int i = 10; i < si; i++) {
			glRasterPos2f(g[i].x, g[i].y);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, b);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c++);
			if (c>'9') {
				c = '0'; b++;
			}
		}
	}
}

/*******************************Create lines********************************/

void disGr() {
	for (int i = 0; i < si; i++) {

		for (int j = i + 1; j < si; j++) {
			if (a[i][j] == 1 || a[j][i] == 1) {
				glVertex2f(g[i].x, g[i].y);
				glVertex2f(g[j].x, g[j].y);
			}
		}
	}
}
/*******************************Create arrows********************************/

void disAr() {
	float xc, yc, xb, yb, va, vb, vra, vrb, r;
	for (int i = 0; i < si; i++) {
		for (int j = i + 1; j < si; j++) {
			if (a[j][i] == 1 && a[i][j] != 1) {
				xc = (g[j].x + g[i].x) / 2;
				yc = (g[j].y + g[i].y) / 2;
				va = (g[i].x - g[j].x);
				vb = (g[i].y - g[j].y);
				r = sqrt(va*va + vb*vb);
				va = -va*R_S / r;
				vb = -vb*R_S / r;
				vra = va*cos(PI / 6) + vb*sin(PI / 6);
				vrb = vb*cos(PI / 6) - va*sin(PI / 6);
				xb = xc + vra;
				yb = yc + vrb;
				glVertex2f(xb, yb);
				glVertex2f(xc, yc);
				vra = va*cos(-PI / 6) + vb*sin(-PI / 6);
				vrb = vb*cos(-PI / 6) - va*sin(-PI / 6);
				xb = xc + vra;
				yb = yc + vrb;
				glVertex2f(xb, yb);
				glVertex2f(xc, yc);
			}
			else if (a[i][j] == 1 && a[j][i] != 1) {
				xc = (g[i].x + g[j].x) / 2;
				yc = (g[i].y + g[j].y) / 2;
				va = (g[j].x - g[i].x);
				vb = (g[j].y - g[i].y);
				r = sqrt(va*va + vb*vb);
				va = -va*R_S / r;
				vb = -vb*R_S / r;
				vra = va*cos(PI / 6) + vb*sin(PI / 6);
				vrb = vb*cos(PI / 6) - va*sin(PI / 6);
				xb = xc + vra;
				yb = yc + vrb;
				glVertex2f(xb, yb);
				glVertex2f(xc, yc);
				vra = va*cos(-PI / 6) + vb*sin(-PI / 6);
				vrb = vb*cos(-PI / 6) - va*sin(-PI / 6);
				xb = xc + vra;
				yb = yc + vrb;
				glVertex2f(xb, yb);
				glVertex2f(xc, yc);
			}
		}
	}
}

/*******************************Create points********************************/
void disPt() {
	for (int i = 0; i < si; i++) {
		glVertex2f(g[i].x, g[i].y);
	}
}

/*******************************Adjust coords of Graph********************************/

void re(GLsizei w, GLsizei h) {
	float ra;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h == 0) h = 1;
	ra = w*1.0f / h;
	if (w >= h) gluOrtho2D(-1.0f*ra, 1.0f*ra, -1.0f, 1.0f);
	else gluOrtho2D(-1.0f, 1.0f, -1.0f / ra, 1.0f / ra);
}

/******************************Whether have route or not*********************************/
bool ro(int x, int y) {
	int op_a[50], op_b[50][2], opb = 1, t = si - 1, in;
	op_b[0][0] = x;
	op_b[0][1] = 0;
	ops = 0;
	tran(op_a, t, x);
	for (int i = 0; i < opb; i++) {
		in = 0;
		while (in <t) {
			if (a[op_b[i][0]][op_a[in]] == 1) {
				op_b[opb][0] = op_a[in];
				op_b[opb][1] = i;
				if (op_b[opb][0] == y) {
					for (int j = opb; j > 0;) {
						op[ops] = op_b[j][0]; j = op_b[j][1];
						ops++;
					}
					op[ops] = x;
					return true;
				}
				opb++;
				del(op_a, in, t);
			}
			else in++;
		}
	}
	return false;
}
/*****************************Secret**********************************/

void tran(int *x, int n, int m) {
	int i = 0, j = 0;
	if (m != -1) {
		while (i != m) {
			x[j] = i;
			i++; j++;
		}
		i++;
	}
	while (j<n) {
		x[j] = i;
		i++; j++;
	}
}

/*****************************Secret**********************************/

void del(int *x, int n, int &m) {
	for (int i = n; i < m; i++) {
		x[i] = x[i + 1];
	}
	m--;
}
/******************************Draw route*********************************/

void drR() {
	for (int i = ops; i >0; i--) {
		glVertex2f(g[op[i]].x, g[op[i]].y);
		glVertex2f(g[op[i - 1]].x, g[op[i - 1]].y);
	}
}

/*******************************Whether connected or not********************************/
bool con() {
	int op_a[50], t = si, in, p = 0;
	pas = 0;
	tran(op_a, t);
	while (t > 0) {
		pa[p][0] = 1;
		pa[p][1] = op_a[0];
		pas++;
		del(op_a, 0, t);
		for (int i = 1; i <= pa[p][0]; i++) {
			in = 0;
			while (in <t) {
				if (a[pa[p][i]][op_a[in]] == 1 || a[op_a[in]][pa[p][i]] == 1) {
					pa[p][0]++;
					pa[p][pa[p][0]] = op_a[in];
					del(op_a, in, t);
				}
				else in++;
			}
		}
		p++;
	}
	if (pas == 1) return true;
	return false;
}

/*******************************Thread********************************/
void th_a(int *ii, int *jj) {
	bool b;
	while (true) {

		std::cout << "nhap hai dinh < " << si << std::endl;

		scanf("%d %d", ii, jj);
		if (*ii >= si || *ii < 0 || *jj >= si || *jj < 0) {
			std::cout << "Nhap loi " << std::endl;
			fflush(stdin);
			continue;
		}
		if (*ii == *jj) {
			ops = 0; op[0] = *ii;
			b = true;
		}
		else b = ro(*ii, *jj);
		if (b) std::cout << *ii << " co duong di den " << *jj << std::endl;
		else std::cout << *ii << " khong co duong di den " << *jj << std::endl;
	}
}
/*******************************Secret*************************************/
void idle(int x) {
	glutPostRedisplay();
	glutTimerFunc(50, idle, 0);
}
