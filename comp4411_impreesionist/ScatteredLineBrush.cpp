//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredlinesbrush.h"
#include <math.h>

extern float frand();
static int size = 1;
static int thickness = 1;
static float angle = 0;

int adjustedLength(int length, int angle) {
	int height = 0;
	if (angle < 0) {
		angle = 360 + angle;
	}
	if (0 <= angle && angle < 90) {
		float rad = angle / 360 * 2 * M_PI;
		height = sin(rad) * length;
		length = cos(rad) * length;
	}
	if (90 <= angle && angle < 180) {
		float tempAngle = 180 - angle;
		height = sin(tempAngle / 360 * 2 * M_PI) * length;
		length = cos(tempAngle / 360 * 2 * M_PI) * length;
		length = -length;
	}
	if (180 <= angle && angle < 270) {
		angle = angle - 180;
		height = sin(angle / 360 * 2 * M_PI) * length;
		length = cos(angle / 360 * 2 * M_PI) * length;
		height = -height;
		length = -length;
	}
	if (270 <= angle && angle < 360) {
		float tempAngle = 360 - angle;
		height = sin(tempAngle / 360 * 2 * M_PI) * length;
		length = cos(tempAngle / 360 * 2 * M_PI) * length;
		height = -height;
	}
	return length;
}

ScatteredLinesBrush::ScatteredLinesBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredLinesBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	size = pDoc->getSize();
	thickness = pDoc->getThickness();

	glPointSize((size / 8) + 1);

	BrushMove(source, target);
}

void ScatteredLinesBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;
	int length = 10 + size;
	int height = thickness;
	if (height == 1) {
		height = 2;
	}
	if (pDoc == NULL) {
		printf("ScatteredLinesBrush::BrushMove  document is NULL\n");
		return;
	}

	int numOfPoints = 6;
	if (((size) / 4) + 1 < 6) {
		numOfPoints = ((size) / 4) + 1;
	}

	if (((size) / 4) + 1 < 3) {
		numOfPoints = 3;
	}

	angle = GetAngle(source);
	for (int i = 0.0f; i < numOfPoints; i++) {
		int randX = rand() % (int)(size * 1.5) - (size * 0.75);
		int randY = rand() % (int)(size * 1.5) - (size * 0.75);
		int targetX = target.x + randX;
		int targetY = target.y + randY;
		int maxWidth = pDoc->m_nWidth - 1;
		int maxHeight = pDoc->m_nHeight - 1;
		if (targetX < 0) {
			targetX = 0;
		}
		if (targetX > maxWidth) {
			targetX = maxWidth;
		}
		if (targetY < 0) {
			targetY = 0;
		}
		if (targetY > maxHeight) {
			targetY = maxHeight;
		}

		glPushMatrix();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glTranslatef(targetX, targetY, 0);
		glRotatef(angle, 0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
			SetColor(source);
			glTexCoord2f(0.0f, 0.0f);
			glVertex2f(-(length / 2), -(height / 2));
			glTexCoord2f(1.0f, 0.0f);
			glVertex2f(-(length / 2), (height / 2));
			glTexCoord2f(1.0f, 1.0f);
			glVertex2f((length / 2), (height / 2));
			glTexCoord2f(0.0f, 1.0f);
			glVertex2f((length / 2), -(height / 2));
		glEnd();
		glDisable(GL_BLEND);
		glPopMatrix();
	}
	SetLastCoordinateForAngle(target);
}

void ScatteredLinesBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}



//for (float i = 0; i < numOfLines; i++) {
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	//glBegin(GL_LINES);
	//SetColor(Point(randX + source.x, randY + source.y));
	//float frac = (i / numOfLines);
	//float newX = (length / 2) - (lengthThickness * frac);
	//float newY = (height / 2) - (heightThickness * frac);
	//glVertex2f(targetX - (length / 2) - (lengthThickness * frac), targetY - (height / 2) - (heightThickness * frac));
	//glVertex2f(targetX + (length / 2) - (lengthThickness * frac), targetY + (height / 2) - (heightThickness * frac));
	//newX = (length / 2) + (lengthThickness * frac);
	//newY = (height / 2) + (heightThickness * frac);
	//glVertex2f(targetX - (length / 2) + (lengthThickness * frac), targetY - (height / 2) + (heightThickness * frac));
	//glVertex2f(targetX + (length / 2) + (lengthThickness * frac), targetY + (height / 2) + (heightThickness * frac));
	//glEnd();
	//glDisable(GL_BLEND);
//}
