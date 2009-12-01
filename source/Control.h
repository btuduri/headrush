void moveSprite(Ball *pBall);

void updateSprite(Ball* pBall);

float rotateSprite(float fromX, float toX, int type, float oldX);

void checkCollision(Ball* pBall1, Ball* pBall2);

// Fix the boundaries of a point so it doesn't go off the screen
void fixBoundary(Ball* pBall);

int scrollCheckX(int type);
int scrollCheckY(int type);