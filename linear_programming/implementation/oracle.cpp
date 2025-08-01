#include <oracle.h>;

double only_positive(double vectorVal) {
    return std::max(0.0, vectorVal);
};

DistanceResponse* simpleOracle(MatrixXd* A, Eigen::VectorXd* b, Eigen::VectorXd* x, double r, double hoffmanConst) {
    DistanceResponse* response;
    //in practice the constant can be set to infinity or doubled in size each time.
    Eigen::VectorXd error = ( ((*A) * (*x)) - (*b)).unaryExpr(&only_positive); //easy to make parallel
    if (error.isZero(1e-10)){ // TODO: determine float precision
        response = new DistanceResponse{ WithinX::ABSOLUTELY, NULL, -1.0 };
        return response;
    }
    double epsilon = 0.0;
    Eigen::VectorXd* direction;
    direction = new Eigen::VectorXd(A->row(1) / (A->row(1)).norm()); // row i of A scaled
    for(int i = 2; i < A->rows(); i++) { //possible to make parallel
        double length = (A->row(i)).norm();
        if (error[i] / length > epsilon) {
            epsilon = error[i] / length; //element i of error vector scaled
            if (direction != NULL) delete direction;
            direction = new Eigen::VectorXd( A->row(i) / length ); // row i of A scaled
        }
    }
    if (r < 1 * epsilon) {
        response = new DistanceResponse{ WithinX::NO, NULL, std::numeric_limits<double>::infinity() };
        return response;
    }
    if (r > hoffmanConst * epsilon) { // TODO: use hoffmanConst to compute oraclePrecision "D". 
        response = new DistanceResponse{ WithinX::YES, NULL, hoffmanConst * epsilon };
        return response;
    }
    response = new DistanceResponse{ WithinX::MAYBE, direction, epsilon };
    return response;
}

