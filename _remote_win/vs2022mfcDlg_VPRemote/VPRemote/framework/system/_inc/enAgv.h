#pragma once



namespace AP_SYS
{

	class enAgv :public Icommon
	{
	public:
		struct cfg_t
		{
			std::shared_ptr<IIO> pIo{};
			AP_DATA::GConfig_dat* p_cfg_data{};

			cfg_t() = default;

			inline cfg_t& operator = (const cfg_t& cfg) {
				if (this != &cfg) {
					pIo = cfg.pIo;
					p_cfg_data = cfg.p_cfg_data;

				}
				return *this;
			}

		};

	private:
		cfg_t m_cfg;


	public:
		enAgv(common::Attribute_t& common_data, cfg_t& cfg)
			: Icommon(common_data), m_cfg(cfg)
		{
		}

		virtual ~enAgv()
		{
		}

	private:

	};
	// end of namesapce class enRobot
}
// end of namespace AP_SYS
