#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// 선택 정렬 함수
// "작은 값부터 찾아서 앞에 쌓는 방식, 비효율적 벗 직관적임
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {

        // 현재 위치를 최소값의 인덱스로 가정
        int min_idx = i;

        // 정렬되지 않은 나머지 배열에서 실제 최소값 찾기
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                // 더 작은 값 발견 시 최소값 인덱스 갱신
                min_idx = j;
            }
        }

        // 현재 위치와 최소값의 위치를 교환
        int temp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = temp;
    }
}

//ㄷㅂㄱ
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 배열을 파일에 쓰는 함수
void writeArrayToFile(int arr[], int n, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Unable to open file %s for writing.\n", filename);
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "%d ", arr[i]); // 배열 원소를 파일에 저장
    }
    fprintf(file, "\n");
    fclose(file);
}

// 파일에서 배열을 읽는 함수
int readArrayFromFile(int arr[], int max_size, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file %s for reading.\n", filename);
        exit(1);
    }

    int n = 0;
    while (n < max_size && fscanf(file, "%d", &arr[n]) != EOF) { // 파일에서 정수 읽기
        n++;
    }
    fclose(file);
    return n; // 읽은 원소 개수 반환
}

int main() {
    const char* input_file = "input.txt"; // 입력 파일 이름
    const char* output_file = "selection_output.txt"; // 출력 파일 이름
    int arr[100]; // 최대 배열 크기
    int n;

    // 파일에서 배열 읽기
    n = readArrayFromFile(arr, 100, input_file);

    printf("Original array: ");
    printArray(arr, n); // 원래 배열 출력

    // 배열 정렬 (선택 정렬 호출)
    selectionSort(arr, n);

    printf("Sorted array: ");
    printArray(arr, n); // 정렬된 배열 출력

    // 정렬된 배열을 파일에 쓰기
    writeArrayToFile(arr, n, output_file);

    printf("정렬된 배열은 여기에 저장됨: %s\n", output_file); // 완료 메시지 출력

    return 0;
}
