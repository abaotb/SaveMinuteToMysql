// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "MdSpi.h"
#include "OperatorData.h"
#include "Custom_Methods.h"
using namespace std;


/************************************************************************/
/*                             �Զ���ı���                             */
/************************************************************************/

//FILE * file;
HANDLE hMutex_Receive,hMutex_Classify;															 //���߳�
CRITICAL_SECTION cs;															 //�ٽ���
CMdSpi_Custom * cmdSpi_custom;													 //����������
CThostFtdcDepthMarketDataField_Custom_Clean*cffex_state,*czce_state,*dce_state,*shfe_state;//�ĸ�����������
DWORD WINAPI Function_Receive(LPVOID lpParamter);                                             //���̺߳���
DWORD WINAPI Classify_Data(LPVOID lpParamter);
DWORD WINAPI Classify_Data2(LPVOID lpParamter);
All_Contracts_Array all_contracts_array;									     //�������к�Լ������
TSMD_Map* tsmd_map;																 //Tick,5Seconds,One_minute,One_Day�����
/************************************************************************/
/*                             ԭ��ϵͳ�������                         */
/************************************************************************/

CThostFtdcMdApi* pUserApi;														// UserApi����							 
// ���ò���
char FRONT_ADDR[] = "tcp://180.169.124.125:41213";								// ǰ�õ�ַ
TThostFtdcBrokerIDType	BROKER_ID = "2030";										// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "00092";									// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "888888";								    // �û�����
char *ppInstrumentID[CONTRACTCOUNT];//={"IF1407","SR407","SR409","TA409"};	// ���鶩���б�
int iInstrumentID = CONTRACTCOUNT;												// ���鶩������
int SUM_CONTRACTS;
int iRequestID = 0;																// ������

/************************************************************************/
/*                                ���������                            */
/************************************************************************/
void main(void)
{

	//�Զ����ɺ�Լ���������
	//void methodAutoContract();
	//�����ռ���������   
//  readdata1();
//  readdata2(); 
// 	readdata3();

	tsmd_map = new TSMD_Map();
  	cmdSpi_custom = new CMdSpi_Custom();										//�����������
	cffex_state=new CThostFtdcDepthMarketDataField_Custom_Clean();			    //�ĸ������������
	dce_state = new CThostFtdcDepthMarketDataField_Custom_Clean();
	czce_state= new CThostFtdcDepthMarketDataField_Custom_Clean();
	shfe_state= new CThostFtdcDepthMarketDataField_Custom_Clean();

	//�Զ����ɺ�Լ�����
	//methodAutoContract();
  	//methodAutoContract2();
	  methodAutoContract3();
	//	cout<<sizeof(CThostFtdcDepthMarketDataField_Custom_Other)<<" "<<sizeof(TSMD_Arrays);
	if (cmdSpi_custom->exchange_map.size()==0)
	{
		cout<<"nULL"<<endl;
	}
    for (int i=0;i<cmdSpi_custom->exchange_map.size()/*SUM_CONTRACTS*/;i++)
    {
   		
      	cout<<ppInstrumentID[i]<<endl;
  	}
	cout<<"----------------------"<<endl;
	//��ʼ���ٽ���
	InitializeCriticalSection(&cs);
	// ��ʼ��UserApi
	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();								// ����UserApi
	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
	pUserApi->RegisterSpi(pUserSpi);											// ע���¼���
	pUserApi->RegisterFront(FRONT_ADDR);										// connect
	pUserApi->Init();   
	//�����Զ����߳�
	hMutex_Receive = CreateThread(NULL, 0, Function_Receive, NULL, 0, NULL);
	//hMutex_Classify = CreateThread(NULL,0,Classify_Data,NULL,0,NULL);
	hMutex_Classify = CreateThread(NULL,0,Classify_Data2,NULL,0,NULL);   //���෽ʽ����
	
	
	pUserApi->Join();
	
	//pUserApi->Release();
}
