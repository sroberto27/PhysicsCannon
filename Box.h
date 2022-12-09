class Box
{
private:
	float boxX;
	float boxY;
	float rotAngle;	
	int lastFrameTime;
	
public:
	Box();
	void MoveBox();
	void RotateBox();
	void TranslateBox(int key);
	void DrawBox();
	
};