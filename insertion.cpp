#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/* 삽입 정렬 함수 */
// 배열을 삽입 정렬로 정렬함
// 데이터 크기 작을 때만 쓸 만한 알고리즘
void insertionSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        // 현재 요소를 정렬된 부분으로 삽입
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j]; // 더 큰 요소를 한 칸씩 뒤로
            j = j - 1;
        }
        arr[j + 1] = key; // 적절한 위치에 삽입
    }
}

/* 배열을 콘솔에 출력하는 함수 */
// 그냥 배열 상태 확인용
void printArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

/* 배열을 파일에 쓰는 함수 */
void writeArrayToFile(const vector<int>& arr, const string& filename) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "에러: " << filename << " 파일 열기 실패" << endl;
        exit(1);
    }
    for (int num : arr) {
        outfile << num << " "; // 공백으로 구분해서 쓰기
    }
    outfile << endl;
    outfile.close(); // 파일 닫기
}

/* 파일에서 배열을 읽는 함수 */
vector<int> readArrayFromFile(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "에러: " << filename << " 파일 열기 실패" << endl;
        exit(1);
    }
    vector<int> arr;
    int num;
    while (infile >> num) { // 공백이나 줄바꿈 기준으로 숫자 읽기
        arr.push_back(num);
    }
    infile.close(); // 파일 닫기
    return arr; // 읽은 데이터를 배열로 반환
}

// 메인 함수
int main() {
    const string input_file = "input.txt"; // 입력 파일
    const string output_file = "insertion_output.txt"; // 출력 파일

    // 파일에서 배열 읽기
    vector<int> arr = readArrayFromFile(input_file);

    cout << "원래 배열: ";
    printArray(arr); // 정렬 전 배열 출력

    // 배열 정렬
    insertionSort(arr);

    cout << "정렬된 배열: ";
    printArray(arr); // 정렬 후 배열 출력

    // 정렬된 배열을 파일에 쓰기
    writeArrayToFile(arr, output_file);

    cout << "정렬된 배열이 " << output_file << " 에 저장됨" << endl;

    return 0;
}
