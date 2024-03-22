#ifndef STUPIDARR_H
#define STUPIDARR_H
//i moved the definition of the comparison constants here,
// because i want to use them in the mergeArrays function
#define LESS -1
#define EQUAL 0
#define GREATER 1

#include <cassert>

template<typename T>
class StupidArr{
public:
	//!!the user is responsible for deleting the array, hence its stupidity!!
	explicit StupidArr(int size) : size(size), arr(new T[size]) {}	
	StupidArr(T* arr, int size) : size(size), arr(arr) {}
	StupidArr(const StupidArr& other) = default;
	StupidArr& operator=(const StupidArr& other) = default;
	int size;
	T* arr;

	T& operator[](int index) {
		assert(index >= 0 && index < size);
		return arr[index];
	}
	
};
template<typename T, typename COMP>
StupidArr<T> mergeArrays(StupidArr<T> arr1, StupidArr<T> arr2, COMP comp){
	T* result = new T[arr1.size + arr2.size];
	int index1 = 0, index2 = 0, resultIndex = 0;
	while (index1 < arr1.size && index2 < arr2.size) {

		if(comp(arr1[index1], arr2[index2])==EQUAL) {	//means we need to remove the duplicate
			result[resultIndex] = arr1[index1];
			index1++;
			index2++;
			resultIndex++;
			continue;
		}
		if (comp(arr1[index1], arr2[index2])==LESS) {
			result[resultIndex] = arr1[index1];
			index1++;
			resultIndex++;
			continue;
		}
		if (comp(arr1[index1], arr2[index2])==GREATER) {
			result[resultIndex] = arr2[index2];
			index2++;
			resultIndex++;
			continue;
		}
	}
	while (index1 < arr1.size) {
		result[resultIndex] = arr1[index1];
		index1++;
		resultIndex++;
	}
	while (index2 < arr2.size) {
		result[resultIndex] = arr2[index2];
		index2++;
		resultIndex++;
	}
	return StupidArr<T>(result, resultIndex);
}

#endif // !STUPIDARR_H