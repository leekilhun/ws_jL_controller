#pragma once

#define IAXIS_DIR_CW				(int)1
#define IAXIS_DIR_CCW				(int)0

//motion�� ���� �⺻ parameter





class IAxis :public virtual Icommon
{

public:

	struct motion_param_t {
		uint32_t jog_speed{}; //���� mmsec
		uint32_t jog_accel{}; //���� mmsec/ms ��ǥ �ӵ��� ������ �ð�ms
		uint32_t jog_decel{}; //����  mmsec/ms 

		uint32_t move_velocity{}; //���� mmsec  
		uint32_t move_accel{}; //���� mmsec/ms  ��ǥ �ӵ��� ������ �ð�ms 
		uint32_t move_decel{}; //���� mmsec/ms

		uint32_t limit_velocity{}; //�Ѱ� �ӵ� mmsec [max_rps * one_turn_move_mm]

		double max_rps{}; // �ʴ� �ִ� ȸ���� (3'000 RPM ==  50rps)
		uint32_t turn_pulse_count{}; // �� ���� �߻� ���ڴ� ī����
		double one_turn_move_mm{}; // �� ���� ���� �� �̵��Ǵ� ���� �Ÿ� mm  (cmd_pos/turn_per_move)*turn_per_pulse

		//double motor_scale_mm_pulse_cnt{}; // 1mm �̵��� �ʿ��� pulse count
		//double mm_per_sec{}; //�ʴ� �̵� mm �Ÿ�

		/*
		(pulse / mm)(cmd_vel * (1/60))*turn_per_pulse
		
		move(vel = 1000, acc = 100, dec = 100) ������ 100ms�� ��ǥ �ӵ� ����, 
		                                       ��ǥ �ӵ��� �ʴ� 1000mm ������ rps �䱸
											   a [��ǥ�Ÿ�mm/one_turn_move_mm]
											   �Ѱ� 500mmsec ����

											   ��ǥ �޽�cnt b ������ [a * turn_pulse_count] �� ���� �Ѵ�  


		���͸� ����
		�ӵ� rpm���� ���� (�д� ȸ�� ��)
		������ rpm/s �ʴ�

		 1ȸ�� ī��Ʈ ���̰� �ʴ�
		 EX) ���ڴ� or Ȧ���� : 1ȸ�� �޽����� 300���̰� 1�ʴ� ȸ���� �Է� �޽����� 200�� �϶� RPM ��� : 200 / 300 = 0.66 (�ʴ�) RPM = 0.66 X 60 (��) = 39.6 RPM (�д�) �Ÿ� ��� : ���� ��

		 �⺻�ӵ� 10 rps 

		*/

		void Init() {
			constexpr double default_rps = 10.0;
			jog_speed = (uint32_t)(one_turn_move_mm * default_rps);
			jog_accel = 100;
			jog_decel = 100;

			move_velocity = (uint32_t)(one_turn_move_mm * default_rps * 3.0);
			move_accel = 100;
			move_decel = 100;

			limit_velocity = (uint32_t)(one_turn_move_mm * max_rps);
		}


		inline void operator = (const motion_param_t* p_cfg) {
			this->jog_speed = p_cfg->jog_speed;
			this->jog_accel = p_cfg->jog_accel;
			this->jog_decel = p_cfg->jog_decel;
			this->move_velocity = p_cfg->move_velocity;
			this->move_accel = p_cfg->move_accel;
			this->move_decel = p_cfg->move_decel;
			this->limit_velocity = p_cfg->limit_velocity;

			this->turn_pulse_count = p_cfg->turn_pulse_count;
			this->max_rps = p_cfg->max_rps;
			this->one_turn_move_mm = p_cfg->one_turn_move_mm;
		}


	};


#if 0
	struct origin_param_t {
		int orgSpeed{};
		int orgSearchSpeed{};
		int orgAccDecTime{};
		int orgMethod{};	// Origin Method = 2 is 'Limit Origin' in the Ezi-SERVOII Plus-E model.
		int orgDir{};
		int orgOffset{};
		int orgPositionSet{};
		int orgTorqueRatio{};

		origin_param_t() {
			orgSpeed = 50000;
			orgSearchSpeed = 1000;
			orgAccDecTime = 50;
			orgMethod = 2;	// Origin Method =
			orgDir = IAXIS_DIR_CW;
			orgOffset = 0;
			orgPositionSet = 0;
			orgTorqueRatio = 50;
		}

		inline void operator = (const origin_param_t* p_cfg) {
			this->orgSpeed = p_cfg->orgSpeed;
			this->orgSearchSpeed = p_cfg->orgSearchSpeed;
			this->orgAccDecTime = p_cfg->orgAccDecTime;
			this->orgMethod = p_cfg->orgMethod;
			this->orgDir = p_cfg->orgDir;
			this->orgOffset = p_cfg->orgOffset;
			this->orgPositionSet = p_cfg->orgPositionSet;
			this->orgTorqueRatio = p_cfg->orgTorqueRatio;
		}

	};

#endif


	virtual errno_t Initialize() = 0;
	virtual bool Disconnect() = 0;
	virtual errno_t MotorOnOff(bool on_off) = 0;
	virtual bool  IsMotorOn() = 0;
	virtual errno_t WaitDone(uint32_t timeout) = 0;
	virtual bool IsAxisDone() = 0;
	virtual errno_t  ClearState() = 0;
	virtual uint32_t  GetMotorState() = 0;
	virtual errno_t SetMotionData(motion_param_t &param) = 0;

	virtual errno_t OriginAxis() = 0;
	virtual errno_t ClearPosition() = 0;

	virtual errno_t Move(int cmd_pos, uint32_t cmd_vel = 0, uint32_t acc = 100, uint32_t decel = 100) = 0;
	//���� �����Ѵ�. (�Ѱ��� �࿡ ���� ����)
	virtual errno_t Stop() = 0;
	//���� ����̵��� ���� �����Ѵ�.
	virtual errno_t VStop() = 0;
	// ���� ��������Ѵ�.
	virtual errno_t EStop() = 0;

	virtual errno_t JogStop() = 0;

	virtual errno_t JogMove(uint32_t cmd_vel = 0, bool is_cw = true) = 0;
	//virtual errno_t JogMove(uint32_t jog_pos, uint32_t cmd_vel = 1000, bool is_cw = true) = 0;

	virtual errno_t GetActualPosition(long& p_position) = 0;
	virtual errno_t GetCommandPosition(long& p_position) = 0;

};






/*

motor_scale : 1 mm �̵��� �ʿ��� pulse��
( 1 turn pulse )/ (1 ���� �̵� �Ÿ� mm)    ��)  1 turn 20'000�޽�  1���� �̵� �Ÿ� 10mm  �� 20'000/10mm   1mm �̵��� �ʿ��� �޽��� 2'000�޽�

turn_scale : �ѹ��� ������ �̵��Ǵ� �Ÿ� mm

motor �ӵ� (m/sec)  �̵��Ÿ�/sec
case 1.  1�ʿ� 1m �̵� �ӵ��� ������ motor_scale(2'000) * 1'000 / 1�ʷ�  1�ʿ� 2'000'000�޽��� ������ �ӵ��� �ʿ�.
case 2.  turn_scale * (50 rps) 1�ʿ� �ִ� �ӵ�


rpm 3'000 ����  :  �� ���� �޽� 20'000   �ʴ� 50 ����
param.max_velocity = case 1. (���� �̵��Ÿ� [50���� * 20'000�޽�] / motor scale(�޽�))
					 case 2.  �ִ� �ӵ� 1�ʿ� �̵��ϴ� �Ÿ� ((50���� * 20'000�޽� ) / motor_scale)/sec

param.jog_speed = 5000; //pps
param.jog_accel = 100; //msec
param.jog_decel = 100; //msec
param.move_velocity = 10000; //pulse
param.move_accel = 100;//msec
param.move_decel = 100;//msec


*/