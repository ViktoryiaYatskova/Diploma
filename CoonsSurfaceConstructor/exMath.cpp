#include "exMath.h"

ExMath::ExMath() {}

//==============================================================================================================
//                              вычисление определителя
//==============================================================================================================
//функция вычисления определителя матрицы
int ExMath::determinant(double **matr, int n) {
    int temp = 0;   //временная переменная для хранения определителя
    int k = 1;      //степень
    if(n < 1) {
        cout<<"Не верный размер матрицы!!!" << endl;
        return 0;
    } else if (n == 1) {
        temp = matr[0][0];
    } else if (n == 2) {
        temp = matr[0][0] * matr[1][1] - matr[1][0] * matr[0][1];
    } else {
        for(int i = 0; i < n; i++){
            int m = n - 1;

            double **temp_matr = new double * [m];
            for(int j = 0; j < m; j++) {
                temp_matr[j] = new double [m];
            }

            getMatrix(matr, n, temp_matr, 0, i);
            temp = temp + k * matr[0][i] * determinant(temp_matr, m);
            k = -k;
            freeMatrixMemory(temp_matr, m);
        }
    }
    return temp;
}

void ExMath::freeMatrixMemory(double **matr, int n) {
    for(int i = 0; i < n; i++)
        delete [] matr[i];
    delete [] matr;
}

//функция вычеркивания строки и столбца
void ExMath::getMatrix(double **matr, int n, double **temp_matr, int indRow, int indCol) {
    int ki = 0;
    for (int i = 0; i < n; i++ ){
        if(i != indRow) {
            for (int j = 0, kj = 0; j < n; j++) {
                if (j != indCol){
                    temp_matr[ki][kj] = matr[i][j];
                    kj++;
                }
            }
            ki++;
        }
    }
}

double** initMatrix(int dim) {
    double **matrix = new double*[dim];

    for(int q = 0; q < dim; q++) {
        matrix[q] = new double[dim];

        for(int m = 0; m < dim; m++) {
            matrix[q][m] = 0;
        }
    }

    return matrix;
}

double* ExMath::solveMatrixEquantion(double** a, double* b, int dim) {
    int n = dim,i,k,j,p;
    double **l,**u,sum,*z,*x;

    l = new double*[dim];
    u = new double*[dim];

    for(int q = 0; q < dim; q++) {
        l[q] = new double[dim];
        u[q] = new double[dim];

        for(int m = 0; m < dim; m++) {
            l[q][m] = 0;
            u[q][m] = 0;
        }
    }

    z = new double[dim];
    x = new double[dim];
    for(int m = 0; m < dim; m++) {
        z[m] = 0;
        x[m] = 0;
    }

    //********** LU decomposition *****//
        for(k=1;k<=n;k++)
        {
            u[k][k]=1;
            for(i=k;i<=n;i++)
            {
                sum=0;
                for(p=1;p<=k-1;p++)
                    sum+=l[i][p]*u[p][k];
                l[i][k]=a[i][k]-sum;
            }

            for(j=k+1;j<=n;j++)
            {
                sum=0;
                for(p=1;p<=k-1;p++)
                    sum+=l[k][p]*u[p][j];
                u[k][j]=(a[k][j]-sum)/l[k][k];
            }
        }
        //******** Displaying LU matrix**********//
        /*cout<<endl<<endl<<"LU matrix is "<<endl;
        for(i=1;i<=n;i++)
        {
            for(j=1;j<=n;j++)
                cout<<l[i][j]<<"  ";
            cout<<endl;
        }
        cout<<endl;
        for(i=1;i<=n;i++)
        {
            for(j=1;j<=n;j++)
                cout<<u[i][j]<<"  ";
            cout<<endl;
        }*/

        //***** FINDING Z; LZ=b*********//

        for(i=1;i<=n;i++) {//forward subtitution method
            sum=0;
            for(p=1;p<i;p++)
            sum+=l[i][p]*z[p];
            z[i]=(b[i]-sum)/l[i][i];
        }
        //********** FINDING X; UX=Z***********//
        for(i=n;i>0;i--) {
            sum=0;
            for(p=n;p>i;p--)
                sum+=u[i][p]*x[p];
            x[i]=(z[i]-sum)/u[i][i];
        }
        //*********** DISPLAYING SOLUTION**************//
        /*cout<<endl<<"Set of solution is"<<endl;
        for(i=1;i<=n;i++)
            cout<<endl<<x[i];*/
        return z;
}
