

/*****************************************************************************
  1 头文件包含
*****************************************************************************/


#include "PsTypeDef.h"
#include "PsCommonDef.h"
#include "NasComm.h"
#include "NasCommDef.h"
#include "TafMmaCtx.h"
#include "TafSdcCtx.h"
#include "MmaAppLocal.h"
#include "Taf_Status.h"
#include "NasUsimmApi.h"
#include "msp_nvim.h"
#include "TafApsProcNvim.h"
#include "TafMmaFsmPhoneModeTbl.h"
#include "TafMmaPreProcAct.h"
#include "TafLog.h"
#include "TafSdcLib.h"
#include "TafMmaSndInternalMsg.h"
#include "TafMmaSndTaf.h"
#include "TafMmaSndXsms.h"
/* Added by k902809 for Iteration 11, 2015-3-24, begin */
#include "TafMmaNetworkNameTbl.h"
#include "MmaAppLocal.h"
/* Added by k902809 for Iteration 11, Iteration 11 2015-3-24, end */
#if (FEATURE_ON == FEATURE_IMS)
#include "MmaMsccInterface.h"
#endif

#include "TafMmaSndApp.h"
#include "TafMmaProcNvim.h"
#include "TafMmaMntn.h"
#include "TafMmaFsmPhoneMode.h"
#include "MmaUphyInterface.h"
#include "TafMmaFsmPhoneMode.h"

#include "TafMmaProcUsim.h"

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
#include "TafMtcApi.h"
#endif
#include "NasComm.h"
#include "TafMmaProcUsim.h"
#include "TafMmaSndMscc.h"

#include "TafStdlib.h"

#include "TafMmaComFunc.h"
/* Add by l00305157 for Service_State_Optimize_PhaseII, 2015-02-11, begin */
#include "Taf_Status.h"
/* Add by l00305157 for Service_State_Optimize_PhaseII, 2015-02-11, end */

/* Added by y00322978 for CDMA 1X Iteration 11, 2015-3-30, begin */
#include "TafMmaMain.h"
/* Added by y00322978 for CDMA 1X Iteration 11, 2015-3-30, end */
/* Added by k902809 for Iteration 11, 2015-3-30, begin */
#include "NasMmlCtx.h"
/* Added by k902809 for Iteration 11, Iteration 11 2015-3-30, end */

#include "TafMmaComFunc.h"

/* Added by l00301449 for CDMA STK, 2015-07-03, begin */
#include "TafMmaSndStk.h"
/* Added by l00301449 for CDMA STK, 2015-07-03, end */

/* Added by y00322978 for CDMA Iteration 17, 2015-7-4, begin */
#include "hsm_mma_pif.h"
#include "NasOmTrans.h"
#include "TafMmaSndOm.h"
#include "xcc_mma_pif.h"
/* Added by y00322978 for CDMA Iteration 17, 2015-7-4, end */


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_TAF_MMA_PREPROC_ACT_C


/*****************************************************************************
  2 全局变量定义
*****************************************************************************/
extern MMA_ME_PERSONALISATION_STATUS_STRU       g_stMmaMePersonalisationStatus;
extern VOS_UINT8                                g_ucMmaOmConnectFlg ;
extern MMA_TIMER_ST                             g_stPhPhResetTimer;
extern STATUS_CONTEXT_STRU                      g_StatusContext;
extern VOS_UINT32                               g_ucUsimHotOutFlag;
extern NAS_MMA_NEW_ADD_GLABAL_VARIABLE_FOR_PC_REPLAY_ST g_MmaNewValue;
extern VOS_UINT8                               g_aucMmaImei[TAF_PH_IMEI_LEN-1];
extern TAF_MMA_GLOBAL_VALUE_ST                 gstMmaValue;
extern MMA_TI_TABLE                            gastMmaTiTab[MMA_MAX_TI];
extern MMA_INTERNAL_TIMER_ST                   gstMmaInternalTimer;
extern MMA_TIMER_ST                            g_MmaSimTimer[TAF_SIM_TIMER_NUM];
extern MMA_TIMER_ST                                     g_stNetScanProtectTimer;
extern MMA_TIMER_ST                                     g_stAbortNetScanProtectTimer;


/*lint -save -e958 */

/*****************************************************************************
  3 函数实现
*****************************************************************************/
#ifdef __PS_WIN32_RECUR__

VOS_UINT32 NAS_MMA_RestoreContextData(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMA_SDT_MSG_ST                      *pRcvMsgCB;
    NAS_MMA_OUTSIDE_RUNNING_CONTEXT_ST      *pstOutsideCtx;

    pRcvMsgCB     = (NAS_MMA_SDT_MSG_ST *)pstMsg;

    if (EVT_NAS_MMA_OUTSIDE_RUNNING_CONTEXT_FOR_PC_REPLAY == pRcvMsgCB->usMsgID)
    {
        pstOutsideCtx = &pRcvMsgCB->stOutsideCtx;


        g_MmaNewValue.ucSimType = pstOutsideCtx->ucSimType;

        TAF_SDC_SetSimImsi(pstOutsideCtx->pc_g_aucMmaImsi);

        PS_MEM_CPY(g_aucMmaImei, pstOutsideCtx->pc_g_aucMmaImei, TAF_PH_IMEI_LEN-1);


        PS_MEM_CPY(&g_StatusContext, &pstOutsideCtx->pc_g_StatusContext, sizeof(STATUS_CONTEXT_STRU));
        PS_MEM_CPY(&gstMmaValue, &pstOutsideCtx->pc_gstMmaValue, sizeof(TAF_MMA_GLOBAL_VALUE_ST));
        gstMmaValue.pg_StatusContext = &g_StatusContext;

        PS_MEM_CPY(&g_stMmaMePersonalisationStatus, &pstOutsideCtx->pc_g_stMmaMePersonalisationStatus, sizeof(MMA_ME_PERSONALISATION_STATUS_STRU));
        PS_MEM_CPY(gastMmaTiTab, pstOutsideCtx->pc_gastMmaTiTab, MMA_MAX_TI*sizeof(MMA_TI_TABLE));

        PS_MEM_CPY(&gstMmaInternalTimer, &pstOutsideCtx->pc_gstMmaInternalTimer, sizeof(MMA_INTERNAL_TIMER_ST));

        PS_MEM_CPY(g_MmaSimTimer, pstOutsideCtx->pc_g_MmaSimTimer, TAF_SIM_TIMER_NUM*sizeof(MMA_TIMER_ST));

        MMA_INFOLOG("MMA: NAS_MMA_RestoreContextData - data is restored.");
    }
    return VOS_TRUE;
}


VOS_UINT32 NAS_MMA_RestoreFixedContextData(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    NAS_MMA_FIXED_CONTEXT_MSG_ST       *pRcvMsgCB;
    NAS_MMA_FIXED_PART_CONTEXT_ST      *pstOutsideCtx;
    VOS_UINT32                          ulVersion;
    NAS_NVIM_ROAM_CFG_INFO_STRU         stRoamFeature;
    MN_MMA_LAST_SETTED_SYSCFG_SET_STRU *pstLastSyscfgSet = VOS_NULL_PTR;

    NAS_NVIM_FOLLOWON_OPENSPEED_FLAG_STRU               stQuickStartFlg;

    stQuickStartFlg.ulQuickStartSta = MMA_QUICK_START_DISABLE;

    pstLastSyscfgSet = MN_MMA_GetLastSyscfgSetAddr();

    pRcvMsgCB        = (NAS_MMA_FIXED_CONTEXT_MSG_ST *)pstMsg;

    if (EVT_NAS_MMA_FIXED_PART_CONTEXT == pRcvMsgCB->usMsgID)
    {
        pstOutsideCtx = &pRcvMsgCB->stFixedOutsideCtx;

        ulVersion = pstOutsideCtx->ulVersion;
        switch (ulVersion)
        {
            case 1:      /* restore new para here, no break. */
                gstMmaBandCapa = pstOutsideCtx->gstMmaBandCapa;
            case 0:
                /* 全局变量恢复 */

                TAF_SDC_SetSimImsi(pstOutsideCtx->aucMmaImsi);

                g_MmaNewValue.ucSimType = pstOutsideCtx->aucSimType;

                pstLastSyscfgSet->stUserSetBand.uUserSetGuBand.ulPrefBand = pstOutsideCtx->ulPrefBand;
                pstLastSyscfgSet->stUserSetBand.uUserSetUeFormatGuBand = pstOutsideCtx->Band;
                gstMmaValue.ulQuickStartFlg = pstOutsideCtx->ulQuickStartFlg;

                gstMmaValue.stEfustServiceCfg = pstOutsideCtx->stEfustServiceCfg;

                PS_MEM_CPY(g_MmaNewValue.aucImeiBcd, pstOutsideCtx->aucImeiBcd, TAF_PH_IMEI_LEN+1);

                gstMmaValue.stSetMsClass.MsClassType = pstOutsideCtx->MsClassType;
                /* Modified by h00313353 for Iteration 13, 2015-4-15, begin */
                pstLastSyscfgSet->enRoam = pstOutsideCtx->enRoam;
                /* Modified by h00313353 for Iteration 13, 2015-4-15, end */
                gstMmaValue.ucRoamFeatureStatus = pstOutsideCtx->ucRoamFeatureStatus;

                gstMmaValue.stMeInfo = pstOutsideCtx->stMeInfo;

                g_MmaNewValue.stSimLockPlmnInfo = pstOutsideCtx->stSimLockPlmnInfo;
                g_MmaNewValue.stCardLockStatus = pstOutsideCtx->stCardLockStatus;
                g_MmaNewValue.stSimLockMaxTimes = pstOutsideCtx->stSimLockMaxTimes;

                PS_MEM_CPY(g_MmaNewValue.aucPwd, pstOutsideCtx->aucPwd, TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX+1);
                g_MmaNewValue.usSimMePersonalisationActFlag = pstOutsideCtx->usSimMePersonalisationActFlag;
                PS_MEM_CPY(g_MmaNewValue.aucSimPersonalisationStr, pstOutsideCtx->aucSimPersonalisationStr, TAF_MAX_IMSI_LEN+1);


                /* NV项的恢复 */
                MMA_WriteValue2Nvim(en_NV_Item_User_Set_Freqbands, &pstOutsideCtx->ulPrefBand, sizeof(VOS_UINT32));
                MMA_WriteValue2Nvim(en_NV_Item_Support_Freqbands, &pstOutsideCtx->Band, sizeof(MMA_UE_SUPPORT_FREQ_BAND_STRU));

                stQuickStartFlg.ulQuickStartSta = pstOutsideCtx->ulQuickStartFlg;
                MMA_WriteValue2Nvim(en_NV_Item_FollowOn_OpenSpeed_Flag, &(stQuickStartFlg.ulQuickStartSta), sizeof(NAS_NVIM_FOLLOWON_OPENSPEED_FLAG_STRU));

                MMA_WriteValue2Nvim(en_NV_Item_RF_Auto_Test_Flag, &pstOutsideCtx->usRfAutoTestFlg, sizeof(VOS_UINT16));
                MMA_WriteValue2Nvim(en_NV_Item_EFust_Service_Cfg, &pstOutsideCtx->stEfustServiceCfg, sizeof(MMA_EFUST_SERVICE_CFG_STRU));

                MMA_WriteValue2Nvim(en_NV_Item_IMEI, pstOutsideCtx->aucImeiBcd, TAF_PH_IMEI_LEN);


                MMA_WriteValue2Nvim(en_NV_Item_MMA_MsClass, &pstOutsideCtx->MsClassType, sizeof(TAF_PH_MS_CLASS_TYPE));
                stRoamFeature.ucRoamFeatureFlg = pstOutsideCtx->ucRoamFeatureStatus;
                /* Modified by h00313353 for Iteration 13, 2015-4-15, begin */
                stRoamFeature.ucRoamCapability = pstOutsideCtx->enRoam;
                /* Modified by h00313353 for Iteration 13, 2015-4-15, end */
                MMA_WriteValue2Nvim(en_NV_Item_Roam_Capa, &stRoamFeature, 2);

                MMA_WriteValue2Nvim(en_NV_Item_FMRID, &pstOutsideCtx->stMeInfo.FmrId, TAF_MAX_MFR_ID_LEN);
                MMA_WriteValue2Nvim(en_NV_Item_ProductName, &pstOutsideCtx->stMeInfo.ProductName, TAF_PH_PRODUCT_NAME_LEN);
                /* MMA_WriteValue2Nvim(en_NV_Item_ModelId, &pstOutsideCtx->stMeInfo.ModelId, TAF_MAX_MODEL_ID_LEN); */

                MMA_WriteValue2Nvim(en_NV_Item_CustomizeSimLockPlmnInfo, &pstOutsideCtx->stSimLockPlmnInfo, sizeof(TAF_CUSTOM_SIM_LOCK_PLMN_INFO_STRU));
                MMA_WriteValue2Nvim(en_NV_Item_CardlockStatus, &pstOutsideCtx->stCardLockStatus, sizeof(TAF_CUSTOM_CARDLOCK_STATUS_STRU));
                MMA_WriteValue2Nvim(en_NV_Item_CustomizeSimLockMaxTimes, &pstOutsideCtx->stSimLockMaxTimes, sizeof(TAF_CUSTOM_SIM_LOCK_MAX_TIMES_STRU));

                MMA_WriteValue2Nvim(en_NV_Item_Sim_Personalisation_Pwd, pstOutsideCtx->aucPwd, TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX);
                MMA_WriteValue2Nvim(en_NV_Item_Me_Personalisation_Active_Flag, &pstOutsideCtx->usSimMePersonalisationActFlag, 2);
                MMA_WriteValue2Nvim(en_NV_Item_Sim_Personalisation_Imsi_Str, pstOutsideCtx->aucSimPersonalisationStr, pstOutsideCtx->aucSimPersonalisationStr[0]+1);

                break;
            default:
                break;
        }

        MMA_INFOLOG("MMA: NAS_MMA_RestoreFixedContextData - data is restored.");
    }
    return VOS_TRUE;
}
#endif


VOS_UINT32 TAF_MMA_SetMiniMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;

    PS_MEM_SET(&stPhMode, 0x00, sizeof(stPhMode));

    stPhMode.CmdType    = TAF_PH_CMD_SET;
    stPhMode.PhMode     = ucSetMode;

    /* 当前不为FULL模式,都可以直接回复成功 */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
#if (FEATURE_ON == FEATURE_LTE)
        (VOS_VOID)NVM_Flush();
#endif
        (VOS_VOID)NV_Flush();

        TAF_SDC_SetCurPhoneMode(ucSetMode);

        /* 上报TAF_PH_EVT_OPER_MODE_CNF事件 */
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 TAF_MMA_SetRfOffMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;

    PS_MEM_SET(&stPhMode, 0x00, sizeof(stPhMode));

    stPhMode.CmdType    = TAF_PH_CMD_SET;
    stPhMode.PhMode     = ucSetMode;

    /* 当前模式不为FULL模式，直接上报成功 */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        TAF_SDC_SetCurPhoneMode(ucSetMode);

        /* 上报TAF_PH_EVT_OPER_MODE_CNF事件 */
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 TAF_MMA_SetFtMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;

    PS_MEM_SET(&stPhMode, 0x00, sizeof(stPhMode));

    stPhMode.CmdType    = TAF_PH_CMD_SET;
    stPhMode.PhMode     = ucSetMode;

    /* 当前模式不为FULL模式，直接上报成功 */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        TAF_SDC_SetCurPhoneMode(ucSetMode);

        /* 上报TAF_PH_EVT_OPER_MODE_CNF事件 */
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 TAF_MMA_SetVdfMiniMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;

    PS_MEM_SET(&stPhMode, 0x00, sizeof(stPhMode));

    stPhMode.CmdType    = TAF_PH_CMD_SET;
    stPhMode.PhMode     = ucSetMode;

    /* 当前模式不为FULL模式，直接上报成功 */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        TAF_SDC_SetCurPhoneMode(ucSetMode);

        /* 上报TAF_PH_EVT_OPER_MODE_CNF事件 */
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 TAF_MMA_SetLowPowerMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;

    PS_MEM_SET(&stPhMode, 0x00, sizeof(stPhMode));

    stPhMode.CmdType    = TAF_PH_CMD_SET;
    stPhMode.PhMode     = ucSetMode;

    /* 当前模式不为FULL模式，直接上报成功 */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        TAF_SDC_SetCurPhoneMode(ucSetMode);

        /* 上报TAF_PH_EVT_OPER_MODE_CNF事件 */
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

    return VOS_FALSE;
}



VOS_UINT32 TAF_MMA_SetResetMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;
    VOS_UINT32                          ulRet;

    PS_MEM_SET(&stPhMode, 0x00, sizeof(stPhMode));

    stPhMode.CmdType    = TAF_PH_CMD_SET;
    stPhMode.PhMode     = ucSetMode;

    /* 上报TAF_PH_EVT_OPER_MODE_CNF事件 */
    MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

    /* 启动延迟定时器待AT恢复OK后重启单板*/
    g_stPhPhResetTimer.ulTimerId = TI_MN_PH_RESET;

    ulRet = NAS_StartRelTimer( &g_stPhPhResetTimer.MmaHTimer,
                               WUEPS_PID_MMA,
                               TI_MN_PH_RESET_LENGTH,
                               TAF_MMA,
                               TI_MN_PH_RESET,
                               VOS_RELTIMER_NOLOOP );

    if (VOS_OK !=  ulRet)
    {
        TAF_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_SetRestMode:start TI_MN_PH_RESET failed!");
    }

    return VOS_TRUE;
}



VOS_UINT32 TAF_MMA_SetFullMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;

    PS_MEM_SET(&stPhMode, 0x00, sizeof(stPhMode));

    /* 当前已经处于在线模式，直接上报成功 */
    if (ucSetMode == TAF_SDC_GetCurPhoneMode())
    {
        stPhMode.CmdType    = TAF_PH_CMD_SET;
        stPhMode.PhMode     = ucSetMode;

        /* 上报TAF_PH_EVT_OPER_MODE_CNF事件 */
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

#if (VOS_WIN32 == VOS_OS_VER)

    if (VOS_TRUE == g_ucMmaOmConnectFlg)
    {
        NAS_MMA_SndNVData();
    }
#endif

    return VOS_FALSE;
}


VOS_UINT32 TAF_MMA_SetPowerOffMode_PreProc(
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId,
    VOS_UINT8                           ucSetMode
)
{
    TAF_PH_OP_MODE_CNF_STRU                                 stPhMode;
    VOS_UINT16                                              usAppCfgSupportType;
#if (FEATURE_ON == FEATURE_POWER_ON_OFF)
    VOS_UINT32                          ulRslt;
#endif

    PS_MEM_SET(&stPhMode, 0x00, sizeof(stPhMode));

    usAppCfgSupportType     = TAF_SDC_GetAppCfgSupportType();
    stPhMode.CmdType        = TAF_PH_CMD_SET;
    stPhMode.PhMode         = ucSetMode;

    TAF_APS_SaveDsFlowInfoToNv();

    if (SYSTEM_APP_WEBUI == usAppCfgSupportType)
    {
#if (FEATURE_ON == FEATURE_POWER_ON_OFF)
        if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
        {
            /* 上报TAF_PH_EVT_OPER_MODE_CNF事件 */
            MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);





            /* 将TASKDELAY修改为启动定时器进行保护 */
            ulRslt = NAS_StartRelTimer(&g_stPowerDownDelayTimer.MmaHTimer,
                                  WUEPS_PID_MMA,
                                  TI_TAF_MMA_DELAY_POWER_DOWN_TIMER_LEN,
                                  TAF_MMA,
                                  TI_TAF_MMA_DELAY_POWER_DOWN,
                                  VOS_RELTIMER_NOLOOP );

            if ( VOS_OK !=  ulRslt)
            {
                MMA_WARNINGLOG("MN_PH_Switch2OffFunc():ERROR:Start TI_TAF_MMA_DELAY_POWER_DOWN Timer Error!");
                mdrv_sysboot_poweroff();
            }



            return VOS_TRUE;
        }

        return VOS_FALSE;
#endif
    }

    if (SYSTEM_APP_ANDROID == usAppCfgSupportType)
    {
        if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
        {
            /* 在上报事件前先调用COMM接口Flush NV */
#if (FEATURE_ON == FEATURE_LTE)
            (VOS_VOID)NVM_Flush();
#endif
            (VOS_VOID)NV_Flush();

            /* 其他模式下使用AT+CFUN=8也需要去激活USIM卡 */
            (VOS_VOID)NAS_USIMMAPI_DeactiveCardReq(WUEPS_PID_MMA);

            MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

            return VOS_TRUE;
        }

        return VOS_FALSE;
    }

    /* 目前除了WEBUI和ANDROID，其他后台不会下发AT+CFUN=8 */
    if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_NO_ERROR);

        return VOS_TRUE;
    }

    return VOS_FALSE;

}

VOS_UINT32 TAF_MMA_IsPhoneModeSetAllowed_PreProc(
    VOS_UINT16                          usClientId,
    TAF_PH_MODE                         ucPhMode
)
{
    TAF_SDC_RAT_PRIO_STRU              *pstRatPrio      = VOS_NULL_PTR;
    VOS_UINT8                           ucCurPhmode;

    /* 模式设置是否允许的表 VOS_TRUE :代表允许, VOS_FALSE:代表不允许 */
    VOS_UINT32                          aulOperModeSetTable[TAF_PH_MODE_NUM_MAX][TAF_PH_MODE_NUM_MAX] =
    {
        /* operate mode: MINI--0,FULL--1,TXOFF--2,RXOFF--3,RFOFF--4,FT--5,RESET--6,LOW--7
                         POWEROFF--8,LOWPOWER--9 */
        /*->0*/     /*->1*/     /*->2*/     /*->3*/     /*->4*/     /*->5*/     /*->6*/     /*->7*/     /* ->8 */   /* ->9 */
/*0->*/ {VOS_TRUE,  VOS_TRUE,   VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_TRUE},
/*1->*/ {VOS_TRUE,  VOS_TRUE,   VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_TRUE},
/*2->*/ {VOS_FALSE, VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE},
/*3->*/ {VOS_FALSE, VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE},
/*4->*/ {VOS_FALSE, VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_FALSE},
/*5->*/ {VOS_TRUE,  VOS_TRUE,   VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_TRUE},
/*6->*/ {VOS_FALSE, VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE,  VOS_FALSE},
/*7->*/ {VOS_TRUE,  VOS_TRUE,   VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_TRUE},
/*8->*/ {VOS_TRUE,  VOS_TRUE,   VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_TRUE},
/*9->*/ {VOS_TRUE,  VOS_TRUE,   VOS_FALSE,  VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_FALSE,  VOS_TRUE,   VOS_TRUE,   VOS_TRUE}
    };

    pstRatPrio                          = TAF_SDC_GetMsPrioRatList();
    ucCurPhmode                         = TAF_SDC_GetCurPhoneMode();

    /* 表中不准许设置，则直接回复失败 */
    if (VOS_FALSE                       == aulOperModeSetTable[ucCurPhmode][ucPhMode] )
    {
        return VOS_FALSE;
    }


    /* 异常保护:如果CL模式，且当前是AT设置模式，则上报设置失败 */
    if ((VOS_TRUE                       == TAF_SDC_IsConfigCLInterWorkCOutside())
     && (usClientId                     < AT_CLIENT_ID_BUTT))
    {
        return VOS_FALSE;
    }

    /* 异常保护:如果不是CL模式，且当前是CMMCA设置模式，则上报CMMCA失败 */
    if ((VOS_FALSE                      == TAF_SDC_IsConfigCLInterWorkCOutside())
     && (CMMCA_CLIENT_ID                == usClientId))
    {
        return VOS_FALSE;
    }


    /* 在CMMCA进行开机时候，如果当前不是L ONLY MODE,则直接回复CMMCA失败 */
    if ( (VOS_TRUE                      != TAF_SDC_IsLteOnlyMode(pstRatPrio))
      && (CMMCA_CLIENT_ID               == usClientId) )
    {
        return VOS_FALSE;
    }

    /* 可以进行模式设置 */
    return VOS_TRUE;

}


VOS_UINT32 TAF_MMA_ProcOmPhoneModeSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucOpId;
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucOldMode;
    MN_APP_REQ_MSG_STRU                *pstRcvMsg       = VOS_NULL_PTR;
    TAF_PH_OP_MODE_STRU                *pstPhModeSet    = VOS_NULL_PTR;

    PS_MEM_SET(&stPhMode, 0x00, sizeof(stPhMode));

    pstRcvMsg       = (MN_APP_REQ_MSG_STRU *)pstMsg;
    pstPhModeSet    = (TAF_PH_OP_MODE_STRU*)pstRcvMsg->aucContent;

    usClientId      = pstRcvMsg->clientId;
    ucOpId          = pstRcvMsg->opId;
    ulRet           = VOS_TRUE;
    ucOldMode       = TAF_SDC_GetCurPhoneMode();

    /* 模式设置状态机在运行则直接进入后处理缓存 */
    if (TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
    {
        return VOS_FALSE;
    }


    /* 判断当前是否允许进行模式设置 */
    if (VOS_FALSE == TAF_MMA_IsPhoneModeSetAllowed_PreProc(usClientId, pstPhModeSet->PhMode))
    {
        stPhMode.CmdType    = TAF_PH_CMD_SET;
        stPhMode.PhMode     = TAF_SDC_GetCurPhoneMode();

        MMA_PhModeReport(usClientId,  ucOpId, stPhMode, TAF_ERR_ERROR);

        return VOS_TRUE;
    }


    /* 根据不同的模式进行设置处理 */
    ulRet =TAF_MMA_ProcPhoneModeSet_PreProc(pstPhModeSet->PhMode, usClientId, ucOpId);

    if (ucOldMode != pstPhModeSet->PhMode)
    {
        /* 离开FT模式需要允许睡眠 */
        if (TAF_PH_MODE_FT == ucOldMode)
        {
            TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK(PWRCTRL_SLEEP_FTM);
            if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
            {
                (VOS_VOID)vos_printf("\n*TAF_MMA_ProcOmPhoneModeSetReq_PreProc:PWRCTRL_SLEEP_FTM,TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK\n");
            }
        }

        /* 进入FT模式需要禁止睡眠 */
        if (TAF_PH_MODE_FT == pstPhModeSet->PhMode)
        {
            TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK(PWRCTRL_SLEEP_FTM);
            if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
            {
                (VOS_VOID)vos_printf("\n*TAF_MMA_ProcOmPhoneModeSetReq_PreProc:PWRCTRL_SLEEP_FTM,TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK\n");
            }
        }
    }

    return ulRet;
}
VOS_UINT32 TAF_MMA_ProcPhoneModeSet_PreProc(
    TAF_PH_MODE                         ucCurrPhMode,
    VOS_UINT16                          usClientId,
    VOS_UINT8                           ucOpId
)
{
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;
    VOS_UINT32                          ulRet;

    PS_MEM_SET(&stPhMode, 0x00, sizeof(stPhMode));

    ulRet = VOS_TRUE;

    /* 根据不同的模式进行设置处理 */
    switch (ucCurrPhMode)
    {
        case TAF_PH_MODE_MINI:

            ulRet = TAF_MMA_SetMiniMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_RFOFF:

            ulRet = TAF_MMA_SetRfOffMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_FT:

            ulRet = TAF_MMA_SetFtMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_VDFMINI:

            ulRet = TAF_MMA_SetVdfMiniMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_LOWPOWER:

            ulRet = TAF_MMA_SetLowPowerMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_FULL:

            ulRet = TAF_MMA_SetFullMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_RESET:

            ulRet = TAF_MMA_SetResetMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        case TAF_PH_MODE_POWEROFF:

            ulRet = TAF_MMA_SetPowerOffMode_PreProc(usClientId, ucOpId, ucCurrPhMode);
            break;

        default:

            /* warning 打印 */
            TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_ProcPhoneModeSet_PreProc:invalid mode!");

            stPhMode.CmdType = TAF_PH_CMD_SET;
            stPhMode.PhMode = TAF_SDC_GetCurPhoneMode();
            MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_ERROR);

            break;
    }

    return ulRet;
}



VOS_UINT32 TAF_MMA_ProcTafPhoneModeSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucOpId;
    TAF_PH_OP_MODE_CNF_STRU             stPhMode;
    VOS_UINT32                          ulRet;
    VOS_UINT8                           ucOldMode;
    TAF_PH_MODE                         ucCurrPhMode;
    TAF_MMA_PHONE_MODE_SET_REQ_STRU    *pstRcvMsg       = VOS_NULL_PTR;

    PS_MEM_SET(&stPhMode, 0x00, sizeof(stPhMode));

    pstRcvMsg       = (TAF_MMA_PHONE_MODE_SET_REQ_STRU *)pstMsg;

    usClientId      = pstRcvMsg->stCtrl.usClientId;
    ucOpId          = pstRcvMsg->stCtrl.ucOpId;
    ulRet           = VOS_TRUE;
    ucOldMode       = TAF_SDC_GetCurPhoneMode();
    ucCurrPhMode    = pstRcvMsg->stPhoneModePara.PhMode;

    /* 开机读取NV */
    if (TAF_PH_MODE_FULL == pstRcvMsg->stPhoneModePara.PhMode)
    {
        TAF_MMA_ReadLcCtrlNvim();

        /* 删除打印，防止在开机时候释放CPU导致睡眠，后续流程异常 */
    }

    /* 模式设置状态机在运行则直接进入后处理缓存 */
    if (TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
    {
        return VOS_FALSE;
    }

    /* 判断当前是否允许进行模式设置 */
    if (VOS_FALSE == TAF_MMA_IsPhoneModeSetAllowed_PreProc(usClientId, ucCurrPhMode))
    {
        stPhMode.CmdType = TAF_PH_CMD_SET;
        stPhMode.PhMode = TAF_SDC_GetCurPhoneMode();
        MMA_PhModeReport(usClientId, ucOpId, stPhMode, TAF_ERR_ERROR);

        return VOS_TRUE;
    }

    /* 在CMMCA进行开机时候，如果当前是手动模式，则修改为自动搜网模式 */
    if ( (NAS_MSCC_PIF_PLMN_SEL_MODE_AUTO    != TAF_MMA_GetPlmnSelectionMode())
      && (CMMCA_CLIENT_ID               == usClientId) )
    {
        /* CL模式时候，如果手动模式，则修正为自动搜网模式，进行异常保护，防止APP没有设置当前为自动搜网模式 */
        TAF_MMA_WritePlmnSelectionModeNvim(NAS_MSCC_PIF_PLMN_SEL_MODE_AUTO);
        TAF_MMA_SetPlmnSelectionMode(TAF_PH_PLMN_SEL_MODE_AUTO);
    }

    /* 根据不同的模式进行设置处理 */
    ulRet = TAF_MMA_ProcPhoneModeSet_PreProc(ucCurrPhMode, usClientId, ucOpId);

    if (ucOldMode != ucCurrPhMode)
    {
        /* 离开FT模式需要允许睡眠 */
        if (TAF_PH_MODE_FT == ucOldMode)
        {

            if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
            {
                (VOS_VOID)vos_printf("\n*TAF_MMA_ProcTafPhoneModeSetReq_PreProc:PWRCTRL_SLEEP_FTM,TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK\n");
            }

            TAF_DRVAPI_PWRCTRL_SLEEPVOTE_UNLOCK(PWRCTRL_SLEEP_FTM);
        }

        /* 进入FT模式需要禁止睡眠 */
        if (TAF_PH_MODE_FT == ucCurrPhMode)
        {
            TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK(PWRCTRL_SLEEP_FTM);
            if (VOS_TRUE == TAF_MMA_GetMmaLogInfoFlag())
            {
                (VOS_VOID)vos_printf("\n*TAF_MMA_ProcTafPhoneModeSetReq_PreProc:PWRCTRL_SLEEP_FTM,TAF_DRVAPI_PWRCTRL_SLEEPVOTE_LOCK\n");
            }
        }
    }

    return ulRet;
}
VOS_UINT32 TAF_MMA_RcvOmPhoneModeSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MN_APP_REQ_MSG_STRU                *pstRcvMsg       = VOS_NULL_PTR;
    TAF_PH_OP_MODE_STRU                *pstPhModeSet    = VOS_NULL_PTR;
    TAF_PH_OP_MODE_CNF_STRU             stPhModeCnf;
    VOS_UINT32                          ulRet;

    PS_MEM_SET(&stPhModeCnf, 0x00, sizeof(stPhModeCnf));

    pstRcvMsg       = (MN_APP_REQ_MSG_STRU *)pstMsg;
    pstPhModeSet    = (TAF_PH_OP_MODE_STRU *)pstRcvMsg->aucContent;

    /* 根据命令类型分发处理 */
    switch (pstPhModeSet->CmdType)
    {
        case TAF_PH_CMD_SET:            /* 设置类分发处理 */

            ulRet = TAF_MMA_ProcOmPhoneModeSetReq_PreProc(ulEventType, pstMsg);

            break;

        case TAF_PH_CMD_QUERY:          /* 查询类分发处理 */

            /* 上报事件命令类型填写 */
            stPhModeCnf.CmdType = TAF_PH_CMD_QUERY;

            /* 上报事件命令当前模式类型填写 */
            stPhModeCnf.PhMode = TAF_SDC_GetCurPhoneMode();

            /* 上报TAF_PH_EVT_OPER_MODE_CNF事件 */
            MMA_PhModeReport(pstRcvMsg->clientId, pstRcvMsg->opId, stPhModeCnf, TAF_ERR_NO_ERROR);

            ulRet = VOS_TRUE;
            break;

        default:

            /* 其他类分发处理 */
            TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvOmPhoneModeSetReq_PreProc:invalid para!");

            stPhModeCnf.CmdType    = pstPhModeSet->CmdType;
            stPhModeCnf.PhMode     = TAF_SDC_GetCurPhoneMode();
            MMA_PhModeReport(pstRcvMsg->clientId, pstRcvMsg->opId, stPhModeCnf, TAF_ERR_PARA_ERROR);
            ulRet = VOS_TRUE;
            break;
    }

    return ulRet;
}
VOS_UINT32 TAF_MMA_RcvUphyInitStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    UPHY_MMA_INIT_STATUS_IND_STRU      *pstRcvMsg = VOS_NULL;

    /* 当前状态机标识 */
    TAF_MMA_FSM_ID_ENUM_UINT32          enFsmId;

    pstRcvMsg                           = (UPHY_MMA_INIT_STATUS_IND_STRU*)pstMsg;
    enFsmId                             = TAF_MMA_GetCurrFsmId();

    NAS_TRACE_HIGH("UPHY Init Status:%d", (VOS_INT32)(pstRcvMsg->enInitStatus));

    if (VOS_TRUE == TAF_MMA_GetTestRoamFlag())
    {
        return VOS_TRUE;
    }

    /* 记录物理层初始化状态 */
    if (MMA_UPHY_INIT_STATUS_BUTT > pstRcvMsg->enInitStatus)
    {
        TAF_MMA_SetPhyInitStatus(pstRcvMsg->enInitStatus);
    }

    /* 开机过程中,等待初始化完成状态指示状态时直接进状态机处理该消息 */
    if ((TAF_MMA_PHONE_MODE_STA_WAIT_EXTERNAL_MODULE_INIT == TAF_MMA_FSM_GetFsmTopState())
     && (TAF_MMA_FSM_PHONE_MODE                           == enFsmId))

    {
        return VOS_FALSE;
    }

    /* 可维可测 */
    if (MMA_UPHY_INIT_STATUS_FAIL == pstRcvMsg->enInitStatus)
    {
        NAS_TRACE_HIGH("MMA UPHY INIT FAIL!!!!!!!!");

        /* 如果状态机是TAF_MMA_FSM_PHONE_MODE则记录失败状态并发送开机失败给AT */
        TAF_MMA_SndAtPsInitRsltInd_PhoneMode(TAF_MMA_PS_INIT_PHY_FAIL);
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvQryPhoneModeReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
   TAF_MMA_PHONE_MODE_QRY_REQ_STRU     *pstRcvMsg;
   TAF_MMA_PHONE_MODE_QRY_CNF_STRU     *pstPhoneModeQryCnf;

    pstRcvMsg          = (TAF_MMA_PHONE_MODE_QRY_REQ_STRU *)pstMsg;
    pstPhoneModeQryCnf = (TAF_MMA_PHONE_MODE_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MMA,
                                                                                         sizeof(TAF_MMA_PHONE_MODE_QRY_CNF_STRU));
    if (VOS_NULL_PTR == pstPhoneModeQryCnf)
    {
        MMA_ERRORLOG("TAF_MMA_RcvPhoneModeQry_PreProc: ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }

    PS_MEM_SET(((VOS_UINT8 *)pstPhoneModeQryCnf + VOS_MSG_HEAD_LENGTH),
               0x00,
               (sizeof(TAF_MMA_PHONE_MODE_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH));

    /* 填写消息头部信息 */
    pstPhoneModeQryCnf->ulReceiverPid          = pstRcvMsg->stCtrl.ulModuleId;
    pstPhoneModeQryCnf->ulMsgName              = ID_TAF_MMA_PHONE_MODE_QRY_CNF;

    PS_MEM_CPY(&pstPhoneModeQryCnf->stCtrl, &pstRcvMsg->stCtrl, sizeof(TAF_MMA_CTRL_STRU));

    pstPhoneModeQryCnf->ucPhMode               = TAF_SDC_GetCurPhoneMode();
    pstPhoneModeQryCnf->ucCmdType              = TAF_PH_CMD_QUERY;
    pstPhoneModeQryCnf->enErrorCause           = TAF_ERR_NO_ERROR;

    /* 发送消息 */
    PS_SEND_MSG(WUEPS_PID_MMA, pstPhoneModeQryCnf);

    return VOS_TRUE;
}



/* Added by h00313353 for iteration 9, 2015-2-17, begin */
/*****************************************************************************
 函 数 名  : TAF_MMA_GetUsimImsi
 功能描述  : 保存内部卡状态指示消息到缓存消息队列中
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年02月17日
    作    者   : h00313353
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID TAF_MMA_GetUsimImsi(
    USIMM_CARD_STATUS_STRU             *pstUsimStatus,
    VOS_UINT8                          *pucImsi
)
{
    if (USIMM_CARD_SERVIC_AVAILABLE == pstUsimStatus->enCardService)
    {
        if (USIMM_API_SUCCESS != NAS_USIMMAPI_GetCardIMSI(pucImsi))
        {
            TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvPihUsimStatusInd_PreProc:GetImSiFailed!");
        }
    }

    return;
}
/* Added by h00313353 for iteration 9, 2015-2-17, end */

VOS_UINT32 TAF_MMA_RcvPihUsimStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Modified by h00313353 for iteration 9, 2015-2-5, begin */
    USIMM_CARDSTATUS_IND_STRU          *pstUsimMsg = VOS_NULL_PTR;
    /* Modified by h00313353 for iteration 9, 2015-2-5, end */
    VOS_UINT32                          ulUsimStaChg;
    VOS_UINT32                          ulImsiChg;
    /* Added by h00313353 for iteration 9, 2015-2-5, begin */
    VOS_UINT8                           aucIMSI[NAS_MAX_IMSI_LENGTH];
    /* Added by h00313353 for iteration 9, 2015-2-5, end */
    VOS_UINT32                          ulRslt;
    TAF_MMA_FSM_ID_ENUM_UINT32          enCurrFsmId;

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    TAF_CTRL_STRU                       stCtrl;
    TAF_MTC_USIMM_CARD_SERVIC_ENUM_UINT16   enMtcCardState;
#endif

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    TAF_MMA_USIMM_CARD_TYPE_ENUM_UINT32           enCardType;                   /* 卡类型:SIM、USIM、ROM-SIM  */
    TAF_MMA_CARD_STATUS_ENUM_UINT8                enCardStatus;
#endif

    /* 初始化 */
#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    PS_MEM_SET(&stCtrl, 0x0,sizeof(stCtrl));
    enMtcCardState = TAF_MTC_USIMM_CARD_SERVIC_ABSENT;
#endif
    pstUsimMsg      = (USIMM_CARDSTATUS_IND_STRU *)pstMsg;

    if (VOS_TRUE == TAF_MMA_GetTestRoamFlag())
    {
        return VOS_TRUE;
    }

    /* Added by h00313353 for iteration 9, 2015-2-5, begin */
    /* 获取IMSI信息 */
    PS_MEM_SET(aucIMSI, 0x00, NAS_MAX_IMSI_LENGTH);
    TAF_MMA_GetUsimImsi(&(pstUsimMsg->stUsimSimInfo), aucIMSI);
    /* Added by h00313353 for iteration 9, 2015-2-5, end */

    /* 发送消息 AT_MMA_USIM_STATUS_IND 给AT */
    /* Modified by h00313353 for iteration 9, 2015-2-7, begin */
    TAF_MMA_SndATUsimmStatusInd(pstUsimMsg, aucIMSI);
    /* Modified by h00313353 for iteration 9, 2015-2-7, end */

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
    /* Modified by h00313353 for iteration 9, 2015-2-6, begin */
    TAF_MMA_ConvertCardType(pstUsimMsg->stUsimSimInfo.enCardType, &enCardType);
    TAF_MMA_ConvertCardStatus(pstUsimMsg->stUsimSimInfo.enCardService, &enCardStatus);
    TAF_MMA_SndSimStatusInd(enCardType, enCardStatus);
    /* Modified by h00313353 for iteration 9, 2015-2-6, end */
#endif

    /* Modified by h00313353 for iteration 9, 2015-2-7, begin */
    /* 分别保存Usim和Csim卡类型 */
    TAF_SDC_SaveUsimCardType(pstUsimMsg->stUsimSimInfo.enCardType);
    TAF_SDC_SaveCsimCardType(pstUsimMsg->stCsimUimInfo.enCardType);
    /* Modified by h00313353 for iteration 9, 2015-2-7, end */


    NAS_TRACE_HIGH("MMA Rcv PIH Usim Status Ind, USIM CardType: %d,USIM CardService: %d, FSM ID: %d, FSM STATE = %d",
                    pstUsimMsg->stUsimSimInfo.enCardType,
                    pstUsimMsg->stUsimSimInfo.enCardService,
                    TAF_MMA_GetCurrFsmId(),
                    TAF_MMA_FSM_GetFsmTopState());

    /* 在收到SIM上报的卡状态时，通知SMS模块当前的卡状态 */
    /* Modified by h00313353 for iteration 9, 2015-2-6, begin */
    MN_PH_SndMsgUsimStatus(pstUsimMsg->stUsimSimInfo.enCardService);
    /* Modified by h00313353 for iteration 9, 2015-2-6, end */

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    /* 填写CTL头 */
    TAF_API_CTRL_HEADER(&stCtrl, WUEPS_PID_MMA, 0, 0);

    /* 转换MTC卡状态 */
    /* Modified by h00313353 for iteration 9, 2015-2-6, begin */
    MMA_MTC_ConvertCardStatus(pstUsimMsg->stUsimSimInfo.enCardService, &enMtcCardState);
    /* Modified by h00313353 for iteration 9, 2015-2-6, end */

    /* 发送消息给MTC模块 */
    MTC_SetModemUsimmState(&stCtrl,enMtcCardState);
#endif

    /* Modified by h00313353 for iteration 9, 2015-2-7, begin */
    /* 判断卡状态和IMSI是否改变 */
    ulUsimStaChg    = TAF_MMA_IsCardStatusChanged(pstUsimMsg);
    ulImsiChg       = TAF_MMA_IsImsiChanged(&(pstUsimMsg->stUsimSimInfo), aucIMSI);
    /* Modified by h00313353 for iteration 9, 2015-2-7, end */

    if (VOS_TRUE == ulImsiChg)
    {
        /* Modified by h00313353 for iteration 9, 2015-2-5, begin */
        TAF_SDC_SetSimImsi(aucIMSI);
        TAF_SDC_SetLastSimImsi(aucIMSI);
        /* Modified by h00313353 for iteration 9, 2015-2-5, end */
    }

    /* Modified by h00313353 for iteration 9, 2015-2-7, begin */
    if (USIMM_CARD_SERVIC_AVAILABLE == (pstUsimMsg->stUsimSimInfo.enCardService))
    /* Modified by h00313353 for iteration 9, 2015-2-7, end */
    {
#if (VOS_WIN32 == VOS_OS_VER)
        /* PC工程持续集成适配代码，卡状态改变重新进行锁网锁卡校验 */
        g_stMmaMePersonalisationStatus.SimLockStatus = MMA_SIM_IS_UNLOCK;
#endif
        /* 突尼斯问题修改:在SIM卡状态发生改变,或者IMSI发生改变时,需要重新校验锁卡状态.
           因为校验函数只有在MMA_SIM_IS_UNLOCK状态才会重新校验,所以重新初始化此变量 */
        if ((VOS_TRUE == ulUsimStaChg)
         || (VOS_TRUE == ulImsiChg))
        {
            g_stMmaMePersonalisationStatus.SimLockStatus = MMA_SIM_IS_UNLOCK;
        }

        /* 检查是否锁卡 */
        (VOS_VOID)MMA_CheckMePersonalisationStatus();
    }

    /* Modified by h00313353 for iteration 9, 2015-2-7, begin */
    /* 分别保存Usim和Csim卡状态 */
    TAF_SDC_SaveUsimCardStatus(pstUsimMsg->stUsimSimInfo.enCardService);
    TAF_SDC_SaveCsimCardStatus(pstUsimMsg->stCsimUimInfo.enCardService);
    /* Modified by h00313353 for iteration 9, 2015-2-7, end */

    /* 更新卡的IMSI和SDC中的IMSI状态以及完成锁卡校验后,再上报卡状态指示给AT */
    TAF_MMA_SndAtIccStatusInd_PreProc(ulUsimStaChg);

    /* 卡状态以及IMSI都未改变则返回 */
    if ((VOS_FALSE == ulUsimStaChg)
     && (VOS_FALSE == ulImsiChg))
    {
        NAS_TRACE_HIGH("*TAF_MMA_RcvPihUsimStatusInd_PrePro:UsimStaChg = VOS_FALSE, ulImsiChg = VOS_FALSE\n");

        return VOS_TRUE;
    }

    /* 如果有卡则发送读文件请求 */
    /* Modified by h00313353 for iteration 9, 2015-2-7, begin */
    if (TAF_SDC_USIM_STATUS_VALID == TAF_SDC_GetSimStatus())
    /* Modified by h00313353 for iteration 9, 2015-2-7, end */
    {
        TAF_MMA_ReadUsimInfo_PreProc();
    }

    /* 公共处理在这里继续添加，不同状态机的区分处理在switch分支里加 */

    ulRslt      = VOS_FALSE;
    enCurrFsmId = TAF_MMA_GetCurrFsmId();

    /* 卡状态变化在不同的状态机时的预处理 */
    switch(enCurrFsmId)
    {
        case TAF_MMA_FSM_PHONE_MODE:

            ulRslt  = TAF_MMA_ProcUsimStatusIndInPhoneModeProc_PreProc(ulUsimStaChg, ulImsiChg);
            break;

        case TAF_MMA_FSM_SYS_CFG:

            ulRslt  = TAF_MMA_ProcUsimStatusIndInFsmSyscfg_PreProc(ulUsimStaChg, ulImsiChg);
            break;

        case TAF_MMA_FSM_IMS_SWITCH:

            ulRslt  = TAF_MMA_ProcUsimStatusIndInFsmImsSwitch_PreProc(ulUsimStaChg, ulImsiChg);
            break;

        default:

            ulRslt  = TAF_MMA_ProcUsimStatusIndInFsmMain_PreProc(ulUsimStaChg, ulImsiChg);
            break;

    }

    return ulRslt;
}
VOS_UINT32 TAF_MMA_RcvMmaInterUsimStatusChangeInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           aucImsi[TAF_SDC_MAX_IMSI_LEN];

    if (TAF_SDC_USIM_STATUS_VALID == TAF_SDC_GetUsimStatus())
    {
        if (USIMM_API_SUCCESS == USIMM_GetCardIMSI(aucImsi))
        {
            TAF_SDC_SetSimImsi(aucImsi);
            TAF_SDC_SetLastSimImsi(aucImsi);
        }
    }

    /* 如果当前为NULL状态则不需要处理 */
    if (STA_FSM_NULL == g_StatusContext.ulFsmState)
    {
        return VOS_TRUE;
    }

    /* 启动phone mode状态机 */
    return VOS_FALSE;
}


VOS_UINT32 TAF_MMA_RcvMmaSimLockStatusChangeInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* 如果当前非available状态则丢弃该消息 */
    if (TAF_SDC_USIM_STATUS_VALID != TAF_SDC_GetSimStatus())
    {
        return VOS_TRUE;
    }

    /* 上报一次卡状态 */
    TAF_MMA_SndAtIccStatusInd_PreProc(VOS_TRUE);

    /* 开机过程中,构造内部卡状态改变指示消息缓存 */
    if (TAF_MMA_FSM_MAIN != TAF_MMA_GetCurrFsmId())
    {
        TAF_MMA_SndInterUsimChangeInd();

        return VOS_TRUE;
    }

    /* 如果当前为NULL状态则不需要处理 */
    if (STA_FSM_NULL == g_StatusContext.ulFsmState)
    {
        return VOS_TRUE;
    }

    /* 启动phone mode状态机 */
    return VOS_FALSE;
}
/* Modified by y00322978 for CDMA 1X Iteration 11, 2015-3-26, begin */
VOS_UINT32 TAF_MMA_RcvMmaQrySyscfgReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_SYSCFG_QRY_REQ_STRU        *pstRcvMsg = VOS_NULL_PTR;
    TAF_MMA_SYSCFG_QRY_CNF_STRU        *pstSyscfgQryCnf;
    NAS_NVIM_ROAM_CFG_INFO_STRU         stRoamFeature;
    VOS_UINT32                          ulLength;

    NAS_MMA_NVIM_ACCESS_MODE_STRU       stAccessMode;
    NAS_NVIM_MS_CLASS_STRU              stMsClass;
    TAF_PH_MS_CLASS_TYPE                ucMsClassResult;

    TAF_MMA_LAST_SETTED_SYSCFG_SET_STRU *pstLastSyscfgSet = VOS_NULL_PTR;

    pstRcvMsg   = (TAF_MMA_SYSCFG_QRY_REQ_STRU *)pstMsg;

    ulLength        = 0;

    pstLastSyscfgSet = TAF_MMA_GetLastSyscfgSetAddr();

    pstSyscfgQryCnf = (TAF_MMA_SYSCFG_QRY_CNF_STRU*)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                           WUEPS_PID_MMA,
                                           sizeof(TAF_MMA_SYSCFG_QRY_CNF_STRU));

    if (VOS_NULL_PTR == pstSyscfgQryCnf)
    {
        MMA_ERRORLOG("TAF_MMA_RcvMmaSyscfgQry_PreProc:ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }


    /* Modified by y00322978 for CDMA 1X Iteration 11, 2015-3-27, begin */
    /* 初始化事件上报 */
    PS_MEM_SET( (VOS_INT8 *)pstSyscfgQryCnf + VOS_MSG_HEAD_LENGTH, 0X00,
            (VOS_SIZE_T)(sizeof(TAF_MMA_SYSCFG_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH) );
    /* Modified by y00322978 for CDMA 1X Iteration 11, 2015-3-27, end */

    /* 获取当前的接入模式优先级，如果关机，从NV:en_NV_Item_RAT_PRIO_LIST中读取 */
    if (STA_FSM_NULL == g_StatusContext.ulFsmState)
    {
        TAF_MMA_ReadNvimRatPrioList(&(pstLastSyscfgSet->stRatPrioList));
        PS_MEM_CPY(&(gstMmaValue.pg_StatusContext->stRatPrioList),
                   &(pstLastSyscfgSet->stRatPrioList), sizeof(TAF_MMA_RAT_ORDER_STRU));

        (VOS_VOID)NV_GetLength(en_NV_Item_MMA_AccessMode, &ulLength);

        if (NV_OK != NV_Read(en_NV_Item_MMA_AccessMode ,
                 &stAccessMode,
                 ulLength))

        {
            (stAccessMode.aucAccessMode)[1] = TAF_PLMN_PRIO_AUTO;
        }
        pstLastSyscfgSet->enPrioRat = (stAccessMode.aucAccessMode)[1] ;
    }

    pstSyscfgQryCnf->stSysCfg.stRatOrder = pstLastSyscfgSet->stRatPrioList;

    /* 获取当前用户设置的GU频段信息 */
    pstSyscfgQryCnf->stSysCfg.stGuBand = pstLastSyscfgSet->stUserSetBand.stOrigUserSetGuBand;

    /* 获取当前用户设置的漫游信息 */
    pstSyscfgQryCnf->stSysCfg.enRoam = pstLastSyscfgSet->enRoam;

    /* 获取当前用户设置的接入优先级 */
    pstSyscfgQryCnf->stSysCfg.enUserPrio = pstLastSyscfgSet->enPrioRat;

    if (STA_FSM_NULL == g_StatusContext.ulFsmState)
    {

        PS_MEM_SET(&stRoamFeature, 0x00, sizeof(stRoamFeature));

        (VOS_VOID)NV_GetLength(en_NV_Item_Roam_Capa, &ulLength);
        if (NV_OK == NV_Read(en_NV_Item_Roam_Capa,
                             &stRoamFeature,
                             ulLength))
        {
            pstSyscfgQryCnf->stSysCfg.enRoam = stRoamFeature.ucRoamCapability;
            MMA_INFOLOG1("TAF_MMA_RcvMmaSyscfgQry_PreProc: usRoamCapa", stRoamFeature.ucRoamCapability);
        }
        else
        {
            MMA_WARNINGLOG("TAF_MMA_RcvMmaSyscfgQry_PreProc:WARNING:Read from NVIM en_NV_Item_Roam_Capa Fail");
            pstSyscfgQryCnf->stSysCfg.enRoam = TAF_MMA_ROAM_NATIONAL_ON_INTERNATIONAL_OFF;
        }
    }

    /* 获取当前用户设置的服务域信息 */
    pstSyscfgQryCnf->stSysCfg.enSrvDomain = pstLastSyscfgSet->enSrvDomain;

    if (STA_FSM_NULL == g_StatusContext.ulFsmState)
    {

        PS_MEM_SET(&stMsClass, 0x00, sizeof(NAS_NVIM_MS_CLASS_STRU));


        /* 明确在关机状态获取服务域信息 */
        (VOS_VOID)NV_GetLength(en_NV_Item_MMA_MsClass, &ulLength);
        if (NV_OK != NV_Read(en_NV_Item_MMA_MsClass ,
                             &stMsClass,
                             ulLength))
        {
            MMA_WARNINGLOG("MMA_PhSysCfgQuery():WARNING:Read from NVIM MSClass Fail");
            ucMsClassResult = TAF_PH_MS_CLASS_A;
        }
        else
        {
            ucMsClassResult = stMsClass.ucMsClass;
        }

        MMA_ChangeClass2Srv(&pstSyscfgQryCnf->stSysCfg.enSrvDomain, ucMsClassResult);
    }

    /* 获取当前产品支持的用户设置的LTE频段信息 */
#if (FEATURE_ON == FEATURE_LTE)
    MN_MMA_GetSupportedUserLteBand(&pstSyscfgQryCnf->stSysCfg.stLBand);
#endif

    /* 设置失败或查询事件上报调用客户端 */
    PS_MEM_CPY(&pstSyscfgQryCnf->stCtrl, &pstRcvMsg->stCtrl, sizeof(TAF_MMA_CTRL_STRU));
    pstSyscfgQryCnf->ulReceiverPid            = WUEPS_PID_AT;
    pstSyscfgQryCnf->ulMsgName                = ID_TAF_MMA_SYSCFG_QRY_CNF;
    pstSyscfgQryCnf->enErrorCause             = TAF_ERR_NO_ERROR;

    PS_SEND_MSG(WUEPS_PID_MMA, pstSyscfgQryCnf);


    return VOS_TRUE;
}

/* Modified by y00322978 for CDMA 1X Iteration 11, 2015-3-26, end */
/* Added by c00318887 for AT&T phaseII, 2015-3-12, begin */
/*****************************************************************************
 函 数 名  : TAF_MMA_RcvAtEonsUcs2Req_PreProc
 功能描述  : 收到AT^EONSUCS2预处理
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年03月12日
    作    者   : c00318887
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvAtEonsUcs2Req_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_EONS_UCS2_REQ_STRU         *pstRcvMsg = VOS_NULL_PTR;
    TAF_MMA_EONS_UCS2_PLMN_NAME_STRU    stEonsUcs2PlmnName;
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucOpId;
    VOS_UINT8                           ucCtxIndex;

    pstRcvMsg = (TAF_MMA_EONS_UCS2_REQ_STRU *)pstMsg;

    PS_MEM_SET(&stEonsUcs2PlmnName, 0x00, sizeof(TAF_MMA_EONS_UCS2_PLMN_NAME_STRU));

    usClientId      = pstRcvMsg->stCtrl.usClientId;
    ucOpId          = pstRcvMsg->stCtrl.ucOpId;

    /* 如果g_stTafMmaCtx.astMmaOperCtx里TAF_MMA_OPER_EONS_UCS2_REQ类型结构正在被使用 */
    if (VOS_TRUE == TAF_MMA_IsOperTypeUsed(TAF_MMA_OPER_EONS_UCS2_REQ))
    {
        TAF_MMA_SndEonsUcs2Rsp(&(pstRcvMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_FAILURE,
                                TAF_ERR_ERROR,
                                &stEonsUcs2PlmnName);

        return VOS_TRUE;
    }

    /* 找不到未被使用的，回复失败 */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndEonsUcs2Rsp(&(pstRcvMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_FAILURE,
                                TAF_ERR_ERROR,
                                &stEonsUcs2PlmnName);

        return VOS_TRUE;
    }

    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    TAF_MMA_SetOperCtx(pstRcvMsg->stCtrl,
                       TAF_MMA_OPER_EONS_UCS2_REQ, ucCtxIndex);



    TAF_MMA_QryEonsUcs2(usClientId,ucOpId);

    return VOS_TRUE;
}
/* Added by c00318887 for AT&T phaseII, 2015-3-12, end */


VOS_UINT32 TAF_MMA_RcvAtParaReadReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MN_APP_REQ_MSG_STRU                *pstRcvMsg = VOS_NULL_PTR;
    VOS_UINT16                          usClientId;
    VOS_UINT8                           ucOpId;
    TAF_PARA_TYPE                       ucParaType;

    pstRcvMsg = (MN_APP_REQ_MSG_STRU *)pstMsg;

    ucParaType  = pstRcvMsg->aucContent[0];
    usClientId  = pstRcvMsg->clientId;
    ucOpId      = pstRcvMsg->opId;

    /* MMA参数查询调用 */
    if ((ucParaType >= TAF_PH_MS_CLASS_PARA )
     && (ucParaType <= (TAF_PH_PRODUCT_NAME_PARA + 1)))
    {
        MMA_QueryProc(usClientId, ucOpId, &ucParaType);

        return VOS_TRUE;
    }


    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_IsNetworkCapInfoChanged(
    TAF_SDC_NETWORK_CAP_INFO_STRU       *pstNewNwCapInfo
)
{
    TAF_SDC_NETWORK_CAP_INFO_STRU       *pstOldNwCapInfo = VOS_NULL_PTR;

    /* 先获取LTE的网络能力信息 */
    pstOldNwCapInfo = TAF_SDC_GetLteNwCapInfo();

    /* 新旧能力是否有变化 */
    if ((pstOldNwCapInfo->enLteCsCap   != pstNewNwCapInfo->enLteCsCap)
     || (pstOldNwCapInfo->enNwEmcBsCap != pstNewNwCapInfo->enNwEmcBsCap)
     || (pstOldNwCapInfo->enNwImsVoCap != pstNewNwCapInfo->enNwImsVoCap))
    {
        return VOS_TRUE;
    }

    /* 再获取GU的网络能力信息 */
    pstOldNwCapInfo = TAF_SDC_GetGuNwCapInfo();

    /* 新旧能力是否有变化 */
    if ((pstOldNwCapInfo->enLteCsCap   != pstNewNwCapInfo->enLteCsCap)
     || (pstOldNwCapInfo->enNwEmcBsCap != pstNewNwCapInfo->enNwEmcBsCap)
     || (pstOldNwCapInfo->enNwImsVoCap != pstNewNwCapInfo->enNwImsVoCap))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}


VOS_UINT32 TAF_MMA_RcvMsccNetworkCapabilityInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_NETWORK_CAPABILITY_INFO_IND_STRU               *pstRcvMsg       = VOS_NULL_PTR;
    TAF_SDC_NETWORK_CAP_INFO_STRU                           stNewNwCapInfo;
    VOS_UINT32                                              ulNwCapInfoChanged;

    pstRcvMsg = (MSCC_MMA_NETWORK_CAPABILITY_INFO_IND_STRU *)pstMsg;

    stNewNwCapInfo.enNwEmcBsCap = (TAF_SDC_NW_EMC_BS_CAP_ENUM_UINT8)pstRcvMsg->enNwEmcBsCap;
    stNewNwCapInfo.enNwImsVoCap = (TAF_SDC_NW_IMS_VOICE_CAP_ENUM_UINT8)pstRcvMsg->enNwImsVoCap;

    switch (pstRcvMsg->enLteCsCap)
    {
        case NAS_MSCC_PIF_LTE_CS_CAPBILITY_NO_ADDITION_INFO:
            stNewNwCapInfo.enLteCsCap = TAF_SDC_LTE_CS_CAPBILITY_NO_ADDITION_INFO;
            break;

        case NAS_MSCC_PIF_LTE_CS_CAPBILITY_CSFB_NOT_PREFER:
            stNewNwCapInfo.enLteCsCap = TAF_SDC_LTE_CS_CAPBILITY_CSFB_NOT_PREFER;
            break;

        case NAS_MSCC_PIF_LTE_CS_CAPBILITY_SMS_ONLY:
            stNewNwCapInfo.enLteCsCap = TAF_SDC_LTE_CS_CAPBILITY_SMS_ONLY;
            break;

        case NAS_MSCC_PIF_LTE_CS_CAPBILITY_NOT_SUPPORTED:
            stNewNwCapInfo.enLteCsCap = TAF_SDC_LTE_CS_CAPBILITY_NOT_SUPPORTED;
            break;

        default:
            stNewNwCapInfo.enLteCsCap = TAF_SDC_LTE_CS_CAPBILITY_BUTT;
            break;
    }

    /* 判断当前的网络能力与GU和L的能力相比是否发生了改变 */
    ulNwCapInfoChanged = TAF_MMA_IsNetworkCapInfoChanged(&stNewNwCapInfo);

    if (VOS_TRUE == ulNwCapInfoChanged)
    {
        /* update infomaton to the corresponding global variable according to RAT */
        TAF_MMA_SetNetworkCapabilityInfo(TAF_SDC_GetSysMode(), &stNewNwCapInfo);
    }

#if (FEATURE_IMS == FEATURE_ON)
    if (VOS_TRUE == TAF_SDC_GetImsSupportFlag())
    {
        /* 通知SPM网络能力变更，触发SPM处理缓存的消息，消息中不带当前的网络能力，由SPM取SDC全局变量中的 */
        TAF_MMA_SndSpmNetworkCapabilityChangeNotify();

        }
#endif


    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvPihUsimRefreshIndMsgProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Modified by h00313353 for iteration 9, 2015-2-6, begin */
    USIMM_STKREFRESH_IND_STRU          *pUsimRefreshIndMsg = VOS_NULL_PTR;
    /* Modified by h00313353 for iteration 9, 2015-2-6, end */
    /* Added by c00318887 for AT&T phaseII, 2015-2-15, begin */
    VOS_UINT8                           ucRefreshAllFileRestartFlag;
    /* Added by c00318887 for AT&T phaseII, 2015-2-15, end */
    VOS_UINT8                          *pucOldImsi;
    VOS_UINT8                           aucNewImsi[TAF_SDC_MAX_IMSI_LEN];
    VOS_UINT8                           ucIsImsiChanged;

    pUsimRefreshIndMsg = (USIMM_STKREFRESH_IND_STRU *)pstMsg;

    ucIsImsiChanged = VOS_FALSE;
    pucOldImsi      = TAF_SDC_GetLastSimImsi();
    if (USIMM_API_SUCCESS == USIMM_GetCardIMSI(aucNewImsi))
    {
        if (0 != VOS_MemCmp(pucOldImsi, aucNewImsi, TAF_SDC_MAX_IMSI_LEN))
        {
            ucIsImsiChanged = VOS_TRUE;
        }
    }

    /* 目前PIH上报文件刷新指示消息后, USIM继续上报卡状态指示给MMA
       后续此处逻辑被USIM删除后,MMA需要重新读取IMSI比较是否发生改变
       ,并进行关机和开机处理 */
    if (USIMM_REFRESH_ALL_FILE == pUsimRefreshIndMsg->enRefreshType)
    {
        /* Modified by c00318887 for AT&T phaseII, 2015-2-15, begin */

        /* 测试卡不重启，否则gcf 27.22.4.7.1.4会失败 */
        ucRefreshAllFileRestartFlag = TAF_SDC_GetRefreshAllFileRestartFlag();

#if (FEATURE_ON == FEATURE_ECALL)
        if (VOS_FALSE == TAF_MMA_ProcPihUsimRefreshIndEfUstFileChanged_PreProc())
        {
            TAF_MMA_SndInterUsimChangeInd();

            return VOS_TRUE;
        }
#endif

        if (((VOS_TRUE  == NAS_USIMMAPI_IsTestCard())
         || (VOS_FALSE == ucRefreshAllFileRestartFlag))
         && (VOS_FALSE == ucIsImsiChanged))
        {
            TAF_MMA_ReadUsimInfo_PreProc();

            return VOS_TRUE;
        }

        if (VOS_TRUE == TAF_SDC_GetCsEccExistFlg())
        {
            /* 存在CS业务，则启动定时器等待CS业务结束后关机 */
            TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_CS_SERVICE_DISCONN, TI_TAF_MMA_WAIT_CS_SERVICE_DISCONN_LEN);
            return VOS_TRUE;
        }

        /* 进入main触发关机流程 */
        return VOS_FALSE;
        /* Modified by c00318887 for AT&T phaseII, 2015-2-15, end */
    }

    if (USIMM_REFRESH_FILE_LIST == pUsimRefreshIndMsg->enRefreshType)
    {
        /* refresh 消息指示特定文件发生变更 */
        TAF_MMA_UsimRefreshIndFileListChangedMsgProc(pUsimRefreshIndMsg);

        return VOS_TRUE;
    }

    if (USIMM_REFRESH_3G_SESSION_RESET == pUsimRefreshIndMsg->enRefreshType)
    {
       return TAF_MMA_ProcPihUsimRefreshInd3gSessionReset_PreProc();
    }

    return VOS_TRUE;
}




VOS_UINT32 TAF_MMA_ProcPihUsimRefreshInd3gSessionReset_PreProc(VOS_VOID)
{
    TAF_MMA_ReadUsimInfo_PreProc();

    /* 当前状态机不是main状态机,构造内部卡状态改变指示消息缓存 */
    if (TAF_MMA_FSM_MAIN != TAF_MMA_GetCurrFsmId())
    {
        TAF_MMA_SndInterUsimChangeInd();

        return VOS_TRUE;
    }

    /* 非开机过程中收到卡状态不触发关机流程 */
    if (STA_FSM_NULL == g_StatusContext.ulFsmState)
    {
       return VOS_TRUE;
    }


    /* Added by c00318887 for AT&T phaseII, 2015-2-15, begin */
    if (VOS_TRUE == TAF_SDC_GetCsEccExistFlg())
    {
        /* 存在CS 业务，则启动定时器等待CS业务结束后关机 */
        TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_CS_SERVICE_DISCONN, TI_TAF_MMA_WAIT_CS_SERVICE_DISCONN_LEN);
        return VOS_TRUE;
    }
    /* Added by c00318887 for AT&T phaseII, 2015-2-15, end */

    /* 已经开机则进入main触发关机流程 */
    return VOS_FALSE;
}




VOS_VOID TAF_MMA_ReadUsimInfo_PreProc(VOS_VOID)
{
#if (FEATURE_ON == FEATURE_ECALL)
    /*  更新ecall模式信息 */
    TAF_MMA_UpdateCallMode();
#endif

    /* 读取SPN文件 */
    TAF_MMA_ReadSpnFile();

    TAF_MMA_ReadSimCphsOperNameFile();

    /* 网络选择菜单控制使能，开机需要读6F15文件 */
    TAF_MMA_ReadCustomerServiceProfileFile_SwitchOn();

    /* 接入技术平衡控制使能，开机读4F36文件 */
    TAF_MMA_ReadRatModeFile();

    TAF_MMA_ReadOplFile();
    TAF_MMA_ReadPnnFile();
    TAF_MMA_ReadSpdiFile();
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_READ_SIM_FILES, TI_TAF_MMA_WAIT_READ_SIM_FILES_LEN);
}





VOS_VOID TAF_MMA_SndAtIccStatusInd_PreProc(
    VOS_UINT32                          ulUsimStaChg
)
{
    VOS_UINT8                           ucSimStatus;
    VOS_UINT8                           ucSimLockStatus;
    TAF_MMA_FSM_ID_ENUM_UINT32          enCurrFsmId;
    VOS_UINT32                          ulFsmTopState;

    enCurrFsmId = TAF_MMA_GetCurrFsmId();

    ulFsmTopState = TAF_MMA_FSM_GetFsmTopState();

    ucSimStatus                         = MMA_GetUsimStatus(ulUsimStaChg);

    /* 获取锁卡状态 */
    if ( MMA_SIM_IS_LOCK == MMA_GetMeLockStatus() )
    {
        ucSimLockStatus                 = VOS_TRUE;
    }
    else
    {
        ucSimLockStatus                 = VOS_FALSE;
    }

    /* 热插拔期间不上报SIMST给AP */
   if ((TAF_PH_MODE_FULL == TAF_SDC_GetCurPhoneMode())
    && (VOS_TRUE         == g_ucUsimHotOutFlag))
   {
       g_ucUsimHotOutFlag = VOS_FALSE;

       return;
   }

   /* 适配RIL逻辑,上电第一次首先收到的是协议栈启动完成的命令(PSINIT)
      上电时上报的卡状态延迟到协议栈初始化完成之后上报上去 */
   if ((TAF_MMA_FSM_PHONE_MODE == enCurrFsmId)
    && (TAF_MMA_PHONE_MODE_STA_WAIT_EXTERNAL_MODULE_INIT == ulFsmTopState))
   {
       return;
   }

   TAF_MMA_SndAtIccStatusInd(ucSimStatus, ucSimLockStatus);

   return;
}

VOS_UINT32 TAF_MMA_ProcUsimStatusIndInPhoneModeProc_PreProc(
    VOS_UINT32                          ulUsimStaChg,
    VOS_UINT32                          ulImsiChg
)
{
    VOS_UINT32                          ulFsmTopState;

    ulFsmTopState                       = TAF_MMA_FSM_GetFsmTopState();

    /* 开机过程中，如果状态机处于以下状态，则直接进状态机处理卡状态上报 */
    if (VOS_TRUE == TAF_MMA_IsNeedProcUsimStatusIndInFsmPhoneMode(ulFsmTopState))
    {
        /* 直接进状态机处理卡状态上报 */
        return VOS_FALSE;
    }

    /* 卡状态或IMSI发生改变则重新读取卡文件并缓存卡状态改变消息 */
    TAF_MMA_SndInterUsimChangeInd();

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_ProcUsimStatusIndInFsmMain_PreProc(
    VOS_UINT32                          ulUsimStaChg,
    VOS_UINT32                          ulImsiChg
)
{
    TAF_PH_FPLMN_OPERATE_STRU           stFplmnOperate;

    if (VOS_TRUE == MN_MMA_GetRoamingBrokerFlg())
    {
        stFplmnOperate.ulCmdType = TAF_PH_FPLMN_DEL_ALL;
        TAF_MMA_SndMsccCFPlmnSetReq(&stFplmnOperate);
        TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_CFPLMN_SET_CNF, TI_TAF_MMA_WAIT_MSCC_CFPLMN_SET_CNF_LEN);
    }

    /* 非开机过程中收到卡状态不触发关机流程 */
    if (STA_FSM_NULL == g_StatusContext.ulFsmState)
    {
        return VOS_TRUE;
    }

    /* Added by c00318887 for AT&T phaseII, 2015-2-26, begin */
    if (VOS_TRUE == TAF_SDC_GetCsEccExistFlg())
    {
        /* 如果IMSI变更且存在CS 业务，则启动定时器等待CS业务结束后关机 */
        TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_CS_SERVICE_DISCONN, TI_TAF_MMA_WAIT_CS_SERVICE_DISCONN_LEN);
        return VOS_TRUE;
    }
    /* Added by c00318887 for AT&T phaseII, 2015-2-26, end */

    /* 已经开机则进入main触发关机流程 */
    return VOS_FALSE;

}
VOS_UINT32 TAF_MMA_ProcUsimStatusIndInFsmSyscfg_PreProc(
    VOS_UINT32                          ulUsimStaChg,
    VOS_UINT32                          ulImsiChg
)
{
    /* 发送内部的卡状态改变指示消息 */
    TAF_MMA_SndInterUsimChangeInd();

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_ProcUsimStatusIndInFsmImsSwitch_PreProc(
    VOS_UINT32                          ulUsimStaChg,
    VOS_UINT32                          ulImsiChg
)
{
    /* 发送内部的卡状态改变指示消息 */
    TAF_MMA_SndInterUsimChangeInd();

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMsccCampOnInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_CAMP_ON_IND_STRU_STRU      *pstCampOnInd = VOS_NULL_PTR;

    pstCampOnInd = (MSCC_MMA_CAMP_ON_IND_STRU_STRU*)pstMsg;

    /* MMA在处于射频被抢占的状态，直接丢弃如下消息，不刷新MMA的变量，
       不上报，此时如果AT有查询，还是射频资源存在时的状态*/
#if (FEATURE_ON == FEATURE_DSDS)
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, begin */
    if (VOS_FALSE == TAF_SDC_Get3GppRfAvailFlag())
    {
        return VOS_TRUE;
    }
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, end */
#endif

    if (VOS_TRUE == pstCampOnInd->ucCampOnFlg)
    {
        TAF_SDC_SetCampOnFlag(VOS_TRUE);
    }
    else
    {
        TAF_SDC_SetCampOnFlag(VOS_FALSE);
    }

    return VOS_TRUE;
}




VOS_UINT32 TAF_MMA_RcvMsccRfAvailableInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_RF_AVAILABLE_IND_STRU      *pstRfAvail = VOS_NULL_PTR;

    pstRfAvail = (MSCC_MMA_RF_AVAILABLE_IND_STRU*)pstMsg;

    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-16, begin */
    if (NAS_MSCC_PIF_RF_AVAILABLE_RAT_TYPE_3GPP == pstRfAvail->enRatType)
    {
        TAF_SDC_Set3GppRfAvailFlag(pstRfAvail->ucRfAvailFlg);

        /* 如果延迟定时器正在运行，重启定时器 */
        if ((TAF_MMA_TIMER_STATUS_RUNING == TAF_MMA_GetTimerStatus(TI_TAF_MMA_DELAY_REPORT_SERVICE_STATUS))
         && (VOS_TRUE == pstRfAvail->ucRfAvailFlg))
        {
            /* 重启定时器 */
            TAF_MMA_StopTimer(TI_TAF_MMA_DELAY_REPORT_SERVICE_STATUS);

            TAF_MMA_StartTimer(TI_TAF_MMA_DELAY_REPORT_SERVICE_STATUS, (TAF_MMA_GetDelayReportServiceStatusCfgTimerLen()));

            return VOS_TRUE;
        }
    }

    if (NAS_MSCC_PIF_RF_AVAILABLE_RAT_TYPE_1X == pstRfAvail->enRatType)
    {
        TAF_SDC_Set1xRfAvailFlag(pstRfAvail->ucRfAvailFlg);
    }

    if (NAS_MSCC_PIF_RF_AVAILABLE_RAT_TYPE_HRPD == pstRfAvail->enRatType)
    {
        TAF_SDC_SetHrpdRfAvailFlag(pstRfAvail->ucRfAvailFlg);
    }
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-16, end */

    return VOS_TRUE;
}



/* Modified by h00313353 for Iteration 13, 2015-4-10, begin */
TAF_MMA_RAT_TYPE_ENUM_UINT8 TAF_MMA_ConvertSdcRatToAppRatType(
                        TAF_SDC_SYS_MODE_ENUM_UINT8 enSdcRatType
                        )
{
    switch (enSdcRatType)
    {
        case TAF_SDC_SYS_MODE_GSM:
             return TAF_MMA_RAT_GSM;

        case TAF_SDC_SYS_MODE_WCDMA:
             return TAF_MMA_RAT_WCDMA;

        case TAF_SDC_SYS_MODE_LTE:
             return TAF_MMA_RAT_LTE;

        default:
            MMA_WARNINGLOG("TAF_MMA_ConvertSdcRatToAppRatType():WORNING:The input is invalid.");
            return TAF_MMA_RAT_BUTT;
    }
}
/* Modified by h00313353 for Iteration 13, 2015-4-10, end */


TAF_MMA_SERVICE_DOMAIN_ENUM_UINT8 TAF_MMA_ConvertMsccDomainToMmaDomainType(
                        NAS_MSCC_PIF_SRVDOMAIN_ENUM_UINT32      enMsccDomainType
)
{
    switch (enMsccDomainType)
    {
        /* Modified by h00313353 for Iteration 13, 2015-4-10, begin */
        case NAS_MSCC_PIF_SRVDOMAIN_NO_DOMAIN:
        case NAS_MSCC_PIF_SRVDOMAIN_NOT_REG_PLMN_SEARCHING:
             return TAF_MMA_SERVICE_DOMAIN_NULL;

        case NAS_MSCC_PIF_SRVDOMAIN_CS:
             return TAF_MMA_SERVICE_DOMAIN_CS;

        case NAS_MSCC_PIF_SRVDOMAIN_PS:
             return TAF_MMA_SERVICE_DOMAIN_PS;

        case NAS_MSCC_PIF_SRVDOMAIN_CS_PS:
             return TAF_MMA_SERVICE_DOMAIN_CS_PS;

        case NAS_MSCC_PIF_SRVDOMAIN_NO_CHANGE:
             return TAF_MMA_SERVICE_DOMAIN_NOCHANGE;
        /* Modified by h00313353 for Iteration 13, 2015-4-10, end */
        default:
            MMA_WARNINGLOG("TAF_MMA_ConvertMsccDomainToMmaDomainType():WORNING:The input is invalid.");
            return TAF_MMA_SERVICE_DOMAIN_NULL;
    }
}

VOS_VOID TAF_MMA_SndRegRejInfoInd(
    MSCC_MMA_REG_RESULT_IND_STRU        *pstRejInfoInd
)
{
    /* 将MSCC的数据类型转换为TAF的数据类型，进行上报 */
    TAF_PH_REG_REJ_INFO_STRU            stTafRejInfo;
    TAF_SDC_SYS_INFO_STRU              *pstSysInfo = VOS_NULL_PTR;

    /* 填充上报事件 */
    pstSysInfo                = TAF_SDC_GetSysInfo();

    stTafRejInfo.enRat        = TAF_MMA_ConvertSdcRatToAppRatType(pstSysInfo->enSysMode);
    stTafRejInfo.stPlmnId.Mcc = pstSysInfo->st3gppSysInfo.stPlmnId.ulMcc;
    stTafRejInfo.stPlmnId.Mnc = pstSysInfo->st3gppSysInfo.stPlmnId.ulMnc;
    /* Modified by h00313353 for Iteration 13, 2015-4-9, begin */
    stTafRejInfo.enSrvDomain  = TAF_MMA_ConvertMsccDomainToMmaDomainType(pstRejInfoInd->enCnDomainId);
    /* Modified by h00313353 for Iteration 13, 2015-4-9, end */
    stTafRejInfo.ulRejCause   = pstRejInfoInd->enRejCause;

    /* 拒绝类型为注册被拒 */
    stTafRejInfo.ucRejType    = NAS_MSCC_PIF_REJ_TYPE_REG;

    stTafRejInfo.aucReserved[0]       = 0;
    /* Modified by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-22, begin */
    stTafRejInfo.ucOriginalRejCause   = pstRejInfoInd->ucOriginalRejCause;
    /* Modified by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-22, end */
    stTafRejInfo.ulCellId             = pstSysInfo->st3gppSysInfo.ulCellId;
    stTafRejInfo.ucRac                = pstSysInfo->st3gppSysInfo.ucRac;
    stTafRejInfo.usLac                = pstSysInfo->st3gppSysInfo.usLac;

    /* 将PLMN ID转为BCD格式 */
    MMA_PlmnId2Bcd(&stTafRejInfo.stPlmnId);

    if (VOS_TRUE == TAF_MMA_IsRegRejChangeNeedRpt())
    {
        TAF_MMA_SndRegRejInd(&stTafRejInfo);
    }

    return;
}


VOS_UINT32 TAF_MMA_RcvMsccRegResultInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_REG_RESULT_IND_STRU        *pstRegResultInd   = VOS_NULL_PTR;
    TAF_SDC_PLMN_ID_STRU                *pstCurrPlmnId = VOS_NULL_PTR;

    pstRegResultInd     = (MSCC_MMA_REG_RESULT_IND_STRU *)pstMsg;
    pstCurrPlmnId       = TAF_SDC_GetCurrCampPlmnId();

    /* 靠服务状态通知，MMC先报REG_RESULT_IND，再报SERVCIE_STATUS_IND，此时通知IMSA的服务状态不准确 */

    /* 为了尽可能使用有效的地理位置信息，注册成功后，需要更新国家码和时间戳 */
    if (VOS_TRUE == pstRegResultInd->ucRegRslt)
    {
        TAF_MMA_UpdateGeoInfo(pstCurrPlmnId);
    }

    /* 注册失败时且需要上报拒绝原因值 */
    if ((VOS_FALSE == pstRegResultInd->ucRegRslt)
     && (VOS_TRUE  == pstRegResultInd->ucReportCauseCtrl))
    {
        TAF_MMA_SndRegRejInfoInd(pstRegResultInd);
    }


    return VOS_TRUE;
}


#if (FEATURE_ON == FEATURE_IMS)
VOS_UINT32 TAF_MMA_RcvMsccImsVoiceCapInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_IMS_VOICE_CAP_IND_STRU *pstImsVoiceCap = VOS_NULL_PTR;

    pstImsVoiceCap = (MSCC_MMA_IMS_VOICE_CAP_IND_STRU *)pstMsg;


    /* 通知SPM当前IMS VOICE是否可用 */
    TAF_MMA_SndSpmImsVoiceCapInd(pstImsVoiceCap->ucAvail);

    /* Added by w00316404 for NEXT B26 Project, 2015-6-17, begin */
    /* 通知MTC当前IMS VOICE是否可用 */
    TAF_MMA_SndMtcImsVoiceCapInd(pstImsVoiceCap->ucAvail);
    /* Added by w00316404 for NEXT B26 Project, 2015-6-17, end */

    return VOS_TRUE;
}

/* 删除定时器处理逻辑 */
#endif


VOS_UINT32 TAF_MMA_RcvMsccUsimAuthFailInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* 将MMC的数据类型转换为TAF的数据类型，进行上报 */
    TAF_PH_REG_REJ_INFO_STRU            stTafRejInfo;
    TAF_SDC_SYS_INFO_STRU              *pstSysInfo = VOS_NULL_PTR;
    MSCC_MMA_USIM_AUTH_FAIL_IND_STRU    *pstAuthFailInd  = VOS_NULL_PTR;

    pstAuthFailInd            = (MSCC_MMA_USIM_AUTH_FAIL_IND_STRU *)pstMsg;

    /* 填充上报事件 */
    pstSysInfo                = TAF_SDC_GetSysInfo();
    stTafRejInfo.enRat        = TAF_MMA_ConvertSdcRatToAppRatType(pstSysInfo->enSysMode);
    stTafRejInfo.stPlmnId.Mcc = pstSysInfo->st3gppSysInfo.stPlmnId.ulMcc;
    stTafRejInfo.stPlmnId.Mnc = pstSysInfo->st3gppSysInfo.stPlmnId.ulMnc;
    /* Modified by h00313353 for Iteration 13, 2015-4-9, begin */
    stTafRejInfo.enSrvDomain  = TAF_MMA_ConvertMsccDomainToMmaDomainType(pstAuthFailInd->enCnDomainId);
    /* Modified by h00313353 for Iteration 13, 2015-4-9, end */

    /* 为了和网络拒绝原因值区分，usim鉴权失败原因值加上一个偏移位 */
    stTafRejInfo.ulRejCause   = pstAuthFailInd->enRejCause + (1 << 16);

    stTafRejInfo.ucRejType   = NAS_MSCC_PIF_REJ_TYPE_USIM_AUTH;

    stTafRejInfo.aucReserved[0]       = 0;
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, begin */
    stTafRejInfo.ucOriginalRejCause   = (VOS_UINT8)NAS_MML_REG_FAIL_CAUSE_NULL;
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, end */
    stTafRejInfo.ulCellId             = pstSysInfo->st3gppSysInfo.ulCellId;
    stTafRejInfo.ucRac                = pstSysInfo->st3gppSysInfo.ucRac;
    stTafRejInfo.usLac                = pstSysInfo->st3gppSysInfo.usLac;

    /* 将PLMN ID转为BCD格式 */
    MMA_PlmnId2Bcd(&stTafRejInfo.stPlmnId);

    if (VOS_TRUE == TAF_MMA_IsRegRejChangeNeedRpt())
    {
        TAF_MMA_SndRegRejInd(&stTafRejInfo);
    }


    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvMsccCsServiceConnStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_CS_SERVICE_CONN_STATUS_IND_STRU                *pstCsServiceConnStatusInd = VOS_NULL_PTR;
#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    TAF_MTC_SRV_CONN_STATE_INFO_STRU                        stMmaMtcConnStInfo;
    TAF_CTRL_STRU                                           stCtrl;
#endif

    /* 初始化 */
#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    PS_MEM_SET(&stCtrl, 0x0,sizeof(stCtrl));
    PS_MEM_SET(&stMmaMtcConnStInfo, 0x0,sizeof(stMmaMtcConnStInfo));
#endif

    pstCsServiceConnStatusInd = (MSCC_MMA_CS_SERVICE_CONN_STATUS_IND_STRU *)pstMsg;

    TAF_SDC_SetCsServiceConnStatusFlag(pstCsServiceConnStatusInd->ucCsServiceConnStatusFlag);

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    stMmaMtcConnStInfo.bitOpCsSrv           = VOS_TRUE;
    stMmaMtcConnStInfo.enCsSrvConnState     = pstCsServiceConnStatusInd->ucCsServiceConnStatusFlag;

    /* 填写CTL头 */
    TAF_API_CTRL_HEADER(&stCtrl, WUEPS_PID_MMA, 0, 0);

    /* 将连接状态报到MTC */
    MTC_SetModemServiceConnState(&stCtrl, &stMmaMtcConnStInfo);
#endif
#if (FEATURE_ON == FEATURE_BASTET)
    if(VOS_TRUE == TAF_SDC_GetBastetSupportFlag())
    {
        TAF_MMA_SndBastetCsServiceInd(pstCsServiceConnStatusInd->ucCsServiceConnStatusFlag);
    }
#endif
    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMsccServRejInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_SERV_REJ_IND_STRU          *pstServRejInd = VOS_NULL_PTR;
    TAF_PH_REG_REJ_INFO_STRU            stTafServRejInfo;
    TAF_SDC_SYS_INFO_STRU              *pstSysInfo = VOS_NULL_PTR;

    pstServRejInd = (MSCC_MMA_SERV_REJ_IND_STRU *) pstMsg;
    PS_MEM_SET(&stTafServRejInfo, 0, sizeof(stTafServRejInfo));

    /* 填充上报事件 */
    pstSysInfo                    = TAF_SDC_GetSysInfo();
    stTafServRejInfo.enRat        = TAF_MMA_ConvertSdcRatToAppRatType(pstSysInfo->enSysMode);
    stTafServRejInfo.stPlmnId.Mcc = pstSysInfo->st3gppSysInfo.stPlmnId.ulMcc;
    stTafServRejInfo.stPlmnId.Mnc = pstSysInfo->st3gppSysInfo.stPlmnId.ulMnc;
    /* Modified by h00313353 for Iteration 13, 2015-4-9, begin */
    stTafServRejInfo.enSrvDomain  = TAF_MMA_ConvertMsccDomainToMmaDomainType(pstServRejInd->enCnDomainId);
    /* Modified by h00313353 for Iteration 13, 2015-4-9, end */

    stTafServRejInfo.ulRejCause   = pstServRejInd->usRejCause;
    stTafServRejInfo.ucRejType    = NAS_MSCC_PIF_REJ_TYPE_SERV_REQ;

    /* 将PLMN ID转为BCD格式 */
    MMA_PlmnId2Bcd(&stTafServRejInfo.stPlmnId);

    stTafServRejInfo.aucReserved[0]       = 0;
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, begin */
    stTafServRejInfo.ucOriginalRejCause   = pstServRejInd->ucOriginalRejCause;
    /* Added by g00322017 for SEARCH_NW_CHR_OPT, 2015-8-17, end */
    stTafServRejInfo.ulCellId             = pstSysInfo->st3gppSysInfo.ulCellId;
    stTafServRejInfo.ucRac                = pstSysInfo->st3gppSysInfo.ucRac;
    stTafServRejInfo.usLac                = pstSysInfo->st3gppSysInfo.usLac;

    if (VOS_TRUE == TAF_MMA_IsRegRejChangeNeedRpt())
    {
        TAF_MMA_SndRegRejInd(&stTafServRejInfo);
    }

    return VOS_TRUE;
}



VOS_UINT32 TAF_MMA_RcvMsccAttachCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_ATTACH_CNF_STRU            *pstAttachCnf = VOS_NULL_PTR;

    pstAttachCnf = (MSCC_MMA_ATTACH_CNF_STRU*)pstMsg;

    if (NAS_MSCC_PIF_SRVDOMAIN_CS == pstAttachCnf->enCnDomainId)
    {
        TAF_SDC_SetCsAttachAllowFlg(VOS_TRUE);
    }
    else if (NAS_MSCC_PIF_SRVDOMAIN_PS == pstAttachCnf->enCnDomainId)
    {
        TAF_SDC_SetPsAttachAllowFlg(VOS_TRUE);
    }
    else if (NAS_MSCC_PIF_SRVDOMAIN_CS_PS == pstAttachCnf->enCnDomainId)
    {
        TAF_SDC_SetCsAttachAllowFlg(VOS_TRUE);
        TAF_SDC_SetPsAttachAllowFlg(VOS_TRUE);
    }
    else
    {
    }

    /* 进状态机处理 */
    return VOS_FALSE;
}
VOS_UINT32 TAF_MMA_RcvMsccDetachInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_DETACH_IND_STRU            *pstDetachInd = VOS_NULL_PTR;
    TAF_PH_REG_REJ_INFO_STRU            stTafServRejInfo;
    TAF_SDC_SYS_INFO_STRU              *pstSysInfo = VOS_NULL_PTR;

    pstDetachInd = (MSCC_MMA_DETACH_IND_STRU *) pstMsg;
    PS_MEM_SET(&stTafServRejInfo, 0, sizeof(stTafServRejInfo));

    if (NAS_MSCC_PIF_NETWORK_DETACH_TYPE_NOATTACH == pstDetachInd->enNetworkDetachType)
    {
        /* 填充上报事件 */
        pstSysInfo                    = TAF_SDC_GetSysInfo();
        stTafServRejInfo.enRat        = TAF_MMA_ConvertSdcRatToAppRatType(pstSysInfo->enSysMode);
        stTafServRejInfo.stPlmnId.Mcc = pstSysInfo->st3gppSysInfo.stPlmnId.ulMcc;
        stTafServRejInfo.stPlmnId.Mnc = pstSysInfo->st3gppSysInfo.stPlmnId.ulMnc;
        /* Modified by h00313353 for Iteration 13, 2015-4-9, begin */
        stTafServRejInfo.enSrvDomain  = TAF_MMA_ConvertMsccDomainToMmaDomainType(pstDetachInd->enCnDomainId);
        /* Modified by h00313353 for Iteration 13, 2015-4-9, end */

        stTafServRejInfo.ulRejCause   = pstDetachInd->ulDetachCause;
        stTafServRejInfo.ucRejType    = NAS_MSCC_PIF_REJ_TYPE_NETWORK_DETACH_IND;

        /* 将PLMN ID转为BCD格式 */
        MMA_PlmnId2Bcd(&stTafServRejInfo.stPlmnId);

        stTafServRejInfo.aucReserved[0]       = 0;
        stTafServRejInfo.ucOriginalRejCause   = pstDetachInd->ucOriginalDetachCause;
        stTafServRejInfo.ulCellId             = pstSysInfo->st3gppSysInfo.ulCellId;
        stTafServRejInfo.ucRac                = pstSysInfo->st3gppSysInfo.ucRac;
        stTafServRejInfo.usLac                = pstSysInfo->st3gppSysInfo.usLac;

        if (VOS_TRUE == TAF_MMA_IsRegRejChangeNeedRpt())
        {
            TAF_MMA_SndRegRejInd(&stTafServRejInfo);
        }
    }

    /* 进状态机处理 */
    return VOS_FALSE;
}



VOS_VOID TAF_MMA_ConvertMsccCauseToTafFormat(
    NAS_MSCC_PIF_NET_SCAN_CAUSE_ENUM_UINT8   enMsccCause,
    TAF_MMA_NET_SCAN_CAUSE_ENUM_UINT8  *pstMmaCause
)
{
    switch ( enMsccCause )
    {
        case NAS_MSCC_PIF_NET_SCAN_CAUSE_SIGNAL_EXIST :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_SIGNAL_EXIST;
            break;

        case NAS_MSCC_PIF_NET_SCAN_CAUSE_STATE_NOT_ALLOWED :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_STATE_NOT_ALLOWED;
            break;

        case NAS_MSCC_PIF_NET_SCAN_CAUSE_FREQ_LOCK :
           *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_FREQ_LOCK;
            break;

        case NAS_MSCC_PIF_NET_SCAN_CAUSE_PARA_ERROR :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_PARA_ERROR;
            break;

        case NAS_MSCC_PIF_NET_SCAN_CAUSE_CONFLICT :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_CONFLICT;
            break;

        case NAS_MSCC_PIF_NET_SCAN_CAUSE_SERVICE_EXIST :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_SERVICE_EXIST;
            break;

        case NAS_MSCC_PIF_NET_SCAN_CAUSE_NOT_CAMPED :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_NOT_CAMPED;
            break;

        case NAS_MSCC_PIF_NET_SCAN_CAUSE_RAT_TYPE_ERROR :
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_RAT_TYPE_ERROR;
            break;

        default:
            *pstMmaCause = TAF_MMA_NET_SCAN_CAUSE_BUTT;
            break;
    }

    return;
}


VOS_VOID TAF_MMA_ConvertMsccBandToTafFormat(
    NAS_MSCC_PIF_UE_SUPPORT_FREQ_BAND_STRU     *pstMsccBand,
    TAF_USER_SET_PREF_BAND64                   *pstUserBand
)
{
    TAF_MMA_USER_PREF_BAND_STRU        *pstUserBitBand      = VOS_NULL_PTR;
    GSM_BAND_STRU                      *pstGsmBitBand       = VOS_NULL_PTR;
    WCDMA_BAND_STRU                    *pstWcdmaBitBand     = VOS_NULL_PTR;
    TAF_MMA_USER_BAND_SET_UN            uUserSetBand;

    PS_MEM_SET(&uUserSetBand, 0x00, sizeof(TAF_MMA_USER_BAND_SET_UN));

    /* uUserSetBand将收到的MSCC格式频段保存成32位 */
    pstUserBitBand                        = &(uUserSetBand.BitBand);
    pstGsmBitBand                         = &(pstMsccBand->unGsmBand.stBitBand);
    pstWcdmaBitBand                       = &(pstMsccBand->unWcdmaBand.stBitBand);

    pstUserBitBand->BandGsm850            = pstGsmBitBand->BandGsm850;
    pstUserBitBand->BandGsmDcs1800        = pstGsmBitBand->BandGsm1800;
    pstUserBitBand->BandGsmEgsm900        = pstGsmBitBand->BandGsmE900;
    pstUserBitBand->BandGsmPcs1900        = pstGsmBitBand->BandGsm1900;
    pstUserBitBand->BandGsmPgsm900        = pstGsmBitBand->BandGsmP900;
    pstUserBitBand->BandGsmRgsm900        = pstGsmBitBand->BandGsmR900;

    pstUserBitBand->BandWcdma_I_Imt2k1    = pstWcdmaBitBand->BandWCDMA_I_2100;
    pstUserBitBand->BandWcdma_II_PCS_1900 = pstWcdmaBitBand->BandWCDMA_II_1900;
    pstUserBitBand->BandWcdma_III_1800    = pstWcdmaBitBand->BandWCDMA_III_1800;
    pstUserBitBand->BandWcdma_IV_1700     = pstWcdmaBitBand->BandWCDMA_IV_1700;
    pstUserBitBand->BandWcdma_V_850       = pstWcdmaBitBand->BandWCDMA_V_850;
    pstUserBitBand->BandWcdma_VI_800      = pstWcdmaBitBand->BandWCDMA_VI_800;
    pstUserBitBand->BandWcdma_VII_2600    = pstWcdmaBitBand->BandWCDMA_VII_2600;
    pstUserBitBand->BandWcdma_VIII_900    = pstWcdmaBitBand->BandWCDMA_VIII_900;
    pstUserBitBand->BandWcdma_IX_1700     = pstWcdmaBitBand->BandWCDMA_IX_J1700;
    pstUserBitBand->BandWCDMA_XIX_850     = pstWcdmaBitBand->BandWCDMA_XIX_850;
    pstUserBitBand->BandWcdma_XI_1500     = pstWcdmaBitBand->BandWCDMA_XI_1500;

    /* 将uUserSetBand转换成低32位和高32位结构 */
    pstUserBand->ulBandHigh = uUserSetBand.ulPrefBand & TAF_PH_BAND_NEED_CHANGE_TO_64BIT;
    pstUserBand->ulBandLow  = uUserSetBand.ulPrefBand & (~TAF_PH_BAND_NEED_CHANGE_TO_64BIT);

    return;
}




VOS_VOID TAF_MMA_ConvertMsccNetScanCnfToMmaFormat(
    MSCC_MMA_NET_SCAN_CNF_STRU          *pstMsg,
    TAF_MMA_NET_SCAN_CNF_STRU          *pstNetScanCnf
)
{
    VOS_UINT32                          i;

    /* 填充消息头 */
    /* Modified by k902809 for Iteration 11, 2015-3-25, begin */
    pstNetScanCnf->ulMsgName                = ID_TAF_MMA_NET_SCAN_CNF;
    pstNetScanCnf->usClientId               = gstMmaValue.stNetScan.usClientId;
    pstNetScanCnf->ucOpId                   = gstMmaValue.stNetScan.ucOpId;
    /* Modified by k902809 for Iteration 11, Iteration 11 2015-3-25, end */

    pstNetScanCnf->enResult    = pstMsg->enResult;
    pstNetScanCnf->ucFreqNum   = pstMsg->ucFreqNum;

    TAF_MMA_ConvertMsccCauseToTafFormat(pstMsg->enCause, &pstNetScanCnf->enCause);

    if ( TAF_MMA_NET_SCAN_MAX_FREQ_NUM < pstNetScanCnf->ucFreqNum )
    {
        pstNetScanCnf->ucFreqNum = TAF_MMA_NET_SCAN_MAX_FREQ_NUM;
    }

    for ( i = 0; i < pstNetScanCnf->ucFreqNum; i++ )
    {
        pstNetScanCnf->astNetScanInfo[i].usArfcn        = pstMsg->astNetScanInfo[i].usArfcn;
        pstNetScanCnf->astNetScanInfo[i].usC1           = pstMsg->astNetScanInfo[i].usC1;
        pstNetScanCnf->astNetScanInfo[i].usC2           = pstMsg->astNetScanInfo[i].usC2;
        pstNetScanCnf->astNetScanInfo[i].usLac          = pstMsg->astNetScanInfo[i].usLac;
        pstNetScanCnf->astNetScanInfo[i].ulMcc          = pstMsg->astNetScanInfo[i].ulMcc;
        pstNetScanCnf->astNetScanInfo[i].ulMnc          = pstMsg->astNetScanInfo[i].ulMnc;
        pstNetScanCnf->astNetScanInfo[i].usBsic         = pstMsg->astNetScanInfo[i].usBsic;
        pstNetScanCnf->astNetScanInfo[i].sRxlev         = pstMsg->astNetScanInfo[i].sRxlev;
        pstNetScanCnf->astNetScanInfo[i].sRssi          = pstMsg->astNetScanInfo[i].sRssi;
        pstNetScanCnf->astNetScanInfo[i].ulCellId       = pstMsg->astNetScanInfo[i].ulCellId;

        TAF_MMA_ConvertMsccBandToTafFormat(&pstMsg->astNetScanInfo[i].stBand,
                                          &pstNetScanCnf->astNetScanInfo[i].stBand);
    }


    return;
}
VOS_UINT32 TAF_MMA_RcvMsccNetScanCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_NET_SCAN_CNF_STRU           stNetScanCnf;
    MSCC_MMA_NET_SCAN_CNF_STRU          *pstMsccNetScanCnf = VOS_NULL_PTR;

    pstMsccNetScanCnf = (MSCC_MMA_NET_SCAN_CNF_STRU *)pstMsg;

    PS_MEM_SET(&stNetScanCnf, 0x00, sizeof(TAF_MMA_NET_SCAN_CNF_STRU));

    TAF_MMA_ConvertMsccNetScanCnfToMmaFormat(pstMsccNetScanCnf, &stNetScanCnf);

    if ( MMA_TIMER_RUN == g_stNetScanProtectTimer.ucTimerStatus )
    {
        TAF_MMA_StopNetScanProtectTimer();

        /* 调用电话管理上报函数 */
        /* Modified by k902809 for Iteration 11, 2015-3-25, begin */
        TAF_MMA_SndNetScanCnf(&stNetScanCnf);
        /* Modified by k902809 for Iteration 11, Iteration 11 2015-3-25, end */
    }
    else if ( MMA_TIMER_RUN == g_stAbortNetScanProtectTimer.ucTimerStatus )
    {
        TAF_MMA_StopAbortNetScanProtectTimer();

        /* 调用电话管理上报函数 */
        /* Modified by k902809 for Iteration 11, 2015-3-25, begin */
        TAF_MMA_SndNetScanCnf(&stNetScanCnf);
        /* Modified by k902809 for Iteration 11, Iteration 11 2015-3-25, end */
    }
    else
    {
        /* NETSCAN的保护定时器不在运行，则认为是异常回复 */
        MMA_WARNINGLOG("TAF_MMA_RcvMsccNetScanCnf: RCV UNEXPECTED NETSCAN CNF MSG!\r\n");
    }

    return VOS_TRUE;
}
VOS_VOID TAF_MMA_ConvertMsccAbortNetScanCnfToMmaFormat(
    MSCC_MMA_ABORT_NET_SCAN_CNF_STRU    *pstMsg,
    TAF_MMA_NET_SCAN_CNF_STRU           *pstNetScanCnf
)
{
    VOS_UINT32                          i;

    /* Modified by k902809 for Iteration 11, 2015-3-25, begin */
    pstNetScanCnf->ulMsgName                = ID_TAF_MMA_NET_SCAN_CNF;
    pstNetScanCnf->usClientId               = gstMmaValue.stNetScan.usClientId;
    pstNetScanCnf->ucOpId                   = gstMmaValue.stNetScan.ucOpId;
    /* Modified by k902809 for Iteration 11, Iteration 11 2015-3-25, end */

    pstNetScanCnf->enResult             = TAF_MMA_NET_SCAN_RESULT_SUCCESS;
    pstNetScanCnf->enCause              = TAF_MMA_NET_SCAN_CAUSE_BUTT;
    pstNetScanCnf->ucFreqNum            = pstMsg->ucFreqNum;

    if ( TAF_MMA_NET_SCAN_MAX_FREQ_NUM < pstNetScanCnf->ucFreqNum )
    {
        pstNetScanCnf->ucFreqNum = TAF_MMA_NET_SCAN_MAX_FREQ_NUM;
    }

    for ( i = 0; i < pstNetScanCnf->ucFreqNum; i++ )
    {
        pstNetScanCnf->astNetScanInfo[i].usArfcn        = pstMsg->astNetScanInfo[i].usArfcn;
        pstNetScanCnf->astNetScanInfo[i].usC1           = pstMsg->astNetScanInfo[i].usC1;
        pstNetScanCnf->astNetScanInfo[i].usC2           = pstMsg->astNetScanInfo[i].usC2;
        pstNetScanCnf->astNetScanInfo[i].usLac          = pstMsg->astNetScanInfo[i].usLac;
        pstNetScanCnf->astNetScanInfo[i].ulMcc          = pstMsg->astNetScanInfo[i].ulMcc;
        pstNetScanCnf->astNetScanInfo[i].ulMnc          = pstMsg->astNetScanInfo[i].ulMnc;
        pstNetScanCnf->astNetScanInfo[i].usBsic         = pstMsg->astNetScanInfo[i].usBsic;
        pstNetScanCnf->astNetScanInfo[i].sRxlev         = pstMsg->astNetScanInfo[i].sRxlev;
        pstNetScanCnf->astNetScanInfo[i].sRssi          = pstMsg->astNetScanInfo[i].sRssi;
        pstNetScanCnf->astNetScanInfo[i].ulCellId       = pstMsg->astNetScanInfo[i].ulCellId;

        TAF_MMA_ConvertMsccBandToTafFormat(&pstMsg->astNetScanInfo[i].stBand,
                                          &pstNetScanCnf->astNetScanInfo[i].stBand);
    }

    return;
}



VOS_UINT32 TAF_MMA_RcvMsccAbortNetScanCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_NET_SCAN_CNF_STRU           stNetScanCnf;
    MSCC_MMA_ABORT_NET_SCAN_CNF_STRU    *pstMsccAbortNetScanCnf = VOS_NULL_PTR;

    pstMsccAbortNetScanCnf = (MSCC_MMA_ABORT_NET_SCAN_CNF_STRU *)pstMsg;

    if ( MMA_TIMER_RUN != g_stAbortNetScanProtectTimer.ucTimerStatus )
    {
        MMA_WARNINGLOG("TAF_MMA_RcvMsccAbortNetScanCnf:abort timer not run!!!\r\n");

        return VOS_TRUE;
    }

    PS_MEM_SET(&stNetScanCnf, 0x00, sizeof(stNetScanCnf));

    /* 组装给AT模块回复的CNF结构 */
    TAF_MMA_ConvertMsccAbortNetScanCnfToMmaFormat(pstMsccAbortNetScanCnf, &stNetScanCnf);

    /* 停止NETSCAN的保护定时器 */
    TAF_MMA_StopAbortNetScanProtectTimer();

    /* 调用电话管理上报函数 */
    /* Modified by k902809 for Iteration 11, 2015-3-25, begin */
    TAF_MMA_SndNetScanCnf(&stNetScanCnf);
    /* Modified by k902809 for Iteration 11, Iteration 11 2015-3-25, end */

    return VOS_TRUE;
}


#if (FEATURE_MULTI_MODEM == FEATURE_ON)
VOS_UINT32 TAF_MMA_RcvMtcNcellInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* 转发给mscc */
    TAF_MMA_SndMsccNcellInfoInd(pstMsg);
    return VOS_TRUE;
}




VOS_UINT32 TAF_MMA_RcvMtcPsTransferInd_PreProc(
     VOS_UINT32                         ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* 转发给mmc */
    TAF_MMA_SndMsccPsTransferNotify(pstMsg);
    return VOS_TRUE;
}





VOS_UINT32 TAF_MMA_RcvMtcOtherModemInfoNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* 转发给mscc */
    TAF_MMA_SndMsccOtherModemInfoNotify(pstMsg);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMtcOtherModemDplmnNplmnInfoNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* 转发给mmc */
    TAF_MMA_SndMsccOtherModemDplmnNplmnInfoNotify(pstMsg);

    return VOS_TRUE;
}

#endif
VOS_UINT32 TAF_MMA_RcvMsccEplmnInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    MSCC_MMA_EPLMN_INFO_IND_STRU        *pstEplmnInfoIndMsg  = VOS_NULL_PTR;

    pstEplmnInfoIndMsg   = (MSCC_MMA_EPLMN_INFO_IND_STRU *)pstMsg;

    TAF_MMA_SndMtcEplmnInfoInd(pstEplmnInfoIndMsg);

#endif
    return VOS_TRUE;
}





VOS_UINT32 TAF_MMA_RcvMmaAcqReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_ACQ_REQ_STRU               *pstMmaAcqReqMsg    = VOS_NULL_PTR;
    MSCC_MMA_ACQ_CNF_STRU               stSndMsg;
    VOS_UINT8                           ucCtxIndex;
    VOS_UINT32                          ulRet;

    pstMmaAcqReqMsg = (TAF_MMA_ACQ_REQ_STRU *)pstMsg;

    PS_MEM_SET(&stSndMsg, 0, sizeof(MSCC_MMA_ACQ_CNF_STRU));

    /* 如果是关机状态，或者是正在开关机，则回复拒绝 */
    if ((TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
     || (STA_FSM_NULL           == g_StatusContext.ulFsmState)
     || (STA_FSM_STOP           == g_StatusContext.ulFsmState))
    {
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaAcqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_REJECT,
                          &stSndMsg);
        return VOS_TRUE;
    }


    /* 如果不是CL模式，且是CMMCA消息，则上报CMMCA拒绝 */
    if ((VOS_FALSE              == TAF_SDC_IsConfigCLInterWorkCOutside())
     && (CMMCA_CLIENT_ID        == pstMmaAcqReqMsg->stCtrl.usClientId))
    {
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaAcqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_REJECT,
                          &stSndMsg);
        return VOS_TRUE;
    }


    /* 如果卡无效，直接回失败 */
    if (VOS_FALSE == TAF_SDC_IsUsimStausValid())
    {
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaAcqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* 如果g_stTafMmaCtx.astMmaOperCtx里TAF_MMA_OPER_ACQ_REQ类型结构正在被使用 */
    if (VOS_TRUE == TAF_MMA_IsOperTypeUsed(TAF_MMA_OPER_ACQ_REQ))
    {
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaAcqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* 找不到未被使用的，回复失败 */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaAcqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    TAF_MMA_SetOperCtx(pstMmaAcqReqMsg->stCtrl,
                       TAF_MMA_OPER_ACQ_REQ, ucCtxIndex);

    /* 给MSCC发acq req消息 */
    ulRet = TAF_MMA_SndMsccAcqReq(&(pstMmaAcqReqMsg->stAcqPara));
    if (VOS_OK != ulRet)
    {
        TAF_MMA_ClearOperCtx(ucCtxIndex);
        return VOS_TRUE;
    }


    /* 启动保护定时器TI_TAF_MMA_WAIT_MSCC_ACQ_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_ACQ_CNF, TI_TAF_MMA_WAIT_MSCC_ACQ_CNF_LEN);

    return VOS_TRUE;
}


TAF_MMA_ATTACH_TYPE_ENUM_UINT8 TAF_MMA_ConvertAttachDomainToAttachType(
    TAF_MMA_SERVICE_DOMAIN_ENUM_UINT8                       enAttachDomain
)
{
    TAF_MMA_ATTACH_TYPE_ENUM_UINT8      enAttachType;

    /* Modified by h00313353 for Iteration 13, 2015-4-9, begin */
    switch ( enAttachDomain )
    {
        case TAF_MMA_SERVICE_DOMAIN_CS:
            enAttachType = TAF_MMA_ATTACH_TYPE_IMSI;
            break;

        case TAF_MMA_SERVICE_DOMAIN_PS:
            enAttachType = TAF_MMA_ATTACH_TYPE_GPRS;
            break;

        case TAF_MMA_SERVICE_DOMAIN_CS_PS:
        case TAF_MMA_SERVICE_DOMAIN_ANY:
            enAttachType = TAF_MMA_ATTACH_TYPE_GPRS_IMSI;
            break;

        case TAF_MMA_SERVICE_DOMAIN_NOCHANGE:
        default:
            enAttachType = TAF_MMA_ATTACH_TYPE_GPRS;
            break;
    }
    /* Modified by h00313353 for Iteration 13, 2015-4-9, end */

    return enAttachType;

}


VOS_UINT32 TAF_MMA_RcvMmaRegReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_REG_REQ_STRU               *pstMmaRegReqMsg = VOS_NULL_PTR;
    MSCC_MMA_REG_CNF_STRU               stSndMsg;
    VOS_UINT8                           ucCtxIndex;
    VOS_UINT32                          ulRet;
    TAF_MMA_ATTACH_TYPE_ENUM_UINT8                          enAttachType;
    VOS_UINT8                                               ucOpID;

    pstMmaRegReqMsg = (TAF_MMA_REG_REQ_STRU*)pstMsg;
    PS_MEM_SET(&stSndMsg, 0, sizeof(MSCC_MMA_REG_CNF_STRU));

    /* 如果是关机状态，或者是正在开关机，则回复拒绝 */
    if ((TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
     || (STA_FSM_NULL           == g_StatusContext.ulFsmState)
     || (STA_FSM_STOP           == g_StatusContext.ulFsmState))
    {
        TAF_MMA_SndRegCnf(&(pstMmaRegReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_REJECT,
                          &stSndMsg);
        return VOS_TRUE;
    }


    /* 如果不是CL模式，且是CMMCA消息，则上报CMMCA拒绝 */
    if ((VOS_FALSE              == TAF_SDC_IsConfigCLInterWorkCOutside())
     && (CMMCA_CLIENT_ID        == pstMmaRegReqMsg->stCtrl.usClientId))
    {
        TAF_MMA_SndRegCnf(&(pstMmaRegReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_REJECT,
                          &stSndMsg);
        return VOS_TRUE;
    }


    /* 如果卡无效，直接回失败 */
    if (VOS_FALSE == TAF_SDC_IsUsimStausValid())
    {
        TAF_MMA_SndRegCnf(&(pstMmaRegReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* 如果g_stTafMmaCtx.astMmaOperCtx里TAF_MMA_OPER_REG_REQ类型结构正在被使用 */
    if (VOS_TRUE == TAF_MMA_IsOperTypeUsed(TAF_MMA_OPER_REG_REQ))
    {
        TAF_MMA_SndRegCnf(&(pstMmaRegReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* 找不到未被使用的，回复失败  */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndRegCnf(&(pstMmaRegReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);
        return VOS_TRUE;
    }

    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    TAF_MMA_SetOperCtx(pstMmaRegReqMsg->stCtrl,
                       TAF_MMA_OPER_REG_REQ, ucCtxIndex);

    /* 给MSCC发reg req消息 */
    ulRet = TAF_MMA_SndMsccRegReq(&(pstMmaRegReqMsg->stRegPara));
    if (VOS_OK != ulRet)
    {
        TAF_MMA_ClearOperCtx(ucCtxIndex);
        return VOS_TRUE;
    }

    if (TAF_MMA_SERVICE_DOMAIN_NULL == pstMmaRegReqMsg->stRegPara.enAttachDomain)
    {
        TAF_MMA_SndRegCnf(&(pstMmaRegReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);

        return VOS_TRUE;
    }

    /* Modified by h00313353 for Iteration 13, 2015-4-9, begin */
    enAttachType = TAF_MMA_ConvertAttachDomainToAttachType(pstMmaRegReqMsg->stRegPara.enAttachDomain);
    /* Modified by h00313353 for Iteration 13, 2015-4-9, end */

    ucOpID = TAF_MMA_GetAttachOpId();

    (VOS_VOID)TAF_MMA_SndMsccAttachReq(ucOpID,
                                       (NAS_MSCC_PIF_ATTACH_TYPE_ENUM_UINT32)enAttachType,
                                       pstMmaRegReqMsg->stRegPara.enEpsAttachReason);

    TAF_MMA_SetAttachAllowFlg(enAttachType);

    /* 启动保护定时器TI_TAF_MMA_WAIT_MSCC_REG_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_REG_CNF, TI_TAF_MMA_WAIT_MSCC_REG_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvMmaPowerSaveReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_POWER_SAVE_REQ_STRU        *pstMmaPowerSaveReqMsg  = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;
    VOS_UINT32                          ulRet;

    pstMmaPowerSaveReqMsg  = (TAF_MMA_POWER_SAVE_REQ_STRU*)pstMsg;

    /* 如果是开关机过程中，直接回复拒绝 */
    if (TAF_MMA_FSM_PHONE_MODE == TAF_MMA_GetCurrFsmId())
    {
        TAF_MMA_SndPowerSaveCnf(&(pstMmaPowerSaveReqMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_REJECT);
        return VOS_TRUE;
    }


    /* 如果不是CL模式，且是CMMCA消息，则上报CMMCA拒绝 */
    if ((VOS_FALSE             == TAF_SDC_IsConfigCLInterWorkCOutside())
     && (CMMCA_CLIENT_ID       == pstMmaPowerSaveReqMsg->stCtrl.usClientId))
    {
        TAF_MMA_SndPowerSaveCnf(&(pstMmaPowerSaveReqMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_REJECT);
        return VOS_TRUE;
    }


    /* 如果已经关机，直接回复成功 */
    if ((STA_FSM_NULL    == g_StatusContext.ulFsmState)
     || (STA_FSM_STOP    == g_StatusContext.ulFsmState))
    {
        TAF_MMA_SndPowerSaveCnf(&(pstMmaPowerSaveReqMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_SUCCESS);
        return VOS_TRUE;
    }

    /* 如果g_stTafMmaCtx.astMmaOperCtx里TAF_MMA_OPER_POWER_SAVE_REQ类型结构正在被使用 */
    if (VOS_TRUE == TAF_MMA_IsOperTypeUsed(TAF_MMA_OPER_POWER_SAVE_REQ))
    {
        TAF_MMA_SndPowerSaveCnf(&(pstMmaPowerSaveReqMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_FAILURE);
        return VOS_TRUE;
    }

    /* 找不到未被使用的，回复失败  */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndPowerSaveCnf(&(pstMmaPowerSaveReqMsg->stCtrl),
                                TAF_MMA_APP_OPER_RESULT_FAILURE);
        return VOS_TRUE;
    }

    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    TAF_MMA_SetOperCtx(pstMmaPowerSaveReqMsg->stCtrl,
                       TAF_MMA_OPER_POWER_SAVE_REQ, ucCtxIndex);

    /* 给MSCC发power save req消息 */
    ulRet = TAF_MMA_SndMsccPowerSaveReq(&(pstMmaPowerSaveReqMsg->stPowerSavePara));
    if (VOS_OK != ulRet)
    {
        TAF_MMA_ClearOperCtx(ucCtxIndex);

        return VOS_TRUE;
    }

    /* 启动保护定时器TI_TAF_MMA_WAIT_MSCC_POWER_SAVE_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_POWER_SAVE_CNF,
                       TI_TAF_MMA_WAIT_MSCC_POWER_SAVE_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvMsccAcqCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_ACQ_CNF_STRU               *pstMsccMmaAcqCnf = VOS_NULL_PTR;
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;
    TAF_MMA_APP_OPER_RESULT_ENUM_UINT32 enRslt;

    pstMsccMmaAcqCnf = (MSCC_MMA_ACQ_CNF_STRU *)pstMsg;
    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();
    enRslt          = TAF_MMA_APP_OPER_RESULT_SUCCESS;

    if (NAS_MSCC_PIF_ACQ_RESULT_SUCCESS != pstMsccMmaAcqCnf->enAcqRslt)
    {
        enRslt = TAF_MMA_APP_OPER_RESULT_FAILURE;
    }

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_OPER_ACQ_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_ACQ_REQ, &ucCtxIndex))
    {
        /*  回复MMA_ACQ_CNF */
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                          enRslt,
                          pstMsccMmaAcqCnf);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    /* 停止保护定时器 */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MSCC_ACQ_CNF);

    return VOS_TRUE;
}

#if (FEATURE_ON == FEATURE_CL_INTERWORK)
VOS_UINT32 TAF_MMA_RcvMsccAcqInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_ACQ_IND_STRU               *pstMsccMmaAcqInd = VOS_NULL_PTR;
    TAF_MMA_APP_OPER_RESULT_ENUM_UINT32 enRslt;

    pstMsccMmaAcqInd = (MSCC_MMA_ACQ_IND_STRU *)pstMsg;
    enRslt          = TAF_MMA_APP_OPER_RESULT_SUCCESS;

    if (NAS_MSCC_PIF_ACQ_RESULT_SUCCESS != pstMsccMmaAcqInd->enAcqRslt)
    {
        enRslt = TAF_MMA_APP_OPER_RESULT_FAILURE;
    }

    /* 回复MMA_ACQ_IND */
    TAF_MMA_SndAcqInd(enRslt, pstMsccMmaAcqInd);

    return VOS_TRUE;
}
#endif

/* Modified by l00301449 for CDMA Iteration 15, 2015-5-15, begin */




VOS_VOID TAF_MMA_SndEndSessionNotifyAccordingSrvType(
    TAF_SDC_TRIG_PLMN_SEARCH_SRV_TYPE_UNION                 *pstSrvType
)
{
    NAS_MSCC_PIF_SRV_ACQ_RAT_LIST_STRU                       stMsccRatList;
    TAF_MMA_SRV_ACQ_RAT_LIST_STRU                           *pstMmaRatListAddr = VOS_NULL_PTR;
    VOS_UINT32                                               i;

    pstMmaRatListAddr = TAF_MMA_GetSrvAcqRatListAddr();

    stMsccRatList.ucRatNum = pstMmaRatListAddr->ucRatNum;

    for (i = 0; i < PS_MIN(pstMmaRatListAddr->ucRatNum, TAF_MMA_SRV_ACQ_RAT_NUM_MAX); i++)
    {
        stMsccRatList.aenRatType[i] = pstMmaRatListAddr->aenRatType[i];
    }

    /* 根据之前的记录的业务类型发送end session释放资源,
          存在多业务并发需要发多条end session消息情况  */
    if (VOS_TRUE == pstSrvType->stTrigPlmnSearchSrvType.usCsMoNormalCall)
    {
        TAF_MMA_SndMsccEndSessionNotify(NAS_MSCC_PIF_SRV_TYPE_CS_MO_NORMAL_CALL, &stMsccRatList);
        pstSrvType->stTrigPlmnSearchSrvType.usCsMoNormalCall = VOS_FALSE;
    }

    if (VOS_TRUE == pstSrvType->stTrigPlmnSearchSrvType.usCsMoEmgCall)
    {
        TAF_MMA_SndMsccEndSessionNotify(NAS_MSCC_PIF_SRV_TYPE_CS_MO_EMERGENCY_CALL, &stMsccRatList);
        pstSrvType->stTrigPlmnSearchSrvType.usCsMoEmgCall = VOS_FALSE;
    }

    if (VOS_TRUE == pstSrvType->stTrigPlmnSearchSrvType.usCsMoSs)
    {
        TAF_MMA_SndMsccEndSessionNotify(NAS_MSCC_PIF_SRV_TYPE_CS_MO_SS, &stMsccRatList);
        pstSrvType->stTrigPlmnSearchSrvType.usCsMoSs = VOS_FALSE;
    }

    if (VOS_TRUE == pstSrvType->stTrigPlmnSearchSrvType.usCsMoSms)
    {
        TAF_MMA_SndMsccEndSessionNotify(NAS_MSCC_PIF_SRV_TYPE_CS_MO_SMS, &stMsccRatList);
        pstSrvType->stTrigPlmnSearchSrvType.usCsMoSms = VOS_FALSE;
    }

    if (VOS_TRUE == pstSrvType->stTrigPlmnSearchSrvType.usPsBackGroundCall)
    {
        TAF_MMA_SndMsccEndSessionNotify(NAS_MSCC_PIF_SRV_TYPE_PS_BACKGROUND_CALL, &stMsccRatList);
        pstSrvType->stTrigPlmnSearchSrvType.usPsBackGroundCall = VOS_FALSE;
    }

    if (VOS_TRUE == pstSrvType->stTrigPlmnSearchSrvType.usPsConversatCall)
    {
        TAF_MMA_SndMsccEndSessionNotify(NAS_MSCC_PIF_SRV_TYPE_PS_CONVERSAT_CALL, &stMsccRatList);
        pstSrvType->stTrigPlmnSearchSrvType.usPsConversatCall = VOS_FALSE;
    }

    if (VOS_TRUE == pstSrvType->stTrigPlmnSearchSrvType.usPsInteractCall)
    {
        TAF_MMA_SndMsccEndSessionNotify(NAS_MSCC_PIF_SRV_TYPE_PS_INTERACT_CALL, &stMsccRatList);
        pstSrvType->stTrigPlmnSearchSrvType.usPsInteractCall = VOS_FALSE;
    }

    if (VOS_TRUE == pstSrvType->stTrigPlmnSearchSrvType.usPsStreamCall)
    {
        TAF_MMA_SndMsccEndSessionNotify(NAS_MSCC_PIF_SRV_TYPE_PS_STREAM_CALL, &stMsccRatList);
        pstSrvType->stTrigPlmnSearchSrvType.usPsStreamCall = VOS_FALSE;
    }

    if (VOS_TRUE == pstSrvType->stTrigPlmnSearchSrvType.usPsSubscribTrafficCall)
    {
        TAF_MMA_SndMsccEndSessionNotify(NAS_MSCC_PIF_SRV_TYPE_PS_SUBSCRIB_TRAFFIC_CALL, &stMsccRatList);
        pstSrvType->stTrigPlmnSearchSrvType.usPsSubscribTrafficCall = VOS_FALSE;
    }

    return;
}
/* Modified by l00301449 for CDMA Iteration 15, 2015-5-16, end */

VOS_VOID TAF_SPM_ConvertSrvTypeToMscc(
    TAF_MMA_SRV_TYPE_ENUM_UINT8              enMmaSrvType,
    NAS_MSCC_PIF_SRV_TYPE_ENUM_UINT8        *pucMsccSrvType
)
{
    switch (enMmaSrvType)
    {
        case TAF_MMA_SRV_TYPE_CS_MO_NORMAL_CALL:
            *pucMsccSrvType = NAS_MSCC_PIF_SRV_TYPE_CS_MO_NORMAL_CALL;
            break;

        case TAF_MMA_SRV_TYPE_CS_MO_SS:
            *pucMsccSrvType = NAS_MSCC_PIF_SRV_TYPE_CS_MO_SS;
            break;

        case TAF_MMA_SRV_TYPE_CS_MO_SMS:
            *pucMsccSrvType = NAS_MSCC_PIF_SRV_TYPE_CS_MO_SMS;
            break;

        case TAF_MMA_SRV_TYPE_CS_MO_EMERGENCY_CALL:
            *pucMsccSrvType = NAS_MSCC_PIF_SRV_TYPE_CS_MO_EMERGENCY_CALL;
            break;

        case TAF_MMA_SRV_TYPE_PS_CONVERSAT_CALL:
            *pucMsccSrvType = NAS_MSCC_PIF_SRV_TYPE_PS_CONVERSAT_CALL;
            break;

        case TAF_MMA_SRV_TYPE_PS_STREAM_CALL:
            *pucMsccSrvType = NAS_MSCC_PIF_SRV_TYPE_PS_STREAM_CALL;
            break;

        case TAF_MMA_SRV_TYPE_PS_INTERACT_CALL:
            *pucMsccSrvType = NAS_MSCC_PIF_SRV_TYPE_PS_INTERACT_CALL;
            break;

        case TAF_MMA_SRV_TYPE_PS_BACKGROUND_CALL:
            *pucMsccSrvType = NAS_MSCC_PIF_SRV_TYPE_PS_BACKGROUND_CALL;
            break;

        case TAF_MMA_SRV_TYPE_PS_SUBSCRIB_TRAFFIC_CALL:
            *pucMsccSrvType = NAS_MSCC_PIF_SRV_TYPE_PS_SUBSCRIB_TRAFFIC_CALL;
            break;

        default:
            *pucMsccSrvType = NAS_MSCC_PIF_SRV_TYPE_BUTT;
            break;
    }

    return;
}


VOS_VOID TAF_SDC_UpdateTrigPlmnSearchSrvType(
    TAF_MMA_SRV_TYPE_ENUM_UINT8         enSrvType
)
{
    TAF_SDC_TRIG_PLMN_SEARCH_SRV_TYPE_UNION                 *pstTrigPlmnSrcSrvType = VOS_NULL_PTR;

    pstTrigPlmnSrcSrvType = TAF_SDC_GetTrigPlmnSearchSrvType();

    switch (enSrvType)
    {
        case TAF_MMA_SRV_TYPE_CS_MO_NORMAL_CALL:
            pstTrigPlmnSrcSrvType->stTrigPlmnSearchSrvType.usCsMoNormalCall = VOS_TRUE;
            break;

        case TAF_MMA_SRV_TYPE_CS_MO_EMERGENCY_CALL:
            pstTrigPlmnSrcSrvType->stTrigPlmnSearchSrvType.usCsMoEmgCall = VOS_TRUE;
            break;

        case TAF_MMA_SRV_TYPE_CS_MO_SS:
            pstTrigPlmnSrcSrvType->stTrigPlmnSearchSrvType.usCsMoSs = VOS_TRUE;
            break;

        case TAF_MMA_SRV_TYPE_CS_MO_SMS:
            pstTrigPlmnSrcSrvType->stTrigPlmnSearchSrvType.usCsMoSms = VOS_TRUE;
            break;

        case TAF_MMA_SRV_TYPE_PS_CONVERSAT_CALL:
            pstTrigPlmnSrcSrvType->stTrigPlmnSearchSrvType.usPsConversatCall = VOS_TRUE;
            break;

        case TAF_MMA_SRV_TYPE_PS_STREAM_CALL:
            pstTrigPlmnSrcSrvType->stTrigPlmnSearchSrvType.usPsStreamCall = VOS_TRUE;
            break;

        case TAF_MMA_SRV_TYPE_PS_INTERACT_CALL:
            pstTrigPlmnSrcSrvType->stTrigPlmnSearchSrvType.usPsInteractCall = VOS_TRUE;
            break;

        case TAF_MMA_SRV_TYPE_PS_BACKGROUND_CALL:
            pstTrigPlmnSrcSrvType->stTrigPlmnSearchSrvType.usPsBackGroundCall = VOS_TRUE;
            break;

        case TAF_MMA_SRV_TYPE_PS_SUBSCRIB_TRAFFIC_CALL:
            pstTrigPlmnSrcSrvType->stTrigPlmnSearchSrvType.usPsSubscribTrafficCall = VOS_TRUE;
            break;

        default:
            break;
    }

    return;
}



VOS_UINT32 TAF_MMA_RcvTafSrvAcqReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_SRV_ACQ_REQ_STRU                               *pstTafSrvAcqReq = VOS_NULL_PTR;
    NAS_MSCC_PIF_SRV_TYPE_ENUM_UINT8                        enMmaMsccSrvType;
    TAF_MMA_TIMER_STATUS_ENUM_UINT8                         enWaitSrvAcqCnfTimerStatus;
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-16, begin */
    NAS_MSCC_PIF_SRV_ACQ_RAT_LIST_STRU                      stMsccRatList;
    VOS_UINT32                                              i;

    PS_MEM_SET(&stMsccRatList, 0, sizeof(NAS_MSCC_PIF_SRV_ACQ_RAT_LIST_STRU));

    pstTafSrvAcqReq            = (TAF_MMA_SRV_ACQ_REQ_STRU *)pstMsg;
    enWaitSrvAcqCnfTimerStatus = TAF_MMA_GetTimerStatus(TI_TAF_MMA_WAIT_MSCC_SRV_ACQ_CNF);

    if (TAF_MMA_SRV_TYPE_BUTT == pstTafSrvAcqReq->enSrvType)
    {
        TAF_WARNING_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvTafSrvAcqReq_PreProc:invalid para!");
        return VOS_TRUE;
    }

    /* 如果存在业务触发的搜网mma做记录，发送begin session请求 */
    TAF_SDC_UpdateTrigPlmnSearchSrvType(pstTafSrvAcqReq->enSrvType);

    TAF_SPM_ConvertSrvTypeToMscc(pstTafSrvAcqReq->enSrvType, &enMmaMsccSrvType);

    /* 如果存在业务触发的搜网mma做记录，发送begin session请求，不需要给mscc再发srv req请求 */
    if (TAF_MMA_TIMER_STATUS_RUNING == enWaitSrvAcqCnfTimerStatus)
    {
        return VOS_TRUE;
    }

    stMsccRatList.ucRatNum = pstTafSrvAcqReq->stRatList.ucRatNum;

    for (i = 0; i < PS_MIN(pstTafSrvAcqReq->stRatList.ucRatNum, TAF_MMA_SRV_ACQ_RAT_NUM_MAX); i++)
    {
        stMsccRatList.aenRatType[i] = pstTafSrvAcqReq->stRatList.aenRatType[i];
    }

    TAF_MMA_SndMsccBeginSessionNotify(enMmaMsccSrvType, &stMsccRatList);

    /* 通知mscc业务类型触发搜网 */
    TAF_MMA_SndMsccSrvAcqReq(enMmaMsccSrvType, &stMsccRatList);

    TAF_MMA_UpdateSrvAcqRatList(&(pstTafSrvAcqReq->stRatList));

    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-16, end */

    /* 启动保护定时器 */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_SRV_ACQ_CNF, TI_TAF_MMA_WAIT_MSCC_SRV_ACQ_CNF_LEN);

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvMsccSrvAcqCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_SRV_ACQ_CNF_STRU           *pstSrvAcqCnf = VOS_NULL_PTR;

    pstSrvAcqCnf = (MSCC_MMA_SRV_ACQ_CNF_STRU *)pstMsg;

    if (TAF_MMA_TIMER_STATUS_RUNING == TAF_MMA_GetTimerStatus(TI_TAF_MMA_WAIT_MSCC_SRV_ACQ_CNF))
    {
        /* 停止保护定时器 */
        TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MSCC_SRV_ACQ_CNF);

        /* 根据之前的记录的业务类型发送end session释放资源,
           存在多业务并发需要发多条end session消息情况  */
        TAF_MMA_SndEndSessionNotifyAccordingSrvType(TAF_SDC_GetTrigPlmnSearchSrvType());

        /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, begin */
        /* 清除保存的SRV ACQ RAT LIST */
        TAF_MMA_ClearSrvAcqRatList();
        /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, end */

        TAF_MMA_SndSrvAcqCnf(pstSrvAcqCnf->enResult);
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvTiWaitMsccSrvAcqCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* 根据之前的记录的业务类型发送end session释放资源,
       存在多业务并发需要发多条end session消息情况  */
    TAF_MMA_SndEndSessionNotifyAccordingSrvType(TAF_SDC_GetTrigPlmnSearchSrvType());

    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, begin */
    /* 清除保存的SRV ACQ RAT LIST */
    TAF_MMA_ClearSrvAcqRatList();
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, end */

    /* 给spm回复 ID_TAF_MMA_SRV_ACQ_CNF搜网失败消息 */
    TAF_MMA_SndSrvAcqCnf(TAF_MMA_SRV_ACQ_RESULT_FAIL);
    return VOS_TRUE;
}

/* Modified by l00301449 for CDMA Iteration 15, 2015-5-15, end */

VOS_UINT32 TAF_MMA_RcvAtPlmnList_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
#if (FEATURE_ON == FEATURE_DSDS)
    TAF_MMA_TIMER_STATUS_ENUM_UINT8     enWaitSrvAcqCnfTimerStatus;
    TAF_MMA_PLMN_LIST_CNF_PARA_STRU    *pstPlmnListCnfPara = VOS_NULL_PTR;
    TAF_MMA_PLMN_LIST_REQ_STRU         *pstRcvMsg          = VOS_NULL_PTR;

    pstRcvMsg = (TAF_MMA_PLMN_LIST_REQ_STRU *)pstMsg;

    pstPlmnListCnfPara = (TAF_MMA_PLMN_LIST_CNF_PARA_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA, sizeof(TAF_MMA_PLMN_LIST_CNF_PARA_STRU));
    if (NULL == pstPlmnListCnfPara)
    {
        return VOS_TRUE;
    }

    PS_MEM_SET(pstPlmnListCnfPara, 0x0, sizeof(TAF_MMA_PLMN_LIST_CNF_PARA_STRU));

    enWaitSrvAcqCnfTimerStatus = TAF_MMA_GetTimerStatus(TI_TAF_MMA_WAIT_MSCC_SRV_ACQ_CNF);

    /* 如果当前no rf状态且存在业务触发的搜网，直接回复失败 */
    if (TAF_MMA_TIMER_STATUS_RUNING == enWaitSrvAcqCnfTimerStatus)
    {
        /* 直接回复plmn list失败 */
        pstPlmnListCnfPara->ucOpError    = VOS_TRUE;
        pstPlmnListCnfPara->enPhoneError = TAF_ERR_NO_RF;
        TAF_MMA_SndPlmnListCnf(&(pstRcvMsg->stCtrl), pstPlmnListCnfPara);

        PS_MEM_FREE(WUEPS_PID_MMA, pstPlmnListCnfPara);

        return VOS_TRUE;
    }

    PS_MEM_FREE(WUEPS_PID_MMA, pstPlmnListCnfPara);
#endif

    return VOS_FALSE;
}


VOS_UINT32 TAF_MMA_RcvSysCfgSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Modified by l00301449 for Iteration 13, 2015-04-07, begin */
    TAF_MMA_SYS_CFG_REQ_STRU           *pstSysCfgReq = VOS_NULL_PTR;
#if (FEATURE_ON == FEATURE_DSDS)
    TAF_MMA_TIMER_STATUS_ENUM_UINT8     enWaitSrvAcqCnfTimerStatus;
#endif
    VOS_UINT8                           ucImsCallFlag;
    VOS_UINT32                          ulOldRatExistedFlg;
    VOS_UINT32                          ulNewRatExistedFlg;
    VOS_UINT8                           ucCsCallFlag;

    pstSysCfgReq               = (TAF_MMA_SYS_CFG_REQ_STRU *)pstMsg;

#if (FEATURE_ON == FEATURE_DSDS)
    enWaitSrvAcqCnfTimerStatus = TAF_MMA_GetTimerStatus(TI_TAF_MMA_WAIT_MSCC_SRV_ACQ_CNF);

    /* 如果当前no rf状态且存在业务触发的搜网，直接回复失败 */
    if (TAF_MMA_TIMER_STATUS_RUNING == enWaitSrvAcqCnfTimerStatus)
    {
        TAF_MMA_SndSysCfgSetCnf(&pstSysCfgReq->stCtrl, TAF_ERR_NO_RF);

        return VOS_TRUE;
    }
#endif

    /* 开机状态，需要先判断通道 */
    if ((STA_FSM_NULL != g_StatusContext.ulFsmState)
     && (STA_FSM_STOP != g_StatusContext.ulFsmState))
    {
        /* C+L模式，此时是AT触发了SYSCFG设置，则回复失败,进行保护  */
        if ((VOS_TRUE        == TAF_MMA_IsPowerOnCLInterWork())
         && (CMMCA_CLIENT_ID != pstSysCfgReq->stCtrl.usClientId))
        {
            TAF_MMA_SndSysCfgSetCnf(&pstSysCfgReq->stCtrl, TAF_ERR_ERROR);

            return VOS_TRUE;
        }

        /* 不是C+L模式，此时是CMMCA触发了SYSCFG设置，则回复失败,进行保护  */
        if ((VOS_FALSE       == TAF_MMA_IsPowerOnCLInterWork())
         && (CMMCA_CLIENT_ID == pstSysCfgReq->stCtrl.usClientId))
        {
            TAF_MMA_SndSysCfgSetCnf(&pstSysCfgReq->stCtrl, TAF_ERR_ERROR);

            return VOS_TRUE;
        }
    }

    /* 判断用户设置的参数是否有效 */
    if (VOS_FALSE == TAF_MMA_IsUserSettedSysCfgParaValid(&(pstSysCfgReq->stSysCfgPara)))
    {
        TAF_MMA_SndSysCfgSetCnf(&pstSysCfgReq->stCtrl, TAF_ERR_PARA_ERROR);

        return VOS_TRUE;
    }

    /* 如果CS业务存在，拒绝系统配置请求 */
    ucImsCallFlag = TAF_SDC_GetImsCallExistFlg();
    ucCsCallFlag  = TAF_SDC_GetCsCallExistFlg();
    ulNewRatExistedFlg = TAF_MMA_SYS_CFG_RAT_NONE_SET;
    ulOldRatExistedFlg = TAF_MMA_SYS_CFG_RAT_NONE_SET;
    (VOS_VOID)TAF_MMA_GetSysCfgRatFlag(&(TAF_MMA_GetLastSyscfgSetAddr()->stRatPrioList), &ulOldRatExistedFlg);
    (VOS_VOID)TAF_MMA_GetSysCfgRatFlag(&pstSysCfgReq->stSysCfgPara.stRatOrder, &ulNewRatExistedFlg);
    if (((VOS_TRUE == ucCsCallFlag) && (ulNewRatExistedFlg & TAF_MMA_SYS_CFG_RAT_C_SET))
     || ((VOS_TRUE == ucCsCallFlag) && (ulOldRatExistedFlg & TAF_MMA_SYS_CFG_RAT_C_SET))
     || (VOS_TRUE == ucImsCallFlag))
    {
        TAF_MMA_SndSysCfgSetCnf(&pstSysCfgReq->stCtrl, TAF_ERR_SYSCFG_CS_SERV_EXIST);

        return VOS_TRUE;
    }

    return VOS_FALSE;
    /* Modified by l00301449 for Iteration 13, 2015-04-07, end */
}



VOS_UINT32 TAF_MMA_RcvMsccRegCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_REG_CNF_STRU               *pstMsccMmaRegCnf = VOS_NULL_PTR;
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;
    TAF_MMA_APP_OPER_RESULT_ENUM_UINT32 enRslt;

    pstMsccMmaRegCnf = (MSCC_MMA_REG_CNF_STRU *)pstMsg;
    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    switch (pstMsccMmaRegCnf->enRegRslt)
    {
        case NAS_MSCC_PIF_REG_RESULT_SUCCESS:
            enRslt = TAF_MMA_APP_OPER_RESULT_SUCCESS;
            break;

        case NAS_MSCC_PIF_REG_RESULT_ACQ_FAILURE:
            enRslt = TAF_MMA_APP_OPER_RESULT_ACQUIRED_FAIL;
            break;

        case NAS_MSCC_PIF_REG_RESULT_REG_FAILURE:
            enRslt = TAF_MMA_APP_OPER_RESULT_FAILURE;
            break;

        default:
            enRslt = TAF_MMA_APP_OPER_RESULT_ACQUIRED_FAIL;
            break;
    }

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_OPER_REG_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_REG_REQ, &ucCtxIndex))
    {

        /*  回复MMA_REG_CNF */
        TAF_MMA_SndRegCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                          enRslt,
                          pstMsccMmaRegCnf);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    /* 停止保护定时器 */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MSCC_REG_CNF);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMsccPowerSaveCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    TAF_MMA_CTRL_STRU                   stCtrlInfo;
    VOS_UINT8                           ucCtxIndex;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_OPER_ACQ_REQ对应的记录项 */
    if (VOS_TRUE != TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_POWER_SAVE_REQ, &ucCtxIndex))
    {
        MMA_WARNINGLOG("TAF_MMA_RcvMsccPowerSaveCnf_PreProc:NOT FIND TAF_MMA_OPER_POWER_SAVE_REQ!");

        TAF_MMA_BuildMmaCtrlInfo(WUEPS_PID_CMMCA, CMMCA_CLIENT_ID, 0, &stCtrlInfo);
        TAF_MMA_SndPowerSaveCnf(&stCtrlInfo,
                                TAF_MMA_APP_OPER_RESULT_SUCCESS);

        return VOS_TRUE;
    }

    /* 回复MMA_POWER_SAVE_CNF */
    TAF_MMA_SndPowerSaveCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                            TAF_MMA_APP_OPER_RESULT_SUCCESS);

    TAF_MMA_ClearOperCtx(ucCtxIndex);

    /* 停止保护定时器 */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MSCC_POWER_SAVE_CNF);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvTiWaitMsccAcqCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    MSCC_MMA_ACQ_CNF_STRU                stSndMsg;
    VOS_UINT8                           ucCtxIndex;

    PS_MEM_SET(&stSndMsg, 0, sizeof(MSCC_MMA_ACQ_CNF_STRU));
    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    stSndMsg.stPlmnId.ulMcc = MMA_INVALID_MCC;
    stSndMsg.stPlmnId.ulMnc = MMA_INVALID_MNC;

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_OPER_ACQ_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_ACQ_REQ, &ucCtxIndex))
    {
        /*  回复MMA_ACQ_CNF */
        TAF_MMA_SndAcqBestNetworkCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE,
                          &stSndMsg);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;

}


VOS_UINT32 TAF_MMA_RcvTiWaitMsccRegCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    MSCC_MMA_REG_CNF_STRU                stSndMsg;
    VOS_UINT8                           ucCtxIndex;

    PS_MEM_SET(&stSndMsg, 0, sizeof(MSCC_MMA_REG_CNF_STRU));
    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    stSndMsg.stPlmnId.ulMcc = MMA_INVALID_MCC;
    stSndMsg.stPlmnId.ulMnc = MMA_INVALID_MNC;

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_OPER_REG_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_REG_REQ, &ucCtxIndex))
    {
        /*  回复MMA_REG_CNF */
        TAF_MMA_SndRegCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                          TAF_MMA_APP_OPER_RESULT_ACQUIRED_FAIL,
                          &stSndMsg);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;

}


VOS_UINT32 TAF_MMA_RcvTiWaitMsccPowerSaveExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_OPER_POWER_SAVE_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_POWER_SAVE_REQ, &ucCtxIndex))
    {
        /*  回复MMA_POWER_SAVE_CNF */
        TAF_MMA_SndPowerSaveCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE);

        TAF_MMA_ClearOperCtx(ucCtxIndex);

    }

    return VOS_TRUE;

}

/* Add by l00305157 for Service_State_Optimize_PhaseII, 2015-02-11, begin */
/*****************************************************************************
 函 数 名  : TAF_MMA_RcvDelayReportServiceStatusExpired_PreProc
 功能描述  : TI_TAF_MMA_DELAY_REPORT_SERVICE_STATUS超时处理
 输入参数  : ulEventType
             pstMsg

 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史    :
1.日    期   : 2015年02月11日
  作    者   : l00305157
  修改内容   : Service_State_Optimize_PhaseII 项目修改
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvDelayReportServiceStatusExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_SDC_REG_STATUS_ENUM_UINT8       enCsRegStatus;
    TAF_SDC_REG_STATUS_ENUM_UINT8       enPsRegStatus;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enCsServiceStatus;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enPsServiceStatus;

    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enAppCsServiceStatus;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enAppPsServiceStatus;
    TAF_SDC_REPORT_SRVSTA_ENUM_UINT8    enRptSrvSta;

    /* 如果有no rf标志，则直接不处理 */
#if (FEATURE_ON == FEATURE_DSDS)
    if (VOS_FALSE == TAF_SDC_Get3GppRfAvailFlag())
    {
        /*NO RF 重开定时器*/
        TAF_MMA_StartTimer(TI_TAF_MMA_DELAY_REPORT_SERVICE_STATUS, (TAF_MMA_GetDelayReportServiceStatusCfgTimerLen()));

        return VOS_TRUE;
    }
#endif

    /*获取服务状态*/
    enRptSrvSta                      = TAF_SDC_GetServiceStatus();

    enCsRegStatus                       = TAF_SDC_GetCsRegStatus();
    enPsRegStatus                       = TAF_SDC_GetPsRegStatus();
    enCsServiceStatus                   = TAF_SDC_GetCsServiceStatus();
    enPsServiceStatus                   = TAF_SDC_GetPsServiceStatus();

    enAppCsServiceStatus                = TAF_SDC_GetAppCsServiceStatus();
    enAppPsServiceStatus                = TAF_SDC_GetAppPsServiceStatus();

    /* 服务状态改变,上报服务状态*/
    if ((enCsServiceStatus    != enAppCsServiceStatus)
     || (enPsServiceStatus    != enAppPsServiceStatus))
    {
        TAF_MMA_ProcMsccServiceStatusReport(enRptSrvSta, TAF_SDC_GetAppServiceStatus());
    }

    /* 注册状态改变,上报注册状态*/
    TAF_MMA_ReportRegStatus(enCsRegStatus, NAS_MSCC_PIF_SRVDOMAIN_CS);

    TAF_MMA_ReportRegStatus(enPsRegStatus, NAS_MSCC_PIF_SRVDOMAIN_PS);

    /* 将内部的服务状态和注册状态更新到APPNetworkInfo中 */
    TAF_MMA_UpdateAppNetworkInfo();

    return VOS_TRUE;
}
/* Add by l00305157 for Service_State_Optimize_PhaseII, 2015-02-11, end */

#if (FEATURE_ON == FEATURE_ECALL)

VOS_UINT32 TAF_MMA_ProcPihUsimRefreshIndEfUstFileChanged_PreProc(VOS_VOID)
{
    TAF_SDC_CALL_MODE_ENUM_UINT8        enOldCallMode;
    TAF_SDC_CALL_MODE_ENUM_UINT8        enNewCallMode;

    /* 关机模式不处理文件变更 */
    if (TAF_SDC_PHONE_MODE_FULL != TAF_SDC_GetCurPhoneMode())
    {
       return VOS_TRUE;
    }

    /* 判断ecall模式是否发生改变 */
    enOldCallMode = TAF_SDC_GetCurCallMode();

    /*  更新ecall模式信息 */
    TAF_MMA_UpdateCallMode();

    enNewCallMode = TAF_SDC_GetCurCallMode();

    /* 进出ecall only模式 */
    if ((enOldCallMode       != enNewCallMode)
     && ((TAF_SDC_ECALL_ONLY == enOldCallMode)
      || (TAF_SDC_ECALL_ONLY == enNewCallMode)))
    {
        /* 当前状态机不是main状态,构造内部卡状态改变指示消息缓存 */
        if (TAF_MMA_FSM_MAIN != TAF_MMA_GetCurrFsmId())
        {
            TAF_MMA_SndInterUsimChangeInd();

            return VOS_TRUE;
        }

        /* 已经开机则进入main触发关机流程 */
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

#endif

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)


TAF_SDC_SERVICE_STATUS_ENUM_UINT8 TAF_MMA_MapMsccServiceStatus(
    NAS_MSCC_PIF_SERVICE_STATUS_ENUM_UINT32     enServiceStatus
)
{
    if (NAS_MSCC_PIF_SERVICE_STATUS_LIMITED_SERVICE == enServiceStatus)
    {
        return TAF_SDC_SERVICE_STATUS_LIMITED_SERVICE;
    }
    else if ( NAS_MSCC_PIF_SERVICE_STATUS_NORMAL_SERVICE == enServiceStatus)
    {
        return TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE;
    }
    else if ( NAS_MSCC_PIF_SERVICE_STATUS_LIMITED_SERVICE_REGION == enServiceStatus)
    {
        return TAF_SDC_SERVICE_STATUS_LIMITED_SERVICE_REGION;
    }
    else if ( NAS_MSCC_PIF_SERVICE_STATUS_NO_IMSI == enServiceStatus)
    {
        return TAF_SDC_SERVICE_STATUS_NO_IMSI;
    }
    else if ( NAS_MSCC_PIF_SERVICE_STATUS_NO_CHANGE == enServiceStatus)
    {
        return TAF_SDC_SERVICE_STATUS_NO_CHANGE;
    }
    else if ( NAS_MSCC_PIF_SERVICE_STATUS_DEEP_SLEEP == enServiceStatus)
    {
        return TAF_SDC_SERVICE_STATUS_DEEP_SLEEP;
    }
    else
    {
        return TAF_SDC_SERVICE_STATUS_NO_SERVICE;
    }


}


TAF_SDC_NETWORK_EXISTANCE_ENUM_UINT32 TAF_MMA_Map1xSrvStaToCdmaNetworkExistance(
    NAS_MSCC_PIF_SERVICE_STATUS_ENUM_UINT32                 enSrvSta
)
{
    if ((NAS_MSCC_PIF_SERVICE_STATUS_NORMAL_SERVICE == enSrvSta)
     || (NAS_MSCC_PIF_SERVICE_STATUS_LIMITED_SERVICE == enSrvSta))
    {
        return TAF_SDC_1X_NETWORK_EXIST;
    }
    else
    {
        return TAF_SDC_1X_NETWORK_NOT_EXIST;
    }
}
/* Deleted by h00313353 for CDMA Iteration 15, 2015-5-30, begin */

/* Deleted by h00313353 for CDMA Iteration 15, 2015-5-30, end */





/* Del by 00322978, 20150316 for CDMA 1x Iteration 9, begin */
/* Del by 00322978, 20150316 for CDMA 1x Iteration 9, end */

/* Added by m00312079 for CDMA Iteration 12 2015-6-3 begin */
/*****************************************************************************
 函 数 名  : TAF_MMA_CovertSubMode_1xLost
 功能描述  : 1x无服务时，获取当前的submode。
 输入参数  : TAF_SDC_SYS_SUBMODE_ENUM_UINT8      enPreSubMode
 输出参数  : 无
 返 回 值  : TAF_SDC_SYS_SUBMODE_ENUM_UINT8
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年06月03日
    作    者   : m00312079
    修改内容   : 新建
*****************************************************************************/
TAF_SDC_SYS_SUBMODE_ENUM_UINT8 TAF_MMA_CovertSubMode_1xLost(
    TAF_SDC_SYS_SUBMODE_ENUM_UINT8      enPreSubMode
)
{
    TAF_SDC_SYS_SUBMODE_ENUM_UINT8      enCurrSubMode;

    switch (enPreSubMode)
    {
        case TAF_SDC_SYS_SUBMODE_HYBRID_EVDO_REL_A:

            enCurrSubMode = TAF_SDC_SYS_SUBMODE_EVDO_REL_A;
            break;

        case TAF_SDC_SYS_SUBMODE_HYBRID_EVDO_REL_0:

            enCurrSubMode = TAF_SDC_SYS_SUBMODE_EVDO_REL_0;
            break;
        default:

            enCurrSubMode = TAF_SDC_GetSysSubMode();
            break;
    }

    return enCurrSubMode;

}

/*****************************************************************************
 函 数 名  : TAF_MMA_CovertSubMode_1xExist
 功能描述  : 1x有服务时，获取当前的submode。
 输入参数  : TAF_SDC_SYS_SUBMODE_ENUM_UINT8      enPreSubMode
 输出参数  : 无
 返 回 值  : TAF_SDC_SYS_SUBMODE_ENUM_UINT8
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年06月03日
    作    者   : m00312079
    修改内容   : 新建
*****************************************************************************/
TAF_SDC_SYS_SUBMODE_ENUM_UINT8 TAF_MMA_CovertSubMode_1xExist(
     TAF_SDC_SYS_SUBMODE_ENUM_UINT8      enPreSubMode

)
{
    TAF_SDC_SYS_SUBMODE_ENUM_UINT8      enCurrSubMode;

    switch (enPreSubMode)
    {
        case TAF_SDC_SYS_SUBMODE_EVDO_REL_A:

            enCurrSubMode = TAF_SDC_SYS_SUBMODE_HYBRID_EVDO_REL_A;
            break;

        case TAF_SDC_SYS_SUBMODE_EVDO_REL_0:

            enCurrSubMode = TAF_SDC_SYS_SUBMODE_HYBRID_EVDO_REL_0;
            break;
        default:

            enCurrSubMode = TAF_SDC_GetSysSubMode();
            break;
    }

    return enCurrSubMode;

}
/* Added by m00312079 for CDMA Iteration 12 2015-6-3 end */



/* Added by l00301449 for CDMA STK, 2015-07-03, begin */
/*****************************************************************************
 函 数 名  : TAF_MMA_BuildStk1xSystemInfo
 功能描述  : 构造给STK的1X system info
 输入参数  : NAS_MSCC_PIF_1X_SYS_SRV_INFO_STRU  *pstMscc1xSysSrvInfo
 输出参数  : MMA_STK_1X_SYS_INFO_STRU           *pstStk1xSysInfo
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月3日
    作    者   : l00301449
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID TAF_MMA_BuildStk1xSystemInfo(
    NAS_MSCC_PIF_1X_SYS_SRV_INFO_STRU  *pstMscc1xSysSrvInfo,
    MMA_STK_1X_SYS_INFO_STRU           *pstStk1xSysInfo
)
{
    TAF_SDC_1X_SYS_INFO_STRU           *pSdc1xSysInfo = VOS_NULL_PTR;

    pSdc1xSysInfo = TAF_SDC_Get1xSysInfo();

    if (PS_TRUE == pstMscc1xSysSrvInfo->enServiceInfoIncl)
    {
        pstStk1xSysInfo->ulMcc           = TAF_STD_TransformBcdMccToDeciDigit(pstMscc1xSysSrvInfo->stServiceInfo.ulMcc);
        pstStk1xSysInfo->usImsi11_12     = TAF_STD_TransformBcdImsi1112ToDeciDigit(pstMscc1xSysSrvInfo->stServiceInfo.usImsi11_12);
    }
    else
    {
        pstStk1xSysInfo->ulMcc           = TAF_STD_TransformBcdMccToDeciDigit(pSdc1xSysInfo->stServiceInfo.ulMcc);
        pstStk1xSysInfo->usImsi11_12     = TAF_STD_TransformBcdImsi1112ToDeciDigit(pSdc1xSysInfo->stServiceInfo.usImsi11_12);
    }

    pstStk1xSysInfo->usSid           = pstMscc1xSysSrvInfo->usSid;
    pstStk1xSysInfo->usNid           = pstMscc1xSysSrvInfo->usNid;

    if (PS_TRUE == pstMscc1xSysSrvInfo->enBaseStationInfoIncl)
    {
        pstStk1xSysInfo->usBaseId        = pstMscc1xSysSrvInfo->stBaseStationInfo.usBaseId;
        pstStk1xSysInfo->lBaseLatitude   = pstMscc1xSysSrvInfo->stBaseStationInfo.lBaseLatitude;
        pstStk1xSysInfo->lBaseLongtitude = pstMscc1xSysSrvInfo->stBaseStationInfo.lBaseLongitude;
    }
    else
    {
        pstStk1xSysInfo->usBaseId        = pSdc1xSysInfo->stBaseStationInfo.usBaseId;
        pstStk1xSysInfo->lBaseLatitude   = pSdc1xSysInfo->stBaseStationInfo.lBaseLatitude;
        pstStk1xSysInfo->lBaseLongtitude = pSdc1xSysInfo->stBaseStationInfo.lBaseLongitude;
    }
}


/*****************************************************************************
 函 数 名  : TAF_MMA_BuildStkSrvStatus
 功能描述  : 构造给STK的服务状态
 输入参数  : NAS_MSCC_PIF_SERVICE_STATUS_ENUM_UINT32     enServiceStatus
             NAS_STK_SERVICE_STATUS_INFO_STRU           *pstServiceStatus
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月3日
    作    者   : l00301449
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID TAF_MMA_BuildStkSrvStatus(
    NAS_MSCC_PIF_SERVICE_STATUS_ENUM_UINT32     enServiceStatus,
    NAS_STK_SERVICE_STATUS_INFO_STRU           *pstServiceStatus
)
{
    switch(enServiceStatus)
    {
        case NAS_MSCC_PIF_SERVICE_STATUS_NORMAL_SERVICE:
            pstServiceStatus->enCsServiceStatus = NAS_STK_SERVICE_STATUS_NORMAL_SERVICE;
            break;

        case NAS_MSCC_PIF_SERVICE_STATUS_LIMITED_SERVICE:
            pstServiceStatus->enCsServiceStatus = NAS_STK_SERVICE_STATUS_LIMITED_SERVICE;
            break;

        case NAS_MSCC_PIF_SERVICE_STATUS_NO_SERVICE:
            pstServiceStatus->enCsServiceStatus = NAS_STK_SERVICE_STATUS_NO_SERVICE;
            break;

        default:
            pstServiceStatus->enCsServiceStatus = NAS_STK_SERVICE_STATUS_NO_SERVICE;
            break;
    }
}

/*****************************************************************************
 函 数 名  : TAF_MMA_IsStk1xSystemInfoChanged
 功能描述  : 判断STK需要的service info是否有改变
 输入参数  : TAF_SDC_SERVICE_STATUS_ENUM_UINT8                       enCurServiceStatus
             NAS_MSCC_PIF_1X_SYS_SRV_INFO_STRU               *pstMscc1xSysSrvInfo
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月8日
    作    者   : l00301449
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_IsStk1xSystemInfoChanged(
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enCurServiceStatus,
    NAS_MSCC_PIF_1X_SYS_SRV_INFO_STRU  *pstMscc1xSysSrvInfo
)
{
    TAF_SDC_1X_SYS_INFO_STRU           *pSdc1xSysInfo = VOS_NULL_PTR;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enLastCsServiceStatus;

    pSdc1xSysInfo = TAF_SDC_Get1xSysInfo();

    enLastCsServiceStatus = TAF_SDC_GetCsServiceStatus();

    /* 判断服务状态 */
    if (enLastCsServiceStatus != enCurServiceStatus)
    {
        return VOS_TRUE;
    }

    if (PS_TRUE == pstMscc1xSysSrvInfo->enServiceInfoIncl)
    {
        if (pSdc1xSysInfo->stServiceInfo.ulMcc != pstMscc1xSysSrvInfo->stServiceInfo.ulMcc)
        {
            return VOS_TRUE;
        }

        if (pSdc1xSysInfo->stServiceInfo.usImsi11_12 != pstMscc1xSysSrvInfo->stServiceInfo.usImsi11_12)
        {
            return VOS_TRUE;
        }
    }

    if (pSdc1xSysInfo->lSid != (0x00000000 | pstMscc1xSysSrvInfo->usSid))
    {
        return VOS_TRUE;
    }

    if (pSdc1xSysInfo->lNid != (0x00000000 | pstMscc1xSysSrvInfo->usNid))
    {
        return VOS_TRUE;
    }


    if (PS_TRUE == pstMscc1xSysSrvInfo->enBaseStationInfoIncl)
    {
        if (pSdc1xSysInfo->stBaseStationInfo.usBaseId != pstMscc1xSysSrvInfo->stBaseStationInfo.usBaseId)
        {
            return VOS_TRUE;
        }

        if (pSdc1xSysInfo->stBaseStationInfo.lBaseLatitude != pstMscc1xSysSrvInfo->stBaseStationInfo.lBaseLatitude)
        {
            return VOS_TRUE;
        }

        if (pSdc1xSysInfo->stBaseStationInfo.lBaseLongitude != pstMscc1xSysSrvInfo->stBaseStationInfo.lBaseLongitude)
        {
            return VOS_TRUE;
        }
    }

    return VOS_FALSE;
}

/* Added by l00301449 for CDMA STK, 2015-07-03, end */


VOS_UINT32 TAF_MMA_RcvMscc1xSystemServiceInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_1X_SYSTEM_SERVICE_INFO_IND_STRU               *pstSysSrvInfoInd = VOS_NULL_PTR;
    TAF_SDC_NETWORK_EXISTANCE_ENUM_UINT32                   enCdmaNetworkExist;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8                       enServiceStatus;
    TAF_SDC_SYS_MODE_ENUM_UINT8                             enSysMode;
    TAF_SDC_SYS_SUBMODE_ENUM_UINT8                          enSubMode;

    /* Added by l00301449 for CDMA STK, 2015-07-03, begin */
    MMA_STK_1X_SYS_INFO_STRU                                stSysInfo;
    NAS_STK_SERVICE_STATUS_INFO_STRU                        stServiceStatus;
    /* Added by l00301449 for CDMA STK, 2015-07-03, end */

    pTafSdcSysmodServiceRegStaUpdateFunc                    pFuncTafSdcSysmodServiceRegStaUpdate;

    /* Added by h00313353 for Iteration 17, 2015-8-13, begin */
    NAS_MSCC_PIF_PLMN_ID_STRU                               stPlmnId;
    /* Added by h00313353 for Iteration 17, 2015-8-13, end */


    pFuncTafSdcSysmodServiceRegStaUpdate = VOS_NULL_PTR;
    enSysMode                            = TAF_SDC_SYS_MODE_BUTT;
    enSubMode                            = TAF_SDC_SYS_SUBMODE_BUTT;

    /* Added by h00313353 for Iteration 17, 2015-8-13, begin */
    PS_MEM_SET(&stPlmnId, 0x00, sizeof(NAS_MSCC_PIF_PLMN_ID_STRU));
    /* Added by h00313353 for Iteration 17, 2015-8-13, end */

    /* Added by l00301449 for CDMA STK, 2015-07-03, begin */
    PS_MEM_SET(&stSysInfo, 0, sizeof(MMA_STK_1X_SYS_INFO_STRU));
    PS_MEM_SET(&stServiceStatus, 0, sizeof(NAS_STK_SERVICE_STATUS_INFO_STRU));
    /* Added by l00301449 for CDMA STK, 2015-07-03, end */

    pstSysSrvInfoInd = (MSCC_MMA_1X_SYSTEM_SERVICE_INFO_IND_STRU *)pstMsg;

    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, begin */
    /* 如果有no rf标志，则直接不处理 */
    if (VOS_FALSE == TAF_SDC_Get1xRfAvailFlag())
    {
        return VOS_TRUE;
    }
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, end */

    enCdmaNetworkExist = TAF_MMA_Map1xSrvStaToCdmaNetworkExistance(pstSysSrvInfoInd->stSysSrvInfo.enServiceStatus);

    /* Cdma have limited service and normal service, so it should be concerned*/
    enServiceStatus = TAF_MMA_MapMsccServiceStatus(pstSysSrvInfoInd->stSysSrvInfo.enServiceStatus);

    /* to calc the sysmode and submode */
    TAF_MMA_MapCdmaNetworkExistanceSrvStaToSysMode(enCdmaNetworkExist, &enSysMode, &enSubMode, &pFuncTafSdcSysmodServiceRegStaUpdate);

    if (TAF_SDC_1X_NETWORK_EXIST  == enCdmaNetworkExist)
    {

        /* Deleted by y00322978 for CDMA Iteration 17, 2015-7-9, begin */

        /* Deleted by y00322978 for CDMA Iteration 17, 2015-7-9, end */

        TAF_MMA_SndApsEpdszidInfoInd(pstSysSrvInfoInd->stSysSrvInfo.usSid,
                                     pstSysSrvInfoInd->stSysSrvInfo.usNid,
                                     pstSysSrvInfoInd->stSysSrvInfo.ucPzid);

        /* Added by m00312079 for CDMA Iteration 12 2015-6-3 begin */
        if (TAF_SDC_SYS_MODE_HYBRID == enSysMode)
        {
            enSubMode = TAF_SDC_GetSysSubMode();

            enSubMode = TAF_MMA_CovertSubMode_1xExist(enSubMode);
        }
        /* Added by m00312079 for CDMA Iteration 12 2015-6-3 end */


        /* ^MODE命令上报处理 */
        Mma_ModeChgReport(enSysMode, enSubMode);

        /* 通知APS当前的服务状态,只要有1X链接就认为可以在1X尝试建链 */
        TAF_MMA_SndTaf1XServiceStatusInd(VOS_TRUE);


        /* 通知XSMS当前的服务状态*/

        TAF_MMA_SndXsmsServiceStatusInd(enSysMode, VOS_FALSE);

        /* Added by l00301449 for CDMA STK, 2015-07-03, begin */
        if (VOS_TRUE == TAF_MMA_IsStk1xSystemInfoChanged(enServiceStatus, &(pstSysSrvInfoInd->stSysSrvInfo)))
        {
            TAF_MMA_BuildStk1xSystemInfo(&(pstSysSrvInfoInd->stSysSrvInfo), &stSysInfo);
            TAF_MMA_BuildStkSrvStatus(pstSysSrvInfoInd->stSysSrvInfo.enServiceStatus, &stServiceStatus);
            TAF_MMA_SndStk1xServiceInfoInd(&stSysInfo, &stServiceStatus);
        }
        /* Added by l00301449 for CDMA STK, 2015-07-03, end */

        /* 更新MMA维护的Mode/SubMode */
        TAF_SDC_SetSysMode(enSysMode);

        /* TO Do:后续如果当前SubMode是DO,则不需要更新SubMode */
        TAF_SDC_SetSysSubMode(enSubMode);

        /* 上报CREG */
        if (TAF_SDC_SYS_ROAMING_STATUS_OFF == pstSysSrvInfoInd->stSysSrvInfo.ucRoamingInd)
        {
            TAF_MMA_Report1xCregStatus(TAF_SDC_REG_REGISTERED_HOME_NETWORK);
        }
        else
        {
            TAF_MMA_Report1xCregStatus(TAF_SDC_REG_REGISTERED_ROAM);
        }


        /* Normal service时，更新系统消息 */
        TAF_SDC_Set1xSysInfo((NAS_MSCC_PIF_1X_SYS_SRV_INFO_STRU*)(&(pstSysSrvInfoInd->stSysSrvInfo)));

    }

    if (TAF_SDC_1X_NETWORK_NOT_EXIST == enCdmaNetworkExist)
    {

        /* Added by m00312079 for CDMA Iteration 12 2015-6-3 begin */
        if (TAF_SDC_SYS_MODE_EVDO == enSysMode)
        {
            enSubMode = TAF_SDC_GetSysSubMode();

            enSubMode = TAF_MMA_CovertSubMode_1xLost(enSubMode);
        }
        /* Added by m00312079 for CDMA Iteration 12 2015-6-3 end */

        /* ^MODE命令上报处理 */
        Mma_ModeChgReport(enSysMode, enSubMode);

        TAF_MMA_SndTaf1XServiceStatusInd(VOS_FALSE);

        /* Added by l00301449 for CDMA STK, 2015-07-03, begin */
        /* 从有服务到无服务才上报 */
        if (TAF_SDC_SERVICE_STATUS_NO_SERVICE == TAF_SDC_GetCsServiceStatus())
        {
            TAF_MMA_BuildStkSrvStatus(NAS_MSCC_PIF_SERVICE_STATUS_NO_SERVICE, &stServiceStatus);
            TAF_MMA_SndStk1xServiceInfoInd(&stSysInfo, &stServiceStatus);
        }
        /* Added by l00301449 for CDMA STK, 2015-07-03, end */

        TAF_SDC_SetSysMode(enSysMode);
        TAF_SDC_SetSysSubMode(enSubMode);

        /* 上报CREG */
        TAF_MMA_Report1xCregStatus(TAF_SDC_REG_NOT_REGISTERED_SEARCHING);
    }

    /* 更新服务状态、域状态 */
    TAF_MMA_Updata1xServiceStatusAndDomain(enServiceStatus, enSysMode);

    /* 上报服务状态 */
    TAF_MMA_ReprotCdmaServiceState(enServiceStatus, enSysMode);

    /* Added by y00322978 for CDMA Iteration 17, 2015-7-10, begin */
    /* 可维可测 */
    TAF_MMA_SndOmCdmaStatusInfoReportInd();
    /* Added by y00322978 for CDMA Iteration 17, 2015-7-10, end */

    /* Added by h00313353 for Iteration 17, 2015-8-13, begin */
    /* 包含服务信息，进行上报 */
    if (PS_TRUE == pstSysSrvInfoInd->stSysSrvInfo.enServiceInfoIncl)
    {
        stPlmnId.ulMcc  = pstSysSrvInfoInd->stSysSrvInfo.stServiceInfo.ulMcc;
        /*mnc只有2位，高位加0x0f */
        stPlmnId.ulMnc  = ((VOS_UINT32)pstSysSrvInfoInd->stSysSrvInfo.stServiceInfo.usMnc) | 0x0f0000;

        TAF_MMA_PlmnIdReport((TAF_PLMN_ID_STRU *)&stPlmnId);
    }
    /* Added by h00313353 for Iteration 17, 2015-8-13, end */

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvMsccSyncServiceAvailInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_SYNC_SERVICE_AVAIL_IND_STRU                   *pstSyncSrvAvailInd = VOS_NULL_PTR;
    TAF_SDC_NETWORK_EXISTANCE_ENUM_UINT32                   enCdmaNetworkExist;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8                       enServiceStatus;
    TAF_SDC_SYS_MODE_ENUM_UINT8                             enSysMode;
    TAF_SDC_SYS_SUBMODE_ENUM_UINT8                          enSubMode;
    MMA_STK_1X_SYS_INFO_STRU                                stSysInfo;
    NAS_STK_SERVICE_STATUS_INFO_STRU                        stServiceStatus;
    pTafSdcSysmodServiceRegStaUpdateFunc                    pFuncTafSdcSysmodServiceRegStaUpdate;
    TAF_SDC_1X_SYS_INFO_STRU                               *pstSdc1xSysInfo = VOS_NULL_PTR;

    pFuncTafSdcSysmodServiceRegStaUpdate = VOS_NULL_PTR;
    enSysMode                            = TAF_SDC_SYS_MODE_BUTT;
    enSubMode                            = TAF_SDC_SYS_SUBMODE_BUTT;

    PS_MEM_SET(&stSysInfo, 0, sizeof(MMA_STK_1X_SYS_INFO_STRU));
    PS_MEM_SET(&stServiceStatus, 0, sizeof(NAS_STK_SERVICE_STATUS_INFO_STRU));

    pstSyncSrvAvailInd = (MSCC_MMA_SYNC_SERVICE_AVAIL_IND_STRU *)pstMsg;

    /* 如果有no rf标志，则直接不处理 */
    if (VOS_FALSE == TAF_SDC_Get1xRfAvailFlag())
    {
        return VOS_TRUE;
    }

    enCdmaNetworkExist = TAF_MMA_Map1xSrvStaToCdmaNetworkExistance(pstSyncSrvAvailInd->enServiceStatus);

    /* Cdma have limited service and normal service, so it should be concerned*/
    enServiceStatus = TAF_MMA_MapMsccServiceStatus(pstSyncSrvAvailInd->enServiceStatus);

    /* to calc the sysmode and submode */
    TAF_MMA_MapCdmaNetworkExistanceSrvStaToSysMode(enCdmaNetworkExist, &enSysMode, &enSubMode, &pFuncTafSdcSysmodServiceRegStaUpdate);

    if (TAF_SDC_SYS_MODE_HYBRID == enSysMode)
    {
        enSubMode = TAF_SDC_GetSysSubMode();

        enSubMode = TAF_MMA_CovertSubMode_1xExist(enSubMode);
    }


    /* ^MODE命令上报处理 */
    Mma_ModeChgReport(enSysMode, enSubMode);

    /* 更新MMA维护的Mode/SubMode */
    TAF_SDC_SetSysMode(enSysMode);

    /* TO Do:后续如果当前SubMode是DO,则不需要更新SubMode */
    TAF_SDC_SetSysSubMode(enSubMode);

    /* 上报CREG */
    if (TAF_SDC_SYS_ROAMING_STATUS_OFF == pstSyncSrvAvailInd->ucRoamingInd)
    {
        TAF_MMA_Report1xCregStatus(TAF_SDC_REG_REGISTERED_HOME_NETWORK);
    }
    else
    {
        TAF_MMA_Report1xCregStatus(TAF_SDC_REG_REGISTERED_ROAM);
    }

    /* Normal service时，更新系统消息 */
    pstSdc1xSysInfo = TAF_SDC_Get1xSysInfo();

    pstSdc1xSysInfo->usBandClass      = pstSyncSrvAvailInd->usBandClass;
    pstSdc1xSysInfo->usFreq           = pstSyncSrvAvailInd->usFreq;
    pstSdc1xSysInfo->lSid             = (0x00000000 | pstSyncSrvAvailInd->usSid);
    pstSdc1xSysInfo->lNid             = (0x00000000 | pstSyncSrvAvailInd->usNid);
    pstSdc1xSysInfo->ucRoamingInd     = pstSyncSrvAvailInd->ucRoamingInd;

    /* 更新服务状态、域状态 */
    TAF_MMA_Updata1xServiceStatusAndDomain(enServiceStatus, enSysMode);

    /* 上报服务状态 */
    TAF_MMA_ReprotCdmaServiceState(enServiceStatus, enSysMode);

    /* 可维可测 */
    TAF_MMA_SndOmCdmaStatusInfoReportInd();

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvTafCdmaMoCallStartNtf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CDMA_MO_CALL_START_NTF_STRU *pstCallStartNtf = VOS_NULL_PTR;
    TAF_SDC_1X_SYS_INFO_STRU            *pstSdc1xSysInfo = VOS_NULL_PTR;

    pstCallStartNtf = (TAF_MMA_CDMA_MO_CALL_START_NTF_STRU *)pstMsg;
    pstSdc1xSysInfo = TAF_SDC_Get1xSysInfo();

    /* 通知XSD */
    TAF_MMA_SndMsccCdmaMoCallStartNtf(pstCallStartNtf->enCallType);

    /* Added by h00313353 for Iteration 17, 2015-7-21, begin */
    /* CallBack状态，非数据呼发送退出CallBack请求 */
    if ((VOS_TRUE == pstSdc1xSysInfo->ulIsInEmcCallBack)
     && (TAF_MMA_1X_NORMAL_DATA_CALL != pstCallStartNtf->enCallType))
    {
        TAF_MMA_SndMsccQuitCallBackNtf();
    }
    /* Added by h00313353 for Iteration 17, 2015-7-21, end */

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvTafCdmaMoCallEndNtf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CDMA_MO_CALL_END_NTF_STRU *pstCallEndNtf = VOS_NULL_PTR;

    pstCallEndNtf = (TAF_MMA_CDMA_MO_CALL_END_NTF_STRU *)pstMsg;

    /* 通知XSD */
    TAF_MMA_SndMsccCdmaMoCallEndNtf(pstCallEndNtf->enCallType);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvTafCdmaMoCallSuccessNtf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CDMA_MO_CALL_SUCCESS_NTF_STRU   *pstCallSuccNtf = VOS_NULL_PTR;

    pstCallSuccNtf = (TAF_MMA_CDMA_MO_CALL_SUCCESS_NTF_STRU *)pstMsg;

    /* 通知XSD */
    TAF_MMA_SndMsccCdmaMoCallSuccessNtf(pstCallSuccNtf->enCallType);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvTafCdmaCallRedialSystemAcquireNtf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CDMA_CALL_REDIAL_SYSTEM_ACQUIRE_NTF_STRU       *pstCallRedialNtf = VOS_NULL_PTR;

    pstCallRedialNtf = (TAF_MMA_CDMA_CALL_REDIAL_SYSTEM_ACQUIRE_NTF_STRU *)pstMsg;

    /* 通知XSD */
    TAF_MMA_SndMsccCdmaMoCallRedialSysAcqNtf(pstCallRedialNtf->enCallType, pstCallRedialNtf->ulCause);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvAtQueryCLocInfo_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    /* Modified by k902809 for Iteration 11, 2015-3-25, begin */
    TAF_MMA_CDMA_LOCINFO_QRY_REQ_STRU  *pstAppReqMsg = VOS_NULL_PTR;
    /* Modified by k902809 for Iteration 11, Iteration 11 2015-3-25, end */
    TAF_MMA_CDMA_LOCINFO_QRY_CNF_STRU  *pstCLocInfoQryCnf = VOS_NULL_PTR;
    TAF_SDC_1X_SYS_INFO_STRU           *pstSdc1xSysInfo;

    pstAppReqMsg      = (TAF_MMA_CDMA_LOCINFO_QRY_REQ_STRU *)pstMsg;

    /* Modified by k902809 for Iteration 11, 2015-3-25, begin */
    pstCLocInfoQryCnf = (TAF_MMA_CDMA_LOCINFO_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MMA,
                                                                                          sizeof(TAF_MMA_CDMA_LOCINFO_QRY_CNF_STRU));
    /* Modified by k902809 for Iteration 11, Iteration 11 2015-3-25, end */
    if (VOS_NULL_PTR == pstCLocInfoQryCnf)
    {
        MMA_ERRORLOG("TAF_MMA_RcvAtQueryCLocInfo_PreProc: ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }

    /* Modified by k902809 for Iteration 11, 2015-3-25, begin */
    PS_MEM_SET(((VOS_UINT8 *)pstCLocInfoQryCnf + VOS_MSG_HEAD_LENGTH),
               0x00,
               (sizeof(TAF_MMA_CDMA_LOCINFO_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH));
    /* Modified by k902809 for Iteration 11, Iteration 11 2015-3-25, end */

    pstSdc1xSysInfo = TAF_SDC_Get1xSysInfo();

    /* 填写消息头部信息 */
    pstCLocInfoQryCnf->ulReceiverPid          = pstAppReqMsg->stCtrl.ulModuleId;
    /* Modified by k902809 for Iteration 11, 2015-3-25, begin */
    pstCLocInfoQryCnf->ulMsgName              = ID_TAF_MMA_CDMA_LOCINFO_QRY_CNF;
    PS_MEM_CPY(&pstCLocInfoQryCnf->stCtrl, &pstAppReqMsg->stCtrl, sizeof(TAF_MMA_CTRL_STRU));
    /* Modified by k902809 for Iteration 11, Iteration 11 2015-3-25, end */
    if (TAF_SDC_SYS_MODE_CDMA_1X == TAF_SDC_GetSysMode())
    {
        pstCLocInfoQryCnf->ulRslt          = VOS_OK;
        pstCLocInfoQryCnf->usBaseId        = pstSdc1xSysInfo->stBaseStationInfo.usBaseId;
        pstCLocInfoQryCnf->lBaseLatitude  = pstSdc1xSysInfo->stBaseStationInfo.lBaseLatitude;
        pstCLocInfoQryCnf->lBaseLongitude = pstSdc1xSysInfo->stBaseStationInfo.lBaseLongitude;
        pstCLocInfoQryCnf->lSid            = pstSdc1xSysInfo->lSid;
        pstCLocInfoQryCnf->lNid            = pstSdc1xSysInfo->lNid;
        pstCLocInfoQryCnf->ulMcc           = (VOS_UINT32)TAF_STD_TransformBcdMccToDeciDigit(pstSdc1xSysInfo->stServiceInfo.ulMcc);
        pstCLocInfoQryCnf->ulMnc           = (VOS_UINT32)TAF_STD_TransformBcdMncToDeciDigit(pstSdc1xSysInfo->stServiceInfo.usImsi11_12);
        pstCLocInfoQryCnf->usPrevInUse     = pstSdc1xSysInfo->ucPrevInUse;

        /* Added by y00322978 for CDMA Iteration 17, 2015-7-14, begin */
        pstCLocInfoQryCnf->ulSidEnable     = pstSdc1xSysInfo->ulSidEnable;
        /* Added by y00322978 for CDMA Iteration 17, 2015-7-14, end */
    }
    else
    {
        pstCLocInfoQryCnf->ulRslt          = VOS_ERR;
    }

    /* 发送消息 */
    PS_SEND_MSG(WUEPS_PID_MMA, pstCLocInfoQryCnf);

    return VOS_TRUE;

}


/* Added by y00307564 for CDMA 1X Iteration 6, 2014-12-29, begin */
/*****************************************************************************
 函 数 名  : TAF_MMA_RcvSetCFreqLock_PreProc
 功能描述  : MMA预处理中处理^CFREQLOCK设置命令
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年12月29日
    作    者   : y00307564
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvSetCFreqLock_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CFREQ_LOCK_SET_REQ_STRU    *pstCFreqLockSetReq     = VOS_NULL_PTR;
    TAF_MMA_CFREQ_LOCK_SET_PARA_STRU   *pstTafMmaCFreqLockPara = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt;

    pstCFreqLockSetReq = (TAF_MMA_CFREQ_LOCK_SET_REQ_STRU *)pstMsg;

    /*  读取全局变量的值 */
    pstTafMmaCFreqLockPara = TAF_MMA_GetCFreqLockSetPara();

    /*  如果设置参数变化则写NV并NTF XSD/HSD */
    if (0 != PS_MEM_CMP(pstTafMmaCFreqLockPara,
                        &pstCFreqLockSetReq->stCFreqLockPara,
                        sizeof(TAF_MMA_CFREQ_LOCK_SET_PARA_STRU)))
    {
        #ifndef DMT
        /* wirte CFREQLOCK informations into NV */
        ulRslt = NV_Write(en_NV_Item_CFreqLock_CFG,
                          &pstCFreqLockSetReq->stCFreqLockPara,
                          sizeof(TAF_NVIM_CFREQ_LOCK_CFG_STRU));

        if (NV_OK != ulRslt)
        {
            TAF_MMA_SndCFreqLockSetCnf(pstCFreqLockSetReq, VOS_FALSE);
            return VOS_TRUE;
        }
        #endif
        /* 更新MMA保存的锁频全局变量 */
        PS_MEM_CPY(pstTafMmaCFreqLockPara,
                   &pstCFreqLockSetReq->stCFreqLockPara,
                   sizeof(TAF_MMA_CFREQ_LOCK_SET_PARA_STRU));

        /* 发送MMA_XSD_CFREQLOCK_SET_NTF */
        TAF_MMA_SndMsccCFreqLockNtf(pstTafMmaCFreqLockPara);

        /* TODO:发送MMA_HSD_CFREQLOCK_SET_NTF */

    }
    /* 发送TAF_MMA_CFREQ_LOCK_SET_CNF */
    TAF_MMA_SndCFreqLockSetCnf(pstCFreqLockSetReq, VOS_TRUE);

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvQueryCFreqLock_PreProc
 功能描述  : MMA预处理中处理^CFREQLOCK查询命令
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年12月29日
    作    者   : y00307564
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvQueryCFreqLock_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CFREQ_LOCK_QUERY_REQ_STRU  *pstAppReqMsg            = VOS_NULL_PTR;
    TAF_MMA_CFREQ_LOCK_QUERY_CNF_STRU  *pstCFreqLockQryCnf      = VOS_NULL_PTR;
    TAF_MMA_CFREQ_LOCK_SET_PARA_STRU   *pstTafMmaCFreqLockPara  = VOS_NULL_PTR;


    pstAppReqMsg     = (TAF_MMA_CFREQ_LOCK_QUERY_REQ_STRU *)pstMsg;

    pstCFreqLockQryCnf = (TAF_MMA_CFREQ_LOCK_QUERY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MMA,
                                                                                       sizeof(TAF_MMA_CFREQ_LOCK_QUERY_CNF_STRU));
    if (VOS_NULL_PTR == pstCFreqLockQryCnf)
    {
        MMA_ERRORLOG("TAF_MMA_RcvAtQueryCFreqLock_PreProc: ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }

    PS_MEM_SET(((VOS_UINT8 *)pstCFreqLockQryCnf + VOS_MSG_HEAD_LENGTH),
               0x00,
               (sizeof(TAF_MMA_CFREQ_LOCK_QUERY_CNF_STRU) - VOS_MSG_HEAD_LENGTH));

    /* 填写消息头部信息 */
    pstCFreqLockQryCnf->ulReceiverPid          = pstAppReqMsg->stCtrl.ulModuleId;
    pstCFreqLockQryCnf->ulMsgName              = ID_TAF_MMA_CDMA_FREQ_LOCK_QRY_CNF;

    PS_MEM_CPY(&pstCFreqLockQryCnf->stCtrl, &pstAppReqMsg->stCtrl, sizeof(TAF_MMA_CTRL_STRU));

    pstTafMmaCFreqLockPara = TAF_MMA_GetCFreqLockSetPara();
    PS_MEM_CPY(&pstCFreqLockQryCnf->stCFreqLockPara,
                pstTafMmaCFreqLockPara,
                sizeof(TAF_MMA_CFREQ_LOCK_SET_PARA_STRU));

    /* 发送消息 */
    PS_SEND_MSG(WUEPS_PID_MMA, pstCFreqLockQryCnf);

    return VOS_TRUE;
}
/* Added by y00307564 for CDMA 1X Iteration 6, 2014-12-29, end */
/* Added by m00312079 for CDMA 1X Iteration 6 2014-12-25 begin */
/*****************************************************************************
 函 数 名  : TAF_MMA_ProcCdmaCsqSetReq_PreProc
 功能描述  : 处理TAF信号强度上报设置请求
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年12月25日
    作    者   : m00312079
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 TAF_MMA_ProcCdmaCsqSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CDMACSQ_SET_REQ_STRU       *pstMmaCdmaCsqReqMsg = VOS_NULL_PTR;
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx       = VOS_NULL_PTR;
    NAS_MSCC_PIF_CDMACSQ_SET_PARA_STRU  stCdmaCsqSetPara;
    VOS_UINT8                           ucCtxIndex;

    pstMmaCdmaCsqReqMsg  = (TAF_MMA_CDMACSQ_SET_REQ_STRU *)pstMsg;

    /* 找不到未被使用的，回复失败 */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndCdmaCsqSetCnf(&(pstMmaCdmaCsqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE);
        return VOS_TRUE;
    }

    /* Modified by y00307564 for CDMA Iteration 8 2015-2-13 begin */
    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    TAF_MMA_SetOperCtx(pstMmaCdmaCsqReqMsg->stCtrl,
                       TAF_MMA_OPER_CDMACSQ_SET_REQ, ucCtxIndex);
    /* Modified by y00307564 for CDMA Iteration 8 2015-2-13 end */

    /*填充消息*/
    PS_MEM_SET(&stCdmaCsqSetPara, 0, sizeof(NAS_MSCC_PIF_CDMACSQ_SET_PARA_STRU));
    stCdmaCsqSetPara.ucTimeInterval = pstMmaCdmaCsqReqMsg->stCdmaCsqPara.ucTimeInterval;
    stCdmaCsqSetPara.ucRptThreshold = pstMmaCdmaCsqReqMsg->stCdmaCsqPara.ucRptThreshold;


    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();
    pstMmaOperCtx[ucCtxIndex].pPara = (TAF_MMA_CDMACSQ_PARA_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA, sizeof(TAF_MMA_CDMACSQ_PARA_STRU));

    if (VOS_NULL_PTR == pstMmaOperCtx[ucCtxIndex].pPara)
    {
        TAF_MMA_SndCdmaCsqSetCnf(&(pstMmaCdmaCsqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
        return VOS_TRUE;
    }

    if (VOS_TRUE != TAF_MMA_SndMsccCdmaCsqSetReq(stCdmaCsqSetPara))
    {
        TAF_MMA_SndCdmaCsqSetCnf(&(pstMmaCdmaCsqReqMsg->stCtrl),
                          TAF_MMA_APP_OPER_RESULT_FAILURE);

        PS_MEM_FREE(WUEPS_PID_MMA, pstMmaOperCtx[ucCtxIndex].pPara);
        pstMmaOperCtx[ucCtxIndex].pPara = VOS_NULL_PTR;

        TAF_MMA_ClearOperCtx(ucCtxIndex);

        return VOS_TRUE;
    }

    PS_MEM_CPY(pstMmaOperCtx[ucCtxIndex].pPara, &pstMmaCdmaCsqReqMsg->stCdmaCsqPara, sizeof(TAF_MMA_CDMACSQ_PARA_STRU));

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMsccCdmaCsqSetCnf_PreProc
 功能描述  : 处理XSD信号强度上报设置的处理结果
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年12月26日
    作    者   : m00312079
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMsccCdmaCsqSetCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_CDMACSQ_SET_CNF_STRU        *pstCdmaCsqCnf          = VOS_NULL_PTR;
    TAF_MMA_OPER_CTX_STRU                *pstMmaOperCtx          = VOS_NULL_PTR;
    TAF_MMA_CDMACSQ_PARA_STRU            *pstMmaCdmaCsqPara      = VOS_NULL_PTR;
    TAF_SDC_1X_SIG_QUALITY_INFO_STRU     *pstSdc1xSigQualityInfo = VOS_NULL_PTR;
    VOS_UINT8                             ucCtxIndex;
    TAF_MMA_APP_OPER_RESULT_ENUM_UINT32   enRslt;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();
    enRslt          = TAF_MMA_APP_OPER_RESULT_SUCCESS;

    pstCdmaCsqCnf   = (MSCC_MMA_CDMACSQ_SET_CNF_STRU *)pstMsg;

    if ( NAS_MSCC_PIF_CDMACSQ_SET_RESULT_SUCC != pstCdmaCsqCnf->enRslt )
    {
        enRslt = TAF_MMA_APP_OPER_RESULT_FAILURE;
    }

    /* Modified by y00307564 for CDMA Iteration 8 2015-2-13 begin */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_CDMACSQ_SET_REQ, &ucCtxIndex))
    {
    /* Modified by y00307564 for CDMA Iteration 8 2015-2-13 end */
        /* 进行CDMACSQ的设置回复 */
        TAF_MMA_SndCdmaCsqSetCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                                enRslt);

        if (TAF_MMA_APP_OPER_RESULT_SUCCESS == enRslt)
        {

            pstMmaCdmaCsqPara  = (TAF_MMA_CDMACSQ_PARA_STRU *)pstMmaOperCtx[ucCtxIndex].pPara;
            /*更新SDC的全局变量*/
            pstSdc1xSigQualityInfo                     = TAF_SDC_Get1xSigQualityInfo();

            pstSdc1xSigQualityInfo->enMode             = pstMmaCdmaCsqPara->enMode;
            pstSdc1xSigQualityInfo->ucTimeInterval     = pstMmaCdmaCsqPara->ucTimeInterval;
            pstSdc1xSigQualityInfo->ucRptThreshold     = pstMmaCdmaCsqPara->ucRptThreshold;
        }

        /* 清除CTX中内容  */
        TAF_MMA_ClearOperCtx(ucCtxIndex);
        PS_MEM_FREE(WUEPS_PID_MMA, pstMmaCdmaCsqPara);
    }
    else
    {
        MMA_ERRORLOG("TAF_MMA_RcvMsccCdmaCsqSetCnf_PreProc:ERROR");
    }

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvCdmaCsqQryReq_PreProc
 功能描述  : 查询信号强度
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年12月26日
    作    者   : m00312079
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvCdmaCsqQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CDMACSQ_QUERY_CNF_STRU       *pstCdmaCsqQryCnf       = VOS_NULL_PTR;
    TAF_MMA_CDMACSQ_QRY_REQ_STRU         *pstRcvMsg              = VOS_NULL_PTR;
    TAF_SDC_1X_SIG_QUALITY_INFO_STRU     *pstSdc1xSigQualityInfo = VOS_NULL_PTR;



    pstRcvMsg = (TAF_MMA_CDMACSQ_QRY_REQ_STRU *)pstMsg;

    pstCdmaCsqQryCnf = (TAF_MMA_CDMACSQ_QUERY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MMA,
                                                                                       sizeof(TAF_MMA_CDMACSQ_QUERY_CNF_STRU));
    if (VOS_NULL_PTR == pstCdmaCsqQryCnf)
    {
        MMA_ERRORLOG("TAF_MMA_RcvAtCdmaCsqQryReq_PreProc: ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }

    PS_MEM_SET(((VOS_UINT8 *)pstCdmaCsqQryCnf + VOS_MSG_HEAD_LENGTH),
               0x00,
               (sizeof(TAF_MMA_CDMACSQ_QUERY_CNF_STRU) - VOS_MSG_HEAD_LENGTH));

    /* 填写消息头部信息 */
    pstCdmaCsqQryCnf->ulReceiverPid          = pstRcvMsg->stCtrl.ulModuleId;
    pstCdmaCsqQryCnf->ulMsgName              = ID_TAF_MMA_CDMACSQ_QRY_CNF;

    PS_MEM_CPY(&pstCdmaCsqQryCnf->stCtrl, &pstRcvMsg->stCtrl, sizeof(TAF_MMA_CTRL_STRU));

    pstSdc1xSigQualityInfo                              = TAF_SDC_Get1xSigQualityInfo();

    pstCdmaCsqQryCnf->enMode                            = pstSdc1xSigQualityInfo->enMode;
    pstCdmaCsqQryCnf->ucTimeInterval                    = pstSdc1xSigQualityInfo->ucTimeInterval;
    pstCdmaCsqQryCnf->ucRptThreshold                    = pstSdc1xSigQualityInfo->ucRptThreshold;
    pstCdmaCsqQryCnf->stSigQualityInfo.cCdmaRssi        = pstSdc1xSigQualityInfo->cCdmaRssi;
    pstCdmaCsqQryCnf->stSigQualityInfo.cCdmaEcIo        = pstSdc1xSigQualityInfo->cCdmaEcIo;

    /* 发送消息 */
    PS_SEND_MSG(WUEPS_PID_MMA, pstCdmaCsqQryCnf);

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMsccCdmaCsqInd_PreProc
 功能描述  : 主动上报信号强度
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年12月26日
    作    者   : m00312079
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMsccCdmaCsqInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_CDMACSQ_IND_STRU            *pstCdmaCsqInd          = VOS_NULL_PTR;
    TAF_SDC_1X_SIG_QUALITY_INFO_STRU     *pstSdc1xSigQualityInfo = VOS_NULL_PTR;
    TAF_MMA_CDMA_CSQ_MODE_ENUM_UINT8      enMode;

    pstCdmaCsqInd = (MSCC_MMA_CDMACSQ_IND_STRU *)pstMsg;

    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, begin */
    /* 如果有no rf标志，则直接不处理 */
    if (VOS_FALSE == TAF_SDC_Get1xRfAvailFlag())
    {
        return VOS_TRUE;
    }
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, end */

    /* 更新全局变量 */
    pstSdc1xSigQualityInfo                 = TAF_SDC_Get1xSigQualityInfo();
    pstSdc1xSigQualityInfo->cCdmaRssi      = pstCdmaCsqInd->cCdmaRssi;
    pstSdc1xSigQualityInfo->cCdmaEcIo      = pstCdmaCsqInd->cCdmaEcIo;

    enMode = pstSdc1xSigQualityInfo->enMode;

    if (TAF_MMA_CDMA_CSQ_MODE_ENABLE == enMode)
    {
        /* 进行CDMACSQ的设置回复 */
        TAF_MMA_SndCdmaCsqInd(pstCdmaCsqInd->cCdmaRssi,pstCdmaCsqInd->cCdmaEcIo);
    }

    return VOS_TRUE;
}
/* Added by m00312079 for CDMA 1X Iteration 6 2014-12-25 end */



TAF_SDC_NETWORK_EXISTANCE_ENUM_UINT32 TAF_MMA_MapHrpdSrvStaToCdmaNetworkExistance(
    MSCC_MMA_HRPD_SERVICE_STATUS_ENUM_UINT32                enSrvSta
)
{
    if (MSCC_MMA_HRPD_SERVICE_STATUS_NORMAL_SERVICE == enSrvSta)
    {
        return TAF_SDC_HRPD_NETWORK_EXIST;
    }
    else
    {
        return TAF_SDC_HRPD_NETWORK_NOT_EXIST;
    }
}

/* Added by m00312079 for CDMA Iteration 12 2015-6-3 begin */
/*****************************************************************************
 函 数 名  : TAF_MMA_CovertSubMode_EvdoExist
 功能描述  : EVDO有服务时，获取当前的submode。
 输入参数  : TAF_SDC_SYS_SUBMODE_ENUM_UINT8      enPreSubMode
 输出参数  : 无
 返 回 值  : TAF_SDC_SYS_SUBMODE_ENUM_UINT8
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年06月03日
    作    者   : m00312079
    修改内容   : 新建
*****************************************************************************/
TAF_SDC_SYS_SUBMODE_ENUM_UINT8 TAF_MMA_CovertSubMode_EvdoExist(
    TAF_SDC_SYS_MODE_ENUM_UINT8                             enSysMode,
    MSCC_MMA_HRPD_SESSION_RELEASE_TYPE_ENUM_UINT32          enCurrSessionRelType
)
{
    TAF_SDC_SYS_SUBMODE_ENUM_UINT8                          enCurrSubMode;

    if (TAF_SDC_SYS_MODE_EVDO == enSysMode)
    {
        switch (enCurrSessionRelType)
        {
            case MSCC_MMA_SESSION_RELEASE_TYPE_A:
                enCurrSubMode = TAF_SDC_SYS_SUBMODE_EVDO_REL_A;
                break;

            default:
                enCurrSubMode = TAF_SDC_SYS_SUBMODE_EVDO_REL_0;
                break;
        }
    }
    else/* hybrid mode */
    {
        switch (enCurrSessionRelType)
        {
            case MSCC_MMA_SESSION_RELEASE_TYPE_A:
                enCurrSubMode = TAF_SDC_SYS_SUBMODE_HYBRID_EVDO_REL_A;
                break;

            default:
                enCurrSubMode = TAF_SDC_SYS_SUBMODE_HYBRID_EVDO_REL_0;
                break;
        }
    }

    return enCurrSubMode;

}
/* Added by m00312079 for CDMA Iteration 12 2015-6-3 end */

VOS_UINT32 TAF_MMA_RcvMsccHrpdServiceStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_HRPD_SERVICE_STATUS_IND_STRU                  *pstHrpdSrvStaInd = VOS_NULL_PTR;
    MSCC_MMA_HRPD_SERVICE_STATUS_ENUM_UINT32                enSrvSta;
    TAF_SDC_NETWORK_EXISTANCE_ENUM_UINT32                   enHrpdNetworkExist;
    TAF_SDC_SYS_MODE_ENUM_UINT8                             enSysMode;
    TAF_SDC_SYS_SUBMODE_ENUM_UINT8                          enSubMode;
    TAF_SDC_REG_STATUS_ENUM_UINT8                           enRegSta;
    TAF_PH_ACCESS_TECH_ENUM_UINT8                           enAct;
    pTafSdcSysmodServiceRegStaUpdateFunc                    pFuncTafSdcSysmodServiceRegStaUpdate;

    pstHrpdSrvStaInd = (MSCC_MMA_HRPD_SERVICE_STATUS_IND_STRU *)pstMsg;
    enSrvSta         = pstHrpdSrvStaInd->enSrvSta;
    enSysMode        = TAF_SDC_SYS_MODE_BUTT;
    enSubMode        = TAF_SDC_SYS_SUBMODE_BUTT;
    pFuncTafSdcSysmodServiceRegStaUpdate = VOS_NULL_PTR;

    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, begin */
    /* 如果有no rf标志，则直接不处理 */
    if (VOS_FALSE == TAF_SDC_GetHrpdRfAvailFlag())
    {
        return VOS_TRUE;
    }
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, end */

    /* report ^mode */
    enHrpdNetworkExist = TAF_MMA_MapHrpdSrvStaToCdmaNetworkExistance(enSrvSta);
    TAF_MMA_MapCdmaNetworkExistanceSrvStaToSysMode(enHrpdNetworkExist, &enSysMode, &enSubMode, &pFuncTafSdcSysmodServiceRegStaUpdate);

    if ( (TAF_SDC_SYS_MODE_SVLTE == enSysMode)
      || (TAF_SDC_SYS_MODE_LTE == enSysMode) )
    {
        return VOS_TRUE;
    }

    /* Added by m00312079 for CDMA Iteration 12 2015-6-2 begin */
    if (TAF_SDC_HRPD_NETWORK_EXIST == enHrpdNetworkExist)
    {
        if (VOS_TRUE == pstHrpdSrvStaInd->ucIsEhrpdSupport)
        {
            enSubMode = TAF_SDC_SYS_SUBMODE_EHRPD;
        }
        else
        {
            enSubMode = TAF_MMA_CovertSubMode_EvdoExist(enSysMode, pstHrpdSrvStaInd->enCurrSessionRelType);
        }
    }
    /* Added by m00312079 for CDMA Iteration 12 2015-6-2 end */

    Mma_ModeChgReport(enSysMode, enSubMode);


    /* TODO:判断当前为e/HRPD or NULL模式 */
    /* notify APS the service status, according to review, when ever there is a service */
    if (MSCC_MMA_HRPD_SERVICE_STATUS_NORMAL_SERVICE == pstHrpdSrvStaInd->enSrvSta)
    {
        if (VOS_TRUE == pstHrpdSrvStaInd->ucIsEhrpdSupport)
        {
            TAF_MMA_SndTafCLServiceStatusInd(MMA_TAF_CL_STATUS_EHRPD_NORMAL_SERVICE, pstHrpdSrvStaInd->ucIsNewSession);
        }
        else
        {
            TAF_MMA_SndTafCLServiceStatusInd(MMA_TAF_CL_STATUS_HRPD_NORMAL_SERVICE, pstHrpdSrvStaInd->ucIsNewSession);
        }
    }

    TAF_SDC_SetSysMode(enSysMode);
    TAF_SDC_SetSysSubMode(enSubMode);

    /* report +CGREG */
    /* when acquire, do not update +cgreg, only ^mode, so
      here only process in normal service and no service */
    if (MSCC_MMA_HRPD_SERVICE_STATUS_NORMAL_SERVICE == enSrvSta)
    {
        enRegSta = TAF_SDC_REG_REGISTERED_HOME_NETWORK;
    }
    else
    {
        enRegSta = TAF_SDC_REG_NOT_REGISTERED_SEARCHING;
    }

    TAF_MMA_ConvertSysSubModeToActForAtCmdCgreg(enSubMode, &enAct);
    TAF_MMA_ReportPsRegStatus(enRegSta, enAct);

    /* update srvstatus and domain */
    TAF_MMA_UpdataHrpdServiceStatusAndDomain(enSysMode);

    /* report ^srvst */
    TAF_MMA_ReprotCdmaServiceState(TAF_SDC_GetCsServiceStatus(), enSysMode);
    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvMsccHrpdOverheadMsgInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_SDC_SYS_MODE_ENUM_UINT8         enSysMode;
    /* Modified by l00324781 for CDMA Iteration 12, 2015-6-18, begin */
    TAF_SDC_SYS_SUBMODE_ENUM_UINT8      enSubMode;
    /* Modified by l00324781 for CDMA Iteration 12, 2015-6-18, begin */
    pTafSdcSysmodServiceRegStaUpdateFunc                    pFuncTafSdcSysmodServiceRegStaUpdate;

    /* Added by y00322978 for CDMA Iteration 17, 2015-7-16, begin */
    MSCC_MMA_HRPD_OVERHEAD_MSG_IND_STRU *pstRcvMsg = VOS_NULL_PTR;
    TAF_SDC_HRPD_SYS_INFO_STRU          *pstHrpdSysInfo = VOS_NULL_PTR;
    /* Added by y00322978 for CDMA Iteration 17, 2015-7-16, end */

    enSysMode                           = TAF_SDC_SYS_MODE_BUTT;
    enSubMode                           = TAF_SDC_SYS_SUBMODE_BUTT;
    pFuncTafSdcSysmodServiceRegStaUpdate = VOS_NULL_PTR;

    /* Added by y00322978 for CDMA Iteration 17, 2015-7-16, begin */
    pstRcvMsg = (MSCC_MMA_HRPD_OVERHEAD_MSG_IND_STRU *)pstMsg;
    pstHrpdSysInfo = TAF_SDC_GethrpdSysInfo();
    /* Added by y00322978 for CDMA Iteration 17, 2015-7-16, end */

    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, begin */
    /* 如果有no rf标志，则直接不处理 */
    if (VOS_FALSE == TAF_SDC_GetHrpdRfAvailFlag())
    {
        return VOS_TRUE;
    }
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, end */

    /* report ^mode*/
    /* todo conver to network existance */
    TAF_MMA_MapCdmaNetworkExistanceSrvStaToSysMode(TAF_SDC_HRPD_NETWORK_EXIST,
                                                   &enSysMode,
                                                   &enSubMode,
                                                   &pFuncTafSdcSysmodServiceRegStaUpdate);

    Mma_ModeChgReport(enSysMode, enSubMode);

    TAF_SDC_SetSysMode(enSysMode);
    TAF_SDC_SetSysSubMode(enSubMode);

    /* Added by y00322978 for CDMA Iteration 17, 2015-7-10, begin */
    /* 保存hrpd信息到本地*/
    pstHrpdSysInfo->ucColorCode         = pstRcvMsg->ucUATIColorCode;
    pstHrpdSysInfo->ucSubNetMask        = pstRcvMsg->ucSubNetMask;
    pstHrpdSysInfo->ulMcc               = pstRcvMsg->ulMcc;
    pstHrpdSysInfo->usBandClass         = pstRcvMsg->usBandClass;
    pstHrpdSysInfo->usFreq              = pstRcvMsg->usFreq;
    pstHrpdSysInfo->ulSector24          = pstRcvMsg->ulSector24;

    /* 可维可测 */
    TAF_MMA_SndOmCdmaStatusInfoReportInd();
    /* Added by y00322978 for CDMA Iteration 17, 2015-7-10, end */

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMsccHrpdSysAcqCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{


    return VOS_TRUE;
}


MMA_TAF_RAT_TYPE_ENUM_UINT32 TAF_MMA_GetPriCdmaSrvMode(
    TAF_SDC_SYS_MODE_ENUM_UINT8         enSysMode
)
{
    switch (enSysMode)
    {
        case TAF_SDC_SYS_MODE_CDMA_1X:
            return MMA_TAF_RAT_TYPE_1X;

        case TAF_SDC_SYS_MODE_EVDO:
            return MMA_TAF_RAT_TYPE_HRPD;

        case TAF_SDC_SYS_MODE_HYBRID:
            return MMA_TAF_RAT_TYPE_HRPD;

        default:
            /* do nothing */
            break;
    }

    return MMA_TAF_RAT_TYPE_NULL;
}
VOS_UINT32 TAF_MMA_RcvMscc1xSystemTimeInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_1X_SYSTEM_TIME_IND_STRU   *pst1xSystemTimeInd = VOS_NULL_PTR;

    pst1xSystemTimeInd = (MSCC_MMA_1X_SYSTEM_TIME_IND_STRU *)pstMsg;

    TAF_MMA_ProcessCSystemTime(&(pst1xSystemTimeInd->stSysTime));

    return VOS_TRUE;
}

/* Added by m00312079 for CDMA Iteration 10 2015-4-7 begin */
/*****************************************************************************
Function Name   :   TAF_MMA_RcvMsccCLSysAcqStartInd_PreProc
Description     :   Receive and process the ID_MSCC_MMA_CL_SYSTEM_ACQUIRE_START_IND
Input parameters:   ulEventType-----message ID +pid
                    pstMsg     -----message content
Outout parameters:  None
Return Value    :   VOS_TRUE------------the current event is processed finished


Modify History  :
1)  Date           : 2015-04-07
    Author         : m00312079
    Modify content : Create
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMsccCLSysAcqStartInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    #if 0
    MSCC_MMA_CL_SYSTEM_ACQUIRE_START_IND_STRU              *pstClSysAcqStartInd;

    pstClSysAcqStartInd = (MSCC_MMA_CL_SYSTEM_ACQUIRE_START_IND_STRU *)pstMsg;
    #endif
    /*TODO*/

    return VOS_TRUE;
}

/*****************************************************************************
Function Name   :   TAF_MMA_RcvMsccCLSysAcqEndInd_PreProc
Description     :   Receive and process the ID_MSCC_MMA_CL_SYSTEM_ACQUIRE_END_IND
Input parameters:   ulEventType-----message ID +pid
                    pstMsg     -----message content
Outout parameters:  None
Return Value    :   VOS_TRUE------------the current event is processed finished


Modify History  :
1)  Date           : 2015-04-07
    Author         : m00312079
    Modify content : Create
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMsccCLSysAcqEndInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    #if 0
    MSCC_MMA_CL_SYSTEM_ACQUIRE_END_IND_STRU                *pstClSysAcqEndInd;

    pstClSysAcqEndInd = (MSCC_MMA_CL_SYSTEM_ACQUIRE_END_IND_STRU *)pstMsg;
    #endif

    /*TODO*/

    return VOS_TRUE;
}
/* Added by m00312079 for CDMA Iteration 10 2015-4-7 end */


/* Added by z00316370 for UTS 2015-5-16 begin */
/*****************************************************************************
 函 数 名  : TAF_MMA_Rcv1xChanSetReq_PreProc
 功能描述  : MMA预处理中处理^1XCHAN设置命令
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年05月16日
    作    者   : z00316370
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_Rcv1xChanSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CFREQ_LOCK_SET_REQ_STRU    *pstCFreqLockSetReq     = VOS_NULL_PTR;
    TAF_MMA_CFREQ_LOCK_SET_PARA_STRU   *pstTafMmaCFreqLockPara = VOS_NULL_PTR;
    VOS_UINT32                          ulRslt;

    pstCFreqLockSetReq = (TAF_MMA_CFREQ_LOCK_SET_REQ_STRU *)pstMsg;

    /*  读取全局变量的值 */
    pstTafMmaCFreqLockPara = TAF_MMA_GetCFreqLockSetPara();

    /*  如果设置参数变化则写NV并NTF XSD/HSD */
    if (0 != PS_MEM_CMP(pstTafMmaCFreqLockPara,
                        &pstCFreqLockSetReq->stCFreqLockPara,
                        sizeof(TAF_MMA_CFREQ_LOCK_SET_PARA_STRU)))
    {
        #ifndef DMT
        /* wirte CFREQLOCK informations into NV */
        ulRslt = NV_Write(en_NV_Item_CFreqLock_CFG,
                          &pstCFreqLockSetReq->stCFreqLockPara,
                          sizeof(TAF_NVIM_CFREQ_LOCK_CFG_STRU));

        if (NV_OK != ulRslt)
        {
            TAF_MMA_Snd1xChanSetCnf(pstCFreqLockSetReq, VOS_ERR);
            return VOS_TRUE;
        }
        #endif

        /* 更新MMA保存的锁频全局变量 */
        PS_MEM_CPY(pstTafMmaCFreqLockPara,
                   &pstCFreqLockSetReq->stCFreqLockPara,
                   sizeof(TAF_MMA_CFREQ_LOCK_SET_PARA_STRU));

        /* 发送MMA_MSCC_CFREQ_LOCK_NTF */
        TAF_MMA_SndMsccCFreqLockNtf(pstTafMmaCFreqLockPara);

    }
    /* 发送TAF_MMA_1XCHAN_SET_CNF */
    TAF_MMA_Snd1xChanSetCnf(pstCFreqLockSetReq, VOS_OK);

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvQuery1xChanReq_PreProc
 功能描述  : MMA预处理中处理^CVER查询命令
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年05月16日
    作    者   : z00316370
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvQuery1xChanReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_1XCHAN_QUERY_REQ_STRU      *pstAppReqMsg;
    TAF_MMA_1XCHAN_QUERY_CNF_STRU      *pst1xChanCnf;

    pstAppReqMsg     = (TAF_MMA_1XCHAN_QUERY_REQ_STRU *)pstMsg;

    pst1xChanCnf = (TAF_MMA_1XCHAN_QUERY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MMA,
                                                                        sizeof(TAF_MMA_1XCHAN_QUERY_CNF_STRU));
    if (VOS_NULL_PTR == pst1xChanCnf)
    {
        MMA_ERRORLOG("TAF_MMA_RcvQuery1xChanReq_PreProc: ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }

    PS_MEM_SET(((VOS_UINT8 *)pst1xChanCnf + VOS_MSG_HEAD_LENGTH),
               0x00,
               (sizeof(TAF_MMA_1XCHAN_QUERY_CNF_STRU) - VOS_MSG_HEAD_LENGTH));

    /* 填写消息头部信息 */
    pst1xChanCnf->ulReceiverPid          = WUEPS_PID_AT;
    pst1xChanCnf->ulMsgName              = ID_TAF_MMA_1XCHAN_QUERY_CNF;
    pst1xChanCnf->stAtAppCtrl.usClientId = pstAppReqMsg->stCtrl.usClientId;
    pst1xChanCnf->stAtAppCtrl.ucOpId     = pstAppReqMsg->stCtrl.ucOpId;

    if (TAF_SDC_SYS_MODE_CDMA_1X == TAF_SDC_GetSysMode())
    {
        pst1xChanCnf->ulRslt             = VOS_OK;

        pst1xChanCnf->usChannel          = TAF_SDC_Get1xChan();
    }
    else
    {
        pst1xChanCnf->ulRslt             = VOS_ERR;
    }

    /* 发送消息 */
    PS_SEND_MSG(WUEPS_PID_MMA, pst1xChanCnf);

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvAtQueryCVER_PreProc
 功能描述  : MMA预处理中处理^CVER查询命令
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年05月16日
    作    者   : z00316370
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvQueryCVerReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CVER_QUERY_REQ_STRU        *pstAppReqMsg;
    TAF_MMA_CVER_QUERY_CNF_STRU        *pstCVerCnf;

    pstAppReqMsg     = (TAF_MMA_CVER_QUERY_REQ_STRU *)pstMsg;

    pstCVerCnf = (TAF_MMA_CVER_QUERY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MMA,
                                                                                       sizeof(TAF_MMA_CVER_QUERY_CNF_STRU));
    if (VOS_NULL_PTR == pstCVerCnf)
    {
        MMA_ERRORLOG("TAF_MMA_RcvQueryCVerReq_PreProc: ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }

    PS_MEM_SET(((VOS_UINT8 *)pstCVerCnf + VOS_MSG_HEAD_LENGTH),
               0x00,
               (sizeof(TAF_MMA_CVER_QUERY_CNF_STRU) - VOS_MSG_HEAD_LENGTH));

    /* 填写消息头部信息 */
    pstCVerCnf->ulReceiverPid          = WUEPS_PID_AT;
    pstCVerCnf->ulMsgName              = ID_TAF_MMA_CVER_QUERY_CNF;
    pstCVerCnf->stAtAppCtrl.usClientId = pstAppReqMsg->stCtrl.usClientId;
    pstCVerCnf->stAtAppCtrl.ucOpId     = pstAppReqMsg->stCtrl.ucOpId;

    if (TAF_SDC_SYS_MODE_CDMA_1X == TAF_SDC_GetSysMode())
    {
        pstCVerCnf->ulRslt             = VOS_OK;

        pstCVerCnf->enPrevInUse        = TAF_SDC_Get1xPrevInUse();
    }
    else
    {
        pstCVerCnf->ulRslt             = VOS_ERR;
    }

    /* 发送消息 */
    PS_SEND_MSG(WUEPS_PID_MMA, pstCVerCnf);

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvQueryStateReq_PreProc
 功能描述  : MMA预处理中处理^GETST查询命令
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年05月16日
    作    者   : z00316370
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvQueryStateReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_STATE_QUERY_REQ_STRU       *pstStateMsg         = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstStateMsg  = (TAF_MMA_STATE_QUERY_REQ_STRU *)pstMsg;

    /* 找不到未被使用的，回复失败 */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndGetStaQryCnf(&(pstStateMsg->stCtrl),
                                VOS_NULL_PTR,
                                TAF_MMA_APP_OPER_RESULT_FAILURE);

        return VOS_TRUE;
    }

    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    TAF_MMA_SetOperCtx(pstStateMsg->stCtrl,
                       TAF_MMA_OPER_GETSTA_QUERY_REQ,
                       ucCtxIndex);

    if (VOS_TRUE != TAF_MMA_SndMsccHandsetInfoQryReq(MMA_MSCC_HANDSET_INFO_TYPE_STATE))
    {
        TAF_MMA_SndGetStaQryCnf(&(pstStateMsg->stCtrl),
                                VOS_NULL_PTR,
                                TAF_MMA_APP_OPER_RESULT_FAILURE);

        TAF_MMA_ClearOperCtx(ucCtxIndex);

        return VOS_TRUE;
    }

    /* 启动保护定时器TI_TAF_MMA_WAIT_MSCC_STATE_QUERY_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_STATE_QUERY_CNF, TI_TAF_MMA_WAIT_MSCC_STATE_QUERY_CNF_LEN);

    return VOS_TRUE;
}



/*****************************************************************************
 函 数 名  : TAF_MMA_RcvQueryHighVerReq_PreProc
 功能描述  : MMA预处理中处理^CHIGHVER设置命令
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年05月16日
    作    者   : z00316370
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvQueryHighVerReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CHIGHVER_QUERY_REQ_STRU    *pstHVerMsg          = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstHVerMsg  = (TAF_MMA_CHIGHVER_QUERY_REQ_STRU *)pstMsg;

    /* 找不到未被使用的，回复失败 */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndCHVerQryCnf(&(pstHVerMsg->stCtrl),
                               TAF_MMA_APP_OPER_RESULT_FAILURE,
                               TAF_MMA_1X_CAS_P_REV_ENUM_BUTT);

        return VOS_TRUE;
    }

    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    TAF_MMA_SetOperCtx(pstHVerMsg->stCtrl,
                       TAF_MMA_OPER_CHVER_QUERY_REQ, ucCtxIndex);

    if (VOS_TRUE != TAF_MMA_SndMsccHandsetInfoQryReq(MMA_MSCC_HANDSET_INFO_TYPE_HVERSION))
    {
        TAF_MMA_SndCHVerQryCnf(&(pstHVerMsg->stCtrl),
                               TAF_MMA_APP_OPER_RESULT_FAILURE,
                               TAF_MMA_1X_CAS_P_REV_ENUM_BUTT);

        TAF_MMA_ClearOperCtx(ucCtxIndex);

        return VOS_TRUE;
    }

    /* 启动保护定时器TI_TAF_MMA_WAIT_MSCC_CHVER_QUERY_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_CHVER_QUERY_CNF, TI_TAF_MMA_WAIT_MSCC_CHVER_QUERY_CNF_LEN);

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMsccHandsetInfoQryCnf_PreProc
 功能描述  : 处理MSCC上报手机信息的处理结果
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年05月22日
    作    者   : z00316370
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMsccHandsetInfoQryCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_HANDSET_INFO_QRY_CNF_STRU   *pstInfoQryCnf          = VOS_NULL_PTR;

    pstInfoQryCnf   = (MSCC_MMA_HANDSET_INFO_QRY_CNF_STRU *)pstMsg;

    if ( MMA_MSCC_HANDSET_INFO_TYPE_STATE == pstInfoQryCnf->enInfoType)
    {
        /* 停定时器 */
        TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MSCC_STATE_QUERY_CNF);

        TAF_MMA_CasStateQry_Proc(pstInfoQryCnf->ucCasState,pstInfoQryCnf->ucCasSubSta);
    }
    else
    {
        /* 停定时器 */
        TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MSCC_CHVER_QUERY_CNF);

        TAF_MMA_HighVerQry_Proc(pstInfoQryCnf->enHighVer);
    }

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_CasStateQry_Proc
 功能描述  : 处理MSCC上报手机状态信息的处理结果
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年05月22日
    作    者   : z00316370
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID TAF_MMA_CasStateQry_Proc(
    VOS_UINT8                           ucCasState,
    VOS_UINT8                           ucCasSubSta
)
{
    TAF_MMA_OPER_CTX_STRU                                  *pstMmaOperCtx   = VOS_NULL_PTR;
    TAF_MMA_APP_OPER_RESULT_ENUM_UINT32                     enRslt;
    TAF_MMA_HANDSET_STATE_STRU                              stHandsetInfo;
    VOS_UINT8                                               ucCtxIndex;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();
    enRslt          = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    ucCtxIndex      = 0;

    PS_MEM_SET(&stHandsetInfo, 0x00, sizeof(TAF_MMA_HANDSET_STATE_STRU));

    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_GETSTA_QUERY_REQ, &ucCtxIndex))
    {
        /* 确认此判断逻辑 */
        if ((MSCC_MMA_1X_CAS_STATE_ENUM_BUTT == ucCasState)
         || (MSCC_MMA_CAS_STA_INVALID_SUBSTA == ucCasSubSta))
        {
            enRslt = TAF_MMA_APP_OPER_RESULT_FAILURE;
        }

        TAF_MMA_CasStateConvert_Proc(ucCasState, ucCasSubSta, &stHandsetInfo);

        TAF_MMA_SndGetStaQryCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                                &stHandsetInfo,
                                enRslt);

        /* 清除CTX中内容  */
        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }
    else
    {
        MMA_ERRORLOG("TAF_MMA_CasStateQry_Proc:ERROR when get SpecOper");
    }

    return;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_CasStateConvert_Proc
 功能描述  : 处理状态转换
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年05月22日
    作    者   : z00316370
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID TAF_MMA_CasStateConvert_Proc(
    VOS_UINT8                           ucCasState,
    VOS_UINT8                           ucCasSubSta,
    TAF_MMA_HANDSET_STATE_STRU         *pstState
)
{
    MSCC_MMA_1X_CAS_STATE_ENUM_UINT8    enCasSta;

    pstState->ucCasState = ucCasState;

    enCasSta = (MSCC_MMA_1X_CAS_STATE_ENUM_UINT8)ucCasState;

    /*
    //INIT substate
    CDMA_SYS_INIT_SYS_DETERM_SUB_STATE   (0)  MSCC_MMA_1X_CAS_INIT_DETERM_SUB_STATE  (0x01)
    CDMA_SYS_PILOT_CH_ACQ_SUB_STATE Pilot(1)  MSCC_MMA_1X_CAS_PILOT_CH_ACQ_SUB_STATE (0x02)
    CDMA_SYS_SYNC_CH_ACQ_SUB_STATE       (2)  MSCC_MMA_1X_CAS_SYNC_CH_ACQ_SUB_STATE  (0x03)
    CDMA_SYS_TIMING_CHNG_SUB_STATE       (3)  MSCC_MMA_1X_CAS_TIMING_CHNG_SUB_STATE  (0x04)

    //IDLE substae
    CDMA_SYS_IDLE_AWAKE_SUB_STATE        (0)  MSCC_MMA_1X_CAS_IDLE_SUB_STATE         (0x10)
    CDMA_SYS_IDLE_SLEEP_SUB_STATE        (1)

    //ACCESS substae
    CDMA_SYS_UPDATE_OVERHEAD_SUB_STATE          (0) MSCC_MMA_1X_CAS_UPDATE_OVERHEAD_SUB_STATE          (0x20)
    CDMA_SYS_MOB_STATION_ORIG_ATTEMPT_SUB_STATE (1) MSCC_MMA_1X_CAS_MOB_STATION_ORIG_ATTEMPT_SUB_STATE (0x21)
    CDMA_SYS_PAG_RESP_SUB_STATE                 (2) MSCC_MMA_1X_CAS_PAG_RESP_SUB_STATE                 (0x22)
    CDMA_SYS_MOB_STATION_ORDR_RESP_SUB_STATE    (3) MSCC_MMA_1X_CAS_MOB_STATION_ORDR_RESP_SUB_STATE    (0x23)
    CDMA_SYS_REG_ACCESS_SUB_STATE               (4) MSCC_MMA_1X_CAS_REG_ACCESS_SUB_STATE               (0x24)
    CDMA_SYS_MOB_STATION_MSG_TRANS_SUB_STATE    (5) MSCC_MMA_1X_CAS_MOB_STATION_MSG_TRANS_SUB_STATE    (0x25)
    CDMA_SYS_PACA_CANCEL_SUB_STATE              (6)

    //TRAFFIC CAS sub state
    CDMA_SYS_TCH_INIT_SUB_STATE Traffic channel    (0)  MSCC_MMA_1X_CAS_TCH_INIT_SUB_STATE      (0x30)
    CDMA_SYS_WAIT_FOR_ORDER_SUB_STATE              (1)  MSCC_MMA_1X_CALL_STATE_WAIT_FOR_ORDER   (0x00)
    CDMA_SYS_WAIT_FOR_MOB_STATION_ANSWER_SUB_STATE (2)  MSCC_MMA_1X_CALL_STATE_WAIT_FOR_ANSWER  (0x01)
    CDMA_SYS_CONVERSATION_SUB_STATE                (3)  MSCC_MMA_1X_CALL_STATE_CONVERSATION     (0x02)
    CDMA_SYS_REL_SUB_STATE                         (4)  MSCC_MMA_1X_CAS_REL_SUB_STATE           (0x31)

    //NO SERVICE
    CDMA_SYS_WAIT_FOR_SERVICE_SUB_STATE   (0)   MSCC_MMA_1X_CAS_WAIT_FOR_SERVICE_SUB_STATE (0x40)
    */

    switch (enCasSta)
    {
        case MSCC_MMA_1X_CAS_INIT_STATE:
            pstState->ucCasSubSta = (ucCasSubSta & 0x0F) - 0x01;
            break;

        case MSCC_MMA_1X_CAS_IDLE_STATE:
        case MSCC_MMA_1X_CAS_ACCESS_STATE:
        case MSCC_MMA_1X_CAS_NO_SERVICE_STATE:
            pstState->ucCasSubSta = (ucCasSubSta & 0x0F);
            break;

        case MSCC_MMA_1X_CAS_TCH_STATE:
            /*cas 上报*/
            if (MSCC_MMA_1X_CAS_TCH_INIT_SUB_STATE == ucCasSubSta)
            {
                pstState->ucCasSubSta = (ucCasSubSta & 0x0F);
            }
            else if (MSCC_MMA_1X_CAS_REL_SUB_STATE == ucCasSubSta)
            {
                pstState->ucCasSubSta = (ucCasSubSta & 0x0F) + 0x03 ;
            }
            /*xcc 上报*/
            else if (MSCC_MMA_CAS_STA_INVALID_SUBSTA != ucCasSubSta)
            {
                pstState->ucCasSubSta = ucCasSubSta + 1;
            }
            else
            {
                pstState->ucCasSubSta = ucCasSubSta;
            }
            break;

        default:
            pstState->ucCasSubSta = ucCasSubSta;
            break;
    }

    return;
}


/*****************************************************************************
 函 数 名  : TAF_MMA_HighVerQry_Proc
 功能描述  : 处理MSCC上报手机版本信息的处理结果
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年05月22日
    作    者   : z00316370
    修改内容   : 新生成函数
*****************************************************************************/
VOS_VOID TAF_MMA_HighVerQry_Proc(
    MSCC_MMA_1X_CAS_P_REV_ENUM_UINT8    enHighVer
)
{
    TAF_MMA_OPER_CTX_STRU                                  *pstMmaOperCtx   = VOS_NULL_PTR;
    TAF_MMA_APP_OPER_RESULT_ENUM_UINT32                     enRslt;
    TAF_MMA_1X_CAS_P_REV_ENUM_UINT8                         enHVer;
    VOS_UINT8                                               ucCtxIndex;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();
    enRslt          = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    ucCtxIndex      = 0;

    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_CHVER_QUERY_REQ, &ucCtxIndex))
    {
        enHVer = (TAF_MMA_1X_CAS_P_REV_ENUM_UINT8)enHighVer;

        /* 确认此判断逻辑 */
        if (TAF_MMA_1X_CAS_P_REV_ENUM_BUTT == enHVer)
        {
            enRslt = TAF_MMA_APP_OPER_RESULT_FAILURE;
        }

        TAF_MMA_SndCHVerQryCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                               enHVer,
                               enRslt);

        /* 清除CTX中内容  */
        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }
    else
    {
        MMA_ERRORLOG("TAF_MMA_HighVerQry_Proc:ERROR when get SpecOper");
    }

    return;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvTiWaitMsccStateQryCnfExpired_PreProc
 功能描述  : MMA预处理中处理^GETST查询命令超时
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年05月16日
    作    者   : z00316370
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvTiWaitMsccStateQryCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_OPER_GETSTA_QUERY_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_GETSTA_QUERY_REQ, &ucCtxIndex))
    {
        TAF_MMA_SndGetStaQryCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                                VOS_NULL_PTR,
                                TAF_MMA_APP_OPER_RESULT_FAILURE);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvTiWaitMsccHVerQryCnfExpired_PreProc
 功能描述  : MMA预处理中处理^CHIGHVER查询命令超时
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年05月16日
    作    者   : z00316370
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvTiWaitMsccHVerQryCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_OPER_CHVER_QUERY_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_CHVER_QUERY_REQ, &ucCtxIndex))
    {
        TAF_MMA_SndCHVerQryCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                               TAF_MMA_APP_OPER_RESULT_FAILURE,
                               TAF_MMA_1X_CAS_P_REV_ENUM_BUTT);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;
}

/* Added by z00316370 for UTS 2015-5-16 end */
/* Added by y00322978 for CDMA Iteration 17, 2015-7-4, begin */

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvHsmHrpdSysInfoInd_PreProc
 功能描述  : MMA收到HSM的hrpd的sys info上报处理，用于可维可测
 输入参数  : VOS_UINT32                          ulEventType,
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月4日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvHsmHrpdSysInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_SDC_HRPD_SYS_INFO_STRU         *pSdchrpdSysInfo;
    HSM_MMA_HRPD_SYS_INFO_IND_STRU     *pRcvMsg = VOS_NULL_PTR;

    pRcvMsg = (HSM_MMA_HRPD_SYS_INFO_IND_STRU*)pstMsg;


    pSdchrpdSysInfo = TAF_SDC_GethrpdSysInfo();

    pSdchrpdSysInfo->ulSessionSeed = pRcvMsg->ulSessionSeed;
    pSdchrpdSysInfo->ucSessionStatus = TAF_MMA_MapHsmSessionStatus(pRcvMsg->enSessionStatus);
    PS_MEM_CPY(pSdchrpdSysInfo->aucCurUATI, pRcvMsg->aucCurUATI, NAS_OM_MMA_UATI_OCTET_LENGTH);

    TAF_MMA_SndOmCdmaStatusInfoReportInd();
    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvHsmHardwareSysInfoInd_PreProc
 功能描述  : 保存HSM发来的HARDWARE INFO到SDC
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月6日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32  TAF_MMA_RcvHsmHardwareSysInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_SDC_SYS_INFO_STRU               *pstSysInfo;
    HSM_MMA_HARDWARE_SYS_INFO_IND_STRU *pRcvMsg = VOS_NULL_PTR;

    pRcvMsg = (HSM_MMA_HARDWARE_SYS_INFO_IND_STRU*)pstMsg;
    pstSysInfo = TAF_SDC_GetSysInfo();

    PS_MEM_CPY(pstSysInfo->aucMeId, pRcvMsg->aucMeId, NAS_OM_MMA_MEID_OCTET_NUM);
    pstSysInfo->ulEsn = pRcvMsg->ulEsn;
    pstSysInfo->enModemId = pRcvMsg->enModemId;

    TAF_MMA_SndOmCdmaStatusInfoReportInd();
    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvOmCdmaStatusInfoReportReq_PreProc
 功能描述  : 收到OM的cdma可维可测查询请求消息的处理
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月4日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvOmCdmaStatusInfoReportReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    OM_NAS_MMA_CDMA_STATUS_INFO_REPORT_REQ_STRU  *pRcvMsg = VOS_NULL_PTR;
    VOS_UINT32                                    ulRptPeriod;

    pRcvMsg = (OM_NAS_MMA_CDMA_STATUS_INFO_REPORT_REQ_STRU*)pstMsg;
    ulRptPeriod = 0;

    if(OM_NAS_MMA_CDMA_STATUS_REPORT_CMD_NOT_REPORT == pRcvMsg->enCmd)
    {
        /*Stop timer if the timer is running*/
        if (TAF_MMA_TIMER_STATUS_RUNING == TAF_MMA_GetTimerStatus(TI_TAF_MMA_FREQUENCE_OF_QRY_CDMA_SYS_INFO))
        {
            TAF_MMA_StopTimer(TI_TAF_MMA_FREQUENCE_OF_QRY_CDMA_SYS_INFO);
        }

    }
    else if (OM_NAS_MMA_CDMA_STATUS_REPORT_CMD_FOREVER == pRcvMsg->enCmd)
    {
        /* Check the rpt period*/
        if (0 == pRcvMsg->ulRptPeriod)
        {
            ulRptPeriod = TI_TAF_MMA_DEFAULT_FREQUENCE_OF_QRY_CDMA_SYS_INFO_LEN;
        }
        /*start timer,and save rpt period */
        TAF_MMA_StartTimer(TI_TAF_MMA_FREQUENCE_OF_QRY_CDMA_SYS_INFO, ulRptPeriod);
        TAF_SDC_SetCdmaStatusReportPeriod(ulRptPeriod);
    }
    else
    {
    }

    TAF_MMA_SndOmCdmaStatusReportCnf(NAS_OM_MMA_CDMA_STATUS_REPORT_RESULT_SUCC, pRcvMsg);
    TAF_MMA_SndOmCdmaStatusInfoReportInd();
    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvTiCdmaSysInfoReport_PreProc
 功能描述  : 定时上报cdma sysinfo给om
 输入参数  :  VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月10日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvTiCdmaSysInfoReport_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_SndOmCdmaStatusInfoReportInd();
    TAF_MMA_StartTimer(TI_TAF_MMA_FREQUENCE_OF_QRY_CDMA_SYS_INFO,
                       TAF_SDC_GetCdmaStatusReportPeriod());

    return VOS_TRUE;

}
/* Added by y00322978 for CDMA Iteration 17, 2015-7-4, end */

/* Added by y00322978 for CDMA Iteration 17, 2015-7-9, begin */
/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMscc1XSidInd_PreProc
 功能描述  : 处理mscc发过来的sid主动上报消息
 输入参数  :  VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月9日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMscc1XSidInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_1X_SID_IND_STRU           *pstRcvMsg =  VOS_NULL_PTR;
    TAF_SDC_1X_SYS_INFO_STRU           *pstSdc1xSysInfo;

    pstRcvMsg = (MSCC_MMA_1X_SID_IND_STRU *)pstMsg;

    pstSdc1xSysInfo = TAF_SDC_Get1xSysInfo();

    if ((VOS_TRUE         == pstSdc1xSysInfo->ulSidEnable)
     && (pstRcvMsg->usSid != pstSdc1xSysInfo->lSid))
    {
        TAF_MMA_ReportCSid((VOS_INT32)pstRcvMsg->usSid);
    }

    /*更新SID信息*/
    pstSdc1xSysInfo->lSid = (VOS_INT32)pstRcvMsg->usSid;

    return VOS_TRUE;
}
/*****************************************************************************
 函 数 名  : TAF_MMA_RcvQuitCallBackReq_PreProc
 功能描述  : AT QCCB请求处理
 输入参数  :  VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月9日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvQuitCallBackInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_QUIT_CALLBACK_SET_REQ_STRU *pstRcvMsg = VOS_NULL_PTR;

    pstRcvMsg = (TAF_MMA_QUIT_CALLBACK_SET_REQ_STRU *)pstMsg;

    TAF_MMA_SndMsccQuitCallBackNtf();

    /*直接回复cnf消息 */
    TAF_MMA_SndQuitCallBackCnf(&(pstRcvMsg->stCtrl), TAF_ERR_NO_ERROR);

    return VOS_TRUE;
}
/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMscc1XEmcCallBackNtf_PreProc
 功能描述  : xsd call back  透传上报
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月14日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMscc1XEmcCallBackNtf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_1X_EMC_CALL_BACK_IND_STRU *pstRcvMsg =  VOS_NULL_PTR;
    TAF_SDC_1X_SYS_INFO_STRU           *pstSdc1xSysInfo;

    pstRcvMsg = (MSCC_MMA_1X_EMC_CALL_BACK_IND_STRU *)pstMsg;
    pstSdc1xSysInfo = TAF_SDC_Get1xSysInfo();

    /*更新当前是否处于callback模式信息*/
    pstSdc1xSysInfo->ulIsInEmcCallBack = (VOS_UINT32)(pstRcvMsg->enCallBackState);

    TAF_MMA_Report1xEmcCallBack((VOS_UINT32)pstRcvMsg->enCallBackState);


    return VOS_TRUE;
}
/*****************************************************************************
 函 数 名  : TAF_MMA_RcvSetCSidListReq_PreProc
 功能描述  : AT CSIDLIST req
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月13日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvSetCSidListReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MMA_MSCC_SET_CSIDLIST_REQ_STRU     *pstSndMsg = VOS_NULL_PTR;
    TAF_MMA_CSIDLIST_SET_REQ_STRU      *pstRcvMsg = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    /* 申请内存  */
    pstSndMsg = (MMA_MSCC_SET_CSIDLIST_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_MMA,
                           sizeof(MMA_MSCC_SET_CSIDLIST_REQ_STRU) - VOS_MSG_HEAD_LENGTH);


    if ( VOS_NULL_PTR == pstSndMsg )
    {
        TAF_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvSetCSidListReq_PreProc:ERROR: Memory Alloc Error for pMsg");
        return VOS_ERR;
    }
    pstRcvMsg = (TAF_MMA_CSIDLIST_SET_REQ_STRU *)pstMsg;
    PS_MEM_SET((VOS_INT8*)pstSndMsg + VOS_MSG_HEAD_LENGTH, 0,
                     sizeof(MMA_MSCC_SET_CSIDLIST_REQ_STRU) - VOS_MSG_HEAD_LENGTH);


    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    if (VOS_TRUE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {

        TAF_MMA_SetOperCtx(pstRcvMsg->stCtrl,
                       TAF_MMA_OPER_SET_CSIDLIST_REQ, ucCtxIndex);

        pstSndMsg->stMsgHeader.ulReceiverCpuId  = VOS_LOCAL_CPUID;
        pstSndMsg->stMsgHeader.ulReceiverPid    = UEPS_PID_MSCC;
        pstSndMsg->stMsgHeader.ulSenderCpuId    = VOS_LOCAL_CPUID;
        pstSndMsg->stMsgHeader.ulSenderPid      = WUEPS_PID_MMA;
        pstSndMsg->stMsgHeader.ulLength         = sizeof(MMA_MSCC_SET_CSIDLIST_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
        pstSndMsg->stMsgHeader.ulMsgName        = ID_MMA_MSCC_SET_CSIDLIST_REQ;
        PS_MEM_CPY(&pstSndMsg->stSidWhiteList, &pstRcvMsg->stWhiteSidInfo, sizeof(TAF_MMA_OPER_LOCK_WHITE_SID_STRU));
        /* 调用VOS发送原语 */
        PS_SEND_MSG( WUEPS_PID_MMA, pstSndMsg );

    }
    /* 找不到未被使用的，回复失败 */
    else
    {
        TAF_MMA_SndSetCSidListCnf(&(pstRcvMsg->stCtrl), TAF_ERR_ERROR);
    }
    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMsccSetCSidListCnf
 功能描述  : 收到mscc 关于csidlist set的结果消息处理
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月13日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMsccSetCSidListCnf(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_SET_CSIDLIST_CNF_STRU     *pstRcvMsg = VOS_NULL_PTR;
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;
    TAF_ERROR_CODE_ENUM_UINT32          enErrorCode;

    pstMmaOperCtx = TAF_MMA_GetOperCtxAddr();

    pstRcvMsg = (MSCC_MMA_SET_CSIDLIST_CNF_STRU *)pstMsg;

    if (VOS_TRUE == pstRcvMsg->ulRslt)
    {
        enErrorCode = TAF_ERR_NO_ERROR;
    }
    else
    {
        enErrorCode = TAF_ERR_ERROR;
    }
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_SET_CSIDLIST_REQ, &ucCtxIndex))
    {
        /*  回复CSIDLIST set cnf*/
        TAF_MMA_SndSetCSidListCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl), enErrorCode);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;
}
/*****************************************************************************
 函 数 名  : TAF_MMA_RcvSetCSidReq_PreProc
 功能描述  : 设置csid 是否可以主动上报
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月14日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvSetCSidReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CSID_SET_REQ_STRU          *pstRcvMsg = VOS_NULL_PTR;
    TAF_MMA_CSID_SET_CNF_STRU          *pstSndMsg = VOS_NULL_PTR;
    TAF_SDC_1X_SYS_INFO_STRU           *pstSdc1xSysInfo;

    pstRcvMsg = (TAF_MMA_CSID_SET_REQ_STRU *)pstMsg;
    pstSdc1xSysInfo = TAF_SDC_Get1xSysInfo();

    pstSndMsg = (TAF_MMA_CSID_SET_CNF_STRU *)PS_ALLOC_MSG(WUEPS_PID_MMA,
                           sizeof(TAF_MMA_CSID_SET_CNF_STRU) - VOS_MSG_HEAD_LENGTH);

    if ( VOS_NULL_PTR == pstSndMsg )
    {
        TAF_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvSetCSidReq_PreProc:ERROR: Memory Alloc Error for pMsg");
        return VOS_FALSE;
    }

    PS_MEM_SET((VOS_INT8*)pstSndMsg + VOS_MSG_HEAD_LENGTH, 0,
                     sizeof(TAF_MMA_CSID_SET_CNF_STRU) - VOS_MSG_HEAD_LENGTH);

    /*保存sid主动上报标志位到MMA本地*/
    pstSdc1xSysInfo->ulSidEnable = pstRcvMsg->ulEnable;

    /* 根据输入参数填充TAF_MMA_CSID_SET_CNF_STRU */
    pstSndMsg->ulSenderPid       = WUEPS_PID_MMA;
    pstSndMsg->ulReceiverPid     = WUEPS_PID_AT;
    pstSndMsg->ulMsgName         = ID_TAF_MMA_CSID_SET_CNF;
    pstSndMsg->enErrorCode       = TAF_ERR_NO_ERROR;
    PS_MEM_CPY(&pstSndMsg->stCtrl, &pstRcvMsg->stCtrl, sizeof(TAF_MMA_CTRL_STRU));

    /* 发送消息 */
    PS_SEND_MSG(WUEPS_PID_MMA, pstSndMsg);

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvQryEmcCallBackMode_PreProc
 功能描述  : 查询当前是否在紧急呼callback模式下
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月14日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvQryEmcCallBackMode_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_1X_EMC_CALL_BACK_QRY_REQ_STRU           *pstRcvMsg;
    TAF_MMA_1X_EMC_CALL_BACK_QRY_CNF_STRU           *pstSndMsg;
    TAF_SDC_1X_SYS_INFO_STRU                        *pstSdc1xSysInfo;

    pstRcvMsg       = (TAF_MMA_1X_EMC_CALL_BACK_QRY_REQ_STRU *)pstMsg;
    pstSdc1xSysInfo = TAF_SDC_Get1xSysInfo();
    pstSndMsg = (TAF_MMA_1X_EMC_CALL_BACK_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MMA,
                                                                        sizeof(TAF_MMA_1X_EMC_CALL_BACK_QRY_CNF_STRU));
    if (VOS_NULL_PTR == pstSndMsg)
    {
        MMA_ERRORLOG("TAF_MMA_RcvQryEmcCallBackMode_PreProc: ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }

    PS_MEM_SET(((VOS_UINT8 *)pstSndMsg + VOS_MSG_HEAD_LENGTH),
               0x00,
               (sizeof(TAF_MMA_1X_EMC_CALL_BACK_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH));

    /* 填写消息头部信息 */
    pstSndMsg->ulReceiverPid          = WUEPS_PID_AT;
    pstSndMsg->ulMsgName              = ID_TAF_MMA_1X_EMC_CALL_BACK_QRY_CNF;
    PS_MEM_CPY(&pstSndMsg->stCtrl, &pstRcvMsg->stCtrl, sizeof(TAF_MMA_CTRL_STRU));
    pstSndMsg->ucIsInCallBackMode     = pstSdc1xSysInfo->ulIsInEmcCallBack;
    pstSndMsg->enErrorCode            = TAF_ERR_NO_ERROR;

    /* 发送消息 */
    PS_SEND_MSG(WUEPS_PID_MMA, pstSndMsg);

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvXcc1xCallStateInd_PreProc
 功能描述  : XCC 1x call state 上报消息处理
 输入参数  : VOS_UINT32                          ulEventType,
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年7月22日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvXcc1xCallStateInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    XCC_MMA_1X_CALL_STATE_IND_STRU     *pstRcvMsg = VOS_NULL_PTR;
    NAS_OM_1X_CALL_STATE_ENUM_UINT8     enPreXcallState;

    pstRcvMsg = (XCC_MMA_1X_CALL_STATE_IND_STRU *)pstMsg;
    enPreXcallState = TAF_SDC_GetXCallState();

    switch (pstRcvMsg->enCallState)
    {
        case XCC_MMA_1X_CALL_STATE_IDLE:
            TAF_SDC_SetXCaLLState(NAS_OM_1X_CALL_STATE_IDLE);
            break;
        case XCC_MMA_1X_CALL_STATE_ORIG_CALL:
            TAF_SDC_SetXCaLLState(NAS_OM_1X_CALL_STATE_ORIGNALING_CALL);
            break;
        case XCC_MMA_1X_CALL_STATE_INCOMMING_CALL:
            TAF_SDC_SetXCaLLState(NAS_OM_1X_CALL_STATE_INCOMING_CALL);
            break;
        case XCC_MMA_1X_CALL_STATE_CONVERSATION:
            TAF_SDC_SetXCaLLState(NAS_OM_1X_CALL_STATE_CONVERSATION);
            break;
        default :
            TAF_SDC_SetXCaLLState(NAS_OM_1X_CALL_STATE_IDLE);
    }

    /* CALL STATE 变化后上报一下*/
    if (TAF_SDC_GetXCallState() != enPreXcallState)
    {
        TAF_MMA_SndOmCdmaStatusInfoReportInd();
    }

    return VOS_TRUE;
}

/* Added by y00322978 for CDMA Iteration 17, 2015-7-9, end */


VOS_UINT32 TAF_MMA_RcvMsccUeStateInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_UE_STATE_IND_STRU         *pstUeStateInd;

    pstUeStateInd       = (MSCC_MMA_UE_STATE_IND_STRU*)pstMsg;

    /* 将当前的UE状态存在SDC全局变量中 */
    TAF_SDC_SetCdmaCurUeState((TAF_SDC_1X_UE_MAIN_STATUS_ENUM_UINT8)pstUeStateInd->ucUeMainState);

    /* 后续有可能給TAF内部PID发送STATE IND */

    return VOS_TRUE;
}

#endif


VOS_UINT32 TAF_MMA_RcvMsccSystemAcquireStartInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_SYSTEM_ACQUIRE_START_IND_STRU                 *pstSysAcqStartInd    = VOS_NULL_PTR;
    TAF_MMA_PLMN_SELECTION_INFO_STRU                        stTafPlmnSelectInfo;

    pstSysAcqStartInd = (MSCC_MMA_SYSTEM_ACQUIRE_START_IND_STRU *)pstMsg;
    PS_MEM_SET(&stTafPlmnSelectInfo, 0, sizeof(TAF_MMA_PLMN_SELECTION_INFO_STRU));

    /* 填充上报事件 */
    stTafPlmnSelectInfo.ulPlmnSelectRlst = TAF_PH_PLMN_SELECTION_RESULT_BUTT;
    stTafPlmnSelectInfo.ulPlmnSelectFlag = TAF_PH_PLMN_SELECTION_START;

    if (MSCC_MMA_ACQ_SYS_TYPE_3GPP == pstSysAcqStartInd->enSysType)
    {
        if (VOS_TRUE == TAF_MMA_IsPlmnSelectInfoNeedRpt())
        {
            TAF_MMA_SndPlmnSelectionInd(&stTafPlmnSelectInfo);
        }
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvMsccSystemAcquireEndInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_SYSTEM_ACQUIRE_END_IND_STRU                   *pstSysAcqEndInd;
    TAF_MMA_PLMN_SELECTION_INFO_STRU                        stTafPlmnSelectInfo;

    pstSysAcqEndInd = (MSCC_MMA_SYSTEM_ACQUIRE_END_IND_STRU*)pstMsg;

    if (MSCC_MMA_ACQ_SYS_TYPE_3GPP == pstSysAcqEndInd->enAcqSysType)
    {
        PS_MEM_SET(&stTafPlmnSelectInfo, 0, sizeof(TAF_MMA_PLMN_SELECTION_INFO_STRU));

        /* RoamingBroker功能使能则向SIM卡发送Location事件 */
        if (VOS_TRUE == MN_MMA_GetRoamingBrokerFlg())
        {
            if (( MSCC_MMA_SYSTEM_ACQUIRE_RESULT_LIMITED_SERVICE == pstSysAcqEndInd->enResult)
             || ( MSCC_MMA_SYSTEM_ACQUIRE_RESULT_NO_SERVICE      == pstSysAcqEndInd->enResult))
            {
                NAS_MMA_ReportLociStatus();
            }
        }
        /* 开机搜网，一个网络都没有搜索到后，UE处于无服务，而由于开机初始化时
           服务状态也是无服务，因此通过服务状态改变无法报location status事件，
           需要在此处报一次,有AT&T认证用例验证此场景 */
        else
        {
            if ((VOS_FALSE == g_StatusContext.ucIsReportedLocStatus)
             && ( MSCC_MMA_SYSTEM_ACQUIRE_RESULT_NO_SERVICE == pstSysAcqEndInd->enResult))
            {
                NAS_MMA_ReportLociStatus();
            }
        }

        stTafPlmnSelectInfo.ulPlmnSelectRlst = TAF_MMA_ConvertServiceTypeToAtType(pstSysAcqEndInd->enResult);

        if (TAF_PH_PLMN_SELECTION_RESULT_INTERRUPT == stTafPlmnSelectInfo.ulPlmnSelectRlst)
        {
            stTafPlmnSelectInfo.ulPlmnSelectFlag = TAF_PH_PLMN_SELECTION_INTERRUPT;
        }
        else
        {
            stTafPlmnSelectInfo.ulPlmnSelectFlag = TAF_PH_PLMN_SELECTION_END;
        }

        if (VOS_TRUE == TAF_MMA_IsPlmnSelectInfoNeedRpt())
        {
            TAF_MMA_SndPlmnSelectionInd(&stTafPlmnSelectInfo);
        }
    }
    else
    {
        /* 当前无服务的时候，如果是DO搜网，则是通知APS当前DO无服务,如果是DO+LTE搜网，则是通知APS当前DO+LTEs无服务  */
        if (MSCC_MMA_SYSTEM_ACQUIRE_RESULT_NORMAL_SERVICE != pstSysAcqEndInd->enResult)
        {
            if (MSCC_MMA_ACQ_SYS_TYPE_DO_LTE == pstSysAcqEndInd->enAcqSysType)
            {
                TAF_MMA_SndTafCLServiceStatusInd(MMA_TAF_CL_STATUS_DO_LTE_NO_SERVICE, VOS_FALSE);
            }

            if (MSCC_MMA_ACQ_SYS_TYPE_DO == pstSysAcqEndInd->enAcqSysType)
            {
                TAF_MMA_SndTafCLServiceStatusInd(MMA_TAF_CL_STATUS_DO_NO_SERVICE, VOS_FALSE);
            }
        }

    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvCopsFormatTypeSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_COPS_FORMAT_TYPE_SET_REQ_STRU                  *pstCopsFormatTypeReq = VOS_NULL_PTR;
    MN_APP_REQ_MSG_STRU                                    *pRcvMsg = VOS_NULL_PTR;

    pRcvMsg         = (MN_APP_REQ_MSG_STRU*)pstMsg;

    pstCopsFormatTypeReq = (TAF_MMA_COPS_FORMAT_TYPE_SET_REQ_STRU *)pRcvMsg->aucContent;

    TAF_SDC_SetCopsFormatType(pstCopsFormatTypeReq->ucCopsFormatType);

    return VOS_TRUE;
}



VOS_UINT32 TAF_MMA_RcvUsimStubSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_USIM_STUB_SET_REQ_STRU     *pstUsimStubReq = VOS_NULL_PTR;
    MN_APP_REQ_MSG_STRU                *pRcvMsg        = VOS_NULL_PTR;
    USIMM_READFILE_CNF_STRU            *pstUsimMsg     = VOS_NULL_PTR;
    VOS_CHAR                           *pucFilePathStr = VOS_NULL_PTR;
    VOS_UINT32                          ulPathLength;
    VOS_UINT32                          ulTotalLen;

    pRcvMsg        = (MN_APP_REQ_MSG_STRU*)pstMsg;
    pstUsimStubReq = (TAF_MMA_USIM_STUB_SET_REQ_STRU *)pRcvMsg->aucContent;

    ulTotalLen = pstUsimStubReq->ucTotalNum * pstUsimStubReq->usEfLen;

    pstUsimMsg = (USIMM_READFILE_CNF_STRU*)VOS_AllocMsg(WUEPS_PID_USIM, (sizeof(USIMM_READFILE_CNF_STRU) - VOS_MSG_HEAD_LENGTH) + ulTotalLen);

    if(VOS_NULL_PTR == pstUsimMsg)
    {
        return VOS_FALSE;
    }

    pstUsimMsg->stCmdResult.ulSenderCpuId     = VOS_LOCAL_CPUID;
    pstUsimMsg->stCmdResult.ulReceiverCpuId   = VOS_LOCAL_CPUID;
    pstUsimMsg->stCmdResult.ulLength          = (sizeof(USIMM_READFILE_CNF_STRU) - VOS_MSG_HEAD_LENGTH);
    pstUsimMsg->stCmdResult.ulReceiverPid     = WUEPS_PID_MMA;
    pstUsimMsg->stCmdResult.ulSenderPid       = WUEPS_PID_USIM;
    pstUsimMsg->stCmdResult.enMsgName         = USIMM_READFILE_CNF;
    pstUsimMsg->stCmdResult.ulErrorCode       = USIMM_SW_OK;
    pstUsimMsg->stCmdResult.ulResult          = VOS_OK;
    pstUsimMsg->stCmdResult.ulSendPara        = 0;
    pstUsimMsg->stCmdResult.enApptype         = pstUsimStubReq->ulApptype;
    pstUsimMsg->usEfLen                       = pstUsimStubReq->usEfLen;
    pstUsimMsg->ucTotalNum                    = pstUsimStubReq->ucTotalNum;

    /* 文件Id转文件路径 */
    if (VOS_OK != USIMM_ChangeDefFileToPath(pstUsimStubReq->usEfId, &pucFilePathStr))
    {
        return VOS_FALSE;
    }

    /* 文件路径长度保护 */
    ulPathLength = VOS_StrLen(pucFilePathStr);
    if (0 == ulPathLength)
    {
         return VOS_FALSE;
    }

    /* FILE PATH */
    pstUsimMsg->stFilePath.ulPathLen = ulPathLength;
    PS_MEM_CPY(pstUsimMsg->stFilePath.acPath, pucFilePathStr, ulPathLength + 1);

    PS_MEM_CPY(pstUsimMsg->aucEf, pstUsimStubReq->aucEf, ulTotalLen);

    PS_SEND_MSG(WUEPS_PID_USIM, pstUsimMsg);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvRefreshStubSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_REFRESH_STUB_SET_REQ_STRU  *pstRefreshStubReq = VOS_NULL_PTR;
    MN_APP_REQ_MSG_STRU                *pRcvMsg           = VOS_NULL_PTR;
    USIMM_STKREFRESH_IND_STRU          *pstUsimRefreshInd = VOS_NULL_PTR;
    VOS_UINT32                          ulTotalLen;
    VOS_UINT8                           i;

    pRcvMsg           = (MN_APP_REQ_MSG_STRU*)pstMsg;
    pstRefreshStubReq = (TAF_MMA_REFRESH_STUB_SET_REQ_STRU *)pRcvMsg->aucContent;

    ulTotalLen = 0;

    if (0 != pstRefreshStubReq->ucTotalNum)
    {
        ulTotalLen = sizeof(USIMM_STKREFRESH_IND_STRU) * (pstRefreshStubReq->ucTotalNum - 1);
    }

    pstUsimRefreshInd = (USIMM_STKREFRESH_IND_STRU*)PS_ALLOC_MSG(MAPS_PIH_PID, ((sizeof(USIMM_STKREFRESH_IND_STRU)-VOS_MSG_HEAD_LENGTH) + ulTotalLen));

    if(VOS_NULL_PTR == pstUsimRefreshInd)
    {
        return VOS_TRUE;
    }

    PS_MEM_SET((VOS_INT8*)pstUsimRefreshInd + VOS_MSG_HEAD_LENGTH, 0, ((sizeof(USIMM_STKREFRESH_IND_STRU) - VOS_MSG_HEAD_LENGTH) + ulTotalLen));

    pstUsimRefreshInd->stIndHdr.enMsgName       = USIMM_STKREFRESH_IND;
    pstUsimRefreshInd->stIndHdr.ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstUsimRefreshInd->stIndHdr.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstUsimRefreshInd->stIndHdr.ulLength        = (sizeof(USIMM_STKREFRESH_IND_STRU) - VOS_MSG_HEAD_LENGTH);
    pstUsimRefreshInd->stIndHdr.ulReceiverPid   = pstRefreshStubReq->ulReceivePid;
    pstUsimRefreshInd->stIndHdr.ulSenderPid     = MAPS_PIH_PID;
    pstUsimRefreshInd->enRefreshType            = pstRefreshStubReq->usRefreshFileType;
    pstUsimRefreshInd->usEfNum                  = pstRefreshStubReq->ucTotalNum;

    for (i = 0; i < pstRefreshStubReq->ucTotalNum; i++)
    {
        pstUsimRefreshInd->astEfId[i].usFileId  = pstRefreshStubReq->ausEfId[i];    /* [false alarm]:通过动态申请内存的结果  */
        pstUsimRefreshInd->astEfId[i].enAppType = USIMM_GUTL_APP;                   /* [false alarm]:通过动态申请内存的结果  */

    }

    PS_SEND_MSG(MAPS_PIH_PID, pstUsimRefreshInd);

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvCSServiceDisconnExpired_PreProc
 功能描述  : 等待cs 业务结束的超时处理
 输入参数  : ulEventType
             pstMsg

 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年2月15日
    作    者   : c00318887
    修改内容   : 新增函数
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvCSServiceDisconnExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    if (VOS_TRUE == TAF_SDC_GetCsEccExistFlg())
    {
        /* 还存在CS 业务，则继续启动定时器等待CS业务结束后关机 */
        TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_CS_SERVICE_DISCONN, TI_TAF_MMA_WAIT_CS_SERVICE_DISCONN_LEN);
    }
    else
    {
        /* 发送内部的卡状态改变指示消息，触发关机 */
        TAF_MMA_SndInterUsimChangeInd();
    }

    return VOS_TRUE;

}


VOS_UINT32 TAF_MMA_RcvWaitReadSimFilesExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                        *pstMsg
)
{
    TAF_MMA_ClearAllWaitSimFilesCnfFlg();
    return VOS_TRUE;

}



VOS_UINT32 TAF_MMA_RcvAutoReselStubSetReq_PreProc(
    VOS_UINT32                                              ulEventType,
    struct MsgCB                                           *pstMsg
)
{
    MN_APP_REQ_MSG_STRU                                    *pRcvMsg = VOS_NULL_PTR;
    TAF_MMA_AUTO_RESEL_STUB_SET_REQ_STRU                   *pstRefreshStubReq = VOS_NULL_PTR;

    pRcvMsg           = (MN_APP_REQ_MSG_STRU*)pstMsg;
    pstRefreshStubReq = (TAF_MMA_AUTO_RESEL_STUB_SET_REQ_STRU *)pRcvMsg->aucContent;

    TAF_MMA_SndMsccAutoReselSetReq(pstRefreshStubReq->ucActiveFlg);

    return VOS_TRUE;
}


#if (FEATURE_MULTI_MODEM == FEATURE_ON)

VOS_UINT32 TAF_MMA_RcvMsccPsServiceConnStatusInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_PS_SERVICE_CONN_STATUS_IND_STRU                *pstPsServiceConnStatusInd = VOS_NULL_PTR;
    TAF_MTC_SRV_CONN_STATE_INFO_STRU                        stMmaMtcConnStInfo;
    TAF_CTRL_STRU                                           stCtrl;
    MSCC_MMC_PS_SIGNALING_TYPE_ENUM_UINT8                   enPsSigType;

    pstPsServiceConnStatusInd = (MSCC_MMA_PS_SERVICE_CONN_STATUS_IND_STRU *)pstMsg;
    PS_MEM_SET(&stMmaMtcConnStInfo, 0, sizeof(TAF_MTC_SRV_CONN_STATE_INFO_STRU));
    PS_MEM_SET(&stCtrl, 0x0, sizeof(stCtrl));

    enPsSigType               = pstPsServiceConnStatusInd->enPsSigType;

    /* 填写CTL头 */
    TAF_API_CTRL_HEADER(&stCtrl, WUEPS_PID_MMA, 0, 0);

    switch(enPsSigType)
    {
    #if (FEATURE_ON == FEATURE_LTE)
        case MSCC_MMC_PS_SIGNALING_TYPE_LTE:

            TAF_SDC_SetEpsServiceConnStatusFlag(pstPsServiceConnStatusInd->ucPsServiceConnStatusFlag);

            stMmaMtcConnStInfo.bitOpEpsSrv          = VOS_TRUE;
            stMmaMtcConnStInfo.enEpsSrvConnState    = pstPsServiceConnStatusInd->ucPsServiceConnStatusFlag;
            break;
    #endif

        case MSCC_MMC_PS_SIGNALING_TYPE_GU:

            TAF_SDC_SetPsServiceConnStatusFlag(pstPsServiceConnStatusInd->ucPsServiceConnStatusFlag);

            stMmaMtcConnStInfo.bitOpPsSrv           = VOS_TRUE;
            stMmaMtcConnStInfo.enPsSrvConnState     = pstPsServiceConnStatusInd->ucPsServiceConnStatusFlag;
            break;

        default:
            MMA_WARNINGLOG("TAF_MMA_RcvMsccPsServiceConnStatusInd_PreProc():WORNING:sig type is invalid.");

            return VOS_TRUE;
    }

    /* 将连接状态报到MTC */
    MTC_SetModemServiceConnState(&stCtrl, &stMmaMtcConnStInfo);

    return VOS_TRUE;
}
#endif



#if (FEATURE_ON == FEATURE_IMS)

VOS_UINT32 TAF_MMA_RcvTafImsSrvInfoNotify_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_IMS_SRV_INFO_NOTIFY_STRU              *pstRcvMsg = VOS_NULL_PTR;

    pstRcvMsg   = (TAF_MMA_IMS_SRV_INFO_NOTIFY_STRU *)pstMsg;

    TAF_MMA_SndMsccImsSrvInfoNotify(pstRcvMsg->ucImsCallFlg);

    return VOS_TRUE;

}


VOS_UINT32 TAF_MMA_RcvTafImsSwitchQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_IMS_SWITCH_QRY_REQ_STRU    *pstSetReq = VOS_NULL_PTR;
    TAF_SDC_IMS_SWITCH_STATE_ENUM_UINT8 enImsSwitchState;

    pstSetReq = (TAF_MMA_IMS_SWITCH_QRY_REQ_STRU *)pstMsg;

    if (VOS_TRUE != TAF_SDC_GetImsSupportFlag())
    {
        enImsSwitchState = TAF_SDC_IMS_SWITCH_STATE_OFF;
    }
    else
    {
        enImsSwitchState = TAF_SDC_IMS_SWITCH_STATE_ON;
    }

    TAF_MMA_SndImsSwitchQryCnf(&pstSetReq->stCtrl, (VOS_UINT8)enImsSwitchState);

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvTafVoiceDomainSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_VOICE_DOMAIN_SET_REQ_STRU  *pstSetReq;
    VOS_UINT8                           ucImsVoiceFlag;
    VOS_UINT32                          ulImsSupportFlag;
    TAF_SDC_VOICE_DOMAIN_ENUM_UINT32    enVoiceDomain;

    pstSetReq           = (TAF_MMA_VOICE_DOMAIN_SET_REQ_STRU *)pstMsg;
    ucImsVoiceFlag      = TAF_SDC_GetVoiceCallOnImsSupportFlag();
    ulImsSupportFlag    = TAF_SDC_GetImsSupportFlag();
    enVoiceDomain       = TAF_SDC_GetVoiceDomain();

    /* IMS语音业务功能关闭或者IMS能力不支持的情况下，不允许设置PS_ONLY */
    if ((VOS_FALSE == ucImsVoiceFlag)
     || (VOS_FALSE == ulImsSupportFlag))
    {
        if (TAF_MMA_VOICE_DOMAIN_IMS_PS_ONLY == pstSetReq->enVoiceDomain)
        {
            /* 给APP回复设置失败 */
            TAF_MMA_SndVoiceDomainSetCnf(&pstSetReq->stCtrl,
                                         TAF_MMA_APP_OPER_RESULT_REJECT,
                                         TAF_ERR_IMS_NOT_SUPPORT);

            return VOS_TRUE;
        }
    }

    /* IMS语音业务过程中，不允许设置CS_ONLY */
    if (VOS_TRUE == TAF_SDC_GetImsCallExistFlg())
    {
        if (TAF_MMA_VOICE_DOMAIN_CS_ONLY == pstSetReq->enVoiceDomain)
        {
            /* 给APP回复设置失败 */
            TAF_MMA_SndVoiceDomainSetCnf(&pstSetReq->stCtrl,
                                         TAF_MMA_APP_OPER_RESULT_REJECT,
                                         TAF_ERR_IMS_SERVICE_EXIST);

            return VOS_TRUE;
        }
    }

    /* 优选域设置有发生变化，则通知MMC */
    if (enVoiceDomain != pstSetReq->enVoiceDomain)
    {
        /* 在飞行模式下面不给MMC发送UE能力，只更新上下文和NV */
        if (TAF_PH_MODE_FULL == TAF_SDC_GetCurPhoneMode())
        {
            /* 给MSCC发送VOICE DOMAIN变化指示 */
            TAF_MMA_SndMsccVoiceDomainChangeInd((NAS_MSCC_PIF_VOICE_DOMAIN_ENUM_UINT32)pstSetReq->enVoiceDomain);
        }

        /* 更新语音优选域的上下文和NV */
        if (VOS_TRUE != TAF_MMA_UpdataVoiceDomain((TAF_SDC_VOICE_DOMAIN_ENUM_UINT32)pstSetReq->enVoiceDomain))
        {
            TAF_MMA_SndVoiceDomainSetCnf(&pstSetReq->stCtrl,
                                         TAF_MMA_APP_OPER_RESULT_FAILURE,
                                         TAF_ERR_UNSPECIFIED_ERROR);
            return VOS_TRUE;
        }

    }

    /* 给APP回复设置成功 */
    TAF_MMA_SndVoiceDomainSetCnf(&pstSetReq->stCtrl,
                                 TAF_MMA_APP_OPER_RESULT_SUCCESS,
                                 TAF_ERR_NO_ERROR);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvTafVoiceDomainQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_VOICE_DOMAIN_QRY_REQ_STRU  *pstSetReq;
    TAF_SDC_VOICE_DOMAIN_ENUM_UINT32    enVoiceDomain;

    pstSetReq       = (TAF_MMA_VOICE_DOMAIN_QRY_REQ_STRU *)pstMsg;
    enVoiceDomain   = TAF_SDC_GetVoiceDomain();

    TAF_MMA_SndVoiceDomainQryCnf(&pstSetReq->stCtrl,
                                 (TAF_MMA_VOICE_DOMAIN_ENUM_UINT32)enVoiceDomain);

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvMsccImsSwitchStateInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_IMS_SWITCH_STATE_IND_STRU  *pstStateInd = VOS_NULL_PTR;

    pstStateInd = (MSCC_MMA_IMS_SWITCH_STATE_IND_STRU *)pstMsg;

    TAF_SDC_SetCurImsSwitchState((TAF_SDC_IMS_SWITCH_STATE_ENUM_UINT8)pstStateInd->enImsSwitch);

#if (FEATURE_MULTI_MODEM == FEATURE_ON)
    /* 通知MTC IMSA的开关机状态 */
    TAF_MMA_SndMtcImsaStateInd((MTC_MODEM_POWER_STATE_ENUM_UINT8)pstStateInd->enImsSwitch);
#endif

    return VOS_TRUE;
}

#endif

/*lint -restore */



#if ( FEATURE_ON == FEATURE_LTE )

VOS_UINT32 TAF_MMA_RcvMsccLmmCellSignReportInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_SDC_CTX_STRU                               *pstSdcCtx     = VOS_NULL_PTR;
    MSCC_MMA_LMM_CELL_SIGN_INFO_REPORT_IND_STRU    *pstRcvMsg          = VOS_NULL_PTR;

    pstRcvMsg        = (MSCC_MMA_LMM_CELL_SIGN_INFO_REPORT_IND_STRU*)pstMsg;
    pstSdcCtx   = TAF_SDC_GetSdcCtx();

    pstSdcCtx->stNetworkInfo.stRssiValue.ucChannalQual = TAF_PH_BER_UNKNOWN;
    pstSdcCtx->stNetworkInfo.stRssiValue.u.st4GCellSignInfo.sRssi = pstRcvMsg->stLteRssi.sRssi;
    pstSdcCtx->stNetworkInfo.stRssiValue.u.st4GCellSignInfo.sRsd  = pstRcvMsg->stLteRssi.sRsd;
    pstSdcCtx->stNetworkInfo.stRssiValue.u.st4GCellSignInfo.sRsrp = pstRcvMsg->stLteRssi.sRsrp;
    pstSdcCtx->stNetworkInfo.stRssiValue.u.st4GCellSignInfo.sRsrq = pstRcvMsg->stLteRssi.sRsrq;
    pstSdcCtx->stNetworkInfo.stRssiValue.u.st4GCellSignInfo.lSINR = pstRcvMsg->stLteRssi.lSINR;

    pstSdcCtx->stNetworkInfo.stRssiValue.u.st4GCellSignInfo.stCQI.usRI      = pstRcvMsg->stLteRssi.stCQI.usRI;
    pstSdcCtx->stNetworkInfo.stRssiValue.u.st4GCellSignInfo.stCQI.ausCQI[0] = pstRcvMsg->stLteRssi.stCQI.ausCQI[0];
    pstSdcCtx->stNetworkInfo.stRssiValue.u.st4GCellSignInfo.stCQI.ausCQI[1] = pstRcvMsg->stLteRssi.stCQI.ausCQI[1];

    TAF_MMA_GetRssiLevelFromLteRssi(pstRcvMsg->stLteRssi.sRssi, &(pstSdcCtx->stNetworkInfo.stRssiValue.ucRssiValue));

    pstSdcCtx->stNetworkInfo.stRssiValue.enRat = TAF_SDC_SYS_MODE_LTE;

    /*RAT 改变暂不上报，等注册结果后上报*/
    if (TAF_SDC_SYS_MODE_LTE != TAF_SDC_GetAppSysMode())
    {
        return VOS_TRUE;
    }

    /* RAT相同时，更新APP中的信号强度 */
    PS_MEM_CPY(&(pstSdcCtx->stAppNetworkInfo.stRssiValue), &(pstSdcCtx->stNetworkInfo.stRssiValue), sizeof(TAF_SDC_RSSI_VALUE_STRU));

    /* RXLEV需要上报给AT,RXQUALITY不需要上报给AT */
    if (NAS_MSCC_PIF_SCELL_MEAS_TYPE_RXLEV == (pstRcvMsg->unMeasReportType.ucMeasReportType & NAS_MSCC_PIF_SCELL_MEAS_TYPE_RXLEV))
    {
        TAF_MMA_SndAtRssiInd();
    }

    return VOS_TRUE;
}
#endif
VOS_UINT32 TAF_MMA_RcvMsccRssiInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                          ulI;

    MSCC_MMA_RSSI_IND_STRU              *pMsg;
    TAF_SDC_CTX_STRU                   *pstSdcCtx = VOS_NULL_PTR;
    TAF_SDC_SYS_MODE_ENUM_UINT8         enAppSysMode;

#if (FEATURE_ON == FEATURE_DSDS)
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, begin */
    if (VOS_FALSE == TAF_SDC_Get3GppRfAvailFlag())
    {
        return VOS_TRUE;
    }
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, end */
#endif

    pMsg            = (MSCC_MMA_RSSI_IND_STRU*)pstMsg;
    enAppSysMode    = TAF_SDC_GetAppSysMode();
    pstSdcCtx       = TAF_SDC_GetSdcCtx();


    /*CERSSI 信息保存移至SDC */
    ulI = 0;
    pstSdcCtx->stNetworkInfo.stRssiValue.ucChannalQual = pMsg->astRssi[ulI].ucBer;

    /* RSCP/ECIO/RSSI存在如下的对应关系：RSCP=ECIO+RSSI*/
    pstSdcCtx->stNetworkInfo.stRssiValue.sUeRfPower   = pMsg->sUeRfPower;
    pstSdcCtx->stNetworkInfo.stRssiValue.usCellDlFreq = pMsg->usCellDlFreq;
    pstSdcCtx->stNetworkInfo.stRssiValue.usCellUlFreq = pMsg->usCellUlFreq;

    if (TAF_SDC_SYS_MODE_GSM == pMsg->enRat)
    {
        MMA_GetRssiLevelFromValue(pMsg->astRssi[ulI].sRssiValue,
                                 &(pstSdcCtx->stNetworkInfo.stRssiValue.ucRssiValue));

        pstSdcCtx->stNetworkInfo.stRssiValue.u.st2GCellSignInfo.sRssiValue = pMsg->astRssi[ulI].sRssiValue;
    }
    else
    {
        pstSdcCtx->stNetworkInfo.stRssiValue.u.st3GCellSignInfo.sRscpValue = pMsg->astRssi[ulI].sRscpValue;
        pstSdcCtx->stNetworkInfo.stRssiValue.u.st3GCellSignInfo.sEcioValue = pMsg->astRssi[ulI].sRscpValue - pMsg->astRssi[ulI].sRssiValue;

        MMA_GetRssiLevelFromValue(pMsg->astRssi[ulI].sRscpValue,
                             &(pstSdcCtx->stNetworkInfo.stRssiValue.ucRssiValue));
    }

    pstSdcCtx->stNetworkInfo.stRssiValue.enRat = pMsg->enRat;

    /* 如果RAT不同，不需要更新APP中的信号强度，也不需要上报给AT */
    if (enAppSysMode != pMsg->enRat)
    {
        return VOS_TRUE;
    }

    /* RAT相同时，更新APP中的信号强度 */
    PS_MEM_CPY(&(pstSdcCtx->stAppNetworkInfo.stRssiValue), &(pstSdcCtx->stNetworkInfo.stRssiValue), sizeof(TAF_SDC_RSSI_VALUE_STRU));

    /* RXLEV需要上报给AT,RXQUALITY不需要上报给AT */
    if(NAS_MSCC_PIF_SCELL_MEAS_TYPE_RXLEV == (pMsg->unMeasReportType.ucMeasReportType & NAS_MSCC_PIF_SCELL_MEAS_TYPE_RXLEV))
    {
        TAF_MMA_SndAtRssiInd();
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvMsccSrchedPlmnInfoInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_SRCHED_PLMN_INFO_IND_STRU *pstSrchedPlmnInfoInd = VOS_NULL_PTR;
    TAF_SDC_CURC_RPT_CTRL_STRU         *pstCurcRptCtrl       = VOS_NULL_PTR;

    pstSrchedPlmnInfoInd = (MSCC_MMA_SRCHED_PLMN_INFO_IND_STRU *) pstMsg;

    /*  功能开启后，上报 */
    pstCurcRptCtrl = TAF_SDC_GetCurcRptCtrl();
    if (VOS_TRUE == TAF_SDC_CheckRptCmdStatus(pstCurcRptCtrl->aucRptCfg,
                                              TAF_SDC_CMD_RPT_CTRL_BY_CURC,
                                              TAF_SDC_RPT_CMD_SRCHEDPLMNINFO))
    {
        TAF_MMA_SndSrchedPlmnInfoInd(pstSrchedPlmnInfoInd);
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvPlmnAutoReselReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
#if (FEATURE_ON == FEATURE_DSDS)
    TAF_MMA_PLMN_AUTO_RESEL_REQ_STRU   *pstPlmnReselMsg = VOS_NULL_PTR;
    TAF_MMA_TIMER_STATUS_ENUM_UINT8     enWaitSrvAcqCnfTimerStatus;

    pstPlmnReselMsg = (TAF_MMA_PLMN_AUTO_RESEL_REQ_STRU *)pstMsg;

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    if (VOS_TRUE == TAF_SDC_IsConfigCLMode())
    {
        /* C模式下直接上报不支持*/
        TAF_MMA_SndPlmnAutoReselCnf(&pstPlmnReselMsg->stCtrl, TAF_ERR_CME_OPT_NOT_SUPPORTED);

        return VOS_TRUE;
    }
#endif


    enWaitSrvAcqCnfTimerStatus = TAF_MMA_GetTimerStatus(TI_TAF_MMA_WAIT_MSCC_SRV_ACQ_CNF);

    /* 如果存在业务触发的搜网，直接回复失败 */
    if (TAF_MMA_TIMER_STATUS_RUNING == enWaitSrvAcqCnfTimerStatus)
    {
        /* 上报APP当前状态事件 */
        TAF_MMA_SndPlmnAutoReselCnf(&pstPlmnReselMsg->stCtrl, TAF_ERR_NO_RF);

        return VOS_TRUE;
    }
#endif

    return VOS_FALSE;
}
VOS_UINT32 TAF_MMA_RcvPlmnSpecialSelReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_PLMN_SPECIAL_SEL_REQ_STRU  *pstPlmnUserSelMsg = VOS_NULL_PTR;
    VOS_UINT8                           ucError;
    TAF_PLMN_ID_STRU                    stPlmn;
    VOS_UINT32                          ulRet;

#if (FEATURE_ON == FEATURE_DSDS)
    TAF_MMA_TIMER_STATUS_ENUM_UINT8     enWaitSrvAcqCnfTimerStatus;
#endif

    pstPlmnUserSelMsg = (TAF_MMA_PLMN_SPECIAL_SEL_REQ_STRU *)pstMsg;

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    if (VOS_TRUE == TAF_SDC_IsConfigCLMode())
    {
        /* C模式下直接上报不支持*/
        TAF_MMA_SndPlmnSpecialSelCnf(&pstPlmnUserSelMsg->stCtrl, TAF_ERR_CME_OPT_NOT_SUPPORTED);

        return VOS_TRUE;
    }
#endif



#if (FEATURE_ON == FEATURE_DSDS)
    enWaitSrvAcqCnfTimerStatus = TAF_MMA_GetTimerStatus(TI_TAF_MMA_WAIT_MSCC_SRV_ACQ_CNF);

    /* 如果存在业务触发的搜网，直接回复失败 */
    if (TAF_MMA_TIMER_STATUS_RUNING == enWaitSrvAcqCnfTimerStatus)
    {
        /* 上报APP当前状态事件 */
        TAF_MMA_SndPlmnSpecialSelCnf(&pstPlmnUserSelMsg->stCtrl, TAF_ERR_NO_RF);

        return VOS_TRUE;
    }
#endif

    /* 在CL模式时候，不支持AT进行选网操作,直接返回ERROR */
    if (VOS_TRUE == TAF_MMA_IsPowerOnCLInterWork())
    {
        TAF_MMA_SndPlmnSpecialSelCnf(&pstPlmnUserSelMsg->stCtrl, TAF_ERR_ERROR);
        return VOS_TRUE;
    }

    /* 转化用户输入PLMN字符为PLMN ID */
    ulRet = MMA_FormatPlmnStr2PlmnId(&stPlmn,
                                      pstPlmnUserSelMsg->stPlmnUserSel.ucFormatType,
                                      pstPlmnUserSelMsg->stPlmnUserSel.usOperNameLen,
                                      pstPlmnUserSelMsg->stPlmnUserSel.ucOperName);
    if( VOS_TRUE == ulRet)
    {
        MMA_PlmnId2NasStyle(&stPlmn);

        /* 通过原有的TAF_PH_EVT_PLMN_SEL_CNF消息把指定搜结果发送给AT */

        ucError            = TAF_ERR_NO_ERROR;

        ucError = MMA_CheckUsimStatusForPlmnSel();
        if (TAF_ERR_NO_ERROR != ucError)
        {
            /*错误处理*/
            TAF_MMA_SndPlmnSpecialSelCnf(&pstPlmnUserSelMsg->stCtrl, ucError);
            return VOS_TRUE;
        }

        /*增加ACCESSMODE选择*/
        if (pstPlmnUserSelMsg->stPlmnUserSel.enAccessMode > TAF_MMA_RAT_BUTT)
        {
            /*错误处理*/
            TAF_MMA_SndPlmnSpecialSelCnf(&pstPlmnUserSelMsg->stCtrl, TAF_ERR_PARA_ERROR);

            return VOS_TRUE;
        }

        /* 入口参数合法性检查 */
        if ( Sta_IsInvalidOpId (pstPlmnUserSelMsg->stCtrl.ucOpId))
        {
            MMA_WARNINGLOG("TAF_MMA_RcvPlmnSpecialSelReq_PreProc():WARNING:parameter invalid");
            return VOS_TRUE;
        }

        /* svlte nv开启允许关机状态设置cops */
        if (TAF_PH_MODE_FULL != TAF_SDC_GetCurPhoneMode())
        {
            (VOS_VOID)TAF_MMA_RcvPlmnSelectionReq_PowerOff(pstPlmnUserSelMsg,
                                                           stPlmn,
                                                           pstPlmnUserSelMsg->stPlmnUserSel.enAccessMode,
                                                           TAF_PLMN_RESELETION_MANUAL);
            return VOS_TRUE;
        }


        /* PLMN_SEL状态输入条件检查异常*/
        if (VOS_TRUE != TAF_MMA_IsEnablePlmnSelect())
        {
            /* 如果不是ENABLE状态，返回错误 */
            if (TAF_MMA_PLMN_MODE_ENUM_DISABLE == TAF_MMA_GetPlmnMode_Refresh())
            {
                ucError = TAF_ERR_NET_SEL_MENU_DISABLE;
            }
            else
            {
                ucError = TAF_ERR_PHONE_MSG_UNMATCH;
            }

            /* 上报APP当前状态事件 */
            TAF_MMA_SndPlmnSpecialSelCnf(&pstPlmnUserSelMsg->stCtrl, ucError);

            return VOS_TRUE;    /* 错误返回 */
        }

        /* USIM卡禁止GSM接入并且是G单模时，不允许用户指定搜
           SIM卡并且是L单模时，也不允许用户指定搜 */
        if (VOS_FALSE == TAF_MMA_IsExistPlmnRatAllowAccess())
        {
            /* 上报APP当前状态事件 */
            TAF_MMA_SndPlmnSpecialSelCnf(&pstPlmnUserSelMsg->stCtrl, TAF_ERR_NO_NETWORK_SERVICE);
            return VOS_TRUE;
        }
    }
    else
    {
        /*错误处理*/
        TAF_MMA_SndPlmnSpecialSelCnf(&pstPlmnUserSelMsg->stCtrl, TAF_ERR_PARA_ERROR);
    }

    return VOS_FALSE;
}


VOS_UINT32 TAF_MMA_RcvLocInfoQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_LOCATION_INFO_QRY_REQ_STRU *pstLocInfoQryReqMsg = VOS_NULL_PTR;
    TAF_MMA_LOCATION_INFO_QRY_CNF_STRU  stLocInfoQryCnfMsg;

    pstLocInfoQryReqMsg = (TAF_MMA_LOCATION_INFO_QRY_REQ_STRU *)pstMsg;

    PS_MEM_SET(&stLocInfoQryCnfMsg, 0x00, sizeof(TAF_MMA_LOCATION_INFO_QRY_CNF_STRU));

    /*调用MN_PH_QryLocinfo查询各参数*/
    stLocInfoQryCnfMsg.ulResult = MN_PH_QryLocinfo(&stLocInfoQryCnfMsg.ulMcc,
                                                   &stLocInfoQryCnfMsg.ulMnc,
                                                   &stLocInfoQryCnfMsg.usLac,
                                                   &stLocInfoQryCnfMsg.ucRac,
                                                   &stLocInfoQryCnfMsg.ulCellid);

    TAF_MMA_SndLocInfoQryCnf(&pstLocInfoQryReqMsg->stCtrl, &stLocInfoQryCnfMsg);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvCipherQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CIPHER_QRY_REQ_STRU        *pstCipherQryReqMsg = VOS_NULL_PTR;

    pstCipherQryReqMsg = (TAF_MMA_CIPHER_QRY_REQ_STRU *)pstMsg;

    MMA_QryCipherInfo(pstCipherQryReqMsg->stCtrl.usClientId, pstCipherQryReqMsg->stCtrl.ucOpId);

    return VOS_TRUE;
}

VOS_UINT32 TAF_MMA_RcvPowerDownReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_MT_POWER_DOWN_REQ_STRU     *pstPowerDownReqMsg = VOS_NULL_PTR;

    pstPowerDownReqMsg = (TAF_MMA_MT_POWER_DOWN_REQ_STRU *)pstMsg;

#if defined(INSTANCE_1)
    /* RF(G/W) OFF,delay n ms */
    DRV_RF_LDODOWN(RF_ID1);
#elif defined(INSTANCE_2)
    /* RF(G/W) OFF,delay n ms */
    DRV_RF_LDODOWN(RF_ID2);
#else
    /* RF(G/W) OFF,delay n ms */
    DRV_RF_LDODOWN(RF_ID0);
#endif

    /* USIM OFF,delay n ms */
    (VOS_VOID)mdrv_sci_deact();
    /* BBP(G/W) OFF,不需要显示关闭 */

    /*标别该事件为MT 下电的回复消息,*/
    TAF_MMA_SndMtPowrDownCnf(&pstPowerDownReqMsg->stCtrl, TAF_ERR_NO_ERROR);
    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvPrefPlmnTypeSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_PREF_PLMN_TYPE_SET_REQ_STRU    *pstPrefPlmnSetReqMsg = VOS_NULL_PTR;
    VOS_UINT32                              ulRet;
    TAF_ERROR_CODE_ENUM_UINT32              enErrorCause;

    pstPrefPlmnSetReqMsg = (TAF_MMA_PREF_PLMN_TYPE_SET_REQ_STRU *)pstMsg;

    ulRet = TAF_SetPrefPlmnType(pstPrefPlmnSetReqMsg->enPrefPlmnType);
    if (TAF_PARA_OK != ulRet)
    {
        enErrorCause = TAF_ERR_CME_OPT_NOT_SUPPORTED;
    }
    else
    {
        enErrorCause = TAF_ERR_NO_ERROR;
    }

    TAF_MMA_SndPrefPlmnTypeSetCnf(&pstPrefPlmnSetReqMsg->stCtrl, enErrorCause, pstPrefPlmnSetReqMsg->enPrefPlmnType);
    return VOS_TRUE;
}





VOS_UINT32 TAF_MMA_RcvCFPlmnSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucCtxIndex;
    VOS_UINT8                           ucSimStatus;
    TAF_MMA_CFPLMN_SET_REQ_STRU        *pstCFPlmnReqMsg = VOS_NULL_PTR;

    pstCFPlmnReqMsg = (TAF_MMA_CFPLMN_SET_REQ_STRU *)pstMsg;

    /* 获取卡状态 */
    (VOS_VOID)MMA_PhoneGetUsimStatus(&ucSimStatus);

    /* 卡不存在则返回错误 */
    if (MMA_USIM_PRESENT != ucSimStatus)
    {
        MMA_WARNINGLOG("TAF_MMA_RcvCFPlmnSetReq_PreProc:SIM/USIM Not Present!");
        TAF_MMA_SndCFPlmnSetCnf(&pstCFPlmnReqMsg->stCtrl, TAF_MMA_APP_CFPLMN_OPER_RESULT_NO_USIM);
        return VOS_TRUE;
    }

    /* 找不到未被使用的，回复失败 */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndCFPlmnSetCnf(&pstCFPlmnReqMsg->stCtrl, TAF_MMA_APP_CFPLMN_OPER_RESULT_FAILURE);
        return VOS_TRUE;
    }

    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    TAF_MMA_SetOperCtx(pstCFPlmnReqMsg->stCtrl,
                       TAF_MMA_OPER_CFPLMN_SET_REQ, ucCtxIndex);

    /* PLMN格式转换 */
    MMA_PlmnId2NasStyle(&pstCFPlmnReqMsg->stCFPlmnPara.stPlmn);

    /* 给MSCC发cfplmn req消息 */
    TAF_MMA_SndMsccCFPlmnSetReq(&pstCFPlmnReqMsg->stCFPlmnPara);

    /* 启动保护定时器TI_TAF_MMA_WAIT_MSCC_CFPLMN_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_CFPLMN_SET_CNF, TI_TAF_MMA_WAIT_MSCC_CFPLMN_SET_CNF_LEN);
    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvCFPlmnQueryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucCtxIndex;
    VOS_UINT8                           ucSimStatus;
    TAF_MMA_CFPLMN_QUERY_REQ_STRU      *pstCFPlmnReqMsg = VOS_NULL_PTR;

    pstCFPlmnReqMsg = (TAF_MMA_CFPLMN_QUERY_REQ_STRU *)pstMsg;

    /* 获取卡状态 */
    (VOS_VOID)MMA_PhoneGetUsimStatus(&ucSimStatus);

    /* 卡不存在则返回错误 */
    if (MMA_USIM_PRESENT != ucSimStatus)
    {
        MMA_WARNINGLOG("TAF_MMA_RcvCFPlmnSetReq_PreProc:SIM/USIM Not Present!");
        TAF_MMA_SndCFPlmnQueryCnf(&pstCFPlmnReqMsg->stCtrl, TAF_MMA_APP_CFPLMN_OPER_RESULT_NO_USIM, VOS_NULL_PTR);
        return VOS_TRUE;
    }

    /* 找不到未被使用的，回复失败 */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndCFPlmnQueryCnf(&pstCFPlmnReqMsg->stCtrl, TAF_MMA_APP_CFPLMN_OPER_RESULT_FAILURE, VOS_NULL_PTR);
        return VOS_TRUE;
    }

    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    TAF_MMA_SetOperCtx(pstCFPlmnReqMsg->stCtrl,
                       TAF_MMA_OPER_CFPLMN_QRY_REQ, ucCtxIndex);

    /* 给MSCC发cfplmn req消息 */
    TAF_MMA_SndMsccCFPlmnQueryReq();

    /* 启动保护定时器TI_TAF_MMA_WAIT_MSCC_CFPLMN_QUERY_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_CFPLMN_QUERY_CNF, TI_TAF_MMA_WAIT_MSCC_CFPLMN_QUERY_CNF_LEN);
    return VOS_TRUE;
}

VOS_UINT32 TAF_MMA_RcvMsccCFPlmnSetCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_CFPLMN_SET_CNF_STRU                           *pstCFPlmnCnf    = VOS_NULL_PTR;
    TAF_MMA_OPER_CTX_STRU                                  *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                                               ucCtxIndex;
    TAF_MMA_APP_CFPLMN_OPER_RESULT_ENUM_UINT32              enRslt;

    pstMmaOperCtx = TAF_MMA_GetOperCtxAddr();

    pstCFPlmnCnf = (MSCC_MMA_CFPLMN_SET_CNF_STRU *)pstMsg;

    if (NAS_MSCC_PIF_CFPLMN_OPER_RESULT_SUCCESS != pstCFPlmnCnf->enResult)
    {
        enRslt = TAF_MMA_APP_CFPLMN_OPER_RESULT_FAILURE;
    }
    else
    {
        enRslt = TAF_MMA_APP_CFPLMN_OPER_RESULT_SUCCESS;
    }

    /* 停止保护定时器 */
    if (TAF_MMA_TIMER_STATUS_RUNING == TAF_MMA_GetTimerStatus(TI_TAF_MMA_WAIT_MSCC_CFPLMN_SET_CNF))
    {
        /* 停止保护定时器 */
        TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MSCC_CFPLMN_SET_CNF);

        /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找ID_TAF_MMA_CFPLMN_REQ对应的记录项 */
        if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_CFPLMN_SET_REQ, &ucCtxIndex))
        {
            /*  回复confirm */
            TAF_MMA_SndCFPlmnSetCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                                    enRslt);

            TAF_MMA_ClearOperCtx(ucCtxIndex);
        }
    }
    return VOS_TRUE;
}

VOS_UINT32 TAF_MMA_RcvMsccCFPlmnQueryCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_CFPLMN_QUERY_CNF_STRU                         *pstCFPlmnCnf  = VOS_NULL_PTR;
    TAF_MMA_OPER_CTX_STRU                                  *pstMmaOperCtx = VOS_NULL_PTR;
    VOS_UINT8                                               ucCtxIndex;
    TAF_MMA_APP_CFPLMN_OPER_RESULT_ENUM_UINT32              enRslt;

    pstMmaOperCtx = TAF_MMA_GetOperCtxAddr();

    pstCFPlmnCnf = (MSCC_MMA_CFPLMN_QUERY_CNF_STRU *)pstMsg;
    if (NAS_MSCC_PIF_CFPLMN_OPER_RESULT_SUCCESS != pstCFPlmnCnf->enResult)
    {
        enRslt = TAF_MMA_APP_CFPLMN_OPER_RESULT_FAILURE;
    }
    else
    {
        enRslt = TAF_MMA_APP_CFPLMN_OPER_RESULT_SUCCESS;
    }

    /* 停止保护定时器 */
    if (TAF_MMA_TIMER_STATUS_RUNING == TAF_MMA_GetTimerStatus(TI_TAF_MMA_WAIT_MSCC_CFPLMN_QUERY_CNF))
    {
        /* 停止保护定时器 */
        TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MSCC_CFPLMN_QUERY_CNF);

        /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找ID_TAF_MMA_CFPLMN_QUERY REQ对应的记录项 */
        if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_CFPLMN_QRY_REQ, &ucCtxIndex))
        {
            /*  回复confirm */
            TAF_MMA_SndCFPlmnQueryCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                                      enRslt,
                                      pstCFPlmnCnf);

            TAF_MMA_ClearOperCtx(ucCtxIndex);
        }
    }
    return VOS_TRUE;
}

VOS_UINT32 TAF_MMA_RcvTiWaitMsccCFPlmnSetCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstMmaOperCtx = TAF_MMA_GetOperCtxAddr();

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_CFPLMN_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_CFPLMN_SET_REQ, &ucCtxIndex))
    {
        /*  回复MMA_CFPLMN_CNF */
        TAF_MMA_SndCFPlmnSetCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl), TAF_MMA_APP_CFPLMN_OPER_RESULT_TIME_OUT);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;
}

VOS_UINT32 TAF_MMA_RcvTiWaitMsccCFPlmnQueryCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstMmaOperCtx = TAF_MMA_GetOperCtxAddr();

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_CFPLMN_QUERY_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_CFPLMN_QRY_REQ, &ucCtxIndex))
    {
        /*  回复MMA_CFPLMN_QUERY_CNF */
        TAF_MMA_SndCFPlmnQueryCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl), TAF_MMA_APP_CFPLMN_OPER_RESULT_TIME_OUT, VOS_NULL_PTR);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;

}

/* Added by y00307564 for CDMA Iteration 8 2015-2-4 begin */
/**********************************************************
 函 数 名  : TAF_MMA_RcvMmaPrefPlmnSetReq_PreProc
 功能描述  : CPOL设置命令处理
 输入参数  : ulEventType
             pstMsg
 输出参数  :
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年2月3日
    作    者   : y00307564
    修改内容   : Iteration 8 新生成
*************************************************************/
VOS_UINT32 TAF_MMA_RcvMmaPrefPlmnSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_PREF_PLMN_SET_REQ_STRU     *pstRcvMsg              = VOS_NULL_PTR;
    TAF_PH_SET_PREFPLMN_STRU           *pstPrefPlmnParam       = VOS_NULL_PTR;
    TAF_PH_PREF_PLMN_OPERATE_STRU       stPrefPlmnOpt;
    TAF_PLMN_ID_STRU                    stPlmnIdFromAt;
    VOS_UINT16                          usRet;
    VOS_UINT8                           ucUsimStatus;
    VOS_UINT8                           ucCtxIndex;

    pstRcvMsg         = (TAF_MMA_PREF_PLMN_SET_REQ_STRU *)pstMsg;
    pstPrefPlmnParam  = (TAF_PH_SET_PREFPLMN_STRU *)(&pstRcvMsg->stPrefPlmn);
    PS_MEM_SET(&stPrefPlmnOpt, 0x00, sizeof(TAF_PH_PREF_PLMN_OPERATE_STRU));
    PS_MEM_SET(&stPlmnIdFromAt, 0x00, sizeof(stPlmnIdFromAt));

    /* 获取卡状态 */
    ucUsimStatus      = MMA_USIM_ABSENT;
    if (MMA_SUCCESS != MMA_GetUsimStatusFromUsim(&ucUsimStatus, VOS_NULL_PTR))
    {
        MMA_WARNINGLOG("TAF_MMA_RcvMmaPrefPlmnSetReq_PreProc():WORNING:Invoke MMA_GetUsimStatusFromUsim fail.");
        TAF_MMA_SndPrefPlmnSetCnf(&(pstRcvMsg->stCtrl), TAF_MMA_APP_PREF_PLMN_OPER_RESULT_USIM_ABSENT);

        return VOS_TRUE;
    }

    if (MMA_USIM_ABSENT == ucUsimStatus)
    {
        TAF_MMA_SndPrefPlmnSetCnf(&(pstRcvMsg->stCtrl), TAF_MMA_APP_PREF_PLMN_OPER_RESULT_USIM_ABSENT);

        return VOS_TRUE;
    }

    usRet = MMA_CheckCpolPara(pstPrefPlmnParam, &stPrefPlmnOpt);
    if (VOS_TRUE != usRet)
    {
        TAF_MMA_SndPrefPlmnSetCnf(&(pstRcvMsg->stCtrl), TAF_MMA_APP_PREF_PLMN_OPER_RESULT_CHECK_PARA_ERR);

        return VOS_TRUE;
    }

    /* 先将AT过来的PLMNID进行转化 */
    PS_MEM_CPY(&stPlmnIdFromAt, &stPrefPlmnOpt.stPlmn, sizeof(stPlmnIdFromAt));
    MMA_PlmnId2NasStyle(&stPlmnIdFromAt);


    MMA_PlmnId2Bcd(&stPlmnIdFromAt);
    if (VOS_FALSE == MN_PH_IsPlmnValid(&stPlmnIdFromAt))
    {
        TAF_MMA_SndPrefPlmnSetCnf(&(pstRcvMsg->stCtrl), TAF_MMA_APP_PREF_PLMN_OPER_RESULT_INVALID_PLMN);
        return VOS_TRUE;
    }

    /* 找不到未被使用的，回复失败 */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndPrefPlmnSetCnf(&(pstRcvMsg->stCtrl), TAF_MMA_APP_PREF_PLMN_OPER_RESULT_FAILURE);

        return VOS_TRUE;
    }

    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    TAF_MMA_SetOperCtx(pstRcvMsg->stCtrl,
                       TAF_MMA_OPER_PREF_PLMN_SET_REQ,
                       ucCtxIndex);

    stPrefPlmnOpt.enPrefPLMNType = pstPrefPlmnParam->enPrefPlmnType;

    /* 向MSCC发送ID_MMA_MSCC_PREF_PLMN_SET_REQ */
    TAF_MMA_SndMsccPrefPlmnSetReq(&(pstRcvMsg->stCtrl), &stPrefPlmnOpt);

    /* 启动保护定时器TI_TAF_MMA_WAIT_MSCC_PREM_CPOL_SET_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_PREF_CPOL_SET_CNF, TI_TAF_MMA_WAIT_MSCC_PREF_CPOL_SET_CNF_LEN);

    return VOS_TRUE;
}
/**********************************************************
 函 数 名  : TAF_MMA_RcvMmaPrefPlmnTestReq_PreProc
 功能描述  : CPOL测试命令处理
 输入参数  : ulEventType
             pstMsg
 输出参数  :
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年2月3日
    作    者   : y00307564
    修改内容   : Iteration 8 新生成
*************************************************************/
VOS_UINT32 TAF_MMA_RcvMmaPrefPlmnTestReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_PREF_PLMN_TEST_REQ_STRU    *pstRcvMsg              = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstRcvMsg = (TAF_MMA_PREF_PLMN_TEST_REQ_STRU *)pstMsg;

    /* 找不到未被使用的，回复失败 */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndPrefPlmnTestCnf(&(pstRcvMsg->stCtrl), TAF_MMA_APP_PREF_PLMN_OPER_RESULT_FAILURE, VOS_NULL_PTR);
        return VOS_TRUE;
    }

    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    TAF_MMA_SetOperCtx(pstRcvMsg->stCtrl,
                       TAF_MMA_OPER_PREF_PLMN_TEST_REQ, ucCtxIndex);

    TAF_MMA_SndMsccPrefPlmnTestReq(pstRcvMsg);

    /* 启动保护定时器TI_TAF_MMA_WAIT_MSCC_PREM_CPOL_TEST_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_PREF_CPOL_TEST_CNF, TI_TAF_MMA_WAIT_MSCC_PREF_CPOL_TEST_CNF_LEN);

    return VOS_TRUE;
}

/**********************************************************
 函 数 名  : TAF_MMA_RcvMmaPrefPlmnQueryReq_PreProc
 功能描述  : CPOL查询命令处理
 输入参数  : ulEventType
             pstMsg
 输出参数  :
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年2月3日
    作    者   : y00307564
    修改内容   : Iteration 8新增
*************************************************************/
VOS_UINT32 TAF_MMA_RcvMmaPrefPlmnQueryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_PREF_PLMN_QUERY_REQ_STRU   *pstRcvMsg              = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstRcvMsg = (TAF_MMA_PREF_PLMN_QUERY_REQ_STRU *)pstMsg;

    /* 找不到未被使用的，回复失败 */
    if (VOS_FALSE == TAF_MMA_GetUnusedOperCtxIndex(&ucCtxIndex))
    {
        TAF_MMA_SndPrefPlmnQueryCnf(&(pstRcvMsg->stCtrl), TAF_MMA_APP_PREF_PLMN_OPER_RESULT_FAILURE, VOS_NULL_PTR);

        return VOS_TRUE;
    }

    /* 分配一个g_stTafMmaCtx.astMmaOperCtx到对应操作 */
    TAF_MMA_SetOperCtx(pstRcvMsg->stCtrl,
                       TAF_MMA_OPER_PREF_PLMN_QUERY_REQ,
                       ucCtxIndex);

    /* 发送ID_MMA_MSCC_PREF_PLMN_QUERY_REQ给MSCC */
    TAF_MMA_SndMsccPrefPlmnQueryReq(pstRcvMsg);

    /* 启动保护定时器TI_TAF_MMA_WAIT_MSCC_PREF_CPOL_QUERY_CNF */
    TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_PREF_CPOL_QUERY_CNF, TI_TAF_MMA_WAIT_MSCC_PREF_CPOL_QUERY_CNF_LEN);

    return VOS_TRUE;
}

/**********************************************************
 函 数 名  : TAF_MMA_RcvMsccPrefPlmnQueryCnf_PreProc
 功能描述  : CPOL查询命令处理
 输入参数  : ulEventType
             pstMsg
 输出参数  :
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年02月05日
    作    者   : y00307564
    修改内容   : Iteration 8新增
*************************************************************/
VOS_UINT32 TAF_MMA_RcvMsccPrefPlmnQueryCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_PREF_PLMN_QUERY_CNF_STRU  *pstRcvMsg       = VOS_NULL_PTR;
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;
    TAF_MMA_APP_OPER_RESULT_ENUM_UINT32 enRslt;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    pstRcvMsg = (MSCC_MMA_PREF_PLMN_QUERY_CNF_STRU *)pstMsg;

    if (NAS_MSCC_PIF_PREF_PLMN_OPER_RESULT_SUCCESS != pstRcvMsg->enRslt)
    {
        enRslt = TAF_MMA_APP_OPER_RESULT_FAILURE;
    }
    else
    {
        enRslt = TAF_MMA_APP_OPER_RESULT_SUCCESS;
    }

    if (NAS_MSCC_PIF_PREF_PLMN_QUERY == pstRcvMsg->enCmdType)
    {
        /* 停定时器 */
        TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MSCC_PREF_CPOL_QUERY_CNF);

        if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_PREF_PLMN_QUERY_REQ, &ucCtxIndex))
        {
            TAF_MMA_SndPrefPlmnQueryCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl), enRslt, pstRcvMsg);

            TAF_MMA_ClearOperCtx(ucCtxIndex);
        }
    }
    else if (NAS_MSCC_PIF_PREF_PLMN_TEST == pstRcvMsg->enCmdType)
    {
        /* 停定时器 */
        TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MSCC_PREF_CPOL_TEST_CNF);

        if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_PREF_PLMN_TEST_REQ, &ucCtxIndex))
        {
            TAF_MMA_SndPrefPlmnTestCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl), enRslt, pstRcvMsg);

            TAF_MMA_ClearOperCtx(ucCtxIndex);
        }
    }
    else
    {
        MMA_WARNINGLOG("TAF_MMA_RcvMsccPrefPlmnQueryCnf_PreProc: MSG enCmdType is wrong ");
    }

    return VOS_TRUE;
}

/**********************************************************
 函 数 名  : TAF_MMA_RcvMsccPrefPlmnSetCnf_PreProc
 功能描述  : CPOL设置命令处理
 输入参数  : ulEventType
             pstMsg
 输出参数  :
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年02月05日
    作    者   : y00307564
    修改内容   : Iteration 8新增
*************************************************************/
VOS_UINT32 TAF_MMA_RcvMsccPrefPlmnSetCnf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_PREF_PLMN_SET_CNF_STRU    *pstRcvMsg           = VOS_NULL_PTR;
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx       = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    /* 停定时器 */
    TAF_MMA_StopTimer(TI_TAF_MMA_WAIT_MSCC_PREF_CPOL_SET_CNF);

    pstRcvMsg = (MSCC_MMA_PREF_PLMN_SET_CNF_STRU *)pstMsg;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_OPER_PREF_PLMN_SET_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_PREF_PLMN_SET_REQ, &ucCtxIndex))
    {
        TAF_MMA_SndPrefPlmnSetCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl), pstRcvMsg->enRslt);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvTiWaitMsccPrefPlmnSetCnfExpired_PreProc
 功能描述  : 等待MSCC模块的pref plmn set CNF信息超时处理
 输入参数  : ulEventType
             pstMsg

 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年02月04日
    作    者   : y00307564
    修改内容   : Iteration 8新增
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvTiWaitMsccPrefPlmnSetCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_OPER_PREF_PLMN_SET_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_PREF_PLMN_SET_REQ, &ucCtxIndex))
    {
        TAF_MMA_SndPrefPlmnSetCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl), TAF_MMA_APP_PREF_PLMN_OPER_RESULT_TIMEOUT);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvTiWaitMsccPrefPlmnQueryCnfExpired_PreProc
 功能描述  : 等待MSCC模块的pref plmn query CNF信息超时处理
 输入参数  : ulEventType
             pstMsg

 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年02月04日
    作    者   : y00307564
    修改内容   : Iteration 8新增
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvTiWaitMsccPrefPlmnQueryCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU                                  *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                                               ucCtxIndex;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_OPER_PREF_PLMN_QUERY_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_PREF_PLMN_QUERY_REQ, &ucCtxIndex))
    {
        TAF_MMA_SndPrefPlmnQueryCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl),
                                       TAF_MMA_APP_PREF_PLMN_OPER_RESULT_TIMEOUT,
                                       VOS_NULL_PTR);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvTiWaitMsccPrefPlmnTestCnfExpired_PreProc
 功能描述  : 等待MSCC模块的pref plmn test CNF信息超时处理
 输入参数  : ulEventType
             pstMsg

 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年02月04日
    作    者   : y00307564
    修改内容   : Iteration 8新增
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvTiWaitMsccPrefPlmnTestCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU                                  *pstMmaOperCtx   = VOS_NULL_PTR;
    VOS_UINT8                                               ucCtxIndex;

    pstMmaOperCtx   = TAF_MMA_GetOperCtxAddr();

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找TAF_MMA_OPER_PREF_PLMN_TEST_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_PREF_PLMN_TEST_REQ, &ucCtxIndex))
    {
        TAF_MMA_SndPrefPlmnTestCnf(&(pstMmaOperCtx->stCtrl),
                                      TAF_MMA_APP_PREF_PLMN_OPER_RESULT_TIMEOUT,
                                      VOS_NULL_PTR);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvCerssiSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CERSSI_SET_REQ_STRU        *pstCerssiSeqReq = VOS_NULL;
    VOS_UINT32                          ulRptCmdStatus;
    VOS_UINT32                          ulRet;

    pstCerssiSeqReq = (TAF_MMA_CERSSI_SET_REQ_STRU *)pstMsg;

    if (0 == pstCerssiSeqReq->ucSignThreshold)
    {
        ulRptCmdStatus = VOS_FALSE;
    }
    else
    {
        ulRptCmdStatus = VOS_TRUE;
    }

    TAF_SDC_UpdateRptCmdStatus(TAF_SDC_RPT_CMD_CERSSI, ulRptCmdStatus);

    if ( (pstCerssiSeqReq->ucActionType > TAF_START_EVENT_INFO_STOP)
       ||(pstCerssiSeqReq->ucRrcMsgType > ( TAF_EVENT_INFO_CELL_BLER
                                           |TAF_EVENT_INFO_CELL_ID
                                           |TAF_EVENT_INFO_CELL_SIGN) ) )
    {
        MMA_WARNINGLOG("TAF_MMA_RcvCerssiSetReq_PreProc():WARNING:Error Para");

        TAF_MMA_SndCerssiSetCnf(&pstCerssiSeqReq->stCtrl, TAF_ERR_PARA_ERROR);

        return VOS_TRUE;
    }
    else
    {
        ulRet = TAF_MMA_SndMsccSignalReportReq(pstCerssiSeqReq->ucActionType, pstCerssiSeqReq->ucRrcMsgType,
                                               pstCerssiSeqReq->ucSignThreshold, pstCerssiSeqReq->ucMinRptTimerInterval);
    }

    if (VOS_OK == ulRet)
    {
        TAF_MMA_SndCerssiSetCnf(&pstCerssiSeqReq->stCtrl, TAF_PARA_OK);
    }
    else
    {
        TAF_MMA_SndCerssiSetCnf(&pstCerssiSeqReq->stCtrl, TAF_ERR_PARA_ERROR);
    }

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvCerssiQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CERSSI_INFO_QRY_CNF_STRU   *pstCerssiInfoQryCnf = VOS_NULL_PTR;
    TAF_SDC_APPNETWORK_INFO            *pstAppNetworkInfo   = VOS_NULL_PTR;
    TAF_MMA_CERSSI_INFO_QRY_REQ_STRU   *pstCerssiQryReq = VOS_NULL;
    TAF_SDC_SYS_MODE_ENUM_UINT8         enRatType;

    pstCerssiQryReq = (TAF_MMA_CERSSI_INFO_QRY_REQ_STRU *)pstMsg;

    enRatType           = TAF_SDC_GetAppSysMode();
    pstAppNetworkInfo   = &(TAF_SDC_GetSdcCtx()->stAppNetworkInfo);
    pstCerssiInfoQryCnf = (TAF_MMA_CERSSI_INFO_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                                              WUEPS_PID_MMA,
                                                              sizeof(TAF_MMA_CERSSI_INFO_QRY_CNF_STRU));

    if (VOS_NULL_PTR == pstCerssiInfoQryCnf)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvCerssiQryReq_PreProc:ERROR: Memory Alloc Error for pMsg");
        return VOS_TRUE;
    }

    /* 初始化消息 */
    PS_MEM_SET((VOS_CHAR*)pstCerssiInfoQryCnf + VOS_MSG_HEAD_LENGTH,
                0x00,
                sizeof(TAF_MMA_CERSSI_INFO_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH);

    /* 填写消息 */
    pstCerssiInfoQryCnf->ulReceiverPid          = WUEPS_PID_AT;
    pstCerssiInfoQryCnf->ulMsgName              = ID_TAF_MMA_CERSSI_QRY_CNF;
    PS_MEM_CPY(&pstCerssiInfoQryCnf->stCtrl, &pstCerssiQryReq->stCtrl, sizeof(TAF_MMA_CTRL_STRU));

    pstCerssiInfoQryCnf->stCerssi.ucCurrentUtranMode    = NAS_UTRANCTRL_GetCurrUtranMode();
    pstCerssiInfoQryCnf->enErrorCause                   = TAF_ERR_NO_ERROR;
    /* Modified by l00305157 for Service_State_Optimize_PhaseI, 2014-11-29, end */
    pstCerssiInfoQryCnf->stCerssi.ucRssiNum             = 1;
    pstCerssiInfoQryCnf->stCerssi.enRatType             = enRatType;

    pstCerssiInfoQryCnf->stCerssi.aRssi[0].ucRssiValue = pstAppNetworkInfo->stRssiValue.ucRssiValue;
    pstCerssiInfoQryCnf->stCerssi.aRssi[0].ucChannalQual = pstAppNetworkInfo->stRssiValue.ucChannalQual;

    switch (enRatType)
    {
        case TAF_SDC_SYS_MODE_GSM:
        {
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stGCellSignInfo.sRssiValue =  pstAppNetworkInfo->stRssiValue.u.st2GCellSignInfo.sRssiValue;
        }
        break;
        case TAF_SDC_SYS_MODE_WCDMA:
        {
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stWCellSignInfo.sRscpValue = pstAppNetworkInfo->stRssiValue.u.st3GCellSignInfo.sRscpValue;
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stWCellSignInfo.sEcioValue = pstAppNetworkInfo->stRssiValue.u.st3GCellSignInfo.sEcioValue;
        }
        break;
#if ( FEATURE_ON == FEATURE_LTE )
        case TAF_SDC_SYS_MODE_LTE:
        {
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stLCellSignInfo.lSINR = pstAppNetworkInfo->stRssiValue.u.st4GCellSignInfo.lSINR;
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stLCellSignInfo.sRsd = pstAppNetworkInfo->stRssiValue.u.st4GCellSignInfo.sRsd;
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stLCellSignInfo.sRsrp = pstAppNetworkInfo->stRssiValue.u.st4GCellSignInfo.sRsrp;
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stLCellSignInfo.sRsrq = pstAppNetworkInfo->stRssiValue.u.st4GCellSignInfo.sRsrq;
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stLCellSignInfo.sRssi = pstAppNetworkInfo->stRssiValue.u.st4GCellSignInfo.sRssi;
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stLCellSignInfo.stCQI.usRI = pstAppNetworkInfo->stRssiValue.u.st4GCellSignInfo.stCQI.usRI;
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stLCellSignInfo.stCQI.ausCQI[0] = pstAppNetworkInfo->stRssiValue.u.st4GCellSignInfo.stCQI.ausCQI[0];
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stLCellSignInfo.stCQI.ausCQI[1] = pstAppNetworkInfo->stRssiValue.u.st4GCellSignInfo.stCQI.ausCQI[1];
        }
        break;
#endif
        default:
        /*还未驻留成功，返回无效值*/
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stGCellSignInfo.sRssiValue = pstAppNetworkInfo->stRssiValue.u.st2GCellSignInfo.sRssiValue;
            pstCerssiInfoQryCnf->stCerssi.aRssi[0].u.stWCellSignInfo.sRscpValue = pstAppNetworkInfo->stRssiValue.u.st3GCellSignInfo.sRscpValue;

            MMA_WARNINGLOG1("TAF_MMA_RcvCerssiQryReq_PreProc():WARNING:TAF_SDC_GetAppSysMode():", enRatType);
         break;
    }


    /* 发送消息 */
    if( VOS_OK != PS_SEND_MSG(WUEPS_PID_MMA, pstCerssiInfoQryCnf))
    {
        TAF_LOG(WUEPS_PID_MMA, VOS_NULL, PS_LOG_LEVEL_ERROR, "TAF_MMA_RcvCerssiQryReq_PreProc:ERROR: Send Msg Error!");
    }

    return VOS_TRUE;
}

/* Added by k902809 for Iteration 11, 2015-3-24, begin */

/*****************************************************************************
  Function Name     :   TAF_MMA_RcvAcInfoQry_PreProc
  Description       :   Get CS and PS details
                        Get the raio mode and send error if radio mode is LTE
                        Assign all the values and send the message with message ID =ID_TAF_MMA_AC_INFO_QRY_CNF

  Input parameters  :   VOS_UINT32                          ulEventType,
                        struct MsgCB                       *pstMsg
  Outout parameters :
  Return Value      :   VOS_UINT32

  Modify History:
      1)  Date      :   2015-03-21
          Author    :   k902809
          Modify content :    Create
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvAcInfoQry_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_AC_INFO_QRY_REQ_STRU       *pstRcvMsg       = VOS_NULL_PTR;
    TAF_MMA_AC_INFO_QRY_CNF_STRU       *pstAcInfoQryCnf = VOS_NULL_PTR;
    TAF_SDC_SYS_MODE_ENUM_UINT8         enRadioMode;
    TAF_SDC_ACCESS_RESTRICTION_STRU    *pstCsAcRestriction = VOS_NULL_PTR;
    TAF_SDC_ACCESS_RESTRICTION_STRU    *pstPsAcRestriction = VOS_NULL_PTR;

    pstRcvMsg = (TAF_MMA_AC_INFO_QRY_REQ_STRU *)pstMsg;

    pstCsAcRestriction = TAF_SDC_GetCsAcRestriction();
    pstPsAcRestriction = TAF_SDC_GetPsAcRestriction();
    pstAcInfoQryCnf    = (TAF_MMA_AC_INFO_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                                         WUEPS_PID_MMA,
                                                         sizeof(TAF_MMA_AC_INFO_QRY_CNF_STRU));

    if (VOS_NULL_PTR == pstAcInfoQryCnf)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvAcInfoQry_PreProc:ERROR: Memory Alloc Error");
        return VOS_TRUE;
    }

    /* Set the allocated memory with 0 */
    PS_MEM_SET( (VOS_UINT8 *)pstAcInfoQryCnf + VOS_MSG_HEAD_LENGTH,
                0x00,
                (VOS_SIZE_T)(sizeof(TAF_MMA_AC_INFO_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH) );

    /* Assigning the values to cnf message */
    pstAcInfoQryCnf->ulReceiverPid          = pstRcvMsg->stCtrl.ulModuleId;
    pstAcInfoQryCnf->ulMsgName              = ID_TAF_MMA_AC_INFO_QRY_CNF;
    pstAcInfoQryCnf->stCtrl.usClientId      = MN_GetRealClientId(pstRcvMsg->stCtrl.usClientId, WUEPS_PID_MMA);
    pstAcInfoQryCnf->stCtrl.ucOpId          = pstRcvMsg->stCtrl.ucOpId;
    pstAcInfoQryCnf->stCtrl.ulModuleId      = pstRcvMsg->stCtrl.ulModuleId;

    /* Get RadioAccess Mode */
    enRadioMode = TAF_SDC_GetSysMode();

    if (TAF_SDC_SYS_MODE_LTE == enRadioMode)
    {
        /* Setting result with ERROR and send message */
        pstAcInfoQryCnf->ulRslt = TAF_ERR_ERROR;
    }
    else
    {
        pstAcInfoQryCnf->ulRslt = TAF_ERR_NO_ERROR;

        pstAcInfoQryCnf->stCellCsAcInfo.enSrvDomain           = TAF_MMA_SERVICE_DOMAIN_CS;
        pstAcInfoQryCnf->stCellCsAcInfo.enCellAcType          = pstCsAcRestriction->enCellAcType;
        pstAcInfoQryCnf->stCellCsAcInfo.ucRestrictPagingRsp   = pstCsAcRestriction->ucRestrictPagingRsp;
        pstAcInfoQryCnf->stCellCsAcInfo.ucRestrictRegister    = pstCsAcRestriction->ucRestrictRegister;

        pstAcInfoQryCnf->stCellPsAcInfo.enSrvDomain           = TAF_MMA_SERVICE_DOMAIN_PS;
        pstAcInfoQryCnf->stCellPsAcInfo.enCellAcType          = pstPsAcRestriction->enCellAcType;
        pstAcInfoQryCnf->stCellPsAcInfo.ucRestrictPagingRsp   = pstPsAcRestriction->ucRestrictPagingRsp;
        pstAcInfoQryCnf->stCellPsAcInfo.ucRestrictRegister    = pstPsAcRestriction->ucRestrictRegister;
    }

    /* Sending message */
    PS_SEND_MSG(WUEPS_PID_MMA, pstAcInfoQryCnf);

    return VOS_TRUE;
}

/*****************************************************************************
  Function Name     :   TAF_MMA_RcvCopnInfoQry_PreProc
  Description       :
                        Get the raio mode and send error if radio mode is LTE
                        Assign all the values and send the message with message
                                                   ID =ID_TAF_MMA_COPN_INFO_QRY_CNF

  Input parameters  :   VOS_UINT32                          ulEventType,
                        struct MsgCB                       *pstMsg
  Outout parameters :
  Return Value      :   VOS_UINT32

  Modify History:
      1)  Date      :   2015-03-24
          Author    :   k902809
          Modify content :    Create
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvCopnInfoQry_PreProc(
             VOS_UINT32                          ulEventType,
             struct MsgCB                       *pstMsg
)
{
    TAF_MMA_COPN_INFO_QRY_REQ_STRU     *pstCopnQerReq = VOS_NULL_PTR;
    TAF_MMA_COPN_INFO_QRY_CNF_STRU     *pstCopnQryCnf = VOS_NULL_PTR;
    VOS_UINT16                          usPlmnNum;
    VOS_UINT16                          usRemainPlmnNum;
    VOS_UINT16                          usTotal;
    VOS_UINT32                          ulMsgSize;

    pstCopnQerReq = (TAF_MMA_COPN_INFO_QRY_REQ_STRU*)pstMsg;

    /*
        设置分组结束标志和分组上报运营商个数: 待发送的运营商数少于一条消息中
        允许发送的运营商数，则设置结束标志并发送所有待发送运营商信息；
    */
    usTotal             = TAF_MMA_GetNetWorkNameTblSize();

    if (pstCopnQerReq->usFromIndex > usTotal)
    {
        TAF_LOG(WUEPS_PID_MMA, VOS_NULL_PTR, PS_LOG_LEVEL_WARNING,
                "TAF_MMA_RcvCopnInfoQry_PreProc: abnormal request!");
        usPlmnNum = 0;
    }
    else
    {
        usRemainPlmnNum = usTotal - (pstCopnQerReq->usFromIndex);

        if (usRemainPlmnNum < pstCopnQerReq->usPlmnNum)
        {
            usPlmnNum = usRemainPlmnNum;
        }
        else
        {
            usPlmnNum = pstCopnQerReq->usPlmnNum;
        }
    }

    if (0 < usPlmnNum)
    {
        ulMsgSize  = sizeof(TAF_MMA_COPN_INFO_QRY_CNF_STRU) - sizeof(pstCopnQryCnf->aucContent);
        ulMsgSize += usPlmnNum * sizeof(TAF_PH_OPERATOR_NAME_STRU);
    }
    else
    {
        ulMsgSize  = sizeof(TAF_MMA_COPN_INFO_QRY_CNF_STRU);
    }

    /* 以TAF_PH_COPN_PLMN_MAX_NUM个运营商为一组分组上报运营商信息 */
    pstCopnQryCnf  = (TAF_MMA_COPN_INFO_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                                       WUEPS_PID_MMA,
                                                       ulMsgSize);

    if (VOS_NULL_PTR == pstCopnQryCnf)
    {
        TAF_LOG(WUEPS_PID_MMA, VOS_NULL_PTR, PS_LOG_LEVEL_ERROR,
                "TAF_MMA_RcvCopnInfoQry_PreProc: Memory Alloc Error for pMsg!");
        return VOS_TRUE;
    }

    /* 初始化消息 */
    PS_MEM_SET( (VOS_UINT8 *)pstCopnQryCnf + VOS_MSG_HEAD_LENGTH,
                0x00,
                (VOS_SIZE_T)(ulMsgSize - VOS_MSG_HEAD_LENGTH) );

    /* 填写消息头部信息 */
    pstCopnQryCnf->ulReceiverPid          = pstCopnQerReq->stCtrl.ulModuleId;
    pstCopnQryCnf->ulMsgName              = ID_TAF_MMA_COPN_INFO_QRY_CNF;
    PS_MEM_CPY(&pstCopnQryCnf->stCtrl, &pstCopnQerReq->stCtrl, sizeof(TAF_MMA_CTRL_STRU));

    /*
    从ulFromIndex开始将gastNetworkNameTbl中运营商信息填写到
    pstCopnQryCnf->astOperNameList结构数组，
    运营商的个数为pstCopnQryCnf->usPlmnNum
    */
    TAF_MMA_FillCopnOperNameList(pstCopnQerReq->usFromIndex, usPlmnNum, pstCopnQryCnf);

    pstCopnQryCnf->usFromIndex = pstCopnQerReq->usFromIndex;

    /* 发送消息 */
    PS_SEND_MSG(WUEPS_PID_MMA, pstCopnQryCnf);

    return VOS_TRUE;
}

/*****************************************************************************
 Function Name     :   TAF_MMA_RcvSimInsertReq_PreProc
 Description       :

 Input parameters  :   VOS_UINT32                          ulEventType,
                       struct MsgCB                       *pstMsg
 Outout parameters :
 Return Value      :   VOS_UINT32

 Modify History:
     1)  Date      :   2015-03-24
         Author    :   k902809
         Modify content :    Create
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvSimInsertReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_SIM_INSERT_REQ_STRU        *pstSimInsertReq = VOS_NULL_PTR;
    TAF_MMA_SIM_INSERT_CNF_STRU        *pstSimInsertCnf = VOS_NULL_PTR;

    pstSimInsertReq = (TAF_MMA_SIM_INSERT_REQ_STRU*) pstMsg;

    pstSimInsertCnf  = (TAF_MMA_SIM_INSERT_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                                      WUEPS_PID_MMA,
                                                      sizeof(TAF_MMA_SIM_INSERT_CNF_STRU));

    if (VOS_NULL_PTR == pstSimInsertCnf)
    {
        TAF_LOG(WUEPS_PID_MMA, VOS_NULL_PTR, PS_LOG_LEVEL_ERROR,
                "TAF_MMA_RcvSimInsertReq_PreProc: Memory Alloc Error for pMsg!");
        return VOS_TRUE;
    }

    /* 初始化消息 */
    PS_MEM_SET( (VOS_UINT8 *)pstSimInsertCnf + VOS_MSG_HEAD_LENGTH,
                0x00,
                (VOS_SIZE_T)(sizeof(TAF_MMA_SIM_INSERT_CNF_STRU) - VOS_MSG_HEAD_LENGTH) );

    /* 填写消息头部信息 */
    pstSimInsertCnf->ulReceiverPid          = pstSimInsertReq->stCtrl.ulModuleId;
    pstSimInsertCnf->ulMsgName              = ID_TAF_MMA_SIM_INSERT_CNF;
    PS_MEM_CPY(&pstSimInsertCnf->stCtrl, &pstSimInsertReq->stCtrl, sizeof(TAF_MMA_CTRL_STRU));

    if (TAF_SIM_INSERT_STATE_REMOVE == pstSimInsertReq->enSimInsertState)
    {
        if (USIMM_API_SUCCESS == NAS_USIMMAPI_DeactiveCardReq(WUEPS_PID_MMA))
        {
            pstSimInsertCnf->ulResult = VOS_OK;
        }
        else
        {
            pstSimInsertCnf->ulResult = VOS_ERR;
        }
    }
    else
    {
        pstSimInsertCnf->ulResult = VOS_OK;
    }

    /* 发送消息 */
    PS_SEND_MSG(WUEPS_PID_MMA, pstSimInsertCnf);

    return VOS_TRUE;
}

/*****************************************************************************
 Function Name     :   TAF_MMA_RcvEOPlmnSetReq_PreProc
 Description       :

 Input parameters  :   VOS_UINT32                          ulEventType,
                       struct MsgCB                       *pstMsg
 Outout parameters :
 Return Value      :   VOS_UINT32

 Modify History:
     1)  Date      :   2015-03-24
         Author    :   k902809
         Modify content :    Create
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvEOPlmnSetReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_EOPLMN_SET_REQ_STRU        *pstSetEOPlmnReq = VOS_NULL_PTR;

    pstSetEOPlmnReq = (TAF_MMA_EOPLMN_SET_REQ_STRU *)pstMsg;

    /* 保存上下文*/
    gstMmaValue.stEOPlmnSet.usClientId = pstSetEOPlmnReq->stCtrl.usClientId;
    gstMmaValue.stEOPlmnSet.ucOpId     = pstSetEOPlmnReq->stCtrl.ucOpId;

    /* 给MSCC模块发送ID_MMA_MSCC_EOPLMN_SET_REQ */
    TAF_MMA_SndMsccEOPlmnSetReq(&(pstSetEOPlmnReq->stEOPlmnSetPara));

    return VOS_TRUE;
}

/*****************************************************************************
 Function Name     :   TAF_MMA_RcvEOPlmnQryReq_PreProc
 Description       :

 Input parameters  :   VOS_UINT32                          ulEventType,
                       struct MsgCB                       *pstMsg
 Outout parameters :
 Return Value      :   VOS_UINT32

 Modify History:
     1)  Date      :   2015-03-30
         Author    :   k902809
         Modify content :    Create
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvEOPlmnQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                                              ulLen;
    TAF_MMA_EOPLMN_QRY_REQ_STRU                            *pstEOPlmnQryReq = VOS_NULL_PTR;
    TAF_MMA_EOPLMN_QRY_CNF_STRU                            *pstEOPlmnQryCnf = VOS_NULL_PTR;
    NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU                *pstNvimCfgOPlmnInfo = VOS_NULL_PTR;

    ulLen           = 0;
    pstEOPlmnQryReq = (TAF_MMA_EOPLMN_QRY_REQ_STRU *)pstMsg;

    pstNvimCfgOPlmnInfo = (NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU*)PS_MEM_ALLOC(
                                                                    WUEPS_PID_MMA,
                                                                    sizeof(NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU));
    if (VOS_NULL_PTR == pstNvimCfgOPlmnInfo)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvEOPlmnQryReq_PreProc():ERROR: Memory Alloc Error");
        return VOS_TRUE;
    }
    PS_MEM_SET(pstNvimCfgOPlmnInfo, 0x00, sizeof(NAS_MMC_NVIM_USER_CFG_OPLMN_EXTEND_STRU));


    pstEOPlmnQryCnf = (TAF_MMA_EOPLMN_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                                     WUEPS_PID_MMA,
                                                     sizeof(TAF_MMA_EOPLMN_QRY_CNF_STRU));
    if (VOS_NULL_PTR == pstEOPlmnQryCnf)
    {
        MN_ERR_LOG("TAF_MMA_RcvEOPlmnQryReq_PreProc: Alloc Msg Failed!");
        PS_MEM_FREE(WUEPS_PID_MMA, pstNvimCfgOPlmnInfo);
        return VOS_TRUE;
    }

    PS_MEM_SET( (VOS_INT8 *)pstEOPlmnQryCnf + VOS_MSG_HEAD_LENGTH, 0x00,
                (VOS_SIZE_T)(sizeof(TAF_MMA_EOPLMN_QRY_CNF_STRU)  - VOS_MSG_HEAD_LENGTH) );

    /* 读NV项en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST，失败，直接返回 */
    (VOS_VOID)NV_GetLength(en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST, &ulLen);

    if (NV_OK != NV_Read(en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST,
                         pstNvimCfgOPlmnInfo, ulLen))
    {
        MN_ERR_LOG("TAF_MMA_RcvEOPlmnQryReq_PreProc():WARNING: read en_NV_Item_USER_CFG_OPLMN_EXTEND_LIST Error");
        /* 填充消息 */
        pstEOPlmnQryCnf->ulResult = TAF_ERR_ERROR;
    }
    else
    {
        /* 填充消息 */
        pstEOPlmnQryCnf->ulResult = TAF_ERR_NO_ERROR;
    }

    /* 填充消息头 */;
    pstEOPlmnQryCnf->ulReceiverPid          = pstEOPlmnQryReq->stCtrl.ulModuleId;
    pstEOPlmnQryCnf->ulMsgName              = ID_TAF_MMA_EOPLMN_QRY_CNF;
    PS_MEM_CPY(&pstEOPlmnQryCnf->stCtrl, &pstEOPlmnQryReq->stCtrl, sizeof(TAF_MMA_CTRL_STRU));

    PS_MEM_CPY(pstEOPlmnQryCnf->aucVersion,
               pstNvimCfgOPlmnInfo->aucVersion,
               NAS_MMC_NVIM_MAX_USER_OPLMN_VERSION_LEN * sizeof(VOS_UINT8));

    pstEOPlmnQryCnf->usOPlmnNum = pstNvimCfgOPlmnInfo->usOplmnListNum;

    PS_MEM_CPY(pstEOPlmnQryCnf->aucOPlmnList,
               pstNvimCfgOPlmnInfo->aucOPlmnList,
               NAS_MMC_NVIM_MAX_USER_CFG_OPLMN_DATA_EXTEND_LEN * sizeof(VOS_UINT8));

    PS_SEND_MSG(WUEPS_PID_MMA, pstEOPlmnQryCnf);

    PS_MEM_FREE(WUEPS_PID_MMA, pstNvimCfgOPlmnInfo);

    return VOS_TRUE;
}

/*****************************************************************************
 Function Name     :   TAF_MMA_RcvNetScanReq_PreProc
 Description       :

 Input parameters  :   VOS_UINT32                          ulEventType,
                       struct MsgCB                       *pstMsg
 Outout parameters :
 Return Value      :   VOS_UINT32

 Modify History:
     1)  Date      :   2015-03-25
         Author    :   k902809
         Modify content :    Create
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvNetScanReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_NET_SCAN_REQ_STRU          *pstNetScanSetPara   = VOS_NULL_PTR;
    TAF_MMA_NET_SCAN_CNF_STRU          *pstNetScanCnf       = VOS_NULL_PTR;

    pstNetScanSetPara = (TAF_MMA_NET_SCAN_REQ_STRU *)pstMsg;

    if ( STA_FSM_ENABLE != g_StatusContext.ulFsmState )
    {

        pstNetScanCnf  = (TAF_MMA_NET_SCAN_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                                      WUEPS_PID_MMA,
                                                      sizeof(TAF_MMA_NET_SCAN_CNF_STRU));

        if (VOS_NULL_PTR == pstNetScanCnf)
        {
            TAF_LOG(WUEPS_PID_MMA, VOS_NULL_PTR, PS_LOG_LEVEL_ERROR,
                    "TAF_MMA_RcvNetScanReq_PreProc: Memory Alloc Error for pMsg!");
            return VOS_TRUE;
        }

        PS_MEM_SET( (VOS_UINT8 *)pstNetScanCnf + VOS_MSG_HEAD_LENGTH, 0x00,
                    (VOS_SIZE_T)(sizeof(TAF_MMA_NET_SCAN_CNF_STRU) - VOS_MSG_HEAD_LENGTH) );

        pstNetScanCnf->ulReceiverPid            = pstNetScanSetPara->stCtrl.ulModuleId;
        pstNetScanCnf->ulMsgName                = ID_TAF_MMA_NET_SCAN_CNF;
        pstNetScanCnf->usClientId               = MN_GetRealClientId(pstNetScanSetPara->stCtrl.usClientId, WUEPS_PID_TAF);
        pstNetScanCnf->ucOpId                   = pstNetScanSetPara->stCtrl.ucOpId;
        pstNetScanCnf->enResult                 = TAF_MMA_NET_SCAN_RESULT_FAILURE;
        pstNetScanCnf->enCause                  = TAF_MMA_NET_SCAN_CAUSE_MMA_STATE_DISABLE;

        PS_SEND_MSG(WUEPS_PID_MMA, pstNetScanCnf);

        return VOS_TRUE;
    }

    /* 保存^NETSCAN下发的ClientId和OpId */
    gstMmaValue.stNetScan.usClientId = pstNetScanSetPara->stCtrl.usClientId;
    gstMmaValue.stNetScan.ucOpId     = pstNetScanSetPara->stCtrl.ucOpId;

    /* 给MSCC模块发送NETSCAN REQ */
    TAF_MMA_SndMsccNetScanMsgReq(pstNetScanSetPara);

    /* 启动STA_TIMER_NET_SCAN 108s定时器，如果超时未收到MSCC的回复则给MSCC发送ABORT NET SCAN REQ */
    TAF_MMA_StartNetScanProtectTimer();

    return VOS_TRUE;
}


/*****************************************************************************
 Function Name     :   TAF_MMA_RcvTafMsgAbortNetScanReq_PreProc
 Description       :

 Input parameters  :   VOS_UINT32                          ulEventType,
                       struct MsgCB                       *pstMsg
 Outout parameters :
 Return Value      :   VOS_UINT32

 Modify History:
     1)  Date      :   2015-03-25
         Author    :   k902809
         Modify content :    Create
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvAbortNetScanReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_NET_SCAN_ABORT_REQ_STRU    *pstNetScanAbortReq = VOS_NULL_PTR;

    pstNetScanAbortReq  = (TAF_MMA_NET_SCAN_ABORT_REQ_STRU*)pstMsg;

    if ( MMA_TIMER_RUN == g_stNetScanProtectTimer.ucTimerStatus )
    {
        TAF_MMA_StopNetScanProtectTimer();

        /* 保存^NETSCAN下发的ClientId和OpId */
        gstMmaValue.stNetScan.usClientId = pstNetScanAbortReq->stCtrl.usClientId;
        gstMmaValue.stNetScan.ucOpId     = pstNetScanAbortReq->stCtrl.ucOpId;

        /* 给MSCC模块发送打断NETSCAN REQ */
        TAF_MMA_SndMsccAbortNetScanMsgReq();

        /* 启动TI_TAF_MMA_ABORT_NET_SCAN_TIMER 10s定时器 */
        (VOS_VOID)TAF_MMA_StartAbortNetScanProtectTimer();
    }

    /* MMA保护定时器超时后，给MSCC发送ABORT请求后，用户此时再下发打断请求，
       则忽略用户下发的打断请求 */

    return VOS_TRUE;
}

/**********************************************************
 Function Name     :   TAF_MMA_QryUserSrvStatePara_PreProc
 Description       :

 Input parameters  :   VOS_UINT32                          ulEventType,
                       struct MsgCB                       *pstMsg
 Outout parameters :
 Return Value      :   VOS_UINT32

 Modify History:
     1)  Date      :   2015-03-27
         Author    :   k902809
         Modify content :    Create
*************************************************************/
VOS_UINT32 TAF_MMA_RcvUserSrvStateQry_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_USER_SRV_STATE_QRY_CNF_STRU  *pstUserservstateCnf   = VOS_NULL_PTR;
    TAF_MMA_USER_SRV_STATE_QRY_REQ_STRU  *pstRcvMsg             = VOS_NULL_PTR;
    VOS_UINT8                           ucCsServiceConnStatusFlag;

    pstRcvMsg = (TAF_MMA_USER_SRV_STATE_QRY_REQ_STRU *)pstMsg;

    pstUserservstateCnf  = (TAF_MMA_USER_SRV_STATE_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                                                  WUEPS_PID_MMA,
                                                                  sizeof(TAF_MMA_USER_SRV_STATE_QRY_CNF_STRU));
    if (VOS_NULL_PTR == pstUserservstateCnf)
    {
        TAF_LOG(WUEPS_PID_MMA, VOS_NULL_PTR, PS_LOG_LEVEL_ERROR,
                "TAF_MMA_RcvUserSrvStateQry_PreProc: Memory Alloc Error for pMsg!");
        return VOS_TRUE;
    }

    PS_MEM_SET( (VOS_UINT8 *)pstUserservstateCnf + VOS_MSG_HEAD_LENGTH,
                0x00,
                (VOS_SIZE_T)(sizeof(TAF_MMA_USER_SRV_STATE_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH) );


    ucCsServiceConnStatusFlag               = TAF_SDC_GetCsServiceConnStatusFlag();
    pstUserservstateCnf->ulCsSrvExistFlg    = VOS_FALSE;

    /* 获取当前是否有CS业务 */
    if ((VOS_TRUE == TAF_SDC_GetCsSrvExistFlg())
     || (VOS_TRUE == ucCsServiceConnStatusFlag))
    {
        pstUserservstateCnf->ulCsSrvExistFlg = VOS_TRUE;
    }
    pstUserservstateCnf->ulReceiverPid          = pstRcvMsg->stCtrl.ulModuleId;
    pstUserservstateCnf->ulMsgName              = ID_TAF_MMA_USER_SRV_STATE_QRY_CNF;
    pstUserservstateCnf->usClientId             = pstRcvMsg->stCtrl.usClientId;
    pstUserservstateCnf->ucOpId                 = pstRcvMsg->stCtrl.ucOpId;

    /* 目前只提供CS业务是否存在的查询，PS业务的查询不提供，固定填成0 */
    pstUserservstateCnf->ulPsSrvExistFlg = VOS_FALSE;

    PS_SEND_MSG(WUEPS_PID_MMA, pstUserservstateCnf);

    return VOS_TRUE;
}

/**********************************************************
 Function Name     :   TAF_MMA_RcvApPwrOnAndRegTimeQryReq_PreProc
 Description       :

 Input parameters  :   VOS_UINT32                          ulEventType,
                       struct MsgCB                       *pstMsg
 Outout parameters :
 Return Value      :   VOS_UINT32

 Modify History:
     1)  Date      :   2015-03-27
         Author    :   k902809
         Modify content :    Create
*************************************************************/
VOS_UINT32 TAF_MMA_RcvApPwrOnAndRegTimeQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_POWER_ON_AND_REG_TIME_QRY_CNF_STRU             *pstAppwronregCnf = VOS_NULL_PTR;
    TAF_MMA_POWER_ON_AND_REG_TIME_QRY_REQ_STRU             *pstRcvMsg        = VOS_NULL_PTR;

    pstRcvMsg = (TAF_MMA_POWER_ON_AND_REG_TIME_QRY_REQ_STRU *)pstMsg;

    pstAppwronregCnf  = (TAF_MMA_POWER_ON_AND_REG_TIME_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                                                      WUEPS_PID_MMA,
                                                                      sizeof(TAF_MMA_POWER_ON_AND_REG_TIME_QRY_CNF_STRU));
    if (VOS_NULL_PTR == pstAppwronregCnf)
    {
        TAF_LOG(WUEPS_PID_MMA, VOS_NULL_PTR, PS_LOG_LEVEL_ERROR,
                "TAF_MMA_RcvApPwrOnAndRegTimeQryReq_PreProc: Memory Alloc Error for pMsg!");
        return VOS_TRUE;
    }

    PS_MEM_SET( (VOS_UINT8 *)pstAppwronregCnf + VOS_MSG_HEAD_LENGTH,
                0x00,
                (VOS_SIZE_T)(sizeof(TAF_MMA_POWER_ON_AND_REG_TIME_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH) );

    pstAppwronregCnf->ulReceiverPid             = pstRcvMsg->stCtrl.ulModuleId;
    pstAppwronregCnf->ulMsgName                 = ID_TAF_MMA_POWER_ON_AND_REG_TIME_QRY_CNF;
    pstAppwronregCnf->usClientId                = pstRcvMsg->stCtrl.usClientId;
    pstAppwronregCnf->ucOpId                    = pstRcvMsg->stCtrl.ucOpId;

    pstAppwronregCnf->ulCostTime                = MN_PH_GetRegCostTime();

    PS_SEND_MSG(WUEPS_PID_MMA, pstAppwronregCnf);

    return VOS_TRUE;
}

/*****************************************************************************
 Function Name     :   TAF_MMA_RcvSpnQryReq_PreProc
 Description       :

 Input parameters  :   VOS_UINT32                          ulEventType,
                       struct MsgCB                       *pstMsg
 Outout parameters :
 Return Value      :   VOS_UINT32

 Modify History:
     1)  Date      :   2015-03-30
         Author    :   k902809
         Modify content :    Create

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvSpnQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_SPN_QRY_REQ_STRU           *pstSpnReq          = VOS_NULL_PTR;
    TAF_MMA_SPN_QRY_CNF_STRU           *pstSpnCnf          = VOS_NULL_PTR;
    TAF_SDC_SPN_FILE_INFO_STRU         *pstUsimSpnFileInfo = VOS_NULL_PTR;
    TAF_SDC_SPN_FILE_INFO_STRU         *pstSimSpnFileInfo  = VOS_NULL_PTR;

    pstSpnReq          = (TAF_MMA_SPN_QRY_REQ_STRU *)pstMsg;
    pstUsimSpnFileInfo = TAF_SDC_GetUsimSpnFileInfo();
    pstSimSpnFileInfo  = TAF_SDC_GetSimSpnFileInfo();

    pstSpnCnf  = (TAF_MMA_SPN_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                             WUEPS_PID_MMA,
                                             sizeof(TAF_MMA_SPN_QRY_CNF_STRU));
    if (VOS_NULL_PTR == pstSpnCnf)
    {
        TAF_LOG(WUEPS_PID_MMA, VOS_NULL_PTR, PS_LOG_LEVEL_ERROR,
                "TAF_MMA_RcvSpnQryReq_PreProc: Memory Alloc Error for pMsg!");
        return VOS_TRUE;
    }

    PS_MEM_SET( (VOS_UINT8 *)pstSpnCnf + VOS_MSG_HEAD_LENGTH,
                0x00,
                (VOS_SIZE_T)(sizeof(TAF_MMA_SPN_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH) );

    pstSpnCnf->ulReceiverPid          = pstSpnReq->stCtrl.ulModuleId;
    pstSpnCnf->ulMsgName              = ID_TAF_MMA_SPN_QRY_CNF;
    pstSpnCnf->usClientId             = pstSpnReq->stCtrl.usClientId;
    pstSpnCnf->ucOpId                 = pstSpnReq->stCtrl.ucOpId;

    /* SIM SPN */
    pstSpnCnf->stMnMmaSpnInfo.stSimSpnInfo.ucCoding        = pstSimSpnFileInfo->stSpnOperName.bitCoding;
    pstSpnCnf->stMnMmaSpnInfo.stSimSpnInfo.ucSpareBit      = pstSimSpnFileInfo->stSpnOperName.bitSpare;
    pstSpnCnf->stMnMmaSpnInfo.stSimSpnInfo.ucLength        = pstSimSpnFileInfo->stSpnOperName.ucLength;
    pstSpnCnf->stMnMmaSpnInfo.stSimSpnInfo.ucDispRplmnMode = pstSimSpnFileInfo->ucDispRplmnMode;
    PS_MEM_CPY(pstSpnCnf->stMnMmaSpnInfo.stSimSpnInfo.aucSpnName, pstSimSpnFileInfo->stSpnOperName.aucOperatorName, TAF_SDC_MAX_OPER_NAME_NUM);

    /* USIM SPN */
    pstSpnCnf->stMnMmaSpnInfo.stUsimSpnInfo.ucCoding        = pstUsimSpnFileInfo->stSpnOperName.bitCoding;
    pstSpnCnf->stMnMmaSpnInfo.stUsimSpnInfo.ucSpareBit      = pstUsimSpnFileInfo->stSpnOperName.bitSpare;
    pstSpnCnf->stMnMmaSpnInfo.stUsimSpnInfo.ucLength        = pstUsimSpnFileInfo->stSpnOperName.ucLength;
    pstSpnCnf->stMnMmaSpnInfo.stUsimSpnInfo.ucDispRplmnMode = pstUsimSpnFileInfo->ucDispRplmnMode;
    PS_MEM_CPY(pstSpnCnf->stMnMmaSpnInfo.stUsimSpnInfo.aucSpnName, pstUsimSpnFileInfo->stSpnOperName.aucOperatorName, TAF_SDC_MAX_OPER_NAME_NUM);

    PS_SEND_MSG(WUEPS_PID_MMA, pstSpnCnf);

    return VOS_TRUE;
}



VOS_UINT32 TAF_MMA_RcvMMPlmnInfoQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_MMPLMNINFO_QRY_REQ_STRU    *pstMMPlmnInfoReq = VOS_NULL_PTR;
    TAF_MMA_MMPLMNINFO_QRY_CNF_STRU    *pstMMPlmnInfoCnf = VOS_NULL_PTR;
    TAF_PLMN_ID_STRU                   *pstPlmnId        = VOS_NULL_PTR;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enCsServiceStatus;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8   enPsServiceStatus;

    TAF_SDC_MM_INFO_STRU               *pstMmInfo = VOS_NULL_PTR;

    pstMmInfo = TAF_SDC_GetMmInfo();

    pstMMPlmnInfoReq = (TAF_MMA_MMPLMNINFO_QRY_REQ_STRU *)pstMsg;

    pstMMPlmnInfoCnf  = (TAF_MMA_MMPLMNINFO_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                                           WUEPS_PID_MMA,
                                                           sizeof(TAF_MMA_MMPLMNINFO_QRY_CNF_STRU));
    if (VOS_NULL_PTR == pstMMPlmnInfoCnf)
    {
        TAF_LOG(WUEPS_PID_MMA, VOS_NULL_PTR, PS_LOG_LEVEL_ERROR,
                "TAF_MMA_RcvMMPlmnInfoQryReq_PreProc: Memory Alloc Error for pMsg!");
        return VOS_TRUE;
    }

    PS_MEM_SET( (VOS_CHAR *)pstMMPlmnInfoCnf + VOS_MSG_HEAD_LENGTH,
                0x00,
                (VOS_SIZE_T)(sizeof(TAF_MMA_MMPLMNINFO_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH) );

    pstMMPlmnInfoCnf->ulReceiverPid          = pstMMPlmnInfoReq->stCtrl.ulModuleId;
    pstMMPlmnInfoCnf->ulMsgName              = ID_TAF_MMA_MMPLMNINFO_QRY_CNF;
    pstMMPlmnInfoCnf->usClientId             = pstMMPlmnInfoReq->stCtrl.usClientId;
    pstMMPlmnInfoCnf->ucOpId                 = pstMMPlmnInfoReq->stCtrl.ucOpId;

    enCsServiceStatus = TAF_SDC_GetAppCsServiceStatus();
    enPsServiceStatus = TAF_SDC_GetAppPsServiceStatus();

    if ( (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == enCsServiceStatus)
      || (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == enPsServiceStatus) )
    {
        pstPlmnId = (TAF_PLMN_ID_STRU*)TAF_SDC_GetAppServicePlmnId();

        if ((pstPlmnId->Mcc == pstMmInfo->stOperatorPlmnId.ulMcc)
        &&  (pstPlmnId->Mnc == pstMmInfo->stOperatorPlmnId.ulMnc))
        {
            pstMMPlmnInfoCnf->stMmPlmnInfo.ucLongNameLen  = TAF_MMA_TranslateOperatorName2UCS2(&pstMmInfo->stOperatorNameLong,
                                                            (VOS_CHAR *)(pstMMPlmnInfoCnf->stMmPlmnInfo.aucLongName),
                                                             NAS_MML_MAX_OPER_LONG_NAME_LEN);

            pstMMPlmnInfoCnf->stMmPlmnInfo.ucShortNameLen = TAF_MMA_TranslateOperatorName2UCS2(&pstMmInfo->stOperatorNameShort,
                                                            (VOS_CHAR *)(pstMMPlmnInfoCnf->stMmPlmnInfo.aucShortName),
                                                            NAS_MML_MAX_OPER_SHORT_NAME_LEN);
        }
    }

    PS_SEND_MSG(WUEPS_PID_MMA, pstMMPlmnInfoCnf);

    return VOS_TRUE;
}

/*****************************************************************************
 Function Name     :   TAF_MMA_RcvPlmnQryReq_PreProc
 Description       :

 Input parameters  :   VOS_UINT32                          ulEventType,
                       struct MsgCB                       *pstMsg
 Outout parameters :
 Return Value      :   VOS_UINT32

 Modify History:
     1)  Date      :   2015-03-30
         Author    :   k902809
         Modify content :    Create

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvPlmnQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_LAST_CAMP_PLMN_QRY_REQ_STRU                    *pstPlmnReq = VOS_NULL_PTR;
    TAF_MMA_LAST_CAMP_PLMN_QRY_CNF_STRU                    *pstPlmnCnf = VOS_NULL_PTR;
    TAF_SDC_PLMN_ID_STRU                                   *pstCurrCampPlmnId = VOS_NULL_PTR;

    pstPlmnReq = (TAF_MMA_LAST_CAMP_PLMN_QRY_REQ_STRU *)pstMsg;

    pstPlmnCnf  = (TAF_MMA_LAST_CAMP_PLMN_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                                         WUEPS_PID_MMA,
                                                         sizeof(TAF_MMA_LAST_CAMP_PLMN_QRY_CNF_STRU));
    if (VOS_NULL_PTR == pstPlmnCnf)
    {
        TAF_LOG(WUEPS_PID_MMA, VOS_NULL_PTR, PS_LOG_LEVEL_ERROR,
                "TAF_MMA_RcvPlmnQryReq_PreProc: Memory Alloc Error for pMsg!");
        return VOS_TRUE;
    }

    PS_MEM_SET( (VOS_CHAR *)pstPlmnCnf + VOS_MSG_HEAD_LENGTH,
                0x00,
                (VOS_SIZE_T)(sizeof(TAF_MMA_LAST_CAMP_PLMN_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH) );

    pstPlmnCnf->ulReceiverPid          = pstPlmnReq->stCtrl.ulModuleId;
    pstPlmnCnf->ulMsgName              = ID_TAF_MMA_LAST_CAMP_PLMN_QRY_CNF;
    pstPlmnCnf->usClientId             = pstPlmnReq->stCtrl.usClientId;
    pstPlmnCnf->ucOpId                 = pstPlmnReq->stCtrl.ucOpId;

    pstCurrCampPlmnId = TAF_SDC_GetCurrCampPlmnId();

    pstPlmnCnf->stPlmnId.Mcc = pstCurrCampPlmnId->ulMcc;
    pstPlmnCnf->stPlmnId.Mnc = pstCurrCampPlmnId->ulMnc;

    MMA_PlmnId2Bcd(&(pstPlmnCnf->stPlmnId));

    PS_SEND_MSG(WUEPS_PID_MMA, pstPlmnCnf);

    return VOS_TRUE;
}

/* Added by k902809 for Iteration 11, Iteration 11 2015-3-24, end */
/* Added by y00322978 for CDMA 1X Iteration 11, 2015-3-24, begin */
/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMmaSetCqstReq_PreProc
 功能描述  : AT^CQST
 输入参数  : ulEventType *pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMmaSetCqstReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_QUICKSTART_SET_REQ_STRU                        *pstRcvMsg;
    TAF_MMA_QUICKSTART_SET_CNF_STRU                        *pstQuickStartSetCnf;
    NAS_NVIM_FOLLOWON_OPENSPEED_FLAG_STRU                   stQuickStartFlg;
    VOS_UINT32                                              ulResult;

    pstRcvMsg              = (TAF_MMA_QUICKSTART_SET_REQ_STRU *)pstMsg;

    pstQuickStartSetCnf    = (TAF_MMA_QUICKSTART_SET_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MMA,
                                                                sizeof(TAF_MMA_QUICKSTART_SET_CNF_STRU));
    if (VOS_NULL_PTR == pstQuickStartSetCnf)
    {
        MMA_ERRORLOG("TAF_MMA_RcvMmaSetCqstReq_PreProc: ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }

    PS_MEM_SET(((VOS_UINT8 *)pstQuickStartSetCnf + VOS_MSG_HEAD_LENGTH),
               0x00,
               (sizeof(TAF_MMA_QUICKSTART_SET_CNF_STRU) - VOS_MSG_HEAD_LENGTH));

    /* 填写消息头部信息 */
    pstQuickStartSetCnf->ulReceiverPid          = pstRcvMsg->stCtrl.ulModuleId;
    pstQuickStartSetCnf->ulMsgName              = ID_TAF_MMA_QUICKSTART_SET_CNF;
    pstQuickStartSetCnf->usClientId             = pstRcvMsg->stCtrl.usClientId;
    pstQuickStartSetCnf->ucOpId                 = pstRcvMsg->stCtrl.ucOpId;

    if (0 == pstRcvMsg->ulQuickStartMode)
    {
        stQuickStartFlg.ulQuickStartSta = MMA_QUICK_START_DISABLE;
    }
    else
    {
        stQuickStartFlg.ulQuickStartSta = MMA_QUICK_START_ENABLE;
    }

    ulResult = MMA_WriteValue2Nvim(en_NV_Item_FollowOn_OpenSpeed_Flag,
                                  (VOS_VOID *)(&stQuickStartFlg),
                                   sizeof(NAS_NVIM_FOLLOWON_OPENSPEED_FLAG_STRU));

    if (MMA_SUCCESS != ulResult)
    {
        pstQuickStartSetCnf->ucResult   = TAF_PARA_WRITE_NVIM_ERROR;

        /* 发送消息 */
        PS_SEND_MSG(WUEPS_PID_MMA, pstQuickStartSetCnf);
    }
    else
    {
        gstMmaValue.ulQuickStartFlg     = stQuickStartFlg.ulQuickStartSta;

        pstQuickStartSetCnf->ucResult   = TAF_PARA_OK;

        /* 发送消息 */
        PS_SEND_MSG(WUEPS_PID_MMA, pstQuickStartSetCnf);
    }

    return VOS_TRUE;

}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMmaQryQuickStartReq_PreProc
 功能描述  : 快速开机查询命令的消息处理
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年3月27日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMmaQryQuickStartReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_QUICKSTART_QRY_REQ_STRU     *pstRcvMsg;
    TAF_MMA_QUICKSTART_QRY_CNF_STRU     *pstQuickStartQryCnf;

    pstRcvMsg  = (TAF_MMA_QUICKSTART_QRY_REQ_STRU *)pstMsg;

    pstQuickStartQryCnf = (TAF_MMA_QUICKSTART_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MMA,
                                                             sizeof(TAF_MMA_QUICKSTART_QRY_CNF_STRU));
    if (VOS_NULL_PTR == pstQuickStartQryCnf)
    {
        MMA_ERRORLOG("TAF_MMA_RcvMmaQryQuickStartReq_PreProc: ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }

    PS_MEM_SET(((VOS_UINT8 *)pstQuickStartQryCnf + VOS_MSG_HEAD_LENGTH),
               0x00,
               (sizeof(TAF_MMA_QUICKSTART_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH));


    pstQuickStartQryCnf->ulReceiverPid            = WUEPS_PID_AT;
    pstQuickStartQryCnf->ulMsgName                = ID_TAF_MMA_QUICKSTART_QRY_CNF;
    pstQuickStartQryCnf->usClientId               = pstRcvMsg->stCtrl.usClientId;
    pstQuickStartQryCnf->ucOpId                   = pstRcvMsg->stCtrl.ucOpId;
    pstQuickStartQryCnf->enErrorCause             = TAF_ERR_NO_ERROR;

    if (MMA_QUICK_START_ENABLE == gstMmaValue.ulQuickStartFlg)
    {
        pstQuickStartQryCnf->ulQuickStartMode = 1;
    }
    else
    {
        pstQuickStartQryCnf->ulQuickStartMode = 0;
    }

    /* 消息发送 */
    PS_SEND_MSG(WUEPS_PID_MMA, pstQuickStartQryCnf);

    return VOS_TRUE;

}
VOS_UINT32 TAF_MMA_RcvMmaQryCrpnReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_PH_OPERATOR_NAME_STRU           stOrigOperName;
    VOS_UINT8                           ucMaxMncLen;
    VOS_UINT8                           ucSendNum;
    VOS_UINT8                           ucLeftNum;
    VOS_UINT32                          ulTotalNum;
    TAF_MMA_CRPN_QRY_INFO_STRU         *pstMnMmaCrpnQryCnf;
    TAF_PH_OPERATOR_NAME_STRU          *pstOperName;
    VOS_UINT32                          i;
    TAF_MMA_CRPN_QRY_REQ_STRU          *pstRcvMsg;

    pstRcvMsg = (TAF_MMA_CRPN_QRY_REQ_STRU *)pstMsg;

    /* 变量初始化 */
    pstMnMmaCrpnQryCnf = VOS_NULL_PTR;
    pstOperName        = VOS_NULL_PTR;
    PS_MEM_SET(&stOrigOperName, 0, sizeof(TAF_PH_OPERATOR_NAME_STRU));

    /* 获取符合条件的PLMN总个数 */
    MMA_CrpnQuery_GetPlmnNum(&stOrigOperName, &pstRcvMsg->stCrpnQryPara, &ucMaxMncLen, &ulTotalNum);

    /* 检查是否有网络，并且MNC是否合法 */
    if ((VOS_FALSE == TAF_MMA_IsMncLenVaild(ucMaxMncLen)
      || (0 == ulTotalNum)))
    {
        TAF_MMA_SndCrpnQryCnf(&pstRcvMsg->stCtrl, VOS_NULL_PTR, TAF_ERR_PB_WRONG_INDEX);

        return VOS_TRUE;
    }

    /* 分配PLMN INFO的存储空间 */
    pstOperName = (TAF_PH_OPERATOR_NAME_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA,
                                                            sizeof(TAF_PH_OPERATOR_NAME_STRU) * ulTotalNum);

    if (VOS_NULL_PTR == pstOperName)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "MMA_CrpnQuery: Alloc mem fail!");
        return VOS_TRUE;
    }

    /* 获取PLMN INFO */
    if (0 == pstRcvMsg->stCrpnQryPara.ucPlmnType)
    {
        MMA_PhoneGetAllOperInfoByLongNameFromOperTbl(&stOrigOperName, pstOperName);

    }
    else if(1 == pstRcvMsg->stCrpnQryPara.ucPlmnType)
    {
        MMA_PhoneGetAllOperInfoByShortNameFromOperTbl(&stOrigOperName, pstOperName);

    }
    else
    {
        /* Mcc */
        if(VOS_ERR == MMA_String2Hex(pstRcvMsg->stCrpnQryPara.aucPlmnName,
                                     3,
                                     &pstOperName->PlmnId.Mcc))
        {
            /* 释放动态内存 */
            PS_MEM_FREE(WUEPS_PID_MMA, pstOperName);

            TAF_MMA_SndCrpnQryCnf(&pstRcvMsg->stCtrl, VOS_NULL_PTR, TAF_ERR_PARA_ERROR);

            return VOS_TRUE;
        }
        /* Mnc */
        if(VOS_ERR == MMA_String2Hex(&pstRcvMsg->stCrpnQryPara.aucPlmnName[3],
                                      pstRcvMsg->stCrpnQryPara.usPlmnNameLen - 3,
                                     &pstOperName->PlmnId.Mnc))
        {
            /* 释放动态内存 */
            PS_MEM_FREE(WUEPS_PID_MMA, pstOperName);

            TAF_MMA_SndCrpnQryCnf(&pstRcvMsg->stCtrl, VOS_NULL_PTR, TAF_ERR_PARA_ERROR);
            return VOS_TRUE;
        }

        pstOperName->PlmnId.Mcc |= 0xFFFFF000;
        pstOperName->PlmnId.Mnc |= (0xFFFFFFFF << ((pstRcvMsg->stCrpnQryPara.usPlmnNameLen-3)*4));

        if (MMA_SUCCESS != MMA_PhoneFindNtwkNameByPlmnId(pstOperName))
        {
            /* 释放动态内存 */
            PS_MEM_FREE(WUEPS_PID_MMA, pstOperName);

            TAF_MMA_SndCrpnQryCnf(&pstRcvMsg->stCtrl, VOS_NULL_PTR, TAF_ERR_PB_WRONG_INDEX);

            return VOS_TRUE;
        }
    }


    /* 申请消息内存 */
    pstMnMmaCrpnQryCnf = (TAF_MMA_CRPN_QRY_INFO_STRU *)PS_MEM_ALLOC(WUEPS_PID_MMA,
                                                            sizeof(TAF_MMA_CRPN_QRY_INFO_STRU));
    if (VOS_NULL_PTR == pstMnMmaCrpnQryCnf)
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "MMA_CrpnQuery: Alloc mem fail!");

        /* 释放动态内存 */
        PS_MEM_FREE(WUEPS_PID_MMA, pstOperName);
        return VOS_TRUE;
    }
    PS_MEM_SET(pstMnMmaCrpnQryCnf, 0, sizeof(TAF_MMA_CRPN_QRY_INFO_STRU));

    /* 发送消息赋值 */
    pstMnMmaCrpnQryCnf->ucMaxMncLen = ucMaxMncLen;

    ucSendNum = pstRcvMsg->stCrpnQryPara.ucQryNum;

    /* 如果要求读取的index大于等于CRPN的总数目，认为是最后一次上报，上报一个空列表 */
    if (pstRcvMsg->stCrpnQryPara.ucCurrIndex >= (VOS_UINT8)ulTotalNum)
    {
        pstMnMmaCrpnQryCnf->ucTotalNum = 0;
        ucSendNum = 0;
    }
    else
    {
        ucLeftNum = (VOS_UINT8)ulTotalNum - pstRcvMsg->stCrpnQryPara.ucCurrIndex;

        if (ucSendNum >= ucLeftNum)
        {
            ucSendNum = ucLeftNum;
        }
        pstMnMmaCrpnQryCnf->ucTotalNum  = (VOS_UINT8)ucSendNum;
        pstMnMmaCrpnQryCnf->ucCurrIndex = pstRcvMsg->stCrpnQryPara.ucCurrIndex;
    }

    PS_MEM_CPY(&pstMnMmaCrpnQryCnf->stMnMmaCrpnQry, &pstRcvMsg->stCrpnQryPara, sizeof(TAF_MMA_CRPN_QRY_PARA_STRU));

    for (i = 0; i < ucSendNum; i++)
    {
        PS_MEM_CPY(&pstMnMmaCrpnQryCnf->stOperNameList[i], (pstOperName + i + pstRcvMsg->stCrpnQryPara.ucCurrIndex), sizeof(TAF_PH_OPERATOR_NAME_STRU));
    }

    TAF_MMA_SndCrpnQryCnf(&pstRcvMsg->stCtrl, pstMnMmaCrpnQryCnf, TAF_ERR_NO_ERROR);

    /* 释放动态内存 */
    PS_MEM_FREE(WUEPS_PID_MMA, pstOperName);
    PS_MEM_FREE(WUEPS_PID_MMA, pstMnMmaCrpnQryCnf);

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMmaSetCmmReq_PreProc
 功能描述  : AT^CMM 设置
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年3月24日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMmaSetCmmReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CMM_SET_REQ_STRU           *pstRcvMsg;
    TAF_MMA_CMM_SET_CNF_STRU           *pstMmaSetCnfMsg;

    pstRcvMsg       = (TAF_MMA_CMM_SET_REQ_STRU *)pstMsg;

    /* 申请消息内存 */
    pstMmaSetCnfMsg = (TAF_MMA_CMM_SET_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(
                                             WUEPS_PID_MMA,
                                             sizeof(TAF_MMA_CMM_SET_CNF_STRU));

    if (VOS_NULL_PTR == pstMmaSetCnfMsg )
    {
        NAS_ERROR_LOG(WUEPS_PID_MMA, "TAF_MMA_RcvMmaSetCmmReq_PreProc:ERROR: Memory Alloc Error for pMsg");
        return VOS_TRUE;
    }

    PS_MEM_SET(((VOS_UINT8 *)pstMmaSetCnfMsg + VOS_MSG_HEAD_LENGTH),
               0x00,
               (sizeof(TAF_MMA_CMM_SET_CNF_STRU) - VOS_MSG_HEAD_LENGTH));

    /* 填写消息 */
    pstMmaSetCnfMsg->ulReceiverPid          = WUEPS_PID_AT;
    pstMmaSetCnfMsg->ulMsgName              = ID_TAF_MMA_CMM_SET_CNF;
    PS_MEM_CPY(&pstMmaSetCnfMsg->stCtrl, &pstRcvMsg->stCtrl, sizeof(TAF_MMA_CTRL_STRU));

    pstMmaSetCnfMsg->ulResult = Mm_AtTest(&pstRcvMsg->stCmmSetReq, &pstMmaSetCnfMsg->stAtCmdRslt);

    /* 发送消息 */
    PS_SEND_MSG(WUEPS_PID_MMA, pstMmaSetCnfMsg);

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMmaTestSyscfgReq_PreProc
 功能描述  : syscfg test req 消息预处理
 输入参数  : 无
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年3月26日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMmaTestSyscfgReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_SYSCFG_TEST_REQ_STRU       *pstRcvMsg;
    TAF_MMA_SYSCFG_TEST_CNF_STRU       *pstSysCfgTestCnf;

    pstRcvMsg        = (TAF_MMA_SYSCFG_TEST_REQ_STRU *)pstMsg;
    pstSysCfgTestCnf = (TAF_MMA_SYSCFG_TEST_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MMA,
                                                                                    sizeof(TAF_MMA_SYSCFG_TEST_CNF_STRU));
    if (VOS_NULL_PTR == pstSysCfgTestCnf)
    {
        MMA_ERRORLOG("TAF_MMA_RcvMmaTestSyscfgReq_PreProc: ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }

    PS_MEM_SET(((VOS_UINT8 *)pstSysCfgTestCnf + VOS_MSG_HEAD_LENGTH),
               0x00,
               (sizeof(TAF_MMA_SYSCFG_TEST_CNF_STRU) - VOS_MSG_HEAD_LENGTH));

    /* 填写消息头部信息 */
    pstSysCfgTestCnf->ulReceiverPid          = pstRcvMsg->stCtrl.ulModuleId;
    pstSysCfgTestCnf->ulMsgName              = ID_TAF_MMA_SYSCFG_TEST_CNF;
    PS_MEM_CPY(&pstSysCfgTestCnf->stCtrl, &pstRcvMsg->stCtrl, sizeof(TAF_MMA_CTRL_STRU));
    pstSysCfgTestCnf->enErrorCause           = TAF_ERR_NO_ERROR;

    Taf_GetSyscfgBandGroupStr(&pstSysCfgTestCnf->stBandInfo);

    PS_SEND_MSG(WUEPS_PID_MMA, pstSysCfgTestCnf);

    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMmaCsnrQryReq_PreProc
 功能描述  : CSNR qry 消息处理并回复
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年3月28日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMmaCsnrQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CSNR_QRY_REQ_STRU          *pstRcvMsg;
    TAF_MMA_CSNR_QRY_CNF_STRU          *pstCsnrQryCnf;
    VOS_UINT32                          ulResult;
    TAF_MMA_CSNR_PARA_STRU              pstCsnrParaStr ;

    pstRcvMsg = (TAF_MMA_CSNR_QRY_REQ_STRU *)pstMsg;

    pstCsnrQryCnf = (TAF_MMA_CSNR_QRY_CNF_STRU *)PS_ALLOC_MSG_WITH_HEADER_LEN(WUEPS_PID_MMA,
                                                 sizeof(TAF_MMA_CSNR_QRY_CNF_STRU));
    if (VOS_NULL_PTR == pstCsnrQryCnf)
    {
        MMA_ERRORLOG("TAF_MMA_RcvMmaCsnrQryReq_PreProc: ERROR:ALLOC MEMORY FAIL.");
        return VOS_TRUE;
    }

    PS_MEM_SET(((VOS_UINT8 *)pstCsnrQryCnf + VOS_MSG_HEAD_LENGTH),
               0x00,
               (sizeof(TAF_MMA_CSNR_QRY_CNF_STRU) - VOS_MSG_HEAD_LENGTH));

    /* 填写消息头部信息 */
    pstCsnrQryCnf->ulReceiverPid          = pstRcvMsg->stCtrl.ulModuleId;
    pstCsnrQryCnf->ulMsgName              = ID_TAF_MMA_CSNR_QRY_CNF;
    PS_MEM_CPY(&pstCsnrQryCnf->stCtrl, &pstRcvMsg->stCtrl, sizeof(TAF_MMA_CTRL_STRU));

    /* 调用 Taf_QueryCSNR 获取信号值 */
    ulResult = Taf_QueryCSNR(&pstCsnrParaStr.sCpichRscp, &pstCsnrParaStr.sCpichEcNo);

    /* 上报AT^CSNR参数查询结果事件 */
    if (VOS_OK != ulResult)
    {
        pstCsnrQryCnf->enErrorCause = TAF_ERR_ERROR;
    }
    else
    {
        pstCsnrQryCnf->enErrorCause = TAF_ERR_NO_ERROR;
        PS_MEM_CPY(&pstCsnrQryCnf->stCsnrPara, &pstCsnrParaStr, sizeof(TAF_MMA_CSNR_PARA_STRU));
    }

    PS_SEND_MSG(WUEPS_PID_MMA, pstCsnrQryCnf);
    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMmaCsqQryReq_PreProc
 功能描述  : CSQ QRY消息预处理
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年3月28日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMmaCsqQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CSQ_QRY_REQ_STRU            *pstRcvMsg;
    VOS_UINT32                           ulI;
    TAF_PH_RSSI_STRU                     stMmaRssiTmp;
    TAF_SDC_CTX_STRU                    *pstSdcCtx = VOS_NULL_PTR;

    TAF_SDC_SYS_MODE_ENUM_UINT8         enSysMode;

    PS_MEM_SET(&stMmaRssiTmp, 0, sizeof(stMmaRssiTmp));

    pstRcvMsg = (TAF_MMA_CSQ_QRY_REQ_STRU *)pstMsg;
    pstSdcCtx = TAF_SDC_GetSdcCtx();
    enSysMode = TAF_SDC_GetAppSysMode();

    ulI = 0;

    /* NO RF时，不上报信号质量，查询时，ucRssiNum填为0 */
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, begin */
    if (VOS_TRUE == TAF_SDC_Get3GppRfAvailFlag())
    {
        stMmaRssiTmp.ucRssiNum = 1;
    }
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-18, end */

    /* RSSI 信息从MMA 移至SDC 保存 */
    switch(enSysMode)
    {
        case TAF_SDC_SYS_MODE_GSM:

            MMA_GetRssiLevelFromValue(pstSdcCtx->stAppNetworkInfo.stRssiValue.u.st2GCellSignInfo.sRssiValue,
                                      &(stMmaRssiTmp.aRssi[ulI].ucRssiValue));
            break;

        case TAF_SDC_SYS_MODE_WCDMA:

            MMA_GetRssiLevelFromValue(pstSdcCtx->stAppNetworkInfo.stRssiValue.u.st3GCellSignInfo.sRscpValue,
                                      &(stMmaRssiTmp.aRssi[ulI].ucRssiValue));
            break;

#if ( FEATURE_ON == FEATURE_LTE )
        case TAF_SDC_SYS_MODE_LTE:

            /* 4G的算法与GU不同, 单独封装函数, 与atSetCsqCnfSameProc处理相同 */
            TAF_MMA_GetRssiLevelFromLteRssi(pstSdcCtx->stAppNetworkInfo.stRssiValue.u.st4GCellSignInfo.sRssi,
                                      &(stMmaRssiTmp.aRssi[ulI].ucRssiValue));
            break;
#endif

        default:
            stMmaRssiTmp.ucRssiNum = 0;

            MMA_WARNINGLOG1("MMA_PhModeReport():WARNING:TAF_SDC_GetAppSysMode():", enSysMode);
            break;

    }

    stMmaRssiTmp.aRssi[ulI].ucChannalQual = pstSdcCtx->stAppNetworkInfo.stRssiValue.ucChannalQual;


    TAF_MMA_SndCsqQryCnf(&pstRcvMsg->stCtrl, &stMmaRssiTmp);
    return VOS_TRUE;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMmaCsqLvlQryReq_PreProc
 功能描述  : csqlvl qry req消息处理
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年3月28日
    作    者   : y00322978
    修改内容   : 新生成函数
  2.日    期   : 2015年02月11日
    作    者   : l00305157
    修改内容   : Service_State_Optimize_PhaseII 项目修改
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMmaCsqLvlQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_CSQLVL_QRY_REQ_STRU        *pstRcvMsg;
    TAF_MMA_CSQLVL_PARA_STRU            stCsqLvlPara;
    TAF_MMA_CSQLVLEXT_PARA_STRU         stCsqLvlExtPara;
    VOS_INT16                           sCpichRscp;
    VOS_INT16                           sCpichEcNo;
    VOS_UINT16                          ausCsqLvl[AT_CSQLVL_MAX_NUM]={105,102,97};
    TAF_ERROR_CODE_ENUM_UINT32          enErrorCode = TAF_ERR_ERROR;

    pstRcvMsg = (TAF_MMA_CSQLVL_QRY_REQ_STRU *)pstMsg;
    PS_MEM_SET(&stCsqLvlPara, 0, sizeof(TAF_MMA_CSQLVL_PARA_STRU));
    PS_MEM_SET(&stCsqLvlExtPara, 0, sizeof(TAF_MMA_CSQLVLEXT_PARA_STRU));

    sCpichRscp = RRC_NAS_MEASURE_INVALID_VALUE;
    sCpichEcNo = MMA_RSCP_MIN;

    /* 获取 Rscp 值 */
    if (TAF_SUCCESS != Taf_QueryCSNR(&sCpichRscp, &sCpichEcNo))
    {
        TAF_LOG(WUEPS_PID_MMA, VOS_NULL, PS_LOG_LEVEL_WARNING, "TAF_MMA_RcvMmaCsqLvlQryReq_PreProc: Taf_QueryCSNR fail.");
        TAF_MMA_SndCsqLvlQryCnf(&pstRcvMsg->stCtrl, &stCsqLvlPara, &stCsqLvlExtPara, TAF_ERR_ERROR);
        return VOS_TRUE;
    }

    /* 取绝对值 */
    stCsqLvlPara.usRscp = (VOS_UINT16)(-sCpichRscp);

    /* 按规则转换为相应的格数 */
    if ( stCsqLvlPara.usRscp <= ausCsqLvl[2] )
    {
        stCsqLvlPara.usLevel = AT_CSQLVL_LEVEL_3;
    }
    else if ( stCsqLvlPara.usRscp <= ausCsqLvl[1] )
    {
        stCsqLvlPara.usLevel = AT_CSQLVL_LEVEL_2;
    }
    else if ( stCsqLvlPara.usRscp <= ausCsqLvl[0] )
    {
        stCsqLvlPara.usLevel = AT_CSQLVL_LEVEL_1;
    }
    else
    {
        stCsqLvlPara.usLevel = AT_CSQLVL_LEVEL_0;
    }

    /* 根据信号强度获取到CSQLVLEXT命令的信号格数，CSQLVLEXT仅关心INDEX为0的RSSI值 */
    /* Modified by l00305157 for Service_State_Optimize_PhaseII, 2015-02-11, begin */
    if (TAF_SDC_REPORT_SRVSTA_NORMAL_SERVICE != TAF_SDC_GetAppServiceStatus())
    /* Modified by l00305157 for Service_State_Optimize_PhaseII, 2015-02-11, begin */
    {
     stCsqLvlExtPara.enRssilv = MN_PH_CSQLVLEXT_RSSILV_VALUE_0;
    }
    /* Modified by l00305157 for Service_State_Optimize_PhaseI, 2014-11-29, begin */
    else
    {
     /*获取当前RSSI的值,调用API*/
        if (TAF_SDC_SYS_MODE_GSM == TAF_SDC_GetAppSysMode())
        {
            MN_PH_GetRssilvForCsqlvlExt(TAF_SDC_GetSdcCtx()->stAppNetworkInfo.stRssiValue.u.st2GCellSignInfo.sRssiValue,
                                     TAF_SDC_GetAppSysMode(),
                                     &stCsqLvlExtPara.enRssilv);
        }
        else
        {
            MN_PH_GetRssilvForCsqlvlExt(TAF_SDC_GetSdcCtx()->stAppNetworkInfo.stRssiValue.u.st3GCellSignInfo.sRscpValue,
                                     TAF_SDC_GetAppSysMode(),
                                     &stCsqLvlExtPara.enRssilv);
        }
    }
    /* Modified by l00305157 for Service_State_Optimize_PhaseI, 2014-11-29, end */
    /* 当前版本不支持CSQLVLEXT命令的比特误码率百分比查询，固定返回99 */
    stCsqLvlExtPara.enBer = MN_PH_CSQLVLEXT_BER_VALUE_99;

    enErrorCode = TAF_ERR_NO_ERROR;

    TAF_MMA_SndCsqLvlQryCnf(&pstRcvMsg->stCtrl, &stCsqLvlPara, &stCsqLvlExtPara, enErrorCode);
    return VOS_TRUE;
}

/* Added by y00322978 for CDMA 1X Iteration 11, 2015-3-24, end */

VOS_UINT32 TAF_MMA_RcvAccessModeQry_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_ACCESS_MODE_QRY_REQ_STRU    *pstRcvMsg;
    TAF_MMA_RAT_ORDER_STRU               stRatOrder;
    NAS_MMA_NVIM_ACCESS_MODE_STRU        stAccessMode;
    VOS_UINT32                           ulLength;
    MN_MMA_CPAM_RAT_TYPE_ENUM_UINT8      enAccessMode;
    TAF_PH_PLMN_PRIO                     ucPlmnPrio;

    pstRcvMsg = (TAF_MMA_ACCESS_MODE_QRY_REQ_STRU *)pstMsg;

    ulLength        = 0;
    PS_MEM_SET(&stRatOrder,   0x00, sizeof(stRatOrder));
    PS_MEM_SET(&stAccessMode, 0x00, sizeof(NAS_MMA_NVIM_ACCESS_MODE_STRU));

    TAF_MMA_ReadNvimRatPrioList(&stRatOrder);


    (VOS_VOID)NV_GetLength(en_NV_Item_MMA_AccessMode, &ulLength);

    if (NV_OK != NV_Read(en_NV_Item_MMA_AccessMode ,
                        &stAccessMode,
                         ulLength))

    {
        stAccessMode.aucAccessMode[1] = TAF_PLMN_PRIO_AUTO;
    }



    /* CPAM查询的优先级与syscfg查询的优先级不一样需要进行转换 */
    if ( TAF_PLMN_GSM_PRIO == stAccessMode.aucAccessMode[1] )
    {
        ucPlmnPrio = MN_MMA_CPAM_RAT_PRIO_GSM;
    }
    else
    {
        ucPlmnPrio = MN_MMA_CPAM_RAT_PRIO_WCDMA;
    }

    /* 提取GU模接入优先级信息 */
    /* MN_MMA_GetCpamGURatPrio((&pAccessMode->PlmnPrio), &stRatOrder); */

    /* 获取GU模接入技术信息 */
    MN_MMA_GetCpamGUAccessRat(&enAccessMode, &stRatOrder);

    /* Snd access Mode qry cnf */
    TAF_MMA_SndAccessModeQryCnf(&pstRcvMsg->stCtrl, enAccessMode, ucPlmnPrio);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvCopsQry_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT32                                              ulIndex;
    TAF_SDC_ENHANCED_OPERATOR_NAME_INFO_STRU               *pstEnhancedOperNameInfo = VOS_NULL_PTR;
    TAF_SDC_PLMN_ID_STRU                                   *pstAppServicePlmnId = VOS_NULL_PTR;
    TAF_PH_NETWORKNAME_STRU                                 stOperatorName;
    TAF_MMA_MM_INFO_PLMN_NAME_STRU                          stMmInfoPlmnName;
    TAF_MMA_COPS_QRY_REQ_STRU                              *pstRcvMsg;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8                       enCsServiceStatus;
    TAF_SDC_SERVICE_STATUS_ENUM_UINT8                       enPsServiceStatus;
    pstRcvMsg = (TAF_MMA_COPS_QRY_REQ_STRU *)pstMsg;

    enCsServiceStatus = TAF_SDC_GetAppCsServiceStatus();
    enPsServiceStatus = TAF_SDC_GetAppPsServiceStatus();

    pstAppServicePlmnId     = TAF_SDC_GetAppServicePlmnId();
    pstEnhancedOperNameInfo = TAF_SDC_GetEnhancedOperatorNameInfo();

    PS_MEM_SET(&stOperatorName, 0, sizeof(stOperatorName));
    PS_MEM_SET(&stMmInfoPlmnName, 0, sizeof(stMmInfoPlmnName));
    stOperatorName.NameFormat                   = TAF_SDC_GetCopsFormatType();
    stOperatorName.RaMode                       = TAF_SDC_GetAppSysMode();
    stOperatorName.PlmnSelMode                  = TAF_MMA_GetPlmnSelectionMode();
    stOperatorName.Name.aucOperatorNameLong[0]  = '\0';
    stOperatorName.Name.aucOperatorNameShort[0] = '\0';
    stOperatorName.Name.PlmnId.Mcc              = 0;
    stOperatorName.Name.PlmnId.Mnc              = 0;

    /* mmc对不同状态的plmn Id判断不准确，使用随SYS_INFO上报的PLMN ID, 加上当前服务状态的判定 */
     /* Modified by l00305157 for Service_State_Optimize_PhaseI, 2014-11-29, begin */
    if ( (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == enCsServiceStatus)
      || (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE == enPsServiceStatus) )
     /* Modified by l00305157 for Service_State_Optimize_PhaseI, 2014-11-29, end */
    {
        stOperatorName.Name.PlmnId.Mcc = pstAppServicePlmnId->ulMcc;
        stOperatorName.Name.PlmnId.Mnc = pstAppServicePlmnId->ulMnc;
    }

    if (TAF_SDC_COPS_FORMAT_TYPE_NUMERIC == TAF_SDC_GetCopsFormatType())
    {
        MMA_PlmnId2Bcd(&(stOperatorName.Name.PlmnId));
        TAF_MMA_SndCopsQryCnf(&pstRcvMsg->stCtrl, &stOperatorName);
        return VOS_TRUE;
    }

    for (ulIndex = 0; ulIndex < pstEnhancedOperNameInfo->stOperNameTypePrio.ucOperNamePrioNum; ulIndex++)
    {
        /* 从PNN中获取运营商名称*/
        if (TAF_SDC_OPER_NAME_TYPE_PNN == pstEnhancedOperNameInfo->stOperNameTypePrio.aucOperNameTypePrio[ulIndex])
        {
            /* 从PNN中获取运营商名称成功，则直接返回*/
            if (VOS_TRUE == TAF_MMA_GetPnnNetWorkName((TAF_SDC_PLMN_ID_STRU *)&stOperatorName.Name.PlmnId, &stMmInfoPlmnName, TAF_SDC_NETWORK_NAME_CODE_SCHEME_7BIT))
            {
                PS_MEM_CPY(stOperatorName.Name.aucOperatorNameLong, stMmInfoPlmnName.aucLongName, sizeof(stOperatorName.Name.aucOperatorNameLong));
                PS_MEM_CPY(stOperatorName.Name.aucOperatorNameShort, stMmInfoPlmnName.aucShortName, sizeof(stOperatorName.Name.aucOperatorNameShort));
                MMA_PlmnId2Bcd(&(stOperatorName.Name.PlmnId));
                TAF_MMA_SndCopsQryCnf(&pstRcvMsg->stCtrl, &stOperatorName);

                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, begin */
                /* 将COPS RESULT 信息LOG出来 */
                TAF_MMA_LogCopsRltInfo(&stOperatorName, TAF_SDC_LOG_OPER_NAME_TYPE_PNN);
                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, end */

                return VOS_TRUE;
            }

            /* 如果从PNN中获取7bit编码的运营商名称失败，获取UCS2编码的运营商名称如果成功，cops需要返回空，应用通过^eonsucs2查询获取ucs2编码的运营商名称 */
            if (VOS_TRUE == TAF_MMA_GetPnnNetWorkName((TAF_SDC_PLMN_ID_STRU *)&stOperatorName.Name.PlmnId, &stMmInfoPlmnName, TAF_SDC_NETWORK_NAME_CODE_SCHEME_COPS_UCS2))
            {
                MMA_PlmnId2Bcd(&(stOperatorName.Name.PlmnId));
                TAF_MMA_SndCopsQryCnf(&pstRcvMsg->stCtrl, &stOperatorName);

                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, begin */
                /* 将COPS RESULT 信息LOG出来 */
                TAF_MMA_LogCopsRltInfo(&stOperatorName, TAF_SDC_LOG_OPER_NAME_TYPE_PNN);
                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, end */

                return VOS_TRUE;
            }
        }

        /* 从CPHS中获取运营商名称*/
        if (TAF_SDC_OPER_NAME_TYPE_CPHS == pstEnhancedOperNameInfo->stOperNameTypePrio.aucOperNameTypePrio[ulIndex])
        {
            /* 从CPHS中获取运营商名称成功，则直接返回*/
            if (VOS_TRUE == TAF_MMA_GetCphsNetWorkName(&stMmInfoPlmnName, TAF_SDC_NETWORK_NAME_CODE_SCHEME_7BIT))
            {
                PS_MEM_CPY(stOperatorName.Name.aucOperatorNameLong, stMmInfoPlmnName.aucLongName, sizeof(stOperatorName.Name.aucOperatorNameLong));
                PS_MEM_CPY(stOperatorName.Name.aucOperatorNameShort, stMmInfoPlmnName.aucShortName, sizeof(stOperatorName.Name.aucOperatorNameShort));
                MMA_PlmnId2Bcd(&(stOperatorName.Name.PlmnId));
                TAF_MMA_SndCopsQryCnf(&pstRcvMsg->stCtrl, &stOperatorName);

                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, begin */
                /* 将COPS RESULT 信息LOG出来 */
                TAF_MMA_LogCopsRltInfo(&stOperatorName, TAF_SDC_LOG_OPER_NAME_TYPE_CPHS);
                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, end */

                return VOS_TRUE;
            }
        }

        /* 从MM INFO中获取运营商名称*/
        if (TAF_SDC_OPER_NAME_TYPE_MMINFO == pstEnhancedOperNameInfo->stOperNameTypePrio.aucOperNameTypePrio[ulIndex])
        {
            /* 从MM INFO中获取运营商名称成功，则直接返回*/
            if (VOS_TRUE == TAF_MMA_GetMmInfoNetWorkName((TAF_SDC_PLMN_ID_STRU *)&stOperatorName.Name.PlmnId, &stMmInfoPlmnName, TAF_SDC_NETWORK_NAME_CODE_SCHEME_7BIT))
            {
                PS_MEM_CPY(stOperatorName.Name.aucOperatorNameLong, stMmInfoPlmnName.aucLongName, sizeof(stOperatorName.Name.aucOperatorNameLong));
                PS_MEM_CPY(stOperatorName.Name.aucOperatorNameShort, stMmInfoPlmnName.aucShortName, sizeof(stOperatorName.Name.aucOperatorNameShort));
                MMA_PlmnId2Bcd(&(stOperatorName.Name.PlmnId));
                TAF_MMA_SndCopsQryCnf(&pstRcvMsg->stCtrl, &stOperatorName);

                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, begin */
                /* 将COPS RESULT 信息LOG出来 */
                TAF_MMA_LogCopsRltInfo(&stOperatorName, TAF_SDC_LOG_OPER_NAME_TYPE_MMINFO);
                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, end */

                return VOS_TRUE;
            }

            /* 如果从MM INFO中获取7bit编码的运营商名称失败，获取UCS2编码的运营商名称如果成功，cops需要返回空，应用通过^eonsucs2查询获取ucs2编码的运营商名称 */
            if (VOS_TRUE == TAF_MMA_GetMmInfoNetWorkName((TAF_SDC_PLMN_ID_STRU *)&stOperatorName.Name.PlmnId, &stMmInfoPlmnName, TAF_SDC_NETWORK_NAME_CODE_SCHEME_COPS_UCS2))
            {
                MMA_PlmnId2Bcd(&(stOperatorName.Name.PlmnId));
                TAF_MMA_SndCopsQryCnf(&pstRcvMsg->stCtrl, &stOperatorName);

                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, begin */
                /* 将COPS RESULT 信息LOG出来 */
                TAF_MMA_LogCopsRltInfo(&stOperatorName, TAF_SDC_LOG_OPER_NAME_TYPE_MMINFO);
                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, end */

                return VOS_TRUE;
            }
        }

        /* 从SPN中获取运营商名称*/
        if (TAF_SDC_OPER_NAME_TYPE_SPN == pstEnhancedOperNameInfo->stOperNameTypePrio.aucOperNameTypePrio[ulIndex])
        {
            /* 从SPN中获取运营商名称成功，则直接返回*/
            if (VOS_TRUE == TAF_MMA_GetSpnNetWorkName((TAF_SDC_PLMN_ID_STRU *)&stOperatorName.Name.PlmnId, &stMmInfoPlmnName, TAF_SDC_NETWORK_NAME_CODE_SCHEME_7BIT))
            {
                PS_MEM_CPY(stOperatorName.Name.aucOperatorNameLong, stMmInfoPlmnName.aucLongName, sizeof(stOperatorName.Name.aucOperatorNameLong));
                PS_MEM_CPY(stOperatorName.Name.aucOperatorNameShort, stMmInfoPlmnName.aucShortName, sizeof(stOperatorName.Name.aucOperatorNameShort));
                MMA_PlmnId2Bcd(&(stOperatorName.Name.PlmnId));
                TAF_MMA_SndCopsQryCnf(&pstRcvMsg->stCtrl, &stOperatorName);

                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, begin */
                /* 将COPS RESULT 信息LOG出来 */
                TAF_MMA_LogCopsRltInfo(&stOperatorName, TAF_SDC_LOG_OPER_NAME_TYPE_SPN);
                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, end */

                return VOS_TRUE;
            }

            /* 如果从SPN中获取7bit编码的运营商名称失败，获取UCS2编码的运营商名称如果成功，cops需要返回空，应用通过^eonsucs2查询获取ucs2编码的运营商名称 */
            if (VOS_TRUE == TAF_MMA_GetSpnNetWorkName((TAF_SDC_PLMN_ID_STRU *)&stOperatorName.Name.PlmnId, &stMmInfoPlmnName, TAF_SDC_NETWORK_NAME_CODE_SCHEME_COPS_UCS2))
            {
                MMA_PlmnId2Bcd(&(stOperatorName.Name.PlmnId));
                TAF_MMA_SndCopsQryCnf(&pstRcvMsg->stCtrl, &stOperatorName);

                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, begin */
                /* 将COPS RESULT 信息LOG出来 */
                TAF_MMA_LogCopsRltInfo(&stOperatorName, TAF_SDC_LOG_OPER_NAME_TYPE_SPN);
                /* Modified by c00318887 for AT&T phaseII, 2015-4-8, end */

                return VOS_TRUE;
            }
        }
    }

    if (VOS_TRUE == TAF_MMA_GetNvimNetWorkName(&stOperatorName))
    {
        /* 从nv中获取运营商名称成功，则直接返回 */
        MMA_PlmnId2Bcd(&(stOperatorName.Name.PlmnId));
        TAF_MMA_SndCopsQryCnf(&pstRcvMsg->stCtrl, &stOperatorName);

        /* Modified by c00318887 for AT&T phaseII, 2015-4-8, begin */
        /* 将COPS RESULT 信息LOG出来 */
        TAF_MMA_LogCopsRltInfo(&stOperatorName, TAF_SDC_LOG_OPER_NAME_TYPE_NV);
        /* Modified by c00318887 for AT&T phaseII, 2015-4-8, end */

        return VOS_TRUE;
    }

    /* 从gastNetworkNameTbl中获取运营商名称，则直接返回 */
    MMA_PlmnId2Bcd(&(stOperatorName.Name.PlmnId));
    if (VOS_TRUE == TAF_MMA_GetStoredInternalNetWorkName(&stOperatorName))
    {
        TAF_MMA_SndCopsQryCnf(&pstRcvMsg->stCtrl, &stOperatorName);

        /* Modified by c00318887 for AT&T phaseII, 2015-4-8, begin */
        /* 将COPS RESULT 信息LOG出来 */
        TAF_MMA_LogCopsRltInfo(&stOperatorName, TAF_SDC_LOG_OPER_NAME_TYPE_TABLE);
        /* Modified by c00318887 for AT&T phaseII, 2015-4-8, end */

        return VOS_TRUE;
    }
    TAF_MMA_SndCopsQryCnf(&pstRcvMsg->stCtrl, &stOperatorName);

    /* Modified by c00318887 for AT&T phaseII, 2015-4-8, begin */
    /* 将COPS RESULT 信息LOG出来 */
    TAF_MMA_LogCopsRltInfo(&stOperatorName, TAF_SDC_OPER_NAME_TYPE_BUTT);
    /* Modified by c00318887 for AT&T phaseII, 2015-4-8, end */

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvRegStateQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_REG_STATE_QRY_REQ_STRU     *pstRcvMsg;
    TAF_PH_REG_STATE_STRU               stRegStatusTmp;
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    TAF_PLMN_ID_STRU                   *pstPlmnId = VOS_NULL_PTR;
#endif

    pstRcvMsg = (TAF_MMA_REG_STATE_QRY_REQ_STRU *)pstMsg;

    PS_MEM_SET(&stRegStatusTmp, 0, sizeof(stRegStatusTmp));

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-15, begin */
    if (VOS_TRUE == TAF_SDC_IsConfigCLMode())
    {
        stRegStatusTmp.RegState     = TAF_SDC_GetCsRegStatus();
        stRegStatusTmp.ucPsRegState = TAF_SDC_GetPsRegStatus();

        TAF_MMA_FillActForAtCmdCregInClMode(pstRcvMsg->enQryRegStaType, &stRegStatusTmp.ucAct);

        /* LTE时填写小区信息 */
        if (TAF_PH_ACCESS_TECH_E_UTRAN == stRegStatusTmp.ucAct)
        {
            /*获取当前PLMN*/
            pstPlmnId           = (TAF_PLMN_ID_STRU *)TAF_SDC_GetAppServicePlmnId();
            stRegStatusTmp.Plmn = *pstPlmnId;

            /*获取当前CI*/
            stRegStatusTmp.CellId.aulCellId[0] = TAF_SDC_GetAppCampCellId();
            stRegStatusTmp.CellId.ucCellNum    = 1;

            /*获取当前LAC*/
            stRegStatusTmp.usLac     = TAF_SDC_GetAppCampLac();

            /*获取当前RAC*/
            stRegStatusTmp.ucRac     = TAF_SDC_GetAppCampRac();
        }
    }
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-15, end */
    else
#endif
    {
        TAF_MMA_GetRegStateInfo(&stRegStatusTmp);
    }

    /* Mma snd reg state Qry cnf */
    TAF_MMA_SndRegStateQryCnf(&pstRcvMsg->stCtrl, &stRegStatusTmp);

    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvSysInfoQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_SYSINFO_QRY_REQ_STRU       *pstRcvMsg;
    TAF_PH_SYSINFO_STRU                 sys_info;
    VOS_UINT32                          ulPinStatus;


    /* Added by k902809 for Iteration 11, 2015-4-15, begin */
    PS_MEM_SET( (VOS_UINT8 *)&sys_info,
                        0x00,
                        (VOS_SIZE_T)(sizeof(TAF_PH_SYSINFO_STRU)) );
    /* Added by k902809 for Iteration 11, Iteration 11 2015-4-15, end */

    pstRcvMsg = (TAF_MMA_SYSINFO_QRY_REQ_STRU *)pstMsg;

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-15, begin */
    if (VOS_TRUE == TAF_SDC_IsConfigCLMode())
    {
        if (VOS_TRUE == pstRcvMsg->ulSysInfoExFlag)
        {
            TAF_MMA_QueryClSysInfo(&sys_info);
        }

        TAF_MMA_SndSystemInfoQryCnf(pstRcvMsg, &sys_info);

        return VOS_TRUE;
    }
    /* Modified by l00301449 for CDMA Iteration 15, 2015-5-15, end */
    else
#endif
    {
        /* 获取当前漫游特性 */
        MN_PH_GetSysInfoRoamStatus(&sys_info);
        /* Modified by l00305157 for Service_State_Optimize_PhaseII, 2015-02-11, begin */
        sys_info.ucSrvStatus = TAF_SDC_GetAppServiceStatus();
        sys_info.ucSrvDomain = TAF_SDC_GetAppServiceDomain();
        /* Modified by l00305157 for Service_State_Optimize_PhaseII, 2015-02-11, end */

        /* 获取当前Service Status信息 */
        MN_PH_GetSysInfoSrvStatus(&sys_info);

        /*如果当前需要PIN码、需要PUK码或者PUK码block，MMA给TAF上报的服务域应该为0(无服务)*/
        ulPinStatus = MMA_CheckPin1SatusSimple();

        /* 需要PIN码时漫游状态显示为未知 */
        if (MMA_NONEED_PIN1 != ulPinStatus)
        {
            sys_info.ucSrvDomain = TAF_PH_INFO_NO_DOMAIN;
            sys_info.ucRoamStatus = TAF_PH_INFO_ROAM_UNKNOW;
        }

        /*获取当前网络接入技术*/
        MMA_INFOLOG1("g_stTafSdcCtx.stAppNetworkInfo.stServPlmnInfo.enServiceSysMode=", TAF_SDC_GetAppSysMode());
        MMA_INFOLOG1("gstMmaValue.pg_StatusContext->ucDataTranStatus=", gstMmaValue.pg_StatusContext->ucDataTranMode);

        if (VOS_TRUE == pstRcvMsg->ulSysInfoExFlag)
        {
            MN_PH_GetSysInfoSysModeEx(&sys_info);
        }
        else
        {
            MMA_GetSysInfoSysMode(&sys_info);
        }


        if ( MMA_SIM_IS_LOCK == MMA_GetMeLockStatus() )
        {
            sys_info.ucSimLockStatus = VOS_TRUE;
        }
        else
        {
            sys_info.ucSimLockStatus = VOS_FALSE;
        }

        if (MMA_PUK1_REMAINS_ZERO == ulPinStatus)
        {
            sys_info.ucSimStatus = TAF_PH_USIM_STATUS_UNAVAIL;
        }
        else
        {
            sys_info.ucSimStatus = MMA_GetUsimStatus(VOS_FALSE);
        }

        TAF_MMA_SndSystemInfoQryCnf(pstRcvMsg, &sys_info);

        return VOS_TRUE;
    }
}
VOS_UINT32 TAF_MMA_RcvAntennaInfoQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_ANTENNA_INFO_QRY_REQ_STRU  *pstRcvMsg;
    MN_MMA_ANQUERY_PARA_STRU            stAnqueryPara;
    VOS_UINT32                          ulResult;
    TAF_SDC_SYS_MODE_ENUM_UINT8         enSysMode;

    pstRcvMsg = (TAF_MMA_ANTENNA_INFO_QRY_REQ_STRU *)pstMsg;

    enSysMode = TAF_SDC_GetAppSysMode();

    PS_MEM_SET(&stAnqueryPara, 0x0, sizeof(MN_MMA_ANQUERY_PARA_STRU));

    if ((TAF_SDC_SYS_MODE_GSM == enSysMode)
     || (TAF_SDC_SYS_MODE_WCDMA == enSysMode))
    {
        /*调用函数MN_PH_QryAnQuery获取rscp, ecio,rssi,cellid */
        ulResult = MN_PH_QryAnQuery(&stAnqueryPara.u.st2G3GCellSignInfo.sCpichRscp,
                                    &stAnqueryPara.u.st2G3GCellSignInfo.sCpichEcNo,
                                    &stAnqueryPara.u.st2G3GCellSignInfo.ucRssi,
                                    &stAnqueryPara.u.st2G3GCellSignInfo.ulCellId);
         /*指示其为2G 3G的小区信号 */
         stAnqueryPara.enServiceSysMode = enSysMode;

    /* 回复AT^ANQUERY查询命令 */
    if (VOS_OK != ulResult)
    {
        TAF_MMA_SndAntennaInfoQryCnf(&pstRcvMsg->stCtrl, TAF_ERR_ERROR, &stAnqueryPara);
    }
    else
    {
        TAF_MMA_SndAntennaInfoQryCnf(&pstRcvMsg->stCtrl, TAF_ERR_NO_ERROR, &stAnqueryPara);
    }
    }
#if ( FEATURE_ON == FEATURE_LTE )
    else if(TAF_SDC_SYS_MODE_LTE == enSysMode )
    {

        /*与 at_CsqInfoProc 中RssiLevel处理相同 */
        stAnqueryPara.u.st4GCellSignInfo.sRssi = TAF_SDC_GetSdcCtx()->stAppNetworkInfo.stRssiValue.u.st4GCellSignInfo.sRssi;
        stAnqueryPara.u.st4GCellSignInfo.sRsrp = TAF_SDC_GetSdcCtx()->stAppNetworkInfo.stRssiValue.u.st4GCellSignInfo.sRsrp;
        stAnqueryPara.u.st4GCellSignInfo.sRsrq = TAF_SDC_GetSdcCtx()->stAppNetworkInfo.stRssiValue.u.st4GCellSignInfo.sRsrq;

        /*指示其为2G 3G的小区信号 */
        stAnqueryPara.enServiceSysMode = enSysMode;

        TAF_MMA_SndAntennaInfoQryCnf(&pstRcvMsg->stCtrl, TAF_ERR_NO_ERROR, &stAnqueryPara);
    }
#endif
    else
    {
        MMA_WARNINGLOG("TAF_MMA_RcvAntennaInfoQryReq_PreProc():RAT IS INVALID");
    }
    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvApHplmnQryReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_HOME_PLMN_QRY_REQ_STRU     *pstRcvMsg;

    pstRcvMsg = (TAF_MMA_HOME_PLMN_QRY_REQ_STRU *)pstMsg;

    MMA_HomePlmnProc(pstRcvMsg->stCtrl.usClientId, pstRcvMsg->stCtrl.ucOpId);

    return VOS_TRUE;
}



VOS_UINT32 TAF_MMA_RcvBatteryCapacityQry_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_BATTERY_CAPACITY_QRY_REQ_STRU                  *pstRcvMsg = VOS_NULL_PTR;
    TAF_MMA_BATTERYPOWER_STRU                               stBatteryStatus;
    TAF_PMU_STATE_STRU                                      stpmuState;

    pstRcvMsg = (TAF_MMA_BATTERY_CAPACITY_QRY_REQ_STRU *)pstMsg;

    PS_MEM_SET(&stBatteryStatus,0x00,sizeof(TAF_MMA_BATTERYPOWER_STRU));

    PS_MEM_SET(&stpmuState,0x00,sizeof(stpmuState));

    (VOS_VOID)DRV_GET_PMU_STATE(&stpmuState);
    /************************************************************/
    /*从电源模块中获取当前所要信息*/
    if(1 == stpmuState.bat_out)
    {
        stBatteryStatus.BatteryPowerStatus = TAF_PH_NOBATTERYFIXED;
    }
    else if(1 == stpmuState.chg_timer_overflow)
    {
        stBatteryStatus.BatteryPowerStatus = TAF_PH_POWERSOURCEERROR;
    }
    else if(1 == stpmuState.chargerstate)
    {
        stBatteryStatus.BatteryPowerStatus = TAF_PH_BATTERYNOSUPPLY;
    }
    else
    {
        stBatteryStatus.BatteryPowerStatus = TAF_PH_BATTERYCHARGING;
    }

    stBatteryStatus.BatteryRemains      = (VOS_UINT8)((stpmuState.bal_Volt * 100) / 900);
    /************************************************************/

    /* Mma snd CBC Qry cnf */
    TAF_MMA_SndBatteryCapacityQryCnf(&pstRcvMsg->stCtrl, &stBatteryStatus);

    return VOS_TRUE;

}


VOS_UINT32 TAF_MMA_RcvHandShakeQry_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_HAND_SHAKE_QRY_REQ_STRU    *pstRcvMsg = VOS_NULL_PTR;
    VOS_UINT8                           aucBuf[AT_HS_PARA_MAX_LENGTH];
    VOS_UINT8                          *pPara = VOS_NULL_PTR;
    VOS_UINT32                          ulId;
    VOS_UINT8                           ucPhoneMode;

    pstRcvMsg   = (TAF_MMA_HAND_SHAKE_QRY_REQ_STRU *)pstMsg;

    ucPhoneMode = TAF_SDC_GetCurPhoneMode();

    ulId        = MMA_HS_ID;
    PS_MEM_CPY(aucBuf, &ulId, sizeof(VOS_UINT32));                              /*id*/

    aucBuf[4]   = MMA_HS_PROTOCOL_APP;                                            /*protocol*/

    if ((TAF_PH_MODE_FULL == ucPhoneMode)                    /*UE CUR CFUN MODE*/
     || (TAF_PH_MODE_FT == ucPhoneMode))
    {
        aucBuf[5] = MMA_HS_ONLINE;
    }
    else if (TAF_PH_MODE_RFOFF == ucPhoneMode)
    {
        aucBuf[5] = MMA_HS_OFFLINE;
    }
    else
    {
        aucBuf[5] = MMA_HS_LPM;
    }

    aucBuf[6] = MMA_HS_PRODUCT_CLASS_GW;                                        /*product_class*/
    aucBuf[7] = MMA_HS_PRODUCT_ID;                                              /*product id*/
    aucBuf[8] = 0x0;                                                            /*software_id*/
    pPara = (VOS_UINT8 *)aucBuf;

    /* Mma snd HS Qry cnf */
    TAF_MMA_SndHandShakeQryCnf(&pstRcvMsg->stCtrl, pPara);

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvTiWaitMsccPlmnAutoSelCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstMmaOperCtx = TAF_MMA_GetOperCtxAddr();

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找ID_TAF_MMA_PLMN_AUTO_RESEL_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_PLMN_AUTO_RESEL_REQ, &ucCtxIndex))
    {
        /*  回复MMA_CFPLMN_CNF */
        TAF_MMA_SndPlmnAutoReselCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl), TAF_ERR_TIME_OUT);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    g_StatusContext.ulFsmState = STA_FSM_ENABLE;
    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvTiWaitMsccAbortSpecPlmnSelCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    pstMmaOperCtx = TAF_MMA_GetOperCtxAddr();

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找ID_TAF_MMA_PLMN_SPECIAL_SEL_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_PLMN_SPEC_SEL_REQ, &ucCtxIndex))
    {
        TAF_MMA_SndPlmnSpecialSelCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl), TAF_ERR_TIME_OUT);

        TAF_MMA_ClearOperCtx(ucCtxIndex);
    }

    /* 迁移状态到ENABLE状态 */
    g_StatusContext.ulFsmState = STA_FSM_ENABLE;
    return VOS_TRUE;
}


VOS_UINT32 TAF_MMA_RcvTiWaitMsccSpecPlmnSelCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    VOS_UINT8                           ucCtxIndex;
    VOS_UINT32                          ulRet;

    /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找ID_TAF_MMA_PLMN_SPECIAL_SEL_REQ对应的记录项 */
    if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_PLMN_SPEC_SEL_REQ, &ucCtxIndex))
    {
        /* MMA的指定搜网保护定时器超时则主动向MSCC发送停止指定搜网请求
           收到MSCC的回复后再向AT回复 */
        ulRet = TAF_MMA_SndMsccSpecPlmnSearchAbortReq();

        if (VOS_ERR == ulRet)
        {
            MMA_WARNINGLOG("TAF_MMA_RcvTiWaitMsccSpecPlmnSelCnfExpired_PreProc():ERROR: failed");
        }

        /* 启动STA_TIMER_SPEC_PLMN_ABORT 10s定时器，如果超时未收到MSCC的回复则上报AT失败 */
        TAF_MMA_StartTimer(TI_TAF_MMA_WAIT_MSCC_ABORT_SPEC_PLMN_SEL_CNF, TI_TAF_MMA_WAIT_MSCC_ABORT_SPEC_PLMN_SEL_CNF_LEN);
    }

    return VOS_TRUE;
}
VOS_UINT32 TAF_MMA_RcvTiWaitMsccPlmnListAbortCnfExpired_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_OPER_CTX_STRU              *pstMmaOperCtx = VOS_NULL_PTR;
    VOS_UINT8                           ucCtxIndex;

    if (TAF_MMA_PLMN_LIST_ABORT_BUTT == g_StatusContext.ucPlmnListAbortProc)
    {
        return VOS_TRUE;
    }

    pstMmaOperCtx = TAF_MMA_GetOperCtxAddr();

    /* PLMN LIST ABORT原因值为定时器超时 */
    if ( TAF_MMA_PLMN_LIST_ABORT_PROCESSING_TIMEOUT == g_StatusContext.ucPlmnListAbortProc )
    {
        /* 向AT回复列表搜超时结果 */
        g_StatusContext.PhoneError    = TAF_ERR_TIME_OUT;
        g_StatusContext.ucOperateType = STA_OP_PHONEERROR;

        /* 上报AT当前状态事件 */
        Sta_EventReport (g_StatusContext.ulCurOpId, TAF_PH_EVT_PLMN_LIST_CNF);
    }
    else
    {
         /* 向AT回复列表搜超时结果 */
        /* 从全局变量g_stTafMmaCtx.astMmaOperCtx查找ID_TAF_MMA_PLMN_LIST_ABORT_REQ对应的记录项 */
        if (VOS_TRUE == TAF_MMA_GetSpecOperTypeIndex(TAF_MMA_OPER_PLMN_LIST_ABORT_REQ, &ucCtxIndex))
        {
            /*  回复ID_TAF_MMA_PLMN_LIST_ABORT_CNF */
            TAF_MMA_SndPlmnListAbortCnf(&(pstMmaOperCtx[ucCtxIndex].stCtrl), TAF_ERR_NO_ERROR);

            TAF_MMA_ClearOperCtx(ucCtxIndex);
        }
    }

    /* 迁移状态到ENABLE状态 */
    g_StatusContext.ulFsmState = STA_FSM_ENABLE;

    /* 设置当前没有处理用户列表搜打断过程 */
    g_StatusContext.ucPlmnListAbortProc = TAF_MMA_PLMN_LIST_ABORT_BUTT;

    return VOS_TRUE;
}
/*****************************************************************************
 函 数 名  : TAF_MMA_ProcAutoAttachSetReq_PreProc
 功能描述  : 根据服务模式启动attach过程
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年4月9日
    作    者   : l00301449
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID TAF_MMA_ProcAutoAttachSetReq_PreProc(VOS_VOID)
{
    TAF_MMA_ATTACH_TYPE_ENUM_UINT8      enAttachType;
    VOS_UINT8                           ucAutoAttachFlg;
    NAS_NVIM_AUTOATTACH_STRU            stAutoattachFlag;
    VOS_UINT32                          ulLength;
    VOS_UINT8                           ucOpID;

    ulLength = 0;
    PS_MEM_SET(&stAutoattachFlag, 0x00, sizeof(NAS_NVIM_AUTOATTACH_STRU));
    (VOS_VOID)NV_GetLength(en_NV_Item_Autoattach, &ulLength);
    if (NV_OK == NV_Read(en_NV_Item_Autoattach,
                         (VOS_VOID*)(&stAutoattachFlag),
                         ulLength))
    {
        if (1 == stAutoattachFlag.usAutoattachFlag)
        {
            ucAutoAttachFlg = MMA_AUTO_ATTACH_ENABLE;
        }
        else
        {
            ucAutoAttachFlg = MMA_AUTO_ATTACH_DISABLE;
        }
    }
    else
    {
        ucAutoAttachFlg = MMA_AUTO_ATTACH_ENABLE;
    }

    switch (g_StatusContext.ucModeService)
    {
        case TAF_PH_MS_CLASS_A:
            enAttachType = TAF_MMA_ATTACH_TYPE_GPRS_IMSI;
            if (MMA_AUTO_ATTACH_DISABLE == ucAutoAttachFlg)
            {
                enAttachType = TAF_MMA_ATTACH_TYPE_IMSI;
            }
            break;

        case TAF_PH_MS_CLASS_CG:
            enAttachType = TAF_MMA_ATTACH_TYPE_GPRS;
            if (MMA_AUTO_ATTACH_DISABLE == ucAutoAttachFlg)
            {
                return;
            }
            break;

        case TAF_PH_MS_CLASS_CC:
            enAttachType = TAF_MMA_ATTACH_TYPE_IMSI;
            break;

        case TAF_PH_MS_CLASS_NULL:
            g_StatusContext.ulFsmState = STA_FSM_ENABLE;
            return;

        default:
            return;
    }

    ucOpID = TAF_MMA_GetAttachOpId();

    (VOS_VOID)TAF_MMA_SndMsccAttachReq(ucOpID, (NAS_MSCC_PIF_ATTACH_TYPE_ENUM_UINT32)enAttachType, TAF_MMA_ATTACH_REASON_INITIAL);

    TAF_MMA_SetAttachAllowFlg(enAttachType);

    return;
}

/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMmaSetAutoAttachReq_PreProc
 功能描述  : AT^CAATT
 输入参数  : ulEventType
             pstMsg
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : y00322978
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMmaSetAutoAttachReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_AUTO_ATTACH_SET_REQ_STRU   *pstRcvMsg;

    VOS_UINT32                          ulMsgAutoAttachFlg;
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucAutoAttachFlg;
    NAS_NVIM_AUTOATTACH_STRU            stAutoattachFlag;
    VOS_UINT32                          ulLength;

    ulLength    = 0;
    ulResult    = 0;

    PS_MEM_SET(&stAutoattachFlag, 0x00, sizeof(NAS_NVIM_AUTOATTACH_STRU));
    pstRcvMsg = (TAF_MMA_AUTO_ATTACH_SET_REQ_STRU *)pstMsg;

    (VOS_VOID)NV_GetLength(en_NV_Item_Autoattach, &ulLength);
    if (NV_OK == NV_Read(en_NV_Item_Autoattach,
                          (VOS_VOID*)(&stAutoattachFlag),
                          ulLength))
    {
        MMA_INFOLOG1("TAF_MMA_RcvMmaSetAutoAttachReq_PreProc:Read result from NV", (VOS_INT32)stAutoattachFlag.usAutoattachFlag);
        if (1 == stAutoattachFlag.usAutoattachFlag)
        {
            ucAutoAttachFlg = MMA_AUTO_ATTACH_ENABLE;
        }
        else
        {
            ucAutoAttachFlg = MMA_AUTO_ATTACH_DISABLE;
        }
    }
    else
    {
        MMA_WARNINGLOG("TAF_MMA_RcvMmaSetAutoAttachReq_PreProc:WARNING: Read NV Failure!");
        ucAutoAttachFlg = MMA_AUTO_ATTACH_ENABLE;
    }

    if (VOS_FALSE == pstRcvMsg->ulAutoAttachEnable)
    {
        ulMsgAutoAttachFlg = MMA_AUTO_ATTACH_DISABLE;
    }
    else if (VOS_TRUE == pstRcvMsg->ulAutoAttachEnable)
    {
        ulMsgAutoAttachFlg = MMA_AUTO_ATTACH_ENABLE;
    }
    else
    {
        MMA_WARNINGLOG("TAF_MMA_RcvMmaSetAutoAttachReq_PreProc:WARNING: ulSetValue error");

        /* Modified by y00322978 for CDMA 1X Iteration 11, 2015-3-26, begin */
        TAF_MMA_SndAutoAttachSetCnf(&pstRcvMsg->stCtrl, TAF_PARA_SET_ERROR);
        return VOS_TRUE;
        /* Modified by y00322978 for CDMA 1X Iteration 11, 2015-3-26, end */

    }

    if ( ucAutoAttachFlg == ulMsgAutoAttachFlg )
    {
        /* NV项值没有改变，则返回成功，否则AT口会被占据 */
        /* Modified by y00322978 for CDMA 1X Iteration 11, 2015-3-26, begin */
        TAF_MMA_SndAutoAttachSetCnf(&pstRcvMsg->stCtrl, TAF_PARA_OK);
        return VOS_TRUE;
        /* Modified by y00322978 for CDMA 1X Iteration 11, 2015-3-26, end */
    }

    /* 值不一样，需要写NV */

    stAutoattachFlag.usAutoattachFlag = (VOS_UINT16)ulMsgAutoAttachFlg;
    ulResult = MMA_WriteValue2Nvim(en_NV_Item_Autoattach, (VOS_VOID *)(&stAutoattachFlag), sizeof(VOS_UINT16));

    if (MMA_SUCCESS != ulResult)
    {
        MMA_WARNINGLOG("TAF_MMA_RcvMmaSetAutoAttachReq_PreProc:WARNING: Write NV Failure!");

        /* Modified by y00322978 for CDMA 1X Iteration 11, 2015-3-26, begin */
        TAF_MMA_SndAutoAttachSetCnf(&pstRcvMsg->stCtrl, TAF_PARA_WRITE_NVIM_ERROR);
        return VOS_TRUE;
        /* Modified by y00322978 for CDMA 1X Iteration 11, 2015-3-26, end */

    }
    else
    {
        /* Modified by y00322978 for CDMA 1X Iteration 11, 2015-3-26, begin */
        TAF_MMA_SndAutoAttachSetCnf(&pstRcvMsg->stCtrl, TAF_PARA_OK);
        /* Modified by y00322978 for CDMA 1X Iteration 11, 2015-3-26, end */

        if ((MMA_AUTO_ATTACH_DISABLE  == ucAutoAttachFlg)
            &&(MMA_AUTO_ATTACH_ENABLE == ulMsgAutoAttachFlg))
        {
            if ( (TAF_SDC_SERVICE_STATUS_NORMAL_SERVICE != TAF_SDC_GetPsServiceStatus())
              && ( (STA_FSM_NULL     != g_StatusContext.ulFsmState)
                && (STA_FSM_STOP     != g_StatusContext.ulFsmState) )
              && (TAF_PH_MS_CLASS_CC != g_StatusContext.ucModeService)  )
            {
                TAF_MMA_ProcAutoAttachSetReq_PreProc();
            }
        }
    }

    return VOS_TRUE;

}

VOS_UINT32 TAF_MMA_RcvMmaQryAutoAttachReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_AUTO_ATTACH_QRY_REQ_STRU   *pstRcvMsg;
    NAS_NVIM_AUTOATTACH_STRU            stAutoattachFlag;
    VOS_UINT32                          ulLength;
    VOS_UINT8                           ucAutoAttachFlg;

    pstRcvMsg = (TAF_MMA_AUTO_ATTACH_QRY_REQ_STRU *)pstMsg;

    ulLength        = 0;
    ucAutoAttachFlg = 0;
    PS_MEM_SET(&stAutoattachFlag, 0x00, sizeof(NAS_NVIM_AUTOATTACH_STRU));
    (VOS_VOID)NV_GetLength(en_NV_Item_Autoattach, &ulLength);

    if (NV_OK != NV_Read(en_NV_Item_Autoattach,
                        (VOS_VOID*)(&stAutoattachFlag),
                         ulLength))
    {
        MMA_WARNINGLOG("MMA_GetPhAutoAttachStatus:Read Auto Attach Status Failed!");

        /* Mma Snd Auto Attach Qry Cnf */
        TAF_MMA_SndAutoAttachQryCnf(&pstRcvMsg->stCtrl, TAF_ERR_ERROR, ucAutoAttachFlg);
    }
    else
    {
        MMA_INFOLOG1("MMA_GetPhAutoAttachStatus:Read result from NV",(VOS_INT32)(stAutoattachFlag.usAutoattachFlag));

        if (0 == stAutoattachFlag.usAutoattachFlag)
        {
            ucAutoAttachFlg = MMA_AUTO_ATTACH_DISABLE;
        }
        else
        {
            ucAutoAttachFlg = MMA_AUTO_ATTACH_ENABLE;
        }

        /* Mma Snd Auto Attach Qry Cnf */
        TAF_MMA_SndAutoAttachQryCnf(&pstRcvMsg->stCtrl, TAF_ERR_NO_ERROR, ucAutoAttachFlg);
    }

    return VOS_TRUE;
}

/* Added by h00313353 for Iteration 13, 2015-4-13, begin */
/*****************************************************************************
 函 数 名  : TAF_MMA_RcvDetachReq_PreProc
 功能描述  : 收到DetachReq消息的预处理
 输入参数  : ulEventType:消息类型
             pstMsg:消息内容
 输出参数  : 无
 返 回 值  : VOS_TRUE:不需要进入状态机,VOS_FALSE:需要进入状态机继续执行
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年4月13日
    作    者   : h00313353
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvDetachReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_DETACH_REQ_STRU            *pstDetachReqMsg = VOS_NULL_PTR;

    pstDetachReqMsg = (TAF_MMA_DETACH_REQ_STRU *)pstMsg;

    /* 检查DETACH请求类型合法性检查 */
    if ((pstDetachReqMsg->stDetachPara.enDetachDomain) > TAF_MMA_SERVICE_DOMAIN_CS_PS)
    {
        TAF_MMA_SndDetachCnf(&(pstDetachReqMsg->stCtrl), TAF_MMA_APP_OPER_RESULT_FAILURE, TAF_ERR_PARA_ERROR);

        return VOS_TRUE;
    }

    /* 如果是C+L模式，此时是AT触发了detach，则回复失败,进行保护  */
    if ((VOS_TRUE == TAF_MMA_IsPowerOnCLInterWork())
     && (CMMCA_CLIENT_ID != (VOS_UINT32)(pstDetachReqMsg->stCtrl.usClientId)))
    {
        TAF_MMA_SndDetachCnf(&(pstDetachReqMsg->stCtrl), TAF_MMA_APP_OPER_RESULT_FAILURE, TAF_ERR_ERROR);

        return VOS_TRUE;
    }

    /* 如果当前不是C+L模式，但是CMMCA触发DETACH,则直接回复失败 */
    if ((VOS_FALSE == TAF_MMA_IsPowerOnCLInterWork())
     && (CMMCA_CLIENT_ID == (VOS_UINT32)(pstDetachReqMsg->stCtrl.usClientId)))
    {
        TAF_MMA_SndDetachCnf(&(pstDetachReqMsg->stCtrl), TAF_MMA_APP_OPER_RESULT_FAILURE, TAF_ERR_ERROR);

        return VOS_TRUE;
    }

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    /* 当前如果是CL模式，且进行detach的域是CS或者CS+PS，则直接返回ERROR */
    if (VOS_TRUE == TAF_SDC_IsConfigCLMode())
    {
        if ( (TAF_MMA_SERVICE_DOMAIN_CS == pstDetachReqMsg->stDetachPara.enDetachDomain)
          || (TAF_MMA_SERVICE_DOMAIN_CS_PS == pstDetachReqMsg->stDetachPara.enDetachDomain) )
        {
            TAF_MMA_SndDetachCnf(&(pstDetachReqMsg->stCtrl), TAF_MMA_APP_OPER_RESULT_FAILURE, TAF_ERR_ERROR);

            return VOS_TRUE;
        }
    }
#endif

    /* 如果消息有效，进入Main状态机处理 */
    return VOS_FALSE;
}

/* Added by h00313353 for Iteration 13, 2015-4-13, end */


/* Modified by l00301449 for Iteration 13, 2015-04-20, begin */

VOS_UINT32 TAF_MMA_RcvAttachReq_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_ATTACH_REQ_STRU                                *pstAttachReqMsg = VOS_NULL_PTR;

    pstAttachReqMsg   = (TAF_MMA_ATTACH_REQ_STRU *)pstMsg;

    /* 如果是C+L模式 */
    if (VOS_TRUE == TAF_MMA_IsPowerOnCLInterWork())
    {
        /*此时是AT触发了attach，则回复失败,进行保护 */
        TAF_MMA_SndAttachCnf(&(pstAttachReqMsg->stCtrl),
                             TAF_MMA_APP_OPER_RESULT_FAILURE,
                             TAF_ERR_PHONE_ATTACH_FORBIDDEN);

        return VOS_TRUE;
    }

    /* 参数有效性检查 */
    if (pstAttachReqMsg->enAttachType > TAF_MMA_ATTACH_TYPE_GPRS_IMSI)
    {
        TAF_MMA_SndAttachCnf(&(pstAttachReqMsg->stCtrl),
                             TAF_MMA_APP_OPER_RESULT_FAILURE,
                             TAF_ERR_PARA_ERROR);

        return VOS_TRUE;
    }

#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
    /* 当前如果是CL模式，且进行detach的域是CS或者CS+PS，则直接返回ERROR */
    if (VOS_TRUE == TAF_SDC_IsConfigCLMode())
    {
        if ( (TAF_MMA_ATTACH_TYPE_IMSI == pstAttachReqMsg->enAttachType)
          || (TAF_MMA_ATTACH_TYPE_GPRS_IMSI == pstAttachReqMsg->enAttachType) )
        {
            TAF_MMA_SndAttachCnf(&(pstAttachReqMsg->stCtrl), TAF_MMA_APP_OPER_RESULT_FAILURE, TAF_ERR_ERROR);

            return VOS_TRUE;
        }
    }
#endif

    /* 如果消息有效，进入Main状态机处理 */
    return VOS_FALSE;
}

/* Modified by l00301449 for Iteration 13, 2015-04-20, end */


VOS_UINT32 TAF_MMA_RcvAttachStatusQry_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_ATTACH_STATUS_ENUM_UINT8    enCsStatus;
    TAF_MMA_ATTACH_STATUS_ENUM_UINT8    enPsStatus;

    Taf_GetCurrentAttachStatus(&enCsStatus, &enPsStatus);

    TAF_MMA_SndAttachStatusQryCnf((TAF_MMA_ATTACH_STATUS_QRY_REQ_STRU *)pstMsg,
                                   TAF_MMA_APP_OPER_RESULT_SUCCESS,
                                   TAF_ERR_NO_ERROR,
                                   enCsStatus,
                                   enPsStatus);

    return VOS_TRUE;
}





VOS_UINT32 TAF_MMA_IsNeedProcUsimStatusIndInFsmPhoneMode(
    VOS_UINT32                          ulFsmTopState
)
{
    switch(ulFsmTopState)
    {
        /* 下面这些状态需要处理卡状态上报 */
        case TAF_MMA_PHONE_MODE_STA_WAIT_EXTERNAL_MODULE_INIT:
        case TAF_MMA_PHONE_MODE_STA_WAIT_MSCC_GET_GEO_CNF:
        case TAF_MMA_PHONE_MODE_STA_GET_GEO_WAIT_MSCC_START_CNF:
        case TAF_MMA_PHONE_MODE_STA_GET_GEO_WAIT_MSCC_POWER_OFF_CNF:
        case TAF_MMA_PHONE_MODE_STA_WAIT_MSCC_STOP_GET_GEO_CNF:
        case TAF_MMA_PHONE_MODE_STA_GET_GEO_CMP_WAIT_USIM_STATUS_IND:

            return VOS_TRUE;

        default:

            return VOS_FALSE;
    }
}


/* Added by y00314741 for CDMA Iteration 15, 2015-6-1, begin */
/*****************************************************************************
 Function Name     :  TAF_MMA_RcvTafPsRatTypeNtf_PreProc
 Description       :  消息透传

 Input parameters  :   VOS_UINT32                          ulEventType,
                       struct MsgCB                       *pstMsg
 Outout parameters :
 Return Value      :   VOS_UINT32

 Modify History:
     1)  Date      :   2015-06-01
         Author    :   y00314741
         Modify content :    Create
*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvTafPsRatTypeNtf_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    TAF_MMA_PS_RAT_TYPE_NTF_STRU       *pstRcvMsg = VOS_NULL_PTR;

    pstRcvMsg   = (TAF_MMA_PS_RAT_TYPE_NTF_STRU *)pstMsg;

    TAF_MMA_SndMsccPsRatTypeNtf(pstRcvMsg->enRatType);

    return VOS_TRUE;
}

/* Added by y00314741 for CDMA Iteration 15, 2015-6-1, end */
/* Added by l00324781 for CDMA Iteration 12, 2015-6-1, begin */
#if 0
#if (FEATURE_ON == FEATURE_UE_MODE_CDMA)
/*****************************************************************************
 函 数 名  : TAF_MMA_RcvMsccInterSysStartInd_PreProc
 功能描述  : 收到MSCC_MMA_INTERSYS_START_IND_STRU的预处理
 输入参数  : VOS_UINT32                          ulEventType
             struct MsgCB                       *pstMsg
 输出参数  : 无
 返 回 值  : VOS_FALSE:消息处理未完成，需要继续处理
             VOS_TRUE: 消息处理完成，后续不需要继续处理
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2015年6月1日
    作    者   : l00324781
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 TAF_MMA_RcvMsccInterSysStartInd_PreProc(
    VOS_UINT32                          ulEventType,
    struct MsgCB                       *pstMsg
)
{
    MSCC_MMA_INTERSYS_START_IND_STRU   *pstInterSysStartInd = VOS_NULL_PTR;

    pstInterSysStartInd = (MSCC_MMA_INTERSYS_START_IND_STRU*)pstMsg;

    /* CL模式下，HRPD重选到LTE，在切换过程中，原接入模式为HRPD,目的接入模式为LTE
        InterSysCause 为Reselect.此时UE暂时处于无接入系统状态，数据业务缓存。当切换到lte后，
        再把接入类型更新为lte。
    */

    /* 此处和yanzhiji确认，不需要向APS通知无服务，由接入层通知 */
    if ((VOS_RATMODE_HRPD == pstInterSysStartInd->enOrignRatMode)
        && (VOS_RATMODE_LTE == pstInterSysStartInd->enDestRatMode)
        && (NAS_MSCC_PIF_INTERSYS_CAUSE_RESELECT == pstInterSysStartInd->enInterSysCause))
    {
        /*TAF_MMA_SndTafServiceStatusInd(MMA_TAF_RAT_TYPE_NULL, VOS_FALSE);*/
    }

    return VOS_TRUE;
}

#endif
#endif
/* Added by l00324781 for CDMA Iteration 12, 2015-6-1, end */

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif


