# Cornix용 ZMK 키보드

## 보드 및 실드 소개

이 저장소에는 Cornix 분할 키보드용 ZMK 펌웨어 설정이 포함되어 있습니다. 아래에서는 이 프로젝트에서 사용할 수 있는 여러 보드와 실드를 설명합니다.

### 보드

프로젝트에는 다음과 같은 세 가지 주요 보드 정의가 포함되어 있습니다.

- **`cornix_left`**: 동글 없이 펌웨어를 빌드할 때 사용하는 Cornix 분할 키보드의 왼쪽 절반입니다.
- **`cornix_right`**: 분할 키보드 구성에서 슬레이브 측으로 사용하는 Cornix 분할 키보드의 오른쪽 절반입니다.
- **`cornix_ph_left`**: 동글 구성에서 사용하도록 특별히 설계된 대체 왼쪽 절반 보드 설정입니다.

### 실드

프로젝트에는 추가 기능을 제공하는 여러 특수 실드가 포함되어 있습니다.

- **`cornix_dongle_adapter`**: 동글 구성에 필요한 키 매트릭스 및 Bluetooth 공통 기능을 제공합니다. Cornix를 사용자 지정 동글과 함께 사용할 때 필요한 실드입니다.
- **`cornix_dongle_eyelash`**: 동글 보드용 디스플레이 장치를 설정하는 예제 실드입니다. 보드의 장치 트리에 `zephyr,display`가 아직 정의되어 있지 않을 때 사용합니다.
- **`cornix_indicator`**: 각 측면의 RGB LED 2개로 배터리 상태와 연결 상태를 표시하는 실드입니다. 기본 좌측·동글용 좌측·우측 빌드에서 활성화되어 있으며, 점등 중에는 전력 소비가 증가합니다.

---

이 커뮤니티 펌웨어는 ZMK를 사용하는 Cornix에서 테스트되었으며, ZMK 분할 키보드 지침에 따른 완전한 분할 역할 설정, 배터리 전원 관리, Bluetooth 센트럴/페리페럴 설정을 제공합니다.


![이미지](images/cornix_with_dongle.png)
![이미지](images/cornix_layout.png)

## 경고: 장치 작동 불능 상태 복구

기존 Cornix는 SoftDevice가 없는 플래시 레이아웃을 사용합니다.
따라서 이 프로젝트의 모든 보드는 기본적으로 `nosd` 레이아웃을 사용합니다.

동글에 펌웨어를 플래시한 후 기존 펌웨어와 함께 작동하지 않는 경우에는 다음 두 가지 해결 방법이 있습니다.

1. **(권장)** `bootloader` 디렉터리에 있는 SD 복원 UF2를 플래시합니다. 이 파일은 nice!nano v2용이지만 대부분의 nRF52840 장치에서도 작동할 것으로 보입니다. 다른 보드용 파일은 다음 링크를 참조하세요: https://github.com/hitsmaxft/Adafruit_nRF52_Bootloader/actions/runs/18398554358
2. `nrf52840-nosd` 스니펫을 사용하여 펌웨어를 빌드하고 ZMK가 SoftDevice를 무시하도록 합니다.


## 할 일 목록

- [x] 52키 전체 레이아웃 키맵(v2.0부터)
- [x] EC11 인코더(v2.2부터)
- [x] no-SD 이미지(v2.3부터)
- [x] 다양한 동글 지원
- [x] Zephyr 4.1 및 LVGL 9으로 업그레이드(v2.7부터, 아직 동글 화면 미지원)
- [x] v3부터 RGB 배터리 및 연결 상태 표시 지원


### RGB 소개

Cornix에는 각 측면에 RGB LED가 2개씩 있습니다. 이 저장소의 기본 좌측·동글용 좌측·우측 빌드는 `cornix_indicator` 실드와 `zmk-rgbled-widget`을 사용하여 배터리 및 분할 연결 상태를 표시합니다.

표시가 끝나고 LED가 유휴 상태가 되면 WS2812 외부 전원을 1초 뒤 차단하여 대기 전력 소비를 줄입니다. 실제로 점등된 LED는 추가 전력을 소비합니다.

## 지원 하드웨어: Cornix 분할 키보드

Cornix 분할형 텐팅 로우 프로파일 인체공학 키보드(Jezail Funder)

Cornix는 Corne에서 영감을 받은 분할형 인체공학 키보드로, 소형 3×6 컬럼 스태거드 배열과 6개의 엄지 클러스터 키(각 절반에 3개)를 갖추고 있습니다. 텐팅 각도를 10°, 18°, 25°로 조절할 수 있어 손목의 부담을 줄이고 사용자에게 맞는 인체공학적 정렬을 찾을 수 있습니다.

- **분할형 컬럼 스태거드 배열**(3×6 + 엄지 클러스터 배열)
- 10°, 18°, 25°의 **조절식 텐팅 지원**(하드웨어 기반이며 펌웨어 수정 불필요)
- **Kailh Choc V2 핫스왑 소켓** 및 LAK 또는 LCK 로우 프로파일 키캡 지원
- **듀얼 모드 연결**: 유선 USB-C 또는 Bluetooth 무선 연결(왼쪽 절반이 마스터)
- **펌웨어**: 키맵과 레이어 사용자 지정을 위한 VIAL 완전 지원. 기본 펌웨어는 RMK입니다.
- 고급 **CNC 가공 알루미늄 섀시**, 사용자 지정 흡음 폼, 휴대용 보관 파우치

> 이 프로젝트의 소유자도 RMK 기여자입니다. RMK도 응원해 주세요: https://rmk.rs/

## --부트로더 복구 방법--

-- 기존 RMK 펌웨어에서는 SoftDevice가 제거되었으므로 `zmk.uf2`를 플래시하기 전에 먼저 SoftDevice를 복원해야 합니다. 자세한 단계는 [bootloader/README.md](./bootloader/README.md)를 참조하세요. --

v2.3부터 이 보드의 플래시 파티션이 업데이트되어 SD가 제거되었습니다(SD 파티션 크기를 150K에서 4K로 축소). 따라서 펌웨어를 바로 플래시할 수 있습니다.

> 이전 버전의 `reset.uf2`를 사용하여 펌웨어를 초기화해야 할 수도 있습니다.

> `rmkfw/`에 백업된 원본 UF2 파일을 플래시하면 기본 펌웨어로 되돌릴 수 있습니다.

## 🔰 쉬운 방법: 이 저장소를 복제하고 GitHub Actions로 빌드하기

ZMK를 처음 사용하고 `west.yml`이나 모듈 관리를 직접 다루고 싶지 않다면, 이 저장소를 그대로 사용하여 펌웨어를 사용자 지정할 수 있습니다.

### 단계

1. **이 저장소 포크 또는 복제**
   - 오른쪽 위의 **Fork**를 클릭하여 이 저장소를 자신의 GitHub 계정으로 복사하거나,
   - 로컬에서 `git clone`을 실행합니다.

   > GitHub Actions가 펌웨어를 자동으로 빌드하므로 포크를 권장합니다.

2. **키맵 편집**
   - `config/cornix.keymap`에서 키맵 파일을 찾습니다(또는 사용자 지정할 다른 `.keymap` 파일을 선택합니다).
   - 파일을 직접 편집하거나 [ZMK Keymap Editor](https://nickcoutsos.github.io/keymap-editor/)를 사용할 수 있습니다.
     - 편집기를 열고 `.keymap` 파일을 불러옵니다.
     - 시각적 편집기에서 원하는 내용을 변경합니다.
     - 업데이트된 파일을 다운로드하여 저장소의 기존 파일을 교체합니다.
     - 변경 사항을 커밋하고 GitHub에 푸시합니다.

3. **GitHub Actions로 빌드**
   - 푸시하면 GitHub Actions가 자동으로 빌드를 실행합니다.
   - 워크플로가 완료되면 **Actions → 가장 최근 실행 → Artifacts**로 이동하여 펌웨어(`.uf2`) 파일을 다운로드합니다.

4. **키보드에 플래시**
   - 보드를 UF2 부트로더 모드로 전환합니다(일반적으로 리셋 버튼을 빠르게 두 번 누릅니다).
   - 마운트된 드라이브에 `.uf2` 파일을 끌어다 놓습니다.

### 이런 분께 적합합니다

- ZMK 입문자
- 키맵만 사용자 지정하려는 사용자
- 드라이버나 하드웨어 정의를 수정할 필요가 없는 사용자

## Cornix ZMK 펌웨어를 처음부터 빌드하는 방법

이 절에서는 공식 ZMK 펌웨어 개발 절차를 사용하여 Cornix ZMK 펌웨어를 처음부터 빌드하는 방법을 안내합니다.


### 사전 준비 사항

시작하기 전에 다음 항목을 준비하세요.

- GitHub 계정
- 시스템에 설치된 Git
- Git 및 GitHub에 대한 기본 지식
- 준비된 Cornix 키보드 PCB

### 1단계: ZMK 설정 저장소 초기화

1. 공식 ZMK 설정 템플릿을 사용하여 **새 저장소를 생성**합니다.
   - 다음 페이지를 방문합니다: https://github.com/zmkfirmware/unified-zmk-config-template
   - **Use this template** → **Create a new repository**를 클릭합니다.
   - 저장소 이름을 지정합니다(예: `cornix-zmk-config`).
   - 원하는 대로 **Public** 또는 **Private**을 선택합니다.
   - **Create repository**를 클릭합니다.

2. **새 저장소를 로컬에 복제**합니다.
   ```bash
   git clone https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
   cd YOUR_REPO_NAME
   ```

3. **ZMK 개발 환경을 초기화**합니다.
   ```bash
   west init -l config/
   west update
   west zephyr-export
   ```

> **중요**: ZMK 펌웨어 개발에는 학습이 필요하므로 계속하기 전에 ZMK 문서를 충분히 읽어보세요.
> - ZMK 사용자 지정 가이드: https://zmk.dev/docs/customization
> - ZMK 설정: https://zmk.dev/docs/user-setup

### 2단계: 프로젝트에 Cornix 실드 추가

zmk-config 저장소를 초기화한 후 다음 절의 단계에 따라 Cornix 실드를 통합합니다.

## 기존 ZMK 프로젝트에 Cornix 실드를 추가하는 방법

기존 zmk-config를 사용하는 경우 `west.yml`을 통해 이 저장소를 의존성으로 추가하고 `west update`로 최신 버전을 가져오세요.

### 1. west.yml 수정

`config/west.yml` 파일을 편집하여 `manifest/remotes` 섹션에 다음 내용을 추가합니다.

```yaml
remotes:
  - name: zmkfirmware
    url-base: https://github.com/zmkfirmware
  - name: cornix-shield
    url-base: https://github.com/hitsmaxft
  - name: urob
    url-base: https://github.com/urob
```

`manifest/projects` 섹션에 다음 내용을 추가합니다.

```yaml
projects:
  - name: zmk
    remote: zmkfirmware
    revision: main
    import: app/west.yml
  - name: zmk-keyboard-cornix
    remote: cornix-shield
    revision: main
  - name: zmk-helpers
    remote: urob
    revision: main
```

### 2. 의존성 업데이트

```bash
west update
```

### 3. 빌드 설정

`build.yaml` 파일을 편집하여 다음 내용을 추가합니다.

> [!NOTE]
> 1. 동글 없이 (기본) Cornix를 사용하는 경우 `cornix_left`, `cornix_right`, `reset`을 선택하세요.
> 2. 동글과 함께 Cornix를 사용하는 경우 `cornix_dongle`, `cornix_left_for_dongle`, `cornix_right`, `reset`을 선택하세요.
> 3. 이 저장소의 기본 빌드에는 `cornix_indicator` 실드가 이미 추가되어 있습니다. RGB LED를 사용하지 않으려면 각 board 항목에서 실드를 제거하세요.

```yaml
include:
  # Use cornix with dongle
  - board: nice_nano
    shield: cornix_dongle_adaptor cornix_dongle_eyelash dongle_display
    snippet: studio-rpc-usb-uart
    artifact-name: cornix_dongle

  - board: cornix_ph_left
    shield: cornix_indicator
    artifact-name: cornix_left_for_dongle

  # Use cornix without dongle
  - board: cornix_left
    shield: cornix_indicator
    artifact-name: cornix_left

  - board: cornix_right
    shield: cornix_indicator
    artifact-name: cornix_right

  - board: cornix_right
    shield: settings_reset
    artifact-name: reset
```

### 4. 펌웨어 빌드

선호하는 방법으로 빌드합니다.

- v2.3부터는 SD를 복구할 필요가 없습니다.
- Cornix 양쪽에 `reset.uf2`를 플래시합니다.
- 왼쪽과 오른쪽 UF2 파일을 각각 플래시합니다.
- 양쪽을 동시에 리셋합니다.

### 5. 펌웨어 플래시

생성된 `.uf2` 파일을 해당 마이크로컨트롤러에 플래시합니다.

- 왼쪽 절반: `build/left/zephyr/zmk.uf2`
- 오른쪽 절반: `build/right/zephyr/zmk.uf2`

## 사용자 지정 동글 사용자를 위한 동글 어댑터 실드

자체 동글 설정을 만들고 싶은 사용자를 위해 이 저장소는 어댑터 실드를 제공합니다. Cornix 동글의 전체 설정에는 여러 실드를 함께 사용할 수 있습니다.

1. **`cornix_dongle_adapter`** - 키 매트릭스 및 Bluetooth 기능을 위한 공통 실드입니다.
2. **`dongle_display`** - 동글 화면용 디스플레이 모듈(또는 다른 디스플레이 프로젝트)입니다.
3. **`cornix_dongle_eyelash`** - 보드에 디스플레이 장치를 설정하기 위한 예제 실드입니다. 보드의 장치 트리에 이미 `zephyr,display`가 있으면 이 디스플레이 오버레이 실드는 필요하지 않습니다.

`build.yaml` 파일의 설정은 eyelash 동글에서 이러한 실드를 사용하는 방법을 보여줍니다.

```yaml
include:
  # Use cornix with dongle
  - board: nice_nano
    shield: cornix_dongle_adapter cornix_dongle_eyelash dongle_display
    snippet: studio-rpc-usb-uart
    artifact-name: cornix_dongle
```

디스플레이 부분을 위한 사용자 지정 실드를 만들려면 다음과 같이 진행합니다.

1. `dongle_display` 모듈은 디스플레이 위젯을 포함하는 모듈이며, west 또는 로컬 설정을 통해 프로젝트 의존성에 포함됩니다.
2. 디스플레이 하드웨어용 사용자 지정 실드가 필요하면 적절한 디스플레이 설정을 제공하는 새 실드를 만들 수 있습니다. 여기서는 `cornix_dongle_eyelash`를 예제로 보여줍니다.
3. 보드의 장치 트리에 이미 `zephyr,display`가 있으면 `cornix_dongle_eyelash` 실드를 생략할 수 있습니다.
4. 빌드 설정에 사용자 지정 실드를 포함합니다.

사용자 지정 동글 화면을 사용하려면 `build.yaml`에 사용자 지정 동글용 새 타깃을 추가합니다.

```yaml
- board: nice_nano
  shield: cornix_dongle_adapter cornix_dongle_eyelash dongle_display
  snippet: studio-rpc-usb-uart zmk-usb-logging
  artifact-name: cornix_dongle
```

디스플레이용 사용자 지정 실드를 만들려면 다음과 같이 진행합니다.

1. 키 매트릭스 및 Bluetooth 기능의 기본 실드로 `cornix_dongle_adapter`를 사용합니다.
2. 적절한 보드와 설정을 사용하여 `build.yaml` 파일에 사용자 지정 실드를 추가합니다.
3. `cornix_dongle_eyelash`를 예제로 삼아 사용자 지정 보드에 맞게 디스플레이 부분을 수정합니다.
4. `cornix_dongle_eyelash`를 프로젝트의 `boards/shield/` 디렉터리에 복사한 뒤 같은 이름을 사용하거나 새 실드 이름으로 변경할 수 있습니다.

`west.yml` 파일의 설정은 그대로 유지합니다.

```yaml
remotes:
  - name: zmkfirmware
    url-base: https://github.com/zmkfirmware
  - name: cornix-shield
    url-base: https://github.com/hitsmaxft
  - name: urob
    url-base: https://github.com/urob
```
```yaml
projects:
  - name: zmk
    remote: zmkfirmware
    revision: main
    import: app/west.yml
  - name: zmk-keyboard-cornix
    remote: cornix-shield
    revision: main
  - name: zmk-helpers
    remote: urob
    revision: main
```

## 이 프로젝트를 로컬에서 빌드하기(west.yaml 의존성 없이)

이 프로젝트를 `west.yaml`의 의존성으로 추가하지 않고 로컬에서 빌드하려면 `ZMK_EXTRA_MODULES` CMake 인수를 사용할 수 있습니다.

### 사전 준비 사항

1. 정상적으로 작동하는 ZMK 개발 환경을 설정합니다.
2. 이 저장소를 로컬 디렉터리에 복제합니다.

### 빌드 단계

1. **이 저장소를 복제**합니다.
   ```bash
   git clone https://github.com/hitsmaxft/zmk-keyboard-cornix.git
   ```

2. **추가 모듈을 사용하도록 ZMK 빌드를 설정**합니다.

   `.west/config` 파일을 편집하고 `[build]` 섹션 아래에 CMake 인수를 추가합니다.

   ```ini
   [build]
   cmake-args = -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DZMK_EXTRA_MODULES=/full/absolute/path/to/zmk-keyboard-cornix
   ```

   `/full/absolute/path/to/zmk-keyboard-cornix`를 이 저장소를 복제한 실제 절대 경로로 바꾸세요.

3. **펌웨어를 빌드**합니다.
   ```bash
   west build -b cornix_left//zmk
   west build -b cornix_right//zmk
   ```

이 방법을 사용하면 기존 ZMK 설정의 `west.yaml` 파일을 수정하지 않고도 Cornix 실드를 사용할 수 있습니다.
