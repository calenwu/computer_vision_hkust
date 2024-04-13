#include "calmullromevaluator.h"
#include "vec.h"
#include "mat.h"

void CatmullRomCurveEvaluator::evaluateCurve(
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
		curve.push_back(ptvCtrlPts[0]);
		curve.push_back(ptvCtrlPts[0]);
		curve.insert(curve.end(), ptvCtrlPts.begin(), ptvCtrlPts.end());
		curve.push_back(ptvCtrlPts[iCtrlPtCount - 1]);
		curve.push_back(ptvCtrlPts[iCtrlPtCount - 1]);
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
	}

	for (int i = 0; i + 3 < curve.size(); i++) {
		for (int j = 0; j < 128; j++) {
			double t = j / 128.0;
			Vec4d T(t * t * t, t * t, t, 1);
			Vec4d CRBx(
				-1 * curve[i].x + 3 * curve[i + 1].x + -3 * curve[i + 2].x + curve[i + 3].x,
				2 * curve[i].x + -5 * curve[i + 1].x + 4 * curve[i + 2].x + -1 * curve[i + 3].x,
				-1 * curve[i].x + curve[i + 2].x,
				2 * curve[i + 1].x
			);
			Vec4d CRBy(
				-1 * curve[i].y + 3 * curve[i + 1].y + -3 * curve[i + 2].y + curve[i + 3].y,
				2 * curve[i].y + -5 * curve[i + 1].y + 4 * curve[i + 2].y + -1 * curve[i + 3].y,
				-1 * curve[i].y + curve[i + 2].y,
				2 * curve[i + 1].y
			);
			Point Q(T * (0.5 * CRBx), T * (0.5 * CRBy));

			if (ptvEvaluatedCurvePts.empty() || Q.x > ptvEvaluatedCurvePts.back().x)
				ptvEvaluatedCurvePts.push_back(Q);
			ptvEvaluatedCurvePts.push_back(Q);
		}
	}
}