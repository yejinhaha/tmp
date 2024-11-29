import java.io.*;
import java.util.ArrayList;
import java.util.List;

class Bubble {

    // 버블 정렬, 그 유명한 느림보 정렬
    // 조금 최적화된 버전이긴 하지만 그래도 느림
    static void bubbleSort(int arr[], int n) {
        int i, j, temp;
        boolean swapped; // 한 번이라도 교환했는지 체크
        for (i = 0; i < n - 1; i++) {
            swapped = false; // 교환 여부 초기화
            for (j = 0; j < n - i - 1; j++) { // 이미 정렬된 부분 제외하고 반복
                if (arr[j] > arr[j + 1]) { // 옆집과 비교
                    // 서로 바꿔줌
                    temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                    swapped = true; // 교환 발생
                }
            }

            // 교환이 한 번도 안 일어났으면 정렬 완료
            if (!swapped)
                break;
        }
    }

    // 배열을 파일에 쓰는 함수
    static void printArrayToFile(int arr[], int size, String fileName) throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter(fileName)); // 파일에 쓰기 위한 Writer 열기
        for (int i = 0; i < size; i++) {
            writer.write(arr[i] + " "); // 배열 원소를 공백으로 구분해서 작성
        }
        writer.newLine(); // 한 줄 끝냄
        writer.close(); // Writer 닫기
    }

    // 파일에서 배열 읽어오는 함수
    static int[] readArrayFromFile(String fileName) throws IOException {
        BufferedReader reader = new BufferedReader(new FileReader(fileName)); // 파일 읽기 위한 Reader 열기
        List<Integer> list = new ArrayList<>(); // 읽은 데이터를 담을 리스트
        String line;

        while ((line = reader.readLine()) != null) { // 파일 끝까지 읽음
            String[] numbers = line.split("\\s+"); // 공백으로 나뉜 숫자들 추출
            for (String num : numbers) {
                list.add(Integer.parseInt(num)); // 숫자로 변환해서 리스트에 추가
            }
        }
        reader.close(); // Reader 닫기

        // List<Integer>를 int[]로 변환
        int[] arr = new int[list.size()];
        for (int i = 0; i < list.size(); i++) {
            arr[i] = list.get(i); // 리스트 내용을 배열로 복사
        }
        return arr;
    }

    // 메인 함수
    public static void main(String args[]) {
        try {
            // 입력 파일 이름
            String inputFileName = "input.txt";
            int[] arr = readArrayFromFile(inputFileName); // 파일에서 배열 읽어옴
            int n = arr.length; // 배열 길이 구하기

            bubbleSort(arr, n); // 버블 정렬 호출

            // 출력 파일 이름
            String outputFileName = "bubble_output.txt";
            printArrayToFile(arr, n, outputFileName); // 정렬된 배열을 파일에 쓰기
            System.out.println("Sorted array has been written to " + outputFileName); // 작업 완료 메시지

        } catch (IOException e) {
            // 파일 읽기/쓰기 에러 처리
            System.err.println("An error occurred: " + e.getMessage());
        }
    }
}
