#pragma once
class CCommConfig
{
	private:
		unsigned char m_ucPort;
		short m_sSpeed;
		unsigned char m_ucDatabit;
		unsigned char m_ucStopbit;
		unsigned char m_ucParitybit;
		unsigned char m_ucDTRDSR;
		unsigned char m_ucRTSCTS;
		unsigned char m_ucXONXOFF;

	public:
		CCommConfig();
		~CCommConfig();

		void Set_Port( unsigned char ucPort );
		void Set_Port(CString str);
		unsigned char Get_Port();
		void Set_Speed(short sSpeed);
		short Get_Speed();
		void Set_Databit(unsigned char ucDatabit);
		unsigned char Get_Databit();
		void Set_Parity(unsigned char ucParity);
		unsigned char Get_Parity();
		void Set_DTRDSR(unsigned char ucDTRDSR);
		unsigned char Get_DTRDSR();
		void Set_RTSCTS(unsigned char ucRTSCTS);
		unsigned char Get_RTSCTS();
		void Set_XONXOFF(unsigned char ucXONXOFF);
		unsigned char Get_XONXOFF();

		void Set_Stop(unsigned char ucStop);
		unsigned char Get_Stop();

		CCommConfig& operator=(const CCommConfig& pCfg);


};

