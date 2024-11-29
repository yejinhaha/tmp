#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <climits>
using namespace std;

// 삽입 정렬 함수
// 진짜 삽입 정렬로 배열 정렬. 느리지만 외부 정렬에서는 쓸만함
void insertionSort(vector<int>& arr) {
    for (int i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 초기 정렬된 덩어리(런)를 만들어 임시 파일에 저장
// 대충 잘게 나눠 정렬하고 임시 파일로 던져버리는 함수
void createInitialRuns(const string& inputFile, int runSize, int numWays) {
    ifstream inFile(inputFile);
    if (!inFile) {
        cerr << "입력 파일 열기 실패. 파일 경로 확인 바람." << endl;
        exit(1);
    }

    vector<int> buffer(runSize);
    int runIndex = 0;

    while (!inFile.eof()) {
        int i = 0;
        // 메모리에 runSize만큼 읽어들임 EOF까지 반복
        while (i < runSize && inFile >> buffer[i]) {
            i++;
        }

        // 읽은 데이터 있으면 정렬 후 임시 파일로 저장
        if (i > 0) {
            buffer.resize(i); // 실제 데이터 크기에 맞게 크기 조정
            insertionSort(buffer); // 삽입 정렬 호출

            ofstream outFile("temp" + to_string(runIndex++) + ".txt");
            for (int num : buffer) {
                outFile << num << " ";
            }
            outFile.close();
        }
    }
    inFile.close(); // 파일 닫기. 안 닫으면 문제가 생길 수도 있음.
}

// 정렬된 임시 파일 병합해서 최종 출력 파일 생성
// 병렬 처리? 그런 거 없음 걍 하나씩 병합함
void mergeRuns(const string& outputFile, int numWays) {
    vector<ifstream> inFiles(numWays);
    vector<int> currentElements(numWays, INT_MAX);

    // 임시 파일 열기 및 첫 번째 숫자 읽기
    for (int i = 0; i < numWays; i++) {
        inFiles[i].open("temp" + to_string(i) + ".txt");
        if (!inFiles[i]) {
            cerr << "임시 파일 temp" << i << ".txt 열기 실패 뭐 잘못됐겠지" << endl;
            exit(1);
        }
        if (inFiles[i] >> currentElements[i]) {
            continue; // 첫 번째 숫자 읽기 성공
        }
        currentElements[i] = INT_MAX; // EOF 처리
    }

    ofstream outFile(outputFile);
    while (true) {
        int minIndex = -1;
        int minValue = INT_MAX;

        // 현재 숫자 중 최소값 찾기
        for (int i = 0; i < numWays; i++) {
            if (currentElements[i] < minValue) {
                minValue = currentElements[i];
                minIndex = i;
            }
        }

        // 파일에서 더 이상 읽을 데이터가 없으면 종료
        if (minIndex == -1) {
            break;
        }

        outFile << minValue << " "; // 최소값 쓰기
        if (!(inFiles[minIndex] >> currentElements[minIndex])) {
            currentElements[minIndex] = INT_MAX; // EOF 처리
        }
    }

    outFile.close();
    for (int i = 0; i < numWays; i++) {
        inFiles[i].close(); // 열었던 파일 다 닫기
    }
}

int main() {
    const string inputFile = "input.txt"; // 입력 파일
    const string outputFile = "external_output.txt"; // 출력 파일
    const int runSize = 100; // 메모리에 올릴 수 있는 데이터 크기
    const int numWays = 10; // 임시 파일 개수

    // Step 1: 임시 파일 생성
    // 대충 데이터를 정렬해서 임시 파일에 던짐
    createInitialRuns(inputFile, runSize, numWays);

    // Step 2: 병합
    // 임시 파일들 병합해서 최종 정렬 결과 파일 생성
    mergeRuns(outputFile, numWays);

    cout << "정렬 완료. " << outputFile << " 확인 바람." << endl;
    return 0;
}
