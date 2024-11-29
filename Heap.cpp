#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// 힙을 재정렬하는 함수
// 노드 i를 기준으로 서브트리를 힙 형태로 맞춤
void heapify(vector<int>& arr, int n, int i) {

    // 현재 노드를 루트로 설정
    int largest = i;

    // 왼쪽 자식의 인덱스
    int l = 2 * i + 1;

    // 오른쪽 자식의 인덱스
    int r = 2 * i + 2;

    // 왼쪽 자식이 루트보다 크다면 갱신
    if (l < n && arr[l] > arr[largest])
        largest = l;

    // 오른쪽 자식이 현재 가장 큰 노드보다 크다면 갱신
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // 가장 큰 노드가 루트가 아니라면 교환
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // 영향을 받은 서브트리를 재귀적으로 힙 정렬
        heapify(arr, n, largest);
    }
}

// 힙 정렬 함수
// "루트 뽑고 다시 힙 만들고 반복" 이거 계속 함
void heapSort(vector<int>& arr) {
    int n = arr.size();

    // 힙 생성 (배열을 힙 구조로 변환)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // 힙에서 하나씩 원소를 추출
    for (int i = n - 1; i > 0; i--) {

        // 현재 루트를 끝으로 보냄
        swap(arr[0], arr[i]);

        // 줄어든 힙에서 다시 정렬
        heapify(arr, i, 0);
    }
}

// 배열을 콘솔에 출력하는 유틸리티 함수
// 사실 디버깅용이긴 한데 그냥 둠
void printArray(vector<int>& arr) {
    for (int i = 0; i < arr.size(); ++i)
        cout << arr[i] << " ";
    cout << "\n";
}

// 배열을 파일에 쓰는 함수
void writeArrayToFile(vector<int>& arr, const string& fileName) {
    ofstream outFile(fileName);
    if (!outFile) {
        cerr << "에러: " << fileName << " 파일 열기 실패\n";
        exit(1);
    }
    for (int num : arr) {
        outFile << num << " ";
    }
    outFile << "\n";
    outFile.close();
}

// 파일에서 배열을 읽어오는 함수
vector<int> readArrayFromFile(const string& fileName) {
    ifstream inFile(fileName);
    if (!inFile) {
        cerr << "에러: " << fileName << " 파일 열기 실패\n";
        exit(1);
    }
    vector<int> arr;
    int num;
    while (inFile >> num) {
        arr.push_back(num);
    }
    inFile.close();
    return arr;
}

// 메인 함수
int main() {
    string inputFile = "input.txt"; // 입력 파일 이름
    string outputFile = "heap_output.txt"; // 출력 파일 이름

    // 파일에서 배열 읽기
    vector<int> arr = readArrayFromFile(inputFile);

    cout << "정렬 전 배열:\n";
    printArray(arr);

    // 힙 정렬 실행
    heapSort(arr);

    cout << "정렬 후 배열:\n";
    printArray(arr);

    // 정렬된 배열 파일에 쓰기
    writeArrayToFile(arr, outputFile);
    cout << "정렬된 배열이 " << outputFile << " 에 저장됨\n";

    return 0;
}
