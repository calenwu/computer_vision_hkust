//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredcirclesbrush.h"
#include <math.h>

extern float frand();
static int size = 1;

ScatteredCirclesBrush::ScatteredCirclesBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredCirclesBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	size = pDoc->getSize();

	glPointSize((size / 8) + 1);

	BrushMove(source, target);
}

void ScatteredCirclesBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredCirclesBrush::BrushMove  document is NULL\n");
		return;
	}

	int numOfPoints = 6;
	if (((size) / 4) + 1 < 6) {
		numOfPoints = ((size) / 4) + 1;
	}

	if (((size) / 4) + 1 < 3) {
		numOfPoints = 3;
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
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
		int r = (size / 4) + 1;
		glBegin(GL_TRIANGLE_FAN);
			SetColor(Point(source.x + randX, source.y + randY));
			glVertex2f(targetX, targetY); // Center
			for (int i = 0.0f; i <= 360; i++) {
				glVertex2f(
					r * cos(M_PI * i / 180.0) + targetX,
					r * sin(M_PI * i / 180.0) + targetY
				);
			}
		glEnd();
	}
	glDisable(GL_BLEND);
}

void ScatteredCirclesBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

