
/*********************{Nam-Minh-Tai}**************************/
/************************OpenGl (vui long cai dat thu vien OpenGl vao may)*************************/
#include <GL/glew.h>
#include <iostream>
#include <GL/freeglut.h>
#include <time.h>
#include <math.h>
#include <thread>
#include <chrono>
#include <fstream>
#include <vector>
#include <SOIL.h> 

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")


#define EW 8
#define EH 6
#define SP 35
#define PI 2*3.14f
#define AN PI/8
/***********************Cau truc Cell************************/

class CE {
public:
	int x;
	int y;
	char st = 'W';
	char h = '0';
	bool clickE = false;
	CE *link = NULL;

	void dr();
};

class Explosion {
public:
	int x;
	int y;
	size_t w;
	size_t h;
	float nw;
	float nh;
	bool ac;
	Explosion(int x, int y, size_t w, size_t h, size_t nw, size_t nh);
	void draw();
private:
	size_t r;
	size_t c;
	size_t inx;
	size_t iny;
	
};




/*********************Bien toan cuc**************************/

static int as_r, n, mi, nw, nx, ny, fmx, fmy, coc;
static short stt, bst;
static bool fst;
static char b1, b2, b3, h, H, L, m, s;
static CE **c;
static unsigned char *img;
static std::vector<Explosion> *bom;
static int wid, hei;
static GLuint g;

/*********************Cac ham**************************/

void expl();
void loadImg(char *src);
char itoc(int x);
void dis();
void ini();
void trunx(CE ***tab, int &x, int nn, int m);
void truny(int *mm, int &y, int m);
void layout();
void iniG();
void des();
void distr();
void distrN();
void re(GLsizei w, GLsizei h);
void drawGrid();
void mouseEv(int key, int st, int x, int y);
void drawMine(int x, int y, bool cl);
void drawNum(int x, int y, char h);
void drawHid(int x, int y);
void drawScore(int x, int y);
void tim();
void drawSt(int x, int y);
void rePaint(int x);
void linkedM(int x, int y);
void rePlay();
void drawF(int x, int y);
void drawRf(int x, int y);
void drawNull(int x, int y);
void openNull(int x, int y);
void gameSt();
void ana();
void drawCM(int x, int y);


/*********************Ham main**************************/

int main(int arg, char **args) {
	srand((unsigned)time(NULL));
	bom = new std::vector<Explosion>;
	stt = 0;
	bst = 0;
	coc = 0;
	fst = false;
	h = '0';
	H = '0';
	L = '0';
	m = '0';
	s = '0';
	loadImg("bomb2.png");
	std::ifstream f("minesweeper.txt");
	if (!f.is_open()) return 0;
	f >> n >> mi;
	f.close();
	if (n <4 || n > 20 || mi> n*n / 4 || mi <= 0) {
		std::cout << "kich thuoc phai >=4 va <=20, so min phai >0 va <= 1/4 lan so o" << std::endl;
		return 0;
	}

	as_r = (600 - (n - 1)) / n;
	nw = as_r*n + n - 1;
	ini();
	if (c) {
		ana();
		distr();
		layout();
		distrN();

		//thiet lap do hoa
		glutInit(&arg, args);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
		glutInitContextProfile(GLUT_CORE_PROFILE);
		glutInitContextVersion(3, 0);
		glutInitWindowPosition(0, 0);
		glutInitWindowSize(650, 700);
		glutCreateWindow("Minesweeper");
		glewInit();
		iniG();
		glutDisplayFunc(dis);
		glutReshapeFunc(re);
		glutMouseFunc(mouseEv);
		rePaint(0);
		glutMainLoop();

	}
	return 1;
}

/*********************Hanh vi cua Cell**************************/
void CE::dr() {
	if (st == 'W') drawHid(x, y);
	else if (st == 'M') drawMine(x, y, clickE);
	else if (st == 'F') drawF(x, y);
	else if (st == '0') drawNull(x, y);
	else drawNum(x, y, h);
}
/***********Constructor**************/
Explosion::Explosion(int x, int y, size_t w, size_t h, size_t nw, size_t nh) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->inx = nw;
	this->iny = nh;
	this->nw = w*1.f / nw;
	this->nh = h*1.f / nh;
	this->r = 0;
	this->c = 0;
	this->ac = true;
}

/****Bom no****/
void Explosion::draw() {
		if (c >= inx) {
			c = 0;
			if (r < iny - 1)
				r++;
			else {
				r = 0;
				ac = false;
				return;
			}
		}
		float dx = c*nw;
		float dy = r*nh;
		
		glColor3f(1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, g);
		glBegin(GL_QUADS);
		glTexCoord2f(dx / w, dy / h); glVertex2i(x - nw / 2, y + nh / 2);
		glTexCoord2f(dx / w, (dy + nh) / h); glVertex2i(x - nw / 2, y - nh / 2);
		glTexCoord2f((dx + nw) / w, (dy + nh) / h); glVertex2i(x + nw / 2, y - nh / 2);
		glTexCoord2f((dx + nw) / w, dy / h); glVertex2i(x + nw / 2, y + nh / 2);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		c++;
}

/****Bom no****/

void expl() {
	if (bom->size() >0) {
		size_t sz = bom->size();
		for (size_t i = 0; i < sz;) {
			(*bom)[i].draw();
			if (!(*bom)[i].ac) {
				bom->erase(bom->begin());
				sz--;
			}
			else i++;
		}
	}
}

/***Load image***/
void loadImg(char *src) {
	img=SOIL_load_image(src,&wid,&hei,NULL,SOIL_LOAD_RGBA);
}

/*********************Hien thi do hoa**************************/

void dis() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	drawScore(0, 630);
	drawCM(0, 660);
	drawSt(nw / 2 - 20, 630);
	drawRf(nw - 40, 630);
	glColor4f(0.9f, 0.9f, 0.9f,1.f);
	drawGrid();
	expl();
	glDisable(GL_BLEND);
	glutSwapBuffers();
}

/*********************Mau nen**************************/

void iniG() {
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glGenTextures(1,&g);
	glBindTexture(GL_TEXTURE_2D,g);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,wid,hei,0,GL_RGBA,GL_UNSIGNED_BYTE,img);
	SOIL_free_image_data(img);
}

/**********************tao bien c*************************/
void ini() {
	c = new CE*[n];
	if (c) {
		for (int i = 0; i < n; i++) {
			c[i] = new CE[n];
		}
	}
}

/********************Huy bien C**************************/
void des() {
	for (int i = 0; i < n; i++) {
		delete[] c[i];
	}
	delete[] c;
	c = NULL;
}


/*********************Thay doi kich thuoc cua so**************************/

void re(GLsizei w, GLsizei h) {
	w = (w - nw) / 2;
	h = (h - 700) / 2;
	nx = w;
	ny = h;
	glViewport(w, h, nw, 700);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., nw, 700., 0.);
}

/***********************Ve cell************************/

void drawGrid() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			c[i][j].dr();
		}
	}
}

/**********************Cai gi se xay ra khi nhan nut trai chuot*************************/

void mouseEv(int key, int st, int x, int y) {
	if (key == GLUT_LEFT_BUTTON && st == GLUT_DOWN) {
		int cx = x - nx, cy = y - ny;
		if (cx >= 0 && cx <= nw && cy >= 0 && cy <= nw && bst == 0) {
			int dx = cx / (as_r + 1), dy = cy / (as_r + 1);
			if (c[dy][dx].st != c[dy][dx].h) {
				if (!fst) {
					if (c[dy][dx].st != 'F') {
						coc++;
						c[dy][dx].st = c[dy][dx].h;
						if (c[dy][dx].st == 'M') {
							bst = -1;
							c[dy][dx].clickE = true;
							linkedM(dx, dy);
							Explosion bo(cx,cy,wid,hei,EW,EH);
							bom->push_back(bo);
						}
						else if (c[dy][dx].st == '0') openNull(dx, dy);
						if (coc == n*n - mi && bst == 0) gameSt();
					}
				}
				else {
					if (c[dy][dx].st == 'F') c[dy][dx].st = 'W';
					else if (c[dy][dx].st == 'W') c[dy][dx].st = 'F';
				}
				if (stt == 0) {
					stt = 1;
					std::thread t(tim);
					t.detach();
				}

			}
		}
		else {
			int dx = nw / 2 - 20;

			if (cx >= dx && cx <= dx + 40 && cy >= 600 && cy <= 640) {
				rePlay();
			}
			else {
				dx = nw - 40;
				if (cx >= dx && cx <= dx + 45 && cy >= 600 && cy <= 640) {
					if (!fst) fst = true;
					else fst = false;
				}

			}
		}
	}
}

/**********************Ve so*************************/

void drawNum(int x, int y, char h) {
	glColor4f(0.8f, 0.8f, 0.8f,1.f);
	glRecti(x, y, x + as_r, y + as_r);
	switch (h) {
	case '1':
		glColor4f(0.f, 0.f, 1.f,1.f);
		break;
	case '2':
		glColor4f(0.f, 179.f / 255, 0.f,1.f);
		break;
	case '3':
		glColor4f(1.f, 0.f, 0.f,1.f);
		break;
	case '4':
		glColor4f(0.f, 0.f, 102.f / 255,1.f);
		break;
	case '5':
		glColor4f(1.f, 0.f, 102.f / 255,1.f);
		break;
	case '6':
		glColor4f(153.f / 255, 0.f, 0.f,1.f);
		break;
	case '7':
		glColor4f(102.f / 255, 102.f / 255, 51.f / 255,1.f);
		break;
	case '8':
		glColor4f(0.f, 0.f, 0.f,1.f);
	}
	glRasterPos2i(x + as_r / 2 - 5, y + as_r / 2 + 10);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, h);

}
/**********************Ve khoi mac dinh*************************/

void drawHid(int x, int y) {
	int cx = as_r - as_r / 7, xx = x + as_r, yy = y + as_r;
	glColor4f(1.f, 1.f, 1.f,1.f);
	glBegin(GL_TRIANGLES);
	glVertex2i(x, y);
	glVertex2i(xx, y);
	glVertex2i(x, yy);
	glEnd();
	glColor4f(0.4f, 0.4f, 0.4f,1.f);
	glBegin(GL_TRIANGLES);
	glVertex2i(xx, y);
	glVertex2i(x, yy);
	glVertex2i(xx, yy);
	glEnd();
	glColor4f(0.8f, 0.8f, 0.8f,1.f);
	glRecti(xx - cx, yy - cx, x + cx, y + cx);
}
/***********************Ve diem so************************/

void drawScore(int x, int y) {
	glColor4f(1.f, 0.f, 0.f,1.f);
	glRasterPos2i(x, y);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, L);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, H);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, h);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, m);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '.');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s);
}
/**********************Ve trang thai game*************************/

void drawSt(int x, int y) {
	glRasterPos2i(x, y);
	if (bst == 0) {
		glColor4f(0.f, 1.f, 1.f,1.f);
		glRasterPos2i(x, y);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '^');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '_');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '^');
	}
	else if (bst == -1) {
		glColor4f(1.f, 191.f / 255, 0.f,1.f);
		glRasterPos2i(x, y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'T');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '_');
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'T');
	}
	else {
		glColor4f(1.f, 1.f, 1.f,1.f);
		glRasterPos2i(x, y);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '^');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'o');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '^');
	}
}
/**********************Chay thoi gian*************************/

void tim() {
	while (bst == 0) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		s += 1;
		if (s > '9') {
			s = '0';
			m++;
			if (m > '9') {
				m = '0';
				h++;
				if (h > '9') {
					h = '0';
					H++;
					if (H > '9') {
						H = '0';
						L++;
						if (L > '9') {
							L = '9'; bst = -1;
						}
					}
				}
			}
		}
	}
}
/***********************Ve lai****************************/
void rePaint(int x) {
	glutPostRedisplay();
	glutTimerFunc(SP, rePaint, 0);
}
/***********************Phan phoi toa do****************************/
void distr() {
	int s, t, o = 0, u;
	for (int i = 0; i < n; i++) {
		s = i*as_r + o;
		u = 0;
		for (int j = 0; j < n; j++) {
			t = j*as_r + u;
			c[i][j].x = t; c[i][j].y = s;
			u++;
		}
		o++;
	}
}
/**********************Phan phoi min*****************************/
void layout() {
	int u, v;
	CE ***tab, *l, *fs;
	int *nn, *mm, m = n;
	nn = new int[n];
	mm = new int[n];
	tab = new CE**[n];
	for (int i = 0; i < n; i++) {
		tab[i] = new CE*[n];
		nn[i] = n;
		mm[i] = i;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			tab[i][j] = &c[i][j];
		}
	}
	u = rand() % m;
	v = rand() % nn[mm[u]];
	fmx = v; fmy = mm[u];
	l = tab[fmy][fmx];
	fs = l;
	l->h = 'M';
	trunx(tab, nn[mm[u]], v, mm[u]);
	for (int i = 1; i < mi; i++) {
		u = rand() % m;
		v = rand() % nn[mm[u]];

		l->link = tab[mm[u]][v];
		l = l->link;
		l->h = 'M';
		trunx(tab, nn[mm[u]], v, mm[u]);
		if (nn[mm[u]] == 0) truny(mm, m, u);
	}
	l->link = fs;

	for (int i = 0; i < n; i++) {
		delete[] tab[i];
	}
	delete[] tab;
	delete[] nn;
	delete[] mm;
}
/***********************Xoa X****************************/
void trunx(CE ***tab, int &x, int nn, int m) {
	for (int i = nn; i < x; i++) {
		tab[m][i] = tab[m][i + 1];
	}
	x--;
}
/***********************Xoa Y****************************/
void truny(int *mm, int &y, int m) {
	for (int i = m; i < y; i++) {
		mm[i] = mm[i + 1];
	}
	y--;
}
/***********************Phan phoi so****************************/
void distrN() {
	CE *l;
	int x, y, xt, yt;
	l = &c[fmy][fmx];
	for (int i = 0; i < mi; i++) {
		x = l->x / (as_r + 1); y = l->y / (as_r + 1);
		for (int j = -1; j <= 1; j++) {
			for (int k = -1; k <= 1; k++) {
				xt = x + j; yt = y + k;
				if ((j == 0 && k == 0) || xt < 0 || yt < 0 || xt >= n || yt >= n) continue;
				if (c[yt][xt].h == 'M') continue;
				c[yt][xt].h++;
			}
		}
		l = l->link;
	}
}
/***********************Danh sach lien ket min****************************/
void linkedM(int x, int y) {
	CE *l;
	l = c[y][x].link;
	for (int i = 1; i < mi; i++) {
		l->st = 'M';
		l = l->link;
	}
}
/***********************Ve co****************************/
void drawF(int x, int y) {
	int cy = as_r - as_r / 7, xx = x + as_r, yy = y + as_r, cr = as_r - 2 * as_r / 7;
	int cx = cr / 3, x_i = xx - cy + cr / 2, y_i = yy - cy + cr / 4;

	glColor4f(1.f, 1.f, 1.f,1.f);
	glBegin(GL_TRIANGLES);
	glVertex2i(x, y);
	glVertex2i(xx, y);
	glVertex2i(x, yy);
	glEnd();
	glColor4f(0.4f, 0.4f, 0.4f,1.f);
	glBegin(GL_TRIANGLES);
	glVertex2i(xx, y);
	glVertex2i(x, yy);
	glVertex2i(xx, yy);
	glEnd();
	glColor4f(0.8f, 0.8f, 0.8f,1.f);
	glRecti(xx - cy, yy - cy, x + cy, y + cy);

	glColor4f(1.f, 0.f, 0.f,1.f);
	glRecti(x_i, y_i, x_i + cx, y_i + cx);
	glLineWidth(2);
	glColor4f(0.f, 0.f, 0.f,1.f);
	glBegin(GL_LINES);
	glVertex2i(x_i, y_i);
	glVertex2i(x_i, y + cy);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2i(x_i, y + cy - cr / 8);
	glVertex2i(x_i - cr / 4, y + cy);
	glVertex2i(x_i + cr / 4, y + cy);
	glEnd();
}
/***********************Ve dang ky co****************************/
void drawRf(int x, int y) {
	if (!fst) glColor4f(0.f, 1.f, 1.f,1.f);
	else glColor4f(1.f, 1.f, 0.f,1.f);
	glRasterPos2i(x, y);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'F');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'l');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'a');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'g');
}
/***********************Ve cell rong****************************/
void drawNull(int x, int y) {
	glColor4f(0.8f, 0.8f, 0.8f,1.f);
	glRecti(x, y, x + as_r, y + as_r);
}
/***********************Choi lai****************************/
void rePlay() {
	if (bst != 0) {
		bst = 0;
		coc = 0;
		stt = 0;
		h = '0', H = '0', m = '0', s = '0';
		des();
		ini();
		distr();
		layout();
		distrN();
	}
	else bst = -1;

}
/************************Mo cac cell rong***************************/
void openNull(int x, int y) {
	int len = 0, max = n*n - mi, xt, yt;
	int **e;
	e = new int*[max];
	for (int i = 0; i < max; i++) {
		e[i] = new int[2];
	}
	e[0][0] = x;
	e[0][1] = y;
	for (int i = 0; i <= len; i++) {
		x = e[i][0]; y = e[i][1];
		for (int j = -1; j < 2; j++) {
			for (int k = -1; k < 2; k++) {
				xt = x + k; yt = y + j;
				if ((j == 0 && k == 0) || xt < 0 || yt < 0 || xt >= n || yt >= n) continue;
				if (c[yt][xt].st == c[yt][xt].h) continue;
				coc++;
				c[yt][xt].st = c[yt][xt].h;
				if (c[yt][xt].h == '0') {
					len++;
					e[len][0] = xt;
					e[len][1] = yt;
				}
			}
		}
	}
	for (int i = 0; i < max; i++) {
		delete[] e[i];
	}
	delete[] e;
}
/***********************Thang tran thi lam gi****************************/
void gameSt() {
	c[fmy][fmx].st = 'M';
	linkedM(fmx, fmy);
	bst = 1;
}
/**********************Ve min*************************/

void drawMine(int x, int y, bool cl) {
	float f = 0, r = as_r / 2.5f, dx = x + as_r / 2.f, dy = y + as_r / 2.f;
	if (!cl)
		glColor4f(0.8f, 0.8f, 0.8f,1.f);
	else glColor4f(1.f, 191.f / 255, 0.f,1.f);
	glRecti(x, y, x + as_r, y + as_r);
	glColor4f(0.f, 0.f, 0.f,1.f);
	glBegin(GL_POLYGON);
	while (f<PI) {
		glVertex2f(dx + r*cos(f), dy + r*sin(f));
		f = f + AN;
	}
	glEnd();
	glColor4f(1.f, 1.f, 1.f,1.f);
	glRectf(x + as_r / 3.2f, y + as_r / 3.2f, x + as_r / 2.2f, y + as_r / 2.2f);
}
/************************Chuyen so thanh ky tu********************************/
char itoc(int x) {
	switch (x) {
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	default:
		return '\0';
	}
}

/**********************Phan tich phan du**************************/
void ana() {
	b3 = itoc(mi % 10);
	b2 = itoc((mi / 10) % 10);
	b1 = itoc((mi / 100) % 10);
}

/**************************So luong min************************************/
void drawCM(int x, int y) {
	glColor4f(1.f, 0.f, 0.f,1.f);
	glRasterPos2i(x, y);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, b1);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, b2);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, b3);

}
