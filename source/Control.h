void moveHead(Ball *pBall);
void updateHead(Ball* pBall);
float rotateHead(float fromX, float toX);
void checkCollision(Ball* pBall1, Ball* pBall2);
// Fix the boundaries of a point so it doesn't go off the screen
void fixBoundary(Ball* pBall);