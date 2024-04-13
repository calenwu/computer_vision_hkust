//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include <utility>
#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"
#include "math.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount = 0;
static int rightStartX = 0;
static int rightStartY = 0;
static int rightEndX = 0;
static int rightEndY = 0;

static int lastMovesX[5];
static int lastMovesY[5];
static int counter = 0;

int getAdjustedAngle(int startX, int startY, int endX, int endY) {

	int angle = 0;
	if (startX == endX) {
		if (startY == endX) {
			angle = 0;
		}
		else {
			if (startY < endX) {
				angle = 90;
			}
			else {
				angle = 270;
			}
		}
	}
	if (startX < endX) {
		if (startY < endY) {
			// top right
			float temp = float(endY - startY) / float(endX - startX);
			angle = atan(temp) / 2 / M_PI * 360;
		}
		else {
			float temp = float(startY - endY) / float(endX - startX);
			angle = atan(temp) / 2 / M_PI * 360;
			angle = 360 - atan(temp) / 2 / M_PI * 360;
			// bottom right
		}
	}
	else {
		if (startY == endY) {
			angle = 180;
		}
		else {
			if (startY <= endY) {
				// top left
				float temp = float(endY - startY) / float(startX - endX);
				angle = 180 - atan(temp) / 2 / M_PI * 360;
			}
			else {
				// bottom left
				float temp = float(startY - endY) / float(startX - endX);
				angle = 180 + atan(temp) / 2 / M_PI * 360;
			}
		}
	}
	return angle;
}


ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();
	float alpha = pDoc->getAlpha();
	GLubyte color[4] = { 1, 2, 3, alpha * 255};
	pDoc->GetOriginalPixel(source);
	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 );

	glColor4ubv( color );
	GLubyte a = color[0];
	GLubyte b = color[1];
	GLubyte c = color[2];
	GLubyte d = color[3];
	glFlush();
}

//0 = slider
//1 = right click mouse
//2 = gradient
//3 = brush direction
int ImpBrush::GetAngle(const Point source) {
	int temp = m_pDoc->getBrushDirection();
	if (temp == 0) {
		return m_pDoc->getAngle();
	}
	if (temp == 1) {
		return m_pDoc->getAngle();
	}
	if (temp == 2) {
		int grayValue[3][3];
		grayValue[0][0] = getGrayValue(Point(source.x - 1, source.y - 1));
		grayValue[0][1] = getGrayValue(Point(source.x, source.y - 1));
		grayValue[0][2] = getGrayValue(Point(source.x + 1, source.y - 1));
		grayValue[1][0] = getGrayValue(Point(source.x - 1, source.y));
		grayValue[1][1] = getGrayValue(Point(source.x, source.y));
		grayValue[1][2] = getGrayValue(Point(source.x + 1, source.y));
		grayValue[2][0] = getGrayValue(Point(source.x - 1, source.y + 1));
		grayValue[2][1] = getGrayValue(Point(source.x, source.y + 1));
		grayValue[2][2] = getGrayValue(Point(source.x + 1, source.y + 1));
		int matrixX[3][3] = {
			{1, 0, -1},
			{2, 0, -2},
			{1, 0, -1}
		};
		int matrixY[3][3] = {
			{1, 2, 1},
			{0, 0, 0},
			{-1, -2, -1},
		};
		float gaussian[3][3] = {
			{0.0947416, 0.118318, 0.0947416},
			{0.118318, 0.147761, 0.118318},
			{0.0947416, 0.118318, 0.0947416},
		};
		float grayValueBlurred[3][3];
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				grayValueBlurred[i][j] = gaussian[i][j] * grayValue[i][j];
			}
		}
		float xValue = 0;
		float yValue = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				xValue += grayValueBlurred[i][j] * matrixX[i][j];
			}
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				yValue += grayValueBlurred[i][j] * matrixY[i][j];
			}
		}
		
		return (atan(yValue / xValue) / (2 * M_PI) * 360) + 90;
	}
	if (temp == 3) {
		if (counter == 0) {
			return 0;
		}
		if (counter == 1) {
			return 0;
		}
		if (counter == 2) {
			return getAdjustedAngle(lastMovesX[0], lastMovesY[0], lastMovesX[1], lastMovesY[1]);
		}
		if (counter == 3) {
			return getAdjustedAngle(lastMovesX[0], lastMovesY[0], lastMovesX[2], lastMovesY[2]);
		}
		if (counter == 4) {
			return getAdjustedAngle(lastMovesX[0], lastMovesY[0], lastMovesX[3], lastMovesY[3]);
		}
		return getAdjustedAngle(lastMovesX[counter % 5], lastMovesY[counter % 5], lastMovesX[(counter - 1) % 5], lastMovesY[(counter - 1) % 5]);
	}
}

float ImpBrush::getGrayValue(const Point source) {
	m_pDoc->getBrushDirection();
	if (source.x < 0 || source.y < 0 || source.y >= m_pDoc->m_nHeight || source.x >= m_pDoc->m_nWidth) {
		return 0;
	}
	return m_pDoc->GetOriginalPixel(source)[0] * 0.299 + m_pDoc->GetOriginalPixel(source)[1] * 0.587 + m_pDoc->GetOriginalPixel(source)[2] * 0.114;
}

void ImpBrush::SetLastCoordinateForAngle(const Point source) {
	lastMovesX[counter % 5] = source.x;
	lastMovesY[counter % 5] = source.y;
	counter++;
}

void ImpBrush::RightDown(const Point source, const Point target) {
	rightStartX = target.x;
	rightStartY = target.y;
};

void ImpBrush::RightDrag(const Point source, const Point target) {

};

void ImpBrush::RightUp(const Point source, const Point target) {
	if (m_pDoc->getBrushDirection() == 1) {
		rightEndX = target.x;
		rightEndY = target.y;
		m_pDoc->setAngle(getAdjustedAngle(rightStartX, rightStartY, rightEndX, rightEndY));
	}
};
