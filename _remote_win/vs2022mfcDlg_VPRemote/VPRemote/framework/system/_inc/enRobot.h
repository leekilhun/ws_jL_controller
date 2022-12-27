#pragma once

namespace AP_SYS
{

	class enRobot :public Icommon
	{
	public:
		struct cfg_t
		{
			std::shared_ptr<IIO> pIo{};
			AP_DATA::GConfig_dat* p_cfg_data{};
			IDistMeasure* pDistSensor{};

			IAxis* pAxis_rewind{};
			FANUC_ROBOT::GFanucRobot* pFanucRobot{};
			cfg_t() = default;

			inline cfg_t& operator = (const cfg_t& cfg) {
				if (this != &cfg) {
					pIo = cfg.pIo;
					p_cfg_data = cfg.p_cfg_data;

					pDistSensor = cfg.pDistSensor;
					pAxis_rewind = cfg.pAxis_rewind;
					pFanucRobot = cfg.pFanucRobot;
				}
				return *this;
			}

		};

	private:
		cfg_t m_cfg;


	public:
		enRobot(common::Attribute_t& common_data, cfg_t& cfg)
			: Icommon(common_data), m_cfg(cfg)
		{
		}

		virtual ~enRobot()
		{
		}

	private:

	};
	// end of namesapce class enRobot
}
// end of namespace AP_SYS
