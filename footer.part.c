int validityCheck() {
    int flags = 0;
    /* Test Arguments
    -------------------
    27 random signed integers from the range [-100<x<100] will be used;
    this sweet combination was found from rigorous testing.
    */
    for(int i=1; i<=27; i++) {
        srand(time(NULL));
        int n = pow(-1, rand()%3)*(rand()%100);
        //first part gives random sign and second part gives random number.
        float x = generalSoulution(n);
        double LHS = leftExpression(x);
        double RHS = rightExpression(x);
        //floating point numbers should never be compared with "==" operator.
        if(fabs(LHS-RHS)>=0.000030) flags++;
    }

    if(flags>1) return 1;
    else return 0;
    /* Return Values
    -----------------
    validityCheck() returning 0 means no problems were found;
    whereas, returning 1 means some problems were encountered.
    */
}

int main() {
    float errorLevel = 0.0;
    //range of errorLevel shall be 0.0 to 1.0.
    for(int i=0; i<10; i++) {
        int errorState = validityCheck();
        printf("%d. Error State = %d\n", i, errorState);
        errorLevel = (errorLevel + errorState)/2.0;
        printf("   Error Level = %f\n", errorLevel);
    }
    /* Return Values 
    -----------------
    returning 0 means there is no problem with the solution;
    whereas, returning 1 means the solution was not right.
    */
    if(errorLevel<0.95) return 0; 
    else return 1;
}