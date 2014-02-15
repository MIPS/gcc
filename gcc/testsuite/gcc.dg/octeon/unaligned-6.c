/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */

#include <stdlib.h>
#include <string.h>

typedef struct Ipv6RuleBase_t
{
    char reduFlag;
    char ucRuleAction;
    unsigned short index;
    unsigned short wMinProtocol;
    unsigned short wMaxProtocol;
    unsigned short wMinSrcIp[8];
    unsigned short wMaxSrcIp[8];
    unsigned short wMinDstIp[8];
    unsigned short wMaxDstIp[8];
    unsigned short wSrcPortMin;
    unsigned short wSrcPortMax;
    unsigned short wDstPortMin;
    unsigned short wDstPortMax;
}__attribute__((packed)) IPv6RuleRecd;
typedef struct in6_addr1 {
 union {
  char __u6_addr8[16];
  unsigned int __u6_addr16[8];
  unsigned int __u6_addr32[4];
 } __u6_addr;
}in6_addr;
typedef struct
{
    char ucFlag;
    char ucRuleAction;
    unsigned short index;
    in6_addr sIpAdd;
    in6_addr dIpAdd;
    unsigned short sIpMaskLen;
    unsigned short dIpMaskLen;
    unsigned short wMinProtocol;
    unsigned short wMaxProtocol;
    unsigned short wSrcPortMin;
    unsigned short wSrcPortMax;
    unsigned short wDstPortMin;
    unsigned short wDstPortMax;
    char ucQosOrDscp;
    char ucDscpQosValue;
    char pad[2];
}RuleRecd_v6;
typedef struct epu_rcv_acl_v6 {
    unsigned short acl_no;
    unsigned short rule_count;
    unsigned short acl_flag;
    unsigned short ruleCurrentNum;
    char seq;
    char pad;
    unsigned short no_ipsec_count;
    RuleRecd_v6 rule[1000];
}epu_rcv_acl_v6;
void test6(int localPort, epu_rcv_acl_v6 *epu_acl,char direct)
{
    IPv6RuleRecd *RulePack;
    IPv6RuleRecd *pGetRulePack;
    int i= 0;
    pGetRulePack = (IPv6RuleRecd *)malloc(sizeof(IPv6RuleRecd)* epu_acl->rule_count);
    if(((void *)0) == pGetRulePack)
    {
        return;
    }
    memset(pGetRulePack, 0, sizeof(IPv6RuleRecd)* epu_acl->rule_count);
    RulePack = pGetRulePack;
    for (i = 0; i < epu_acl->rule_count; i++)
    {
        RulePack->reduFlag = 0;
        RulePack->ucRuleAction = epu_acl->rule[i].ucRuleAction;
        RulePack->index = epu_acl->rule[i].index;
        RulePack->wMinProtocol = epu_acl->rule[i].wMinProtocol;
        RulePack->wMaxProtocol= epu_acl->rule[i].wMaxProtocol;
        RulePack +=1;
    }
    return;
}
