//GPPOPT: -std=c++11
#include <algorithm>
#include <iostream>
#include <random>

template<typename _Ty>
_Ty LseW1(_Ty *pX, _Ty *pY, int nCnt) {
	_Ty meanX = 0, meanY = 0;
	for (int i = 0; i < nCnt; ++i) {
		meanX += pX[i];
		meanY += pY[i];
	}
	meanX /= nCnt;
	meanY /= nCnt;
	_Ty sumA = 0, sumB = 0;
	for (int i = 0; i < nCnt; ++i) {
		sumA += pX[i] * (pY[i] - meanY);
		sumB += pX[i] * (pX[i] - meanX);
	}
	return sumA / sumB;
}

template<typename _Ty>
_Ty LseW2(_Ty *pX, _Ty *pY, int nCnt) {
	_Ty meanX = 0;
	for (int i = 0; i < nCnt; ++i) {
		meanX += pX[i];
	}
	meanX /= nCnt;
	_Ty sumA = 0, sumX = 0, sumX2 = 0;
	for (int i = 0; i < nCnt; ++i) {
		sumA += pY[i] * (pX[i] - meanX);
		sumX += pX[i];
		sumX2 += pX[i] * pX[i];
	}
	_Ty sumB = sumX2 - sumX * sumX / nCnt;
	return sumA / sumB;
}

int main(int nArgCnt, char *ppArgs[]) {
	const int nDataCnt = 5;
	const int nIterCnt = 10 * 1000 * 1000;
	std::vector<float> Xf(nDataCnt), Yf(nDataCnt);
	std::vector<double> Xd(nDataCnt), Yd(nDataCnt);
	
	std::random_device rd;
	std::mt19937 rg(rd());
	std::uniform_real_distribution<double> uniRand(-1, 1);

	double dMaxErr = 0;
	double dSumErr1 = 0;
	double dSumErr2 = 0;
	for (int k = 0; k < nIterCnt; ++k) {
		for (int i = 0; i < nDataCnt; ++i) {
			Xd[i] = uniRand(rg);
			Yd[i] = uniRand(rg);
			Xf[i] = Xd[i];
			Yf[i] = Yd[i];
		}
		
		double dLseW1 = LseW1(Xd.data(), Yd.data(), nDataCnt);
		double dLseW2 = LseW2(Xd.data(), Yd.data(), nDataCnt);
		float fLseW1 = LseW1(Xf.data(), Yf.data(), nDataCnt);
		float fLseW2 = LseW2(Xf.data(), Yf.data(), nDataCnt);
		dMaxErr = std::max(dMaxErr, dLseW1 - dLseW2);
		dSumErr1 += std::abs(dLseW1 - fLseW1);
		dSumErr2 += std::abs(dLseW2 - fLseW2);
	}
	std::cout << "dMaxErr=" << dMaxErr << std::endl;
	std::cout << dSumErr1 << " vs " << dSumErr2 << std::endl;
	return 0;
}
