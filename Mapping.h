//NURAFRINA BATRISYIA BINTI NORDZAMAN 123130337 TT3L

#ifndef MAPPING_H
#define MAPPING_H

#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <stdlib.h>
#include <time.h> 

using namespace std;

template <typename T1, typename T2> 
class mapper {
	private: 
		T1* arrayAptr;
		T2* arrayBptr;
		int Arraysize;
	public:
		mapper(){};
		mapper(T1* arrayA,T2* arrayB, int size){
			arrayAptr = arrayA;
			arrayBptr = arrayB;
			Arraysize = size;
		}
		T2 get(T1 x){
			for (int i = 0; i < Arraysize; i++){
				if (arrayAptr[i] == x){
					return arrayBptr[i];
				}
			}
			throw std::logic_error(string("Array B Key not found")) ;
		}
		T1 get(T2 x){
			for (int i = 0; i < Arraysize; i++){
				if (arrayBptr[i] == x){
					return arrayAptr[i];
				}
			}
			throw std::logic_error(string("Array A Key not found")) ;
		}
};

#endif // MAPPER_H
