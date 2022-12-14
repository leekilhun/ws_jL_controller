/*
 * ap_io.h
 *
 *  Created on: 2022. 3. 1.
 *      Author: gns2l
 */

#ifndef SRC_AP_AP_IO_H_
#define SRC_AP_AP_IO_H_

namespace MCU_IO
{

#ifdef _USE_HW_RTOS
#define AP_IO_LOCK_BEGIN osMutexWait(ap_io_mutex_id, osWaitForever)
#define AP_IO_LOCK_END   osMutexRelease(ap_io_mutex_id)
#else
#define AP_IO_LOCK_BEGIN
#define AP_IO_LOCK_END
#endif




	class ap_io: public iio
	{
#ifdef _USE_HW_RTOS
		osMutexId ap_io_mutex_id;
#endif

	public:
		enum in_e// in_name
		{
			in_vinyl_vac_on = AP_DEF_START_IN_ADDR,
			in_jig_on_phone,
			in_mode_key_manual,
			in_modde_key_auto,
			in_04,
			in_05,
			in_06,
			in_safety_sensor, //

			in10, // reserved
			in11,
			in12,
			in13,
			in14,
			in15,
			in16,
			in17,

			in_cyl_jig_for, // ext_io 1
			in_cyl_jig_back,
			in_cyl_jig_open,
			in_cyl_jig_close,
			in_cyl_vinyl_push,
			in_cyl_vinyl_back,
			in_cyl_vinylhold_up,
			in_cyl_vinylhold_down,

			in_30,
			in_grip_vinyle_detect,
			in32,
			in33,
			in34,
			in35,
			in36,
			in37,

		};

		enum out_e//out_name
		{
			out00 = AP_DEF_START_OUT_ADDR,
			out01,
			out02,
			out03,
			out_jig_axis_dir,
			out_jig_axis_pulse,
			out_roll_axis_dir,
			out_roll_axis_pulse,

			out10, // reserved
			out11,
			out12,
			out13,
			out14,
			out15,
			out16,
			out17,

			out_cyl_jig_for, // ext_io 1
			out_cyl_jig_back,
			out_cyl_jig_open,
			out_cyl_jig_close,
			out_cyl_vinyl_push,
			out_cyl_vinyl_back,
			out_cyl_vinylhold_up,
			out_cyl_vinylhold_down,

			out_vac_detect_on,
			out_vac_detect_blow,
			out32,
			out33,
			out34,
			out35,
			out36,
			out37,
		};

		union in_u
		{
			uint32_t data{};
			struct
			{
				unsigned in_vinyl_vac_on:1;
				unsigned in_jig_on_phone:1;
				unsigned in_mode_key_manual:1;
				unsigned in_modde_key_auto:1;
				unsigned in04:1;
				unsigned in05:1;
				unsigned in06:1;
				unsigned in_safety_sensor:1;

				unsigned in10:1;
				unsigned in11:1;
				unsigned in12:1;
				unsigned in13:1;
				unsigned in14:1;
				unsigned in15:1;
				unsigned in16:1;
				unsigned in17:1;

				unsigned in_cyl_jig_for:1;
				unsigned in_cyl_jig_back:1;
				unsigned in_cyl_jig_open:1;
				unsigned in_cyl_jig_close:1;
				unsigned in_cyl_vinyl_push:1;
				unsigned in_cyl_vinyl_back:1;
				unsigned in_cyl_vinylhold_up:1;
				unsigned in_cyl_vinylhold_down:1;

				unsigned in30:1;
				unsigned in_grip_vinyle_detect:1;
				unsigned in32:1;
				unsigned in33:1;
				unsigned in34:1;
				unsigned in35:1;
				unsigned in36:1;
				unsigned in37:1;
			};
		};

		// 1bank 4byte
		union out_u
		{
			uint32_t data{};
			struct
			{
				unsigned out00:1;
				unsigned out01:1;
				unsigned out02:1;
				unsigned out03:1;
				unsigned out_jig_axis_dir:1;
				unsigned out_jig_axis_pulse:1;
				unsigned out_roll_axis_dir:1;
				unsigned out_roll_axis_pulse:1;

				unsigned out10:1;
				unsigned out11:1;
				unsigned out12:1;
				unsigned out13:1;
				unsigned out14:1;
				unsigned out15:1;
				unsigned out16:1;
				unsigned out17:1;

				unsigned out_cyl_jig_for:1;
				unsigned out_cyl_jig_back:1;
				unsigned out_cyl_jig_open:1;
				unsigned out_cyl_jig_close:1;
				unsigned out_cyl_vinyl_push:1;
				unsigned out_cyl_vinyl_back:1;
				unsigned out_cyl_vinylhold_up:1;
				unsigned out_cyl_vinylhold_down:1;

				unsigned out_vac_detect_on:1;
				unsigned out_vac_detect_blow:1;
				unsigned out32:1;
				unsigned out33:1;
				unsigned out34:1;
				unsigned out35:1;
				unsigned out36:1;
				unsigned out37:1;
			};
		};



		in_u m_in;
		out_u m_out;

		uint16_t m_extIn;
		uint16_t m_extOut;

		uint8_t m_errCnt;
	public:
		ap_io() : m_in{}, m_out{} , m_extIn{0}, m_extOut{0} ,m_errCnt{}{
#ifdef _USE_HW_RTOS
			osMutexDef(ap_io_mutex_id);
			ap_io_mutex_id = osMutexCreate (osMutex(ap_io_mutex_id));
#endif
		}
		virtual ~ap_io(){}

		inline int Init(){
			pca8575pw_Read(_PCA8575PW_EX_IO_CH_OUT, &m_extOut);
			return ERROR_SUCCESS;
		}
		inline void assignObj(iio* p_io){
			//pIo = p_io;
		}

		//Update information that has changed compared to previous data
		inline void SetOutputReg(uint32_t reg, uint8_t bank = 0)
		{
			uint32_t x_reg = reg ^ m_out.data;

			for(uint8_t i = 0 ; i < 32; i++)
			{
				if((x_reg >>(i))&1)
				{
					SetGpioOut((out_e)(i+AP_DEF_START_OUT_ADDR),((reg>>(i))&1));
				}
			}
		}

		inline int SetGpioOut(out_e out_idx, bool onoff = true){
			AP_IO_LOCK_BEGIN;
			switch (out_idx)
			{
				case out00:
					gpioPinWrite(_GPIO_IO_OUT_01, onoff);
					break;
				case out01:
					gpioPinWrite(_GPIO_IO_OUT_02, onoff);
					break;
				case out02:
					gpioPinWrite(_GPIO_IO_OUT_03, onoff);
					break;
				case out03:
					gpioPinWrite(_GPIO_IO_OUT_04, onoff);
					break;
				case out_jig_axis_dir:
					gpioPinWrite(_GPIO_IO_OUT_05, onoff);
					break;
				case out_jig_axis_pulse:
					gpioPinWrite(_GPIO_IO_OUT_06, onoff);
					break;
				case out_roll_axis_dir:
					gpioPinWrite(_GPIO_IO_OUT_07, onoff);
					break;
				case out_roll_axis_pulse:
					gpioPinWrite(_GPIO_IO_OUT_08, onoff);
					break;
				case out_e::out10:
				case out_e::out11:
				case out_e::out12:
				case out_e::out13:
				case out_e::out14:
				case out_e::out15:
				case out_e::out16:
				case out_e::out17:

				case out_e::out_cyl_jig_for:
				case out_e::out_cyl_jig_back:
				case out_e::out_cyl_jig_open:
				case out_e::out_cyl_jig_close:
				case out_e::out_cyl_vinyl_push:
				case out_e::out_cyl_vinyl_back:
				case out_e::out_cyl_vinylhold_up:
				case out_e::out_cyl_vinylhold_down:
				case out_e::out_vac_detect_on:
				case out_e::out_vac_detect_blow:
				case out_e::out32:
				case out_e::out33:
				case out_e::out34:
				case out_e::out35:
				case out_e::out36:
				case out_e::out37:
				{
					uint32_t no = 0;
					if (onoff)
					{
						no = 1<<((uint32_t)out_idx - (uint32_t)out_cyl_jig_for );
						m_extOut = m_extOut | no;
					}
					else
					{
						no = ~(1<<((uint32_t)out_idx - (uint32_t)out_cyl_jig_for  ));
						m_extOut = m_extOut & no;
					}
				}
				break;


				default:
					break;
			}
			AP_IO_LOCK_END;
			return 0;
		}

		inline void Update_io(void){
			enum
			{
				bank_1,
				bank_2,
				bank_3,
				bank_4,
				_max
			};
			std::array<uint8_t, _max> data {};

			uint8_t gpio {};
			// out
			{
				gpio = 0x0 ;
				gpio = gpioPinRead(_GPIO_IO_OUT_01) << 0
						| gpioPinRead(_GPIO_IO_OUT_02) << 1
						| gpioPinRead(_GPIO_IO_OUT_03) << 2
						| gpioPinRead(_GPIO_IO_OUT_04) << 3
						| gpioPinRead(_GPIO_IO_OUT_05) << 4
						| gpioPinRead(_GPIO_IO_OUT_06) << 5
						| gpioPinRead(_GPIO_IO_OUT_07) << 6
						| gpioPinRead(_GPIO_IO_OUT_08) << 7
						;
				data[bank_1] = gpio;

				data[bank_3] =  (uint8_t)(m_extOut >> 0);
				data[bank_4] =  (uint8_t)(m_extOut >> 8);

				m_out.data = utilDwToUint(data.data());

				// extension io module
				{
					uint16_t out_data = m_extOut;
					if(pca8575pw_Write(_PCA8575PW_EX_IO_CH_OUT, ~(out_data)) == false)
					{
						++m_errCnt;
					}
				}
				//pca8575pw_Read(_PCA8575PW_EX_IO_CH_OUT, &m_extOut);

			}


			// in
			{
				data.fill(0);
				gpio = 0x0 ;
				gpio = gpioPinRead(_GPIO_IO_IN_01) << 0
						| gpioPinRead(_GPIO_IO_IN_02) << 1
						| gpioPinRead(_GPIO_IO_IN_03) << 2
						| gpioPinRead(_GPIO_IO_IN_04) << 3
						| gpioPinRead(_GPIO_IO_IN_05) << 4
						| gpioPinRead(_GPIO_IO_IN_06) << 5
						| gpioPinRead(_GPIO_IO_IN_07) << 6
						| gpioPinRead(_GPIO_IO_IN_08) << 7
						;
				data[bank_1] = gpio;

				// extension io module
				if (pca8575pw_Read(_PCA8575PW_EX_IO_CH_IN, &m_extIn))
				{
					data[bank_3] =  (uint8_t)(m_extIn >> 0);
					data[bank_4] =  (uint8_t)(m_extIn >> 8);
				}
				else
				{
					++m_errCnt;
				}

				m_in.data = utilDwToUint(data.data());
			}

			/* check i2c communication recovery*/
			constexpr uint8_t limit_recovery_cnt = 100;
			if (m_errCnt >= limit_recovery_cnt)
			{
				m_errCnt = 0;
				pca8575pw_Recovery();
				Init();
			}

		}



		/* IO  ??????*/



		/* IO  control function */
		inline bool IsOn(uint32_t addr) {
			bool ret = false;
			if (addr <AP_DEF_START_OUT_ADDR)
			{
				//in
				ret = (bool)((m_in.data >> (addr - AP_DEF_START_IN_ADDR))&1);
			}
			else
			{
				//out
				ret = (bool)((m_out.data >> (addr - AP_DEF_START_OUT_ADDR))&1);

			}
			return ret;
		}

		inline bool IsOff(uint32_t addr) {
			return !IsOn(addr);
		}

		inline int OutputOn(uint32_t addr) {
			return SetGpioOut((out_e)addr);
		}

		inline int OutputOff(uint32_t addr) {
			return SetGpioOut((out_e)addr, false);
		}

		inline int OutputToggle(uint32_t addr){
			int ret = ERROR_SUCCESS;
			if (IsOn(addr))
				ret = OutputOff(addr);
			else
				ret = OutputOn(addr);
			return ret;
		}


	};

}
#endif /* SRC_AP_AP_IO_H_ */
