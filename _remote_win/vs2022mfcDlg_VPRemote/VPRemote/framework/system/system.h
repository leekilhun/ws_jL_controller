#pragma once

/*
 * system.h
 * 
 * 작성자 : 이길훈
 * 22.04.04 
 * system header
 */

 /* 아래 순서대로 절차를 진행하며 가능한 interface를 상속한다 */
 /*
  system_def.h   <-   system.h
                         |
                      hal_xx.h
                         |
                      engine_xx.h
                         |
                      control_xx.h
                         |
                      ap_xx.h

 */



#include "system_def.h"

/* object*/
#include "system_obj.h"

/* data */

/* io*/

/* mechanical object*/

/* engine object*/

/* control object*/

/* ap object*/


namespace AP_SYS
{ 
    using namespace HAL;
    using namespace AP_OBJ;
    using namespace std;
    

    class MSystem
    {
        /****************************************************
         *	data
         ****************************************************/
        char m_ProjectDir[_MAX_DIR];

        MSystemObject* m_pObjinfo;
        IComm* m_pSerialComm;
        ModulePeeler* m_pModulePeeler;


    public:

        /****************************************************
         *	Constructor
         ****************************************************/
    public:
        MSystem();
        virtual ~MSystem();

        /****************************************************
         *	func
         ****************************************************/
    private:
        void init();
        void terminate();
        void destroy();
        void startThreads();
        void stopThreads();
        void getMcuData();

        errno_t _createSerialCommComponents();
        errno_t _createBasicHWComponents();


    public:
        errno_t Initialize();

        IComm* GetSerialCommComponent() const;

        ModulePeeler* GetModulePeelerComponent() const;

    };

}