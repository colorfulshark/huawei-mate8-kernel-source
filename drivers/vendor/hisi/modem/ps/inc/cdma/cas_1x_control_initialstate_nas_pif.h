/** ****************************************************************************

                    Huawei Technologies Sweden AB (C), 2001-2015

 ********************************************************************************
 * @author    Automatically generated by DAISY
 * @version
 * @date      2015-04-16
 * @file
 * @brief
 *
 * @copyright Huawei Technologies Sweden AB
 *******************************************************************************/
#ifndef CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_H
#define CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_H

/*******************************************************************************
 1. Other files included
*******************************************************************************/

#include "vos.h"

#ifdef __cplusplus
#if __cplusplus
    extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#pragma pack(4)

/*******************************************************************************
 2. Macro definitions
*******************************************************************************/

#define CNAS_CAS_1X_MAX_REDIRECTION_CHAN_NUM                ( 16 )
#define CNAS_CAS_1X_MAX_SYS_RESELECTION_CHAN_NUM            ( 40 )
#define CNAS_CAS_1X_MAX_SAME_PRIO_SYS_NUM                   ( 8 )
#define CNAS_CAS_1X_MAX_FREQ_NUM                            ( 32 )
#define CNAS_CAS_1X_MAX_PREF_SYSTEM_NUM                     ( 16384 )
#define CNAS_CAS_1X_MAX_SYSTEM_NUM                          ( 16 )

/*******************************************************************************
 3. Enumerations declarations
*******************************************************************************/

/** ****************************************************************************
 * Name        : CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16
 *
 * Description :
 *******************************************************************************/
enum CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM
{
    ID_CNAS_CAS_1X_CAMP_SYNCED_CHANNEL_NTF                                      = 0x5000, /**< @sa CNAS_CAS_1X_CAMP_SYNCED_CHANNEL_NTF_STRU */
    ID_CNAS_CAS_1X_STOP_SYSTEM_ACQUIRE_REQ                                      = 0x5001, /**< @sa CNAS_CAS_1X_STOP_SYSTEM_ACQUIRE_REQ_STRU */
    ID_CAS_CNAS_1X_STOP_SYSTEM_ACQUIRE_CNF                                      = 0x5002, /**< @sa CAS_CNAS_1X_STOP_SYSTEM_ACQUIRE_CNF_STRU */
    ID_CNAS_CAS_1X_STOP_SYSTEM_SYNC_REQ                                         = 0x5003, /**< @sa CNAS_CAS_1X_STOP_SYSTEM_SYNC_REQ_STRU */
    ID_CAS_CNAS_1X_STOP_SYSTEM_SYNC_CNF                                         = 0x5004, /**< @sa CAS_CNAS_1X_STOP_SYSTEM_SYNC_CNF_STRU */
    ID_CNAS_CAS_1X_SYSTEM_SYNC_REQ                                              = 0x5005, /**< @sa CNAS_CAS_1X_SYSTEM_SYNC_REQ_STRU */
    ID_CAS_CNAS_1X_SYSTEM_SYNC_CNF                                              = 0x5006, /**< @sa CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU */
    ID_CNAS_CAS_1X_ALLOWED_SYSTEM_NOTIFY                                        = 0x5007, /**< @sa CNAS_CAS_1X_ALLOWED_SYSTEM_NOTIFY_STRU */
    ID_CNAS_CAS_1X_START_REQ                                                    = 0x5008, /**< @sa CNAS_CAS_1X_START_REQ_STRU */
    ID_CAS_CNAS_1X_START_CNF                                                    = 0x5009, /**< @sa CAS_CNAS_1X_START_CNF_STRU */
    ID_CNAS_CAS_1X_SYSTEM_ACQUIRE_REQ                                           = 0x500A, /**< @sa CNAS_CAS_1X_SYSTEM_ACQUIRE_REQ_STRU */
    ID_CAS_CNAS_1X_SYSTEM_ACQUIRE_CNF                                           = 0x500B, /**< @sa CAS_CNAS_1X_SYSTEM_ACQUIRE_CNF_STRU */
    ID_CAS_CNAS_1X_SYSTEM_DETERMIN_IND                                          = 0x500C, /**< @sa CAS_CNAS_1X_SYSTEM_DETERMIN_IND_STRU */
    ID_CNAS_CAS_1X_SYS_CFG_REQ                                                  = 0x500D, /**< @sa CNAS_CAS_1X_SYS_CFG_REQ_STRU */
    ID_CAS_CNAS_1X_SYS_CFG_CNF                                                  = 0x500E, /**< @sa CAS_CNAS_1X_SYS_CFG_CNF_STRU */
    ID_CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_BUTT                   = 0xFFFF
};
typedef VOS_UINT16 CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16;

/** ****************************************************************************
 * Name        : CNASITF_CARD_STATUS_ENUM_UINT16
 *
 * Description :
 *******************************************************************************/
enum CNASITF_CARD_STATUS_ENUM
{
    CNASITF_CARD_STATUS_ABSENT          = 0x0000,
    CNASITF_CARD_STATUS_UIM_PRESENT     = 0x0001,
    CNASITF_CARD_STATUS_CSIM_PRESENT    = 0x0002,
    CNASITF_CARD_STATUS_ENUM_BUTT       = 0x0003
};
typedef VOS_UINT16 CNASITF_CARD_STATUS_ENUM_UINT16;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_RSLT_ENUM_UINT8
 *
 * Description :
 *******************************************************************************/
enum CNAS_CAS_1X_RSLT_ENUM
{
    CNAS_CAS_1X_RSLT_SUCCESS            = 0x00,
    CNAS_CAS_1X_RSLT_FAILURE            = 0x01,
    CNAS_CAS_1X_RSLT_NO_RF              = 0x02,
    CNAS_CAS_1X_RSLT_BUTT               = 0x03
};
typedef VOS_UINT8 CNAS_CAS_1X_RSLT_ENUM_UINT8;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_SYS_SRCH_FAIL_REASON_ENUM_UINT16
 *
 * Description :
 *******************************************************************************/
enum CNAS_CAS_1X_SYS_SRCH_FAIL_REASON_ENUM
{
    CNAS_CAS_1X_SYS_SRCH_SYNC_FAIL_FREQ_SRCH_FAIL           = 0x0000,
    CNAS_CAS_1X_SYS_SRCH_SYNC_FAIL_PROTO_MISMATCH           = 0x0001, /**< protocol mismatch */
    CNAS_CAS_1X_SYS_SRCH_SYNC_FAIL_WRONG_SYSTEM             = 0x0002,
    CNAS_CAS_1X_SYS_SRCH_ACQ_FAIL_PAGE_CH_FAIL              = 0x0003, /**< paging channel create fail */
    CNAS_CAS_1X_SYS_SRCH_FAIL_REASON_BUTT                   = 0x0004
};
typedef VOS_UINT16 CNAS_CAS_1X_SYS_SRCH_FAIL_REASON_ENUM_UINT16;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_SD_REASON_ENUM_UINT8
 *
 * Description :
 *******************************************************************************/
enum CAS_CNAS_1X_SD_REASON_ENUM
{
    CAS_CNAS_1X_SD_RESCAN_IND                               = 0x00,
    CAS_CNAS_1X_SD_PROTO_MISMATCH_IND                       = 0x01,
    CAS_CNAS_1X_SD_SYS_LOST_IND                             = 0x02,
    CAS_CNAS_1X_SD_LOCK_IND                                 = 0x03,
    CAS_CNAS_1X_SD_UNLOCK_IND                               = 0x04,
    CAS_CNAS_1X_SD_ACCESS_DENIED_IND                        = 0x05,
    CAS_CNAS_1X_SD_ACCT_BLOCKED_IND                         = 0x06,
    CAS_CNAS_1X_SD_NDSS_OFF_IND                             = 0x07,
    CAS_CNAS_1X_SD_ERR_IND                                  = 0x08,
    CAS_CNAS_1X_SD_REDIR_IND                                = 0x09,
    CAS_CNAS_1X_SD_REGIST_REJ_IND                           = 0x0A,
    CAS_CNAS_1X_SD_RELEASE_IND                              = 0x0B,
    CAS_CNAS_1X_SD_RESELECTION_IND                          = 0x0C,
    CAS_CNAS_1X_SD_SYSTEM_RESELECTION_IND                   = 0x0D,
    CAS_CNAS_1X_SD_NEW_SYSTEM_IND                           = 0x0E,
    CAS_CNAS_1X_SD_MAX_ACCESS_FAILURE                       = 0x0F,
    CAS_CNAS_1X_SD_ABNORMAL_IND                             = 0x10,
    CAS_CNAS_1X_SD_ACQUISITION_FAIL                         = 0x11,
    CAS_CNAS_1X_SD_NO_RF                                    = 0x12,
    CAS_CNAS_1X_SD_REASON_BUTT                              = 0x13
};
typedef VOS_UINT8 CAS_CNAS_1X_SD_REASON_ENUM_UINT8;

/*******************************************************************************
 4. Message Header declaration
*******************************************************************************/

/*******************************************************************************
 5. Message declaration
*******************************************************************************/

/*******************************************************************************
 6. STRUCT and UNION declaration
*******************************************************************************/

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_NEW_SYSTEM_INFO_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT8                           enIsRelease;
    VOS_UINT8                           aucReserve[3];
} CNAS_CAS_1X_NEW_SYSTEM_INFO_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_SUPPORT_BAND_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT32                          bitOpBC0_A:1;
    VOS_UINT32                          bitOpBC0_B:1;
    VOS_UINT32                          bitOpBC1:1;
    VOS_UINT32                          bitOpBC2:1;
    VOS_UINT32                          bitOpBC3:1;
    VOS_UINT32                          bitOpBC4:1;
    VOS_UINT32                          bitOpBC5:1;
    VOS_UINT32                          bitOpBC6:1;
    VOS_UINT32                          bitOpBC7:1;
    VOS_UINT32                          bitOpBC8:1;
    VOS_UINT32                          bitOpBC9:1;
    VOS_UINT32                          bitOpBC10:1;
    VOS_UINT32                          bitOpBC11:1;
    VOS_UINT32                          bitOpBC12:1;
    VOS_UINT32                          bitOpBC14:1;
    VOS_UINT32                          bitOpBC15:1;
    VOS_UINT32                          bitOpSpare:16;
} CNAS_CAS_1X_SUPPORT_BAND_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_LOCK_REASON_CODE_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucReasonCode;
    VOS_UINT8                           aucReserve[3];
} CNAS_CAS_1X_LOCK_REASON_CODE_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_SYSTEM_IDENTITY_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT16                          usSid;
    VOS_UINT16                          usNid;
} CNAS_CAS_1X_SYSTEM_IDENTITY_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_FREQENCY_CHANNEL_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT16                          usBandClass;
    VOS_UINT16                          usChannel;
} CNAS_CAS_1X_FREQENCY_CHANNEL_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_PREF_SYSTEM_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT16                          usSid;
    VOS_UINT16                          usNid;
    VOS_UINT8                           ucPrefFlg;
    VOS_UINT8                           aucReserve[3];
} CNAS_CAS_1X_PREF_SYSTEM_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_REDIRECTION_INFO_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT8                           ucInvalidRedir;
    VOS_UINT8                           ucInvalidBandClass;
    VOS_UINT8                           aucReserve[2];
    VOS_UINT16                          usExpectedSid;
    VOS_UINT16                          usExpectedNid;
    VOS_UINT8                           ucReturnIfFail;
    VOS_UINT8                           ucChannelNum;
    VOS_UINT16                          usBandClass;
    VOS_UINT16                          ausChannelList[CNAS_CAS_1X_MAX_REDIRECTION_CHAN_NUM];
} CNAS_CAS_1X_REDIRECTION_INFO_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_SYS_RESELECTION_INFO_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT16                          usChannelNum;
    VOS_UINT8                           aucReserve[2];
    CNAS_CAS_1X_FREQENCY_CHANNEL_STRU   astChannelList[CNAS_CAS_1X_MAX_SYS_RESELECTION_CHAN_NUM];
} CNAS_CAS_1X_SYS_RESELECTION_INFO_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_ACQ_SYSTEM_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    CNAS_CAS_1X_FREQENCY_CHANNEL_STRU   stFreq;
    VOS_UINT16                          usAcqSysNum;
    VOS_UINT16                          usReserv;
    CNAS_CAS_1X_SYSTEM_IDENTITY_STRU    astSamePrioSys[CNAS_CAS_1X_MAX_SAME_PRIO_SYS_NUM];
} CNAS_CAS_1X_ACQ_SYSTEM_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_ACQ_RSLT_INFO_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    CNAS_CAS_1X_FREQENCY_CHANNEL_STRU                       stFreq;
    CNAS_CAS_1X_SYS_SRCH_FAIL_REASON_ENUM_UINT16            enFailReason;
    VOS_UINT16                                              usAcqSysNum;
    CNAS_CAS_1X_SYSTEM_IDENTITY_STRU                        astAcqedSys[CNAS_CAS_1X_MAX_SAME_PRIO_SYS_NUM];
    VOS_UINT32                                              ulSlice;
} CNAS_CAS_1X_ACQ_RSLT_INFO_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_SYNC_RSLT_INFO_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    CNAS_CAS_1X_FREQENCY_CHANNEL_STRU                       stFreq;
    CNAS_CAS_1X_SYS_SRCH_FAIL_REASON_ENUM_UINT16            enFailReason;
    VOS_UINT8                                               aucReserve[2];
    VOS_UINT32                                              ulSlice;
} CNAS_CAS_1X_SYNC_RSLT_INFO_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_SYSTEM_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_UINT16                          usSid;
    VOS_UINT16                          usNid;
    CNAS_CAS_1X_FREQENCY_CHANNEL_STRU   stFreq;
} CNAS_CAS_1X_SYSTEM_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_SYS_DETERMIN_INFO_UNION
 *
 * Description :
 *******************************************************************************/
typedef union
{
    CNAS_CAS_1X_REDIRECTION_INFO_STRU                       stRedirectionInfo;
    CNAS_CAS_1X_SYS_RESELECTION_INFO_STRU                   stSysReselInfo;
    CNAS_CAS_1X_LOCK_REASON_CODE_STRU                       stLockReasonCode;
    CNAS_CAS_1X_NEW_SYSTEM_INFO_STRU                        stNewSysInfo;
} CNAS_CAS_1X_SYS_DETERMIN_INFO_UNION;

/*******************************************************************************
 7. OTHER declarations
*******************************************************************************/

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_CAMP_SYNCED_CHANNEL_NTF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    VOS_UINT8                                                                   aucReserve[4];
} CNAS_CAS_1X_CAMP_SYNCED_CHANNEL_NTF_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_STOP_SYSTEM_ACQUIRE_REQ_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    VOS_UINT8                                                                   aucReserve[4];
} CNAS_CAS_1X_STOP_SYSTEM_ACQUIRE_REQ_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_STOP_SYSTEM_ACQUIRE_CNF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    VOS_UINT8                                                                   aucReserve[4];
} CAS_CNAS_1X_STOP_SYSTEM_ACQUIRE_CNF_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_STOP_SYSTEM_SYNC_REQ_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    VOS_UINT8                                                                   aucReserve[4];
} CNAS_CAS_1X_STOP_SYSTEM_SYNC_REQ_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_STOP_SYSTEM_SYNC_CNF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    VOS_UINT8                                                                   aucReserve[4];
} CAS_CNAS_1X_STOP_SYSTEM_SYNC_CNF_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_SYSTEM_SYNC_REQ_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    VOS_UINT32                                                                  ulFreqNum;                             /**< Number of frequencies included in list */
    CNAS_CAS_1X_FREQENCY_CHANNEL_STRU                                           astFreqList[CNAS_CAS_1X_MAX_FREQ_NUM]; /**< List of frequencies to search, in priority order */
} CNAS_CAS_1X_SYSTEM_SYNC_REQ_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    CNAS_CAS_1X_RSLT_ENUM_UINT8                                                 enSyncRslt;
    VOS_UINT8                                                                   aucReserve[3];
    CNAS_CAS_1X_FREQENCY_CHANNEL_STRU                                           stFreq;
    CNAS_CAS_1X_SYSTEM_IDENTITY_STRU                                            stSyncedSysId;
    VOS_UINT32                                                                  ulSyncFailFreqNum;                         /**< Number of frequencies included in list */
    CNAS_CAS_1X_SYNC_RSLT_INFO_STRU                                             astSyncRsltList[CNAS_CAS_1X_MAX_FREQ_NUM]; /**< List of frequencies to search, in priority order */
} CAS_CNAS_1X_SYSTEM_SYNC_CNF_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_ALLOWED_SYSTEM_NOTIFY_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    VOS_UINT32                                                                  ulPrefOnly;                                     /**< VOS_TRUE: only preferred system is allowed; VOS_FALSE: only negative system is not allowed */
    VOS_UINT32                                                                  ulSystemNum;
    CNAS_CAS_1X_PREF_SYSTEM_STRU                                                astSystemList[CNAS_CAS_1X_MAX_PREF_SYSTEM_NUM]; /**< List of frequencies to search, in priority order */
} CNAS_CAS_1X_ALLOWED_SYSTEM_NOTIFY_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_START_REQ_STRU
 *
 * Description : NAS use this signal indicate L3AS power on.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    MODEM_ID_ENUM_UINT16                                                        enModemId;
    CNASITF_CARD_STATUS_ENUM_UINT16                                             enUimStatus;
    VOS_UINT8                                                                   aucReserve[3];
    VOS_UINT8                                                                   ucSuppRatNum;
    VOS_RATMODE_ENUM_UINT32                                                     aenRatMode[VOS_RATMODE_BUTT];
} CNAS_CAS_1X_START_REQ_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_START_CNF_STRU
 *
 * Description : NAS use this signal indicate L3AS power on.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    CNAS_CAS_1X_RSLT_ENUM_UINT8                                                 enRslt;
    VOS_UINT8                                                                   aucRsved[3];
} CAS_CNAS_1X_START_CNF_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_SYSTEM_ACQUIRE_REQ_STRU
 *
 * Description : Start pilot selection process in 1x_Layer_3_AS.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    VOS_UINT32                                                                  ulAllowedSystemFlg;                           /**< Only search allowed system flag */
    VOS_UINT32                                                                  ulFreqNum;                                    /**< Number of frequencies included in list */
    CNAS_CAS_1X_ACQ_SYSTEM_STRU                                                 astAcqSystemList[CNAS_CAS_1X_MAX_SYSTEM_NUM]; /**< List of frequencies to search, in priority order */
} CNAS_CAS_1X_SYSTEM_ACQUIRE_REQ_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_SYSTEM_ACQUIRE_CNF_STRU
 *
 * Description : Select cell procedure request with
 * CNAS_CAS_1X_SYSTEM_ACQUIRE_REQ completed successfully. It is the
 * responsibility of Layer_3_NAS to update the registration timers when this
 * signal is received.
 * If the procedure fails a CAS_CNAS_1X_SYSTEM_DETERMIN_IND with an
 * Acquisition Failure Indication is returned to Layer_3_NAS.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    CNAS_CAS_1X_RSLT_ENUM_UINT8                                                 enAcqRslt;
    VOS_UINT8                                                                   aucReserve[3];
    CNAS_CAS_1X_SYSTEM_STRU                                                     stAcqSystem;
    VOS_UINT32                                                                  ulAcqFailSysNum;
    CNAS_CAS_1X_ACQ_RSLT_INFO_STRU                                              astAcqRsltList[CNAS_CAS_1X_MAX_SYSTEM_NUM];
} CAS_CNAS_1X_SYSTEM_ACQUIRE_CNF_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_SYSTEM_DETERMIN_IND_STRU
 *
 * Description : This signal is used to send notifications to Layer_3_NAS that
 * the mobile station need goto system determination state.
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    CAS_CNAS_1X_SD_REASON_ENUM_UINT8                                            enSdReason;
    VOS_UINT8                                                                   aucReserve[3];
    CNAS_CAS_1X_FREQENCY_CHANNEL_STRU                                           stCurChannelInfo;
    CNAS_CAS_1X_SYS_DETERMIN_INFO_UNION                                         unSysDeterminInfo;
} CAS_CNAS_1X_SYSTEM_DETERMIN_IND_STRU;

/** ****************************************************************************
 * Name        : CNAS_CAS_1X_SYS_CFG_REQ_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    VOS_UINT8                                                                   aucReserve[3];
    VOS_UINT8                                                                   ucSuppRatNum;
    VOS_RATMODE_ENUM_UINT32                                                     aenRatMode[VOS_RATMODE_BUTT];
    CNAS_CAS_1X_SUPPORT_BAND_STRU                                               stSupportBand;
} CNAS_CAS_1X_SYS_CFG_REQ_STRU;

/** ****************************************************************************
 * Name        : CAS_CNAS_1X_SYS_CFG_CNF_STRU
 *
 * Description :
 *******************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    CAS_1X_CONTROL_INITIALSTATE_NAS_PIF_MSG_TYPE_ENUM_UINT16                    enMsgId;
    VOS_UINT16                                                                  usOpId;
    VOS_UINT8                                                                   aucReserve[4];
} CAS_CNAS_1X_SYS_CFG_CNF_STRU;

/*******************************************************************************
 8. Global  declaration
*******************************************************************************/

/*******************************************************************************
 9. Function declarations
*******************************************************************************/

#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif