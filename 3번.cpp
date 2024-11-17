#include <GL/glut.h>
#include <cmath>
#include <cstdlib> // rand()�� srand()�� ����ϱ� ���� ���
#include <ctime>   // time()�� ����ϱ� ���� ���
#include <algorithm>

float ballRadius = 0.2f; // ���� ������
float ballX = 0.0f; // ���� X ��ġ
float ballY = 0.5f; // ���� Y ��ġ
float ballZ = 0.0f; // ���� Z ��ġ
float ballVelocityX = -0.02f; // ���� X ���� �ӵ�
float ballVelocityY = 0.0f; // ���� Y ���� �ӵ�
float ballVelocityZ = 0.01f; // ���� Z ���� �ӵ�
float gravity = -0.01f; // �߷� ���ӵ�
float elasticity = 0.8f; // ź�� ���
float jumpForce = 0.2f; // �����̽��ٷ� ������ ���� ��
float rotationAngleX = 0.0f; // ���� X�� ȸ�� ����
float rotationAngleY = 0.0f; // ���� Y�� ȸ�� ����
float rotationSpeedFactor = 180.0f; // ȸ�� �ӵ� ���� ���
int bounceCount = 0; // �ٿ Ƚ���� �����ϴ� ����
bool isLastBounce = false; // ������ �ٿ ���θ� �����ϴ� ����

// ���� �ӵ� ���� ����
float speedFactor = 1.0f; // �⺻ �ӵ� ���� ���

// ���� ũ�� ���� ����
float scaleX = 1.0f; // X ���� ũ��
float scaleY = 1.0f; // Y ���� ũ��
float scaleZ = 1.0f; // Z ���� ũ��
float scaleFactor = 0.3f; // ũ�� ��ȭ ����
bool isJumping = false; // ���� ���� ������ ����

void drawBall(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // ���� ũ�� ����
    glScalef(scaleX, scaleY, scaleZ);

    // ���� ȸ�� ����
    glRotatef(rotationAngleX, 1.0f, 0.0f, 0.0f); // X���� �߽����� ȸ��
    glRotatef(rotationAngleY, 0.0f, 1.0f, 0.0f); // Y���� �߽����� ȸ��

    // ���� ������ ���������� ����
    glColor3f(1.0f, 0.0f, 0.0f); // ������ (R, G, B)

    glutSolidSphere(ballRadius, 20, 20); // �ָ��� ��
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // ī�޶� ����
    gluLookAt(0.0, 2.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // �ٴ� ������ ����
    glColor3f(0.5f, 0.5f, 0.5f); // ȸ�� (R, G, B)

    // �ٴ� �׸���
    glBegin(GL_QUADS);
    glVertex3f(-2.0f, -0.1f, -2.0f);
    glVertex3f(2.0f, -0.1f, -2.0f);
    glVertex3f(2.0f, -0.1f, 2.0f);
    glVertex3f(-2.0f, -0.1f, 2.0f);
    glEnd();

    // �� �׸���
    drawBall(ballX, ballY, ballZ);

    glutSwapBuffers();
}

void update(int value) {
    // �߷� ����
    ballVelocityY += gravity;
    ballY += ballVelocityY;
    ballX += ballVelocityX * speedFactor; // X �������� �̵�
    ballZ += ballVelocityZ * speedFactor; // Z �������� �̵�

    // �ٴڿ� ����� �� �ݻ�
    if (ballY <= ballRadius) {
        ballY = ballRadius; // �ٴڿ� ������ ��ġ ����
        ballVelocityY = -ballVelocityY * elasticity; // Y ���� �ӵ� ���� �� ź�� ����

        // �ٿ Ƚ�� ����
        bounceCount++;

        // ������ �ٿ ���� Ȯ��
        if (bounceCount >= 8) { // ���� ���, 8�� �ٿ �� ������ �ٿ
            isLastBounce = true;
        }
        else {
            isLastBounce = false;
        }

        // ũ�� ��ȭ ����
        if (!isLastBounce) {
            scaleX = 1.0f + scaleFactor; // �¿�� �þ
            scaleY = 1.0f - scaleFactor; // ���Ϸ� �پ��
        }
        else {
            // ������ �ٿ�� ��� ũ�⸦ ������� ����
            scaleX = 1.0f; // ���� ũ��� ����
            scaleY = 1.0f; // ���� ũ��� ����
        }
    }
    else {
        // �ٴڿ� ���� ������ ũ�� ����
        scaleX = 1.0f; // ���� ũ��� ����
        scaleY = 1.0f; // ���� ũ��� ����
    }

    // X ���� ��� üũ
    if (ballX >= 2.0f - ballRadius || ballX <= -2.0f + ballRadius) {
        ballVelocityX = -ballVelocityX; // X ���� �ӵ� ����
    }

    // Z ���� ��� üũ
    if (ballZ >= 2.0f - ballRadius || ballZ <= -2.0f + ballRadius) {
        ballVelocityZ = -ballVelocityZ; // Z ���� �ӵ� ����
    }

    // ���� ȸ�� ���� ������Ʈ
    float speed = sqrt(ballVelocityX * ballVelocityX + ballVelocityZ * ballVelocityZ);
    rotationAngleX += speed * rotationSpeedFactor; // X�� ȸ��
    rotationAngleY += speed * rotationSpeedFactor; // Y�� ȸ��

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // �� 60 FPS
}




void resetBall() {
    // ���� �ʱ� ���·� ����
    ballX = 0.0f;
    ballY = 0.5f;
    ballZ = 0.0f;

    // ������ �������� �ӵ� ����
    ballVelocityX = ((rand() % 200) / 1000.0f) - 0.1f; // -0.1f ~ 0.1f ����
    ballVelocityY = 0.0f; // Y ���� �ӵ��� 0���� ����
    ballVelocityZ = ((rand() % 200) / 1000.0f) - 0.1f; // -0.1f ~ 0.1f ����

    rotationAngleX = 0.0f; // ȸ�� ���� �ʱ�ȭ
    rotationAngleY = 0.0f; // ȸ�� ���� �ʱ�ȭ
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // ���̾������� ���
        break;
    case 's':
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // �ָ��� ���
        break;
    case 27: // ESC Ű
        exit(0);
        break;
    case ' ': // �����̽���
        // ���� ���� ���� �־� ����
        if (ballY <= ballRadius) { // �ٴڿ� ��� ���� ���� ���� ����
            ballVelocityY += jumpForce; // ���� �� �߰�
            isJumping = true; // ���� ���� ����
            bounceCount = 0; //�ٿ Ƚ�� �ʱ�ȭ
        }
        break;
    case 'r': // r Ű
        resetBall(); // ���� �ʱ� ���·� ����
        bounceCount = 0; //�ٿ Ƚ�� �ʱ�ȭ
        break;
    case 'q':
        exit(0);
        break;
    }
}

void specialKeyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP: // ���� ȭ��ǥ
        speedFactor += 0.1f; // �ӵ� ����
        break;
    case GLUT_KEY_DOWN: // �Ʒ��� ȭ��ǥ
        speedFactor = std::max(0.1f, speedFactor - 0.1f); // �ӵ� ���� (�ּ� 0.1f)
        break;
    }
}

void init() {
    glEnable(GL_DEPTH_TEST); // ���� �׽�Ʈ Ȱ��ȭ
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0); // ���� ���� ����
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0))); // ���� �õ� �ʱ�ȭ

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rolling Ball Simulation");

    init(); // �ʱ�ȭ �Լ� ȣ��
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard); // Ư�� Ű �Է� ó��
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
