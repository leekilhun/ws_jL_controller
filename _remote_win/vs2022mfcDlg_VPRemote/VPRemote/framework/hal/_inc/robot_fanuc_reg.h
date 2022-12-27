#pragma once

/*

사용한 인자를 정의한다


*/
namespace FANUC_ROBOT
{

	enum pos_reg
	{
		GP1,
		GP2,
		GP3,
		GP4,
		GP5,
	};

	enum class io_e
	{
		robot,digital,user_op,system_op,group,analog,wio
	};

	enum task
	{
		ignore_macro,
		ignore_karel,
		ignore_both,
		_all,

	};

	enum class fanuc_dio_rob_2_sys
	{
		out_1 = 1,
		out_2, out_3, out_4, out_5, out_6, out_7, out_8,
		_max
	};

	enum class fanuc_dio_sys_2_rob
	{
		in_1 = 1,
		in_2, in_3, in_4, in_5, in_6, in_7, in_8,
		_max
	};


	enum class fanuc_rio_rob_2_sys
	{
		out_1 = 1,
		out_2, out_3, out_4, out_5, out_6, out_7, out_8,
		_max
	};

	enum class fanuc_rio_sys_2_rob
	{
		in_1 = 1,
		in_2, in_3, in_4, in_5, in_6, in_7, in_8,
		_max
	};

	enum class fanuc_uio_rob_2_sys
	{
		out_1 = 1,
		out_2, out_3, out_4, out_5, out_6, out_7, out_8,
		out_9, out_10, out_11, out_12, out_13, out_14, out_15,
		out_16, out_17, out_18, out_19, out_20,
		_max
	};

	enum class fanuc_uio_sys_2_rob
	{
		in_1 = 1,
		in_2, in_3, in_4, in_5, in_6, in_7, in_8,
		in_9, in_10, in_11, in_12, in_13, in_14, in_15,
		in_16, in_17, in_18,
		_max
	};


	enum class fanuc_sio_rob_2_sys
	{
		out_1 = 0,
		out_2, out_3, out_4, out_5, out_6, out_7, out_8,
		out_9, out_10, out_11, out_12, out_13, out_14, out_15, out_16,
		_max
	};

	enum class fanuc_sio_sys_2_rob
	{
		in_1 = 0,
		in_2, in_3, in_4, in_5, in_6, in_7, in_8,
		in_9, in_10, in_11, in_12, in_13, in_14, in_15, in_16,
		_max
	};

	enum class fanuc_gio_rob_2_sys
	{
		out_1 = 1,
		out_2, out_3, out_4, out_5, out_6, out_7, out_8,
		_max
	};

	enum class fanuc_gio_sys_2_rob
	{
		in_1 = 1,
		in_2, in_3, in_4, in_5, in_6, in_7, in_8,
		_max
	};





	/**
	 * X, Y, T 좌표 값에 대한 +/- 연산자 및 값 설정에 관련된 기능을 수행한다.
	 */

	 //template <typename T>
	struct robot_coordinate
	{
		/** XYZ 좌표 값 */
		float X{};
		float Y{};
		float Z{};

		/** ABC 좌표 값 */
		float W{};
		float P{};
		float R{};

		/* 좌표계 */
		int UF{};
		int UT{};

		short valid{};
		// FANUC 일 경우, config 저장
		std::array<short, 7> config{};	// Config
		std::array<float, 9> posdata{};

		robot_coordinate() = default;

		inline errno_t Set(std::vector<float>& xyzwpr_v, std::vector<short>& config_v, short uf, short ut, short valid_c) {
			if (xyzwpr_v.size() != 9)
				return -1;
			if (config_v.size() != 7)
				return -2;
			for (size_t i = 0; i < xyzwpr_v.size(); i++)
			{
				posdata[i] = xyzwpr_v[i];
			}

			X = xyzwpr_v[0]; Y = xyzwpr_v[1]; Z = xyzwpr_v[2]; W = xyzwpr_v[3]; P = xyzwpr_v[4]; R = xyzwpr_v[5];
			for (size_t i = 0; i < 7; i++)
			{
				config[i] = config_v[i];
			}
			UF = uf;
			UT = ut;
			valid = valid_c;

			return ERROR_SUCCESS; 
		}

		inline std::string to_string() {
			/*
			UF = 0, UT = 4
			XYZWPR = -727.362244 -137.107071 -9.191835 59.087845 -46.146450 -85.941910 0.000000 0.000000 0.000000
			CONFIG = NRUT 0 0 0
			*/
			std::ostringstream stringStream("");
			// Formatting. 소수점 3자리로 고정한다.
			stringStream << std::fixed << std::setprecision(3);

			stringStream << "UF = " << UF << ", ";
			stringStream << "UT = " << UT << "\r\n";
			if (valid)
			{
				// Create XYZWPR.
				stringStream << "XYZWPR = ";
				for (int i = 0; i < posdata.size() - 1; i++)
				{
					stringStream << posdata[i] << " ";
				}
				stringStream << posdata[posdata.size() - 1] << "\r\n";

				/*
				stringStream << "XYZWPR = ";
				stringStream << posdata[0] << " ";
				stringStream << posdata[1] << " ";
				stringStream << posdata[2] << " ";
				stringStream << posdata[3] << " ";
				stringStream << posdata[4] << " ";
				stringStream << posdata[5] << " ";

				stringStream << "\r\n";
				*/

				// Create CONFIG.
				stringStream << "CONFIG = ";
				if (config[0]) stringStream << "F"; else  stringStream << "N";
				if (config[1]) stringStream << "L"; else  stringStream << "R";
				if (config[2]) stringStream << "U"; else  stringStream << "D";
				if (config[3]) stringStream << "T"; else  stringStream << "B";
				stringStream << " " << config[4] << " " << config[5] << " " << config[6] << "\r\n";
			}


			return stringStream.str();
		}
	};



	struct robot_joint
	{
		/** XYZ 좌표 값 */
		float J1{};
		float J2{};
		float J3{};

		/** ABC 좌표 값 */
		float J4{};
		float J5{};
		float J6{};

		/* 좌표계 */
		int UT{};

		short valid{};

		robot_joint() = default;

		inline errno_t Set(std::vector<float>& joint_v, short ut, short valid_c) {
			if (joint_v.size() != 9)
				return -1;

			J1 = joint_v[0]; J2 = joint_v[1]; J3 = joint_v[2]; J4 = joint_v[3]; J5 = joint_v[4]; J6 = joint_v[5];			
			UT = ut;
			valid = valid_c;

			return ERROR_SUCCESS;
		}

		inline std::string to_string() {
			/*
			UF = 0, UT = 4
			JOINT = -167.033936 40.313892 -64.404846 150.070053 -88.830925 -127.166168 0.000000 0.000000 0.000000
			*/
			std::ostringstream stringStream("");
			// Formatting. 소수점 6자리로 고정한다.
			stringStream << std::fixed << std::setprecision(3);

			stringStream << "UT = " << UT << "\r\n";
			if (valid)
			{
				// Create XYZWPR.
				stringStream << "JOINT = ";
				stringStream << J1 << " ";
				stringStream << J2 << " ";
				stringStream << J3 << " ";
				stringStream << J4 << " ";
				stringStream << J5 << " ";
				stringStream << J6 << " ";

				stringStream << "\r\n";
			}


			return stringStream.str();
		}
	};



	struct task_t
	{
		short Line_number{};
		short State{};
		std::string ProgName{};
		std::string ParentProgName{};

		task_t() = default;

		inline errno_t Set(std::string& name, short line, short state, std::string& parent_name) {
			Line_number = line;
			State = state;
			ProgName = name;
			ParentProgName = parent_name;
			return ERROR_SUCCESS;
		}

		inline std::string to_string() {
			/*
			TASK1 : Prog= Line=0 State=0 ParentProg=
			*/
			std::ostringstream stringStream("");
			// Formatting. 소수점 6자리로 고정한다.
			stringStream << "TASK" << 1 << " : ";
			stringStream << "Prog=" << ProgName << " ";
			stringStream << "Line=" << Line_number << " ";
			stringStream << "State=" << State << " ";
			stringStream << "ParentProg=" << ParentProgName << "\r\n";


			return stringStream.str();
		}

	};

	template<typename T>
	struct io_t
	{
		size_t data_size{};
		T* p_value{};
		DataString* p_command{};
		std::string* p_desc{};

		io_t() = default;

		inline errno_t Set(T* p_val, DataString* p_cmd, size_t size) {
			data_size = size;
			p_value = p_val;
			p_command = p_cmd;
			return ERROR_SUCCESS;
		}

		inline errno_t Set(T* p_val, std::string* desc, size_t size) {
			data_size = size;
			p_value = p_val;
			p_desc = desc;
			return ERROR_SUCCESS;
		}

		
		inline bool GetValue(int idx) {
			bool ret = false;
			if (data_size > idx)
			{
				ret = (p_value[idx] == TRUE ? true : false);
			}
			return ret;
		}

		inline std::string GetComment(int idx) {
			std::string ret{};
			if (data_size > idx && p_command)
			{
				p_command->GetValue(idx + 1, ret);
			}
			return ret;
		}


		inline std::string to_string() {
			/*
			TASK1 : Prog= Line=0 State=0 ParentProg=
			*/
			std::ostringstream stringStream("");
			for (size_t i = 0; i < data_size; i++)
			{
				stringStream << "NO = " << i+1 << ",";
				stringStream << "STATUS = " << (*p_value?"ON":"OFF") << ",";
				stringStream << "COMMENT = " << p_command << "\r\n";
				p_command++;
				p_value++;
			}			


			return stringStream.str();
		}
	};


	struct sys_variable 
	{

		int FastClock{};
		int TimerValue{};
		float CurrentAngle{};
		float CurrentTemperature{};



		/*
		
		$FAST_CLOCK = 31269877
		$TIMER[10].$TIMER_VAL = 2
		$MOR_GRP[1].$CURRENT_ANG[1] = -2.915292
		$DUTY_TEMP = 851.000000
		$TIMER[10].$COMMENT = settingTest
		$TIMER[2].$COMMENT = 
		UF = -1, UT = -1
		XYZWPR = 829.596802 -92.712761 86.360733 -132.462784 -87.476433 132.928329 0.000000 0.000000 0.000000
		CONFIG = NRDB 0 0 0
		$[HTTPKCL]CMDS[1] = MOVE
		
		
		
		*/
	};





}