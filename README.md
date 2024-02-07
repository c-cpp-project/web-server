# 핵심 변경 사항

## Controller

1. MyController: nginx 설정 파일에서 uri의 경로가 location에 등록된 경우, Request가 허용 메서드인 경우 내부적으로 적절한 uri로 변환 후에 Controller 실행
2. DefaultController: nginx 설정 파일에서 uri의 경로가 등록되지 않은 경우, GET 메서드로 uri 그대로 실행한다.

## ResponseConfig.hpp
1. Controller를 설정하기 위한 Response 설정 파일로 serverConfigs을 생성자의 매개변수로 받아 std::map에 등록한다.

## default.conf에 맞게 설정값 변경하기

# 추가 변경 사항

## transfer-encoding:chunked
"HTTP 1.1의 문자열" 중복 여부 검사에서 "sendBody의 중복 검사"로 변경 이를 통해 response message는 항상 하나임을 예외없이 구현 가능하다.

## Range-request
get에서 일부 데이터만 보내준다.

## Cookie-Sesstion

