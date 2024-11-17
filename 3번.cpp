#include <GL/glut.h>
#include <cmath>
#include <cstdlib> // rand()와 srand()를 사용하기 위한 헤더
#include <ctime>   // time()을 사용하기 위한 헤더
#include <algorithm>

float ballRadius = 0.2f; // 공의 반지름
float ballX = 0.0f; // 공의 X 위치
float ballY = 0.5f; // 공의 Y 위치
float ballZ = 0.0f; // 공의 Z 위치
float ballVelocityX = -0.02f; // 공의 X 방향 속도
float ballVelocityY = 0.0f; // 공의 Y 방향 속도
float ballVelocityZ = 0.01f; // 공의 Z 방향 속도
float gravity = -0.01f; // 중력 가속도
float elasticity = 0.8f; // 탄성 계수
float jumpForce = 0.2f; // 스페이스바로 점프할 때의 힘
float rotationAngleX = 0.0f; // 공의 X축 회전 각도
float rotationAngleY = 0.0f; // 공의 Y축 회전 각도
float rotationSpeedFactor = 180.0f; // 회전 속도 조절 계수
int bounceCount = 0; // 바운스 횟수를 추적하는 변수
bool isLastBounce = false; // 마지막 바운스 여부를 추적하는 변수

// 공의 속도 조절 변수
float speedFactor = 1.0f; // 기본 속도 조절 계수

// 공의 크기 조절 변수
float scaleX = 1.0f; // X 방향 크기
float scaleY = 1.0f; // Y 방향 크기
float scaleZ = 1.0f; // Z 방향 크기
float scaleFactor = 0.3f; // 크기 변화 비율
bool isJumping = false; // 공이 점프 중인지 여부

void drawBall(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    // 공의 크기 조절
    glScalef(scaleX, scaleY, scaleZ);

    // 공의 회전 적용
    glRotatef(rotationAngleX, 1.0f, 0.0f, 0.0f); // X축을 중심으로 회전
    glRotatef(rotationAngleY, 0.0f, 1.0f, 0.0f); // Y축을 중심으로 회전

    // 공의 색깔을 빨간색으로 설정
    glColor3f(1.0f, 0.0f, 0.0f); // 빨간색 (R, G, B)

    glutSolidSphere(ballRadius, 20, 20); // 솔리드 구
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // 카메라 설정
    gluLookAt(0.0, 2.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // 바닥 색깔을 설정
    glColor3f(0.5f, 0.5f, 0.5f); // 회색 (R, G, B)

    // 바닥 그리기
    glBegin(GL_QUADS);
    glVertex3f(-2.0f, -0.1f, -2.0f);
    glVertex3f(2.0f, -0.1f, -2.0f);
    glVertex3f(2.0f, -0.1f, 2.0f);
    glVertex3f(-2.0f, -0.1f, 2.0f);
    glEnd();

    // 공 그리기
    drawBall(ballX, ballY, ballZ);

    glutSwapBuffers();
}

void update(int value) {
    // 중력 적용
    ballVelocityY += gravity;
    ballY += ballVelocityY;
    ballX += ballVelocityX * speedFactor; // X 방향으로 이동
    ballZ += ballVelocityZ * speedFactor; // Z 방향으로 이동

    // 바닥에 닿았을 때 반사
    if (ballY <= ballRadius) {
        ballY = ballRadius; // 바닥에 닿으면 위치 조정
        ballVelocityY = -ballVelocityY * elasticity; // Y 방향 속도 반전 및 탄성 적용

        // 바운스 횟수 증가
        bounceCount++;

        // 마지막 바운스 여부 확인
        if (bounceCount >= 8) { // 예를 들어, 8번 바운스 후 마지막 바운스
            isLastBounce = true;
        }
        else {
            isLastBounce = false;
        }

        // 크기 변화 적용
        if (!isLastBounce) {
            scaleX = 1.0f + scaleFactor; // 좌우로 늘어남
            scaleY = 1.0f - scaleFactor; // 상하로 줄어듦
        }
        else {
            // 마지막 바운스일 경우 크기를 원래대로 복원
            scaleX = 1.0f; // 원래 크기로 복원
            scaleY = 1.0f; // 원래 크기로 복원
        }
    }
    else {
        // 바닥에 닿지 않으면 크기 유지
        scaleX = 1.0f; // 원래 크기로 복원
        scaleY = 1.0f; // 원래 크기로 복원
    }

    // X 방향 경계 체크
    if (ballX >= 2.0f - ballRadius || ballX <= -2.0f + ballRadius) {
        ballVelocityX = -ballVelocityX; // X 방향 속도 반전
    }

    // Z 방향 경계 체크
    if (ballZ >= 2.0f - ballRadius || ballZ <= -2.0f + ballRadius) {
        ballVelocityZ = -ballVelocityZ; // Z 방향 속도 반전
    }

    // 공의 회전 각도 업데이트
    float speed = sqrt(ballVelocityX * ballVelocityX + ballVelocityZ * ballVelocityZ);
    rotationAngleX += speed * rotationSpeedFactor; // X축 회전
    rotationAngleY += speed * rotationSpeedFactor; // Y축 회전

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 약 60 FPS
}




void resetBall() {
    // 공의 초기 상태로 리셋
    ballX = 0.0f;
    ballY = 0.5f;
    ballZ = 0.0f;

    // 랜덤한 방향으로 속도 설정
    ballVelocityX = ((rand() % 200) / 1000.0f) - 0.1f; // -0.1f ~ 0.1f 범위
    ballVelocityY = 0.0f; // Y 방향 속도는 0으로 유지
    ballVelocityZ = ((rand() % 200) / 1000.0f) - 0.1f; // -0.1f ~ 0.1f 범위

    rotationAngleX = 0.0f; // 회전 각도 초기화
    rotationAngleY = 0.0f; // 회전 각도 초기화
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 와이어프레임 모드
        break;
    case 's':
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 솔리드 모드
        break;
    case 27: // ESC 키
        exit(0);
        break;
    case ' ': // 스페이스바
        // 공에 위로 힘을 주어 점프
        if (ballY <= ballRadius) { // 바닥에 닿아 있을 때만 점프 가능
            ballVelocityY += jumpForce; // 점프 힘 추가
            isJumping = true; // 점프 상태 설정
            bounceCount = 0; //바운스 횟수 초기화
        }
        break;
    case 'r': // r 키
        resetBall(); // 공을 초기 상태로 리셋
        bounceCount = 0; //바운스 횟수 초기화
        break;
    case 'q':
        exit(0);
        break;
    }
}

void specialKeyboard(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP: // 위쪽 화살표
        speedFactor += 0.1f; // 속도 증가
        break;
    case GLUT_KEY_DOWN: // 아래쪽 화살표
        speedFactor = std::max(0.1f, speedFactor - 0.1f); // 속도 감소 (최소 0.1f)
        break;
    }
}

void init() {
    glEnable(GL_DEPTH_TEST); // 깊이 테스트 활성화
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0); // 원근 투영 설정
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned int>(time(0))); // 랜덤 시드 초기화

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Rolling Ball Simulation");

    init(); // 초기화 함수 호출
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard); // 특수 키 입력 처리
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
