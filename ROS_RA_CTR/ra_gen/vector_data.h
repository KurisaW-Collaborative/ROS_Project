/* generated vector header file - do not edit */
        #ifndef VECTOR_DATA_H
        #define VECTOR_DATA_H
                /* Number of interrupts allocated */
        #ifndef VECTOR_DATA_IRQ_COUNT
        #define VECTOR_DATA_IRQ_COUNT    (18)
        #endif
        /* ISR prototypes */
        void sci_uart_rxi_isr(void);
        void sci_uart_txi_isr(void);
        void sci_uart_tei_isr(void);
        void sci_uart_eri_isr(void);
        void r_icu_isr(void);
        void iic_master_rxi_isr(void);
        void iic_master_txi_isr(void);
        void iic_master_tei_isr(void);
        void iic_master_eri_isr(void);
        void gpt_counter_overflow_isr(void);
        void rtc_carry_isr(void);

        /* Vector table allocations */
        #define VECTOR_NUMBER_SCI7_RXI ((IRQn_Type) 0) /* SCI7 RXI (Received data full) */
        #define SCI7_RXI_IRQn          ((IRQn_Type) 0) /* SCI7 RXI (Received data full) */
        #define VECTOR_NUMBER_SCI7_TXI ((IRQn_Type) 1) /* SCI7 TXI (Transmit data empty) */
        #define SCI7_TXI_IRQn          ((IRQn_Type) 1) /* SCI7 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI7_TEI ((IRQn_Type) 2) /* SCI7 TEI (Transmit end) */
        #define SCI7_TEI_IRQn          ((IRQn_Type) 2) /* SCI7 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI7_ERI ((IRQn_Type) 3) /* SCI7 ERI (Receive error) */
        #define SCI7_ERI_IRQn          ((IRQn_Type) 3) /* SCI7 ERI (Receive error) */
        #define VECTOR_NUMBER_ICU_IRQ0 ((IRQn_Type) 4) /* ICU IRQ0 (External pin interrupt 0) */
        #define ICU_IRQ0_IRQn          ((IRQn_Type) 4) /* ICU IRQ0 (External pin interrupt 0) */
        #define VECTOR_NUMBER_SCI3_RXI ((IRQn_Type) 5) /* SCI3 RXI (Received data full) */
        #define SCI3_RXI_IRQn          ((IRQn_Type) 5) /* SCI3 RXI (Received data full) */
        #define VECTOR_NUMBER_SCI3_TXI ((IRQn_Type) 6) /* SCI3 TXI (Transmit data empty) */
        #define SCI3_TXI_IRQn          ((IRQn_Type) 6) /* SCI3 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_SCI3_TEI ((IRQn_Type) 7) /* SCI3 TEI (Transmit end) */
        #define SCI3_TEI_IRQn          ((IRQn_Type) 7) /* SCI3 TEI (Transmit end) */
        #define VECTOR_NUMBER_SCI3_ERI ((IRQn_Type) 8) /* SCI3 ERI (Receive error) */
        #define SCI3_ERI_IRQn          ((IRQn_Type) 8) /* SCI3 ERI (Receive error) */
        #define VECTOR_NUMBER_IIC0_RXI ((IRQn_Type) 9) /* IIC0 RXI (Receive data full) */
        #define IIC0_RXI_IRQn          ((IRQn_Type) 9) /* IIC0 RXI (Receive data full) */
        #define VECTOR_NUMBER_IIC0_TXI ((IRQn_Type) 10) /* IIC0 TXI (Transmit data empty) */
        #define IIC0_TXI_IRQn          ((IRQn_Type) 10) /* IIC0 TXI (Transmit data empty) */
        #define VECTOR_NUMBER_IIC0_TEI ((IRQn_Type) 11) /* IIC0 TEI (Transmit end) */
        #define IIC0_TEI_IRQn          ((IRQn_Type) 11) /* IIC0 TEI (Transmit end) */
        #define VECTOR_NUMBER_IIC0_ERI ((IRQn_Type) 12) /* IIC0 ERI (Transfer error) */
        #define IIC0_ERI_IRQn          ((IRQn_Type) 12) /* IIC0 ERI (Transfer error) */
        #define VECTOR_NUMBER_GPT3_COUNTER_OVERFLOW ((IRQn_Type) 13) /* GPT3 COUNTER OVERFLOW (Overflow) */
        #define GPT3_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 13) /* GPT3 COUNTER OVERFLOW (Overflow) */
        #define VECTOR_NUMBER_GPT2_COUNTER_OVERFLOW ((IRQn_Type) 14) /* GPT2 COUNTER OVERFLOW (Overflow) */
        #define GPT2_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 14) /* GPT2 COUNTER OVERFLOW (Overflow) */
        #define VECTOR_NUMBER_ICU_IRQ1 ((IRQn_Type) 15) /* ICU IRQ1 (External pin interrupt 1) */
        #define ICU_IRQ1_IRQn          ((IRQn_Type) 15) /* ICU IRQ1 (External pin interrupt 1) */
        #define VECTOR_NUMBER_ICU_IRQ4 ((IRQn_Type) 16) /* ICU IRQ4 (External pin interrupt 4) */
        #define ICU_IRQ4_IRQn          ((IRQn_Type) 16) /* ICU IRQ4 (External pin interrupt 4) */
        #define VECTOR_NUMBER_RTC_CARRY ((IRQn_Type) 17) /* RTC CARRY (Carry interrupt) */
        #define RTC_CARRY_IRQn          ((IRQn_Type) 17) /* RTC CARRY (Carry interrupt) */
        #endif /* VECTOR_DATA_H */