/*
 * enOp.hpp
 *
 *  Created on: 2022. 12. 3.
 *      Author: gns2l
 */

#ifndef SRC_AP__INC_ENOP_HPP_
#define SRC_AP__INC_ENOP_HPP_



class enOp
{

public:
	enum mode_e
	{
		READY,
		AUTORUN,
		STOP,
		DRY_RUN,
	};

	enum status_e
	{
		INIT,
		ERR_STOP,
		STEP_STOP,
		RUN_READY,
		RUN,
	};

	enum panel_e
	{
		SW_START,
		SW_STOP,
		SW_RESET,
		SW_ESTOP,
		SW_MAX
	};

	enum lamp_e
	{
		LAMP_START,
		LAMP_STOP,
		LAMP_RESET,
	};

	struct cfg_t
	{
		uint8_t sw_pin_start{};
		uint8_t sw_pin_stop{};
		uint8_t sw_pin_reset{};
		uint8_t sw_pin_estop{};


    uint8_t lamp_pin_start{};
		uint8_t lamp_pin_stop{};
		uint8_t lamp_pin_reset{};

		cfg_t() = default;

		inline cfg_t& operator = (const cfg_t& cfg) {
			if ( this != &cfg){
				sw_pin_start = cfg.sw_pin_start;
				sw_pin_stop = cfg.sw_pin_stop;
				sw_pin_reset = cfg.sw_pin_reset;
				sw_pin_estop = cfg.sw_pin_estop;

				lamp_pin_start = cfg.lamp_pin_start;
				lamp_pin_stop = cfg.lamp_pin_stop;
				lamp_pin_reset= cfg.lamp_pin_reset;

			}
			return *this;
		}

	};

private:
	enOp::status_e m_status;
	enOp::mode_e m_mode;
	enOp::cfg_t m_cfg;

public:
	enOp(){
		m_status = enOp::status_e::INIT;
		m_mode = enOp::mode_e::STOP;
	}
	virtual ~enOp(){}
	inline int Init(enOp::cfg_t& cfg){
		m_cfg = cfg;
		return 0;
	}

	inline enOp::status_e GetStatus() const{
		return m_status;
	}

	inline void SetStatus(enOp::status_e state){
		m_status = state;
	}

	inline enOp::mode_e GetMode() const{
		return m_mode;
	}

	inline void SetMode(enOp::mode_e md){
		m_mode = md;
	}


	inline bool GetPressed(enOp::panel_e op_sw){
		uint8_t ch = 0;
		switch (op_sw)
		{
			case enOp::panel_e::SW_START:
				ch = m_cfg.sw_pin_start;
				break;

			case enOp::panel_e::SW_STOP:
				ch = m_cfg.sw_pin_stop;
				break;

			case enOp::panel_e::SW_RESET:
				ch = m_cfg.sw_pin_reset;
				break;

			case enOp::panel_e::SW_ESTOP:
				ch = m_cfg.sw_pin_estop;
				break;

			default:
				break;
		}
		return gpioPinRead(ch);
	}

	inline void LampOnOff(lamp_e lamp, bool state = true){
		switch (lamp) {
			case LAMP_START:
				gpioPinWrite(m_cfg.lamp_pin_start, state);
				break;
			case LAMP_STOP:
				gpioPinWrite(m_cfg.lamp_pin_stop, state);
				break;
			case LAMP_RESET:
				gpioPinWrite(m_cfg.lamp_pin_reset, state);
				break;
			default:
				break;
		};
	}

	inline void LampToggle(lamp_e lamp){
			switch (lamp) {
				case LAMP_START:
					gpioPinToggle(m_cfg.lamp_pin_start);
					break;
				case LAMP_STOP:
					gpioPinToggle(m_cfg.lamp_pin_stop);
					break;
				case LAMP_RESET:
					gpioPinToggle(m_cfg.lamp_pin_reset);
					break;
				default:
					break;
			};
		}


};





#endif /* SRC_AP__INC_ENOP_HPP_ */
