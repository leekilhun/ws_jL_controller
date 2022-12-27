
#ifndef IMOTIONLIB_H
#define IMOTIONLIB_H

#include "Icommon.h"

//#include "DefMotionStruct.h"

  /** This class is defined Motion component interface class of Motion libraries as a abstract class.
   * �� Class�� Motion Library�� ���� Class�� ���� Interface Class�� �����Ѵ�.
   * 
   * @interface
   */
class IMotionLib : public virtual Icommon
{

public:
	virtual ~IMotionLib() {};

	virtual  errno_t Initialize() = 0;


	/// <summary>
	/// ���� ���� �Ϸ�� ������ ����Ѵ�.
	/// �̵� �� �ӵ� �̵� �Ϸ� Ȯ�� �Ǵ� ���Ҿ� In - Position ���α��� Ȯ���Ѵ�.
	/// </summary>
	/// <param name="axis_id">�� ID </param>
	/// <returns>0=SUCCESS , �׿� ����</returns>
	virtual errno_t WaitDone(short axis_id, unsigned short timeout) = 0;

	virtual errno_t MotorOnOff(short axis_id, bool on_off) = 0;

	virtual bool IsMotorOn(short axis_id) = 0;

	/// <summary>
	/// ���� ���� �Ϸ� ���θ� Return
	/// </summary>
	/// <param name="axis_id">�� ID </param>
	/// <returns>0=SUCCESS , �׿� ����</returns>
	virtual bool IsAxisDone(short axis_id) = 0;

	/// <summary>
	/// �� ���� ���¸� �д´�.
	/// ���´� bit �������� �����Ǿ� �ִ�.
	/// </summary>
	/// <param name="axis_id">�� ID</param>
	/// <param name="p_state">���� ��ȯ ����</param>
	/// <returns>0=SUCCESS , �׿� ����</returns>
	virtual errno_t GetAxisStatus(short axis_id, DWORD& p_state) = 0;


	/// <summary>
	/// ��ǥ	(Command)��ġ���� ����(Actual) ��ġ���� 0 ����
	/// </summary>
	/// <param name="axis_id">�� ID</param>
	/// <returns>0=SUCCESS , �׿� ����</returns>
	virtual errno_t ClearPosition(short axis_id) = 0;

	virtual errno_t AlarmReset(short axis_id) = 0;

	virtual errno_t GetAlarmType(short axis_id, DWORD& p_state) = 0;

	virtual errno_t GetCommandPosition(short axis_id, long& p_position) = 0;

	virtual errno_t GetActualPosition(short axis_id, long& p_position) = 0;
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="axis_id"></param>
	/// <param name="velocity"></param>
	/// <param name="is_cw"></param>
	/// <returns></returns>
	virtual errno_t JogMove(short axis_id, DWORD velocity, bool is_cw) = 0;

	virtual errno_t JogStop(short axis_id) = 0;

	virtual errno_t MoveStop(short axis_id) = 0;

	virtual errno_t Move(short axis_id, long position, DWORD velocity, bool wait_done = false) = 0;

	virtual errno_t OriginAxis(short axis_id) = 0;
	

};

#endif //IMOTIONLIB_H

// End of File IMotionLib.h