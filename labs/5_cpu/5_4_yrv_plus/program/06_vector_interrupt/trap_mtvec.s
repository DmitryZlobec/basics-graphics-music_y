.section .text.mtvec
.balign 256, 0
.global __irq_handlers
__irq_handlers:

IRQ_0:
        j default_exception_handler
IRQ_1:
        j default_vector_handler
IRQ_2:
        j default_vector_handler
IRQ_3:
        j software_handler
IRQ_4:
        j default_vector_handler
IRQ_5:
        j default_vector_handler
IRQ_6:
        j default_vector_handler
IRQ_7:
        j timer_handler
IRQ_8:
        j default_vector_handler
IRQ_9:
	j default_vector_handler
IRQ_10:
        j default_vector_handler
IRQ_11:
        j external_handler
IRQ_12:
        j default_vector_handler
IRQ_13:
        j default_vector_handler
IRQ_14:
        j default_vector_handler
IRQ_15:
        j default_vector_handler
IRQ_16:
        j default_vector_handler
IRQ_17:
        j default_vector_handler
IRQ_18:
        j default_vector_handler
IRQ_19:
        j default_vector_handler
IRQ_20:
        j default_vector_handler
IRQ_21:
        j default_vector_handler
IRQ_22:
        j default_vector_handler
IRQ_23:
        j default_vector_handler
IRQ_24:
        j default_vector_handler
IRQ_25:
        j default_vector_handler
IRQ_26:
        j default_vector_handler
IRQ_27:
        j default_vector_handler
IRQ_28:
        j default_vector_handler
IRQ_29:
        j default_vector_handler
IRQ_30:
        j default_vector_handler
IRQ_31:
        j default_vector_handler
IRQ_32:
        j default_vector_handler
IRQ_33:
        j default_vector_handler
IRQ_34:
        j default_vector_handler
IRQ_35:
        j default_vector_handler
IRQ_36:
        j default_vector_handler
IRQ_37:
        j default_vector_handler
IRQ_38:
        j default_vector_handler
IRQ_39:
        j default_vector_handler
IRQ_40:
        j default_vector_handler
IRQ_41:
        j default_vector_handler
IRQ_42:
        j default_vector_handler
IRQ_43:
        j default_vector_handler
IRQ_44:
        j default_vector_handler
IRQ_45:
        j default_vector_handler
IRQ_46:
        j default_vector_handler
IRQ_47:
        j default_vector_handler
        