/* Testcase from bug #174.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "bbit\[01\]|bgez" 5 } } */
/* { dg-final { scan-assembler-not "dsrl" } } */

typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;

typedef union
{
    void* ptr;
    uint64_t u64;
    struct
    {
        uint64_t i : 1;
        uint64_t back : 4;
        uint64_t pool : 3;
        uint64_t size :16;
        uint64_t addr :40;
    } s;
} cvmx_buf_ptr_t;

typedef enum
{
    CVMX_POW_TAG_TYPE_ORDERED = 0L,
    CVMX_POW_TAG_TYPE_ATOMIC = 1L,
    CVMX_POW_TAG_TYPE_NULL = 2L,
    CVMX_POW_TAG_TYPE_NULL_NULL = 3L
} cvmx_pow_tag_type_t;

typedef union
{
    uint64_t u64;
    struct
    {
        uint64_t vlan_valid : 1;
        uint64_t bufs : 8;
        uint64_t ip_offset : 8;
        uint64_t unassigned : 2;
        uint64_t vlan_cfi : 1;
        uint64_t vlan_id :12;
        uint64_t unassigned2 :12;
        uint64_t dec_ipcomp : 1;
        uint64_t tcp_or_udp : 1;
        uint64_t dec_ipsec : 1;
        uint64_t is_v6 : 1;
        uint64_t software : 1;
        uint64_t L4_error : 1;
        uint64_t is_frag : 1;
        uint64_t IP_exc : 1;
        uint64_t is_bcast : 1;
        uint64_t is_mcast : 1;
        uint64_t not_IP : 1;
        uint64_t rcv_error : 1;
        uint64_t err_code : 8;
    } s;
    struct
    {
        uint64_t unused1 :16;
        uint64_t vlan :16;
        uint64_t unused2 :32;
    } svlan;
    struct
    {
        uint64_t vlan_valid : 1;
        uint64_t bufs : 8;
        uint64_t unused : 8;
        uint64_t unassigned : 2;
        uint64_t vlan_cfi : 1;
        uint64_t vlan_id :12;
        uint64_t unassigned2 :16;
        uint64_t software : 1;
        uint64_t unassigned3 : 1;
        uint64_t is_rarp : 1;
        uint64_t is_arp : 1;
        uint64_t is_bcast : 1;
        uint64_t is_mcast : 1;
        uint64_t not_IP : 1;
        uint64_t rcv_error : 1;
        uint64_t err_code : 8;
    } snoip;
} cvmx_pip_wqe_word2;

typedef struct
{
    uint16_t hw_chksum;
    uint8_t unused;
    uint64_t next_ptr : 40;
    uint64_t len :16;
    uint64_t ipprt : 6;
    uint64_t qos : 3;
    uint64_t grp : 4;
    cvmx_pow_tag_type_t tag_type : 3;
    uint64_t tag :32;
    cvmx_pip_wqe_word2 word2;
    cvmx_buf_ptr_t packet_ptr;
    uint8_t packet_data[96];
} cvmx_wqe_t __attribute__ ((aligned (128)));

int test(cvmx_wqe_t * work, int x)
{
    if (work->word2.s.software)	/* bit 15 */
        x = x + 10;
    if ((work->word2.s.tcp_or_udp /* bit 18 */
	 && work->word2.s.vlan_valid) /* bit 63 */
	|| work->word2.s.dec_ipsec) /* bit 17 */
      {
        if (work->word2.s.software)
          x = x + 10;
        return x + 5;
      }
    else
        return x + 15;
}
