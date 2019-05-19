#include "tower.h"


//constructor cua class Stack
Stack::Stack(int dist) {
	this->dist = dist;
	this->sp = -SH / 2;
}


//dua dia vao Stack
void Stack::push(Stack &st) {
	this->unit.push_back(st.unit.back());
	st.unit.pop_back();
	this->sp += DH;
	st.sp -= DH;
	dev = new Deliverer(&this->unit.back(), this->dist + SX, this->sp - DH / 2);
	delay();
}

//dua dia vao Stack
void Stack::push(Disk &dk) {
	this->unit.push_back(dk);
	this->sp += DH;
	(*(this->unit.end() - 1)).dy = this->sp - DH / 2;
	(*(this->unit.end() - 1)).dx = this->dist + SX;
}


//Lay dia ra khoi Stack va chuyen qua  Stack khac
void Stack::pop(Stack &st) {
	st.unit.push_back(this->unit.back());
	this->unit.pop_back();
	st.sp += DH;
	this->sp -= DH;
	dev = new Deliverer(&st.unit.back(), st.dist + SX, st.sp - DH / 2);
	delay();
}

//ham ve Stack
void Stack::draw() {
	glBegin(GL_QUADS);
	glColor4d(0, 0, 0, 1);
	glVertex2i(SX - SW / 2 + this->dist, SY + SH / 2);
	glColor4d(0, 0, 0, 1);
	glVertex2i(SX - SW / 2 + this->dist, SY - SH / 2);
	glColor4d(1, 1, 1, 1);
	glVertex2i(SX + SW / 2 + this->dist, SY - SH / 2);
	glColor4d(1, 1, 1, 1);
	glVertex2i(SX + SW / 2 + this->dist, SY + SH / 2);
	glEnd();
}


//constructor cua class Disk
Disk::Disk(int dx, int dy, size_t w) {
	this->dx = dx;
	this->dy = dy;
	this->w = w;
}

//ham ve Disk
void Disk::draw() {
	glBegin(GL_QUADS);
	glColor4d(0, 0, 0, 1);
	glVertex2i(this->dx - this->w / 2, this->dy + DH / 2);
	glColor4d(0, 0, 0, 1);
	glVertex2i(this->dx - this->w / 2, this->dy - DH / 2);
	glColor4d(1, 1, 163. / 255, 1);
	glVertex2i(this->dx + this->w / 2, this->dy - DH / 2);
	glColor4d(1, 1, 163. / 255, 1);
	glVertex2i(this->dx + this->w / 2, this->dy + DH / 2);
	glEnd();
}

//constructor cua class Deliverer
Deliverer::Deliverer(Disk *pr, int desx, int desy) {
	this->pr = pr;
	this->desx = desx;
	this->desy = desy;
	this->proc = true;
}
Deliverer::~Deliverer() {}

//di chuyen Disk
void Deliverer::move() {
	
	int dist = this->desx-this->pr->dx;
	int adist = abs(dist);
	int vec = (dist==adist) ? 1: -1;
	if (adist >0 && this->pr->dy < LY) {

		this->pr->dy+=SP;
		if (this->pr->dy > LY) this->pr->dy = LY;
		
	}
	else if (adist>0 && this->pr->dy == LY) {
		if (adist<SP) this->pr->dx = this->desx;
		else this->pr->dx+= vec * SP;
	}
	else if (adist==0 && this->pr->dy > this->desy) {
		this->pr->dy-=SP;
		if (this->pr->dy < this->desy) this->pr->dy = this->desy;
		if (this->pr->dy == this->desy) this->proc = false;
	}
	
	
	
}

//ham nhap kich thuoc cua Stack 2
void input() {
	std::cin >> n;
	if (n < 2 || n>=10) {
		std::cout << "phai nhap >= 2, <10";
		abort();
	}
	if (std::cin.fail()) abort();
}

//ham khoi tao Stack, cac Disk, mau nen
void iniG() {
	sta[0] = new Stack(0);
	sta[1] = new Stack(DEDIS);
	sta[2] = new Stack(-DEDIS);
	for (size_t i = 0; i < n; i++) {
		Disk ds(0, 0, DEDW - 2 * i*DX);
		sta[2]->push(ds);
	}
	glClearColor(0, 0, 0, 1);
	std::thread t(misson, n, sta);
	t.detach();
}


//ham ve do hoa
void dis() {
	glClear(GL_COLOR_BUFFER_BIT);
	if (dev !=NULL) {
		dev->move();
	}
	for (int i = 0; i < 3; i++) {
		sta[i]->draw();
	}
	for (int i = 0; i < 3; i++) {
		size_t s = sta[i]->unit.size();
		for (size_t j = 0; j < s; j++) {
			sta[i]->unit[j].draw();
		}
	}
	glutSwapBuffers();
}

//ham dieu chinh truc toa do 2D cua cua so
void re(int w, int h) {
	glViewport((w - WID) / 2, (h - HEI) / 2, WID, HEI);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-WID / 2, WID / 2, -HEI / 2, HEI / 2);
}

//Ham sap xep cac dia
void misson(int s, Stack *st[3]) {
	int subs = 2;
	int pos1 = (s / 2. - s / 2) * 2;
	s--;
	int pos2 = (s / 2. - s / 2) * 2;
	s -= 2;
	st[2]->pop(*st[pos1]);
	st[2]->pop(*st[pos2]);
	st[pos1]->pop(*st[pos2]);
	for (int i = s; i >= 0; i--) {
		int pos = ((i + 1) / 2. - (i + 1) / 2) * 2;
		int pos3;
		if (pos == 1) {
			pos3 = 0;
		}
		else pos3 = 1;
		st[2]->pop(*st[pos]);
		Stack *p[3] = { st[2],st[pos],st[pos3] };
		misson(subs,p);
		subs++;
	}
	
}

//tam hoan
void delay() {
	while (dev->proc) {
		std::this_thread::sleep_for(std::chrono::milliseconds(DT));
	}
	dev->~Deliverer();
	dev =NULL;
}

//lap
void loopG(int x) {
	glutPostRedisplay();
	glutTimerFunc(TI, loopG, 0);
}


