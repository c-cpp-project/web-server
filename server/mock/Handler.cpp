#include "Handler.hpp"

// 매개변수가 있는 생성자의 구현
Handler::Handler(int fd) : fd(fd) {
    // 여기에 필요한 초기화 코드를 작성할 수 있습니다.
}

// 기본 생성자의 구현
Handler::Handler() : fd(-1) { // fd를 -1로 초기화하여 "유효하지 않음"을 나타냅니다.
    // 여기에 기본 초기화 코드를 작성할 수 있습니다.
}
