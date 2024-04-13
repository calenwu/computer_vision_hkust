//
// LineBrush.cpp
//
// The implementation of Line Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "linebrush.h"
#include "math.h"

extern float frand();
static int size = 1;
static float angle = 0;
static int thickness = 1;

LineBrush::LineBrush( ImpressionistDoc* pDoc, char* name ) :
	ImpBrush(pDoc,name)
{
}

void LineBrush::BrushBegin( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;

	size = pDoc->getSize();
	thickness = pDoc->getThickness();


	glPointSize(1);
	glLineWidth(1);

	BrushMove( source, target );
}

void LineBrush::BrushMove( const Point source, const Point target )
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg=pDoc->m_pUI;
	int length = 10 + size;
	int height = thickness;
	if (height == 1) {
		height = 2;
	}
	if ( pDoc == NULL ) {
		printf( "LineBrush::BrushMove  document is NULL\n" );
		return;
	}
	angle = GetAngle(source);
	int targetX = target.x;
	int targetY = target.y;
	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glTranslatef(targetX, targetY, 0);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		SetColor(source);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(-(length / 2), - (height / 2));
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(-(length / 2), (height / 2));
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f((length / 2), (height / 2));
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f((length / 2), -(height / 2));
	glEnd();
	glDisable(GL_BLEND);
	glPopMatrix();
	SetLastCoordinateForAngle(target);
}

void LineBrush::BrushEnd( const Point source, const Point target )
{
	// do nothing so far
}

