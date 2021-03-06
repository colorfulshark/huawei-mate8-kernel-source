

#ifndef __CASMNTNOMINTERFACE_H__
#define __CASMNTNOMINTERFACE_H__


/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
#include "CUeSpec.h"
#include "PsTypeDef.h"
#include "TTFComm.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

/*****************************************************************************
  2 宏定义
*****************************************************************************/


/*****************************************************************************
  3 枚举定义
*****************************************************************************/

/*****************************************************************************
 枚举名    : CAS_MNTN_RPT_TYPE_ENUM_UINT8
 协议表格  :
 ASN.1描述 :
 枚举说明  :报告小区信息的类型
*****************************************************************************/
enum CAS_MNTN_RPT_TYPE_ENUM
{
    CAS_MNTN_RPT_ONLYONE              = 0,
    CAS_MNTN_RPT_PERIOD                  ,
    CAS_MNTN_RPT_CHNGE_RPT               ,
    CAS_MNTN_RPT_TYPE_BUTT
};
typedef VOS_UINT8 CAS_MNTN_RPT_TYPE_ENUM_UINT8;


/*****************************************************************************
 枚举名    : CAS_MNTN_RPT_CMD_ENUM_UINT8
 协议表格  :
 ASN.1描述 :
 枚举说明  :开始查询或者停止查询命令
*****************************************************************************/
enum CAS_MNTN_RPT_CMD_ENUM
{
    CAS_MNTN_RPT_STOP                 = 0,
    CAS_MNTN_RPT_START                = 1,
    CAS_MNTN_RPT_BUTT
};
typedef VOS_UINT8 CAS_MNTN_RPT_CMD_ENUM_UINT8;


/*****************************************************************************
  4 全局变量声明
*****************************************************************************/


/*****************************************************************************
  5 消息头定义
*****************************************************************************/


/*****************************************************************************
  6 消息定义
*****************************************************************************/


/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/

/*****************************************************************************
 结构名    : WAS_OM_ACT_OR_SERV_CELL_INFO_CNF_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 透明消息-主小区或活动集测量Cnf消息
*****************************************************************************/
typedef struct
{
    VOS_UINT8                               aucReserved[3];

    CAS_MNTN_RPT_TYPE_ENUM_UINT8            enReportType;                       /*一次上报，周期上报或者变化上报*/

    PS_RSLT_CODE_ENUM_UINT32                enRslt;                             /* 返回查询结构成功或失败 */

}CAS_1X_OM_CELL_INFO_STRU;

/*****************************************************************************
 结构名    : OM_CAS_1X_CELL_INFO_REQ_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : CELL_INFO CONFIG   OM融合整改消息结构
*****************************************************************************/

typedef struct
{
    CAS_MNTN_RPT_CMD_ENUM_UINT8             enCommand;                          /* start or stop */

    CAS_MNTN_RPT_TYPE_ENUM_UINT8            enReportType;                       /* 一次上报，周期上报或者变化上报*/

    VOS_UINT16                              usReportPeriod;                      /* report time period */
} OM_CAS_1X_CELL_INFO_STRU;


typedef struct
{
    VOS_MSG_HEADER

    VOS_UINT32                      ulMsgId;

    VOS_UINT16                      usOriginalId;
    VOS_UINT16                      usTerminalId;
    VOS_UINT32                      ulTimeStamp;
    VOS_UINT32                      ulSN;

    OM_CAS_1X_CELL_INFO_STRU        stCellInfoStru;

}OM_CAS_1X_CELL_INFO_REQ_STRU;

/*****************************************************************************
 结构名    :  CAS_1X_OM_CELL_INFO_CNF_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 透明消息-主小区或活动集测量Cnf消息 --- OM融合整改消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER

    VOS_UINT32                      ulMsgId;

    VOS_UINT16                      usOriginalId;
    VOS_UINT16                      usTerminalId;
    VOS_UINT32                      ulTimeStamp;
    VOS_UINT32                      ulSN;

    CAS_1X_OM_CELL_INFO_STRU        stCellInfoStru;

}CAS_1X_OM_CELL_INFO_CNF_STRU;


/*****************************************************************************
 结构名    : OM_CAS_HRPD_CELL_INFO_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : CELL_INFO CONFIG
*****************************************************************************/
typedef struct
{
    CAS_MNTN_RPT_CMD_ENUM_UINT8             enCommand;                          /* start or stop */

    CAS_MNTN_RPT_TYPE_ENUM_UINT8            enReportType;                       /* 一次上报，周期上报或者变化上报*/

    VOS_UINT16                              usReportPeriod;                     /* report time period */
} OM_CAS_HRPD_CELL_INFO_STRU;

/*****************************************************************************
 结构名    : CAS_HRPD_OM_CELL_INFO_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 透明消息-主小区或活动集测量Cnf消息
*****************************************************************************/
typedef struct
{
    VOS_UINT8                               aucReserved[3];

    CAS_MNTN_RPT_TYPE_ENUM_UINT8            enReportType;                       /*一次上报，周期上报或者变化上报*/

    PS_RSLT_CODE_ENUM_UINT32                enRslt;                             /* 返回查询结构成功或失败 */
}CAS_HRPD_OM_CELL_INFO_STRU;

/*****************************************************************************
 结构名    : OM_CAS_HRPD_CELL_INFO_REQ_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : CELL_INFO CONFIG   OM融合整改消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER

    VOS_UINT32                      ulMsgId;

    VOS_UINT16                      usOriginalId;
    VOS_UINT16                      usTerminalId;
    VOS_UINT32                      ulTimeStamp;
    VOS_UINT32                      ulSN;

    OM_CAS_HRPD_CELL_INFO_STRU      stCellInfoStru;

}OM_CAS_HRPD_CELL_INFO_REQ_STRU;

/*****************************************************************************
 结构名    :  CAS_HRPD_OM_CELL_INFO_CNF_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 透明消息-主小区或活动集测量Cnf消息 --- OM融合整改消息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER

    VOS_UINT32                      ulMsgId;

    VOS_UINT16                      usOriginalId;
    VOS_UINT16                      usTerminalId;
    VOS_UINT32                      ulTimeStamp;
    VOS_UINT32                      ulSN;

    CAS_HRPD_OM_CELL_INFO_STRU      stCellInfoStru;

}CAS_HRPD_OM_CELL_INFO_CNF_STRU;


/*****************************************************************************
  8 UNION定义
*****************************************************************************/


/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/


/*****************************************************************************
  10 函数声明
*****************************************************************************/








#if (VOS_OS_VER == VOS_WIN32)
#pragma pack()
#else
#pragma pack(0)
#endif




#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of CasMntnOmInterface.h */

