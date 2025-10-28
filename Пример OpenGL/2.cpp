#include <math.h>
#include "glut.h"

int rotX = 0, rotY = 0, rotZ = 0;
float rotC2 = 0.0f, rotC3 = 0.0f, rotC4 = 0.0f;
int ex = 0;
float smRad = 0.38f;
float smRadH = 0.15f;
float smRadE = 0.025f;
float smNoseHeight = 0.35f;
float smRadNoseWidth = 0.05f;

enum Axis { X_AXIS, Y_AXIS, Z_AXIS };

struct Params {
	bool isBottomAligned = false;
	bool isRotationAroundYMiddleSnowman = false;
	bool isRotationAroundYLittleSnowman = false;
	bool isMiddleSnowmanRotatingAroundZ = false;
	bool isAllRotatingAroundOwnYAxes = false;
};

void specialkeys(int key, int x, int y) {
	int mods = glutGetModifiers();

	if (key == GLUT_KEY_LEFT) {
		if (mods & GLUT_ACTIVE_SHIFT) {
			rotZ = rotZ - 2;
		}
		else {
			rotY = rotY - 2;
		}
	}
	else if (key == GLUT_KEY_RIGHT) {
		if (mods & GLUT_ACTIVE_SHIFT) {
			rotZ = rotZ + 2;
		}
		else {
			rotY = rotY + 2;
		}
	}
	else if (key == GLUT_KEY_UP) { rotX = rotX - 2; }
	else if (key == GLUT_KEY_DOWN) { rotX = rotX + 2; }
	else {
		ex = key;
		rotX = 0;
		rotY = 0;
		rotZ = 0;
	}
	rotX = rotX % 360;
	rotY = rotY % 360;
	rotZ = rotZ % 360;
	glutPostRedisplay();
}

void drawAxes() {
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);

	glEnd();
}

void drawSnowManCrash() {
	drawAxes();

	// ���� 
	glPushMatrix();
	glTranslatef(0.0f, smRad, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutWireSphere(smRad, 16, 16);
	glPopMatrix();

	// ������
	glPushMatrix();
	glTranslatef(smRad + smRadH + 0.05f, smRadH, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(smRadH, 32, 32);
	glPopMatrix();

	// �����
	glPushMatrix();
	glTranslatef(smRad + smRadH * 2.0f + smRadE + 0.1f, smRadE, 0.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(smRadE, 32, 32);

	glTranslatef(smRadE * 2.0f + 0.05f, 0.0f, 0.0f);
	glutSolidSphere(smRadE, 32, 32);
	glPopMatrix();

	// ���
	glPushMatrix();
	glTranslatef(smRad + smRadH * 2.0f + smRadE * 4.0f + smRadNoseWidth + 0.2f, 0.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.65f, 0.0f);
	glutSolidCone(smRadNoseWidth, smNoseHeight, 32, 32);
	glPopMatrix();
}

void drawSnowMan() {
	drawAxes();

	// ����
	glPushMatrix();
	glColor3f(0.85f, 0.85f, 0.85f);
	glutSolidSphere(smRad, 32, 32);
	glPopMatrix();

	// ������
	glPushMatrix();
	glTranslatef(0.0f, smRad + smRadH - 0.04, 0.0f); // - 0.025f
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(smRadH, 32, 32);
	glPopMatrix();

	// �����
	glPushMatrix();
	glTranslatef(0.0f, smRad + smRadH, 0.0f); // � ������
	glTranslatef(-smRadE * 1.4f, smRadE * 0.6f, smRadH * 0.9f); // ����� 
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(smRadE, 32, 32);

	glTranslatef(smRadE * 2.8f, 0.0f, 0.0f); // ������
	glutSolidSphere(smRadE, 32, 32);
	glPopMatrix();

	// ���
	glPushMatrix();
	glTranslatef(0.0f, smRad + smRadH, 0.0f); // � ������
	glTranslatef(0.0f, -smRadH * 0.15f, -smRadH * 0.2f); // ����� �� Z
	glColor3f(1.0f, 0.65f, 0.0f);
	glutSolidCone(smRadNoseWidth, smNoseHeight, 32, 32);
	glPopMatrix();
}

void drawThreeSnowmen(Axis placementAxis, float scaleFactor, Params& params) {
	float bodyR1 = smRad;
	float bodyR2 = bodyR1 * scaleFactor;
	float bodyR3 = bodyR2 * scaleFactor;
	float headR1 = 0, headR2 = 0, headR3 = 0;

	float x1 = 0.0f, y1 = 0.0f, z1 = 0.0f,
		x2 = 0.0f, y2 = 0.0f, z2 = 0.0f,
		x3 = 0.0f, y3 = 0.0f, z3 = 0.0f;

	switch (placementAxis) {
	case X_AXIS:
		x2 = bodyR1 + bodyR2;
		x3 = x2 + bodyR2 + bodyR3;

		if (params.isBottomAligned) {
			y2 = -(bodyR1 - bodyR2);
			y3 = y2 - (bodyR2 - bodyR3);
		}
		break;
	case Y_AXIS:
		headR1 = smRadH;
		headR2 = headR1 * scaleFactor;
		headR3 = headR2 * scaleFactor;

		y2 = bodyR1 + bodyR2 + headR1 + headR2;
		y3 = y2 + bodyR2 + bodyR3 + headR2 + headR3;
		break;
	case Z_AXIS:
		z2 = bodyR1 + bodyR2;
		z3 = z2 + bodyR2 + bodyR3;

		if (params.isBottomAligned) {
			y2 = -(bodyR1 - bodyR2);
			y3 = y2 - (bodyR2 - bodyR3);
		}
		break;
	}

	if (params.isRotationAroundYMiddleSnowman) {
		glTranslatef(x2, 0.0f, 0.0f);
		glRotatef(rotY, 0.0f, 1.0f, 0.0f);
		glTranslatef(-x2, 0.0f, 0.0f);
	}
	if (params.isRotationAroundYLittleSnowman) {
		glTranslatef(x3, 0.0f, 0.0f);
		glRotatef(rotY, 0.0f, 1.0f, 0.0f);
		glTranslatef(-x3, 0.0f, 0.0f);
	}

	glPushMatrix();
	glTranslatef(x1, y1, z1);
	if (params.isAllRotatingAroundOwnYAxes) {
		glRotatef(rotY, 0.0f, 1.0f, 0.0f);
	}
	drawSnowMan();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(x2, y2, z2);
	glScalef(scaleFactor, scaleFactor, scaleFactor);
	if (params.isMiddleSnowmanRotatingAroundZ) {
		glTranslatef(0.0f, smRad + smRadH - smRadH * 0.15f, 0.0f);
		glRotatef(rotZ, 0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, -(smRad + smRadH - smRadH * 0.15f), 0.0f);
	}
	if (params.isAllRotatingAroundOwnYAxes) {
		glRotatef(rotY, 0.0f, 1.0f, 0.0f);
	}
	drawSnowMan();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(x3, y3, z3);
	if (params.isAllRotatingAroundOwnYAxes) {
		glRotatef(rotY, 0.0f, 1.0f, 0.0f);
	}
	glScalef(scaleFactor * scaleFactor, scaleFactor * scaleFactor, scaleFactor * scaleFactor);
	drawSnowMan();
	glPopMatrix();
}

void drawThreeSnowmen(float ax, float ay, float az, float scaleFactor) {
	float bodyR1 = smRad;
	float bodyR2 = bodyR1 * scaleFactor;
	float bodyR3 = bodyR2 * scaleFactor;
	float d1 = bodyR1 + bodyR2;
	float d2 = bodyR2 + bodyR3;

	// ����������� ������ ���
	float len = sqrt(ax * ax + ay * ay + az * az);
	float vx = ax / len;
	float vy = ay / len;
	float vz = az / len;

	// ������ ���� ����� ���
	float cx1 = 0.0f, cy1 = 0.0f, cz1 = 0.0f;

	float cx2 = cx1 + vx * d1;
	float cy2 = cy1 + vy * d1;
	float cz2 = cz1 + vz * d1;

	float cx3 = cx2 + vx * d2;
	float cy3 = cy2 + vy * d2;
	float cz3 = cz2 + vz * d2;

	glPushMatrix();

	glRotatef(rotZ, vx, vy, vz);
	glRotatef(rotY, 0.0f, 1.0f, 0.0f);

	glPushMatrix();
	glTranslatef(cx1, cy1, cz1);
	drawSnowMan();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(cx2, cy2, cz2);
	glScalef(scaleFactor, scaleFactor, scaleFactor);
	drawSnowMan();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(cx3, cy3, cz3);
	glScalef(scaleFactor * scaleFactor, scaleFactor * scaleFactor, scaleFactor * scaleFactor);
	drawSnowMan();
	glPopMatrix();

	glPopMatrix();
}

void drawRotatingHeadSnowMan() {
	drawAxes();

	glPushMatrix();
	glColor3f(0.85f, 0.85f, 0.85f);
	glutSolidSphere(smRad, 32, 32);
	glPopMatrix();

	// --- ����������� ������ ������ �������� (��� Z �������) ---
	glPushMatrix();

	// ����� ������ (�� ���������� ������ ��������)
	float headYOffset = smRad + smRadH - 0.04f;

	// ������� ������ ���������� ��� Z
	glTranslatef(0.0f, 0.0f, 0.0f);        // ����� � ��������
	glRotatef(rotZ, 0.0f, 0.0f, 1.0f); // ���������� �������� �� Z
	glTranslatef(0.0f, headYOffset, 0.0f);  // ����������� �����

	// --- ������ ---
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(smRadH, 32, 32);
	glPopMatrix();

	// --- ����� ---
	glPushMatrix();
	glTranslatef(-smRadE * 1.4f, smRadE * 0.6f, smRadH * 0.9f); // ����� 
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(smRadE, 32, 32);

	glTranslatef(smRadE * 2.8f, 0.0f, 0.0f); // ������
	glutSolidSphere(smRadE, 32, 32);
	glPopMatrix();

	// --- ��� ---
	glPushMatrix();
	glTranslatef(0.0f, -smRadH * 0.15f, -smRadH * 0.2f); // ����� �� Z
	glColor3f(1.0f, 0.65f, 0.0f);
	glutSolidCone(smRadNoseWidth, smNoseHeight, 32, 32);
	glPopMatrix();

	glPopMatrix(); // ����� ������	
}

void drawScaledSnowman(float scale) {
	glPushMatrix();
	glScalef(scale, scale, scale);
	drawSnowMan();
	glPopMatrix();
}

void drawScene() {
	float scale1 = 1.0f;
	float scale2 = scale1 * 0.5f;
	float scale3 = scale2 * 0.7f;
	float scale4 = scale3 * 0.7f;

	float r1 = smRad * scale1;
	float r2 = smRad * scale2;
	float r3 = smRad * scale3;
	float r4 = smRad * scale4;

	float gap = 1.4f;
	float d12 = (r1 + r2) * (gap + 0.3f);
	float d23 = (r2 + r3) * gap;
	float d34 = (r3 + r4) * gap;

	glPushMatrix();
	drawScaledSnowman(scale1);
	glPopMatrix();

	// C2 ������ Y C1
	glPushMatrix();
	glRotatef(rotC2, 0, 1, 0);
	glTranslatef(d12, 0, 0);
	drawScaledSnowman(scale2);
	glPopMatrix();

	// C3 ������ Z C2
	glPushMatrix();
	glRotatef(rotC2, 0, 1, 0);    // ����� C3 ��� ������ � C2
	glTranslatef(d12, 0, 0);      // � C2
	glRotatef(rotC3, 0, 0, 1);    // �������� ������ Z C2
	glTranslatef(d23, 0, 0);      // �������� �� C2
	drawScaledSnowman(scale3);
	glPopMatrix();

	// C4 ������ Y C2 �� ������ ������
	float headHeight = (smRad + smRadH) * scale2;

	glPushMatrix();
	glRotatef(rotC2, 0, 1, 0);    // ������ � C2
	glTranslatef(d12, 0, 0);      // � C2
	glTranslatef(0, headHeight, 0); // ����� �� ������� ������
	glRotatef(rotC4, 0, 1, 0);    // �������� ������
	glTranslatef(d34, 0, 0);      // �������� �� ������
	drawScaledSnowman(scale4);
	glTranslatef(0, -headHeight, 0); // ��������� ������� �� Y
	glPopMatrix();
}

void update(int value) {
	rotC2 += 1.0f;
	rotC3 += 0.8f;
	rotC4 += 1.5f;

	rotC2 = fmod(rotC2, 360.0f);
	rotC3 = fmod(rotC3, 360.0f);
	rotC4 = fmod(rotC4, 360.0f);

	glutPostRedisplay();
	glutTimerFunc(10, update, 0);
}

void scene() {
	glPushMatrix();

	switch (ex) {
	case GLUT_KEY_F1: { // 1+
		drawSnowManCrash();
		break;
	}
	case GLUT_KEY_F2: { // 2+
		glRotatef(rotY, 0.0f, 1.0f, 0.0f);
		drawSnowMan();
		break;
	}
	case GLUT_KEY_F3: { // 3+
		glRotatef(rotY, 0.0f, 1.0f, 0.0f);
		Params p;
		drawThreeSnowmen(X_AXIS, 0.7f, p);
		break;
	}
	case GLUT_KEY_F4: { // 4+
		Params p;
		p.isMiddleSnowmanRotatingAroundZ = true;
		drawThreeSnowmen(Z_AXIS, 0.7f, p);
		break;
	}
	case GLUT_KEY_F5: { // 5+
		glTranslatef(0.0f, -0.5f, 0.0f);
		glRotatef(rotY, 0.0f, 1.0f, 0.0f);
		Params p;
		drawThreeSnowmen(Y_AXIS, 0.7f, p);
		break;
	}
	case GLUT_KEY_F6: { // 6+
		drawThreeSnowmen(1.0f, 1.0f, 1.0f, 0.7f);
		break;
	}
	case GLUT_KEY_F7: { // 7+
		Params p;
		p.isBottomAligned = true;
		p.isRotationAroundYMiddleSnowman = true;
		glTranslatef(-0.5f, 0.0f, 0.0f);
		drawThreeSnowmen(X_AXIS, 0.7f, p);
		break;
	}
	case GLUT_KEY_F8: { // 8+
		Params p;
		p.isBottomAligned = true;
		p.isAllRotatingAroundOwnYAxes = true;
		glTranslatef(-0.5f, 0.0f, 0.0f);
		drawThreeSnowmen(X_AXIS, 0.7f, p);
		break;
	}
	case GLUT_KEY_F9: { // 9+
		Params p;
		p.isBottomAligned = true;
		p.isRotationAroundYLittleSnowman = true;
		glTranslatef(-1.1f, 0.0f, 0.0f);
		drawThreeSnowmen(X_AXIS, 0.7f, p);
		break;
	}
	case GLUT_KEY_F10: { // 10+
		drawRotatingHeadSnowMan();
		break;
	}
	case GLUT_KEY_F11: { // 11+
		drawScene();
		break;
	}
	default: { // 1
		drawSnowManCrash();
		break;
	}
	}
	glPopMatrix();
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 800, 800);
	glLoadIdentity();
	scene();

	glViewport(800, 0, 800, 800);
	glRotatef(90, 1, 0, 0);
	scene();
	glutSwapBuffers();
}

void Initialize() {
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.5, 1.5, -1.5, 1.5, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);

	glutTimerFunc(0, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1600, 800);
	glutInitWindowPosition(40, 100);
	glutCreateWindow("SnowMan!");
	glutDisplayFunc(Display);
	glutSpecialFunc(specialkeys);
	glEnable(GL_DEPTH_TEST);
	Initialize();
	glutMainLoop();
	return 0;
}