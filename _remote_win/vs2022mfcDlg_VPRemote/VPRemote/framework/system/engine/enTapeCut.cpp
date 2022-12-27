#include "pch.h"
#include "system.h"

#include "enTapeCut.h"

namespace AP_SYS
{

	errcode enTapeCut::InitUnits()
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
			ERR_PRINT("enTapeCut - InitUnits : Axis_ForBack->MotorOn Fail!");
			// error pop up
			return ERR_TAPECUT_AXIS_DISABLE;
		}

		/* 2. motor 2 tapeLoad */
		if (initMotor(m_cfg.pAxis_tapeLoad, retry_cnt) == 0)
		{
			ERR_PRINT("enTapeCut - InitUnits : pAxis_tapeLoad->MotorOn Fail!");
			// error pop up
			return ERR_TAPECUT_AXIS_DISABLE;
		}

		/* 3. pCyl_pressGuide_R_UD */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_pressGuide_R_UD->Up() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeCut - InitUnits : pCyl_pressGuide_R_UD->Up Fail!");
			// error pop up
			return ERR_TAPECUT_CYLINDER_TIMEOUT;
		}
		/* 4. pCyl_pressGuide_L_UD */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_pressGuide_L_UD->Up() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeCut - InitUnits : pCyl_pressGuide_L_UD->Up Fail!");
			// error pop up
			return ERR_TAPECUT_CYLINDER_TIMEOUT;
		}

		/* 5. pCyl_pressGuide_L_UD */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_cutterKnife_UD->Up() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeCut - InitUnits : pCyl_cutterKnife_UD->Up Fail!");
			// error pop up
			return ERR_TAPECUT_CYLINDER_TIMEOUT;
		}


		/* 6. pCyl_tapePosition_UD */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_tapePosition_UD->Up() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeCut - InitUnits : pCyl_tapePosition_UD->Up Fail!");
			// error pop up
			return ERR_TAPECUT_CYLINDER_TIMEOUT;
		}

		/* 7. pCyl_holeDetector_UD */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_holeDetector_UD->Up() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeCut - InitUnits : pCyl_holeDetector_UD->Up Fail!");
			// error pop up
			return ERR_TAPECUT_CYLINDER_TIMEOUT;
		}

		/* 8. pCyl_centeringTape_OC */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_centeringTape_OC->Open() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeCut - InitUnits : pCyl_centeringTape_OC->Open Fail!");
			// error pop up
			return ERR_TAPECUT_CYLINDER_TIMEOUT;
		}


		/* 9. pCyl_cutterUnit_FB */
		retry_cnt = def_retry_cnt;
		while (retry_cnt)
		{
			if (m_cfg.pCyl_cutterUnit_FB->Forward() == ERROR_SUCCESS)
				break;
			retry_cnt--;
		}
		if (retry_cnt == 0)
		{
			ERR_PRINT("enTapeCut - InitUnits : pCyl_cutterUnit_FB->Forward Fail!");
			// error pop up
			return ERR_TAPECUT_CYLINDER_TIMEOUT;
		}


		/* 10. motor position init */
		double pose = m_cfg.p_motion_data->GetPositionData(GMotion_dat::addr_e::moon_ForBack_TU_forward);
		int velocity = (int)m_cfg.p_axis_data_ForBack->RunVelocity;
		if (m_cfg.pAxis_ForBack->Move((int)pose, velocity) != ERROR_SUCCESS)
		{
			ERR_PRINT("enTapeCut - InitUnits : pAxis_ForBack->Move Fail!");
			// error pop up
			return ERR_TAPECUT_AXIS_RET_ERR;
		}


		return ERROR_SUCCESS;
	}




}
// end of namespace AP_SYS