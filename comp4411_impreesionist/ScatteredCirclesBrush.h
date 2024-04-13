#pragma once
#pragma once
#pragma once
//
// ScatteredCirclesBrush.h
//
// The header file for Line Brush. 
//

#ifndef SCATTEREDCIRCLESBRUSH_H
#define SCATTEREDCIRCLESBRUSH_H

#include "ImpBrush.h"

class ScatteredCirclesBrush : public ImpBrush
{
public:
	ScatteredCirclesBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif