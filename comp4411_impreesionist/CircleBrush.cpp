//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circlebrush.h"
#include <math.h>

extern float frand();
static int size = 1;

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	size = pDoc->getSize();

	glPointSize((float)size * 10);
	glLineWidth((float)size * 10);

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove  document is NULL\n");
		return;
	}
	int r = (size + 1) / 2;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBegin(GL_TRIANGLE_FAN);
		SetColor(source);
		glVertex2f(target.x, target.y); // Center
		for (int i = 0.0f; i <= 360; i++) {
			glVertex2f(
				r * cos(M_PI * i / 180.0) + target.x,
				r * sin(M_PI * i / 180.0) + target.y
			);
		}
	glEnd();
	glDisable(GL_BLEND);
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}

