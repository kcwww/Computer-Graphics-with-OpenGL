# OpenGL을 활용한 Computer Graphics 실습

## Introduction

해당 학습 내용 및 실습코드는 [강원대학교 컴퓨터 그래픽스 강의](https://www.youtube.com/watch?v=kEAKvJKnvfA&list=PLvNHCGtd4kh_cYLKMP_E-jwF3YKpDP4hf&index=1) 를 참고하여 만들어졌습니다.

## Requirement

c++ 컴파일러, cmake가 필요합니다.

빌드하길 원하는 디렉토리로 이동한 뒤 cmake를 통해 빌드합니다

```bash
$ cmake 빌드디렉터리경로 -Bbuild
$ cmake --build 빌드디렉터리경로
```

빌드가 완료되면 아래 경로에 실행 파일이 생성됩니다.

```bash
$ ./빌드디렉터리/프로젝트이름
```

시리즈별 README.md 에 자세한 내용을 확인하실 수 있습니다.

## 시리즈일람

| Seires                                                                                | Description                                             |
| ------------------------------------------------------------------------------------- | ------------------------------------------------------- |
| [series01](https://github.com/kcwww/Computer-Graphics-with-OpenGL/tree/main/series01) | Computer Graphics 를 실습하기 위한 개발환경 세팅입니다. |
| [series02](https://www.chanwooyam.dev/series/computer-graphics/2IYzBkIto4h4tlnClmzU)  | OpenGL 을 활용한 GLFW 기초 예제입니다.                  |
| [series03](https://github.com/kcwww/Computer-Graphics-with-OpenGL/tree/main/series03) | OpenGL 그래픽스 파이프라인 구축하기                     |
| [series04](https://github.com/kcwww/Computer-Graphics-with-OpenGL/tree/main/series04) | shader 로 삼각형 그리기 (with. GLSL)                    |
| [series05](https://github.com/kcwww/Computer-Graphics-with-OpenGL/tree/main/series05) | Texture in OpenGL      |
| [series06](https://github.com/kcwww/Computer-Graphics-with-OpenGL/tree/main/series06) | 선형 변환을 이용한 큐브 그리기 (with. GLM)   |
| [series07](https://github.com/kcwww/Computer-Graphics-with-OpenGL/tree/main/series07) | 카메라 조작하기 |
