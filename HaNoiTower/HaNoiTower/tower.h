#pragma once
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <thread>
#include <chrono>
#include <math.h>

//kich thuoc cua so
#define WID		1000
#define HEI		600

//kih thuoc cua 3 cot stack
#define SW		20
#define SH		400

//chieu cao cua dia
#define DH		30 

//gia tri vi tri ban dau cua stack
#define SX		0
#define SY		0

//gia tri khoang cach tuyet doi giua cac stack
#define DEDIS	300

//gia tri ba dau cua chieu dai dia
#define DEDW	200

//chenh lech do dai giua hai dia ke nhau
#define DX		10

//limit y
#define LY		250
//Time
#define TI 20

//toc do cua Deliverer
#define SP 25

//thoi gian tam hoan thread
#define DT 100


//Disk class
class Disk {
public:
	int dx;
	int dy;
	size_t w;
	Disk(int dx, int dy, size_t w);
	void draw();
};


//Stack class
class Stack {
public:
	int dist;
	int sp;
	std::vector<Disk> unit;
	void push(Stack &st);
	void push(Disk &dk);
	void pop(Stack &st);
	void draw();
	Stack(int dist);

};

class Deliverer {
public:
	Disk *pr;
	int desx;
	int desy;
	bool proc;
	Deliverer(Disk *pr, int desx, int desy);
	~Deliverer();
	void move();
};

//con tro toi 3 stack va kich thuoc cua stack 2
static Stack *sta[3];
static Deliverer *dev;
static size_t n;

//ham nhap kich thuoc cua stack 2
void input();

//ham khoi tao stack, cac dia, mau nen cua so
void iniG();

//ham ve do hoa
void dis();

//ham dieu chinh truc toa do 2D cua cua so
void re(int w, int h);

//ham sap xep cac dia
void misson(int s, Stack *st[3]);

//Tam hoan
 void delay();

//lap
void loopG(int x);