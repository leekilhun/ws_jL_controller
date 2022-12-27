#include "pch.h"
#include "system.h"


#include "enTapeLoadUnload.h"
namespace AP_SYS
{
	errcode enTapeLoadUnload::InitUnits()
    {

		constexpr int def_retry_cnt = 3;
		uint8_t retry_cnt{ def_retry_cnt };

		auto initMotor = [](IAxis* ptr_motor, uint8_t retry)
		{
			while (retry)
			{
				if (ptr_motor->IsMotorOn())
					break;
				else
				{
					if (ptr_motor->MotorOnOff(true) == ERROR_SUCCESS)
						break;
				}
				Sleep(1000 * 1);
				retry--;
			}
			return retry;
		};
		// end of initMotor

		/* 1. motor 1 ForBack */
		if (initMotor(m_cfg.pAxis_ForBack, retry_cnt) == 0)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : Axis_ForBack->MotorOn Fail!");
			// error pop up
			return ERR_TAPELOADER_AXIS_DISABLE;
		}

		/* 2. motor 2 pAxis_splicingLoader_L */
		if (initMotor(m_cfg.pAxis_splicingLoader_L, retry_cnt) == 0)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : pAxis_splicingLoader_L->MotorOn Fail!");
			// error pop up
			return ERR_TAPELOADER_AXIS_DISABLE;
		}

		/* 3. motor 3 pAxis_splicingLoader_R */
		if (initMotor(m_cfg.pAxis_splicingLoader_R, retry_cnt) == 0)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : pAxis_splicingLoader_R->MotorOn Fail!");
			// error pop up
			return ERR_TAPELOADER_AXIS_DISABLE;
		}

		/* 4. motor 4 pAxis_UpDown_L */
		if (initMotor(m_cfg.pAxis_UpDown_L, retry_cnt) == 0)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : pAxis_UpDown_L->MotorOn Fail!");
			// error pop up
			return ERR_TAPELOADER_AXIS_DISABLE;
		}

		/* 5. motor 5 pAxis_UpDown_R */
		if (initMotor(m_cfg.pAxis_UpDown_R, retry_cnt) == 0)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : pAxis_UpDown_R->MotorOn Fail!");
			// error pop up
			return ERR_TAPELOADER_AXIS_DISABLE;
		}

		/* 6. pCyl_loderUD_R_UD */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_loderUD_R_UD->Up() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : pCyl_loderUD_R_UD->Up Fail!");
			// error pop up
			return ERR_TAPELOADER_CYLINDER_TIMEOUT;
		}


		/* 7. pCyl_loderUD_L_UD */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_loderUD_L_UD->Up() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : pCyl_loderUD_L_UD->Up Fail!");
			// error pop up
			return ERR_TAPELOADER_CYLINDER_TIMEOUT;
		}

		/* 8. pCyl_loderFB_R_FB */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_loderFB_R_FB->Forward() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : pCyl_loderFB_R_FB->Forward Fail!");
			// error pop up
			return ERR_TAPELOADER_CYLINDER_TIMEOUT;
		}

		/* 9. pCyl_loderFB_L_FB */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_loderFB_L_FB->Forward() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : pCyl_loderFB_L_FB->Forward Fail!");
			// error pop up
			return ERR_TAPELOADER_CYLINDER_TIMEOUT;
		}

		/* 10. pCyl_tapePullOut_R_FB */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_tapePullOut_R_FB->Backward() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : pCyl_tapePullOut_R_FB->Backward Fail!");
			// error pop up
			return ERR_TAPELOADER_CYLINDER_TIMEOUT;
		}

		/* 11. pCyl_tapePullOut_L_FB */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_tapePullOut_L_FB->Backward() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : pCyl_tapePullOut_L_FB->Backward Fail!");
			// error pop up
			return ERR_TAPELOADER_CYLINDER_TIMEOUT;
		}

		/**/

		double pose = m_cfg.p_motion_data->GetPositionData(GMotion_dat::addr_e::moon_ForBack_SU_forward);
		int velocity = (int)m_cfg.p_axis_data_ForBack->RunVelocity;
		if (m_cfg.pAxis_ForBack->Move((int)pose, velocity) != ERROR_SUCCESS)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : pAxis_ForBack->Move Fail!");
			// error pop up
			return ERR_TAPELOADER_AXIS_RET_ERR;
		}

		pose = m_cfg.p_motion_data->GetPositionData(GMotion_dat::addr_e::moon_UpDown_L_down);
		velocity = (int)m_cfg.p_axis_data_UpDown_L->RunVelocity;
		if (m_cfg.pAxis_UpDown_L->Move((int)pose, velocity) != ERROR_SUCCESS)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : p_axis_data_UpDown_L->Move Fail!");
			// error pop up
			return ERR_TAPELOADER_AXIS_RET_ERR;
		}
		pose = m_cfg.p_motion_data->GetPositionData(GMotion_dat::addr_e::moon_UpDown_R_down);
		velocity = (int)m_cfg.p_axis_data_UpDown_R->RunVelocity;
		if (m_cfg.pAxis_UpDown_R->Move((int)pose, velocity) != ERROR_SUCCESS)
		{
			ERR_PRINT("enTapeLoadUnload - InitUnits : p_axis_data_UpDown_R->Move Fail!");
			// error pop up
			return ERR_TAPELOADER_AXIS_RET_ERR;
		}

        return ERROR_SUCCESS;
    }


}
// end of namespace AP_SYS