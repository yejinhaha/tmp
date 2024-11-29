import java.io.*;
import java.util.ArrayList;
import java.util.List;

class ShellSort {
    //ㄷㅂㄱ
    static void printArray(int arr[]) {
        for (int value : arr)
            System.out.print(value + " ");
        System.out.println();
    }

    static void writeArrayToFile(int arr[], String fileName) throws IOException {
        BufferedWriter writer = new BufferedWriter(new FileWriter(fileName));
        for (int value : arr) {
            writer.write(value + " "); // 배열 원소를 공백으로 구분해서 기록
        }
        writer.newLine(); // 파일 끝에 줄 바꿈 추가
        writer.close(); // 파일 닫기
    }

    static int[] readArrayFromFile(String fileName) throws IOException {
        BufferedReader reader = new BufferedReader(new FileReader(fileName));
        List<Integer> list = new ArrayList<>(); // 동적으로 크기가 변하는 리스트 사용
        String line;

        while ((line = reader.readLine()) != null) { // 파일 끝까지 읽음
            String[] numbers = line.split("\\s+"); // 공백을 기준으로 숫자 분리
            for (String num : numbers) {
                list.add(Integer.parseInt(num)); // 문자열을 정수로 변환 후 리스트에 추가
            }
        }
        reader.close(); // 파일 닫기

        // List<Integer>를 int[]로 변환
        int[] arr = new int[list.size()];
        for (int i = 0; i < list.size(); i++) {
            arr[i] = list.get(i); // 리스트의 값을 배열로 복사
        }
        return arr;
    }

    /* Shell Sort를 사용하여 배열 정렬 */
    // 간격 배열(gap sequence)을 미리 정의한 상태로 정렬
    int sort(int arr[]) {
        int n = arr.length;

        // 미리 정의된 간격 배열
        int[] gaps = {100, 50, 10, 5, 1};

        for (int gap : gaps) {
            if (gap >= n) continue; // 배열 크기보다 큰 간격은 무시

            // 현재 간격 크기만큼 삽입 정렬 수행
            for (int i = gap; i < n; i++) {
                int temp = arr[i]; // 현재 원소 저장
                int j;

                // 간격 크기만큼 떨어진 이전 원소들과 비교해 위치 조정
                for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                    arr[j] = arr[j - gap]; // 큰 값을 뒤로 이동
                }
                arr[j] = temp; // 저장한 원소를 적절한 위치에 삽입
            }
        }
        return 0; // 반환값은 사용되지 않음
    }

    // 실행 메서드
    public static void main(String args[]) {
        String inputFile = "input.txt"; // 입력 파일 이름
        String outputFile = "shell_output.txt"; // 출력 파일 이름

        try {
            // 파일에서 배열 읽기
            int[] arr = readArrayFromFile(inputFile);

            System.out.println("정렬 전 배열:");
            printArray(arr); // 정렬 전 배열 출력

            // 배열 정렬
            ShellSort ob = new ShellSort();
            ob.sort(arr);

            System.out.println("정렬 후 배열:");
            printArray(arr); // 정렬 후 배열 출력

            // 정렬된 배열을 파일에 쓰기
            writeArrayToFile(arr, outputFile);
            System.out.println("정렬된 배열이 " + outputFile + "에 저장됨");

        } catch (IOException e) { // 파일 처리 중 예외 처리
            System.err.println("오류 발생: " + e.getMessage());
        }
    }
}
