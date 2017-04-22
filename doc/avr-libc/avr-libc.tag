<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>assert.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/</path>
    <filename>assert_8h</filename>
    <includes id="stdlib_8h" name="stdlib.h" local="no" imported="no">stdlib.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>assert</name>
      <anchorfile>group__avr__assert.html</anchorfile>
      <anchor>ga0041af519e0e7d47c9bcc83760c4669e</anchor>
      <arglist>(expression)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>atomic.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/util/</path>
    <filename>atomic_8h</filename>
    <includes id="io_8h" name="io.h" local="no" imported="no">avr/io.h</includes>
    <includes id="interrupt_8h" name="interrupt.h" local="no" imported="no">avr/interrupt.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>ATOMIC_BLOCK</name>
      <anchorfile>group__util__atomic.html</anchorfile>
      <anchor>gaaaea265b31dabcfb3098bec7685c39e4</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ATOMIC_FORCEON</name>
      <anchorfile>group__util__atomic.html</anchorfile>
      <anchor>ga92b11103b4b3b000a3190f0d26ba7062</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ATOMIC_RESTORESTATE</name>
      <anchorfile>group__util__atomic.html</anchorfile>
      <anchor>ga362c18b15a09703e42e1c246c47420ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NONATOMIC_BLOCK</name>
      <anchorfile>group__util__atomic.html</anchorfile>
      <anchor>ga6e195ee2117559a25f77fbba9054674a</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NONATOMIC_FORCEOFF</name>
      <anchorfile>group__util__atomic.html</anchorfile>
      <anchor>gafb959d7d00d2d790b58d0e9880ea255a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NONATOMIC_RESTORESTATE</name>
      <anchorfile>group__util__atomic.html</anchorfile>
      <anchor>gab075653bf638fae9db049575741d3152</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>boot.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/avr/</path>
    <filename>boot_8h</filename>
    <includes id="io_8h" name="io.h" local="no" imported="no">avr/io.h</includes>
    <includes id="inttypes_8h" name="inttypes.h" local="no" imported="no">inttypes.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>boot_is_spm_interrupt</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gacd6843d51a4fa7d68f3c4bc5aa393a3e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_lock_bits_set</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gadaa352690ac1c82c535da9d8faf77590</anchor>
      <arglist>(lock_bits)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_lock_bits_set_safe</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gac80bd4f863ff7645204adb0eda3556ea</anchor>
      <arglist>(lock_bits)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_lock_fuse_bits_get</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gad2cbdea59ffec2e77ee2e63106459797</anchor>
      <arglist>(address)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_page_erase</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga7249d12e06789cd306583abf7def8176</anchor>
      <arglist>(address)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_page_erase_safe</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga5c345350a2fb349619b859dcf0dc0466</anchor>
      <arglist>(address)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_page_fill</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga61add108c778273ff68233d044c3acca</anchor>
      <arglist>(address, data)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_page_fill_safe</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga3f549e2ef9ca0f119a7070064c4dc372</anchor>
      <arglist>(address, data)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_page_write</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga013d6d8c679ebdbc0e5fac179c38c9aa</anchor>
      <arglist>(address)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_page_write_safe</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga107ad4c789e5c498062947d394ed98d5</anchor>
      <arglist>(address)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_rww_busy</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gaaba5a29359af671a37bccaa6a77bca7c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_rww_enable</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga8d2baaca2991318e0b06fdf9a5264923</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_rww_enable_safe</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gaeb0dba1dd9d338516a94c0bd8a8db78a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_signature_byte_get</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gaf375d2543ba38dc56697b4f4bc37a717</anchor>
      <arglist>(addr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_spm_busy</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gae50dc01180110f30093a92fe64fe1565</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_spm_busy_wait</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga24900c15109e8b419736d4b81b093fb8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_spm_interrupt_disable</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gaf1e01d09c839793a6c0c990b1c5ae8b3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_spm_interrupt_enable</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga072040fce54f112355cb9ebf9ce41dd7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BOOTLOADER_SECTION</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga8a60eb0985d40ff71c42bb18f0f5789e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GET_EXTENDED_FUSE_BITS</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gaf08aabaebbd69da659357f402d4d28ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GET_HIGH_FUSE_BITS</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga44e70c9662e7ac06484144c15bc69aea</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GET_LOCK_BITS</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gae12d288a22cfbfa9d0cde12b1a779bfe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GET_LOW_FUSE_BITS</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gac68c37ecf2354ba2af6e08379d65899f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>cpufunc.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/avr/</path>
    <filename>cpufunc_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>_MemoryBarrier</name>
      <anchorfile>group__avr__cpufunc.html</anchorfile>
      <anchor>ga1060ab52075a71b61c88636e204321b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_NOP</name>
      <anchorfile>group__avr__cpufunc.html</anchorfile>
      <anchor>ga46388d9db8422abfea56ae2323f7a77c</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>crc16.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/util/</path>
    <filename>crc16_8h</filename>
    <includes id="stdint_8h" name="stdint.h" local="no" imported="no">stdint.h</includes>
    <member kind="function" static="yes">
      <type>static __inline__ uint16_t</type>
      <name>_crc16_update</name>
      <anchorfile>group__util__crc.html</anchorfile>
      <anchor>ga95371c87f25b0a2497d9cba13190847f</anchor>
      <arglist>(uint16_t __crc, uint8_t __data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __inline__ uint8_t</type>
      <name>_crc8_ccitt_update</name>
      <anchorfile>group__util__crc.html</anchorfile>
      <anchor>gab27eaaef6d7fd096bd7d57bf3f9ba083</anchor>
      <arglist>(uint8_t __crc, uint8_t __data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __inline__ uint16_t</type>
      <name>_crc_ccitt_update</name>
      <anchorfile>group__util__crc.html</anchorfile>
      <anchor>ga1c1d3ad875310cbc58000e24d981ad20</anchor>
      <arglist>(uint16_t __crc, uint8_t __data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __inline__ uint8_t</type>
      <name>_crc_ibutton_update</name>
      <anchorfile>group__util__crc.html</anchorfile>
      <anchor>ga37b2f691ebbd917e36e40b096f78d996</anchor>
      <arglist>(uint8_t __crc, uint8_t __data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __inline__ uint16_t</type>
      <name>_crc_xmodem_update</name>
      <anchorfile>group__util__crc.html</anchorfile>
      <anchor>gaca726c22a1900f9bad52594c8846115f</anchor>
      <arglist>(uint16_t __crc, uint8_t __data)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>ctype.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/</path>
    <filename>ctype_8h</filename>
    <member kind="function">
      <type>int</type>
      <name>isalnum</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gae62f8f9e20430c822d07697b277c5de7</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isalpha</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga66e77eea7376ce0c1e00223f45d0aba9</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isascii</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga41f2d44607ddcc15266bd63cb8e563c4</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isblank</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gad1a49aed816573f1b5787176f5ec701e</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>iscntrl</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga659606df93739b0193c9e98df2953ae0</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isdigit</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga49b21b266576c2490dab1e8f897c801a</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isgraph</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gae89269f754744bbd5486788d06e0c7c9</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>islower</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gae293e218972b626f6304aecbae044ef6</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isprint</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gaa2ded03e3a0d8f5ef2a8df3e834e916d</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ispunct</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gaa46e128f7550afb4f2cda133594cb4b6</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isspace</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga16e58710aba8108549a1a3c33691b24e</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isupper</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga125dbbabac1caef46a0c6dad060a79d1</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isxdigit</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga04826722a22243889683efde883e8ab3</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>toascii</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga21d5ec3792b2704ecca5778b758dd91f</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tolower</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga9ba1fce7148e9b63ca6296e02c79bedd</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>toupper</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga924ed052807e23cfa160d5f171cf5e2a</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isalnum</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gae62f8f9e20430c822d07697b277c5de7</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isalpha</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga66e77eea7376ce0c1e00223f45d0aba9</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isascii</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga41f2d44607ddcc15266bd63cb8e563c4</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isblank</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gad1a49aed816573f1b5787176f5ec701e</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>iscntrl</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga659606df93739b0193c9e98df2953ae0</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isdigit</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga49b21b266576c2490dab1e8f897c801a</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isgraph</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gae89269f754744bbd5486788d06e0c7c9</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>islower</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gae293e218972b626f6304aecbae044ef6</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isprint</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gaa2ded03e3a0d8f5ef2a8df3e834e916d</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ispunct</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gaa46e128f7550afb4f2cda133594cb4b6</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isspace</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga16e58710aba8108549a1a3c33691b24e</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isupper</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga125dbbabac1caef46a0c6dad060a79d1</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isxdigit</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga04826722a22243889683efde883e8ab3</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>toascii</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga21d5ec3792b2704ecca5778b758dd91f</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tolower</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga9ba1fce7148e9b63ca6296e02c79bedd</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>toupper</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga924ed052807e23cfa160d5f171cf5e2a</anchor>
      <arglist>(int __c)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>delay.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/util/</path>
    <filename>delay_8h</filename>
    <includes id="inttypes_8h" name="inttypes.h" local="no" imported="no">inttypes.h</includes>
    <includes id="delay__basic_8h" name="delay_basic.h" local="no" imported="no">util/delay_basic.h</includes>
    <includes id="math_8h" name="math.h" local="no" imported="no">math.h</includes>
    <member kind="function">
      <type>void</type>
      <name>_delay_ms</name>
      <anchorfile>group__util__delay.html</anchorfile>
      <anchor>gad22e7a36b80e2f917324dc43a425e9d3</anchor>
      <arglist>(double __ms)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_delay_us</name>
      <anchorfile>group__util__delay.html</anchorfile>
      <anchor>gab20bfffeacc678cb960944f5519c0c4f</anchor>
      <arglist>(double __us)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>delay_basic.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/util/</path>
    <filename>delay__basic_8h</filename>
    <includes id="inttypes_8h" name="inttypes.h" local="no" imported="no">inttypes.h</includes>
    <member kind="function">
      <type>void</type>
      <name>_delay_loop_1</name>
      <anchorfile>group__util__delay__basic.html</anchorfile>
      <anchor>ga4e3957917c4c447d0f9166dac881b4e3</anchor>
      <arglist>(uint8_t __count)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_delay_loop_2</name>
      <anchorfile>group__util__delay__basic.html</anchorfile>
      <anchor>ga74a94fec42bac9f1ff31fd443d419a6a</anchor>
      <arglist>(uint16_t __count)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>errno.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/</path>
    <filename>errno_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>EDOM</name>
      <anchorfile>group__avr__errno.html</anchorfile>
      <anchor>ga5fe247e079b591a68e0fdbf7caec5b70</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERANGE</name>
      <anchorfile>group__avr__errno.html</anchorfile>
      <anchor>gaa1591a4f3a86360108de5b9ba34980ca</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fdevopen.c</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/libc/stdio/</path>
    <filename>fdevopen_8c</filename>
    <includes id="stdint_8h" name="stdint.h" local="no" imported="no">stdint.h</includes>
    <includes id="stdio_8h" name="stdio.h" local="no" imported="no">stdio.h</includes>
    <includes id="stdlib_8h" name="stdlib.h" local="no" imported="no">stdlib.h</includes>
    <member kind="function">
      <type>FILE *</type>
      <name>fdevopen</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gab599ddf60819df4cc993c724a83cb1a4</anchor>
      <arglist>(int(*put)(char, FILE *), int(*get)(FILE *))</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>fuse.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/avr/</path>
    <filename>fuse_8h</filename>
  </compound>
  <compound kind="file">
    <name>interrupt.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/avr/</path>
    <filename>interrupt_8h</filename>
    <includes id="io_8h" name="io.h" local="no" imported="no">avr/io.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>sei</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>gaad5ebd34cb344c26ac87594f79b06b73</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>cli</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga68c330e94fe121eba993e5a5973c3162</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>gad28590624d422cdf30d626e0a506255f</anchor>
      <arglist>(vector, attributes)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIGNAL</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga67cd0dea412157775c2f2a3ffe9fb8ff</anchor>
      <arglist>(vector)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EMPTY_INTERRUPT</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga751c22101f7e8f2fbe792c64a81f8dba</anchor>
      <arglist>(vector)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_ALIAS</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>gade46eb4e42cc9d56c19b1f91448f1b76</anchor>
      <arglist>(vector, target_vector)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>reti</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga3b991e8168db8fc866e31f9a6d10533b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BADISR_vect</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga1f6459a85cda682b2163a20af03ac744</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_BLOCK</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga5fc50a0507a58e16aca4c70345ddac6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_NOBLOCK</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga44569cb914d2aaf8fbb436f8f7c4ca68</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_NAKED</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga8b4c7e44627db0a60d676213add42d64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_ALIASOF</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>gaa87c0c624b62f40a17539be6946c3e26</anchor>
      <arglist>(target_vector)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>sei</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>gaad5ebd34cb344c26ac87594f79b06b73</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>cli</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga68c330e94fe121eba993e5a5973c3162</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>gad28590624d422cdf30d626e0a506255f</anchor>
      <arglist>(vector, attributes)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIGNAL</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga67cd0dea412157775c2f2a3ffe9fb8ff</anchor>
      <arglist>(vector)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EMPTY_INTERRUPT</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga751c22101f7e8f2fbe792c64a81f8dba</anchor>
      <arglist>(vector)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_ALIAS</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>gade46eb4e42cc9d56c19b1f91448f1b76</anchor>
      <arglist>(vector, target_vector)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>reti</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga3b991e8168db8fc866e31f9a6d10533b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BADISR_vect</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga1f6459a85cda682b2163a20af03ac744</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_BLOCK</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga5fc50a0507a58e16aca4c70345ddac6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_NOBLOCK</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga44569cb914d2aaf8fbb436f8f7c4ca68</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_NAKED</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga8b4c7e44627db0a60d676213add42d64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_ALIASOF</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>gaa87c0c624b62f40a17539be6946c3e26</anchor>
      <arglist>(target_vector)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>inttypes.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/</path>
    <filename>inttypes_8h</filename>
    <includes id="stdint_8h" name="stdint.h" local="no" imported="no">stdint.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>PRId8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae53c45f590033ad1f2f517faf3ab2f1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga404fd01f0b890cb8fac8641aaa704b57</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga943961b7e7e564388dd743593db5bbbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIi8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gadbe02b78cca747b2fe1a8f7fc5f5cd47</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga526151b1725956030b501d9dd506f2e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga64fb4e44c3ff09179fc445979b7fdad1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRId16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga087e50fe0283aacc71d7138d13e91939</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae90ab00cb4417081dc68e9fd6c0e129a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga58cdfb02574b8c23d964a6e88a268782</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIi16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga655e9b358e0371a4bf5ff21cc08273e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga96945864cb2d1f7de861ccaf639af02e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gac273fb2a05215962fbeae76abaaf0131</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRId32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6d94d1417e1b35c53aee6306590de72b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gad36a6b276bd808d713cc5603ba008c58</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaef5a98227a6af5fde95353ed303cfd1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIi32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae212e57631ec729f70e0cc42e51dd91e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gad7a1bae7ca12c7b5415fae1b3f258207</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga192a69a2e6e63ed8393d306b4078d63f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7c8a9ccd40bd2053ca588d1b15e76a30</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gac2d52bf83b783f530f02fa2eeabe703a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIo8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gad12493b9063f7b2630b90b7f9a7f3301</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa5b3ca8091f4ed7d43f5eb971ce11114</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga37f93445f1795033c9ba577661da6a91</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIu8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga8673208d2d48018fcce020ef59f8ec4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga74cb15b101649124009c010a9055e885</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga0b0c7ad693c391e3e353e8f2d1df2ec3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIx8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gadac1acc1d24060aeee7791a99d1a3a8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga45d80a42b6cd25f3ed57b0e800e6e398</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae7e1780719eb0e4b2826a0da06255780</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIX8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4e9b835c85ffa875e8304e2b852b4c86</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga70aa3faf72084587fb18d03aa033a212</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gab153efc9e6547ca56f42de767cde2595</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIo16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga55494a16151668ea78e0b808ef38c8c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1ecbd31333b358c22423a541fffbd122</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga3eda49c829de683e701eaed3cbaf0e73</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIu16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga86bc00ee87e8e40787e0681fc34c576a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa3ba696eef7c107c76c26eea76dcb4b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa82e218a186691ebf7149b36746c12e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIx16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga70f5e38b517f714518c970a4da37bef1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gad00e2a12b813425800cad731f61497ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6f66e34285ab57a86aeb2f0f4895417d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIX16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga570ca9af5087023f75fc8a1a602d26ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gafa4303b077ae4c6c58686178e4b90d18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga785eabe6337a2fa85874ae99300abb66</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIo32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7276f64276fd7223ca6f4cca0444239a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1e5c50a1ca71da7ff8c4f3f007411be8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6ac7e3111d008785ddf3b29dcd088732</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIu32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaaf2af4a10f0bd308e9c349c8382382be</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaab353a2898377162c1829f1a9708352e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaccc383115328197264988682edfcb72c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIx32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga80ca66bcc9e366733f02c90ed4b0838c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1d766603a3524c9e03effbbece9c2118</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga22caa684d44725e1e6e638983380f68e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIX32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga32b0c8a04aae5d4454d15e6cbe109f64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaaf100a10f9cd73d46294fd0e8db5246d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gace7057a6fa96ac7e2a05946ee96cf2d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1468793ce960b477922ef92b36a6c802</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa1ca3a85113e897b5cf7ed6b92d74de2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga9c3c25e6145e629e4c9fabddc6061c30</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga65d9856517198cfc21558c0d6df64207</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNd8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gabf98c3a9ad120b11ec2911b9398e3f2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gab0af8c396d9c885950d423f8dee54164</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6dc7d2f030d25e79ae8398088161b860</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNi8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga535485ea35661ff75a8d2bc0d2ebe807</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1a0b88bf6f131db927f2e7f1f6abb644</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gac864120101e01707ca52c0976b4e539a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNd16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga35974d44b5dcebcb222b8e2c1384241d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga10db5de9c84ccfa6dc0e487dd72051f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga09c9f36f654aa50a548d7820421cdc57</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNi16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7b8508989273ad152f9b3b7cd4db6eee</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga14ec2649667b53ff91a1103c02975837</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaad333b5bea32321b312a3b4967ff357f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNd32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga2b7ab77ff6ede9c3c285b714496f77e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae36c293972a5b770349d74f2c0cfa52f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gadd733be35bef9dcef225edc99ade9e33</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNi32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga52cfc41a1e5ad73788faebbfeb9c14b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga39be8ffb41be80bc951e955f111e4121</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4739f89fa519cd77097677bf33320091</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gabf657ee6bd4b009b5b072840a3d7364f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga9c632ab51b24b93cc315b27a883be9eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNo8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4e274a339187359a91963d22f8e6faa6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga873157069430be3ab2cade457e92f187</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga9716b5135de22733c9c59bc4fc0e3a66</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNu8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae0d5458bfaf4c45083b1e92013d77f51</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae409b3af282bc394819a5dd289cdf57c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga01b368195aa26130d44bf0efe07833fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNx8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga79b1f201c12273510e1fdebfb3a66e9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga5cac5341d60e594c818c0f9d25377928</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga251936e4d698e68846c0917270b5f8a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNo16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga9bc6b517c0117327e832824ff2d6a6b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga5b05c70b4807922992a9ca529361b44d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga021e130b06fc46198c71dca0fdf89788</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNu16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga37bbde0e3f124b7f482d54adb13b0248</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7a78b92618044bb2d798b57fc6a2e439</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7cf58abc57bb03d809e6fc41c2a40c33</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNx16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga12dbc2ac6a36b893ef1c25c357f90a9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga24647dd309d4138846376a51a6098304</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga8b67140c216180e4e5d18003038ee689</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNo32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gab561c947d62a3c7cd396d4aeef553f3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6b324310e03b8ecbe6888a52b7d8581d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae40f8b90cb75998e70910e7b377288a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNu32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gabd19a83130f8d1bd2f77b765ad804f75</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae30d5cc7dbc15051e21b72229a2487f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4ce14b7ebee0cfd5c4c935cf79a9a504</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNx32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4c5370556f793ac7b2c3abe896dba8e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gabd82b99090a28a84541959ac7ab14ad9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gac45f394be3c199938a85a631711ce22e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4a30d36e06018d8e13046079098905a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gab7dbf5d0ea41679dface5855896e4273</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa58d290d968643862aec7a8a56e1c8e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t</type>
      <name>int_farptr_t</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga5082b177b7d7b2039652c26a72b96d18</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>uint_farptr_t</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga72b6692e3f3123903c1a0d9a960c59b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t</type>
      <name>int_farptr_t</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga5082b177b7d7b2039652c26a72b96d18</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>uint_farptr_t</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga72b6692e3f3123903c1a0d9a960c59b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRId8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae53c45f590033ad1f2f517faf3ab2f1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga404fd01f0b890cb8fac8641aaa704b57</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga943961b7e7e564388dd743593db5bbbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIi8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gadbe02b78cca747b2fe1a8f7fc5f5cd47</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga526151b1725956030b501d9dd506f2e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga64fb4e44c3ff09179fc445979b7fdad1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRId16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga087e50fe0283aacc71d7138d13e91939</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae90ab00cb4417081dc68e9fd6c0e129a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga58cdfb02574b8c23d964a6e88a268782</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIi16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga655e9b358e0371a4bf5ff21cc08273e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga96945864cb2d1f7de861ccaf639af02e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gac273fb2a05215962fbeae76abaaf0131</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRId32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6d94d1417e1b35c53aee6306590de72b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gad36a6b276bd808d713cc5603ba008c58</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaef5a98227a6af5fde95353ed303cfd1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIi32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae212e57631ec729f70e0cc42e51dd91e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gad7a1bae7ca12c7b5415fae1b3f258207</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga192a69a2e6e63ed8393d306b4078d63f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7c8a9ccd40bd2053ca588d1b15e76a30</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gac2d52bf83b783f530f02fa2eeabe703a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIo8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gad12493b9063f7b2630b90b7f9a7f3301</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa5b3ca8091f4ed7d43f5eb971ce11114</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga37f93445f1795033c9ba577661da6a91</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIu8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga8673208d2d48018fcce020ef59f8ec4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga74cb15b101649124009c010a9055e885</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga0b0c7ad693c391e3e353e8f2d1df2ec3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIx8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gadac1acc1d24060aeee7791a99d1a3a8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga45d80a42b6cd25f3ed57b0e800e6e398</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae7e1780719eb0e4b2826a0da06255780</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIX8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4e9b835c85ffa875e8304e2b852b4c86</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga70aa3faf72084587fb18d03aa033a212</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gab153efc9e6547ca56f42de767cde2595</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIo16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga55494a16151668ea78e0b808ef38c8c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1ecbd31333b358c22423a541fffbd122</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga3eda49c829de683e701eaed3cbaf0e73</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIu16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga86bc00ee87e8e40787e0681fc34c576a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa3ba696eef7c107c76c26eea76dcb4b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa82e218a186691ebf7149b36746c12e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIx16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga70f5e38b517f714518c970a4da37bef1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gad00e2a12b813425800cad731f61497ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6f66e34285ab57a86aeb2f0f4895417d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIX16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga570ca9af5087023f75fc8a1a602d26ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gafa4303b077ae4c6c58686178e4b90d18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga785eabe6337a2fa85874ae99300abb66</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIo32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7276f64276fd7223ca6f4cca0444239a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1e5c50a1ca71da7ff8c4f3f007411be8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6ac7e3111d008785ddf3b29dcd088732</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIu32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaaf2af4a10f0bd308e9c349c8382382be</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaab353a2898377162c1829f1a9708352e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaccc383115328197264988682edfcb72c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIx32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga80ca66bcc9e366733f02c90ed4b0838c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1d766603a3524c9e03effbbece9c2118</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga22caa684d44725e1e6e638983380f68e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIX32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga32b0c8a04aae5d4454d15e6cbe109f64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaaf100a10f9cd73d46294fd0e8db5246d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gace7057a6fa96ac7e2a05946ee96cf2d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1468793ce960b477922ef92b36a6c802</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa1ca3a85113e897b5cf7ed6b92d74de2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga9c3c25e6145e629e4c9fabddc6061c30</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga65d9856517198cfc21558c0d6df64207</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNd8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gabf98c3a9ad120b11ec2911b9398e3f2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gab0af8c396d9c885950d423f8dee54164</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6dc7d2f030d25e79ae8398088161b860</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNi8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga535485ea35661ff75a8d2bc0d2ebe807</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1a0b88bf6f131db927f2e7f1f6abb644</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gac864120101e01707ca52c0976b4e539a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNd16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga35974d44b5dcebcb222b8e2c1384241d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga10db5de9c84ccfa6dc0e487dd72051f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga09c9f36f654aa50a548d7820421cdc57</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNi16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7b8508989273ad152f9b3b7cd4db6eee</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga14ec2649667b53ff91a1103c02975837</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaad333b5bea32321b312a3b4967ff357f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNd32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga2b7ab77ff6ede9c3c285b714496f77e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae36c293972a5b770349d74f2c0cfa52f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gadd733be35bef9dcef225edc99ade9e33</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNi32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga52cfc41a1e5ad73788faebbfeb9c14b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga39be8ffb41be80bc951e955f111e4121</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4739f89fa519cd77097677bf33320091</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gabf657ee6bd4b009b5b072840a3d7364f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga9c632ab51b24b93cc315b27a883be9eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNo8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4e274a339187359a91963d22f8e6faa6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga873157069430be3ab2cade457e92f187</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga9716b5135de22733c9c59bc4fc0e3a66</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNu8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae0d5458bfaf4c45083b1e92013d77f51</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae409b3af282bc394819a5dd289cdf57c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga01b368195aa26130d44bf0efe07833fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNx8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga79b1f201c12273510e1fdebfb3a66e9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga5cac5341d60e594c818c0f9d25377928</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga251936e4d698e68846c0917270b5f8a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNo16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga9bc6b517c0117327e832824ff2d6a6b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga5b05c70b4807922992a9ca529361b44d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga021e130b06fc46198c71dca0fdf89788</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNu16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga37bbde0e3f124b7f482d54adb13b0248</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7a78b92618044bb2d798b57fc6a2e439</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7cf58abc57bb03d809e6fc41c2a40c33</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNx16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga12dbc2ac6a36b893ef1c25c357f90a9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga24647dd309d4138846376a51a6098304</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga8b67140c216180e4e5d18003038ee689</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNo32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gab561c947d62a3c7cd396d4aeef553f3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6b324310e03b8ecbe6888a52b7d8581d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae40f8b90cb75998e70910e7b377288a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNu32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gabd19a83130f8d1bd2f77b765ad804f75</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae30d5cc7dbc15051e21b72229a2487f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4ce14b7ebee0cfd5c4c935cf79a9a504</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNx32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4c5370556f793ac7b2c3abe896dba8e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gabd82b99090a28a84541959ac7ab14ad9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gac45f394be3c199938a85a631711ce22e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4a30d36e06018d8e13046079098905a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gab7dbf5d0ea41679dface5855896e4273</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa58d290d968643862aec7a8a56e1c8e9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>io.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/avr/</path>
    <filename>io_8h</filename>
    <includes id="fuse_8h" name="fuse.h" local="no" imported="no">avr/fuse.h</includes>
    <includes id="lock_8h" name="lock.h" local="no" imported="no">avr/lock.h</includes>
  </compound>
  <compound kind="file">
    <name>lock.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/avr/</path>
    <filename>lock_8h</filename>
  </compound>
  <compound kind="file">
    <name>math.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/</path>
    <filename>math_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>acosf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga3e085dcc8e09f400136816dbc2a56a25</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>asinf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga68e4969cdd396dc3d4ad5abef13880b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>atan2f</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gae31ff8b5990823ebb5d9746cc0843293</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>atanf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga629de3dc6ca25cb7195928581f5359d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>cbrtf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gad724f06c46a6f4ad699fece14f219552</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ceilf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gadb970de70c0549110ae218f6e244c871</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>copysignf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga1f0a9e760e0cd6faaf9619fd3aebfc3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>cosf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gacc171f0e087ed26abd701c827944a43b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>coshf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga218c068c47d79563f3fdbcf0ecb45220</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>expf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga1afc25e5db4387a8480d7cc4ed8d2e6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fabsf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga3745ca949b1ef9df47fb2f52c2536e68</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fdimf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga1305189e3aa1637d4018365c1c08698b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>floorf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga322df600e1eaa1b96b0f994dcd49316f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fmaf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga49c61eb85541c0504867fb05e3598679</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fmaxf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga645ac7eb8b4f0d7e09bcdcadc36044d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fminf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaa591e76a6dddd36f3b745dbf8c8b90a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fmodf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga7f41f3c7a21457008063af86e4eded3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>frexpf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga23413d0a6d19da20534e53f9870cf588</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>hypotf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaae0bb7cd216d5b824d90da083e67021d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INFINITY</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga956e2723d559858d08644ac99146e910</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>isfinitef</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga9e007830d194ee4b5f0a27d2112a0c53</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>isinff</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaf048a8eca1cce97556a96c532c0d2b56</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>isnanf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaf3c3e624db1ecfe93b86972bca1dc4b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ldexpf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga374c9d6918a8bc1f8bbe054940795e0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>log10f</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaf356651f154a73fef76067055cc54c16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>logf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaccce424ce6effa1bfd476479d55dde9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>lrintf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaaf59e045a8a86a9be3370d69dec98a25</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>lroundf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga9031359ea4116de9288526c9e75fd8c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_1_PI</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga08dfac3cca9601a02fc88356cc078e1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_2_PI</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga97f6d6514d3d3dd50c3a2a6d622673db</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_2_SQRTPI</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga631ff334c4a1a6db2e8a7ff4acbe48a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_E</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga9bf5d952c5c93c70f9e66c9794d406c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_LN10</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga0a53871497a155afe91424c28a8ec3c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_LN2</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga92428112a5d24721208748774a4f23e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_LOG10E</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga9ed2b5582226f3896424ff6d2a3ebb14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_LOG2E</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gac5c747ee5bcbe892875672a0b9d8171c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_PI</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gae71449b1cc6e6250b91f539153a7a0d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_PI_2</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga958e4508ed28ee5cc04249144312c15f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_PI_4</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaeb24420b096a677f3a2dc5a72b36bf22</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_SQRT1_2</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gacdbb2c2f9429f08916f03c8786d2d2d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_SQRT2</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga66b3ab30f1332874326ed93969e496e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NAN</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga8abfcc76130f3f991d124dd22d7e69bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>powf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaa5a93b1934ef470b7a44ae47e6f2798b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>roundf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gac6950642117c821388ec37e7b656a346</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>signbitf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gacff7dd1db8fe93d3e52d5bd161d8117d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>sinf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga6776b8d1a4149fe35e282a76effd8a9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>sinhf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gadd3c4414cb88e4071380d6b1d6e69dcd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>squaref</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gadd592f5a4371f7154f95df59a22f5bef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tanf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga841e8ff746ff8b099a00ee83d2eed1d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tanhf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gae1308938bb768033031c381d148ad8f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>truncf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga329f9e84df43e59c5999e661aeab14ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>acos</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gae9c5790d8a29cbee8f54f8eb522decbc</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>asin</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga98384ad60834911ec93ac5ae1af4cf0a</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>atan</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga3abd1a0b68d157914a0ee01acaedfe5e</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>atan2</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga054230cd7e4c12958dbfac75ab6886e5</anchor>
      <arglist>(double __y, double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>cbrt</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga9dff6efc5e63405ba23afb75eb3e4af0</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>ceil</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga61470611f23ceef5d3e9cf63d84cd8a7</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>copysign</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaaebf29a8e50e6d8f88b6caf697021c86</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>cos</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga542f5e42e0d3b5df63de0e34ec06bb40</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>cosh</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga2ec1caf3ba3b1ba62eccb3eddf029438</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>exp</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga4ea549372745dda4018ab4b5a94483a6</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>fabs</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga0838a00d86d5e920c80aa7c7532a5559</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>fdim</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga63fcb86a13cf17110582fd6560fb09dd</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>floor</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga0f0bf9ac2651b80846a9d9d89bd4cb85</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>fma</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaf7fbeddc2e7721682e69f59be04ae9d5</anchor>
      <arglist>(double __x, double __y, double __z)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>fmax</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gafd8ab12e9d230e8213ac0c8b77c306d9</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>fmin</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga577eee92b9969942de4d8d134d37c7b8</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>fmod</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaefa8edb8c13adf7fb4b6b7dbe7261a24</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>frexp</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga89f0cb053e3cdb0c9c952ef040087c80</anchor>
      <arglist>(double __x, int *__pexp)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>hypot</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga711412ca8746712e0f19508118bf5154</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>isfinite</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gafb997a6a8d6dd0498238098826998aa0</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isinf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga18a7409e0b2341afaa41993960961772</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isnan</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga468be9ed380771eca5a18e812b2470d4</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>ldexp</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga91643e944a94341bd2a3ed1d3ffbae4f</anchor>
      <arglist>(double __x, int __exp)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>log</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga7f7d556ab6b6235777a179647c152126</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>log10</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga3630cb8cef4560cf0d92e82ae05b03f0</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>lrint</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga9b995838b7bdd4886549dd7e308d0619</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>lround</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaa759c9a1684b0cf2c4c5d133771192ce</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>modf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga85861fee92c0904e9bb7a9875ee77579</anchor>
      <arglist>(double __x, double *__iptr)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>modff</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gad180f98cb1dac86425c2f4df7faf2a15</anchor>
      <arglist>(float __x, float *__iptr)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>pow</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga41b41c307b8f96760e9c0c17180b241b</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>round</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga6eb04604d801054c5a2afe195d1dd75d</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>signbit</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga36894d73110fda52c84dfc050f1004bb</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>sin</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga46e799ece7e1dd323d22cdb53c81cd73</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>sinh</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaae45da8f56ba936dc8624157d1770faa</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>sqrt</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gadd1e40e370b2165c186f72ec631b1853</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>square</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gad11825ff1d6095a08b193c564d524aef</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>tan</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaa2c2303658b8b2555bc97cce3f806bda</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>tanh</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaf4b72825a245b794090135251f0ead22</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>trunc</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga1883497d16352bd92875499f1b39a4b6</anchor>
      <arglist>(double __x)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>parity.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/util/</path>
    <filename>parity_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>parity_even_bit</name>
      <anchorfile>group__util__parity.html</anchorfile>
      <anchor>ga4180eaa9b8f27f8efc589f3a3ba1724c</anchor>
      <arglist>(val)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>pgmspace.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/avr/</path>
    <filename>pgmspace_8h</filename>
    <includes id="inttypes_8h" name="inttypes.h" local="no" imported="no">inttypes.h</includes>
    <includes id="io_8h" name="io.h" local="no" imported="no">avr/io.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>PGM_P</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga963f816fc88a5d8479c285ed4c630229</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_byte</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga73084a8bbde259ffae72980354b3f027</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_byte_far</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga0d4d7f6231716747c52b969d4febdeed</anchor>
      <arglist>(address_long)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_byte_near</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga88d7dd4863f87530e1a34ece430a587c</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_dword</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gabb68859ac5dfa6a09ac048b4037a83b6</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_dword_far</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga309908c1b7430f0d140edb78fcf8f2b9</anchor>
      <arglist>(address_long)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_dword_near</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga7fa92c0a662403a643859e0f33b0a182</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_float</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga7911bddb066a8a038efc4b7857728fa8</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_float_far</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga4a5a593aa84062ca08b3f2d564a6f466</anchor>
      <arglist>(address_long)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_float_near</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga066040df814dabc7980cd1422508b46b</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_ptr</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gac9f61931fd8cf4cf4764744dbaee81b6</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_ptr_far</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gaa7756dce7bcc8ba46e648981fafebbf2</anchor>
      <arglist>(address_long)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_ptr_near</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gadcfd4df2e156efc1f6a380dfc58a5b81</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_word</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga32d8ab354156f4b1ffdb77a275ba6223</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_word_far</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gad7082c45c2c96f015c76eff1ad00a99a</anchor>
      <arglist>(address_long)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_word_near</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gaf51eeaa847dd2668d2a66b70ecfb7398</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PGM_VOID_P</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga84a61d55b7efefabd8419e28f02704f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PROGMEM</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga75acaba9e781937468d0911423bc0c35</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PSTR</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga05ca900ebf7cd121be73c654d9ccb3eb</anchor>
      <arglist>(s)</arglist>
    </member>
    <member kind="typedef">
      <type>char PROGMEM</type>
      <name>prog_char</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gaa475b6b81fd8b34de45695da1da523b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int16_t PROGMEM</type>
      <name>prog_int16_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gaafc910d0b2c4d76afffa4710b98df6fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t PROGMEM</type>
      <name>prog_int32_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gaa839901aa518fb43d361588dd8d2b44b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int64_t PROGMEM</type>
      <name>prog_int64_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga5b1f9927f06d841e9ac07af62e71cfef</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int8_t PROGMEM</type>
      <name>prog_int8_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga48c7cb011ea5f82f4b73df40e07dff46</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned char PROGMEM</type>
      <name>prog_uchar</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga7d4701843a2019e3ef5a9866dc7586ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t PROGMEM</type>
      <name>prog_uint16_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga93ec00229866bf6a125384ad08cefa73</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t PROGMEM</type>
      <name>prog_uint32_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga31bad0d22ead95a41e725c38ea63eb26</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t PROGMEM</type>
      <name>prog_uint64_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gaa50eebe90a40e0276bcc49ea0482b211</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t PROGMEM</type>
      <name>prog_uint8_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga39235a28487ae7790ce5f4c8178c8ed7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void PROGMEM</type>
      <name>prog_void</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gadb50761b9f19d45449445208778ee420</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>power.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/avr/</path>
    <filename>power_8h</filename>
    <includes id="io_8h" name="io.h" local="no" imported="no">avr/io.h</includes>
    <includes id="stdint_8h" name="stdint.h" local="no" imported="no">stdint.h</includes>
  </compound>
  <compound kind="file">
    <name>setbaud.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/util/</path>
    <filename>setbaud_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>BAUD_TOL</name>
      <anchorfile>group__util__setbaud.html</anchorfile>
      <anchor>gaddaa61b72aca51ed4835978d500f8755</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UBRR_VALUE</name>
      <anchorfile>group__util__setbaud.html</anchorfile>
      <anchor>ga0b029ff580b042a27aaac4bd2ec925e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UBRRH_VALUE</name>
      <anchorfile>group__util__setbaud.html</anchorfile>
      <anchor>ga8188daef772f5eb3fc81dfee168905e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UBRRL_VALUE</name>
      <anchorfile>group__util__setbaud.html</anchorfile>
      <anchor>ga39ebec5d04e582b7b6ed9a72b973983c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USE_2X</name>
      <anchorfile>group__util__setbaud.html</anchorfile>
      <anchor>ga6977ce48ae3197f3f016b85d98380509</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>setjmp.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/</path>
    <filename>setjmp_8h</filename>
    <member kind="function">
      <type>void</type>
      <name>longjmp</name>
      <anchorfile>group__setjmp.html</anchorfile>
      <anchor>ga87f44eafaab5ec0ef8f5a11a8b853acf</anchor>
      <arglist>(jmp_buf __jmpb, int __ret) __ATTR_NORETURN__</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>setjmp</name>
      <anchorfile>group__setjmp.html</anchorfile>
      <anchor>ga2687c5ef7a3f376db90908999a9a7fc6</anchor>
      <arglist>(jmp_buf __jmpb)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>signature.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/avr/</path>
    <filename>signature_8h</filename>
    <includes id="io_8h" name="io.h" local="no" imported="no">avr/io.h</includes>
  </compound>
  <compound kind="file">
    <name>sleep.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/avr/</path>
    <filename>sleep_8h</filename>
    <includes id="io_8h" name="io.h" local="no" imported="no">avr/io.h</includes>
    <includes id="stdint_8h" name="stdint.h" local="no" imported="no">stdint.h</includes>
    <member kind="function">
      <type>void</type>
      <name>sleep_cpu</name>
      <anchorfile>group__avr__sleep.html</anchorfile>
      <anchor>ga157b2578d95309c197b739f812938d94</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sleep_disable</name>
      <anchorfile>group__avr__sleep.html</anchorfile>
      <anchor>gaeae22433a78fd8d50f915fb68c416efd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sleep_enable</name>
      <anchorfile>group__avr__sleep.html</anchorfile>
      <anchor>ga475174a7aa4eda03dfa7a4483e400a9e</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>stdint.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/</path>
    <filename>stdint_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>INT8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaaf7f29f45f1a513b4748a4e5014ddf6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT8_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaadcf2a81af243df333b31efa6461ab8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaeb4e270a084ee26fe73e799861bd0252</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac58f2c111cc9989c86db2a7dc4fd84ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT16_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad4e9955955b27624963643eac448118a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga3ea490c9b3617d4479bd80ef93cd5602</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga181807730d4a375f848ba139813ce04f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga688eb21a22db27c2b2bd5836943cdcbe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab5eb23180f7cc12b7d6c04a8ec067fdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad0d744f05898e32d01f73f8af3cd2071</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT64_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab21f12f372f67b8ff0aa3432336ede67</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga30654b4b67d97c42ca3f9b6052dda916</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaa05109908fb2770f632d2b646b9f85bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST8_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga3e986cad833f63f420962ff60eda87e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_LEAST8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2a80bde77ee1698d0f42f334adad4f2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga7eb2a8e2a1c65d6c9ad0f86154890baa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST16_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1f91bfd5820c2f27af3d260fc75813e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_LEAST16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga6ef6a1a518bbf516ca8b0180b11c358f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga5618711a0a54f722190a3a1219e278c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST32_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2360a536116dd734820a6b5b3d560ce7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_LEAST32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga70cad8bacc9a6db301e1cdc86cc8d571</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga35d0f98a2e507fd1be779d49da92724e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST64_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac12b4f6966b57ad82feb683b284b4060</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_LEAST64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaab530113fa96e280e49c3c138b0f917d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gacbcdb3bee0f5f904da5df8de69a80ca3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST8_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaad8fb982cb19143efd5ee9a1a7a89390</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_FAST8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2c6f97ea2d76d0cf6260c84046cdb44e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2fd35d0ea091e04caec504ff0042cf00</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST16_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga169460a4e2a79138723d68d99372d958</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_FAST16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaed28ca63d9b222f6f1377358fe73a183</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac96fa0f41b19e89f109e4f9913ca6635</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST32_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad93df1652ed0635513d5efe4f1219926</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_FAST32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad51246a178143208b2db3315efd21c45</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga13c95cf9c209d8daacb36cbf0d5ba275</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST64_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga50f0fdcb00ea2500cec0f3d6d45c36f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_FAST64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaeb74410af7781bc84b5f64ae7a8f4a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTPTR_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga9e5742f2bae4a5283431a3c03499e3a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTPTR_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2aaa6d3aa1d7d1e0e326955aa24db752</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINTPTR_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab2355300ea19395357e62d780f4dd073</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTMAX_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga022b9b0a3564d786244a4631847c37a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTMAX_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2b0a3edfc672154f606dc3ad26277b61</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINTMAX_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaa54fd5210434219e9027bfa0f0e325c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PTRDIFF_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gadd2ef7bffac19cfdd1f4b5495409672f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PTRDIFF_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad9b88ba2fb858f98b50b38e49875d90e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIG_ATOMIC_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1f5fe9445d0ad0bee21bab1de4cc3e58</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIG_ATOMIC_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga21e605b9ac3a03b6de93cdf5a69e129f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIZE_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga3c75bb398badb69c7577b21486f9963f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT8_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gacc666d136a198406538bafb8288f9c1b</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT8_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gacb579905e23358f9a5035b075c57ffcd</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT16_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1e6d0a954851b2f07b54a2178dc6eb40</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT16_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab93c67f535ddedd6e88a9a8030b333f1</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1ddf83530d986214005d1e766f036b28</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT32_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad76f76a71e9ffc13963ee895d1828b2c</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT64_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaab7df05e4698e14dbef512294ff39203</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT64_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gae770776c04e6d959364d2612ddfce99d</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTMAX_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaefea1274b2789b420969856ac1a8f19e</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINTMAX_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga97a030e600e111b2b5981b4c39265ab6</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="typedef">
      <type>signed char</type>
      <name>int8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaef44329758059c91c76d334e8fc09700</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned char</type>
      <name>uint8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaba7bc1797add20fe3efdf37ced1182c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>signed int</type>
      <name>int16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga932e6ccc3d54c58f761c1aead83bd6d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned int</type>
      <name>uint16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1f1825b69244eb3ad2c7165ddc99c956</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>signed long int</type>
      <name>int32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gadb828ef50c2dbb783109824e94cf6c47</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned long int</type>
      <name>uint32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga33594304e786b158f3fb30289278f5af</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>signed long long int</type>
      <name>int64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga831d6234342279926bb11bad3a37add9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned long long int</type>
      <name>uint64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad27ed092432b64ff558d2254c278720f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int16_t</type>
      <name>intptr_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga9d283932a52e505ae3b2a4f902e8b53c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>uintptr_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2c8c1b9f53772a86b0827ce7399b68aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int8_t</type>
      <name>int_least8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaadfd725efbf565422ab13db91ccca53c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>uint_least8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac76e2383debd5a3b100514044828961a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int16_t</type>
      <name>int_least16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga17f379713bed2a28ac431760401253cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>uint_least16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga4f3f6e6631cb4aaeadf1c59ff597b2fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t</type>
      <name>int_least32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2676b57a778795e5d0598970f1407f38</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>uint_least32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac0af81082969e5e3f4d939b1de7002ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int64_t</type>
      <name>int_least64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2073b30b3170d509bc756bf5c7862caf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>uint_least64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab604f73dd823867b43702ae88b4f4445</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int8_t</type>
      <name>int_fast8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga880ed9ceb8621521452c81d03bd08cfb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>uint_fast8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad0fca8b15c218d2c687f8c373a71d228</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int16_t</type>
      <name>int_fast16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga9b7386d4af0e20ee32296d9a158c9f3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>uint_fast16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga6ed085329b153773ff76afa0702cf897</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t</type>
      <name>int_fast32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga920d4b149da0252281b6762375fb644a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>uint_fast32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga8f075c759c74e109e8184306c663809d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int64_t</type>
      <name>int_fast64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gafd072b9a94c14417246175f6442422ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>uint_fast64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga6fd055dddb7d91fab0635146851af8d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int64_t</type>
      <name>intmax_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga036cd61bb4b30bb510b9538af4cebd1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>uintmax_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2ba5f6c0633401558d277b2c0e4f758d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>signed char</type>
      <name>int8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaef44329758059c91c76d334e8fc09700</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned char</type>
      <name>uint8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaba7bc1797add20fe3efdf37ced1182c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>signed int</type>
      <name>int16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga932e6ccc3d54c58f761c1aead83bd6d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned int</type>
      <name>uint16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1f1825b69244eb3ad2c7165ddc99c956</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>signed long int</type>
      <name>int32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gadb828ef50c2dbb783109824e94cf6c47</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned long int</type>
      <name>uint32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga33594304e786b158f3fb30289278f5af</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>signed long long int</type>
      <name>int64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga831d6234342279926bb11bad3a37add9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned long long int</type>
      <name>uint64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad27ed092432b64ff558d2254c278720f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int16_t</type>
      <name>intptr_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga9d283932a52e505ae3b2a4f902e8b53c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>uintptr_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2c8c1b9f53772a86b0827ce7399b68aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int8_t</type>
      <name>int_least8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaadfd725efbf565422ab13db91ccca53c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>uint_least8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac76e2383debd5a3b100514044828961a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int16_t</type>
      <name>int_least16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga17f379713bed2a28ac431760401253cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>uint_least16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga4f3f6e6631cb4aaeadf1c59ff597b2fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t</type>
      <name>int_least32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2676b57a778795e5d0598970f1407f38</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>uint_least32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac0af81082969e5e3f4d939b1de7002ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int64_t</type>
      <name>int_least64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2073b30b3170d509bc756bf5c7862caf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>uint_least64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab604f73dd823867b43702ae88b4f4445</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int8_t</type>
      <name>int_fast8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga880ed9ceb8621521452c81d03bd08cfb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>uint_fast8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad0fca8b15c218d2c687f8c373a71d228</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int16_t</type>
      <name>int_fast16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga9b7386d4af0e20ee32296d9a158c9f3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>uint_fast16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga6ed085329b153773ff76afa0702cf897</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t</type>
      <name>int_fast32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga920d4b149da0252281b6762375fb644a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>uint_fast32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga8f075c759c74e109e8184306c663809d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int64_t</type>
      <name>int_fast64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gafd072b9a94c14417246175f6442422ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>uint_fast64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga6fd055dddb7d91fab0635146851af8d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int64_t</type>
      <name>intmax_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga036cd61bb4b30bb510b9538af4cebd1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>uintmax_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2ba5f6c0633401558d277b2c0e4f758d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaaf7f29f45f1a513b4748a4e5014ddf6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT8_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaadcf2a81af243df333b31efa6461ab8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaeb4e270a084ee26fe73e799861bd0252</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac58f2c111cc9989c86db2a7dc4fd84ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT16_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad4e9955955b27624963643eac448118a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga3ea490c9b3617d4479bd80ef93cd5602</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga181807730d4a375f848ba139813ce04f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga688eb21a22db27c2b2bd5836943cdcbe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab5eb23180f7cc12b7d6c04a8ec067fdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad0d744f05898e32d01f73f8af3cd2071</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT64_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab21f12f372f67b8ff0aa3432336ede67</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga30654b4b67d97c42ca3f9b6052dda916</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaa05109908fb2770f632d2b646b9f85bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST8_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga3e986cad833f63f420962ff60eda87e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_LEAST8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2a80bde77ee1698d0f42f334adad4f2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga7eb2a8e2a1c65d6c9ad0f86154890baa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST16_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1f91bfd5820c2f27af3d260fc75813e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_LEAST16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga6ef6a1a518bbf516ca8b0180b11c358f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga5618711a0a54f722190a3a1219e278c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST32_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2360a536116dd734820a6b5b3d560ce7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_LEAST32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga70cad8bacc9a6db301e1cdc86cc8d571</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga35d0f98a2e507fd1be779d49da92724e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST64_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac12b4f6966b57ad82feb683b284b4060</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_LEAST64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaab530113fa96e280e49c3c138b0f917d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gacbcdb3bee0f5f904da5df8de69a80ca3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST8_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaad8fb982cb19143efd5ee9a1a7a89390</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_FAST8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2c6f97ea2d76d0cf6260c84046cdb44e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2fd35d0ea091e04caec504ff0042cf00</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST16_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga169460a4e2a79138723d68d99372d958</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_FAST16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaed28ca63d9b222f6f1377358fe73a183</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac96fa0f41b19e89f109e4f9913ca6635</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST32_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad93df1652ed0635513d5efe4f1219926</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_FAST32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad51246a178143208b2db3315efd21c45</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga13c95cf9c209d8daacb36cbf0d5ba275</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST64_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga50f0fdcb00ea2500cec0f3d6d45c36f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_FAST64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaeb74410af7781bc84b5f64ae7a8f4a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTPTR_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga9e5742f2bae4a5283431a3c03499e3a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTPTR_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2aaa6d3aa1d7d1e0e326955aa24db752</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINTPTR_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab2355300ea19395357e62d780f4dd073</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTMAX_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga022b9b0a3564d786244a4631847c37a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTMAX_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2b0a3edfc672154f606dc3ad26277b61</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINTMAX_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaa54fd5210434219e9027bfa0f0e325c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PTRDIFF_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gadd2ef7bffac19cfdd1f4b5495409672f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PTRDIFF_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad9b88ba2fb858f98b50b38e49875d90e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIG_ATOMIC_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1f5fe9445d0ad0bee21bab1de4cc3e58</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIG_ATOMIC_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga21e605b9ac3a03b6de93cdf5a69e129f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIZE_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga3c75bb398badb69c7577b21486f9963f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT8_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gacc666d136a198406538bafb8288f9c1b</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT8_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gacb579905e23358f9a5035b075c57ffcd</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT16_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1e6d0a954851b2f07b54a2178dc6eb40</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT16_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab93c67f535ddedd6e88a9a8030b333f1</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1ddf83530d986214005d1e766f036b28</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT32_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad76f76a71e9ffc13963ee895d1828b2c</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT64_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaab7df05e4698e14dbef512294ff39203</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT64_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gae770776c04e6d959364d2612ddfce99d</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTMAX_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaefea1274b2789b420969856ac1a8f19e</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINTMAX_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga97a030e600e111b2b5981b4c39265ab6</anchor>
      <arglist>(value)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>stdio.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/</path>
    <filename>stdio_8h</filename>
    <includes id="inttypes_8h" name="inttypes.h" local="no" imported="no">inttypes.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>_FDEV_EOF</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga280c47b9edff2751b8dbb9a715dc44c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_FDEV_ERR</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga132917c782a0ba854c3ae9abe57c3b12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_FDEV_SETUP_READ</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga79826a1c733e4f67843256d34ad64352</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_FDEV_SETUP_RW</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga93db55caeeaf13488a635f8896a6bdf3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_FDEV_SETUP_WRITE</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga822a791a1c06a12b95c14ace75b48ad2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EOF</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga59adc4c82490d23754cd39c2fb99b0da</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fdev_close</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaa1226b8f734a1b5148d931ae2908c45d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fdev_get_udata</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gac04f5faa8e86a4ce4ae084d85386af40</anchor>
      <arglist>(stream)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fdev_set_udata</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga793fcb8f10b6b8746d4186012929ddf1</anchor>
      <arglist>(stream, u)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fdev_setup_stream</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaf41f158c022cbb6203ccd87d27301226</anchor>
      <arglist>(stream, put, get, rwflag)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FDEV_SETUP_STREAM</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaea2b6be92ead4673bc487b271b7227fb</anchor>
      <arglist>(put, get, rwflag)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>getc</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gacff255b3a0079ebb1516e8a4eb23a6fb</anchor>
      <arglist>(__stream)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>getchar</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gac0484b3e3a4d8361d91c3322440f9195</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>putc</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga247111653f3e91bf2cb4930d590d8201</anchor>
      <arglist>(__c, __stream)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>putchar</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga61e2590ae5f2f2e351365640d2ac755a</anchor>
      <arglist>(__c)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>stderr</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga5ce35bd5ba5021fd3b2e951e8f497656</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>stdin</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaaca70138f0cb63ddb026921afc635179</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>stdout</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga0c0ef221f95f64e8632451312fd18cc8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct __file</type>
      <name>FILE</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaed4dabeb9f7c518ded42f930a04abce8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clearerr</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaaa6d255675688c736c99ebd32f2a7214</anchor>
      <arglist>(FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fclose</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gad3d27a6dcc225237171196dd0739bb10</anchor>
      <arglist>(FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>feof</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga47b00053272d6443c9befdb1304002cb</anchor>
      <arglist>(FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ferror</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaa89248dd55efb5f59a96e82976c97758</anchor>
      <arglist>(FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fflush</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gadb974f28765a31026ee6bf71d5175951</anchor>
      <arglist>(FILE *stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fgetc</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga818d63019adc9d518a13f9c36ed04f35</anchor>
      <arglist>(FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>fgets</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga00d34a8bff0293d2d6f4563d248d8fb2</anchor>
      <arglist>(char *__str, int __size, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga0e41285401c397eb29692205a95fcd9c</anchor>
      <arglist>(FILE *__stream, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fprintf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga36173b4a8551b61811089198beec69d9</anchor>
      <arglist>(FILE *__stream, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fputc</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaf3387285b3eb0ce4e0e7ca3ccaed9014</anchor>
      <arglist>(int __c, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fputs</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga19c2bbe9ce4af9f0a7e3448387004fd3</anchor>
      <arglist>(const char *__str, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fputs_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga3d25813cb225ca410518a3f48eb00caa</anchor>
      <arglist>(const char *__str, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>fread</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga54fa47156a34c1659a29ed96e46e3518</anchor>
      <arglist>(void *__ptr, size_t __size, size_t __nmemb, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fscanf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga0beb4fd9ff6833a364e3ce60370de058</anchor>
      <arglist>(FILE *__stream, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fscanf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga7aec94e711ad64724076666586a26839</anchor>
      <arglist>(FILE *__stream, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>fwrite</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gadd5777719a41713629a62b68c239a774</anchor>
      <arglist>(const void *__ptr, size_t __size, size_t __nmemb, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>gets</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaf577dcba9afe50a9d068d0b69ac85d2f</anchor>
      <arglist>(char *__str)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>printf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga4c04da4953607fa5fa4d3908fecde449</anchor>
      <arglist>(const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>printf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga418e63921ed6259e873cd21b6c5c8e6e</anchor>
      <arglist>(const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>puts</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga33f7bd99d40bf6f68a00d5507d65363d</anchor>
      <arglist>(const char *__str)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>puts_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gab4de83c560c79bf880fa39b997d61610</anchor>
      <arglist>(const char *__str)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>scanf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga3f0edc16dcabb5344d59d42cf7682102</anchor>
      <arglist>(const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>scanf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga0fb7fd70cd7618f27d8219c97e61bcf3</anchor>
      <arglist>(const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>snprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga77070c245d4ca4f7ec7d7144260fb875</anchor>
      <arglist>(char *__s, size_t __n, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>snprintf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga53ff61856759709eeceae10aaa10a0a3</anchor>
      <arglist>(char *__s, size_t __n, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga6017094d9fd800fa02600d35399f2a2a</anchor>
      <arglist>(char *__s, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sprintf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga2b829d696b17dedbf181cd5dc4d7a31d</anchor>
      <arglist>(char *__s, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sscanf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga5507d0e1bbfd387fbb2ffcfd8f5dca6f</anchor>
      <arglist>(const char *__buf, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sscanf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaeca11dc4b3757ed4ff2f2a4950eba117</anchor>
      <arglist>(const char *__buf, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ungetc</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gab4f9b130166e5811519513d6178c1ae3</anchor>
      <arglist>(int __c, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vfprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaa3b98c0d17b35642c0f3e4649092b9f1</anchor>
      <arglist>(FILE *__stream, const char *__fmt, va_list __ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vfprintf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga55b25ecbfd3811ea4495d1f235e2e186</anchor>
      <arglist>(FILE *__stream, const char *__fmt, va_list __ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vfscanf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga67bae1ad3af79809fd770be392f90e21</anchor>
      <arglist>(FILE *__stream, const char *__fmt, va_list __ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vfscanf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga6c6b5b881ce8f4739777ff3a615e988a</anchor>
      <arglist>(FILE *__stream, const char *__fmt, va_list __ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga0b15be24dd9db93355e1f62937fdfd9a</anchor>
      <arglist>(const char *__fmt, va_list __ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vscanf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga8bd4b760f67791a54e73111734caa82f</anchor>
      <arglist>(const char *__fmt, va_list __ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vsnprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gac92e8c42a044c8f50aad5c2c69e638e0</anchor>
      <arglist>(char *__s, size_t __n, const char *__fmt, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vsnprintf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga2071feb5c92bf50a6bd508a07ead9515</anchor>
      <arglist>(char *__s, size_t __n, const char *__fmt, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vsprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaaeb1bbe21a1b9b50b207ab059a67993f</anchor>
      <arglist>(char *__s, const char *__fmt, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vsprintf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaf47f5141509d1e434f9da2b27287a707</anchor>
      <arglist>(char *__s, const char *__fmt, va_list ap)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>stdlib.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/</path>
    <filename>stdlib_8h</filename>
    <class kind="struct">div_t</class>
    <class kind="struct">ldiv_t</class>
    <member kind="define">
      <type>#define</type>
      <name>RAND_MAX</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga690f251553b39fd4f31894826141b61a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>__compar_fn_t</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga35e28bfcc8d641e0eebd66f4dc559a3f</anchor>
      <arglist>)(const void *, const void *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>abort</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga63e28bec3592384b44606f011634c5a8</anchor>
      <arglist>(void) __ATTR_NORETURN__</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>abs</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gadb8c83badc195efc1229799391fececc</anchor>
      <arglist>(int __i)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>atoi</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga3a1fe00c1327bbabc76688a7a1d73370</anchor>
      <arglist>(const char *__s) __ATTR_PURE__</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>atol</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga764de49bd918caf24ce1caf3a10b3823</anchor>
      <arglist>(const char *__s) __ATTR_PURE__</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>bsearch</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga885c1ccefb716ff16ab73a57003140be</anchor>
      <arglist>(const void *__key, const void *__base, size_t __nmemb, size_t __size, int(*__compar)(const void *, const void *))</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>calloc</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga51ac965dacbc9daf922f469bdcfe00c2</anchor>
      <arglist>(size_t __nele, size_t __size) __ATTR_MALLOC__</arglist>
    </member>
    <member kind="function">
      <type>div_t</type>
      <name>div</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga7486ea9a8a90ac6b93bed37d08ebbd9e</anchor>
      <arglist>(int __num, int __denom) __asm__(&quot;__divmodhi4&quot;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>exit</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga137096a48cc0c731052cadfb69c39b34</anchor>
      <arglist>(int __status) __ATTR_NORETURN__</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>free</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gafb8699abb1f51d920a176e695ff3be8a</anchor>
      <arglist>(void *__ptr)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>labs</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gae017047d6d0a688ccb622ff062dcd230</anchor>
      <arglist>(long __i)</arglist>
    </member>
    <member kind="function">
      <type>ldiv_t</type>
      <name>ldiv</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga5b688b463f9faaa82f31ac7587e06849</anchor>
      <arglist>(long __num, long __denom) __asm__(&quot;__divmodsi4&quot;)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>malloc</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga4996af830ebe744d9678e5251dfd3ebd</anchor>
      <arglist>(size_t __size) __ATTR_MALLOC__</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>qsort</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gafd4bf2faec43342e7ad3d2ab37bac1fe</anchor>
      <arglist>(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>rand</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gae23144bcbb8e3742b00eb687c36654d1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>rand_r</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gaf5085001be836a0f2a5d3269a7c9fd04</anchor>
      <arglist>(unsigned long *__ctx)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>realloc</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gafd300bad8b4dd2e88b07d464d76c92aa</anchor>
      <arglist>(void *__ptr, size_t __size) __ATTR_MALLOC__</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>srand</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gacceff9dc95bb3c5562dc9e61fc3d9075</anchor>
      <arglist>(unsigned int __seed)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>strtod</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga5ee4d110a3bb55d2eadda05e3ebedf8a</anchor>
      <arglist>(const char *__nptr, char **__endptr)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>strtol</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gaf8ce3b8dae3d45c34c3b172de503f7b3</anchor>
      <arglist>(const char *__nptr, char **__endptr, int __base)</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>strtoul</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gaea44aa48bda8261f794dcb2d1e7ab2b2</anchor>
      <arglist>(const char *__nptr, char **__endptr, int __base)</arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>__malloc_heap_end</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga4d7b1bf0f75d529cc75229a266132115</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>__malloc_heap_start</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga9310042b3956282440c091d20cb98c5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>__malloc_margin</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gaffadd67736fd340e893fb22c207de597</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RANDOM_MAX</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga3bd31f0d9a9127548b734e7ca03cc6df</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>itoa</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gaa571de9e773dde59b0550a5ca4bd2f00</anchor>
      <arglist>(int val, char *s, int radix)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>ltoa</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga5a3fce5fbd20140584619ba9aed09f75</anchor>
      <arglist>(long val, char *s, int radix)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>utoa</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gad50d835af7e358518fd100179732e948</anchor>
      <arglist>(unsigned int val, char *s, int radix)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>ultoa</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga34f41757388f40e340f31e370ac009b5</anchor>
      <arglist>(unsigned long val, char *s, int radix)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>random</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga114aeb1751119382aaf3340355b22cfd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>srandom</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga4fca27496ca29c0a723d197696641eff</anchor>
      <arglist>(unsigned long __seed)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>random_r</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gaa99a0733f06d2b9960a1401c2721af1e</anchor>
      <arglist>(unsigned long *__ctx)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DTOSTR_ALWAYS_SIGN</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga815d0d26e215d9b78af1cb2288d22361</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DTOSTR_PLUS_SIGN</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga268774f80047812307365f3113109767</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DTOSTR_UPPERCASE</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga125348f1e1fed1793426f4c4dc6fb2f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EXIT_SUCCESS</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga687984f47d8cce148d1b914d2b79612a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EXIT_FAILURE</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga73efe787c131b385070f25d18b7c9aa4</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>dtostre</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga6c140bdd3b9bd740a1490137317caa44</anchor>
      <arglist>(double __val, char *__s, unsigned char __prec, unsigned char __flags)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>dtostrf</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga060c998e77fb5fc0d3168b3ce8771d42</anchor>
      <arglist>(double __val, signed char __width, unsigned char __prec, char *__s)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>strdup.c</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/libc/string/</path>
    <filename>strdup_8c</filename>
    <includes id="stdlib_8h" name="stdlib.h" local="no" imported="no">stdlib.h</includes>
    <includes id="string_8h" name="string.h" local="no" imported="no">string.h</includes>
    <member kind="function">
      <type>char *</type>
      <name>strdup</name>
      <anchorfile>group__avr__string.html</anchorfile>
      <anchor>ga8569f20e38a030b5a28fd951abec2c9b</anchor>
      <arglist>(const char *s1)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>string.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/</path>
    <filename>string_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>_FFS</name>
      <anchorfile>group__avr__string.html</anchorfile>
      <anchor>gaedb0e7bb7333d6122472acddb5df20ac</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>strdup</name>
      <anchorfile>group__avr__string.html</anchorfile>
      <anchor>ga8569f20e38a030b5a28fd951abec2c9b</anchor>
      <arglist>(const char *s1)</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>strlcat</name>
      <anchorfile>group__avr__string.html</anchorfile>
      <anchor>ga63e609bfa0d354dcd7e35b297c2e6fdd</anchor>
      <arglist>(char *, const char *, size_t)</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>strlcpy</name>
      <anchorfile>group__avr__string.html</anchorfile>
      <anchor>ga64bc119cf084d1ecfd95098994597f12</anchor>
      <arglist>(char *, const char *, size_t)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>strtok</name>
      <anchorfile>group__avr__string.html</anchorfile>
      <anchor>ga6ace85338eafe22a0ff52c00eb9779b8</anchor>
      <arglist>(char *, const char *)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>strtok.c</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/libc/string/</path>
    <filename>strtok_8c</filename>
    <includes id="string_8h" name="string.h" local="no" imported="no">string.h</includes>
    <member kind="function">
      <type>char *</type>
      <name>strtok</name>
      <anchorfile>group__avr__string.html</anchorfile>
      <anchor>ga6ace85338eafe22a0ff52c00eb9779b8</anchor>
      <arglist>(char *s, const char *delim)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>strtok_P.c</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/libc/pmstring/</path>
    <filename>strtok__P_8c</filename>
    <includes id="pgmspace_8h" name="pgmspace.h" local="no" imported="no">avr/pgmspace.h</includes>
    <member kind="function">
      <type>char *</type>
      <name>strtok_P</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga652cbaf54885c73c7ebbefe73524fa22</anchor>
      <arglist>(char *s, PGM_P delim)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>time.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/</path>
    <filename>time_8h</filename>
    <includes id="inttypes_8h" name="inttypes.h" local="no" imported="no">inttypes.h</includes>
    <includes id="stdlib_8h" name="stdlib.h" local="no" imported="no">stdlib.h</includes>
    <class kind="struct">tm</class>
    <class kind="struct">week_date</class>
    <member kind="define">
      <type>#define</type>
      <name>NTP_OFFSET</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gaa7c3e93bff90674e9e74de6bdec85613</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ONE_DAY</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga44eb26d33eb6c5e235c6cbfa377f60ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ONE_DEGREE</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga33af778316d8cac3769692c311825a52</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ONE_HOUR</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gae8dbeb3eda9f88f8f222adea789ec63d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UNIX_OFFSET</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga0643a5b723dde6420363401dffc2e67e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>time_t</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga3346b04b0420b32ccf6b706551b70762</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>_MONTHS_</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga9ea260b185fe2c5bb23c64f5954c28aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>_WEEK_DAYS_</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gae6535271b8e2884a9ed9fe92ecfd87e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>asctime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga68a0e9d22417dfcf9c0be64261352e64</anchor>
      <arglist>(const struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>asctime_r</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gaa1128e048925e3b208213febb8573b88</anchor>
      <arglist>(const struct tm *timeptr, char *buf)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>ctime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga72a5383fadfbaa033ac36e9e45b0bd6d</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ctime_r</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga34fa7766f6b0a8a74a41683693eabad9</anchor>
      <arglist>(const time_t *timer, char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>daylight_seconds</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga71762e360eaab527a334d9114f68aa21</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>difftime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga1732c46d47837951d399755899af5240</anchor>
      <arglist>(time_t time1, time_t time0)</arglist>
    </member>
    <member kind="function">
      <type>int16_t</type>
      <name>equation_of_time</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga633438d79d5dd6f946c8b6ca4ee32111</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>fatfs_time</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga0e2cda2d867a784b4dfc938ad56bb94f</anchor>
      <arglist>(const struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>gm_sidereal</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gaae725f7944578bf948bb5c44588b7b7c</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>struct tm *</type>
      <name>gmtime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gae76343d6a51c93b9318ac0ced05225cd</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmtime_r</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga7cb461118a6aeebeaacd48ee127b952a</anchor>
      <arglist>(const time_t *timer, struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>is_leap_year</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga70762b48818b0b5bfde86973cbdd6c86</anchor>
      <arglist>(int16_t year)</arglist>
    </member>
    <member kind="function">
      <type>struct week_date *</type>
      <name>iso_week_date</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gafe4b12d1be1149c088d61c7cddc945e5</anchor>
      <arglist>(int year, int yday)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>iso_week_date_r</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga8a4fd6a03c19f96f656316eba80b9c9a</anchor>
      <arglist>(int year, int yday, struct week_date *)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>isotime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga1a85cae701479294885ec9f3c6b2fade</anchor>
      <arglist>(const struct tm *tmptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>isotime_r</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gae200cfc8cc7309d0ce5fef3ee490dc47</anchor>
      <arglist>(const struct tm *, char *)</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>lm_sidereal</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga9136313e476a6277cb6ca759542b83e3</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>struct tm *</type>
      <name>localtime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gaa9336f97d394e8e02d97b9ff4fb0aca2</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>localtime_r</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gacd9715002deaf28b432a8f2b54dd9aa5</anchor>
      <arglist>(const time_t *timer, struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>time_t</type>
      <name>mk_gmtime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gab687d0e66f0c74b9723ff9c718ea57da</anchor>
      <arglist>(const struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>time_t</type>
      <name>mktime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gae114b9ba0fce4e2d6297e0e66768bb75</anchor>
      <arglist>(struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>month_length</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gaa743162fd5cbcbaf67371785e812436a</anchor>
      <arglist>(int16_t year, uint8_t month)</arglist>
    </member>
    <member kind="function">
      <type>int8_t</type>
      <name>moon_phase</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga6a79f1da4cd50f398a05679cddc3fede</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_dst</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga3b0c7666163850cc95f0f231f388eb2c</anchor>
      <arglist>(int(*)(const time_t *, int32_t *))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_position</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga90f87ccc97456fe3dcd867ccf71538ee</anchor>
      <arglist>(int32_t latitude, int32_t longitude)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_system_time</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga02663b926feab893c0e0ec1f29b3c315</anchor>
      <arglist>(time_t timestamp)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_zone</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gab4c127c1405b775b6d25a610d3fc26c3</anchor>
      <arglist>(int32_t)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>solar_declination</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga34ba5295b14e45cef7f99c386fff0230</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>time_t</type>
      <name>solar_noon</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga35cf3a10ed003c844c54b7185fdb5b8a</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>strftime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gaa0344a18cbf5cfc45ffdf43890eac51c</anchor>
      <arglist>(char *s, size_t maxsize, const char *format, const struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>time_t</type>
      <name>sun_rise</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga21a491a92bce972d041e2194b4577943</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>time_t</type>
      <name>sun_set</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga0865e7b9f075981fafaabd0964de2d7f</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>system_tick</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga15aea81a8985a4c3cc9c8b00a06f5d31</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>time_t</type>
      <name>time</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga26c7d1dbf93fa8c23c5effbacec91f8c</anchor>
      <arglist>(time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>week_of_month</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga53e37af6d547a0afa624fc2230c8db7a</anchor>
      <arglist>(const struct tm *timeptr, uint8_t start)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>week_of_year</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga6e351ab03dbd6cb078dd48d1f40b9133</anchor>
      <arglist>(const struct tm *timeptr, uint8_t start)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>util/twi.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/util/</path>
    <filename>util_2twi_8h</filename>
    <includes id="io_8h" name="io.h" local="no" imported="no">avr/io.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>TW_START</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga8ec630c2063d0353c53d140b99382d80</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_REP_START</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga8dce3e4b7e35355a8add9ed63d1fa3ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga85cae14b4190042335d25ed9a1b72369</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_SLA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gaca64b973c51d6548a962c1d7cae0663e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga4c28186053b5298305b131ad3e1111f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga91c107a74d268f8578d866ed1bbbd4f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_ARB_LOST</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga5959251c4bd80f48b5a029447d86adb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_ARB_LOST</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gad85222308836bfbe099255662ffb510c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gac16504b87d15d83b97bb0ce61577bb40</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_SLA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga655325b6f8a1818103b126cc3774d8e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga3c8c79b2ecb8d22358839890161cc33b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga532f0ffa12f684346c74a5cbec15950e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga0ecd2ca38d00279194460e65028a0533</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_ARB_LOST_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga1064659d0758206d43d69cd582d1f5da</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga2de84bf7cbf1cd7ae43a6e0f0eeca719</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gacc1837317b1d45e9bb49b8e83cfe6d42</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_LAST_DATA</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gaf92d03d0051d47f5b9375f0ef9293d64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga9e9e761d674793772e6b3f77fc9d7fab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_ARB_LOST_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga1c3ed5ed345d17972002b9fd07f4f829</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_GCALL_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga6eb3b4230887a8b73d7787ff231ea911</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_ARB_LOST_GCALL_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga4756103341a65e0e3889996cdf15b2fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gac50f1a59c74c3109d4913dbecfb472c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga6c0fa701fe9d96b0b7df29e8af154f94</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_GCALL_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga4abaad098fd92bed63719ffb01802c8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_GCALL_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga42234278f9b01b9af75dbbc617b97890</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_STOP</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga98d2570f965790884cf1774e716ec629</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_NO_INFO</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gabcff70642634cb53e9d8e93872f70c90</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_BUS_ERROR</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga90d373160b1d0a3f0c454af83c57df71</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_STATUS_MASK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga8d3aca0acc182f459a51797321728168</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_STATUS</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga4440385d1818b4fe89b20341ea47b75a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_READ</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gaf40f13cadca25e0a83dc096858907819</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_WRITE</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gac8a7cb1a91946c6e3955608a91371148</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_START</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga8ec630c2063d0353c53d140b99382d80</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_REP_START</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga8dce3e4b7e35355a8add9ed63d1fa3ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga85cae14b4190042335d25ed9a1b72369</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_SLA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gaca64b973c51d6548a962c1d7cae0663e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga4c28186053b5298305b131ad3e1111f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga91c107a74d268f8578d866ed1bbbd4f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_ARB_LOST</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga5959251c4bd80f48b5a029447d86adb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_ARB_LOST</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gad85222308836bfbe099255662ffb510c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gac16504b87d15d83b97bb0ce61577bb40</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_SLA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga655325b6f8a1818103b126cc3774d8e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga3c8c79b2ecb8d22358839890161cc33b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga532f0ffa12f684346c74a5cbec15950e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga0ecd2ca38d00279194460e65028a0533</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_ARB_LOST_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga1064659d0758206d43d69cd582d1f5da</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga2de84bf7cbf1cd7ae43a6e0f0eeca719</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gacc1837317b1d45e9bb49b8e83cfe6d42</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_LAST_DATA</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gaf92d03d0051d47f5b9375f0ef9293d64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga9e9e761d674793772e6b3f77fc9d7fab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_ARB_LOST_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga1c3ed5ed345d17972002b9fd07f4f829</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_GCALL_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga6eb3b4230887a8b73d7787ff231ea911</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_ARB_LOST_GCALL_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga4756103341a65e0e3889996cdf15b2fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gac50f1a59c74c3109d4913dbecfb472c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga6c0fa701fe9d96b0b7df29e8af154f94</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_GCALL_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga4abaad098fd92bed63719ffb01802c8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_GCALL_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga42234278f9b01b9af75dbbc617b97890</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_STOP</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga98d2570f965790884cf1774e716ec629</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_NO_INFO</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gabcff70642634cb53e9d8e93872f70c90</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_BUS_ERROR</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga90d373160b1d0a3f0c454af83c57df71</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_STATUS_MASK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga8d3aca0acc182f459a51797321728168</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_STATUS</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga4440385d1818b4fe89b20341ea47b75a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_READ</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gaf40f13cadca25e0a83dc096858907819</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_WRITE</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gac8a7cb1a91946c6e3955608a91371148</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>wdt.h</name>
    <path>/mnt/amy/d1/flip/PROJEKTE/arduino-uno-sdk/avr-libc/avr-libc-1.8.1/include/avr/</path>
    <filename>wdt_8h</filename>
    <includes id="io_8h" name="io.h" local="no" imported="no">avr/io.h</includes>
    <includes id="stdint_8h" name="stdint.h" local="no" imported="no">stdint.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>wdt_disable</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>gab3784e1b871d61ed338da5658184b725</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>wdt_enable</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>gaf6cfea2a1b61e2530ea0c4ef8fc572b3</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>wdt_reset</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga9e52c54d10b6a6a7ce04aaaa4abea51f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_120MS</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga7d028bcdb4a4103549fc6fb4ec07fbcd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_15MS</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>gad45893280f49113ffc2e67e1d741f29d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_1S</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga36302e15f38a4eeb8a328724bb8165e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_250MS</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga66d5f50cc76b92c76900d77ef577d53e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_2S</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga05fc682d276a36d8cc4e9178340ff004</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_30MS</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga057dd21dc54e71de0e20d8bd5734915d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_4S</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga752b0b1b5ba9009bc09976494313e30d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_500MS</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>gacf89fc5fb6c8aa9efaadb86872cfbcdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_60MS</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga7a5b072c51c05a34cc38111f0e6724e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_8S</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>gaf074e538b2a1d5031931530f29a09fce</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>atexit_s</name>
    <filename>structatexit__s.html</filename>
  </compound>
  <compound kind="struct">
    <name>div_t</name>
    <filename>structdiv__t.html</filename>
    <member kind="variable">
      <type>int</type>
      <name>quot</name>
      <anchorfile>structdiv__t.html</anchorfile>
      <anchor>a0b9dda2884048daa68ca4aaa12b17b9a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>rem</name>
      <anchorfile>structdiv__t.html</anchorfile>
      <anchor>ac64389de252de53eda8b4f8dbb7c623f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>ldiv_t</name>
    <filename>structldiv__t.html</filename>
    <member kind="variable">
      <type>long</type>
      <name>quot</name>
      <anchorfile>structldiv__t.html</anchorfile>
      <anchor>a73efd59c176304c327cb4214d0e5e5c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>long</type>
      <name>rem</name>
      <anchorfile>structldiv__t.html</anchorfile>
      <anchor>a0f217ff62b8640aa945ec84d6d0bd000</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>tm</name>
    <filename>structtm.html</filename>
  </compound>
  <compound kind="struct">
    <name>week_date</name>
    <filename>structweek__date.html</filename>
  </compound>
  <compound kind="group">
    <name>alloca</name>
    <title>&lt;alloca.h&gt;: Allocate space in the stack</title>
    <filename>group__alloca.html</filename>
    <member kind="function">
      <type>void *</type>
      <name>alloca</name>
      <anchorfile>group__alloca.html</anchorfile>
      <anchor>gabdd78c9d072e9d390bed0e8b79087a85</anchor>
      <arglist>(size_t __size)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_assert</name>
    <title>&lt;assert.h&gt;: Diagnostics</title>
    <filename>group__avr__assert.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>assert</name>
      <anchorfile>group__avr__assert.html</anchorfile>
      <anchor>ga0041af519e0e7d47c9bcc83760c4669e</anchor>
      <arglist>(expression)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>ctype</name>
    <title>&lt;ctype.h&gt;: Character Operations</title>
    <filename>group__ctype.html</filename>
    <member kind="function">
      <type>int</type>
      <name>isalnum</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gae62f8f9e20430c822d07697b277c5de7</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isalpha</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga66e77eea7376ce0c1e00223f45d0aba9</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isascii</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga41f2d44607ddcc15266bd63cb8e563c4</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isblank</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gad1a49aed816573f1b5787176f5ec701e</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>iscntrl</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga659606df93739b0193c9e98df2953ae0</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isdigit</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga49b21b266576c2490dab1e8f897c801a</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isgraph</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gae89269f754744bbd5486788d06e0c7c9</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>islower</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gae293e218972b626f6304aecbae044ef6</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isprint</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gaa2ded03e3a0d8f5ef2a8df3e834e916d</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ispunct</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>gaa46e128f7550afb4f2cda133594cb4b6</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isspace</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga16e58710aba8108549a1a3c33691b24e</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isupper</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga125dbbabac1caef46a0c6dad060a79d1</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isxdigit</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga04826722a22243889683efde883e8ab3</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>toascii</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga21d5ec3792b2704ecca5778b758dd91f</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>tolower</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga9ba1fce7148e9b63ca6296e02c79bedd</anchor>
      <arglist>(int __c)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>toupper</name>
      <anchorfile>group__ctype.html</anchorfile>
      <anchor>ga924ed052807e23cfa160d5f171cf5e2a</anchor>
      <arglist>(int __c)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_errno</name>
    <title>&lt;errno.h&gt;: System Errors</title>
    <filename>group__avr__errno.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>EDOM</name>
      <anchorfile>group__avr__errno.html</anchorfile>
      <anchor>ga5fe247e079b591a68e0fdbf7caec5b70</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ERANGE</name>
      <anchorfile>group__avr__errno.html</anchorfile>
      <anchor>gaa1591a4f3a86360108de5b9ba34980ca</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_inttypes</name>
    <title>&lt;inttypes.h&gt;: Integer Type conversions</title>
    <filename>group__avr__inttypes.html</filename>
    <member kind="typedef">
      <type>int32_t</type>
      <name>int_farptr_t</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga5082b177b7d7b2039652c26a72b96d18</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>uint_farptr_t</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga72b6692e3f3123903c1a0d9a960c59b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRId8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae53c45f590033ad1f2f517faf3ab2f1b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga404fd01f0b890cb8fac8641aaa704b57</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga943961b7e7e564388dd743593db5bbbb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIi8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gadbe02b78cca747b2fe1a8f7fc5f5cd47</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga526151b1725956030b501d9dd506f2e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga64fb4e44c3ff09179fc445979b7fdad1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRId16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga087e50fe0283aacc71d7138d13e91939</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae90ab00cb4417081dc68e9fd6c0e129a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga58cdfb02574b8c23d964a6e88a268782</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIi16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga655e9b358e0371a4bf5ff21cc08273e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga96945864cb2d1f7de861ccaf639af02e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gac273fb2a05215962fbeae76abaaf0131</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRId32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6d94d1417e1b35c53aee6306590de72b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gad36a6b276bd808d713cc5603ba008c58</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaef5a98227a6af5fde95353ed303cfd1e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIi32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae212e57631ec729f70e0cc42e51dd91e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gad7a1bae7ca12c7b5415fae1b3f258207</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga192a69a2e6e63ed8393d306b4078d63f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIdPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7c8a9ccd40bd2053ca588d1b15e76a30</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIiPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gac2d52bf83b783f530f02fa2eeabe703a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIo8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gad12493b9063f7b2630b90b7f9a7f3301</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa5b3ca8091f4ed7d43f5eb971ce11114</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga37f93445f1795033c9ba577661da6a91</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIu8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga8673208d2d48018fcce020ef59f8ec4f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga74cb15b101649124009c010a9055e885</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga0b0c7ad693c391e3e353e8f2d1df2ec3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIx8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gadac1acc1d24060aeee7791a99d1a3a8c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga45d80a42b6cd25f3ed57b0e800e6e398</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae7e1780719eb0e4b2826a0da06255780</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIX8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4e9b835c85ffa875e8304e2b852b4c86</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga70aa3faf72084587fb18d03aa033a212</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gab153efc9e6547ca56f42de767cde2595</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIo16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga55494a16151668ea78e0b808ef38c8c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1ecbd31333b358c22423a541fffbd122</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga3eda49c829de683e701eaed3cbaf0e73</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIu16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga86bc00ee87e8e40787e0681fc34c576a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa3ba696eef7c107c76c26eea76dcb4b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa82e218a186691ebf7149b36746c12e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIx16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga70f5e38b517f714518c970a4da37bef1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gad00e2a12b813425800cad731f61497ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6f66e34285ab57a86aeb2f0f4895417d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIX16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga570ca9af5087023f75fc8a1a602d26ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gafa4303b077ae4c6c58686178e4b90d18</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga785eabe6337a2fa85874ae99300abb66</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIo32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7276f64276fd7223ca6f4cca0444239a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1e5c50a1ca71da7ff8c4f3f007411be8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6ac7e3111d008785ddf3b29dcd088732</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIu32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaaf2af4a10f0bd308e9c349c8382382be</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaab353a2898377162c1829f1a9708352e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaccc383115328197264988682edfcb72c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIx32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga80ca66bcc9e366733f02c90ed4b0838c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1d766603a3524c9e03effbbece9c2118</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga22caa684d44725e1e6e638983380f68e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIX32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga32b0c8a04aae5d4454d15e6cbe109f64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaaf100a10f9cd73d46294fd0e8db5246d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gace7057a6fa96ac7e2a05946ee96cf2d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIoPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1468793ce960b477922ef92b36a6c802</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIuPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa1ca3a85113e897b5cf7ed6b92d74de2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIxPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga9c3c25e6145e629e4c9fabddc6061c30</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PRIXPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga65d9856517198cfc21558c0d6df64207</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNd8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gabf98c3a9ad120b11ec2911b9398e3f2f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gab0af8c396d9c885950d423f8dee54164</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6dc7d2f030d25e79ae8398088161b860</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNi8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga535485ea35661ff75a8d2bc0d2ebe807</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga1a0b88bf6f131db927f2e7f1f6abb644</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gac864120101e01707ca52c0976b4e539a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNd16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga35974d44b5dcebcb222b8e2c1384241d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga10db5de9c84ccfa6dc0e487dd72051f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga09c9f36f654aa50a548d7820421cdc57</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNi16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7b8508989273ad152f9b3b7cd4db6eee</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga14ec2649667b53ff91a1103c02975837</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaad333b5bea32321b312a3b4967ff357f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNd32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga2b7ab77ff6ede9c3c285b714496f77e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae36c293972a5b770349d74f2c0cfa52f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gadd733be35bef9dcef225edc99ade9e33</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNi32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga52cfc41a1e5ad73788faebbfeb9c14b0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga39be8ffb41be80bc951e955f111e4121</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4739f89fa519cd77097677bf33320091</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNdPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gabf657ee6bd4b009b5b072840a3d7364f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNiPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga9c632ab51b24b93cc315b27a883be9eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNo8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4e274a339187359a91963d22f8e6faa6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga873157069430be3ab2cade457e92f187</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga9716b5135de22733c9c59bc4fc0e3a66</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNu8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae0d5458bfaf4c45083b1e92013d77f51</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae409b3af282bc394819a5dd289cdf57c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga01b368195aa26130d44bf0efe07833fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNx8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga79b1f201c12273510e1fdebfb3a66e9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxLEAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga5cac5341d60e594c818c0f9d25377928</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxFAST8</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga251936e4d698e68846c0917270b5f8a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNo16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga9bc6b517c0117327e832824ff2d6a6b5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga5b05c70b4807922992a9ca529361b44d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga021e130b06fc46198c71dca0fdf89788</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNu16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga37bbde0e3f124b7f482d54adb13b0248</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7a78b92618044bb2d798b57fc6a2e439</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga7cf58abc57bb03d809e6fc41c2a40c33</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNx16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga12dbc2ac6a36b893ef1c25c357f90a9f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxLEAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga24647dd309d4138846376a51a6098304</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxFAST16</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga8b67140c216180e4e5d18003038ee689</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNo32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gab561c947d62a3c7cd396d4aeef553f3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga6b324310e03b8ecbe6888a52b7d8581d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae40f8b90cb75998e70910e7b377288a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNu32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gabd19a83130f8d1bd2f77b765ad804f75</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gae30d5cc7dbc15051e21b72229a2487f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4ce14b7ebee0cfd5c4c935cf79a9a504</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNx32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4c5370556f793ac7b2c3abe896dba8e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxLEAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gabd82b99090a28a84541959ac7ab14ad9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxFAST32</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gac45f394be3c199938a85a631711ce22e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNoPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>ga4a30d36e06018d8e13046079098905a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNuPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gab7dbf5d0ea41679dface5855896e4273</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SCNxPTR</name>
      <anchorfile>group__avr__inttypes.html</anchorfile>
      <anchor>gaa58d290d968643862aec7a8a56e1c8e9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_math</name>
    <title>&lt;math.h&gt;: Mathematics</title>
    <filename>group__avr__math.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>acosf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga3e085dcc8e09f400136816dbc2a56a25</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>asinf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga68e4969cdd396dc3d4ad5abef13880b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>atan2f</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gae31ff8b5990823ebb5d9746cc0843293</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>atanf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga629de3dc6ca25cb7195928581f5359d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>cbrtf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gad724f06c46a6f4ad699fece14f219552</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ceilf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gadb970de70c0549110ae218f6e244c871</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>copysignf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga1f0a9e760e0cd6faaf9619fd3aebfc3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>cosf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gacc171f0e087ed26abd701c827944a43b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>coshf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga218c068c47d79563f3fdbcf0ecb45220</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>expf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga1afc25e5db4387a8480d7cc4ed8d2e6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fabsf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga3745ca949b1ef9df47fb2f52c2536e68</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fdimf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga1305189e3aa1637d4018365c1c08698b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>floorf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga322df600e1eaa1b96b0f994dcd49316f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fmaf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga49c61eb85541c0504867fb05e3598679</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fmaxf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga645ac7eb8b4f0d7e09bcdcadc36044d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fminf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaa591e76a6dddd36f3b745dbf8c8b90a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fmodf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga7f41f3c7a21457008063af86e4eded3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>frexpf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga23413d0a6d19da20534e53f9870cf588</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>hypotf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaae0bb7cd216d5b824d90da083e67021d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INFINITY</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga956e2723d559858d08644ac99146e910</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>isfinitef</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga9e007830d194ee4b5f0a27d2112a0c53</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>isinff</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaf048a8eca1cce97556a96c532c0d2b56</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>isnanf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaf3c3e624db1ecfe93b86972bca1dc4b3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ldexpf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga374c9d6918a8bc1f8bbe054940795e0f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>log10f</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaf356651f154a73fef76067055cc54c16</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>logf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaccce424ce6effa1bfd476479d55dde9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>lrintf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaaf59e045a8a86a9be3370d69dec98a25</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>lroundf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga9031359ea4116de9288526c9e75fd8c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_1_PI</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga08dfac3cca9601a02fc88356cc078e1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_2_PI</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga97f6d6514d3d3dd50c3a2a6d622673db</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_2_SQRTPI</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga631ff334c4a1a6db2e8a7ff4acbe48a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_E</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga9bf5d952c5c93c70f9e66c9794d406c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_LN10</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga0a53871497a155afe91424c28a8ec3c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_LN2</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga92428112a5d24721208748774a4f23e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_LOG10E</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga9ed2b5582226f3896424ff6d2a3ebb14</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_LOG2E</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gac5c747ee5bcbe892875672a0b9d8171c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_PI</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gae71449b1cc6e6250b91f539153a7a0d3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_PI_2</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga958e4508ed28ee5cc04249144312c15f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_PI_4</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaeb24420b096a677f3a2dc5a72b36bf22</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_SQRT1_2</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gacdbb2c2f9429f08916f03c8786d2d2d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_SQRT2</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga66b3ab30f1332874326ed93969e496e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NAN</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga8abfcc76130f3f991d124dd22d7e69bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>powf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaa5a93b1934ef470b7a44ae47e6f2798b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>roundf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gac6950642117c821388ec37e7b656a346</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>signbitf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gacff7dd1db8fe93d3e52d5bd161d8117d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>sinf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga6776b8d1a4149fe35e282a76effd8a9d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>sinhf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gadd3c4414cb88e4071380d6b1d6e69dcd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>squaref</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gadd592f5a4371f7154f95df59a22f5bef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tanf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga841e8ff746ff8b099a00ee83d2eed1d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>tanhf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gae1308938bb768033031c381d148ad8f1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>truncf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga329f9e84df43e59c5999e661aeab14ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>acos</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gae9c5790d8a29cbee8f54f8eb522decbc</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>asin</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga98384ad60834911ec93ac5ae1af4cf0a</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>atan</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga3abd1a0b68d157914a0ee01acaedfe5e</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>atan2</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga054230cd7e4c12958dbfac75ab6886e5</anchor>
      <arglist>(double __y, double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>cbrt</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga9dff6efc5e63405ba23afb75eb3e4af0</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>ceil</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga61470611f23ceef5d3e9cf63d84cd8a7</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>copysign</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaaebf29a8e50e6d8f88b6caf697021c86</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>cos</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga542f5e42e0d3b5df63de0e34ec06bb40</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>cosh</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga2ec1caf3ba3b1ba62eccb3eddf029438</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>exp</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga4ea549372745dda4018ab4b5a94483a6</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>fabs</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga0838a00d86d5e920c80aa7c7532a5559</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>fdim</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga63fcb86a13cf17110582fd6560fb09dd</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>floor</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga0f0bf9ac2651b80846a9d9d89bd4cb85</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>fma</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaf7fbeddc2e7721682e69f59be04ae9d5</anchor>
      <arglist>(double __x, double __y, double __z)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>fmax</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gafd8ab12e9d230e8213ac0c8b77c306d9</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>fmin</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga577eee92b9969942de4d8d134d37c7b8</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>fmod</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaefa8edb8c13adf7fb4b6b7dbe7261a24</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>frexp</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga89f0cb053e3cdb0c9c952ef040087c80</anchor>
      <arglist>(double __x, int *__pexp)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>hypot</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga711412ca8746712e0f19508118bf5154</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int</type>
      <name>isfinite</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gafb997a6a8d6dd0498238098826998aa0</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isinf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga18a7409e0b2341afaa41993960961772</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>isnan</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga468be9ed380771eca5a18e812b2470d4</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>ldexp</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga91643e944a94341bd2a3ed1d3ffbae4f</anchor>
      <arglist>(double __x, int __exp)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>log</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga7f7d556ab6b6235777a179647c152126</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>log10</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga3630cb8cef4560cf0d92e82ae05b03f0</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>lrint</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga9b995838b7bdd4886549dd7e308d0619</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>lround</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaa759c9a1684b0cf2c4c5d133771192ce</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>modf</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga85861fee92c0904e9bb7a9875ee77579</anchor>
      <arglist>(double __x, double *__iptr)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>modff</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gad180f98cb1dac86425c2f4df7faf2a15</anchor>
      <arglist>(float __x, float *__iptr)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>pow</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga41b41c307b8f96760e9c0c17180b241b</anchor>
      <arglist>(double __x, double __y)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>round</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga6eb04604d801054c5a2afe195d1dd75d</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>signbit</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga36894d73110fda52c84dfc050f1004bb</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>sin</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga46e799ece7e1dd323d22cdb53c81cd73</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>sinh</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaae45da8f56ba936dc8624157d1770faa</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>sqrt</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gadd1e40e370b2165c186f72ec631b1853</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>square</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gad11825ff1d6095a08b193c564d524aef</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>tan</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaa2c2303658b8b2555bc97cce3f806bda</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>tanh</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>gaf4b72825a245b794090135251f0ead22</anchor>
      <arglist>(double __x)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>trunc</name>
      <anchorfile>group__avr__math.html</anchorfile>
      <anchor>ga1883497d16352bd92875499f1b39a4b6</anchor>
      <arglist>(double __x)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>setjmp</name>
    <title>&lt;setjmp.h&gt;: Non-local goto</title>
    <filename>group__setjmp.html</filename>
    <member kind="function">
      <type>void</type>
      <name>longjmp</name>
      <anchorfile>group__setjmp.html</anchorfile>
      <anchor>ga87f44eafaab5ec0ef8f5a11a8b853acf</anchor>
      <arglist>(jmp_buf __jmpb, int __ret) __ATTR_NORETURN__</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>setjmp</name>
      <anchorfile>group__setjmp.html</anchorfile>
      <anchor>ga2687c5ef7a3f376db90908999a9a7fc6</anchor>
      <arglist>(jmp_buf __jmpb)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_stdint</name>
    <title>&lt;stdint.h&gt;: Standard Integer Types</title>
    <filename>group__avr__stdint.html</filename>
    <member kind="typedef">
      <type>signed char</type>
      <name>int8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaef44329758059c91c76d334e8fc09700</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned char</type>
      <name>uint8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaba7bc1797add20fe3efdf37ced1182c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>signed int</type>
      <name>int16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga932e6ccc3d54c58f761c1aead83bd6d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned int</type>
      <name>uint16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1f1825b69244eb3ad2c7165ddc99c956</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>signed long int</type>
      <name>int32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gadb828ef50c2dbb783109824e94cf6c47</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned long int</type>
      <name>uint32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga33594304e786b158f3fb30289278f5af</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>signed long long int</type>
      <name>int64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga831d6234342279926bb11bad3a37add9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned long long int</type>
      <name>uint64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad27ed092432b64ff558d2254c278720f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int16_t</type>
      <name>intptr_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga9d283932a52e505ae3b2a4f902e8b53c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>uintptr_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2c8c1b9f53772a86b0827ce7399b68aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int8_t</type>
      <name>int_least8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaadfd725efbf565422ab13db91ccca53c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>uint_least8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac76e2383debd5a3b100514044828961a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int16_t</type>
      <name>int_least16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga17f379713bed2a28ac431760401253cd</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>uint_least16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga4f3f6e6631cb4aaeadf1c59ff597b2fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t</type>
      <name>int_least32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2676b57a778795e5d0598970f1407f38</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>uint_least32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac0af81082969e5e3f4d939b1de7002ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int64_t</type>
      <name>int_least64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2073b30b3170d509bc756bf5c7862caf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>uint_least64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab604f73dd823867b43702ae88b4f4445</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int8_t</type>
      <name>int_fast8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga880ed9ceb8621521452c81d03bd08cfb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t</type>
      <name>uint_fast8_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad0fca8b15c218d2c687f8c373a71d228</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int16_t</type>
      <name>int_fast16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga9b7386d4af0e20ee32296d9a158c9f3a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t</type>
      <name>uint_fast16_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga6ed085329b153773ff76afa0702cf897</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t</type>
      <name>int_fast32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga920d4b149da0252281b6762375fb644a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>uint_fast32_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga8f075c759c74e109e8184306c663809d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int64_t</type>
      <name>int_fast64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gafd072b9a94c14417246175f6442422ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>uint_fast64_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga6fd055dddb7d91fab0635146851af8d5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int64_t</type>
      <name>intmax_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga036cd61bb4b30bb510b9538af4cebd1d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>uintmax_t</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2ba5f6c0633401558d277b2c0e4f758d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaaf7f29f45f1a513b4748a4e5014ddf6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT8_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaadcf2a81af243df333b31efa6461ab8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaeb4e270a084ee26fe73e799861bd0252</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac58f2c111cc9989c86db2a7dc4fd84ca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT16_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad4e9955955b27624963643eac448118a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga3ea490c9b3617d4479bd80ef93cd5602</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga181807730d4a375f848ba139813ce04f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga688eb21a22db27c2b2bd5836943cdcbe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab5eb23180f7cc12b7d6c04a8ec067fdd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad0d744f05898e32d01f73f8af3cd2071</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT64_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab21f12f372f67b8ff0aa3432336ede67</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga30654b4b67d97c42ca3f9b6052dda916</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaa05109908fb2770f632d2b646b9f85bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST8_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga3e986cad833f63f420962ff60eda87e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_LEAST8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2a80bde77ee1698d0f42f334adad4f2b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga7eb2a8e2a1c65d6c9ad0f86154890baa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST16_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1f91bfd5820c2f27af3d260fc75813e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_LEAST16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga6ef6a1a518bbf516ca8b0180b11c358f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga5618711a0a54f722190a3a1219e278c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST32_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2360a536116dd734820a6b5b3d560ce7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_LEAST32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga70cad8bacc9a6db301e1cdc86cc8d571</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga35d0f98a2e507fd1be779d49da92724e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_LEAST64_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac12b4f6966b57ad82feb683b284b4060</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_LEAST64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaab530113fa96e280e49c3c138b0f917d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gacbcdb3bee0f5f904da5df8de69a80ca3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST8_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaad8fb982cb19143efd5ee9a1a7a89390</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_FAST8_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2c6f97ea2d76d0cf6260c84046cdb44e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2fd35d0ea091e04caec504ff0042cf00</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST16_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga169460a4e2a79138723d68d99372d958</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_FAST16_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaed28ca63d9b222f6f1377358fe73a183</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gac96fa0f41b19e89f109e4f9913ca6635</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST32_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad93df1652ed0635513d5efe4f1219926</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_FAST32_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad51246a178143208b2db3315efd21c45</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga13c95cf9c209d8daacb36cbf0d5ba275</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT_FAST64_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga50f0fdcb00ea2500cec0f3d6d45c36f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT_FAST64_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaeb74410af7781bc84b5f64ae7a8f4a17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTPTR_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga9e5742f2bae4a5283431a3c03499e3a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTPTR_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2aaa6d3aa1d7d1e0e326955aa24db752</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINTPTR_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab2355300ea19395357e62d780f4dd073</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTMAX_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga022b9b0a3564d786244a4631847c37a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTMAX_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga2b0a3edfc672154f606dc3ad26277b61</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINTMAX_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaa54fd5210434219e9027bfa0f0e325c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PTRDIFF_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gadd2ef7bffac19cfdd1f4b5495409672f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PTRDIFF_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad9b88ba2fb858f98b50b38e49875d90e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIG_ATOMIC_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1f5fe9445d0ad0bee21bab1de4cc3e58</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIG_ATOMIC_MIN</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga21e605b9ac3a03b6de93cdf5a69e129f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIZE_MAX</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga3c75bb398badb69c7577b21486f9963f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT8_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gacc666d136a198406538bafb8288f9c1b</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT8_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gacb579905e23358f9a5035b075c57ffcd</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT16_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1e6d0a954851b2f07b54a2178dc6eb40</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT16_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gab93c67f535ddedd6e88a9a8030b333f1</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga1ddf83530d986214005d1e766f036b28</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT32_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gad76f76a71e9ffc13963ee895d1828b2c</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT64_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaab7df05e4698e14dbef512294ff39203</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINT64_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gae770776c04e6d959364d2612ddfce99d</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTMAX_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>gaefea1274b2789b420969856ac1a8f19e</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UINTMAX_C</name>
      <anchorfile>group__avr__stdint.html</anchorfile>
      <anchor>ga97a030e600e111b2b5981b4c39265ab6</anchor>
      <arglist>(value)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_stdio</name>
    <title>&lt;stdio.h&gt;: Standard IO facilities</title>
    <filename>group__avr__stdio.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>_FDEV_EOF</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga280c47b9edff2751b8dbb9a715dc44c5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_FDEV_ERR</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga132917c782a0ba854c3ae9abe57c3b12</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_FDEV_SETUP_READ</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga79826a1c733e4f67843256d34ad64352</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_FDEV_SETUP_RW</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga93db55caeeaf13488a635f8896a6bdf3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_FDEV_SETUP_WRITE</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga822a791a1c06a12b95c14ace75b48ad2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EOF</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga59adc4c82490d23754cd39c2fb99b0da</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fdev_close</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaa1226b8f734a1b5148d931ae2908c45d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fdev_get_udata</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gac04f5faa8e86a4ce4ae084d85386af40</anchor>
      <arglist>(stream)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fdev_set_udata</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga793fcb8f10b6b8746d4186012929ddf1</anchor>
      <arglist>(stream, u)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>fdev_setup_stream</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaf41f158c022cbb6203ccd87d27301226</anchor>
      <arglist>(stream, put, get, rwflag)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FDEV_SETUP_STREAM</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaea2b6be92ead4673bc487b271b7227fb</anchor>
      <arglist>(put, get, rwflag)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>getc</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gacff255b3a0079ebb1516e8a4eb23a6fb</anchor>
      <arglist>(__stream)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>getchar</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gac0484b3e3a4d8361d91c3322440f9195</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>putc</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga247111653f3e91bf2cb4930d590d8201</anchor>
      <arglist>(__c, __stream)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>putchar</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga61e2590ae5f2f2e351365640d2ac755a</anchor>
      <arglist>(__c)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>stderr</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga5ce35bd5ba5021fd3b2e951e8f497656</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>stdin</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaaca70138f0cb63ddb026921afc635179</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>stdout</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga0c0ef221f95f64e8632451312fd18cc8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct __file</type>
      <name>FILE</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaed4dabeb9f7c518ded42f930a04abce8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clearerr</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaaa6d255675688c736c99ebd32f2a7214</anchor>
      <arglist>(FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fclose</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gad3d27a6dcc225237171196dd0739bb10</anchor>
      <arglist>(FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>FILE *</type>
      <name>fdevopen</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gab599ddf60819df4cc993c724a83cb1a4</anchor>
      <arglist>(int(*put)(char, FILE *), int(*get)(FILE *))</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>feof</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga47b00053272d6443c9befdb1304002cb</anchor>
      <arglist>(FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ferror</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaa89248dd55efb5f59a96e82976c97758</anchor>
      <arglist>(FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fflush</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gadb974f28765a31026ee6bf71d5175951</anchor>
      <arglist>(FILE *stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fgetc</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga818d63019adc9d518a13f9c36ed04f35</anchor>
      <arglist>(FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>fgets</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga00d34a8bff0293d2d6f4563d248d8fb2</anchor>
      <arglist>(char *__str, int __size, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga0e41285401c397eb29692205a95fcd9c</anchor>
      <arglist>(FILE *__stream, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fprintf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga36173b4a8551b61811089198beec69d9</anchor>
      <arglist>(FILE *__stream, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fputc</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaf3387285b3eb0ce4e0e7ca3ccaed9014</anchor>
      <arglist>(int __c, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fputs</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga19c2bbe9ce4af9f0a7e3448387004fd3</anchor>
      <arglist>(const char *__str, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fputs_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga3d25813cb225ca410518a3f48eb00caa</anchor>
      <arglist>(const char *__str, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>fread</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga54fa47156a34c1659a29ed96e46e3518</anchor>
      <arglist>(void *__ptr, size_t __size, size_t __nmemb, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fscanf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga0beb4fd9ff6833a364e3ce60370de058</anchor>
      <arglist>(FILE *__stream, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>fscanf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga7aec94e711ad64724076666586a26839</anchor>
      <arglist>(FILE *__stream, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>fwrite</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gadd5777719a41713629a62b68c239a774</anchor>
      <arglist>(const void *__ptr, size_t __size, size_t __nmemb, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>gets</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaf577dcba9afe50a9d068d0b69ac85d2f</anchor>
      <arglist>(char *__str)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>printf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga4c04da4953607fa5fa4d3908fecde449</anchor>
      <arglist>(const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>printf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga418e63921ed6259e873cd21b6c5c8e6e</anchor>
      <arglist>(const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>puts</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga33f7bd99d40bf6f68a00d5507d65363d</anchor>
      <arglist>(const char *__str)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>puts_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gab4de83c560c79bf880fa39b997d61610</anchor>
      <arglist>(const char *__str)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>scanf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga3f0edc16dcabb5344d59d42cf7682102</anchor>
      <arglist>(const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>scanf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga0fb7fd70cd7618f27d8219c97e61bcf3</anchor>
      <arglist>(const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>snprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga77070c245d4ca4f7ec7d7144260fb875</anchor>
      <arglist>(char *__s, size_t __n, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>snprintf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga53ff61856759709eeceae10aaa10a0a3</anchor>
      <arglist>(char *__s, size_t __n, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga6017094d9fd800fa02600d35399f2a2a</anchor>
      <arglist>(char *__s, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sprintf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga2b829d696b17dedbf181cd5dc4d7a31d</anchor>
      <arglist>(char *__s, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sscanf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga5507d0e1bbfd387fbb2ffcfd8f5dca6f</anchor>
      <arglist>(const char *__buf, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>sscanf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaeca11dc4b3757ed4ff2f2a4950eba117</anchor>
      <arglist>(const char *__buf, const char *__fmt,...)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>ungetc</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gab4f9b130166e5811519513d6178c1ae3</anchor>
      <arglist>(int __c, FILE *__stream)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vfprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaa3b98c0d17b35642c0f3e4649092b9f1</anchor>
      <arglist>(FILE *__stream, const char *__fmt, va_list __ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vfprintf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga55b25ecbfd3811ea4495d1f235e2e186</anchor>
      <arglist>(FILE *__stream, const char *__fmt, va_list __ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vfscanf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga67bae1ad3af79809fd770be392f90e21</anchor>
      <arglist>(FILE *__stream, const char *__fmt, va_list __ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vfscanf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga6c6b5b881ce8f4739777ff3a615e988a</anchor>
      <arglist>(FILE *__stream, const char *__fmt, va_list __ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga0b15be24dd9db93355e1f62937fdfd9a</anchor>
      <arglist>(const char *__fmt, va_list __ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vscanf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga8bd4b760f67791a54e73111734caa82f</anchor>
      <arglist>(const char *__fmt, va_list __ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vsnprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gac92e8c42a044c8f50aad5c2c69e638e0</anchor>
      <arglist>(char *__s, size_t __n, const char *__fmt, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vsnprintf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>ga2071feb5c92bf50a6bd508a07ead9515</anchor>
      <arglist>(char *__s, size_t __n, const char *__fmt, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vsprintf</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaaeb1bbe21a1b9b50b207ab059a67993f</anchor>
      <arglist>(char *__s, const char *__fmt, va_list ap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vsprintf_P</name>
      <anchorfile>group__avr__stdio.html</anchorfile>
      <anchor>gaf47f5141509d1e434f9da2b27287a707</anchor>
      <arglist>(char *__s, const char *__fmt, va_list ap)</arglist>
    </member>
    <docanchor file="group__avr__stdio">stdio_without_malloc</docanchor>
    <docanchor file="group__avr__stdio">stdio_note1</docanchor>
    <docanchor file="group__avr__stdio">stdio_note2</docanchor>
    <docanchor file="group__avr__stdio">stdio_note3</docanchor>
  </compound>
  <compound kind="group">
    <name>avr_stdlib</name>
    <title>&lt;stdlib.h&gt;: General utilities</title>
    <filename>group__avr__stdlib.html</filename>
    <class kind="struct">div_t</class>
    <class kind="struct">ldiv_t</class>
    <member kind="define">
      <type>#define</type>
      <name>RAND_MAX</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga690f251553b39fd4f31894826141b61a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int(*</type>
      <name>__compar_fn_t</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga35e28bfcc8d641e0eebd66f4dc559a3f</anchor>
      <arglist>)(const void *, const void *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>abort</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga63e28bec3592384b44606f011634c5a8</anchor>
      <arglist>(void) __ATTR_NORETURN__</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>abs</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gadb8c83badc195efc1229799391fececc</anchor>
      <arglist>(int __i)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>atoi</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga3a1fe00c1327bbabc76688a7a1d73370</anchor>
      <arglist>(const char *__s) __ATTR_PURE__</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>atol</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga764de49bd918caf24ce1caf3a10b3823</anchor>
      <arglist>(const char *__s) __ATTR_PURE__</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>bsearch</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga885c1ccefb716ff16ab73a57003140be</anchor>
      <arglist>(const void *__key, const void *__base, size_t __nmemb, size_t __size, int(*__compar)(const void *, const void *))</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>calloc</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga51ac965dacbc9daf922f469bdcfe00c2</anchor>
      <arglist>(size_t __nele, size_t __size) __ATTR_MALLOC__</arglist>
    </member>
    <member kind="function">
      <type>div_t</type>
      <name>div</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga7486ea9a8a90ac6b93bed37d08ebbd9e</anchor>
      <arglist>(int __num, int __denom) __asm__(&quot;__divmodhi4&quot;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>exit</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga137096a48cc0c731052cadfb69c39b34</anchor>
      <arglist>(int __status) __ATTR_NORETURN__</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>free</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gafb8699abb1f51d920a176e695ff3be8a</anchor>
      <arglist>(void *__ptr)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>labs</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gae017047d6d0a688ccb622ff062dcd230</anchor>
      <arglist>(long __i)</arglist>
    </member>
    <member kind="function">
      <type>ldiv_t</type>
      <name>ldiv</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga5b688b463f9faaa82f31ac7587e06849</anchor>
      <arglist>(long __num, long __denom) __asm__(&quot;__divmodsi4&quot;)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>malloc</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga4996af830ebe744d9678e5251dfd3ebd</anchor>
      <arglist>(size_t __size) __ATTR_MALLOC__</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>qsort</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gafd4bf2faec43342e7ad3d2ab37bac1fe</anchor>
      <arglist>(void *__base, size_t __nmemb, size_t __size, __compar_fn_t __compar)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>rand</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gae23144bcbb8e3742b00eb687c36654d1</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>rand_r</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gaf5085001be836a0f2a5d3269a7c9fd04</anchor>
      <arglist>(unsigned long *__ctx)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>realloc</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gafd300bad8b4dd2e88b07d464d76c92aa</anchor>
      <arglist>(void *__ptr, size_t __size) __ATTR_MALLOC__</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>srand</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gacceff9dc95bb3c5562dc9e61fc3d9075</anchor>
      <arglist>(unsigned int __seed)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>strtod</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga5ee4d110a3bb55d2eadda05e3ebedf8a</anchor>
      <arglist>(const char *__nptr, char **__endptr)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>strtol</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gaf8ce3b8dae3d45c34c3b172de503f7b3</anchor>
      <arglist>(const char *__nptr, char **__endptr, int __base)</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>strtoul</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gaea44aa48bda8261f794dcb2d1e7ab2b2</anchor>
      <arglist>(const char *__nptr, char **__endptr, int __base)</arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>__malloc_heap_end</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga4d7b1bf0f75d529cc75229a266132115</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>char *</type>
      <name>__malloc_heap_start</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga9310042b3956282440c091d20cb98c5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>__malloc_margin</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gaffadd67736fd340e893fb22c207de597</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>ltoa</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga5a3fce5fbd20140584619ba9aed09f75</anchor>
      <arglist>(long val, char *s, int radix)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>utoa</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gad50d835af7e358518fd100179732e948</anchor>
      <arglist>(unsigned int val, char *s, int radix)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>ultoa</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga34f41757388f40e340f31e370ac009b5</anchor>
      <arglist>(unsigned long val, char *s, int radix)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>random</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga114aeb1751119382aaf3340355b22cfd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>srandom</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga4fca27496ca29c0a723d197696641eff</anchor>
      <arglist>(unsigned long __seed)</arglist>
    </member>
    <member kind="function">
      <type>long</type>
      <name>random_r</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gaa99a0733f06d2b9960a1401c2721af1e</anchor>
      <arglist>(unsigned long *__ctx)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>itoa</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>gaa571de9e773dde59b0550a5ca4bd2f00</anchor>
      <arglist>(int val, char *s, int radix)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RANDOM_MAX</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga3bd31f0d9a9127548b734e7ca03cc6df</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>dtostre</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga6c140bdd3b9bd740a1490137317caa44</anchor>
      <arglist>(double __val, char *__s, unsigned char __prec, unsigned char __flags)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>dtostrf</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga060c998e77fb5fc0d3168b3ce8771d42</anchor>
      <arglist>(double __val, signed char __width, unsigned char __prec, char *__s)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DTOSTR_ALWAYS_SIGN</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga815d0d26e215d9b78af1cb2288d22361</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DTOSTR_PLUS_SIGN</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga268774f80047812307365f3113109767</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DTOSTR_UPPERCASE</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga125348f1e1fed1793426f4c4dc6fb2f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EXIT_SUCCESS</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga687984f47d8cce148d1b914d2b79612a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EXIT_FAILURE</name>
      <anchorfile>group__avr__stdlib.html</anchorfile>
      <anchor>ga73efe787c131b385070f25d18b7c9aa4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_string</name>
    <title>&lt;string.h&gt;: Strings</title>
    <filename>group__avr__string.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>_FFS</name>
      <anchorfile>group__avr__string.html</anchorfile>
      <anchor>gaedb0e7bb7333d6122472acddb5df20ac</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>strdup</name>
      <anchorfile>group__avr__string.html</anchorfile>
      <anchor>ga8569f20e38a030b5a28fd951abec2c9b</anchor>
      <arglist>(const char *s1)</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>strlcat</name>
      <anchorfile>group__avr__string.html</anchorfile>
      <anchor>ga63e609bfa0d354dcd7e35b297c2e6fdd</anchor>
      <arglist>(char *, const char *, size_t)</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>strlcpy</name>
      <anchorfile>group__avr__string.html</anchorfile>
      <anchor>ga64bc119cf084d1ecfd95098994597f12</anchor>
      <arglist>(char *, const char *, size_t)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>strtok</name>
      <anchorfile>group__avr__string.html</anchorfile>
      <anchor>ga6ace85338eafe22a0ff52c00eb9779b8</anchor>
      <arglist>(char *, const char *)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_time</name>
    <title>&lt;time.h&gt;: Time</title>
    <filename>group__avr__time.html</filename>
    <class kind="struct">tm</class>
    <class kind="struct">week_date</class>
    <member kind="define">
      <type>#define</type>
      <name>NTP_OFFSET</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gaa7c3e93bff90674e9e74de6bdec85613</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ONE_DAY</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga44eb26d33eb6c5e235c6cbfa377f60ae</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ONE_DEGREE</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga33af778316d8cac3769692c311825a52</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ONE_HOUR</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gae8dbeb3eda9f88f8f222adea789ec63d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UNIX_OFFSET</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga0643a5b723dde6420363401dffc2e67e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t</type>
      <name>time_t</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga3346b04b0420b32ccf6b706551b70762</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>_MONTHS_</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga9ea260b185fe2c5bb23c64f5954c28aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>_WEEK_DAYS_</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gae6535271b8e2884a9ed9fe92ecfd87e1</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>asctime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga68a0e9d22417dfcf9c0be64261352e64</anchor>
      <arglist>(const struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>asctime_r</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gaa1128e048925e3b208213febb8573b88</anchor>
      <arglist>(const struct tm *timeptr, char *buf)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>ctime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga72a5383fadfbaa033ac36e9e45b0bd6d</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>ctime_r</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga34fa7766f6b0a8a74a41683693eabad9</anchor>
      <arglist>(const time_t *timer, char *buf)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>daylight_seconds</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga71762e360eaab527a334d9114f68aa21</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>int32_t</type>
      <name>difftime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga1732c46d47837951d399755899af5240</anchor>
      <arglist>(time_t time1, time_t time0)</arglist>
    </member>
    <member kind="function">
      <type>int16_t</type>
      <name>equation_of_time</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga633438d79d5dd6f946c8b6ca4ee32111</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>fatfs_time</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga0e2cda2d867a784b4dfc938ad56bb94f</anchor>
      <arglist>(const struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>gm_sidereal</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gaae725f7944578bf948bb5c44588b7b7c</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>struct tm *</type>
      <name>gmtime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gae76343d6a51c93b9318ac0ced05225cd</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>gmtime_r</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga7cb461118a6aeebeaacd48ee127b952a</anchor>
      <arglist>(const time_t *timer, struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>is_leap_year</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga70762b48818b0b5bfde86973cbdd6c86</anchor>
      <arglist>(int16_t year)</arglist>
    </member>
    <member kind="function">
      <type>struct week_date *</type>
      <name>iso_week_date</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gafe4b12d1be1149c088d61c7cddc945e5</anchor>
      <arglist>(int year, int yday)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>iso_week_date_r</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga8a4fd6a03c19f96f656316eba80b9c9a</anchor>
      <arglist>(int year, int yday, struct week_date *)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>isotime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga1a85cae701479294885ec9f3c6b2fade</anchor>
      <arglist>(const struct tm *tmptr)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>isotime_r</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gae200cfc8cc7309d0ce5fef3ee490dc47</anchor>
      <arglist>(const struct tm *, char *)</arglist>
    </member>
    <member kind="function">
      <type>unsigned long</type>
      <name>lm_sidereal</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga9136313e476a6277cb6ca759542b83e3</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>struct tm *</type>
      <name>localtime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gaa9336f97d394e8e02d97b9ff4fb0aca2</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>localtime_r</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gacd9715002deaf28b432a8f2b54dd9aa5</anchor>
      <arglist>(const time_t *timer, struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>time_t</type>
      <name>mk_gmtime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gab687d0e66f0c74b9723ff9c718ea57da</anchor>
      <arglist>(const struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>time_t</type>
      <name>mktime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gae114b9ba0fce4e2d6297e0e66768bb75</anchor>
      <arglist>(struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>month_length</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gaa743162fd5cbcbaf67371785e812436a</anchor>
      <arglist>(int16_t year, uint8_t month)</arglist>
    </member>
    <member kind="function">
      <type>int8_t</type>
      <name>moon_phase</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga6a79f1da4cd50f398a05679cddc3fede</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_dst</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga3b0c7666163850cc95f0f231f388eb2c</anchor>
      <arglist>(int(*)(const time_t *, int32_t *))</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_position</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga90f87ccc97456fe3dcd867ccf71538ee</anchor>
      <arglist>(int32_t latitude, int32_t longitude)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_system_time</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga02663b926feab893c0e0ec1f29b3c315</anchor>
      <arglist>(time_t timestamp)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_zone</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gab4c127c1405b775b6d25a610d3fc26c3</anchor>
      <arglist>(int32_t)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>solar_declination</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga34ba5295b14e45cef7f99c386fff0230</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>time_t</type>
      <name>solar_noon</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga35cf3a10ed003c844c54b7185fdb5b8a</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>strftime</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>gaa0344a18cbf5cfc45ffdf43890eac51c</anchor>
      <arglist>(char *s, size_t maxsize, const char *format, const struct tm *timeptr)</arglist>
    </member>
    <member kind="function">
      <type>time_t</type>
      <name>sun_rise</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga21a491a92bce972d041e2194b4577943</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>time_t</type>
      <name>sun_set</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga0865e7b9f075981fafaabd0964de2d7f</anchor>
      <arglist>(const time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>system_tick</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga15aea81a8985a4c3cc9c8b00a06f5d31</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>time_t</type>
      <name>time</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga26c7d1dbf93fa8c23c5effbacec91f8c</anchor>
      <arglist>(time_t *timer)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>week_of_month</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga53e37af6d547a0afa624fc2230c8db7a</anchor>
      <arglist>(const struct tm *timeptr, uint8_t start)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>week_of_year</name>
      <anchorfile>group__avr__time.html</anchorfile>
      <anchor>ga6e351ab03dbd6cb078dd48d1f40b9133</anchor>
      <arglist>(const struct tm *timeptr, uint8_t start)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_boot</name>
    <title>&lt;avr/boot.h&gt;: Bootloader Support Utilities</title>
    <filename>group__avr__boot.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>boot_is_spm_interrupt</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gacd6843d51a4fa7d68f3c4bc5aa393a3e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_lock_bits_set</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gadaa352690ac1c82c535da9d8faf77590</anchor>
      <arglist>(lock_bits)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_lock_bits_set_safe</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gac80bd4f863ff7645204adb0eda3556ea</anchor>
      <arglist>(lock_bits)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_lock_fuse_bits_get</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gad2cbdea59ffec2e77ee2e63106459797</anchor>
      <arglist>(address)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_page_erase</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga7249d12e06789cd306583abf7def8176</anchor>
      <arglist>(address)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_page_erase_safe</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga5c345350a2fb349619b859dcf0dc0466</anchor>
      <arglist>(address)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_page_fill</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga61add108c778273ff68233d044c3acca</anchor>
      <arglist>(address, data)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_page_fill_safe</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga3f549e2ef9ca0f119a7070064c4dc372</anchor>
      <arglist>(address, data)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_page_write</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga013d6d8c679ebdbc0e5fac179c38c9aa</anchor>
      <arglist>(address)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_page_write_safe</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga107ad4c789e5c498062947d394ed98d5</anchor>
      <arglist>(address)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_rww_busy</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gaaba5a29359af671a37bccaa6a77bca7c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_rww_enable</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga8d2baaca2991318e0b06fdf9a5264923</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_rww_enable_safe</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gaeb0dba1dd9d338516a94c0bd8a8db78a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_signature_byte_get</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gaf375d2543ba38dc56697b4f4bc37a717</anchor>
      <arglist>(addr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_spm_busy</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gae50dc01180110f30093a92fe64fe1565</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_spm_busy_wait</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga24900c15109e8b419736d4b81b093fb8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_spm_interrupt_disable</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gaf1e01d09c839793a6c0c990b1c5ae8b3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>boot_spm_interrupt_enable</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga072040fce54f112355cb9ebf9ce41dd7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BOOTLOADER_SECTION</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga8a60eb0985d40ff71c42bb18f0f5789e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GET_EXTENDED_FUSE_BITS</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gaf08aabaebbd69da659357f402d4d28ce</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GET_HIGH_FUSE_BITS</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>ga44e70c9662e7ac06484144c15bc69aea</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GET_LOCK_BITS</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gae12d288a22cfbfa9d0cde12b1a779bfe</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GET_LOW_FUSE_BITS</name>
      <anchorfile>group__avr__boot.html</anchorfile>
      <anchor>gac68c37ecf2354ba2af6e08379d65899f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_cpufunc</name>
    <title>&lt;avr/cpufunc.h&gt;: Special AVR CPU functions</title>
    <filename>group__avr__cpufunc.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>_MemoryBarrier</name>
      <anchorfile>group__avr__cpufunc.html</anchorfile>
      <anchor>ga1060ab52075a71b61c88636e204321b2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_NOP</name>
      <anchorfile>group__avr__cpufunc.html</anchorfile>
      <anchor>ga46388d9db8422abfea56ae2323f7a77c</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_eeprom</name>
    <title>&lt;avr/eeprom.h&gt;: EEPROM handling</title>
    <filename>group__avr__eeprom.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>EEMEM</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga79a42ec6c6c8bbbe6e34ed57a52aac59</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>eeprom_busy_wait</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga44c64ebb425fcb34b8d4dc19c4079869</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>eeprom_is_ready</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga7ccd65b76bb9ed5ee03855779bae28de</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_read_block</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga0ebd0e867b6f4a03d053801d3508f8de</anchor>
      <arglist>(void *__dst, const void *__src, size_t __n)</arglist>
    </member>
    <member kind="function">
      <type>uint8_t</type>
      <name>eeprom_read_byte</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga2d4ee8b92a592c764785fb5e4af5662b</anchor>
      <arglist>(const uint8_t *__p) __ATTR_PURE__</arglist>
    </member>
    <member kind="function">
      <type>uint32_t</type>
      <name>eeprom_read_dword</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga88df934c94c037b10ebeb337a6883a74</anchor>
      <arglist>(const uint32_t *__p) __ATTR_PURE__</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>eeprom_read_float</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga88da5e9bf80b4acded6018314baab6cd</anchor>
      <arglist>(const float *__p) __ATTR_PURE__</arglist>
    </member>
    <member kind="function">
      <type>uint16_t</type>
      <name>eeprom_read_word</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>gabeef2e14398b47268f88462b3d7738dc</anchor>
      <arglist>(const uint16_t *__p) __ATTR_PURE__</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_update_block</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>gaa42f9b445115c9bbbeca19dab6f7fba9</anchor>
      <arglist>(const void *__src, void *__dst, size_t __n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_update_byte</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga63aee2719099e8435e8584d4b3e51991</anchor>
      <arglist>(uint8_t *__p, uint8_t __value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_update_dword</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>gaab79c4b2b5e8159d5f37d26d11d3f954</anchor>
      <arglist>(uint32_t *__p, uint32_t __value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_update_float</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga106a2e703b12cad4d35380b1a69ac586</anchor>
      <arglist>(float *__p, float __value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_update_word</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga131cff4e1ae5fcdf5685cab524ea4553</anchor>
      <arglist>(uint16_t *__p, uint16_t __value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_write_block</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>gac5c2be42eb170be7a26fe8b7cce4bc4d</anchor>
      <arglist>(const void *__src, void *__dst, size_t __n)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_write_byte</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga682e0b2ca0badd47e1e30e61617e1361</anchor>
      <arglist>(uint8_t *__p, uint8_t __value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_write_dword</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga012c7fa61d9695b7f0f5187d2560e598</anchor>
      <arglist>(uint32_t *__p, uint32_t __value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_write_float</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>gac738db5e12a0369d332ca43563755095</anchor>
      <arglist>(float *__p, float __value)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>eeprom_write_word</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga38e4426e45512adb5e33bf8eff20ab41</anchor>
      <arglist>(uint16_t *__p, uint16_t __value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_EEPUT</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>gae63647a896a731935077dbeb0a1b52ef</anchor>
      <arglist>(addr, val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__EEPUT</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>gae436d1b201970ad3836f663982d81dbf</anchor>
      <arglist>(addr, val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_EEGET</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>ga98c859a256541fe119e47ac4fdfd1ebd</anchor>
      <arglist>(var, addr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__EEGET</name>
      <anchorfile>group__avr__eeprom.html</anchorfile>
      <anchor>gac76dc1465b220dfcdd9aa7300a5cfe6c</anchor>
      <arglist>(var, addr)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_fuse</name>
    <title>&lt;avr/fuse.h&gt;: Fuse Support</title>
    <filename>group__avr__fuse.html</filename>
  </compound>
  <compound kind="group">
    <name>avr_interrupts</name>
    <title>&lt;avr/interrupt.h&gt;: Interrupts</title>
    <filename>group__avr__interrupts.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>sei</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>gaad5ebd34cb344c26ac87594f79b06b73</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>cli</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga68c330e94fe121eba993e5a5973c3162</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>gad28590624d422cdf30d626e0a506255f</anchor>
      <arglist>(vector, attributes)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SIGNAL</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga67cd0dea412157775c2f2a3ffe9fb8ff</anchor>
      <arglist>(vector)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>EMPTY_INTERRUPT</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga751c22101f7e8f2fbe792c64a81f8dba</anchor>
      <arglist>(vector)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_ALIAS</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>gade46eb4e42cc9d56c19b1f91448f1b76</anchor>
      <arglist>(vector, target_vector)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>reti</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga3b991e8168db8fc866e31f9a6d10533b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>BADISR_vect</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga1f6459a85cda682b2163a20af03ac744</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_BLOCK</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga5fc50a0507a58e16aca4c70345ddac6a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_NOBLOCK</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga44569cb914d2aaf8fbb436f8f7c4ca68</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_NAKED</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>ga8b4c7e44627db0a60d676213add42d64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ISR_ALIASOF</name>
      <anchorfile>group__avr__interrupts.html</anchorfile>
      <anchor>gaa87c0c624b62f40a17539be6946c3e26</anchor>
      <arglist>(target_vector)</arglist>
    </member>
    <docanchor file="group__avr__interrupts">attr_interrupt</docanchor>
    <docanchor file="group__avr__interrupts">avr_signames</docanchor>
  </compound>
  <compound kind="group">
    <name>avr_io</name>
    <title>&lt;avr/io.h&gt;: AVR device-specific IO definitions</title>
    <filename>group__avr__io.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>_PROTECTED_WRITE</name>
      <anchorfile>group__avr__io.html</anchorfile>
      <anchor>gaaaa396e67cc85f68fa0474d70edf3d4c</anchor>
      <arglist>(reg, value)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_lock</name>
    <title>&lt;avr/lock.h&gt;: Lockbit Support</title>
    <filename>group__avr__lock.html</filename>
  </compound>
  <compound kind="group">
    <name>avr_pgmspace</name>
    <title>&lt;avr/pgmspace.h&gt;: Program Space Utilities</title>
    <filename>group__avr__pgmspace.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>PGM_P</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga963f816fc88a5d8479c285ed4c630229</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_byte</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga73084a8bbde259ffae72980354b3f027</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_byte_far</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga0d4d7f6231716747c52b969d4febdeed</anchor>
      <arglist>(address_long)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_byte_near</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga88d7dd4863f87530e1a34ece430a587c</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_dword</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gabb68859ac5dfa6a09ac048b4037a83b6</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_dword_far</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga309908c1b7430f0d140edb78fcf8f2b9</anchor>
      <arglist>(address_long)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_dword_near</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga7fa92c0a662403a643859e0f33b0a182</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_float</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga7911bddb066a8a038efc4b7857728fa8</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_float_far</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga4a5a593aa84062ca08b3f2d564a6f466</anchor>
      <arglist>(address_long)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_float_near</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga066040df814dabc7980cd1422508b46b</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_ptr</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gac9f61931fd8cf4cf4764744dbaee81b6</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_ptr_far</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gaa7756dce7bcc8ba46e648981fafebbf2</anchor>
      <arglist>(address_long)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_ptr_near</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gadcfd4df2e156efc1f6a380dfc58a5b81</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_word</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga32d8ab354156f4b1ffdb77a275ba6223</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_word_far</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gad7082c45c2c96f015c76eff1ad00a99a</anchor>
      <arglist>(address_long)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>pgm_read_word_near</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gaf51eeaa847dd2668d2a66b70ecfb7398</anchor>
      <arglist>(address_short)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PGM_VOID_P</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga84a61d55b7efefabd8419e28f02704f9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PROGMEM</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga75acaba9e781937468d0911423bc0c35</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PSTR</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga05ca900ebf7cd121be73c654d9ccb3eb</anchor>
      <arglist>(s)</arglist>
    </member>
    <member kind="typedef">
      <type>char PROGMEM</type>
      <name>prog_char</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gaa475b6b81fd8b34de45695da1da523b6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int16_t PROGMEM</type>
      <name>prog_int16_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gaafc910d0b2c4d76afffa4710b98df6fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int32_t PROGMEM</type>
      <name>prog_int32_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gaa839901aa518fb43d361588dd8d2b44b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int64_t PROGMEM</type>
      <name>prog_int64_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga5b1f9927f06d841e9ac07af62e71cfef</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>int8_t PROGMEM</type>
      <name>prog_int8_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga48c7cb011ea5f82f4b73df40e07dff46</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned char PROGMEM</type>
      <name>prog_uchar</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga7d4701843a2019e3ef5a9866dc7586ed</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint16_t PROGMEM</type>
      <name>prog_uint16_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga93ec00229866bf6a125384ad08cefa73</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint32_t PROGMEM</type>
      <name>prog_uint32_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga31bad0d22ead95a41e725c38ea63eb26</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t PROGMEM</type>
      <name>prog_uint64_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gaa50eebe90a40e0276bcc49ea0482b211</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint8_t PROGMEM</type>
      <name>prog_uint8_t</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga39235a28487ae7790ce5f4c8178c8ed7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void PROGMEM</type>
      <name>prog_void</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>gadb50761b9f19d45449445208778ee420</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>strtok_P</name>
      <anchorfile>group__avr__pgmspace.html</anchorfile>
      <anchor>ga652cbaf54885c73c7ebbefe73524fa22</anchor>
      <arglist>(char *s, PGM_P delim)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_power</name>
    <title>&lt;avr/power.h&gt;: Power Reduction Management</title>
    <filename>group__avr__power.html</filename>
    <docanchor file="group__avr__power">avr_powermacros</docanchor>
  </compound>
  <compound kind="group">
    <name>avr_sfr_notes</name>
    <title>Additional notes from &lt;avr/sfr_defs.h&gt;</title>
    <filename>group__avr__sfr__notes.html</filename>
  </compound>
  <compound kind="group">
    <name>avr_sfr</name>
    <title>&lt;avr/sfr_defs.h&gt;: Special function registers</title>
    <filename>group__avr__sfr.html</filename>
    <subgroup>avr_sfr_notes</subgroup>
    <member kind="define">
      <type>#define</type>
      <name>_BV</name>
      <anchorfile>group__avr__sfr.html</anchorfile>
      <anchor>ga11643f271076024c395a93800b3d9546</anchor>
      <arglist>(bit)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>bit_is_set</name>
      <anchorfile>group__avr__sfr.html</anchorfile>
      <anchor>ga3b034cb1d74b9addc7599bd6ea6bd0d9</anchor>
      <arglist>(sfr, bit)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>bit_is_clear</name>
      <anchorfile>group__avr__sfr.html</anchorfile>
      <anchor>gad188fb0fbfd923bdb01294072367d024</anchor>
      <arglist>(sfr, bit)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>loop_until_bit_is_set</name>
      <anchorfile>group__avr__sfr.html</anchorfile>
      <anchor>gaaf6857fa882da35f8685e2001e5c3bbe</anchor>
      <arglist>(sfr, bit)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>loop_until_bit_is_clear</name>
      <anchorfile>group__avr__sfr.html</anchorfile>
      <anchor>gabf06c3d703cf5adc691c0be5d72e7839</anchor>
      <arglist>(sfr, bit)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_signature</name>
    <title>&lt;avr/signature.h&gt;: Signature Support</title>
    <filename>group__avr__signature.html</filename>
  </compound>
  <compound kind="group">
    <name>avr_sleep</name>
    <title>&lt;avr/sleep.h&gt;: Power Management and Sleep Modes</title>
    <filename>group__avr__sleep.html</filename>
    <member kind="function">
      <type>void</type>
      <name>sleep_cpu</name>
      <anchorfile>group__avr__sleep.html</anchorfile>
      <anchor>ga157b2578d95309c197b739f812938d94</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sleep_disable</name>
      <anchorfile>group__avr__sleep.html</anchorfile>
      <anchor>gaeae22433a78fd8d50f915fb68c416efd</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>sleep_enable</name>
      <anchorfile>group__avr__sleep.html</anchorfile>
      <anchor>ga475174a7aa4eda03dfa7a4483e400a9e</anchor>
      <arglist>(void)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_version</name>
    <title>&lt;avr/version.h&gt;: avr-libc version macros</title>
    <filename>group__avr__version.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>__AVR_LIBC_DATE_</name>
      <anchorfile>group__avr__version.html</anchorfile>
      <anchor>ga09c3581176726e46963330ae12270bf6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__AVR_LIBC_DATE_STRING__</name>
      <anchorfile>group__avr__version.html</anchorfile>
      <anchor>gadecde61fc0ef6e31f4dd6c732913aba8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__AVR_LIBC_MAJOR__</name>
      <anchorfile>group__avr__version.html</anchorfile>
      <anchor>ga8a657f9b659536d9c0c3c03ca441bb00</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__AVR_LIBC_MINOR__</name>
      <anchorfile>group__avr__version.html</anchorfile>
      <anchor>gaaabd42be463ab59be357639b50bd722c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__AVR_LIBC_REVISION__</name>
      <anchorfile>group__avr__version.html</anchorfile>
      <anchor>ga45476ad3a5db9be5a45d098bb1b093b1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__AVR_LIBC_VERSION__</name>
      <anchorfile>group__avr__version.html</anchorfile>
      <anchor>ga161e98ee1c31a16eeb71fc87dc73526f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__AVR_LIBC_VERSION_STRING__</name>
      <anchorfile>group__avr__version.html</anchorfile>
      <anchor>gaace584c6d368436814b4fe12d71230f9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>avr_watchdog</name>
    <title>&lt;avr/wdt.h&gt;: Watchdog timer handling</title>
    <filename>group__avr__watchdog.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>wdt_disable</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>gab3784e1b871d61ed338da5658184b725</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>wdt_enable</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>gaf6cfea2a1b61e2530ea0c4ef8fc572b3</anchor>
      <arglist>(value)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>wdt_reset</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga9e52c54d10b6a6a7ce04aaaa4abea51f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_120MS</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga7d028bcdb4a4103549fc6fb4ec07fbcd</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_15MS</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>gad45893280f49113ffc2e67e1d741f29d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_1S</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga36302e15f38a4eeb8a328724bb8165e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_250MS</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga66d5f50cc76b92c76900d77ef577d53e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_2S</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga05fc682d276a36d8cc4e9178340ff004</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_30MS</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga057dd21dc54e71de0e20d8bd5734915d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_4S</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga752b0b1b5ba9009bc09976494313e30d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_500MS</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>gacf89fc5fb6c8aa9efaadb86872cfbcdf</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_60MS</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>ga7a5b072c51c05a34cc38111f0e6724e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>WDTO_8S</name>
      <anchorfile>group__avr__watchdog.html</anchorfile>
      <anchor>gaf074e538b2a1d5031931530f29a09fce</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>util_atomic</name>
    <title>&lt;util/atomic.h&gt; Atomically and Non-Atomically Executed Code Blocks</title>
    <filename>group__util__atomic.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>ATOMIC_BLOCK</name>
      <anchorfile>group__util__atomic.html</anchorfile>
      <anchor>gaaaea265b31dabcfb3098bec7685c39e4</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ATOMIC_FORCEON</name>
      <anchorfile>group__util__atomic.html</anchorfile>
      <anchor>ga92b11103b4b3b000a3190f0d26ba7062</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ATOMIC_RESTORESTATE</name>
      <anchorfile>group__util__atomic.html</anchorfile>
      <anchor>ga362c18b15a09703e42e1c246c47420ef</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NONATOMIC_BLOCK</name>
      <anchorfile>group__util__atomic.html</anchorfile>
      <anchor>ga6e195ee2117559a25f77fbba9054674a</anchor>
      <arglist>(type)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NONATOMIC_FORCEOFF</name>
      <anchorfile>group__util__atomic.html</anchorfile>
      <anchor>gafb959d7d00d2d790b58d0e9880ea255a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NONATOMIC_RESTORESTATE</name>
      <anchorfile>group__util__atomic.html</anchorfile>
      <anchor>gab075653bf638fae9db049575741d3152</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>util_crc</name>
    <title>&lt;util/crc16.h&gt;: CRC Computations</title>
    <filename>group__util__crc.html</filename>
    <member kind="function" static="yes">
      <type>static __inline__ uint16_t</type>
      <name>_crc16_update</name>
      <anchorfile>group__util__crc.html</anchorfile>
      <anchor>ga95371c87f25b0a2497d9cba13190847f</anchor>
      <arglist>(uint16_t __crc, uint8_t __data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __inline__ uint8_t</type>
      <name>_crc8_ccitt_update</name>
      <anchorfile>group__util__crc.html</anchorfile>
      <anchor>gab27eaaef6d7fd096bd7d57bf3f9ba083</anchor>
      <arglist>(uint8_t __crc, uint8_t __data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __inline__ uint16_t</type>
      <name>_crc_ccitt_update</name>
      <anchorfile>group__util__crc.html</anchorfile>
      <anchor>ga1c1d3ad875310cbc58000e24d981ad20</anchor>
      <arglist>(uint16_t __crc, uint8_t __data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __inline__ uint8_t</type>
      <name>_crc_ibutton_update</name>
      <anchorfile>group__util__crc.html</anchorfile>
      <anchor>ga37b2f691ebbd917e36e40b096f78d996</anchor>
      <arglist>(uint8_t __crc, uint8_t __data)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static __inline__ uint16_t</type>
      <name>_crc_xmodem_update</name>
      <anchorfile>group__util__crc.html</anchorfile>
      <anchor>gaca726c22a1900f9bad52594c8846115f</anchor>
      <arglist>(uint16_t __crc, uint8_t __data)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>util_delay</name>
    <title>&lt;util/delay.h&gt;: Convenience functions for busy-wait delay loops</title>
    <filename>group__util__delay.html</filename>
    <member kind="function">
      <type>void</type>
      <name>_delay_ms</name>
      <anchorfile>group__util__delay.html</anchorfile>
      <anchor>gad22e7a36b80e2f917324dc43a425e9d3</anchor>
      <arglist>(double __ms)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_delay_us</name>
      <anchorfile>group__util__delay.html</anchorfile>
      <anchor>gab20bfffeacc678cb960944f5519c0c4f</anchor>
      <arglist>(double __us)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>util_delay_basic</name>
    <title>&lt;util/delay_basic.h&gt;: Basic busy-wait delay loops</title>
    <filename>group__util__delay__basic.html</filename>
    <member kind="function">
      <type>void</type>
      <name>_delay_loop_1</name>
      <anchorfile>group__util__delay__basic.html</anchorfile>
      <anchor>ga4e3957917c4c447d0f9166dac881b4e3</anchor>
      <arglist>(uint8_t __count)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>_delay_loop_2</name>
      <anchorfile>group__util__delay__basic.html</anchorfile>
      <anchor>ga74a94fec42bac9f1ff31fd443d419a6a</anchor>
      <arglist>(uint16_t __count)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>util_parity</name>
    <title>&lt;util/parity.h&gt;: Parity bit generation</title>
    <filename>group__util__parity.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>parity_even_bit</name>
      <anchorfile>group__util__parity.html</anchorfile>
      <anchor>ga4180eaa9b8f27f8efc589f3a3ba1724c</anchor>
      <arglist>(val)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>util_setbaud</name>
    <title>&lt;util/setbaud.h&gt;: Helper macros for baud rate calculations</title>
    <filename>group__util__setbaud.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>BAUD_TOL</name>
      <anchorfile>group__util__setbaud.html</anchorfile>
      <anchor>gaddaa61b72aca51ed4835978d500f8755</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UBRR_VALUE</name>
      <anchorfile>group__util__setbaud.html</anchorfile>
      <anchor>ga0b029ff580b042a27aaac4bd2ec925e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UBRRH_VALUE</name>
      <anchorfile>group__util__setbaud.html</anchorfile>
      <anchor>ga8188daef772f5eb3fc81dfee168905e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>UBRRL_VALUE</name>
      <anchorfile>group__util__setbaud.html</anchorfile>
      <anchor>ga39ebec5d04e582b7b6ed9a72b973983c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USE_2X</name>
      <anchorfile>group__util__setbaud.html</anchorfile>
      <anchor>ga6977ce48ae3197f3f016b85d98380509</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>util_twi</name>
    <title>&lt;util/twi.h&gt;: TWI bit mask definitions</title>
    <filename>group__util__twi.html</filename>
    <member kind="define">
      <type>#define</type>
      <name>TW_START</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga8ec630c2063d0353c53d140b99382d80</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_REP_START</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga8dce3e4b7e35355a8add9ed63d1fa3ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga85cae14b4190042335d25ed9a1b72369</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_SLA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gaca64b973c51d6548a962c1d7cae0663e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga4c28186053b5298305b131ad3e1111f7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga91c107a74d268f8578d866ed1bbbd4f3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MT_ARB_LOST</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga5959251c4bd80f48b5a029447d86adb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_ARB_LOST</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gad85222308836bfbe099255662ffb510c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gac16504b87d15d83b97bb0ce61577bb40</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_SLA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga655325b6f8a1818103b126cc3774d8e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga3c8c79b2ecb8d22358839890161cc33b</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_MR_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga532f0ffa12f684346c74a5cbec15950e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga0ecd2ca38d00279194460e65028a0533</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_ARB_LOST_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga1064659d0758206d43d69cd582d1f5da</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga2de84bf7cbf1cd7ae43a6e0f0eeca719</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gacc1837317b1d45e9bb49b8e83cfe6d42</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_ST_LAST_DATA</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gaf92d03d0051d47f5b9375f0ef9293d64</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga9e9e761d674793772e6b3f77fc9d7fab</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_ARB_LOST_SLA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga1c3ed5ed345d17972002b9fd07f4f829</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_GCALL_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga6eb3b4230887a8b73d7787ff231ea911</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_ARB_LOST_GCALL_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga4756103341a65e0e3889996cdf15b2fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gac50f1a59c74c3109d4913dbecfb472c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga6c0fa701fe9d96b0b7df29e8af154f94</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_GCALL_DATA_ACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga4abaad098fd92bed63719ffb01802c8d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_GCALL_DATA_NACK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga42234278f9b01b9af75dbbc617b97890</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_SR_STOP</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga98d2570f965790884cf1774e716ec629</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_NO_INFO</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gabcff70642634cb53e9d8e93872f70c90</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_BUS_ERROR</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga90d373160b1d0a3f0c454af83c57df71</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_STATUS_MASK</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga8d3aca0acc182f459a51797321728168</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_STATUS</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>ga4440385d1818b4fe89b20341ea47b75a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_READ</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gaf40f13cadca25e0a83dc096858907819</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TW_WRITE</name>
      <anchorfile>group__util__twi.html</anchorfile>
      <anchor>gac8a7cb1a91946c6e3955608a91371148</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>deprecated_items</name>
    <title>&lt;compat/deprecated.h&gt;: Deprecated items</title>
    <filename>group__deprecated__items.html</filename>
    <member kind="function" static="yes">
      <type>static __inline__ void</type>
      <name>timer_enable_int</name>
      <anchorfile>group__deprecated__items.html</anchorfile>
      <anchor>ga46f0b87ccc2ab63dea1ff28207270b82</anchor>
      <arglist>(unsigned char ints)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>enable_external_int</name>
      <anchorfile>group__deprecated__items.html</anchorfile>
      <anchor>ga9a3fe82a7199e9e84f7be6a5f23127bc</anchor>
      <arglist>(mask)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INTERRUPT</name>
      <anchorfile>group__deprecated__items.html</anchorfile>
      <anchor>gaa0b2d3a87492967c01615f32f30d06d5</anchor>
      <arglist>(signame)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>inp</name>
      <anchorfile>group__deprecated__items.html</anchorfile>
      <anchor>ga5cfa4750a0633c34c7a361d8fd62c042</anchor>
      <arglist>(port)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>outp</name>
      <anchorfile>group__deprecated__items.html</anchorfile>
      <anchor>gaab324bd721e821e275f00c3478e240c9</anchor>
      <arglist>(val, port)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>inb</name>
      <anchorfile>group__deprecated__items.html</anchorfile>
      <anchor>gad6488a48837d179b1833e2f48dac9665</anchor>
      <arglist>(port)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>outb</name>
      <anchorfile>group__deprecated__items.html</anchorfile>
      <anchor>ga3a3b4c1ddf0c05701f933d70de330f08</anchor>
      <arglist>(port, val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>sbi</name>
      <anchorfile>group__deprecated__items.html</anchorfile>
      <anchor>ga014ef751e83f97569c06f3cdd888f3f7</anchor>
      <arglist>(port, bit)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>cbi</name>
      <anchorfile>group__deprecated__items.html</anchorfile>
      <anchor>ga08ee265dc07048dbb5a8b6c84551d520</anchor>
      <arglist>(port, bit)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>compat_ina90</name>
    <title>&lt;compat/ina90.h&gt;: Compatibility with IAR EWB 3.x</title>
    <filename>group__compat__ina90.html</filename>
  </compound>
  <compound kind="group">
    <name>demos</name>
    <title>Demo projects</title>
    <filename>group__demos.html</filename>
    <subgroup>asmdemo</subgroup>
    <subgroup>demo_project</subgroup>
    <subgroup>largedemo</subgroup>
    <subgroup>stdiodemo</subgroup>
    <subgroup>twi_demo</subgroup>
  </compound>
  <compound kind="group">
    <name>asmdemo</name>
    <title>Combining C and assembly source files</title>
    <filename>group__asmdemo.html</filename>
    <docanchor file="group__asmdemo" title="Hardware setup">asmdemo_hw</docanchor>
    <docanchor file="group__asmdemo" title="A code walkthrough">asmdemo_code</docanchor>
    <docanchor file="group__asmdemo" title="asmdemo.c">asmdemo_main</docanchor>
    <docanchor file="group__asmdemo" title="project.h">asmdemo_project</docanchor>
    <docanchor file="group__asmdemo" title="isrs.S">asmdemo_isrs</docanchor>
    <docanchor file="group__asmdemo" title="The source code">asmdemo_src</docanchor>
  </compound>
  <compound kind="group">
    <name>demo_project</name>
    <title>A simple project</title>
    <filename>group__demo__project.html</filename>
    <docanchor file="group__demo__project" title="The Project">demo_project_desc</docanchor>
    <docanchor file="group__demo__project">demo_project_schematic</docanchor>
    <docanchor file="group__demo__project" title="The Source Code">demo_project_src</docanchor>
    <docanchor file="group__demo__project" title="Compiling and Linking">demo_project_compile</docanchor>
    <docanchor file="group__demo__project" title="Examining the Object File">demo_project_obj</docanchor>
    <docanchor file="group__demo__project" title="Linker Map Files">demo_project_map</docanchor>
    <docanchor file="group__demo__project" title="Generating Intel Hex Files">demo_ihex</docanchor>
    <docanchor file="group__demo__project" title="Letting Make Build the Project">demo_make</docanchor>
    <docanchor file="group__demo__project" title="Reference to the source code">demo_sourceref</docanchor>
  </compound>
  <compound kind="group">
    <name>largedemo</name>
    <title>A more sophisticated project</title>
    <filename>group__largedemo.html</filename>
    <docanchor file="group__largedemo" title="Hardware setup">largedemo_hw</docanchor>
    <docanchor file="group__largedemo" title="Functional overview">largedemo_overview</docanchor>
    <docanchor file="group__largedemo" title="A code walkthrough">largedemo_code</docanchor>
    <docanchor file="group__largedemo" title="Part 1: Macro definitions">largedemo_code_p1</docanchor>
    <docanchor file="group__largedemo" title="Part 2: Variable definitions">largedemo_code_p2</docanchor>
    <docanchor file="group__largedemo" title="Part 3: Interrupt service routines">largedemo_code_p3</docanchor>
    <docanchor file="group__largedemo" title="Part 4: Auxiliary functions">largedemo_code_p4</docanchor>
    <docanchor file="group__largedemo" title="Part 5: main()">largedemo_code_p5</docanchor>
    <docanchor file="group__largedemo" title="The source code">largedemo_src</docanchor>
  </compound>
  <compound kind="group">
    <name>stdiodemo</name>
    <title>Using the standard IO facilities</title>
    <filename>group__stdiodemo.html</filename>
    <docanchor file="group__stdiodemo" title="Hardware setup">stdiodemo_hw</docanchor>
    <docanchor file="group__stdiodemo" title="Functional overview">stdiodemo_overview</docanchor>
    <docanchor file="group__stdiodemo" title="A code walkthrough">stdiodemo_code</docanchor>
    <docanchor file="group__stdiodemo" title="stdiodemo.c">stdiodemo_main</docanchor>
    <docanchor file="group__stdiodemo" title="defines.h">stdiodemo_defines</docanchor>
    <docanchor file="group__stdiodemo" title="hd44780.h">stdiodemo_hd44780_h</docanchor>
    <docanchor file="group__stdiodemo" title="hd44780.c">stdiodemo_hd44780_c</docanchor>
    <docanchor file="group__stdiodemo" title="lcd.h">stdiodemo_lcd_h</docanchor>
    <docanchor file="group__stdiodemo" title="lcd.c">stdiodemo_lcd_c</docanchor>
    <docanchor file="group__stdiodemo" title="uart.h">stdiodemo_uart_h</docanchor>
    <docanchor file="group__stdiodemo" title="uart.c">stdiodemo_uart_c</docanchor>
    <docanchor file="group__stdiodemo" title="The source code">stdiodemo_src</docanchor>
  </compound>
  <compound kind="group">
    <name>twi_demo</name>
    <title>Example using the two-wire interface (TWI)</title>
    <filename>group__twi__demo.html</filename>
    <docanchor file="group__twi__demo" title="Introduction into TWI">twi_demo_intro</docanchor>
    <docanchor file="group__twi__demo" title="The TWI example project">twi_demo_project</docanchor>
    <docanchor file="group__twi__demo" title="The Source Code">twi_demo_src</docanchor>
  </compound>
  <compound kind="page">
    <name>overview</name>
    <title>Toolchain Overview</title>
    <filename>overview</filename>
    <docanchor file="overview" title="Introduction">overview_introduction</docanchor>
    <docanchor file="overview" title="FSF and GNU">overview_fsf_and_gnu</docanchor>
    <docanchor file="overview" title="GCC">overview_gcc</docanchor>
    <docanchor file="overview" title="GNU Binutils">overview_binutils</docanchor>
    <docanchor file="overview" title="avr-libc">overview_avr-libc</docanchor>
    <docanchor file="overview" title="Building Software">overview_building_software</docanchor>
    <docanchor file="overview" title="AVRDUDE">overview_avrdude</docanchor>
    <docanchor file="overview" title="GDB / Insight / DDD">overview_gdb_insight_ddd</docanchor>
    <docanchor file="overview" title="AVaRICE">overview_avarice</docanchor>
    <docanchor file="overview" title="SimulAVR">overview_simulavr</docanchor>
    <docanchor file="overview" title="Utilities">overview_utilities</docanchor>
    <docanchor file="overview" title="Toolchain Distributions (Distros)">overview_distros</docanchor>
    <docanchor file="overview" title="Open Source">overview_open_source</docanchor>
  </compound>
  <compound kind="page">
    <name>malloc</name>
    <title>Memory Areas and Using malloc()</title>
    <filename>malloc</filename>
    <docanchor file="malloc" title="Introduction">malloc_intro</docanchor>
    <docanchor file="malloc" title="Internal vs. external RAM">malloc_where</docanchor>
    <docanchor file="malloc" title="Tunables for malloc()">malloc_tunables</docanchor>
    <docanchor file="malloc">malloc_extram</docanchor>
    <docanchor file="malloc" title="Implementation details">malloc_impl</docanchor>
  </compound>
  <compound kind="page">
    <name>mem_sections</name>
    <title>Memory Sections</title>
    <filename>mem_sections</filename>
    <docanchor file="mem_sections" title="The .text Section">sec_dot_text</docanchor>
    <docanchor file="mem_sections" title="The .data Section">sec_dot_data</docanchor>
    <docanchor file="mem_sections" title="The .bss Section">sec_dot_bss</docanchor>
    <docanchor file="mem_sections" title="The .eeprom Section">sec_dot_eeprom</docanchor>
    <docanchor file="mem_sections" title="The .noinit Section">sec_dot_noinit</docanchor>
    <docanchor file="mem_sections">harvard_arch</docanchor>
    <docanchor file="mem_sections" title="The .initN Sections">sec_dot_init</docanchor>
    <docanchor file="mem_sections" title="The .finiN Sections">sec_dot_fini</docanchor>
    <docanchor file="mem_sections" title="Using Sections in Assembler Code">asm_sections</docanchor>
    <docanchor file="mem_sections" title="Using Sections in C Code">c_sections</docanchor>
  </compound>
  <compound kind="page">
    <name>pgmspace</name>
    <title>Data in Program Space</title>
    <filename>pgmspace</filename>
    <docanchor file="pgmspace" title="Introduction">pgmspace_introduction</docanchor>
    <docanchor file="pgmspace" title="A Note On const">pgmspace_const</docanchor>
    <docanchor file="pgmspace" title="Storing and Retrieving Data in the Program Space">pgmspace_data</docanchor>
    <docanchor file="pgmspace" title="Storing and Retrieving Strings in the Program Space">pgmspace_strings</docanchor>
    <docanchor file="pgmspace" title="Caveats">pgmspace_caveats</docanchor>
  </compound>
  <compound kind="page">
    <name>assembler</name>
    <title>avr-libc and assembler programs</title>
    <filename>assembler</filename>
    <docanchor file="assembler" title="Introduction">ass_intro</docanchor>
    <docanchor file="assembler" title="Invoking the compiler">ass_tools</docanchor>
    <docanchor file="assembler" title="Example program">ass_example</docanchor>
    <docanchor file="assembler">ass_isr</docanchor>
    <docanchor file="assembler" title="Pseudo-ops and operators">ass_pseudoops</docanchor>
  </compound>
  <compound kind="page">
    <name>inline_asm</name>
    <title>Inline Assembler Cookbook</title>
    <filename>inline_asm</filename>
    <docanchor file="inline_asm" title="GCC asm Statement">gcc_asm</docanchor>
    <docanchor file="inline_asm" title="Assembler Code">asm_code</docanchor>
    <docanchor file="inline_asm" title="Input and Output Operands">io_ops</docanchor>
    <docanchor file="inline_asm" title="Clobbers">clobbers</docanchor>
    <docanchor file="inline_asm" title="Assembler Macros">asm_macros</docanchor>
    <docanchor file="inline_asm" title="C Stub Functions">asm_c_stubs</docanchor>
    <docanchor file="inline_asm" title="C Names Used in Assembler Code">c_names_in_asm</docanchor>
    <docanchor file="inline_asm" title="Links">links</docanchor>
  </compound>
  <compound kind="page">
    <name>library</name>
    <title>How to Build a Library</title>
    <filename>library</filename>
    <docanchor file="library" title="Introduction">library_intro</docanchor>
    <docanchor file="library" title="How the Linker Works">library_linker</docanchor>
    <docanchor file="library" title="How to Design a Library">library_design</docanchor>
    <docanchor file="library" title="Creating a Library">library_creating</docanchor>
    <docanchor file="library" title="Using a Library">library_using</docanchor>
  </compound>
  <compound kind="page">
    <name>benchmarks</name>
    <title>Benchmarks</title>
    <filename>benchmarks</filename>
    <docanchor file="benchmarks" title="A few of libc functions.">bench_libc</docanchor>
    <docanchor file="benchmarks" title="Math functions.">bench_libm</docanchor>
  </compound>
  <compound kind="page">
    <name>porting</name>
    <title>Porting From IAR to AVR GCC</title>
    <filename>porting</filename>
    <docanchor file="porting" title="Introduction">iar_porting_intro</docanchor>
    <docanchor file="porting" title="Registers">iar_porting_register</docanchor>
    <docanchor file="porting" title="Interrupt Service Routines (ISRs)">iar_porting_isr</docanchor>
    <docanchor file="porting" title="Intrinsic Routines">iar_porting_intrinsic</docanchor>
    <docanchor file="porting" title="Flash Variables">iar_porting_flash</docanchor>
    <docanchor file="porting" title="Non-Returning main()">iar_porting_non_returning_main</docanchor>
    <docanchor file="porting" title="Locking Registers">iar_porting_locking</docanchor>
  </compound>
  <compound kind="page">
    <name>FAQ</name>
    <title>Frequently Asked Questions</title>
    <filename>FAQ</filename>
    <docanchor file="FAQ" title="FAQ Index">faq_index</docanchor>
    <docanchor file="FAQ" title="My program doesn&apos;t recognize a variable updated within an interrupt routine">faq_volatile</docanchor>
    <docanchor file="FAQ" title="I get &quot;undefined reference to...&quot; for functions like &quot;sin()&quot;">faq_libm</docanchor>
    <docanchor file="FAQ" title="How to permanently bind a variable to a register?">faq_regbind</docanchor>
    <docanchor file="FAQ" title="How to modify MCUCR or WDTCR early?">faq_startup</docanchor>
    <docanchor file="FAQ" title="What is all this _BV() stuff about?">faq_use_bv</docanchor>
    <docanchor file="FAQ" title="Can I use C++ on the AVR?">faq_cplusplus</docanchor>
    <docanchor file="FAQ" title="Shouldn&apos;t I initialize all my variables?">faq_varinit</docanchor>
    <docanchor file="FAQ" title="Why do some 16-bit timer registers sometimes get trashed?">faq_16bitio</docanchor>
    <docanchor file="FAQ" title="How do I use a #define&apos;d constant in an asm statement?">faq_asmconst</docanchor>
    <docanchor file="FAQ" title="Why does the PC randomly jump around when single-stepping through my program in avr-gdb?">faq_gdboptimize</docanchor>
    <docanchor file="FAQ" title="How do I trace an assembler file in avr-gdb?">faq_asmstabs</docanchor>
    <docanchor file="FAQ" title="How do I pass an IO port as a parameter to a function?">faq_port_pass</docanchor>
    <docanchor file="FAQ" title="What registers are used by the C compiler?">faq_reg_usage</docanchor>
    <docanchor file="FAQ" title="How do I put an array of strings completely in ROM?">faq_rom_array</docanchor>
    <docanchor file="FAQ" title="How to use external RAM?">faq_ext_ram</docanchor>
    <docanchor file="FAQ" title="Which -O flag to use?">faq_optflags</docanchor>
    <docanchor file="FAQ" title="How do I relocate code to a fixed address?">faq_reloc_code</docanchor>
    <docanchor file="FAQ" title="My UART is generating nonsense!  My ATmega128 keeps crashing! Port F is completely broken!">faq_fuses</docanchor>
    <docanchor file="FAQ" title="Why do all my &quot;foo...bar&quot; strings eat up the SRAM?">faq_flashstrings</docanchor>
    <docanchor file="FAQ" title="Why does the compiler compile an 8-bit operation that uses bitwise operators into a 16-bit operation in assembly?">faq_intpromote</docanchor>
    <docanchor file="FAQ" title="How to detect RAM memory and variable overlap problems?">faq_ramoverlap</docanchor>
    <docanchor file="FAQ" title="Is it really impossible to program the ATtinyXX in C?">faq_tinyavr_c</docanchor>
    <docanchor file="FAQ" title="What is this &quot;clock skew detected&quot; message?">faq_clockskew</docanchor>
    <docanchor file="FAQ" title="Why are (many) interrupt flags cleared by writing a logical 1?">faq_intbits</docanchor>
    <docanchor file="FAQ" title="Why have &quot;programmed&quot; fuses the bit value 0?">faq_fuselow</docanchor>
    <docanchor file="FAQ" title="Which AVR-specific assembler operators are available?">faq_asmops</docanchor>
    <docanchor file="FAQ" title="Why are interrupts re-enabled in the middle of writing the stack pointer?">faq_spman</docanchor>
    <docanchor file="FAQ" title="Why are there five different linker scripts?">faq_linkerscripts</docanchor>
    <docanchor file="FAQ" title="How to add a raw binary image to linker output?">faq_binarydata</docanchor>
    <docanchor file="FAQ" title="How do I perform a software reset of the AVR?">faq_softreset</docanchor>
    <docanchor file="FAQ" title="I am using floating point math. Why is the compiled code so big? Why does my code not work?">faq_math</docanchor>
    <docanchor file="FAQ" title="What pitfalls exist when writing reentrant code?">faq_reentrant</docanchor>
    <docanchor file="FAQ" title="Why are some addresses of the EEPROM corrupted (usually address zero)?">faq_eeprom_corruption</docanchor>
    <docanchor file="FAQ" title="Why is my baud rate wrong?">faq_wrong_baud_rate</docanchor>
    <docanchor file="FAQ" title="On a device with more than 128 KiB of flash, how to make function pointers work?">faq_funcptr_gt128kib</docanchor>
    <docanchor file="FAQ" title="Why is assigning ports in a &quot;chain&quot; a bad idea?">faq_assign_chain</docanchor>
  </compound>
  <compound kind="page">
    <name>install_tools</name>
    <title>Building and Installing the GNU Tool Chain</title>
    <filename>install_tools</filename>
    <docanchor file="install_tools" title="Building and Installing under Linux, FreeBSD, and Others">install_unix</docanchor>
    <docanchor file="install_tools">path</docanchor>
    <docanchor file="install_tools" title="Required Tools">required_tools</docanchor>
    <docanchor file="install_tools" title="Optional Tools">optional_tools</docanchor>
    <docanchor file="install_tools" title="GNU Binutils for the AVR target">install_avr_binutils</docanchor>
    <docanchor file="install_tools" title="GCC for the AVR target">install_avr_gcc</docanchor>
    <docanchor file="install_tools" title="AVR LibC">install_avr_libc</docanchor>
    <docanchor file="install_tools" title="AVRDUDE">install_avrprog</docanchor>
    <docanchor file="install_tools" title="GDB for the AVR target">install_gdb</docanchor>
    <docanchor file="install_tools" title="SimulAVR">install_simulavr</docanchor>
    <docanchor file="install_tools" title="AVaRICE">install_avarice</docanchor>
    <docanchor file="install_tools" title="Building and Installing under Windows">install_windows</docanchor>
    <docanchor file="install_tools" title="Tools Required for Building the Toolchain for Windows">install_windows_tools</docanchor>
    <docanchor file="install_tools" title="Building the Toolchain for Windows">install_windows_building</docanchor>
  </compound>
  <compound kind="page">
    <name>using_tools</name>
    <title>Using the GNU tools</title>
    <filename>using_tools</filename>
    <docanchor file="using_tools" title="Options for the C compiler avr-gcc">using_avr_gcc</docanchor>
    <docanchor file="using_tools" title="Machine-specific options for the AVR">using_avr_gcc_mach_opt</docanchor>
    <docanchor file="using_tools" title="Selected general compiler options">using_sel_gcc_opts</docanchor>
    <docanchor file="using_tools">gcc_optO</docanchor>
    <docanchor file="using_tools">gcc_minusW</docanchor>
    <docanchor file="using_tools" title="Options for the assembler avr-as">using_avr_as</docanchor>
    <docanchor file="using_tools" title="Machine-specific assembler options">using_avr_as_mach_opts</docanchor>
    <docanchor file="using_tools" title="Examples for assembler options passed through the C compiler">using_avr_example</docanchor>
    <docanchor file="using_tools" title="Controlling the linker avr-ld">using_avr_ld</docanchor>
    <docanchor file="using_tools" title="Selected linker options">using_sel_ld_opts</docanchor>
    <docanchor file="using_tools" title="Passing linker options from the C compiler">using_pass_ld_opts</docanchor>
  </compound>
  <compound kind="page">
    <name>optimization</name>
    <title>Compiler optimization</title>
    <filename>optimization</filename>
    <docanchor file="optimization" title="Problems with reordering code">optim_code_reorder</docanchor>
  </compound>
  <compound kind="page">
    <name>using_avrprog</name>
    <title>Using the avrdude program</title>
    <filename>using_avrprog</filename>
  </compound>
  <compound kind="page">
    <name>release_method</name>
    <title>Release Numbering and Methodology</title>
    <filename>release_method</filename>
    <docanchor file="release_method" title="Release Version Numbering Scheme">version_info</docanchor>
    <docanchor file="release_method" title="Releasing AVR Libc">release_info</docanchor>
    <docanchor file="release_method" title="Creating an SVN branch">release_branch</docanchor>
    <docanchor file="release_method" title="Making a release">release_rolling</docanchor>
  </compound>
  <compound kind="page">
    <name>acks</name>
    <title>Acknowledgments</title>
    <filename>acks</filename>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>AVR Libc</title>
    <filename>index</filename>
    <docanchor file="index" title="Introduction">avr_libc_intro</docanchor>
    <docanchor file="index" title="General information about this library">avr_libc_general</docanchor>
    <docanchor file="index" title="Supported Devices">supp_devices</docanchor>
    <docanchor file="index">supp_dev_footnote_one</docanchor>
    <docanchor file="index">supp_dev_footnote_two</docanchor>
    <docanchor file="index">supp_dev_footnote_three</docanchor>
    <docanchor file="index">supp_dev_footnote_four</docanchor>
    <docanchor file="index" title="avr-libc License">license</docanchor>
  </compound>
</tagfile>
