#include <oracle.h>;

double only_positive( double vectorVal){
    return max(0.0, vectorVal);
}

*DistanceResponse simpleOracle(MatrixXd* A, VectorXd* b, VectorXd* x, double r, double hoffmanConst) {
    double c = 1;
    //double hoffmanConst = Weird_HOFFMAN_CONSTANT(A) // the constant which makes it a C-D-Orcale
    //in practice the constant can be set to infinity or doubled in size each time.
    VectorXd* error = ((A*x)-b).unaryExpr(&only_positive); //easy to make parallel
    if error == 0.0 {
        return (WithinX.ABSOLUTELY,_,_)
    }
    double epsilon = 0.0
    for i in 1 to m: //possible to make parallel
        length = 2-norm(A_i)
        if error_i/length > epsilon :
            epsilon = error_i / length //element i of error vector scaled
            direction = A_i / length // row i of A scaled
    if r < C*epsilon :
        return (WithinX.NO,_,_ )
    if r > D* epsilon :
        return (WithinX.YES,_, D* epsilon )
    return (WithinX.MAYBE, direction, epsilon)
}

