#include <kernel/ksyms.h>

ksyms_t symlist[] = {
    {0x00100000, "__kernel_text_section_start"},
    {0x0010000c, "code"},
    {0x0010000c, "__code"},
    {0x0010000c, "_code"},
    {0x00100010, "MultibootHeader"},
    {0x00100028, "kernel_load"},
    {0x00100028, "MultibootHeaderEnd"},
    {0x00100037, "SystemHalt"},
    {0x0010003a, "StackBottom"},
    {0x0020003a, "get_cmdline"},
    {0x0020003a, "StackTop"},
    {0x0020004f, "register_devices"},
    {0x002001c3, "bss_init"},
    {0x002001f2, "KernelMain"},
    {0x00200397, "KernelMainStage2"},
    {0x00200422, "VideoOutputCharVGA"},
    {0x002004c0, "scroll"},
    {0x0020055b, "VideoOutputVGA"},
    {0x002005d4, "VideoClearVGA"},
    {0x00200629, "VideoSetColor"},
    {0x00200642, "VideoUpdateBackgroundVGA"},
    {0x002006a6, "VideoUpdateCursorVGA"},
    {0x0020071d, "reverse"},
    {0x0020079d, "itoa"},
    {0x00200870, "dump_eip"},
    {0x00200873, "arch_get_brand_string"},
    {0x00200938, "InterruptsEnabled"},
    {0x0020095a, "InterruptsRelease"},
    {0x00200974, "InterruptsLock"},
    {0x0020098f, "IoPortByteWrite"},
    {0x002009ba, "IoPortWordWrite"},
    {0x002009e5, "IoPortDwordWrite"},
    {0x00200a07, "IoPortByteRead"},
    {0x00200a2e, "IoPortWordRead"},
    {0x00200a57, "IoPortDwordRead"},
    {0x00200a7d, "SerialInitialize"},
    {0x00200b71, "SerialTransitEmpty"},
    {0x00200b9f, "SerialWrite"},
    {0x00200bdf, "SerialReceived"},
    {0x00200c0d, "SerialRead"},
    {0x00200c3e, "_serial_read"},
    {0x00200c72, "init_mem"},
    {0x00200ca1, "arch_init"},
    {0x00200ccd, "InitializeHALStage1"},
    {0x00200d38, "DebugOutputChar"},
    {0x00200d67, "DebugOutput"},
    {0x00200f02, "InitializeDebugIO"},
    {0x00200f66, "strlen"},
    {0x00200f96, "WriteTSS"},
    {0x00201050, "SetGateGDT"},
    {0x002010e8, "InitializeGDT"},
    {0x002011af, "SetKernelStack"},
    {0x002011d0, "InstallGDT"},
    {0x002011ec, "far_jump"},
    {0x002011ed, "SetGateIDT"},
    {0x0020125b, "InitializeIDT"},
    {0x00201740, "InstallIDT"},
    {0x00201750, "ExceptionHandler"},
    {0x00201775, "Exception_0"},
    {0x0020177c, "Exception_1"},
    {0x00201783, "Exception_2"},
    {0x0020178a, "Exception_3"},
    {0x00201791, "Exception_4"},
    {0x00201798, "Exception_5"},
    {0x0020179f, "Exception_6"},
    {0x002017a6, "Exception_7"},
    {0x002017ad, "Exception_8"},
    {0x002017b2, "Exception_9"},
    {0x002017b9, "Exception_10"},
    {0x002017be, "Exception_11"},
    {0x002017c3, "Exception_12"},
    {0x002017c8, "Exception_13"},
    {0x002017cd, "Exception_14"},
    {0x002017d5, "Exception_15"},
    {0x002017df, "Exception_16"},
    {0x002017e9, "Exception_17"},
    {0x002017f3, "Exception_18"},
    {0x002017fd, "Exception_19"},
    {0x00201807, "Exception_20"},
    {0x00201811, "Exception_21"},
    {0x0020181b, "Exception_22"},
    {0x00201825, "Exception_23"},
    {0x0020182f, "Exception_24"},
    {0x00201839, "Exception_25"},
    {0x00201843, "Exception_26"},
    {0x0020184d, "Exception_27"},
    {0x00201857, "Exception_28"},
    {0x00201861, "Exception_29"},
    {0x0020186b, "Exception_30"},
    {0x00201875, "Exception_31"},
    {0x0020187f, "Exception_128"},
    {0x00201890, "HandlerIRQ"},
    {0x002018b7, "irq_0"},
    {0x002018be, "irq_1"},
    {0x002018c5, "irq_2"},
    {0x002018cc, "irq_3"},
    {0x002018d3, "irq_4"},
    {0x002018da, "irq_5"},
    {0x002018e1, "irq_6"},
    {0x002018e8, "irq_7"},
    {0x002018ef, "irq_8"},
    {0x002018f6, "irq_9"},
    {0x002018fd, "irq_10"},
    {0x00201904, "irq_11"},
    {0x0020190b, "irq_12"},
    {0x00201915, "irq_13"},
    {0x0020191f, "irq_14"},
    {0x00201929, "irq_15"},
    {0x00201933, "RegisterInterrupt"},
    {0x00201959, "IRQHandlerISR"},
    {0x002019b5, "DumpRegisters"},
    {0x00201a70, "ExceptionHandlerISR"},
    {0x00201b2f, "InitializePIC"},
    {0x00201c27, "EndOfInterruptPIC"},
    {0x00201c6c, "SetFrequencyTimer"},
    {0x00201cfa, "TimerHandler"},
    {0x00201d40, "get_uptime_seconds"},
    {0x00201d65, "get_uptime_milliseconds"},
    {0x00201d7e, "InitializeTimer"},
    {0x00201dba, "memset"},
    {0x00201df6, "invalidate"},
    {0x00201e0a, "MapVirtualToPhys"},
    {0x00201e82, "PageFaultHandler"},
    {0x00201f9e, "InitializePaging"},
    {0x0020205c, "exec"},
    {0x002020d8, "ftoa"},
    {0x00202225, "SetControlWordFPU"},
    {0x00202242, "EnableFPU"},
    {0x0020228f, "memcpy"},
    {0x002022d9, "InitializeMemoryManagement"},
    {0x0020239c, "pmalloc"},
    {0x002023f7, "pfree"},
    {0x0020244c, "malloc"},
    {0x002025d7, "free"},
    {0x00202616, "pci_config_read_short"},
    {0x002026cd, "pci_bruteforce"},
    {0x0020281a, "pci_init"},
    {0x00202832, "pci_get_vendor"},
    {0x00202884, "pci_get_device"},
    {0x002028d6, "pci_get_class"},
    {0x0020292b, "pci_get_subclass"},
    {0x00202980, "pci_check_vendor"},
    {0x002029e0, "LoadTSS"},
    {0x002029e8, "kthread"},
    {0x00202a1d, "get_free_pid"},
    {0x00202a98, "createProcess"},
    {0x00202c68, "getProcess"},
    {0x00202cac, "reload_context"},
    {0x00202ce2, "preempt_now"},
    {0x00202d00, "wakeup"},
    {0x00202d3f, "sleep"},
    {0x00202d78, "__kill__"},
    {0x00202e4f, "task_exit"},
    {0x00202e8b, "__notify__"},
    {0x00202f3c, "initTasking"},
    {0x00202fb9, "getCurrentProcess"},
    {0x00202fce, "addProcess"},
    {0x00203057, "is_pid_running"},
    {0x002030ad, "kill"},
    {0x00203153, "preempt"},
    {0x002031fd, "prepareDisk"},
    {0x002032dc, "readSector"},
    {0x0020335b, "writeSector"},
    {0x002033f2, "writeSectors"},
    {0x00203440, "readSectors"},
    {0x0020348e, "getFirstPartition"},
    {0x002034fa, "strcmp"},
    {0x00203543, "indexOf"},
    {0x002035a0, "substrr"},
    {0x002035e7, "strcat"},
    {0x00203672, "strcpy"},
    {0x002036c2, "sys_exit"},
    {0x00203705, "sys_fork"},
    {0x002037ff, "sys_uname"},
    {0x002038e8, "sys_getpid"},
    {0x00203922, "sys_write"},
    {0x00203936, "sys_read"},
    {0x0020394a, "sys_open"},
    {0x0020395a, "sys_execve"},
    {0x0020396e, "SyscallHandler"},
    {0x002039a2, ".L28"},
    {0x002039b9, ".L27"},
    {0x002039c3, ".L21"},
    {0x002039cd, ".L26"},
    {0x002039f1, ".L25"},
    {0x00203a15, ".L23"},
    {0x00203a3b, ".L20"},
    {0x00203a68, ".L30"},
    {0x00203a74, "EnableSystemCalls"},
    {0x00203aa4, "parse_cmdline"},
    {0x00203c3f, "StackTrace"},
    {0x00203ce0, "strstr"},
    {0x00203d40, "memcmp"},
    {0x00203db2, "strcspn"},
    {0x00203e13, "strchr"},
    {0x00203e60, "check_cpuid"},
    {0x00203e77, "check_cpuid.cpuid_no_support"},
    {0x00203e7b, "cmos_get_register"},
    {0x00203eb7, "cmos_get_update_flag"},
    {0x00203eed, "shift"},
    {0x00203f76, "do_keybd_intr"},
    {0x00204090, ".L27"},
    {0x00204094, ".L26"},
    {0x00204098, ".L23"},
    {0x0020409c, ".L25"},
    {0x002040a0, ".L24"},
    {0x002040a4, ".L21"},
    {0x002040a8, ".L28"},
    {0x002040ac, ".L20"},
    {0x00204124, "dev_keybd_read"},
    {0x00204133, "keyboard_init"},
    {0x002041e3, "rand"},
    {0x0020425b, "divide_by_zero_handler"},
    {0x00204281, "gpf_handler"},
    {0x002042a7, "invalid_opcode_handler"},
    {0x002042cd, "install_exception_handlers"},
    {0x00204321, "notify"},
    {0x00204353, "spin_lock_new"},
    {0x0020437e, "spin_lock_acquire"},
    {0x002043a1, "spin_lock_release"},
    {0x002043bb, "putq"},
    {0x0020442a, "getq"},
    {0x0020448c, "eraseq"},
    {0x002044e0, "tty_canon"},
    {0x00204622, "tty_output"},
    {0x002046bd, "tty_input"},
    {0x00204793, "tty_start"},
    {0x002047e2, "tty_open"},
    {0x00204861, "tty_close"},
    {0x0020487f, "tty_read"},
    {0x00204946, "tty_write"},
    {0x002049ff, "dev_tty_read"},
    {0x00204a25, "dev_tty_write"},
    {0x00204a4b, "tty_init"},
    {0x00204ae8, "get_num_partitions"},
    {0x00204b55, "__get_cpuid_max"},
    {0x00204bc7, "__get_cpuid"},
    {0x00204c3c, "check_apic"},
    {0x00204c70, "initialize_apic"},
    {0x00204cc1, "trace_address"},
    {0x00204d22, "strncmp"},
    {0x00204d82, "validate_checksum"},
    {0x00204dd7, "xsdp_detect"},
    {0x00204ea8, "findFACP"},
    {0x00204f3c, "acpi_init"},
    {0x00205040, "LoadPageDirectory"},
    {0x0020505b, "devfs_get_device_byname"},
    {0x00205130, "devfs_get_registered_devices"},
    {0x0020517a, "devfs_get_device_node"},
    {0x00205216, "devfs_read_dir"},
    {0x00205438, "devfs_create_device"},
    {0x002054db, "devfs_get_free_node_id"},
    {0x00205521, "devfs_unregister"},
    {0x002055bd, "devfs_node2dev"},
    {0x00205671, "devfs_register"},
    {0x00205867, "devfs_path2node"},
    {0x0020598c, "devfs_open"},
    {0x00205a28, "devfs_init"},
    {0x00205b01, "devfs_read"},
    {0x00205bfa, "devfs_write"},
    {0x00205d47, "devfs_mount"},
    {0x00205d7a, "printk_print_time"},
    {0x00205e12, "vprintk"},
    {0x00205e68, ".L8"},
    {0x00205e89, ".L11"},
    {0x00205ebf, ".L6"},
    {0x00205ef5, ".L9"},
    {0x00205f2c, ".L10"},
    {0x00205fbc, ".L17"},
    {0x00206004, "printk"},
    {0x0020602f, "PANIC"},
    {0x00206060, "ext2_is_valid"},
    {0x002060a6, "ext2_read_superblock"},
    {0x002060fe, "ext2_read_inode"},
    {0x0020626c, "ext2_list_dir"},
    {0x0020636f, "ext2_read_dir"},
    {0x00206480, "ext2_inode2vfs_node"},
    {0x002065ea, "ext2_mount"},
    {0x0020672c, "ext2_block_to_sector"},
    {0x00206753, "ext2_read_block"},
    {0x00206795, "ext2_block_allocate"},
    {0x002067c2, "ext2_block_free"},
    {0x002067e9, "ext2_get_block_group_inode"},
    {0x00206814, "ext2_get_index_inode"},
    {0x0020683d, "ext2_get_block_inode"},
    {0x0020687c, "gop_putpx"},
    {0x002068ed, "set_bg"},
    {0x00206944, "uefi_gop_init"},
    {0x00209000, "__x86.get_pc_thunk.ax"},
    {0x00209000, "__kernel_text_section_end"},
    {0x00209004, "__x86.get_pc_thunk.bx"},
    {0x00209008, "__x86.get_pc_thunk.cx"},
    {0x0020900c, "__x86.get_pc_thunk.si"},
    {0xc0000000, "kernel_base"},
  {0xffffffff, ""}
};