#ifndef      _DHCPV4S_VAR_H_
#define      _DHCPV4S_VAR_H_


extern UCHAR *g_pucDHCPSRecvMsgBuff;

extern DHCPS_EXPIRED_REC_S *g_pstExpiredInfor;
extern ULONG g_ulDhcpServerTaskId ;

extern ULONG g_ulDhcpServerSwitch ;
extern ULONG g_ulDHCPSIpnum;
extern MC_QUE_ID g_pDHCPSRcvDhcpMsgQueId;
extern ULONG *g_ulDhcpStatisc ;

extern ULONG  g_ulDhcpsTimerQueId;
extern ULONG  g_ulLeaseCircleTimerID;
extern ULONG  g_ulStateCircleTimerID;
extern ULONG  g_ulDhcpsInnerQueId;

/* BEGIN: Modified by jixiaoming at 2011-05-04 */
extern  DHCPS_TIMER_LINKLIST_S** g_ppstStateTimerList;
extern  DHCPS_TIMER_LINKLIST_S** g_ppstLeaseTimerList;
/* END: Modified by jixiaoming at 2011-05-04 */
extern DHCPS_TIMER_LINKLIST_S*  g_pstLeaseTimer;
extern DHCPS_TIMER_LINKLIST_S*  g_pstStateTimer;


extern ULONG g_ulLeaseTimerCount;         /* 基准定时计数 */
extern ULONG g_ulStateTimerCount;         /* 基准定时计数 */

extern ULONG  g_ulCurrentStateNode;              /*  当前处理的state 节点 */
extern ULONG  g_ulCurrentLeaseNode;              /*  当前处理的lease 节点 */


extern DHCPS_LEASE_S *g_pstDhcpSLease;   /* g_pstDhcpSLease[0]不使用 */
extern DHCPS_CFG_OPTION_S *g_pstOption; 
extern const UCHAR g_ucDhcpOptionParseType[];
extern ULONG g_ulServerIpAddr;

/* Added start by dongmingzhou 42828 at 2008-12-05 UGW9811V900R001C01 for ADDRESS SUBSYSTEM */
extern DHCPS_ANALYZEDPACKET_S g_stAnalyzPacket;
extern UCHAR g_aucMsg2Event[];
extern UCHAR g_aucMsgUSM2Event[];
/* Added end by dongmingzhou 42828 at 2008-12-05 UGW9811V900R001C01 for ADDRESS SUBSYSTEM */

extern DHCPS_MSG_BUFFLIST_HEAD *g_pstDHCPMsgBuffList;

/* Added start by dongmingzhou 42828 at 2008-11-27 UGW9811V900R001C01 for ADDRESS SUBSYSTEM */
extern DHCPS_STATE_MACHINE_NODE  g_stDHCPSStateMachine[DHCPS_STATE_MAX + 1];

extern DHCPS_STATE_MESSAGE_NODE  g_stDHCPSMsgNode[DHCPS_MSG_MAX];

/*DHCPS和USM间的消息队列ID*/
extern ULONG g_ulDHCPSUSMQueId; 

/*调试数据*/
extern ULONG g_aulDHCPSDbgCnt[DHCPS_MAX_FAILURE_COUNTER];
extern UCHAR g_DHCPStateMachineDescrip[DHCPS_STATE_MAX][30];
extern UCHAR g_DHCPMsgNodeDescrip[DHCPS_MSG_MAX][50];
extern UCHAR g_DHCPStatiscDescrip[DHCPS_MAX_STATISC][50];
/* Added end by dongmingzhou 42828 at 2008-11-27 UGW9811V900R001C01 for ADDRESS SUBSYSTEM */

extern STUB_SDB_CONTEXT_S g_stStubSdbContext;

extern ULONG g_ulUsmStub;
extern ULONG ulSendRequestMsg;
extern ULONG g_ulDhcpsDebug;
extern ULONG g_ulUdpsStub;

/* Added start by dongmingzhou 42828 at 2009-02-20 UGWV9R1C1 for 地址分配 */
extern ULONG g_ulUsmInternalMsgQue[]; /*内部消息的队列ID*/
extern ULONG g_ulAmUsmInternalMsgEve[];/*内部消息的事件位*/
extern ULONG g_ulAmUsmTaskId;
/* Added end by dongmingzhou 42828 at 2009-02-20 UGWV9R1C1 for 地址分配 */

/* BEGIN: Modified by jixiaoming at 2011-03-02 */
extern VOS_SIZE_T  g_UEtoPGW_ulDHCPV4DiscoveryNumAddr ; /*用于统计PGW接收的UE始发的DHCPV4 Discovery消息包数*/
extern VOS_SIZE_T  g_UEtoPGW_ulDHCPV4RequestNumAddr ;   /*用于统计PGW接收的UE始发的DHCPV4 Request消息包数*/
extern VOS_SIZE_T  g_UEtoPGW_ulDHCPV4DeclineNumAddr ;   /*用于统计PGW接收的UE始发的DHCPV4 Decline消息包数*/
extern VOS_SIZE_T  g_UEtoPGW_ulDHCPV4ReleaseNumAddr ;   /*用于统计PGW接收的UE始发的DHCPV4 Releases消息包数*/
extern VOS_SIZE_T  g_UEtoPGW_ulDHCPV4InformNumAddr ;    /*用于统计PGW接收的UE始发的DHCPV4 Inform消息包数*/
extern VOS_SIZE_T  g_PGWtoUE_ulDHCPV4OfferNumAddr ;     /*用于统计PGW发送给UE的DHCPV4 Offer消息包数*/
extern VOS_SIZE_T  g_PGWtoUE_ulDHCPV4AckNumAddr ;       /*用于统计PGW发送给UE的DHCPV4 Ack消息包数*/
extern VOS_SIZE_T  g_PGWtoUE_ulDHCPV4NakNumAddr ;       /*用于统计PGW发送给UE的DHCPV4 Nak消息包数*/


extern VOS_SIZE_T  g_UEtoSGW_ulDHCPV4DiscoveryNumAddr ;       /*用于统计SGW接收的UE始发的DHCPV4 Discovery消息包数*/
extern VOS_SIZE_T  g_UEtoSGW_ulDHCPV4RequestNumAddr ;       /*用于统计SGW接收的UE始发的DHCPV4 Request消息包数*/
extern VOS_SIZE_T  g_UEtoSGW_ulDHCPV4DeclineNumAddr ;       /*用于统计SGW接收的UE始发的DHCPV4 Decline消息包数*/
extern VOS_SIZE_T  g_UEtoSGW_ulDHCPV4ReleaseNumAddr ;       /*用于统计SGW接收的UE始发的DHCPV4 Releases消息包数*/
extern VOS_SIZE_T  g_UEtoSGW_ulDHCPV4InformNumAddr ;       /*用于统计SGW接收的UE始发的DHCPV4 Inform消息包数*/
extern VOS_SIZE_T  g_SGWtoUE_ulDHCPV4OfferNumAddr ;       /*用于统计SGW发送给UE的DHCPV4 Offer消息包数*/
extern VOS_SIZE_T  g_SGWtoUE_ulDHCPV4AckNumAddr ;       /*用于统计SGW发送给UE的DHCPV4 Ack消息包数*/
extern VOS_SIZE_T  g_SGWtoUE_ulDHCPV4NakNumAddr ;       /*用于统计SGW发送给UE的DHCPV4 Nak消息包数*/
/* END: Modified by jixiaoming at 2011-03-02 */

extern ULONG g_ulGreProType;

/* BEGIN: Added by zhangjinquan at 2011-10-06 for log optimize */
extern ULONG g_ulDhcpv4sLogRecordRegId;
/* END: Added by zhangjinquan at 2011-10-06 for log optimize */
extern ULONG ulDhcpv4sTaskStep;

#endif          /* end of _DHCPV4S_VAR_H_ */
