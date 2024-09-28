# 개발환경 세팅

해당 소스코드의 루트 디렉터리의 이름은 `cmake_project` 입니다.

`CMakeLists.txt` 파일내 본인의 프로젝트 이름에 맞게 수정해주세요.

## 빌드

```bash
cmake -Bbuild . -DCMAKE_BUILD_TYPE=Debug
```

```bash
cmake --build build
```

위 명령어를 통해 프로그램 파일을 빌드할 수 있습니다.

Visual Studio Code 의 CMake Extension 을 사용해 명령어 입력의 반복을 간소화 하거나, 본인은 .zshrc 나 .bashrc 에 alias 로 명령어를 간소화 해주었습니다.
