
#include <iostream>
#include "oracle.h"

// polytope (composes half-spaces), hyperball
// we choose a half-space based on the greatest error
// iff hoffman = inf (no, maybe, abs) then 3 cases, iff hoffman != inf then 4 cases (Yes, no, maybe, absolutely)
// 

// if we divide A/b, the oracle should give us a direction of 1, and the minimum
void trivialRandomizedVals1dCase() {
	Eigen::MatrixXd A(10,1);
	A << 4017.8973, 4132.6299, 3596.1312, -1044.8135, -4136.2482, -3394.1659, -4027.5872, -1707.4608, -1534.5558, 701.5546;
	std::cout << "Values in A:\n" << A << std::endl;
	
	Eigen::VectorXd b(10);
	b << -1959.983434, 4571.2734, 4690.65132, 4374.614089, 968.265026, -1461.149328, 2526.940175, 4188.608556, -2117.31621, -4150.438897;
	Eigen::VectorXd x(1);
	x << 9132.6299;
	double r = 10.0;
	double hoffman = 9;

	DistanceResponse* response = simpleOracle(&A, &b, &x, r, hoffman);
	std::cout << "Values in A:\n" << A << std::endl;
	std::cout << "WithinX: " << static_cast<int>(response->wx);
	std::cout << ", distance bound: " << response->distanceBound << std::endl;
	
}


// if we divide A/b, the oracle should give us a direction of 1, and the minimum
void trivialNonRandomVals2dCase() {
	Eigen::MatrixXd A(4, 2);
	/*A << -1.0, 0.0,
		  1.0, 0.0,
		  0.0, -1.0,
		0.0, 1.0;*/
	A << -1.0, 0.0,
		0.0, -1.0,
		3.0, 2.5,
		-5.0, -7.1;
	std::cout << "Values in A:\n" << A << std::endl;

	Eigen::VectorXd b(4);
	b << 0, 0, 23, -4;
	Eigen::VectorXd x(2);
	x << 2, 8;
	double r = 10.0;
	double hoffman = 9;

	DistanceResponse* response = simpleOracle(&A, &b, &x, r, hoffman);
	std::cout << "Values in A:\n" << A << std::endl;
	std::cout << "WithinX: " << static_cast<int>(response->wx);
	std::cout << ", distance bound: " << response->distanceBound << std::endl;

}

int main() {
	std::cout << "================ TRIVIAL_RANDOMZED_VALS_1D ===================\n";
	trivialRandomizedVals1dCase();

	std::cout << "================ TRIVIAL_NONRANDOM_VALS_2D ===================\n";
	trivialNonRandomVals2dCase();
	return 0;
}

