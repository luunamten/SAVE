#include "tower.h"
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")


int main(int arg, char **args) {
	input();
	glutInit(&arg,args);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextVersion(3,0);
	glutInitWindowSize(WID,HEI);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Ha Noi Tower");
	iniG();
	glutReshapeFunc(re);
	glutDisplayFunc(dis);
	loopG(0);
	glutMainLoop();
	return 1;
}

