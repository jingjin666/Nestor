;*******************************************************************
; ȫ�ֱ���&����
;*******************************************************************
	IMPORT OSTCBCurPtr ; �ⲿ�ļ����˵Ĳο�
	IMPORT OSTCBHighRdyPtr
	IMPORT OSPrioCur
	IMPORT OSPrioHighRdy

	EXPORT OSStartHighRdy ; ���ļ�����ĺ���
	EXPORT OS_CPU_PendSVHandler
	EXPORT CPU_IntDis
	EXPORT CPU_IntEn
	EXPORT CPU_SR_Save
        EXPORT CPU_SR_Restore
	EXPORT CPU_CntLeadZeros
	EXPORT CPU_CntTrailZeros

;*******************************************************************
;����
;�й��ں�����Ĵ�������ɲο��ٷ��ĵ���STM32F10xxx Cortex-M3 programming manual
;ϵͳ���ƿ����� SCB ��ַ��Χ��0xE000ED00-0xE000ED3F
;*******************************************************************
NVIC_INT_CTRL 	EQU 0xE000ED04 ; �жϿ��Ƽ�״̬�Ĵ��� SCB_ICSR��
NVIC_SYSPRI14 	EQU 0xE000ED22 ; ϵͳ���ȼ��Ĵ��� SCB_SHPR3��
							   ; bit16~23
NVIC_PENDSV_PRI EQU 0xFF 	   ; PendSV ���ȼ���ֵ(���)��
NVIC_PENDSVSET 	EQU 0x10000000 ; ���� PendSV �쳣��ֵ Bit28��PENDSVSET

;********************************************************************************************************
;                                     CODE GENERATION DIRECTIVES
;********************************************************************************************************
    
	PRESERVE8
    	THUMB

    	AREA CODE, CODE, READONLY

;*******************************************************************
;                                    DISABLE and ENABLE INTERRUPTS
;
; Description: Disable/Enable interrupts.
;
; Prototypes : void  CPU_IntDis(void);
;              void  CPU_IntEn (void);
;*******************************************************************
CPU_IntDis
        CPSID   I
        BX      LR


CPU_IntEn
        CPSIE   I
        BX      LR

;********************************************************************************************************
;                                      CRITICAL SECTION FUNCTIONS
;
; Description : Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking, the
;               state of the interrupt disable flag is stored in the local variable 'cpu_sr' & interrupts
;               are then disabled ('cpu_sr' is allocated in all functions that need to disable interrupts).
;               The previous interrupt state is restored by copying 'cpu_sr' into the CPU's status register.
;
; Prototypes  : CPU_SR  CPU_SR_Save   (void);
;               void    CPU_SR_Restore(CPU_SR  cpu_sr);
;
; Note(s)     : (1) These functions are used in general like this :
;
;                       void  Task (void  *p_arg)
;                       {
;                           CPU_SR_ALLOC();                     /* Allocate storage for CPU status register */
;                               :
;                               :
;                           CPU_CRITICAL_ENTER();               /* cpu_sr = CPU_SR_Save();                  */
;                               :
;                               :
;                           CPU_CRITICAL_EXIT();                /* CPU_SR_Restore(cpu_sr);                  */
;                               :
;                       }
;********************************************************************************************************

CPU_SR_Save
        MRS     R0, PRIMASK                     ; Set prio int mask to mask all (except faults)
        CPSID   I
        BX      LR


CPU_SR_Restore                                  ; See Note #2.
        MSR     PRIMASK, R0
        BX      LR
        
;********************************************************************************************************
;                                          ����ǰ��0����
;
; ����      ��
;
; ��������  �� CPU_DATA  CPU_CntLeadZeros(CPU_DATA  val);
;             
;********************************************************************************************************
CPU_CntLeadZeros
        CLZ     R0, R0                          ; Count leading zeros
        BX      LR

;********************************************************************************************************
;                                          �����0����
;
; ����      ��
;
; ��������  �� CPU_DATA  CPU_CntTrailZeros(CPU_DATA  val);
;             
;********************************************************************************************************

CPU_CntTrailZeros
        RBIT    R0, R0                          ; Reverse bits
        CLZ     R0, R0                          ; Count trailing zeros
        BX      LR
        
;*******************************************************************
;				��ʼһ���������л�
;
;
;*******************************************************************
OSStartHighRdy
	LDR R0, = NVIC_SYSPRI14 ; ���� PendSV �쳣���ȼ�Ϊ���
	LDR R1, = NVIC_PENDSV_PRI
	STRB R1, [R0]

	MOVS R0, #0 ;���� psp ��ֵΪ 0����ʼ��һ���������л�
	MSR PSP, R0

	LDR R0, = NVIC_INT_CTRL ; ���� PendSV �쳣

	LDR R1, = NVIC_PENDSVSET
	STR R1, [R0]

	CPSIE I ; ʹ�����жϣ�NMI �� HardFault ����

OSStartHang
	B OSStartHang ; ����Ӧ��Զ�������е�����

;***********************************************************************
; OS_CPU_PendSVHandler �쳣
;***********************************************************************
OS_CPU_PendSVHandler
	; ���жϣ�NMI �� HardFault ���⣬��ֹ�������л����ж�
	CPSID I

	; �� psp ��ֵ���ص� R0
	MRS R0, PSP

	; �ж� R0�����ֵΪ 0 ����ת�� OS_CPU_PendSVHandler_nosave
	; ���е�һ�������л���ʱ��R0 �϶�Ϊ 0
	CBZ R0, OS_CPU_PendSVHandler_nosave

	;-----------------------һ����������-----------------------------
	; ������л���������һ��Ҫ���е�����Ķ�ջ���ݼ��ص� CPU �Ĵ�����
	;-------------------------------------------------------------- 
	; �ڽ��� PendSV �쳣��ʱ�򣬵�ǰ CPU �� xPSR��PC��������ڵ�ַ����
	; R14��R12��R3��R2��R1��R0 ���Զ��洢����ǰ�����ջ��
	; ͬʱ�ݼ� PSP ��ֵ�����ͨ�� ���룺MRS R0, PSP �� PSP ��ֵ���� R0

	; �ֶ��洢 CPU �Ĵ��� R4-R11 ��ֵ����ǰ����Ķ�ջ
	STMDB R0!, {R4-R11}

	; ���� OSTCBCurPtr ָ��ĵ�ַ�� R1������ LDR ����αָ��
	LDR R1, = OSTCBCurPtr
	; ���� OSTCBCurPtr ָ�뵽 R1������ LDR ���� ARM ָ��
	LDR R1, [R1]
	;  �洢 R0 ��ֵ�� OSTCBCurPtr->OSTCBStkPtr�����ʱ�� R0 ������������ջ��ջ��
	STR R0, [R1]

;-----------------------�����л�����-----------------------------
; ʵ�� OSTCBCurPtr = OSTCBHighRdyPtr
; ����һ��Ҫ���е�����Ķ�ջ���ݼ��ص� CPU �Ĵ�����
;--------------------------------------------------------------
OS_CPU_PendSVHandler_nosave
	; OSPrioCur = OSPrioHighRdy
	LDR R0, = OSPrioCur
	LDR R1, = OSPrioHighRdy
	LDRB R2, [R1]
	STRB R2, [R0]

	; ���� OSTCBCurPtr ָ��ĵ�ַ�� R0������ LDR ����αָ��
	LDR R0, = OSTCBCurPtr
	; ���� OSTCBHighRdyPtr ָ��ĵ�ַ�� R1������ LDR ����αָ��
	LDR R1, = OSTCBHighRdyPtr
	; ���� OSTCBHighRdyPtr ָ�뵽 R2������ LDR ���� ARM ָ�� 
	LDR R2, [R1]
	; �洢 OSTCBHighRdyPtr �� OSTCBCurPtr
	STR R2, [R0]

	; ���� OSTCBHighRdyPtr �� R0 
	LDR R0, [R2]
	; ������Ҫ�ֶ��������Ϣ�� CPU �Ĵ��� R4-R11
	LDMIA R0!, {R4-R11}

	; ���� PSP ��ֵ�����ʱ�� PSP ָ����һ��Ҫִ�е�����Ķ�ջ��ջ��
	;�����ջ���Ѿ����ϸո��ֶ����ص� CPU �Ĵ��� R4-R11 ��ƫ�ƣ� 
	MSR PSP, R0

	; ȷ���쳣����ʹ�õĶ�ջָ���� PSP���� LR �Ĵ�����λ 2 ҪΪ 1
	ORR LR, LR, #0x04

	; ���ж�
	CPSIE I

	; �쳣���أ����ʱ�������ջ�е�ʣ�����ݽ����Զ����ص� xPSR��
	; PC��������ڵ�ַ����R14��R12��R3��R2��R1��R0��������βΣ�
	; ͬʱ PSP ��ֵҲ�����£���ָ�������ջ��ջ����
	; �� STM32 �У���ջ���ɸߵ�ַ��͵�ַ�����ġ�
	BX LR 
	END
