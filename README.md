# 🛜 MyGameServerLibrary

본 프로젝트는 게임 서버 구현에 대한 이해를 목표로, TCP/IP 기반 소켓의 패킷 입출력 및 Windows의 I/O Completion Port를 통한 비동기 고속 입출력 방식의 내부 이해를 학습하고, 게임 서버를 위한 라이브러리를 구축함에 있어서 필요한 기능들에 대한 여러 클래스들의 유기적 동작 방식을 이해하며 게임 서버 아키텍처의 기능적·구조적인 이해를 목적으로 두고 있습니다. 
| Project | Role |
| --- | --- |
| `MyGameShared` | 공용 타입, 로깅, 스레드 관리, 동기화 자료구조 등 공용 기능 |
| `MyGameServerProtocol` | `.proto` 정의와 Protocol Buffers 기반 패킷 코드 |
| `MyGameServerLibrary` | IOCP 네트워크 코어, 세션, 버퍼, 서비스 및 패킷 처리 |
| `MyGameFramework` | 게임 오브젝트, 씬, 리소스, 렌더링, 입력 및 UI 프레임워크 |
| `MyGameServer` | 게임 서버 실행 파일과 세션·룸·오브젝트 관리 |
| `My2DSandbox` | 2D 클라이언트 및 네트워크 기능 검증용 샌드박스 실행 파일 |

# 📺 Development Preview

[https://github.com/user-attachments/assets/](https://github.com/user-attachments/assets/ee5f9852-2b28-47d7-a4bb-84368a843bbf)

### 1. 패킷 설계 및 클라이언트 보간
- 클라이언트는 게임 룸 입장, 이동·점프, 공격·무기 변경을 요청 패킷으로 서버에 전송하며, 서버는 이를 바탕으로 게임 로직을 처리합니다. 서버는 클라이언트의 요청을 권위적으로 처리하며 처리된 결과를 플레이어와 총알의 생성·삭제·상태 동기화 패킷 및 공격 이펙트 패킷으로 클라이언트에 전달합니다.
- 클라이언트는 이동을 먼저 예측한 뒤, 서버 위치와의 오차를 `glm::mix`로 선형 보간하여 보정하였습니다. 보정
  비율은 `glm::clamp`로 제한하며, 오차가 큰 경우에는 서버 위치로 즉시 동기화하였습니다.

### 2. 렌더링 파이프라인
- 클라이언트의 렌더링 파이프라인은 다음과 같은 순서로 렌더링 하였습니다. 

    > **Sprite Render Pass → Post-processing Pass → Debug Render Pass → UI Render Pass**

    각 명시적 렌더 패스를 활용하여 전체적으로 2D Forward 렌더링 방식을 구현하였습니다. Post Processing 효과로는 색수차(Chromatic Aberration), CRT 모니터 효과, 피격 시 붉은 색 비네팅 효과를 적용하였습니다.


- 드로우 콜을 줄이기 위해 하나의 아틀라스 텍스쳐로 관리하고, 각 스프라이트의 월드 행렬, 아틀라스 텍스처 내 UV 영역, 색상, 크기를 인스턴싱 속성으로 전달하였습니다. 동일한 아틀라스 텍스처를 사용하는 스프라이트들은 하나의 Render Batch로 묶으며, 공용 Quad Mesh와 `glDrawElementsInstanced`를 사용하여 여러 스프라이트를 한 번에 렌더링하였습니다.

# 🧑‍💻 Technology Stack & Libraries
| **Library / API**            | **Usage**                                                   | **Used In**                       |
| ---------------------------- | ----------------------------------------------------------- | --------------------------------- |
| [Protocol Buffers](https://github.com/protocolbuffers/protobuf)             | `.proto` 기반 네트워크 패킷 정의            | `MyGameServerProtocol`            |
| [spdlog](https://github.com/gabime/spdlog)                       | 서버 패킷 디버그 로깅                                                  | `MyGameShared / Logger`           |
| [OpenGL](https://www.opengl.org/) / [glew](https://github.com/nigels-com/glew) / [glm](https://github.com/g-truc/glm)          | 2D 렌더링 파이프라인, 그래픽 리소스 처리 및 좌표 변환 수학 연산                  | `MyGameShared`, `MyGameFramework` |
| [KTX-Software](https://github.com/KhronosGroup/KTX-Software)                 | KTX/KTX2 텍스처 로딩 및 OpenGL 텍스처 업로드                            | `GLTexture`, `Texture2D`          |
| [FreeType](https://github.com/freetype/freetype)                     | 폰트 로딩 및 글리프 아틀라스 생성                                         | `Font`                            |
| [Windows API (Winsock2, IOCP)](https://learn.microsoft.com/en-us/windows/win32/winsock/windows-sockets-start-page-2) | TCP 소켓 통신 및 Overlapped I/O 기반 비동기 네트워크 이벤트 처리               | `MyGameServerLibrary`             |


# 🛠️ 문제 및 개선 사항
현재 버전은 핵심 기능 구현과 네트워크 구조 학습을 우선적으로 마무리한 상태이며,
향후 스트레스 테스트와 프로파일링을 통해 실제 병목을 측정하고 개선 방안을 모색할 예정입니다.

1. **대규모 접속 환경 성능 검증** : 50 ~ 1000 플레이어 환경에서 서버 Tick Time, CPU/GPU 부하, Packet/sec 및 대역폭 측정을 통한 Client / Server / Network 중 실제 병목 지점 분석.
2. **서버 Collision Broad Phase 개선** : Collider 증가에 따른 Pair Test 및 Collision Update 비용 측정 및 충돌 알고리즘 개선.
3. **네트워크 동기화 및 보간 개선** : 서버 Tick에 대해서 60 / 30 / 20 / 10 Hz Sync Rate에 따른 네트워크 비용 비교 및 위치·속도 등 실수형 상태값에 대해 이전/현재 서버 상태를 기준으로 시간 기반 선형 보간을 적용하여 낮은 Sync Rate에서도 시각적 품질을 유지할 수 있는지 검증.
4. **IOCP 및 메모리 최적화** : SendBuffer / RecvBuffer의 Heap 할당 병목 및 IOCP 워커 스레드 수에 따른 성능 비교를 통해 실제 병목 확인 시 Thread-Local한 버퍼 도입 검토.
5. **IOCP 아키텍처 복습 및 재구현** : Overlapped I/O, IOCP Completion 흐름 및 Session Lifetime 재검증하며 Echo Server부터 Session / Buffer / Worker 구조를 단계적으로 재구현을 통한 복습.