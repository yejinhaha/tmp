#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// 배열에서 최대값을 찾는 유틸리티 함수
// 최대값 찾아야 자릿수를 계산할 수 있음
int getMax(vector<int>& arr)
{
    int mx = arr[0];
    for (int i = 1; i < arr.size(); i++) // 배열 돌면서 최대값 찾기
        if (arr[i] > mx)
            mx = arr[i];
    return mx;
}

// 주어진 자릿수(exp) 기준으로 카운팅 정렬하는 함수
// LSD Radix Sort의 핵심 중 하나
void countSort(vector<int>& arr, int exp)
{
    vector<int> output(arr.size()); // 정렬된 결과를 담을 배열
    int count[10] = { 0 }; // 각 자릿수(0~9)의 빈도를 저장하는 배열

    // 자릿수 빈도 계산
    for (int i = 0; i < arr.size(); i++)
        count[(arr[i] / exp) % 10]++;

    // 누적 합으로 변환 (정렬 위치 결정)
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // 정렬된 배열 생성
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--; // 사용한 위치 감소
    }

    // 정렬 결과를 원래 배열로 복사
    for (int i = 0; i < arr.size(); i++)
        arr[i] = output[i];
}

// Radix Sort 메인 함수
// LSD 방식으로 자릿수별로 정렬
void radixsort(vector<int>& arr)
{
    // 최대값을 찾아서 자릿수 계산
    int m = getMax(arr);

    // 자릿수별로 카운팅 정렬
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, exp);
}

// 디버깅
void print(vector<int>& arr)
{
    for (int i = 0; i < arr.size(); i++)
        cout << arr[i] << " ";
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

    // 배열 정렬 (Radix Sort 실행)
    radixsort(arr);

    // 출력 파일에 정렬 결과 쓰기
    ofstream outputFile("radix_lsd_output.txt");
    if (!outputFile) { // 파일 열기 실패 처리
        cerr << "에러: 출력 파일 열기 실패" << endl;
        return 1;
    }

    for (int num : arr) { // 정렬된 배열 파일에 쓰기
        outputFile << num << " ";
    }
    outputFile.close();

    cout << "정렬 완료. radix_lsd_output.txt에서 결과 확인 바람" << endl;
    return 0;
}
