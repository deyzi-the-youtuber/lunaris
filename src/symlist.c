#include <lunaris/ksyms.h>

ksyms_t symlist[] = {
    {0x00000000, "__x86.get_pc_thunk.ax"},
    {0x00000000, "__x86.get_pc_thunk.bx"},
    {0x00000000, "__x86.get_pc_thunk.cx"},
    {0x00000000, "__x86.get_pc_thunk.si"},
    {0x00100000, "InstallGDT"},
    {0x00100000, "kernel_base"},
    {0x0010001c, "far_jump"},
    {0x00100020, "ExceptionHandler"},
    {0x00100045, "Exception_0"},
    {0x0010004c, "Exception_1"},
    {0x00100053, "Exception_2"},
    {0x0010005a, "Exception_3"},
    {0x00100061, "Exception_4"},
    {0x00100068, "Exception_5"},
    {0x0010006f, "Exception_6"},
    {0x00100076, "Exception_7"},
    {0x0010007d, "Exception_8"},
    {0x00100082, "Exception_9"},
    {0x00100089, "Exception_10"},
    {0x0010008e, "Exception_11"},
    {0x00100093, "Exception_12"},
    {0x00100098, "Exception_13"},
    {0x0010009d, "Exception_14"},
    {0x001000a5, "Exception_15"},
    {0x001000af, "Exception_16"},
    {0x001000b9, "Exception_17"},
    {0x001000c3, "Exception_18"},
    {0x001000cd, "Exception_19"},
    {0x001000d7, "Exception_20"},
    {0x001000e1, "Exception_21"},
    {0x001000eb, "Exception_22"},
    {0x001000f5, "Exception_23"},
    {0x001000ff, "Exception_24"},
    {0x00100109, "Exception_25"},
    {0x00100113, "Exception_26"},
    {0x0010011d, "Exception_27"},
    {0x00100127, "Exception_28"},
    {0x00100131, "Exception_29"},
    {0x0010013b, "Exception_30"},
    {0x00100145, "Exception_31"},
    {0x0010014f, "Exception_128"},
    {0x00100160, "HandlerIRQ"},
    {0x00100187, "irq_0"},
    {0x0010018e, "irq_1"},
    {0x00100195, "irq_2"},
    {0x0010019c, "irq_3"},
    {0x001001a3, "irq_4"},
    {0x001001aa, "irq_5"},
    {0x001001b1, "irq_6"},
    {0x001001b8, "irq_7"},
    {0x001001bf, "irq_8"},
    {0x001001c6, "irq_9"},
    {0x001001cd, "irq_10"},
    {0x001001d4, "irq_11"},
    {0x001001db, "irq_12"},
    {0x001001e5, "irq_13"},
    {0x001001ef, "irq_14"},
    {0x001001f9, "irq_15"},
    {0x00100210, "MultibootHeader"},
    {0x00100220, "FramebufferTagEnd"},
    {0x00100220, "FramebufferTag"},
    {0x00100228, "kernel_load"},
    {0x00100228, "MultibootHeaderEnd"},
    {0x00100238, "SystemHalt"},
    {0x00100240, "LoadTSS"},
    {0x00100250, "check_cpuid"},
    {0x00100267, "check_cpuid.cpuid_no_support"},
    {0x00100270, "dump_eip"},
    {0x00100280, "InstallIDT"},
    {0x00100290, "putq"},
    {0x00100302, "getq"},
    {0x00100367, "eraseq"},
    {0x001003bf, "tty_canon"},
    {0x001004ff, "tty_output"},
    {0x0010059c, "tty_input"},
    {0x0010068c, "tty_start"},
    {0x001006db, "tty_open"},
    {0x0010075a, "tty_close"},
    {0x00100778, "tty_read"},
    {0x0010085b, "tty_write"},
    {0x001008fe, "dev_tty_read"},
    {0x0010094d, "dev_tty_write"},
    {0x0010099c, "tty_init"},
    {0x00100a39, "smbios_locate_entry"},
    {0x00100b1f, "smbios_get_table_len"},
    {0x00100b83, "smbios_init"},
    {0x00100c50, "SerialInitialize"},
    {0x00100d44, "SerialTransitEmpty"},
    {0x00100d72, "SerialWrite"},
    {0x00100db2, "SerialReceived"},
    {0x00100de0, "_serial_read"},
    {0x00100e11, "SerialRead"},
    {0x00100e62, "DebugOutputChar"},
    {0x00100e91, "DebugOutput"},
    {0x0010104d, "prepareDisk"},
    {0x0010112c, "readSector"},
    {0x001011ad, "writeSector"},
    {0x00101244, "writeSectors"},
    {0x00101292, "readSectors"},
    {0x001012e0, "getFirstPartition"},
    {0x0010134c, "get_num_partitions"},
    {0x001013eb, "shift"},
    {0x00101474, "do_keybd_intr"},
    {0x0010158f, ".L27"},
    {0x00101593, ".L26"},
    {0x00101597, ".L23"},
    {0x0010159b, ".L25"},
    {0x0010159f, ".L24"},
    {0x001015a3, ".L21"},
    {0x001015a7, ".L28"},
    {0x001015ab, ".L20"},
    {0x0010160a, "dev_keybd_read"},
    {0x0010161a, "keyboard_init"},
    {0x001016ca, "pci_read_word"},
    {0x00101781, "pci_get_class"},
    {0x001017d6, "pci_get_subclass"},
    {0x0010182b, "pci_get_device"},
    {0x0010187d, "pci_get_vendor"},
    {0x001018ce, "pci_device_filter"},
    {0x0010192d, "pci_get_details"},
    {0x00101a9b, "pci_init"},
    {0x00101ab3, "_pci_do_probe"},
    {0x00101ac3, "pci_enum"},
    {0x00101c51, "pci_device_parse"},
    {0x00101c8f, ".L43"},
    {0x00101ca8, ".L42"},
    {0x00101cc1, ".L41"},
    {0x00101cda, ".L40"},
    {0x00101cf3, ".L39"},
    {0x00101d0c, ".L38"},
    {0x00101d25, ".L37"},
    {0x00101d3e, ".L36"},
    {0x00101d57, ".L35"},
    {0x00101d70, ".L34"},
    {0x00101d89, ".L33"},
    {0x00101da2, ".L32"},
    {0x00101db8, ".L31"},
    {0x00101dce, ".L30"},
    {0x00101de4, ".L29"},
    {0x00101dfa, ".L28"},
    {0x00101e10, ".L26"},
    {0x00101e26, ".L45"},
    {0x00101e2d, "__vga_generic_scroll"},
    {0x00101f09, "__vga_generic_write"},
    {0x00101fb1, "__vga_generic_remap"},
    {0x0010203d, "vga_generic_init"},
    {0x00102052, "video_init"},
    {0x00102075, "video_putc"},
    {0x001020a9, "video_puts"},
    {0x001020f3, "video_getx"},
    {0x0010210b, "video_gety"},
    {0x00102123, "video_get_driver"},
    {0x00102138, "video_remap"},
    {0x00102165, "spinlock_acquire"},
    {0x00102188, "spinlock_release"},
    {0x001021a2, "notify"},
    {0x001021d7, "_trace_return"},
    {0x001021e7, "sys_exit"},
    {0x00102222, "sys_fork"},
    {0x00102322, "sys_uname"},
    {0x00102442, "sys_getpid"},
    {0x0010247c, "sys_write"},
    {0x00102574, "sys_read"},
    {0x00102645, "is_ext2"},
    {0x001026cf, "sys_open"},
    {0x001028e6, "sys_execve"},
    {0x001029b9, "SyscallHandler"},
    {0x001029ef, ".L67"},
    {0x00102a06, ".L66"},
    {0x00102a13, ".L60"},
    {0x00102a20, ".L65"},
    {0x00102a4a, ".L64"},
    {0x00102a71, ".L63"},
    {0x00102a9b, ".L62"},
    {0x00102ac4, ".L59"},
    {0x00102b1a, "EnableSystemCalls"},
    {0x00102b4a, "kthread"},
    {0x00102b80, "get_free_pid"},
    {0x00102bfe, "createProcess"},
    {0x00102e73, "createUserProcess"},
    {0x00103122, "getProcess"},
    {0x00103169, "reload_context"},
    {0x001031a2, "preempt_now"},
    {0x001031c6, "wakeup"},
    {0x00103216, "sleep"},
    {0x00103278, "__kill__"},
    {0x00103366, "__notify__"},
    {0x00103433, "initTasking"},
    {0x001034b6, "getCurrentProcess"},
    {0x001034cb, "addProcess"},
    {0x00103566, "is_pid_running"},
    {0x001035b8, "kill"},
    {0x00103674, "preempt"},
    {0x00103716, "strncmp"},
    {0x00103776, "strtok"},
    {0x00103813, "substrr"},
    {0x0010385a, "strcpy"},
    {0x001038aa, "strsplit"},
    {0x0010390b, "memcmp"},
    {0x0010397d, "strlen"},
    {0x001039ab, "rand"},
    {0x00103a21, "strchr"},
    {0x00103a69, "memset"},
    {0x00103aa5, "strcmp"},
    {0x00103aee, "strcat"},
    {0x00103b79, "ftoa"},
    {0x00103ce8, "memcpy"},
    {0x00103d2c, "reverse"},
    {0x00103dac, "itoa"},
    {0x00103e78, "strncpy"},
    {0x00103eb0, "vsprintf"},
    {0x001040ff, "indexOf"},
    {0x0010415c, "__stack_chk_fail"},
    {0x00104181, "__stack_chk_fail_local"},
    {0x00104196, "strstr"},
    {0x001041f6, "strcspn"},
    {0x00104257, "WriteTSS"},
    {0x00104311, "SetGateGDT"},
    {0x001043a9, "InitializeGDT"},
    {0x0010445b, "SetKernelStack"},
    {0x00104474, "cmos_get_register"},
    {0x001044b0, "cmos_get_update_flag"},
    {0x001044e6, "SetControlWordFPU"},
    {0x00104503, "EnableFPU"},
    {0x00104550, "cpu_get_brand_string"},
    {0x00104637, "InterruptsEnabled"},
    {0x00104659, "InterruptsRelease"},
    {0x0010466a, "InterruptsLock"},
    {0x0010467b, "outb"},
    {0x001046a4, "outw"},
    {0x001046cf, "outl"},
    {0x001046f1, "inb"},
    {0x00104718, "inw"},
    {0x00104741, "inl"},
    {0x00104767, "RegisterInterrupt"},
    {0x0010478d, "IRQHandlerISR"},
    {0x001047e9, "DumpRegisters"},
    {0x001048a4, "ExceptionHandlerISR"},
    {0x00104976, "SetGateIDT"},
    {0x001049e2, "InitializeIDT"},
    {0x00104ec2, "InitializePIC"},
    {0x00104fba, "EndOfInterruptPIC"},
    {0x00104fff, "divide_by_zero_handler"},
    {0x00105025, "gpf_handler"},
    {0x0010504b, "invalid_opcode_handler"},
    {0x00105071, "install_exception_handlers"},
    {0x001050c5, "validate_checksum"},
    {0x0010511a, "xsdp_detect"},
    {0x001051eb, "findFACP"},
    {0x0010527f, "acpi_init"},
    {0x00105383, "__get_cpuid_max"},
    {0x001053f5, "__get_cpuid"},
    {0x0010546a, "check_apic"},
    {0x0010549e, "initialize_apic"},
    {0x001054ef, "InitializeMemoryManagement"},
    {0x001055b0, "pmalloc"},
    {0x0010560b, "pfree"},
    {0x00105660, "malloc"},
    {0x001057eb, "free"},
    {0x00105824, "__page_irq"},
    {0x0010591b, "load_page_directory"},
    {0x00105947, "__tlb_flush"},
    {0x0010595d, "paging_init"},
    {0x00105ae6, "paging_map_page"},
    {0x00105bd4, "PANIC"},
    {0x00105c26, "SetFrequencyTimer"},
    {0x00105cbb, "TimerHandler"},
    {0x00105d00, "get_uptime_seconds"},
    {0x00105d21, "get_uptime_milliseconds"},
    {0x00105d36, "timer_sleep_ms"},
    {0x00105d66, "InitializeTimer"},
    {0x00105da2, "elf_is_valid"},
    {0x00105df1, "elf_print_info"},
    {0x00105f00, "elf_execute"},
    {0x00105fd8, "register_devices"},
    {0x0010611f, "bss_init"},
    {0x0010614e, "KernelMain"},
    {0x001062e2, "KernelMainStage2"},
    {0x00106390, "get_cmdline"},
    {0x001063e4, "parse_cmdline"},
    {0x00106540, "__printm"},
    {0x00106593, "init_mem"},
    {0x001065c2, "arch_init"},
    {0x001065ee, "InitializeHALStage1"},
    {0x00106659, "printk"},
    {0x0010668a, "trace_address"},
    {0x001066e5, "StackTrace"},
    {0x00106781, "mboot_info_verify"},
    {0x001067b5, "devfs_get_device_byname"},
    {0x0010688a, "devfs_get_registered_devices"},
    {0x001068d4, "devfs_get_device_node"},
    {0x001069a5, "devfs_read_dir"},
    {0x00106bc4, "devfs_create_device"},
    {0x00106cb9, "devfs_get_free_node_id"},
    {0x00106cff, "devfs_unregister"},
    {0x00106d9b, "devfs_node2dev"},
    {0x00106e9b, "devfs_register"},
    {0x00107091, "devfs_path2node"},
    {0x001071b6, "devfs_open"},
    {0x00107277, "devfs_read"},
    {0x00107370, "devfs_write"},
    {0x001074bd, "devfs_init"},
    {0x001075ba, "devfs_mount"},
    {0x001075ed, "vfs_read"},
    {0x0010762e, "vfs_write"},
    {0x0010766f, "vfs_open"},
    {0x001076b9, "vfs_close"},
    {0x001076ed, "vfs_register_filesystem"},
    {0x0010776b, "find_fd"},
    {0x001077d4, "vfs_get_fs"},
    {0x001078be, "ext2_is_valid"},
    {0x00107929, "ext2_read_superblock"},
    {0x00107981, "ext2_read_inode"},
    {0x00107a99, "ext2_list_dir"},
    {0x00107b9c, "ext2_read_dir"},
    {0x00107c99, "ext2_inode2vfs_node"},
    {0x00107dec, "ext2_find_file"},
    {0x00108103, "ext2_get_file"},
    {0x00108189, "ext2_read_slink"},
    {0x0010823c, "ext2_read_dlink"},
    {0x001082fb, "ext2_read"},
    {0x00108496, "ext2_mount"},
    {0x00108621, "ext2_block_to_sector"},
    {0x00108648, "ext2_read_block"},
    {0x0010868a, "ext2_block_allocate"},
    {0x001086b7, "ext2_block_free"},
    {0x001086de, "ext2_inode_allocate"},
    {0x0010870f, "ext2_inode_free"},
    {0x00108736, "ext2_get_block_group_inode"},
    {0x00108761, "ext2_get_index_inode"},
    {0x0010878a, "ext2_get_block_inode"},
  {0xffffffff, ""}
};
