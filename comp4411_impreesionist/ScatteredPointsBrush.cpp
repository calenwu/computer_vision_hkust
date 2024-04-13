//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "scatteredpointsbrush.h"
#include <math.h>

extern float frand();
static int size = 1;

ScatteredPointsBrush::ScatteredPointsBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void ScatteredPointsBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	size = pDoc->getSize();

	glPointSize((size / 8) + 1);

	BrushMove(source, target);
}

void ScatteredPointsBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredPointsBrush::BrushMove  document is NULL\n");
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
		glBegin(GL_POINTS);
			SetColor(Point(source.x + randX, source.y + randY));
			glVertex2d(targetX, targetY);
		glEnd();
	}
	glDisable(GL_BLEND);
}

void ScatteredPointsBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

