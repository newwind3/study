# TypeC 드라이버 아키텍처 분석

## 개요

MediaTek MT83xx TypeC 드라이버는 크게 **TCPC (Type-C Port Controller)** 서브시스템과 **MUX (Multiplexer)** 서브시스템으로 구성되어 있습니다.

- **위치**: `MT83xx_T/kernel-5.15/drivers/misc/mediatek/typec/`
- **총 파일 수**: 81개
- **주요 서브디렉토리**: `tcpc/`, `mux/`

---

## 전체 아키텍처

```mermaid
graph TB
    subgraph "Application Layer"
        A[User Space Applications]
    end
    
    subgraph "Platform Integration"
        B[rt_pd_manager.c]
        C[typec framework]
    end
    
    subgraph "TCPC Subsystem"
        D[tcpci_core.c<br/>Device Registration]
        E[tcpci_typec.c<br/>Type-C State Machine]
        F[pd_core.c<br/>PD Protocol]
        G[pd_policy_engine.c<br/>Policy Engine]
        H[pd_dpm_core.c<br/>Device Policy Manager]
    end
    
    subgraph "TCPC Chip Drivers"
        I[tcpc_mt6360.c]
        J[tcpc_mt6370.c]
        K[tcpc_rt1711h.c]
        L[Other Chip Drivers]
    end
    
    subgraph "MUX Subsystem"
        M[mux_switch.c<br/>MUX/Switch Manager]
    end
    
    subgraph "MUX Chip Drivers"
        N[hd3ss460.c]
        O[ps5169.c]
        P[pi3dpx1205a.c]
        Q[Other MUX Drivers]
    end
    
    subgraph "Hardware"
        R[TCPC Chip]
        S[MUX/Switch Chip]
        T[USB-C Connector]
    end
    
    A --> B
    B --> C
    B --> D
    C --> B
    D --> E
    D --> F
    E --> D
    F --> G
    F --> H
    G --> F
    H --> F
    D --> I
    D --> J
    D --> K
    D --> L
    I --> R
    J --> R
    K --> R
    L --> R
    R --> T
    
    B --> M
    M --> N
    M --> O
    M --> P
    M --> Q
    N --> S
    O --> S
    P --> S
    Q --> S
    S --> T
```

---

## 1. TCPC 서브시스템

### 1.1 핵심 파일 구조

#### **tcpci_core.c** - TCPC 디바이스 코어
- **역할**: TCPC 디바이스 등록, 관리, 이벤트 처리
- **주요 함수**:
  - `tcpc_device_register()`: TCPC 디바이스 등록
  - `tcpc_device_unregister()`: TCPC 디바이스 해제
  - `register_tcp_dev_notifier()`: 알림 등록
  - `tcpc_event_init_work()`: 초기화 작업
- **연계 파일**:
  - → `tcpci_typec.c`: Type-C 상태 머신 호출
  - → `pd_core.c`: PD 프로토콜 초기화
  - → `rt_pd_manager.c`: 플랫폼 통합
  - ← 칩 드라이버들: MT6360, MT6370, RT1711H 등

#### **tcpci_typec.c** - Type-C 상태 머신
- **역할**: Type-C 연결 상태 관리 및 CC 라인 처리
- **주요 함수**:
  - `tcpc_typec_init()`: Type-C 초기화
  - `typec_transfer_state()`: 상태 전환
  - `typec_alert_attach_state_change()`: 연결 상태 변경 처리
  - `typec_enable_vconn()`: VCONN 제어
- **상태 머신**:
  - Unattached (SNK/SRC)
  - AttachWait (SNK/SRC)
  - Attached (SNK/SRC)
  - Try.SRC / Try.SNK
  - Debug Accessory
- **연계 파일**:
  - ← `tcpci_core.c`: 코어에서 호출
  - → `tcpci.c`: 하드웨어 제어 함수 호출
  - → `pd_core.c`: PD 연결 시 협력

#### **pd_core.c** - PD 프로토콜 코어
- **역할**: USB Power Delivery 프로토콜 처리
- **주요 함수**:
  - `pd_core_init()`: PD 초기화
  - `pd_parse_pdata()`: DTS에서 PD 설정 파싱
  - `pd_reset_protocol_layer()`: 프로토콜 레이어 리셋
  - `pd_set_rx_enable()`: 수신 활성화
  - `pd_extract_rdo_power()`: RDO 전력 추출
- **연계 파일**:
  - ← `tcpci_core.c`: 코어에서 초기화
  - → `pd_policy_engine.c`: 정책 엔진 호출
  - → `pd_dpm_core.c`: DPM 호출
  - → `pd_process_evt_*.c`: 이벤트 처리

#### **pd_policy_engine.c** - PD 정책 엔진
- **역할**: PD 프로토콜 상태 머신 및 정책 관리
- **주요 기능**:
  - Source/Sink 협상
  - Power Role Swap
  - Data Role Swap
  - VCONN Swap
  - Alternate Mode 진입
- **연계 파일**:
  - ← `pd_core.c`: 코어에서 호출
  - → `pd_policy_engine_src.c`: Source 정책
  - → `pd_policy_engine_snk.c`: Sink 정책
  - → `pd_policy_engine_dfp.c`: DFP 정책
  - → `pd_policy_engine_ufp.c`: UFP 정책

#### **pd_dpm_core.c** - Device Policy Manager
- **역할**: 디바이스 정책 관리 및 VDM 처리
- **주요 기능**:
  - Discover Identity/SVID/Modes
  - Alternate Mode 관리
  - UVDM (Unstructured VDM) 처리
- **연계 파일**:
  - ← `pd_core.c`: 코어에서 호출
  - → `pd_dpm_alt_mode_dp.c`: DisplayPort Alt Mode
  - → `pd_dpm_uvdm.c`: UVDM 처리
  - → `pd_dpm_pdo_select.c`: PDO 선택

#### **rt_pd_manager.c** - 플랫폼 통합 매니저
- **역할**: TCPC와 플랫폼 서비스 통합
- **주요 함수**:
  - `pd_tcp_notifier_call()`: TCPC 이벤트 처리
  - `typec_init()`: typec framework 초기화
  - `usb_dpdm_pulldown()`: USB DP/DM 제어
- **통합 기능**:
  - typec framework 연동
  - Charger 연동
  - USB role switch
  - Water detection
- **연계 파일**:
  - ← `tcpci_core.c`: 알림 수신
  - → Linux typec framework
  - → Charger driver
  - → USB driver

### 1.2 칩별 드라이버

각 TCPC 칩은 `tcpc_ops` 구조체를 구현하여 하드웨어 추상화 제공:

- **tcpc_mt6360.c**: MT6360 TCPC 칩
- **tcpc_mt6370.c**: MT6370 TCPC 칩
- **tcpc_mt6375.c**: MT6375 TCPC 칩
- **tcpc_rt1711h.c**: RT1711H TCPC 칩

**공통 인터페이스** (`tcpc_ops`):
- `init()`: 칩 초기화
- `get_cc()`: CC 상태 읽기
- `set_cc()`: CC 설정
- `set_polarity()`: 극성 설정
- `set_vconn()`: VCONN 제어
- `transmit()`: PD 메시지 전송
- `get_message()`: PD 메시지 수신

### 1.3 보조 모듈

- **tcpci_timer.c**: 타이머 관리
- **tcpci_event.c**: 이벤트 큐 관리
- **tcpci_alert.c**: 인터럽트 처리
- **pd_dbg_info.c**: 디버그 정보 출력
- **tcpci_late_sync.c**: 지연 동기화

---

## 2. MUX 서브시스템

### 2.1 핵심 파일

#### **mux_switch.c** - MUX/Switch 매니저
- **역할**: USB-C MUX 및 Switch 관리
- **주요 함수**:
  - `mtk_typec_mux_register()`: MUX 등록
  - `mtk_typec_switch_register()`: Switch 등록
  - `mtk_typec_mux_set()`: MUX 상태 설정
  - `mtk_typec_switch_set()`: Switch 방향 설정
- **기능**:
  - USB/DP 신호 라우팅
  - 극성 전환
  - DisplayPort Alt Mode 지원
- **연계 파일**:
  - ← `rt_pd_manager.c`: PD 이벤트에 따라 MUX 제어
  - → 칩별 MUX 드라이버들

### 2.2 MUX 칩 드라이버

각 MUX 칩은 `typec_mux_desc` 및 `typec_switch_desc` 구현:

- **hd3ss460.c**: TI HD3SS460 (USB3.1 + DP)
- **ps5169.c**: Parade PS5169 (USB3.1 + DP ReDriver)
- **ps5170.c**: Parade PS5170
- **pi3dpx1205a.c**: Pericom PI3DPX1205A
- **fusb304.c**: ON Semi FUSB304
- **it5205fn.c**: ITE IT5205
- **ptn36241g.c**: NXP PTN36241G
- **usb_dp_selector.c**: USB/DP 선택기

**공통 기능**:
- USB 신호 라우팅
- DisplayPort 신호 라우팅
- 극성 제어
- 전력 관리

---

## 3. 파일 간 연계성

### 3.1 초기화 플로우

```mermaid
sequenceDiagram
    participant Kernel
    participant ChipDriver as TCPC Chip Driver
    participant Core as tcpci_core.c
    participant TypeC as tcpci_typec.c
    participant PD as pd_core.c
    participant Manager as rt_pd_manager.c
    participant MUX as mux_switch.c
    
    Kernel->>ChipDriver: probe()
    ChipDriver->>Core: tcpc_device_register()
    Core->>Core: 디바이스 생성 및 초기화
    Core->>TypeC: tcpc_typec_init()
    TypeC->>TypeC: 상태 머신 초기화
    Core->>PD: pd_core_init()
    PD->>PD: PD 프로토콜 초기화
    Core->>Manager: 알림 등록
    Manager->>Core: register_tcp_dev_notifier()
    Manager->>MUX: MUX/Switch 초기화
    Core->>ChipDriver: ops->init()
    ChipDriver-->>Core: 완료
    Core-->>Kernel: 등록 완료
```

### 3.2 연결 이벤트 플로우

```mermaid
sequenceDiagram
    participant HW as Hardware
    participant ChipDriver as TCPC Chip Driver
    participant Core as tcpci_core.c
    participant TypeC as tcpci_typec.c
    participant PD as pd_core.c
    participant Manager as rt_pd_manager.c
    participant MUX as mux_switch.c
    
    HW->>ChipDriver: IRQ (CC 변경)
    ChipDriver->>Core: tcpci_alert()
    Core->>TypeC: tcpc_typec_handle_cc_change()
    TypeC->>TypeC: 상태 전환 (Attached.SNK)
    TypeC->>Core: 알림 전송
    Core->>Manager: TCP_NOTIFY_TYPEC_STATE
    Manager->>Manager: typec_set_data_role()
    Manager->>MUX: mtk_typec_switch_set()
    MUX->>MUX: 극성 설정
    
    opt PD 지원 시
        TypeC->>PD: PD 연결 시작
        PD->>PD: Source Capabilities 수신
        PD->>Manager: TCP_NOTIFY_PD_STATE
        Manager->>Manager: 전력 협상 처리
    end
```

### 3.3 DisplayPort Alt Mode 플로우

```mermaid
sequenceDiagram
    participant PD as pd_core.c
    participant DPM as pd_dpm_core.c
    participant DPDPM as pd_dpm_alt_mode_dp.c
    participant Manager as rt_pd_manager.c
    participant MUX as mux_switch.c
    participant DPDriver as DP Driver
    
    PD->>DPM: Discover SVID
    DPM->>DPM: DisplayPort SVID 발견
    DPM->>DPDPM: Discover Modes
    DPDPM->>DPDPM: DP 모드 확인
    DPDPM->>PD: Enter Mode
    PD->>Manager: TCP_NOTIFY_ENTER_MODE
    Manager->>MUX: mtk_typec_mux_set(DP)
    MUX->>MUX: DP 신호 라우팅
    DPDPM->>Manager: TCP_NOTIFY_AMA_DP_STATE
    Manager->>DPDriver: mtk_dp_SWInterruptSet()
    DPDriver->>DPDriver: DP 출력 활성화
```

---

## 4. 주요 데이터 구조

### 4.1 TCPC 디바이스 구조체

```c
struct tcpc_device {
    struct tcpc_desc desc;          // 디바이스 설명
    struct tcpc_ops *ops;            // 하드웨어 ops
    void *drv_data;                  // 드라이버 데이터
    
    // Type-C 상태
    uint8_t typec_state;
    uint8_t typec_role;
    uint8_t typec_attach_old;
    uint8_t typec_attach_new;
    
    // PD 상태
    struct pd_port *pd_port;
    
    // 이벤트 관리
    struct tcpc_timer tcpc_timer[PD_TIMER_NR];
    struct pd_event pd_event_ring[PD_EVENT_BUF_SIZE];
    
    // 알림
    struct srcu_notifier_head evt_nh[TCP_NOTIFY_IDX_NR];
};
```

### 4.2 PD Port 구조체

```c
struct pd_port {
    struct tcpc_device *tcpc;
    struct pe_data pe_data;          // Policy Engine 데이터
    struct dpm_data dpm_data;        // DPM 데이터
    
    // PDO 정보
    uint32_t local_src_cap[PDO_MAX_NR];
    uint32_t local_snk_cap[PDO_MAX_NR];
    uint32_t remote_src_cap[PDO_MAX_NR];
    uint32_t remote_snk_cap[PDO_MAX_NR];
    
    // VDM 정보
    struct svdm_svid_data svid_data[SVID_DISCOVERY_MAX];
};
```

---

## 5. 빌드 구성

### 5.1 Kconfig 옵션

**TCPC 관련**:
- `CONFIG_TCPC_CLASS`: TCPC 클래스 드라이버
- `CONFIG_USB_POWER_DELIVERY`: PD 프로토콜 지원
- `CONFIG_TCPC_MT6360/MT6370/RT1711H`: 칩별 드라이버

**MUX 관련**:
- `CONFIG_MTK_USB_TYPEC_MUX`: MUX 서브시스템
- `CONFIG_TYPEC_MUX_HD3SS460/PS5169/...`: 칩별 MUX 드라이버

### 5.2 Makefile 구조

```makefile
# 메인 Makefile
obj-$(CONFIG_TCPC_CLASS) += tcpc/
obj-$(CONFIG_MTK_USB_TYPEC_MUX) += mux/

# tcpc/Makefile
tcpc_class-objs := tcpci_core.o tcpci_typec.o tcpci_timer.o ...
tcpc_class-objs += pd_core.o pd_policy_engine.o pd_dpm_core.o ...

# mux/Makefile
obj-$(CONFIG_MTK_USB_TYPEC_MUX) += mux_switch.o
obj-$(CONFIG_TYPEC_MUX_HD3SS460) += hd3ss460.o
```

---

## 6. 주요 기능별 파일 매핑

### 6.1 Type-C 연결 감지
- **tcpci_typec.c**: 상태 머신
- **tcpci_alert.c**: 인터럽트 처리
- **칩 드라이버**: CC 라인 읽기

### 6.2 PD 협상
- **pd_core.c**: 프로토콜 처리
- **pd_policy_engine.c**: 정책 관리
- **pd_policy_engine_src.c/snk.c**: Source/Sink 정책
- **pd_dpm_pdo_select.c**: PDO 선택

### 6.3 Alternate Mode
- **pd_dpm_core.c**: VDM 관리
- **pd_dpm_alt_mode_dp.c**: DisplayPort
- **pd_dpm_alt_mode_dc.c**: Direct Charge

### 6.4 Role Swap
- **pd_policy_engine_prs.c**: Power Role Swap
- **pd_policy_engine_drs.c**: Data Role Swap
- **pd_process_evt_prs.c/drs.c**: 이벤트 처리

### 6.5 USB/DP 라우팅
- **mux_switch.c**: MUX 관리
- **칩별 MUX 드라이버**: 하드웨어 제어
- **rt_pd_manager.c**: PD와 MUX 연동

---

## 7. 디버깅 및 모니터링

### 7.1 디버그 파일
- **pd_dbg_info.c**: PD 디버그 정보
- **sysfs 인터페이스**: `/sys/class/type-c/`
- **procfs 인터페이스**: `/proc/typec_mux`, `/proc/typec_switch`

### 7.2 로그 레벨
- `TCPC_INFO_ENABLE`: TCPC 정보
- `PE_INFO_ENABLE`: Policy Engine 정보
- `DPM_DBG_ENABLE`: DPM 디버그
- `DP_INFO_ENABLE`: DisplayPort 정보

---

## 8. 요약

### 핵심 파일 역할

| 파일 | 역할 | 주요 연계 |
|------|------|-----------|
| `tcpci_core.c` | TCPC 디바이스 관리 | 모든 TCPC 모듈의 중심 |
| `tcpci_typec.c` | Type-C 상태 머신 | CC 감지 및 상태 전환 |
| `pd_core.c` | PD 프로토콜 코어 | PD 메시지 처리 |
| `pd_policy_engine.c` | PD 정책 엔진 | 협상 및 상태 관리 |
| `pd_dpm_core.c` | DPM | VDM 및 Alt Mode |
| `rt_pd_manager.c` | 플랫폼 통합 | typec/charger/USB 연동 |
| `mux_switch.c` | MUX 관리 | USB/DP 라우팅 |

### 데이터 흐름

1. **하드웨어 이벤트** → 칩 드라이버 → `tcpci_core.c`
2. **Type-C 처리** → `tcpci_typec.c` → 상태 전환
3. **PD 처리** → `pd_core.c` → `pd_policy_engine.c` → `pd_dpm_core.c`
4. **플랫폼 통합** → `rt_pd_manager.c` → typec framework/charger/USB
5. **MUX 제어** → `mux_switch.c` → 칩별 MUX 드라이버
