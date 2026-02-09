# TypeC 드라이버 상세 플로우 차트

## 1. 파일 간 의존성 다이어그램

### 1.1 TCPC 서브시스템 파일 의존성

```mermaid
graph LR
    subgraph "Header Files"
        H1[tcpci_core.h]
        H2[tcpci_typec.h]
        H3[pd_core.h]
        H4[tcpm.h]
        H5[tcpci.h]
    end
    
    subgraph "Core Files"
        C1[tcpci_core.c]
        C2[tcpci_typec.c]
        C3[pd_core.c]
        C4[tcpm.c]
        C5[tcpci.c]
    end
    
    subgraph "PD Policy Engine"
        P1[pd_policy_engine.c]
        P2[pd_policy_engine_src.c]
        P3[pd_policy_engine_snk.c]
        P4[pd_policy_engine_dfp.c]
        P5[pd_policy_engine_ufp.c]
    end
    
    subgraph "PD Event Processing"
        E1[pd_process_evt.c]
        E2[pd_process_evt_src.c]
        E3[pd_process_evt_snk.c]
        E4[pd_process_evt_vdm.c]
    end
    
    subgraph "DPM"
        D1[pd_dpm_core.c]
        D2[pd_dpm_alt_mode_dp.c]
        D3[pd_dpm_uvdm.c]
        D4[pd_dpm_pdo_select.c]
    end
    
    H1 --> C1
    H2 --> C2
    H3 --> C3
    H4 --> C4
    H5 --> C5
    
    C1 --> C2
    C1 --> C3
    C2 --> C5
    C3 --> P1
    P1 --> P2
    P1 --> P3
    P1 --> P4
    P1 --> P5
    C3 --> E1
    E1 --> E2
    E1 --> E3
    E1 --> E4
    C3 --> D1
    D1 --> D2
    D1 --> D3
    D1 --> D4
```

### 1.2 MUX 서브시스템 파일 의존성

```mermaid
graph TB
    subgraph "MUX Core"
        M1[mux_switch.c]
        M2[mux_switch.h]
    end
    
    subgraph "MUX Chip Drivers"
        D1[hd3ss460.c]
        D2[ps5169.c]
        D3[ps5170.c]
        D4[pi3dpx1205a.c]
        D5[fusb304.c]
        D6[it5205fn.c]
        D7[ptn36241g.c]
        D8[usb_dp_selector.c]
    end
    
    M2 --> M1
    M1 --> D1
    M1 --> D2
    M1 --> D3
    M1 --> D4
    M1 --> D5
    M1 --> D6
    M1 --> D7
    M1 --> D8
```

---

## 2. 주요 기능별 호출 플로우

### 2.1 TCPC 디바이스 등록 플로우

```mermaid
sequenceDiagram
    participant I2C as I2C Driver
    participant Chip as Chip Driver<br/>(mt6360/mt6370/rt1711h)
    participant Core as tcpci_core.c
    participant TypeC as tcpci_typec.c
    participant PD as pd_core.c
    participant Timer as tcpci_timer.c
    participant Event as tcpci_event.c
    
    I2C->>Chip: probe()
    Chip->>Chip: 칩 초기화
    Chip->>Chip: tcpc_desc 설정
    Chip->>Chip: tcpc_ops 설정
    Chip->>Core: tcpc_device_register(desc, ops, drv_data)
    
    Core->>Core: tcpc_device 할당
    Core->>Core: device_initialize()
    Core->>Core: sysfs 속성 등록
    Core->>Timer: 타이머 초기화
    Core->>Event: 이벤트 큐 초기화
    Core->>Core: workqueue 생성
    Core->>Core: notifier chain 초기화
    
    Core->>TypeC: tcpc_typec_init(tcpc, role)
    TypeC->>TypeC: 상태 머신 초기화
    TypeC->>TypeC: Unattached 상태로 설정
    TypeC-->>Core: 완료
    
    Core->>PD: pd_core_init(tcpc)
    PD->>PD: pd_port 할당
    PD->>PD: DTS 파싱 (PDO, VDM 등)
    PD->>PD: Policy Engine 초기화
    PD->>PD: DPM 초기화
    PD-->>Core: 완료
    
    Core->>Core: device_add()
    Core->>Core: schedule_work(init_work)
    Core-->>Chip: tcpc_device 반환
    
    Note over Core: init_work 실행
    Core->>Chip: ops->init(tcpc, sw_reset)
    Chip->>Chip: 레지스터 초기화
    Chip->>Chip: Alert Mask 설정
    Chip-->>Core: 완료
    
    Core->>Chip: ops->init_alert_mask(tcpc)
    Chip-->>Core: 완료
    
    Core->>TypeC: 초기 CC 감지 시작
    TypeC->>Chip: ops->get_cc(tcpc)
    Chip-->>TypeC: CC 상태
```

### 2.2 Type-C 연결 감지 플로우

```mermaid
sequenceDiagram
    participant HW as Hardware
    participant IRQ as IRQ Handler
    participant Chip as Chip Driver
    participant Alert as tcpci_alert.c
    participant Core as tcpci_core.c
    participant TypeC as tcpci_typec.c
    participant Timer as tcpci_timer.c
    participant Notify as Notifier
    
    HW->>IRQ: CC 변경 감지
    IRQ->>Chip: IRQ 핸들러
    Chip->>Alert: tcpci_alert(tcpc)
    
    Alert->>Chip: ops->get_alert_status(tcpc, &alert)
    Chip-->>Alert: alert = CC_STATUS
    
    Alert->>Alert: alert & ALERT_CC_STATUS?
    Alert->>TypeC: tcpc_typec_handle_cc_change(tcpc)
    
    TypeC->>Chip: ops->get_cc(tcpc)
    Chip-->>TypeC: cc1, cc2 상태
    
    TypeC->>TypeC: CC 상태 분석
    
    alt Sink 연결 감지
        TypeC->>TypeC: 현재 상태 = Unattached.SNK
        TypeC->>TypeC: CC = Rp (Source 연결됨)
        TypeC->>Timer: tcpc_enable_timer(AttachWait.SNK)
        Timer-->>TypeC: 타이머 시작
        
        Note over Timer: tCCDebounce 대기
        Timer->>TypeC: 타이머 만료
        TypeC->>TypeC: 상태 전환: Attached.SNK
        TypeC->>TypeC: Rp 레벨 확인 (전류 능력)
        TypeC->>Chip: ops->set_polarity(tcpc, polarity)
        TypeC->>Chip: ops->set_vconn(tcpc, enable)
        
        TypeC->>Core: tcpci_notify_typec_state(tcpc)
        Core->>Notify: srcu_notifier_call_chain(TYPEC_STATE)
        Notify->>Notify: rt_pd_manager 알림
        
    else Source 연결 감지
        TypeC->>TypeC: 현재 상태 = Unattached.SRC
        TypeC->>TypeC: CC = Rd (Sink 연결됨)
        TypeC->>Timer: tcpc_enable_timer(AttachWait.SRC)
        Timer-->>TypeC: 타이머 시작
        
        Note over Timer: tCCDebounce 대기
        Timer->>TypeC: 타이머 만료
        TypeC->>TypeC: 상태 전환: Attached.SRC
        TypeC->>Chip: ops->set_polarity(tcpc, polarity)
        TypeC->>Chip: ops->set_vconn(tcpc, enable)
        TypeC->>Chip: ops->source_vbus(tcpc, type, mv, ma)
        
        TypeC->>Core: tcpci_notify_typec_state(tcpc)
        Core->>Notify: srcu_notifier_call_chain(TYPEC_STATE)
    end
```

### 2.3 PD 협상 플로우 (Sink 관점)

```mermaid
sequenceDiagram
    participant TypeC as tcpci_typec.c
    participant PD as pd_core.c
    participant PE as pd_policy_engine.c
    participant PE_SNK as pd_policy_engine_snk.c
    participant DPM as pd_dpm_core.c
    participant PDO as pd_dpm_pdo_select.c
    participant Chip as Chip Driver
    participant Notify as Notifier
    
    TypeC->>PD: Attached.SNK 상태
    PD->>PD: pd_set_rx_enable(ENABLE)
    PD->>Chip: ops->set_rx_enable(tcpc, ENABLE)
    
    Note over Chip: Source Capabilities 수신
    Chip->>PD: PD 메시지 수신 IRQ
    PD->>Chip: ops->get_message(tcpc, &payload, &header, &type)
    Chip-->>PD: Source_Capabilities
    
    PD->>PE: pd_handle_msg(SOURCE_CAPABILITIES)
    PE->>PE_SNK: PE_SNK_Evaluate_Capability
    
    PE_SNK->>DPM: dpm_evaluate_src_cap(tcpc, src_cap, src_cap_nr)
    DPM->>PDO: pd_dpm_select_pdo(tcpc, src_cap, src_cap_nr)
    
    PDO->>PDO: Local Sink Cap과 비교
    PDO->>PDO: 최적 PDO 선택
    PDO->>PDO: RDO 생성
    PDO-->>DPM: selected_pdo, rdo
    DPM-->>PE_SNK: 선택 완료
    
    PE_SNK->>PE: PE_SNK_Select_Capability
    PE->>PD: pd_send_data_msg(REQUEST, rdo)
    PD->>Chip: ops->transmit(tcpc, SOP, header, &rdo)
    Chip->>Chip: PD 메시지 전송
    
    Note over Chip: Accept 수신
    Chip->>PD: PD 메시지 수신 IRQ
    PD->>Chip: ops->get_message(tcpc, &payload, &header, &type)
    Chip-->>PD: Accept
    
    PD->>PE: pd_handle_msg(ACCEPT)
    PE->>PE_SNK: PE_SNK_Transition_Sink
    
    Note over Chip: PS_RDY 수신
    Chip->>PD: PD 메시지 수신 IRQ
    PD->>PE: pd_handle_msg(PS_RDY)
    PE->>PE_SNK: PE_SNK_Ready
    
    PE_SNK->>Notify: TCP_NOTIFY_PD_STATE(PD_CONNECT_PE_READY_SNK)
    Notify->>Notify: rt_pd_manager 알림
    Notify->>Notify: Charger에 전력 정보 전달
```

### 2.4 DisplayPort Alt Mode 진입 플로우

```mermaid
sequenceDiagram
    participant PE as pd_policy_engine.c
    participant DPM as pd_dpm_core.c
    participant DP_DPM as pd_dpm_alt_mode_dp.c
    participant VDM as pd_process_evt_vdm.c
    participant PD as pd_core.c
    participant Chip as Chip Driver
    participant Manager as rt_pd_manager.c
    participant MUX as mux_switch.c
    participant DP as DP Driver
    
    Note over PE: PE_Ready 상태
    PE->>DPM: dpm_discover_svids(tcpc)
    DPM->>PD: pd_send_vdm_discover_svids(tcpc, SOP)
    PD->>Chip: ops->transmit(VDM, Discover_SVID)
    
    Note over Chip: Discover_SVID ACK 수신
    Chip->>PD: VDM 수신
    PD->>VDM: pd_process_vdm(tcpc, vdm_hdr, vdos)
    VDM->>DPM: dpm_vdm_discover_svids_ack(tcpc, svids)
    
    DPM->>DPM: SVID 리스트에 저장
    DPM->>DPM: DisplayPort SVID (0xFF01) 발견
    
    DPM->>DP_DPM: dp_dfp_u_discover_modes(tcpc)
    DP_DPM->>PD: pd_send_vdm_discover_modes(tcpc, SOP, DP_SVID)
    PD->>Chip: ops->transmit(VDM, Discover_Modes)
    
    Note over Chip: Discover_Modes ACK 수신
    Chip->>PD: VDM 수신
    PD->>VDM: pd_process_vdm(tcpc, vdm_hdr, vdos)
    VDM->>DP_DPM: dp_dfp_u_discover_modes_ack(tcpc, modes)
    
    DP_DPM->>DP_DPM: DP 모드 분석
    DP_DPM->>DP_DPM: Pin Assignment 확인
    
    DP_DPM->>PD: pd_send_vdm_enter_mode(tcpc, SOP, DP_SVID, mode)
    PD->>Chip: ops->transmit(VDM, Enter_Mode)
    
    Note over Chip: Enter_Mode ACK 수신
    Chip->>PD: VDM 수신
    PD->>VDM: pd_process_vdm(tcpc, vdm_hdr, vdos)
    VDM->>DP_DPM: dp_dfp_u_enter_mode_ack(tcpc)
    
    DP_DPM->>Manager: TCP_NOTIFY_ENTER_MODE
    Manager->>Manager: DP Alt Mode 진입 확인
    
    DP_DPM->>PD: pd_send_vdm_dp_status(tcpc, SOP)
    PD->>Chip: ops->transmit(VDM, DP_Status_Update)
    
    Note over Chip: DP_Status_Update ACK 수신
    Chip->>PD: VDM 수신
    PD->>VDM: pd_process_vdm(tcpc, vdm_hdr, vdos)
    VDM->>DP_DPM: dp_dfp_u_status_update_ack(tcpc, dp_status)
    
    DP_DPM->>DP_DPM: DP 설정 결정 (pin assignment)
    DP_DPM->>PD: pd_send_vdm_dp_config(tcpc, SOP, dp_config)
    PD->>Chip: ops->transmit(VDM, DP_Configure)
    
    Note over Chip: DP_Configure ACK 수신
    Chip->>PD: VDM 수신
    PD->>VDM: pd_process_vdm(tcpc, vdm_hdr, vdos)
    VDM->>DP_DPM: dp_dfp_u_configure_ack(tcpc)
    
    DP_DPM->>Manager: TCP_NOTIFY_AMA_DP_STATE
    Manager->>Manager: DP 설정 정보 추출
    Manager->>MUX: mtk_typec_mux_set(mux, DP_MODE)
    MUX->>MUX: MUX 칩 드라이버 호출
    MUX->>MUX: USB/DP 신호 라우팅
    
    Manager->>DP: mtk_dp_SWInterruptSet(1)
    Manager->>DP: mtk_dp_set_pin_assign(pin)
    DP->>DP: DP 출력 활성화
```

---

## 3. Type-C 상태 머신 다이어그램

### 3.1 Sink 상태 머신

```mermaid
stateDiagram-v2
    [*] --> Disabled
    Disabled --> Unattached.SNK: 초기화
    
    Unattached.SNK --> AttachWait.SNK: Rp 감지
    AttachWait.SNK --> Unattached.SNK: Rp 사라짐
    AttachWait.SNK --> Attached.SNK: tCCDebounce 만료
    
    Attached.SNK --> Unattached.SNK: VBUS 제거 또는 CC Open
    
    state Attached.SNK {
        [*] --> Wait_PD
        Wait_PD --> PD_Connected: PD 협상 성공
        PD_Connected --> [*]: Hard Reset
    }
    
    Attached.SNK --> TryWait.SRC: Try.SRC 시도 (DRP)
    TryWait.SRC --> Attached.SRC: Rd 감지
    TryWait.SRC --> Attached.SNK: 타임아웃
```

### 3.2 Source 상태 머신

```mermaid
stateDiagram-v2
    [*] --> Disabled
    Disabled --> Unattached.SRC: 초기화
    
    Unattached.SRC --> AttachWait.SRC: Rd 감지
    AttachWait.SRC --> Unattached.SRC: Rd 사라짐
    AttachWait.SRC --> Attached.SRC: tCCDebounce 만료
    
    Attached.SRC --> Unattached.SRC: CC Open
    
    state Attached.SRC {
        [*] --> VBUS_On
        VBUS_On --> Wait_PD
        Wait_PD --> PD_Connected: PD 협상 성공
        PD_Connected --> [*]: Hard Reset
    }
    
    Attached.SRC --> TryWait.SNK: Try.SNK 시도 (DRP)
    TryWait.SNK --> Attached.SNK: Rp 감지
    TryWait.SNK --> Attached.SRC: 타임아웃
```

### 3.3 DRP (Dual Role Port) 상태 머신

```mermaid
stateDiagram-v2
    [*] --> Unattached.SNK
    
    Unattached.SNK --> Unattached.SRC: DRP Toggle
    Unattached.SRC --> Unattached.SNK: DRP Toggle
    
    Unattached.SNK --> AttachWait.SNK: Rp 감지
    Unattached.SRC --> AttachWait.SRC: Rd 감지
    
    AttachWait.SNK --> Attached.SNK: tCCDebounce
    AttachWait.SRC --> Attached.SRC: tCCDebounce
    
    Attached.SNK --> Unattached.SNK: 연결 해제
    Attached.SRC --> Unattached.SRC: 연결 해제
```

---

## 4. PD Policy Engine 상태 머신

### 4.1 Sink Policy Engine 주요 상태

```mermaid
stateDiagram-v2
    [*] --> PE_SNK_Startup
    
    PE_SNK_Startup --> PE_SNK_Discovery
    PE_SNK_Discovery --> PE_SNK_Wait_for_Capabilities: VBUS 감지
    
    PE_SNK_Wait_for_Capabilities --> PE_SNK_Evaluate_Capability: Source_Cap 수신
    PE_SNK_Wait_for_Capabilities --> PE_SNK_Hard_Reset: 타임아웃
    
    PE_SNK_Evaluate_Capability --> PE_SNK_Select_Capability: PDO 선택
    PE_SNK_Select_Capability --> PE_SNK_Transition_Sink: Accept 수신
    PE_SNK_Select_Capability --> PE_SNK_Wait_for_Capabilities: Reject 수신
    
    PE_SNK_Transition_Sink --> PE_SNK_Ready: PS_RDY 수신
    
    state PE_SNK_Ready {
        [*] --> Idle
        Idle --> Get_Sink_Cap: Get_Sink_Cap 수신
        Idle --> DR_Swap: DR_Swap 요청/수신
        Idle --> PR_Swap: PR_Swap 요청/수신
        Idle --> VCONN_Swap: VCONN_Swap 요청/수신
        Idle --> VDM: VDM 처리
    }
    
    PE_SNK_Ready --> PE_SNK_Hard_Reset: Hard Reset 요청
    PE_SNK_Hard_Reset --> PE_SNK_Transition_to_default
    PE_SNK_Transition_to_default --> PE_SNK_Startup
```

### 4.2 Source Policy Engine 주요 상태

```mermaid
stateDiagram-v2
    [*] --> PE_SRC_Startup
    
    PE_SRC_Startup --> PE_SRC_Send_Capabilities
    PE_SRC_Send_Capabilities --> PE_SRC_Negotiate_Capability: Request 수신
    PE_SRC_Send_Capabilities --> PE_SRC_Send_Capabilities: GoodCRC 수신, 재전송
    
    PE_SRC_Negotiate_Capability --> PE_SRC_Transition_Supply: Accept 전송
    PE_SRC_Negotiate_Capability --> PE_SRC_Capability_Response: Reject 전송
    
    PE_SRC_Transition_Supply --> PE_SRC_Ready: PS_RDY 전송
    
    state PE_SRC_Ready {
        [*] --> Idle
        Idle --> Get_Source_Cap: Get_Source_Cap 수신
        Idle --> DR_Swap: DR_Swap 요청/수신
        Idle --> PR_Swap: PR_Swap 요청/수신
        Idle --> VCONN_Swap: VCONN_Swap 요청/수신
        Idle --> VDM: VDM 처리
    }
    
    PE_SRC_Ready --> PE_SRC_Hard_Reset: Hard Reset 요청
    PE_SRC_Hard_Reset --> PE_SRC_Transition_to_default
    PE_SRC_Transition_to_default --> PE_SRC_Startup
```

---

## 5. MUX 제어 플로우

### 5.1 MUX 상태 전환

```mermaid
stateDiagram-v2
    [*] --> Disconnected
    
    Disconnected --> USB_Only: Type-C 연결 (USB만)
    USB_Only --> USB_DP_2Lane: DP Alt Mode (2-lane)
    USB_Only --> DP_4Lane: DP Alt Mode (4-lane)
    
    USB_DP_2Lane --> USB_Only: DP 종료
    DP_4Lane --> USB_Only: DP 종료
    
    USB_Only --> Disconnected: Type-C 연결 해제
    USB_DP_2Lane --> Disconnected: Type-C 연결 해제
    DP_4Lane --> Disconnected: Type-C 연결 해제
    
    state USB_Only {
        [*] --> Normal_Orientation
        Normal_Orientation --> Flipped_Orientation: 극성 변경
        Flipped_Orientation --> Normal_Orientation: 극성 변경
    }
```

### 5.2 MUX 설정 플로우

```mermaid
sequenceDiagram
    participant Manager as rt_pd_manager.c
    participant MUX_Core as mux_switch.c
    participant MUX_Driver as MUX Chip Driver
    participant HW as MUX Hardware
    
    Manager->>MUX_Core: mtk_typec_switch_set(sw, orientation)
    MUX_Core->>MUX_Core: 등록된 모든 switch 순회
    
    loop 각 Switch
        MUX_Core->>MUX_Driver: sw_desc.set(sw, orientation)
        MUX_Driver->>MUX_Driver: I2C 레지스터 설정
        MUX_Driver->>HW: 극성 제어 레지스터 쓰기
        HW-->>MUX_Driver: 완료
        MUX_Driver-->>MUX_Core: 0 (성공)
    end
    
    MUX_Core-->>Manager: 0 (성공)
    
    Note over Manager: DP Alt Mode 진입
    Manager->>MUX_Core: mtk_typec_mux_set(mux, DP_STATE)
    MUX_Core->>MUX_Core: 등록된 모든 mux 순회
    
    loop 각 MUX
        MUX_Core->>MUX_Driver: mux_desc.set(mux, state)
        MUX_Driver->>MUX_Driver: DP 설정 분석
        MUX_Driver->>MUX_Driver: I2C 레지스터 설정
        MUX_Driver->>HW: MUX 모드 레지스터 쓰기
        MUX_Driver->>HW: DP 설정 레지스터 쓰기
        HW-->>MUX_Driver: 완료
        MUX_Driver-->>MUX_Core: 0 (성공)
    end
    
    MUX_Core-->>Manager: 0 (성공)
```

---

## 6. 이벤트 처리 플로우

### 6.1 TCPC 이벤트 큐 처리

```mermaid
sequenceDiagram
    participant Source as Event Source
    participant Event as tcpci_event.c
    participant Queue as Event Queue
    participant Worker as Event Worker
    participant Handler as Event Handler
    
    Source->>Event: tcpc_put_event(tcpc, event)
    Event->>Queue: 이벤트 큐에 추가
    Event->>Worker: schedule_work(&tcpc->event_work)
    
    Note over Worker: Workqueue에서 실행
    Worker->>Queue: 이벤트 큐에서 가져오기
    Queue-->>Worker: event
    
    alt Type-C 이벤트
        Worker->>Handler: tcpc_typec_handle_event(tcpc, event)
    else PD 이벤트
        Worker->>Handler: pd_handle_event(tcpc, event)
    else Timer 이벤트
        Worker->>Handler: tcpc_handle_timer_event(tcpc, event)
    end
    
    Handler->>Handler: 이벤트 처리
    Handler-->>Worker: 완료
    
    Worker->>Queue: 다음 이벤트 확인
    
    alt 큐에 이벤트 있음
        Queue-->>Worker: event
        Worker->>Handler: 이벤트 처리
    else 큐 비어있음
        Worker->>Worker: 대기
    end
```

---

## 7. 타이머 관리 플로우

### 7.1 타이머 시작 및 만료

```mermaid
sequenceDiagram
    participant Caller as Caller
    participant Timer as tcpci_timer.c
    participant HRTimer as hrtimer
    participant Event as tcpci_event.c
    
    Caller->>Timer: tcpc_enable_timer(tcpc, timer_id)
    Timer->>Timer: tcpc_timer[timer_id] 설정
    Timer->>HRTimer: hrtimer_start(&timer->hr_timer, timeout)
    HRTimer-->>Timer: 타이머 시작됨
    Timer-->>Caller: 완료
    
    Note over HRTimer: 타임아웃 대기
    HRTimer->>Timer: timer_callback()
    Timer->>Event: tcpc_put_event(tcpc, TIMER_EVENT)
    Event->>Event: 이벤트 큐에 추가
    Event->>Event: schedule_work()
    
    Note over Event: Event Worker 실행
    Event->>Caller: 타이머 이벤트 처리
    Caller->>Caller: 타이머 만료 처리
```

---

## 8. 전체 시스템 통합 플로우

```mermaid
graph TB
    subgraph "User Space"
        A1[sysfs]
        A2[procfs]
        A3[typec class]
    end
    
    subgraph "Platform Integration"
        B1[rt_pd_manager.c]
    end
    
    subgraph "TCPC Core"
        C1[tcpci_core.c]
        C2[tcpci_typec.c]
        C3[pd_core.c]
    end
    
    subgraph "Event System"
        D1[tcpci_event.c]
        D2[tcpci_timer.c]
        D3[tcpci_alert.c]
    end
    
    subgraph "Policy & DPM"
        E1[pd_policy_engine.c]
        E2[pd_dpm_core.c]
    end
    
    subgraph "MUX System"
        F1[mux_switch.c]
        F2[MUX Drivers]
    end
    
    subgraph "Hardware"
        G1[TCPC Chip]
        G2[MUX Chip]
    end
    
    A1 --> C1
    A2 --> F1
    A3 --> B1
    
    B1 --> C1
    B1 --> F1
    
    C1 --> C2
    C1 --> C3
    C1 --> D1
    
    C2 --> D3
    C3 --> E1
    E1 --> E2
    
    D1 --> D2
    D3 --> D1
    
    C2 --> G1
    C3 --> G1
    F1 --> F2
    F2 --> G2
```

이 플로우 차트들은 TypeC 드라이버의 주요 동작 흐름과 파일 간 상호작용을 시각적으로 보여줍니다.
