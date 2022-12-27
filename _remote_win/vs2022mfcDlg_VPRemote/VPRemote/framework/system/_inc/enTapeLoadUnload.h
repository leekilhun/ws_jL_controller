#pragma once


namespace AP_SYS
{
	class enTapeLoadUnload :public Icommon
	{
	public:
		enum axis_e
		{
			Axis_UpDown_L,
			Axis_UpDown_R,
			Axis_ForBack,
			Axis_splicingLoader_L,
			Axis_splicingLoader_R,
			Axis_max,
		};
		enum cyl_e
		{
			Cyl_loderUD_R,
			Cyl_loderUD_L,
			Cyl_loderFB_R,
			Cyl_loderFB_L,
			Cyl_tapePullOut_R,
			Cyl_tapePullOut_L,
			Cyl_max
		};

		struct cfg_t
		{
			std::shared_ptr<IIO> pIo{};
			AP_DATA::GConfig_dat* p_cfg_data{};
			AP_DATA::GMotion_dat* p_motion_data{};

			IAxis* pAxis_UpDown_L{};
			IAxis* pAxis_UpDown_R{};
			IAxis* pAxis_ForBack{};
			IAxis* pAxis_splicingLoader_L{};
			IAxis* pAxis_splicingLoader_R{};
			AP_DATA::GAxis_dat::dat_t* p_axis_data_UpDown_L{};
			AP_DATA::GAxis_dat::dat_t* p_axis_data_UpDown_R{};
			AP_DATA::GAxis_dat::dat_t* p_axis_data_ForBack{};
			AP_DATA::GAxis_dat::dat_t* p_axis_data_splicingLoader_L{};
			AP_DATA::GAxis_dat::dat_t* p_axis_data_splicingLoader_R{};

			MCylinder* pCyl_loderUD_R_UD{};
			MCylinder* pCyl_loderUD_L_UD{};
			MCylinder* pCyl_loderFB_R_FB{};
			MCylinder* pCyl_loderFB_L_FB{};
			MCylinder* pCyl_tapePullOut_R_FB{};
			MCylinder* pCyl_tapePullOut_L_FB{};

			cfg_t() = default;

			inline cfg_t& operator = (const cfg_t& cfg) {
				if (this != &cfg){
					pIo = cfg.pIo;
					p_cfg_data = cfg.p_cfg_data;
					p_motion_data = cfg.p_motion_data;

					p_axis_data_UpDown_L = cfg.p_axis_data_UpDown_L;
					p_axis_data_UpDown_R = cfg.p_axis_data_UpDown_R;
					p_axis_data_ForBack = cfg.p_axis_data_ForBack;
					p_axis_data_splicingLoader_L = cfg.p_axis_data_splicingLoader_L;
					p_axis_data_splicingLoader_R = cfg.p_axis_data_splicingLoader_R;

					pAxis_UpDown_L = cfg.pAxis_UpDown_L;
					pAxis_UpDown_R = cfg.pAxis_UpDown_R;
					pAxis_ForBack = cfg.pAxis_ForBack;
					pAxis_splicingLoader_L = cfg.pAxis_splicingLoader_L;
					pAxis_splicingLoader_R = cfg.pAxis_splicingLoader_R;

					pCyl_loderUD_R_UD = cfg.pCyl_loderUD_R_UD;
					pCyl_loderUD_L_UD = cfg.pCyl_loderUD_L_UD;
					pCyl_loderFB_R_FB = cfg.pCyl_loderFB_R_FB;
					pCyl_loderFB_L_FB = cfg.pCyl_loderFB_L_FB;
					pCyl_tapePullOut_R_FB = cfg.pCyl_tapePullOut_R_FB;
					pCyl_tapePullOut_L_FB = cfg.pCyl_tapePullOut_L_FB;
				}
				return *this;
			}

		};

	private:
		cfg_t m_cfg;

	public:
		enTapeLoadUnload(common::Attribute_t& common_data, cfg_t& cfg)
			: Icommon(common_data), m_cfg(cfg)
		{
		}

		virtual ~enTapeLoadUnload()
		{
		}

	private:

	public:
		errcode InitUnits();

		inline errcode On_Cyl(cyl_e cyl, bool skip_sensing = true) {
			errno_t ret = 0;
			bool interlock{ false };
			switch (cyl)
			{
			case Cyl_loderUD_R:
				ret = m_cfg.pCyl_loderUD_R_UD->Up(skip_sensing);
				break;
			case Cyl_loderUD_L:
				ret = m_cfg.pCyl_loderUD_L_UD->Up(skip_sensing);
				break;
			case Cyl_loderFB_R:
				ret = m_cfg.pCyl_loderFB_R_FB->Up(skip_sensing);
				break;
			case Cyl_loderFB_L:
				ret = m_cfg.pCyl_loderFB_L_FB->Up(skip_sensing);
				break;
			case Cyl_tapePullOut_R:
				ret = m_cfg.pCyl_tapePullOut_R_FB->Forward(skip_sensing);
				break;
			case Cyl_tapePullOut_L:
				ret = m_cfg.pCyl_tapePullOut_L_FB->Forward(skip_sensing);
				break;
			default:
				break;
			}
			if (ret != ERROR_SUCCESS)
				return ERR_TAPELOADER_CYLINDER_TIMEOUT;
			return (errcode)ret;
		}

		inline errcode Off_Cyl(cyl_e cyl, bool skip_sensing = true) {
			errno_t ret = 0;
			bool interlock{ false };
			switch (cyl)
			{
			case Cyl_loderUD_R:
				ret = m_cfg.pCyl_loderUD_R_UD->Down(skip_sensing);
				break;
			case Cyl_loderUD_L:
				ret = m_cfg.pCyl_loderUD_L_UD->Down(skip_sensing);
				break;
			case Cyl_loderFB_R:
				ret = m_cfg.pCyl_loderFB_R_FB->Down(skip_sensing);
				break;
			case Cyl_loderFB_L:
				ret = m_cfg.pCyl_loderFB_L_FB->Down(skip_sensing);
				break;
			case Cyl_tapePullOut_R:
				ret = m_cfg.pCyl_tapePullOut_R_FB->Backward(skip_sensing);
				break;
			case Cyl_tapePullOut_L:
				ret = m_cfg.pCyl_tapePullOut_L_FB->Backward(skip_sensing);
				break;
			default:
				break;
			}
			if (ret != ERROR_SUCCESS)
				return ERR_TAPELOADER_CYLINDER_TIMEOUT;
			return (errcode)ret;
		}

		inline errcode Stop_Motor(axis_e axis = axis_e::Axis_max) {
			switch (axis)
			{
			case Axis_UpDown_L:
				if (m_cfg.pAxis_UpDown_L->Stop() != ERROR_SUCCESS) 
					return ERR_TAPELOADER_AXIS_RET_ERR;
				break;

			case Axis_UpDown_R:
				if (m_cfg.pAxis_UpDown_R->Stop() != ERROR_SUCCESS) 
					return ERR_TAPELOADER_AXIS_RET_ERR;
				break;

			case Axis_ForBack:
				if (m_cfg.pAxis_ForBack->Stop() != ERROR_SUCCESS) 
					return ERR_TAPELOADER_AXIS_RET_ERR;
				break;

			case Axis_splicingLoader_L:
				if (m_cfg.pAxis_splicingLoader_L->Stop() != ERROR_SUCCESS) 
					return ERR_TAPELOADER_AXIS_RET_ERR;
				break;

			case Axis_splicingLoader_R:
				if (m_cfg.pAxis_splicingLoader_R->Stop() != ERROR_SUCCESS) 
					return ERR_TAPELOADER_AXIS_RET_ERR;
				break;

			case Axis_max:
				__fallthrough;
			default:
				if (m_cfg.pAxis_ForBack->Stop() != ERROR_SUCCESS) return ERR_TAPELOADER_AXIS_RET_ERR;
				if (m_cfg.pAxis_UpDown_R->Stop() != ERROR_SUCCESS) return ERR_TAPELOADER_AXIS_RET_ERR;
				if (m_cfg.pAxis_UpDown_L->Stop() != ERROR_SUCCESS) return ERR_TAPELOADER_AXIS_RET_ERR;
				if (m_cfg.pAxis_splicingLoader_L->Stop() != ERROR_SUCCESS) return ERR_TAPELOADER_AXIS_RET_ERR;
				if (m_cfg.pAxis_splicingLoader_R->Stop() != ERROR_SUCCESS) return ERR_TAPELOADER_AXIS_RET_ERR;
				break;
			}

			return ERROR_SUCCESS;
		}


		inline errcode Loading_NewReelTape() {
			if (m_cfg.pAxis_splicingLoader_L->IsMotorOn() == false)
				return ERR_TAPELOADER_AXIS_DISABLE;

			if (m_cfg.pAxis_splicingLoader_L->JogMove(1000) != ERROR_SUCCESS)
				return  ERR_TAPELOADER_AXIS_RET_ERR;
			return ERROR_SUCCESS;
		}

		inline errcode Loading_OldReelTape() {
			if (m_cfg.pAxis_splicingLoader_R->IsMotorOn() == false)
				return ERR_TAPELOADER_AXIS_DISABLE;

			if (m_cfg.pAxis_splicingLoader_R->JogMove(1000) != ERROR_SUCCESS)
				return ERR_TAPELOADER_AXIS_RET_ERR;
			return ERROR_SUCCESS;
		}

		inline errcode Forward_TapeLoader() {
			if (m_cfg.pAxis_ForBack->IsMotorOn() == false)
				return ERR_TAPELOADER_AXIS_DISABLE;

			double pose = m_cfg.p_motion_data->GetPositionData(GMotion_dat::addr_e::moon_ForBack_SU_forward);
			int velocity = (int)m_cfg.p_axis_data_ForBack->RunVelocity;
			if (m_cfg.pAxis_ForBack->Move((int)pose, velocity) != ERROR_SUCCESS)
			{
				ERR_PRINT("enTapeLoadUnload - InitUnits : pAxis_ForBack->Move Fail!");
				// error pop up
				return ERR_TAPELOADER_AXIS_RET_ERR;
			}
			return ERROR_SUCCESS;
		}

		inline errcode Backward_TapeLoader() {
			if (m_cfg.pAxis_ForBack->IsMotorOn() == false)
				return ERR_TAPELOADER_AXIS_DISABLE;

			double pose = m_cfg.p_motion_data->GetPositionData(GMotion_dat::addr_e::moon_ForBack_SU_backward);
			int velocity = (int)m_cfg.p_axis_data_ForBack->RunVelocity;
			if (m_cfg.pAxis_ForBack->Move((int)pose, velocity) != ERROR_SUCCESS)
			{
				ERR_PRINT("enTapeLoadUnload - InitUnits : pAxis_ForBack->Move Fail!");
				// error pop up
				return ERR_TAPELOADER_AXIS_RET_ERR;
			}
			return ERROR_SUCCESS;
		}

		inline errcode Up_NewReelTapeLoader() {
			if (m_cfg.pAxis_UpDown_L->IsMotorOn() == false)
				return ERR_TAPELOADER_AXIS_DISABLE;

			double pose = m_cfg.p_motion_data->GetPositionData(GMotion_dat::addr_e::moon_UpDown_L_up);
			int velocity = (int)m_cfg.p_axis_data_UpDown_L->RunVelocity;
			if (m_cfg.pAxis_UpDown_L->Move((int)pose, velocity) != ERROR_SUCCESS)
			{
				ERR_PRINT("enTapeLoadUnload - InitUnits : pAxis_UpDown_L->Move Fail!");
				// error pop up
				return ERR_TAPELOADER_AXIS_RET_ERR;
			}
			return ERROR_SUCCESS;
		}

		inline errcode Down_NewReelTapeLoader() {
			if (m_cfg.pAxis_UpDown_L->IsMotorOn() == false)
				return ERR_TAPELOADER_AXIS_DISABLE;

			double pose = m_cfg.p_motion_data->GetPositionData(GMotion_dat::addr_e::moon_UpDown_L_down);
			int velocity = (int)m_cfg.p_axis_data_UpDown_L->RunVelocity;
			if (m_cfg.pAxis_UpDown_L->Move((int)pose, velocity) != ERROR_SUCCESS)
			{
				ERR_PRINT("enTapeLoadUnload - InitUnits : pAxis_UpDown_L->Move Fail!");
				// error pop up
				return ERR_TAPELOADER_AXIS_RET_ERR;
			}
			return ERROR_SUCCESS;
		}

		inline errcode Up_OldReelTapeLoader() {
			if (m_cfg.pAxis_UpDown_R->IsMotorOn() == false)
				return ERR_TAPELOADER_AXIS_DISABLE;

			double pose = m_cfg.p_motion_data->GetPositionData(GMotion_dat::addr_e::moon_UpDown_R_up);
			int velocity = (int)m_cfg.p_axis_data_UpDown_R->RunVelocity;
			if (m_cfg.pAxis_UpDown_R->Move((int)pose, velocity) != ERROR_SUCCESS)
			{
				ERR_PRINT("enTapeLoadUnload - InitUnits : pAxis_UpDown_R->Move Fail!");
				// error pop up
				return ERR_TAPELOADER_AXIS_RET_ERR;
			}
			return ERROR_SUCCESS;
		}

		inline errcode Down_OldReelTapeLoader() {
			if (m_cfg.pAxis_UpDown_R->IsMotorOn() == false)
				return ERR_TAPELOADER_AXIS_DISABLE;

			double pose = m_cfg.p_motion_data->GetPositionData(GMotion_dat::addr_e::moon_UpDown_R_down);
			int velocity = (int)m_cfg.p_axis_data_UpDown_R->RunVelocity;
			if (m_cfg.pAxis_UpDown_R->Move((int)pose, velocity) != ERROR_SUCCESS)
			{
				ERR_PRINT("enTapeLoadUnload - InitUnits : pAxis_UpDown_R->Move Fail!");
				// error pop up
				return ERR_TAPELOADER_AXIS_RET_ERR;
			}
			return ERROR_SUCCESS;
		}

	};
	// end of class enTapeLoadUnload

} 
// end of namespace AP_SYS
