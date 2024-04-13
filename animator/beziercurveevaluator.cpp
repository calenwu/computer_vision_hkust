#include "beziercurveevaluator.h"
#include "vec.h"
#include "mat.h"

void BezierCurveEvaluator::evaluateCurve(
		const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const {

	int iCtrlPtCount = ptvCtrlPts.size();
	std::vector<Point> curve;
	ptvEvaluatedCurvePts.clear();
	curve.assign(ptvCtrlPts.begin(), ptvCtrlPts.end()); 
	if (bWrap) {
		curve.push_back(Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y));
	} else {
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
	}

	bool wrapped = false;
	int i = 0;
	for(; i + 3 < curve.size(); i += 3) {
		ptvEvaluatedCurvePts.push_back(curve[i]);
		ptvEvaluatedCurvePts.push_back(curve[i + 3]);
		Vec4d px(curve[i].x, curve[i + 1].x, curve[i + 2].x, curve[i + 3].x);
		Vec4d py(curve[i].y, curve[i + 1].y, curve[i + 2].y, curve[i + 3].y);
		for (int j = 0; j < 128; j++) {
			double t = j / 128.0;
			Vec4d T(t * t * t, t * t, t, 1);
			Vec4d Mx(
				-1 * curve[i].x + 3 * curve[i + 1].x + -3 * curve[i + 2].x + 1 * curve[i + 3].x,
				3 * curve[i].x + -6 * curve[i + 1].x + 3 * curve[i + 2].x,
				-3 * curve[i].x + 3 * curve[i + 1].x,
				1 * curve[i].x
			);
			Vec4d My(
				-1 * curve[i].y + 3 * curve[i + 1].y + -3 * curve[i + 2].y + 1 * curve[i + 3].y,
				3 * curve[i].y + -6 * curve[i + 1].y + 3 * curve[i + 2].y,
				-3 * curve[i].y + 3 * curve[i + 1].y,
				1 * curve[i].y
			);
			Point Q(T * Mx, T * My);

			if (bWrap && Q.x > fAniLength) {
				if (!wrapped) {
					ptvEvaluatedCurvePts.push_back(Point(ptvEvaluatedCurvePts.back().x - fAniLength, ptvEvaluatedCurvePts.back().y));
					wrapped = true;
				}
				Q.x = fmod(Q.x, fAniLength);
			}
			ptvEvaluatedCurvePts.push_back(Q);
		}
	}

	for (;i < ptvCtrlPts.size(); i++) {
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
	}
	if (bWrap && !wrapped) {
		float y = 0;
		if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
			y = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) + 
				  ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
				 (ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
		} else {
			y = ptvCtrlPts[0].y;
		}
		ptvEvaluatedCurvePts.push_back(Point(0, y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, y));
	}
	
}