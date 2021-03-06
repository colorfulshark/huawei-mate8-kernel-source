

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "PsCommonDef.h"
#include "NVIM_Interface.h"
#include "TafApsProcNvim.h"
#include "TafApsDsFlowStats.h"
#include "TafLog.h"
#include "TafApsCtx.h"
#include "TafSdcLib.h"
#include "TafApsMntn.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*lint -e958*/

#define THIS_FILE_ID PS_FILE_ID_TAF_APS_PROCNVIM_C

/*****************************************************************************
  2 全局变量定义
*****************************************************************************/


/*****************************************************************************
  3 函数实现
*****************************************************************************/


VOS_VOID  TAF_APS_ReadDsFlowInfoFromNv(
    TAF_APS_DSFLOW_STATS_CTX_STRU      *pstApsDsFlowCtx
)
{
    TAF_APS_DSFLOW_NV_STRU              stApsDsflowNv;
    TAF_APS_DSFLOW_STATS_CTRL_NV_STRU   stApsDsflowCtrlNv;

    /* 初始化 */
    PS_MEM_SET(&stApsDsflowNv, 0, sizeof(TAF_APS_DSFLOW_NV_STRU));
    PS_MEM_SET(&stApsDsflowCtrlNv, 0, sizeof(TAF_APS_DSFLOW_STATS_CTRL_NV_STRU));

    /* 从NV中读取流量统计NV项 */
    if (NV_OK != NV_Read (en_NV_Item_DSFLOW_STATS_CTRL,
                          &stApsDsflowCtrlNv, sizeof(TAF_APS_DSFLOW_STATS_CTRL_NV_STRU)))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_ReadDsFlowCfgFromNv(): NV_Read en_NV_Item_DSFLOW_STATS_CTRL Error");

        /* 流量上报和保存NV控制均置为FALSE */
        pstApsDsFlowCtx->ucDsFlowSave2NvFlg     = VOS_FALSE;
        pstApsDsFlowCtx->ucDsFlowSavePeriod     = TI_TAF_APS_DEFAULT_DSFLOW_WRITE_NV_LEN;
        pstApsDsFlowCtx->ucDsFlowATRptPeriod    = TAF_APS_DSFLOW_AT_REPORT_PERIOD;

        /* 更新到SDC全局变量中 */
        TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_DSFLOWRPT, VOS_FALSE);
        return;
    }

    /* 刷新流量统计控制上下文 */
    pstApsDsFlowCtx->ucDsFlowSave2NvFlg     = stApsDsflowCtrlNv.ucDsFlowStatsSave2NvCtrl;
    pstApsDsFlowCtx->ucDsFlowSavePeriod     = stApsDsflowCtrlNv.ucDsFlowSavePeriod;
    pstApsDsFlowCtx->ucDsFlowATRptPeriod    = TAF_APS_DSFLOW_AT_REPORT_PERIOD;

    /* 更新到SDC全局变量中 */
    TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_DSFLOWRPT, (VOS_UINT32)stApsDsflowCtrlNv.ucDsFlowStatsRptCtrl);


    /* 保存历史流量特性使能 */
    if ( VOS_TRUE == pstApsDsFlowCtx->ucDsFlowSave2NvFlg )
    {
        if (NV_OK != NV_Read (en_NV_Item_DSFLOW_REPORT,
                              &stApsDsflowNv, sizeof(TAF_APS_DSFLOW_NV_STRU)))
        {
            TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_ReadDsFlowCfgFromNv(): NV_Read en_NV_Item_DSFLOW_REPORT Error");
            return;
        }


        /* 历史流量信息,默认为1 */
        pstApsDsFlowCtx->astApsDsFlowStats[0].stCurrentFlowInfo.ulDSLinkTime
                                        = stApsDsflowNv.ulDSLastLinkTime;

        pstApsDsFlowCtx->astApsDsFlowStats[0].stTotalFlowInfo.ulDSLinkTime
                                        = stApsDsflowNv.ulDSTotalLinkTime;

        pstApsDsFlowCtx->astApsDsFlowStats[0].stTotalFlowInfo.ulDSReceiveFluxHigh
                                        = stApsDsflowNv.ulDSTotalReceiveFluxHigh;

        pstApsDsFlowCtx->astApsDsFlowStats[0].stTotalFlowInfo.ulDSReceiveFluxLow
                                        = stApsDsflowNv.ulDSTotalReceiveFluxLow;

        pstApsDsFlowCtx->astApsDsFlowStats[0].stTotalFlowInfo.ulDSSendFluxHigh
                                        = stApsDsflowNv.ulDSTotalSendFluxHigh;

        pstApsDsFlowCtx->astApsDsFlowStats[0].stTotalFlowInfo.ulDSSendFluxLow
                                        = stApsDsflowNv.ulDSTotalSendFluxLow;

    }

    return;
}



VOS_VOID  TAF_APS_ClearDsFlowInfoInNv( VOS_VOID )
{
    TAF_APS_DSFLOW_NV_STRU              stApsDsflowNv;

    /* 初始化 */
    PS_MEM_SET(&stApsDsflowNv, 0, sizeof(TAF_APS_DSFLOW_NV_STRU));

    /* 刷新NV项 */
    if (NV_OK != NV_Write(en_NV_Item_DSFLOW_REPORT, &stApsDsflowNv, sizeof(TAF_APS_DSFLOW_NV_STRU)))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_ClearDsFlowInfoInNv:Write NVIM Failed");
    }

    return;
}


VOS_VOID  TAF_APS_SaveDsFlowInfoToNv( VOS_VOID )
{
    TAF_APS_DSFLOW_STATS_CTX_STRU      *pstApsDsFlowCtx;
    TAF_APS_DSFLOW_NV_STRU              stApsDsflowNv;
    TAF_DSFLOW_QUERY_INFO_STRU          stDsFlowQryInfo;

    /* 初始化 */
    pstApsDsFlowCtx = TAF_APS_GetDsFlowCtxAddr();
    PS_MEM_SET(&stApsDsflowNv, 0, sizeof(TAF_APS_DSFLOW_NV_STRU));
    PS_MEM_SET(&stDsFlowQryInfo, 0, sizeof(TAF_DSFLOW_QUERY_INFO_STRU));

    if (VOS_TRUE == pstApsDsFlowCtx->ucDsFlowSave2NvFlg)
    {

        /* 获取历史流量信息项 */
        TAF_APS_QryAllRabDsFlowStats(&stDsFlowQryInfo);

        /* 刷新流量信息相关NV项 */
        stApsDsflowNv.ulDSLastLinkTime          = stDsFlowQryInfo.stCurrentFlowInfo.ulDSLinkTime;
        stApsDsflowNv.ulDSTotalLinkTime         = stDsFlowQryInfo.stTotalFlowInfo.ulDSLinkTime;
        stApsDsflowNv.ulDSTotalReceiveFluxHigh  = stDsFlowQryInfo.stTotalFlowInfo.ulDSReceiveFluxHigh;
        stApsDsflowNv.ulDSTotalReceiveFluxLow   = stDsFlowQryInfo.stTotalFlowInfo.ulDSReceiveFluxLow;
        stApsDsflowNv.ulDSTotalSendFluxHigh     = stDsFlowQryInfo.stTotalFlowInfo.ulDSSendFluxHigh;
        stApsDsflowNv.ulDSTotalSendFluxLow      = stDsFlowQryInfo.stTotalFlowInfo.ulDSSendFluxLow;

        /* 写NV项 */
        if (NV_OK != NV_Write(en_NV_Item_DSFLOW_REPORT, &stApsDsflowNv, sizeof(TAF_APS_DSFLOW_NV_STRU)))
        {
            TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_SaveDsFlowInfoToNv: Write NVIM Failed");
        }
    }
    else
    {
        TAF_NORMAL_LOG(WUEPS_PID_TAF, "TAF_APS_SaveDsFlowInfoToNv: Save2NvFlg is not set.");
    }

    return;
}


VOS_VOID TAF_APS_ReadPdpActLimitFlgFromNv(VOS_VOID)
{
    TAF_APS_NVIM_PDP_ACT_LIMIT_STRU     stNvPdpActLimit;
    TAF_APS_PDP_ACT_LIMIT_INFO_STRU    *pstPdpActLimitInfo = VOS_NULL_PTR;
    VOS_UINT32                          ulLength;

    ulLength = 0;

    PS_MEM_SET(&stNvPdpActLimit, 0x00, sizeof(TAF_APS_NVIM_PDP_ACT_LIMIT_STRU));

    (VOS_VOID)NV_GetLength(en_NV_Item_Pdp_Act_Limit_Para, &ulLength);
    if (ulLength > sizeof(stNvPdpActLimit))
    {
        return;
    }

    /* 从en_NV_Item_Pdp_Act_Limit_Para中读取PDP激活受限标志 */
    if (NV_OK !=  NV_Read(en_NV_Item_Pdp_Act_Limit_Para,
                          &stNvPdpActLimit,
                          ulLength))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_ReadPdpActLimitFlgFromNv:NVIM Read Failed");

        return;
    }

    /* PDP激活受限标志有效性检查 */
    if ((stNvPdpActLimit.ucNvimActiveFlg != VOS_TRUE)
     && (stNvPdpActLimit.ucNvimActiveFlg != VOS_FALSE))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_ReadPdpActLimitFlgFromNv:NVIM Is Anavil");
        return;
    }

    pstPdpActLimitInfo = TAF_APS_GetPdpActLimitInfoAddr();

    pstPdpActLimitInfo->ucPdpActLimitFlg = stNvPdpActLimit.ucNvimActiveFlg;

    return;
}


VOS_VOID TAF_APS_ReadDsflowRateConfigFromNv(TAF_APS_SWITCH_DDR_RATE_INFO_STRU *pstSwitchDdrInfo)
{
    TAF_NV_DFS_DSFLOW_RATE_CONFIG_STRU          stRateConfig;
    TAF_NV_MULTI_DFS_DSFLOW_RATE_CONFIG_STRU    stMultiDfsConfig;
    VOS_UINT32                                  ulIndex;

    PS_MEM_SET(&stRateConfig, 0, sizeof(TAF_NV_DFS_DSFLOW_RATE_CONFIG_STRU));
    PS_MEM_SET(&stMultiDfsConfig, 0, sizeof(TAF_NV_MULTI_DFS_DSFLOW_RATE_CONFIG_STRU));

    /* 从NV中读取流速配置NV项 */
    if (NV_OK != NV_Read (en_NV_Item_Dfs_Dsflow_Rate_Config,
                          &stRateConfig, sizeof(TAF_NV_DFS_DSFLOW_RATE_CONFIG_STRU)))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_ReadDsflowRateConfigFromNv(): NV_Read en_NV_Item_Dsflow_Rate_Config Error");
        return;
    }

    if (NV_OK != NV_Read (en_NV_Item_Multi_Dfs_Dsflow_Rate_CFG,
                          &stMultiDfsConfig, sizeof(TAF_NV_MULTI_DFS_DSFLOW_RATE_CONFIG_STRU)))
    {
        TAF_ERROR_LOG(WUEPS_PID_TAF, "TAF_APS_ReadDsflowRateConfigFromNv(): NV_Read en_NV_Item_Multi_Dfs_Dsflow_Rate_CFG Error");
        return;
    }

    if (0 == stMultiDfsConfig.astDfsConfig[0].ulProfileNum)
    {
        if (0 == stRateConfig.ulProfileNum)
        {
            PS_MEM_SET((VOS_VOID *)pstSwitchDdrInfo->astDfsConfig, 0, sizeof(TAF_NV_DFS_DSFLOW_RATE_CONFIG_STRU) * TAF_NVIM_DIFF_DFS_NUM);
            return;
        }

        pstSwitchDdrInfo->ulMultiDfsFlg = VOS_FALSE;

        TAF_APS_Recorrect_ProfileNum(&stRateConfig, 1);
        PS_MEM_CPY(&pstSwitchDdrInfo->astDfsConfig[0], &stRateConfig, sizeof(TAF_NV_DFS_DSFLOW_RATE_CONFIG_STRU));
    }
    else
    {
        pstSwitchDdrInfo->ulMultiDfsFlg = VOS_TRUE;

        TAF_APS_Recorrect_ProfileNum(stMultiDfsConfig.astDfsConfig, TAF_NVIM_DIFF_DFS_NUM);
        PS_MEM_CPY((VOS_VOID *)pstSwitchDdrInfo->astDfsConfig, (VOS_VOID *)stMultiDfsConfig.astDfsConfig, sizeof(TAF_NV_MULTI_DFS_DSFLOW_RATE_CONFIG_STRU));
    }

    /* NV的第一个profile存的是最小的ddr频率 */
    pstSwitchDdrInfo->ulMinBand = pstSwitchDdrInfo->astDfsConfig[0].astProfile[0].ulDdrBand;

    /* NV的最后一个profile存的是最大的ddr频率 */
    ulIndex = pstSwitchDdrInfo->astDfsConfig[0].ulProfileNum - 1;
    pstSwitchDdrInfo->ulMaxBand = pstSwitchDdrInfo->astDfsConfig[0].astProfile[ulIndex].ulDdrBand;

    return;
}
VOS_VOID TAF_APS_Recorrect_ProfileNum(
    TAF_NV_DFS_DSFLOW_RATE_CONFIG_STRU     *pstDfsProfile,
    VOS_UINT32                              ulProfileNum)
{
    VOS_UINT32                              i;
    TAF_NV_DFS_DSFLOW_RATE_CONFIG_STRU     *pstDfsProTemp;

    for (i = 0; i < ulProfileNum; i++)
    {
        pstDfsProTemp = pstDfsProfile + i;

        if (pstDfsProTemp->ulProfileNum > TAF_NVIM_DFS_MAX_PROFILE_NUM)
        {
            pstDfsProTemp->ulProfileNum = TAF_NVIM_DFS_MAX_PROFILE_NUM;
        }
    }

    return;
}

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)

/* Modified by y00314741 for  CDMA Iteration 17, 2015-7-17, begin */
VOS_VOID TAF_APS_Read1xPktDataInactivityTimeLenFromNv(VOS_VOID)
{
    TAF_NVIM_1X_DATA_GENERIC_CONFIG_STRU   stDataGenericConfig;

    PS_MEM_SET(&stDataGenericConfig, 0 , sizeof(stDataGenericConfig));

    if (NV_OK != NV_Read(en_NV_Item_1X_DATA_GENERIC_CONFIG, &stDataGenericConfig, sizeof(stDataGenericConfig)))
    {
        /*读取NV 失败，设置默认值 */
        TAF_APS_Set1xPktDataInactivityTimerLen(TAF_APS_1X_PACKET_DATA_INACTIVITY_TIME_LEN);

        return;
    }

    /* Added by y00322978 for CDMA Iteration 17, 2015-7-8, begin */
    TAF_APS_LogReadNVInfo(en_NV_Item_1X_DATA_GENERIC_CONFIG,
                          sizeof(TAF_NVIM_1X_DATA_GENERIC_CONFIG_STRU),
                          WUEPS_PID_TAF,
                          (VOS_UINT8 *)&stDataGenericConfig);
    /* Added by y00322978 for CDMA Iteration 17, 2015-7-8, end */

    TAF_APS_Set1xPktDataInactivityTimerLen(stDataGenericConfig.uc1XPktDtInactivityTmrLen);

    return;
}
/* Modified by y00314741 for  CDMA Iteration 17, 2015-7-17, end */
/* Added by y00314741 for  CDMA Iteration 17, 2015-7-2, begin */
/*****************************************************************************
 函 数 名  : TAF_APS_ReadEvdoPktDataInactivityTimeLenFromNv
 功能描述  : 从NV中获取MaxNoDataTimeLen
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月2日
    作    者   : y00314741
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID TAF_APS_ReadEvdoPktDataInactivityTimeLenFromNv(VOS_VOID)
{
    TAF_NVIM_EVDO_DATA_GENERIC_CONFIG_STRU                      stDataGenericConfig;

    PS_MEM_SET(&stDataGenericConfig, 0 , sizeof(stDataGenericConfig));

    if (NV_OK != NV_Read(en_NV_Item_EVDO_DATA_GENERIC_CONFIG, &stDataGenericConfig, sizeof(stDataGenericConfig)))
    {
        /*读取NV 失败，设置默认值 */
        TAF_APS_SetEvdoPktDataInactivityTimerLen(TAF_APS_EVDO_PACKET_DATA_INACTIVITY_TIME_LEN);

        return;
    }

    TAF_APS_SetEvdoPktDataInactivityTimerLen(stDataGenericConfig.ucEvdoPktDtInactivityTmrLen);

    TAF_APS_LogReadNVInfo(en_NV_Item_EVDO_DATA_GENERIC_CONFIG,
                          sizeof(TAF_NVIM_EVDO_DATA_GENERIC_CONFIG_STRU),
                          WUEPS_PID_TAF,
                          (VOS_UINT8 *)&stDataGenericConfig);

    return;
}
/* Added by y00314741 for  CDMA Iteration 17, 2015-7-2, end */


VOS_VOID TAF_APS_Read1xDataSoCfgNvim(VOS_VOID)
{
    TAF_NVIM_1X_DATA_SO_CFG             st1xDataSoCfg;

    PS_MEM_SET(&st1xDataSoCfg, 0 , sizeof(st1xDataSoCfg));

    if (NV_OK != NV_Read(en_Nv_Item_1X_DATA_SO_CFG, &st1xDataSoCfg, sizeof(st1xDataSoCfg)))
    {
        /*读取NV 失败，设置默认值 */
        TAF_APS_SetFsmCtxCdataSoInitType(TAF_APS_CDATA_SO_33_PPP_PKT_DATA);

        return;
    }

    if ((TAF_APS_CDATA_SO_33_PPP_PKT_DATA       != st1xDataSoCfg.usSo)
     && (TAF_APS_CDATA_SO_7_PPP_PKT_DATA_PRE707 != st1xDataSoCfg.usSo))
    {
       TAF_APS_SetFsmCtxCdataSoInitType(TAF_APS_CDATA_SO_33_PPP_PKT_DATA);
    }
    else
    {
       TAF_APS_SetFsmCtxCdataSoInitType(st1xDataSoCfg.usSo);
    }
}


#endif

#if (FEATURE_ON == FEATURE_LTE)
VOS_VOID TAF_APS_ReadPdnTeardownPolicyNV(VOS_VOID)
{
    TAF_NV_PDN_TEARDOWN_POLICY_STRU     stNvPdnPolicy;
    VOS_UINT32                          ulRslt;

    PS_MEM_SET(&stNvPdnPolicy, 0x00, sizeof(TAF_NV_PDN_TEARDOWN_POLICY_STRU));

    ulRslt = NV_Read(en_NV_Item_PDN_TEARDOWN_POLICY,
                &stNvPdnPolicy, sizeof(TAF_NV_PDN_TEARDOWN_POLICY_STRU));
    if (NV_OK == ulRslt)
    {
        TAF_APS_SetAllowDefPdnTeardownFlg(stNvPdnPolicy.ucAllowDefPdnTeardownFlg);
    }
    else
    {
        TAF_WARNING_LOG(WUEPS_PID_TAF,
            "TAF_APS_ReadPdnTeardownPolicyNV: Read en_NV_Item_PDN_TEARDOWN_POLICY failed!");
    }

    return;
}
#endif

/*lint +e958*/

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
