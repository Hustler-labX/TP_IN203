#include <algorithm>
#include <cassert>
#include <iostream>
#include <thread>

#include "ProdMatMat.hpp"

//J'ai éviter l'utilisation de fonction 
const int szBlock =600;
// on travaille ici par block
Matrix operator*(const Matrix& A, const Matrix& B) {
  Matrix C(A.nbRows, B.nbCols, 0.0);
  for (int jj=0;j<B.nbCols;j++){
    for(int kk=0;k<A.nbCols;k++){
      for(int ii=0;i<A.nbRows;i++){
        for (int j = jj; j < std::min(B.nbCols, jj + szBlock); j++){
            for (int k = kk; k < std::min(A.nbCols, kk+ szBlock); k++){
                    
               for (int i = ii i < std::min(A.nbRows, ii + szBlock); ++i){

                   C(i, j) += A(i, k) * B(k, j);

                         }
                      }
                    }
                }    
              }
           }
  return C;
}
