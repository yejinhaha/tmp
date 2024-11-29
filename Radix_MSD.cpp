#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

// 배열에서 최대값 찾는 함수
// 최대값 찾아야 가장 큰 자릿수를 계산 가능
int getMax(const vector<int>& arr)
{
    int mx = arr[0];
    for (int num : arr) // 배열 전체 순회하면서 최대값 찾음
        if (num > mx)
            mx = num;
    return mx;
}

// MSD Radix Sort를 위한 재귀 함수
// 큰 자릿수부터 정렬하고 나머지는 재귀적으로 처리
void msdRadixSortHelper(vector<int>& arr, int start, int end, int exp)
{
    if (start >= end || exp == 0) // 더 이상 나눌 게 없으면 종료
        return;

    vector<int> count(10, 0); // 0~9까지의 자릿수 카운팅
    vector<int> output(end - start); // 정렬된 결과를 임시로 저장할 공간

    // 현재 자릿수의 값 빈도 계산
    for (int i = start; i < end; i++) {
        int digit = (arr[i] / exp) % 10; // 자릿수 값 추출
        count[digit]++;
    }

    // 누적 빈도를 계산 (정렬 위치 결정)
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // 자릿수 기준으로 정렬
    for (int i = end - 1; i >= start; i--) {
        int digit = (arr[i] / exp) % 10;
        output[--count[digit]] = arr[i]; // 정렬된 위치에 삽입
    }

    // 정렬 결과를 원래 배열에 복사
    for (int i = 0; i < output.size(); i++)
        arr[start + i] = output[i];

    // 각 자릿수(버킷)별로 재귀적으로 정렬
    int prev = start;
    for (int i = 0; i < 10; i++) {
        if (count[i] > prev - start) { // 해당 버킷에 데이터가 있다면
            msdRadixSortHelper(arr, prev, start + count[i], exp / 10); // 다음 자릿수로 이동
            prev = start + count[i]; // 시작 위치 갱신
        }
    }
}

// MSD Radix Sort 메인 함수
// 가장 큰 자릿수부터 정렬 시작
void msdRadixSort(vector<int>& arr)
{
    int maxVal = getMax(arr); // 최대값 찾기
    int exp = pow(10, (int)log10(maxVal)); // 가장 큰 자릿수 계산
    msdRadixSortHelper(arr, 0, arr.size(), exp); // 재귀 호출로 정렬
}

//디버깅
void printArray(const vector<int>& arr)
{
    for (int num : arr)
        cout << num << " ";
    cout << endl;
}

int main()
{
    vector<int> arr;

    // 입력 파일에서 데이터 읽기
    ifstream inputFile("input.txt");
    if (!inputFile) { // 파일 열기 실패 처리
        cerr << "에러: 입력 파일 열기 실패" << endl;
        return 1;
    }

    int number;
    while (inputFile >> number) { // 숫자 읽어서 배열에 추가
        arr.push_back(number);
    }
    inputFile.close();

    // MSD Radix Sort로 배열 정렬
    msdRadixSort(arr);

    // 출력 파일에 정렬된 결과 쓰기
    ofstream outputFile("radix_msd_output.txt");
    if (!outputFile) { // 파일 열기 실패 처리
        cerr << "에러: 출력 파일 열기 실패" << endl;
        return 1;
    }

    for (int num : arr) { // 정렬된 배열 쓰기
        outputFile << num << " ";
    }
    outputFile.close();

    cout << "정렬 완료. radix_msd_output.txt에서 결과 확인 바람" << endl;
    return 0;
}
