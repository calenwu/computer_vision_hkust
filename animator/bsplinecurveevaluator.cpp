#include "bsplinecurveevaluator.h"
#include "beziercurveevaluator.h"
#include "vec.h"
#include "mat.h"

void BSplineCurveEvaluator::evaluateCurve(
		const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const {
	int iCtrlPtCount = ptvCtrlPts.size();

	std::vector<Point> curve;
	ptvEvaluatedCurvePts.clear();

	if (bWrap == true)  {
		curve.push_back(Point(ptvCtrlPts[iCtrlPtCount - 2].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 2].y));
		curve.push_back(Point(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
		curve.insert(curve.end(), ptvCtrlPts.begin(), ptvCtrlPts.end());
		curve.push_back(Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y));
		curve.push_back(Point(ptvCtrlPts[1].x + fAniLength, ptvCtrlPts[1].y));
	} else {
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts.back().y));
		curve.push_back(ptvCtrlPts[0]);
		curve.push_back(ptvCtrlPts[0]);
		curve.insert(curve.end(), ptvCtrlPts.begin(), ptvCtrlPts.end());
		curve.push_back(ptvCtrlPts[iCtrlPtCount - 1]);
		curve.push_back(ptvCtrlPts[iCtrlPtCount - 1]);
	}
	BezierCurveEvaluator bezier;
	
	for (int i = 0; i + 3 < curve.size(); i++) {
		Vec4d Bx (
			curve[i].x + 4 * curve[i + 1].x + curve[i + 2].x,
			4 * curve[i + 1].x + 2 * curve[i + 2].x,
			2 * curve[i + 1].x + 4 * curve[i + 2].x,
			curve[i + 1].x + 4 * curve[i + 2].x + curve[i + 3].x
		);
		Bx = (1.0 / 6.0) * Bx;
		Vec4d By (
			curve[i].y + 4 * curve[i + 1].y + curve[i + 2].y,
			4 * curve[i + 1].y + 2 * curve[i + 2].y,
			2 * curve[i + 1].y + 4 * curve[i + 2].y,
			curve[i + 1].y + 4 * curve[i + 2].y + curve[i + 3].y
		);
		By = (1.0 / 6.0) * By;

		std::vector<Point> pts;
		for (int j = 0; j < 4; j++) {
			pts.push_back(Point(Bx[j], By[j]));
		}
		std::vector<Point> ptv;
		bezier.evaluateCurve(pts, ptv, fAniLength, false);
		ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.end(), ptv.begin(), ptv.end());
	}
	
}