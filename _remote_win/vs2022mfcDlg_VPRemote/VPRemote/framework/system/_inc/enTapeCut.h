#pragma once



namespace AP_SYS
{

	class enTapeCut :public Icommon
	{
	public:
		enum axis_e
		{
			Axis_tapeLoad,
			Axis_ForBack,
			Axis_max,
		};

		enum cyl_e 
		{
			Cyl_cutterUnit,
			Cyl_pressGuide_R,
			Cyl_pressGuide_L,
			Cyl_cutterKnife,
			Cyl_tapePosition,
			Cyl_holeDetector,
			Cyl_centeringTape,

			Cyl_max

		};


		struct cfg_t
		{
			std::shared_ptr<IIO> pIo{};
			AP_DATA::GConfig_dat* p_cfg_data{};
			AP_DATA::GMotion_dat* p_motion_data{};

			IAxis* pAxis_tapeLoad{};
			IAxis* pAxis_ForBack{};

			AP_DATA::GAxis_dat::dat_t* p_axis_data_tapeLoad{};
			AP_DATA::GAxis_dat::dat_t* p_axis_data_ForBack{};
			
			MCylinder* pCyl_cutterUnit_FB{};
			MCylinder* pCyl_pressGuide_R_UD{};
			MCylinder* pCyl_pressGuide_L_UD{};
			MCylinder* pCyl_cutterKnife_UD{};
			MCylinder* pCyl_tapePosition_UD{};
			MCylinder* pCyl_holeDetector_UD{};
			MCylinder* pCyl_centeringTape_OC{};

			cfg_t() = default;

			inline cfg_t& operator = (const cfg_t& cfg) {
				if (this != &cfg) {
					pIo = cfg.pIo;
					p_cfg_data = cfg.p_cfg_data;
					p_motion_data = cfg.p_motion_data;

					p_axis_data_tapeLoad = cfg.p_axis_data_tapeLoad;
					p_axis_data_ForBack = cfg.p_axis_data_ForBack;

					pAxis_tapeLoad = cfg.pAxis_tapeLoad;
					pAxis_ForBack = cfg.pAxis_ForBack;
					pCyl_cutterUnit_FB = cfg.pCyl_cutterUnit_FB;
					pCyl_pressGuide_R_UD = cfg.pCyl_pressGuide_R_UD;
					pCyl_pressGuide_L_UD = cfg.pCyl_pressGuide_L_UD;
					pCyl_cutterKnife_UD = cfg.pCyl_cutterKnife_UD;
					pCyl_tapePosition_UD = cfg.pCyl_tapePosition_UD;
					pCyl_holeDetector_UD = cfg.pCyl_holeDetector_UD;
					pCyl_centeringTape_OC = cfg.pCyl_centeringTape_OC;
				}
				return *this;
			}

		};

	private:
		cfg_t m_cfg;

	public:
		enTapeCut(common::Attribute_t& common_data, cfg_t& cfg)
			: Icommon(common_data), m_cfg(cfg)
		{
		}

		virtual ~enTapeCut()
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
			case Cyl_cutterUnit:
				interlock = m_cfg.pCyl_pressGuide_R_UD->IsUp();
				interlock &= m_cfg.pCyl_pressGuide_L_UD->IsUp();
				interlock &= m_cfg.pCyl_cutterKnife_UD->IsUp();
				interlock &= m_cfg.pCyl_tapePosition_UD->IsUp();
				interlock &= m_cfg.pCyl_holeDetector_UD->IsUp();
				if (interlock == false)
					return ERR_TAPECUT_CYLINDER_MOVE_INTERLOCK;
				ret = m_cfg.pCyl_cutterUnit_FB->Forward(skip_sensing);
				break;

			case Cyl_pressGuide_R:
				ret = m_cfg.pCyl_pressGuide_R_UD->Up(skip_sensing);
				break;

			case Cyl_pressGuide_L:
				ret = m_cfg.pCyl_pressGuide_L_UD->Up(skip_sensing);
				break;

			case Cyl_cutterKnife:
				ret = m_cfg.pCyl_cutterKnife_UD->Up(skip_sensing);
				break;

			case Cyl_tapePosition:
				ret = m_cfg.pCyl_tapePosition_UD->Up(skip_sensing);
				break;

			case Cyl_holeDetector:
				ret = m_cfg.pCyl_holeDetector_UD->Up(skip_sensing);
				break;

			case Cyl_centeringTape:
				ret = m_cfg.pCyl_centeringTape_OC->Open(skip_sensing);
				break;
			default:
				break;
			}
			if (ret != ERROR_SUCCESS)
				return ERR_TAPECUT_CYLINDER_TIMEOUT;
			return (errno_t)ret;
		}

		inline errcode Off_Cyl(cyl_e cyl, bool skip_sensing = true) {
			int ret = 0;
			bool interlock{ false };
			switch (cyl)
			{
			case Cyl_cutterUnit:
				interlock = m_cfg.pCyl_pressGuide_R_UD->IsUp();
				interlock &= m_cfg.pCyl_pressGuide_L_UD->IsUp();
				interlock &= m_cfg.pCyl_cutterKnife_UD->IsUp();
				interlock &= m_cfg.pCyl_tapePosition_UD->IsUp();
				interlock &= m_cfg.pCyl_holeDetector_UD->IsUp();
				if (interlock == false)
					return ERR_TAPECUT_CYLINDER_MOVE_INTERLOCK;
				ret = m_cfg.pCyl_cutterUnit_FB->Backward(skip_sensing);
				break;

			case Cyl_pressGuide_R:
				ret = m_cfg.pCyl_pressGuide_R_UD->Down(skip_sensing);
				break;

			case Cyl_pressGuide_L:
				ret = m_cfg.pCyl_pressGuide_L_UD->Down(skip_sensing);
				break;

			case Cyl_cutterKnife:
				ret = m_cfg.pCyl_cutterKnife_UD->Down(skip_sensing);
				break;

			case Cyl_tapePosition:
				ret = m_cfg.pCyl_tapePosition_UD->Down(skip_sensing);
				break;

			case Cyl_holeDetector:
				ret = m_cfg.pCyl_holeDetector_UD->Down(skip_sensing);
				break;

			case Cyl_centeringTape:
				ret = m_cfg.pCyl_centeringTape_OC->Close(skip_sensing);
				break;

			default:
				break;
			}
			if (ret != ERROR_SUCCESS)
				return ERR_TAPECUT_CYLINDER_TIMEOUT;
			return (errno_t)ret;
		}

		inline errcode Stop_Motor(axis_e axis = axis_e::Axis_max) {
			switch (axis)
			{
			case Axis_ForBack:
				if (m_cfg.pAxis_ForBack->Stop() != ERROR_SUCCESS)
					return ERR_TAPECUT_AXIS_RET_ERR;
				break;

			case Axis_tapeLoad:
				if (m_cfg.pAxis_tapeLoad->Stop() != ERROR_SUCCESS)
					return ERR_TAPECUT_AXIS_RET_ERR;
				break;

			case Axis_max:
				__fallthrough;
			default:
				if (m_cfg.pAxis_ForBack->Stop() != ERROR_SUCCESS) return ERR_TAPELOADER_AXIS_RET_ERR;
				if (m_cfg.pAxis_tapeLoad->Stop() != ERROR_SUCCESS) return ERR_TAPELOADER_AXIS_RET_ERR;
				break;
			}
			return ERROR_SUCCESS;
		}


		inline errcode Loading_OldReelTape() {
			if (m_cfg.pAxis_tapeLoad->IsMotorOn() == false)
				return ERR_TAPECUT_AXIS_DISABLE;

			if (m_cfg.pAxis_tapeLoad->JogMove(1000) != ERROR_SUCCESS)
				return ERR_TAPECUT_AXIS_RET_ERR;
			return ERROR_SUCCESS;
		}



		inline errcode Forward_TapeLoader() {
			if (m_cfg.pAxis_ForBack->IsMotorOn() == false)
				return ERR_TAPECUT_AXIS_DISABLE;

			double pose = m_cfg.p_motion_data->GetPositionData(GMotion_dat::addr_e::moon_ForBack_TU_forward);
			int velocity = (int)m_cfg.p_axis_data_ForBack->RunVelocity;
			if (m_cfg.pAxis_ForBack->Move((int)pose, velocity) != ERROR_SUCCESS)
			{
				ERR_PRINT("enTapeCut - Forward_TapeLoader : pAxis_ForBack->Move Fail!");
				// error pop up
				return ERR_TAPECUT_AXIS_RET_ERR;
			}
			return ERROR_SUCCESS;
		}

		inline errcode Backward_TapeLoader() {
			if (m_cfg.pAxis_ForBack->IsMotorOn() == false)
				return ERR_TAPECUT_AXIS_DISABLE;

			double pose = m_cfg.p_motion_data->GetPositionData(GMotion_dat::addr_e::moon_ForBack_TU_backward);
			int velocity = (int)m_cfg.p_axis_data_ForBack->RunVelocity;
			if (m_cfg.pAxis_ForBack->Move((int)pose, velocity) != ERROR_SUCCESS)
			{
				ERR_PRINT("enTapeCut - Backward_TapeLoader : pAxis_ForBack->Move Fail!");
				// error pop up
				return ERR_TAPECUT_AXIS_RET_ERR;
			}
			return ERROR_SUCCESS;
		}


	};
	// end of class enTapeCut

}
// end of namespace AP_SYS