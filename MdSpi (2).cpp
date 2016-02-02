							
					/************************************************************************/
					/*           ���ݽ��ճ����ļ�����Ҫ��������ȱʧ�������ظ�               */
					/************************************************************************/
#include "MdSpi.h"
#include "Custom_Methods.h"
using namespace std;
#pragma warning(disable : 4996)

/************************************************************************/
/*                             �Զ���ı���                             */
/************************************************************************/
//extern FILE * file;  
bool time_morning=true,time_afternoon=true,time_evening=true,initflag=true;			 //���ò����ı�־	 												 //����������һ��ʱ��λ
int timenow;                                                                         //��������ʱ����94112
extern CRITICAL_SECTION cs;                                                          //�ٽ���
extern CMdSpi_Custom * cmdSpi_custom;												 //����������
extern CThostFtdcDepthMarketDataField_Custom_Clean*cffex_state,*czce_state,*dce_state,*shfe_state;//�ĸ�����������
/************************************************************************/
/*                             ԭ��ϵͳ�������                         */
/************************************************************************/

extern CThostFtdcMdApi* pUserApi;													// USER_API����
// ���ò���
extern char FRONT_ADDR[];		
extern TThostFtdcBrokerIDType	BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType	PASSWORD;
extern char* ppInstrumentID[];	
extern int iInstrumentID;
extern int iRequestID;																// ������


/***********************************************************************************************************/
/*                         �̺߳���																		   */
/***********************************************************************************************************/ 	
DWORD WINAPI Function_Receive(LPVOID lpParamter)
{
	timenow = method_TimeToInt_Int();
	//��˯��һ��,����տ�ʼȡ�������ݡ�
	Sleep(1000);
	while(1)
	{
		
		EnterCriticalSection(&cs);
		//ȡ����,������������
		CThostFtdcDepthMarketDataField_Custom *data = cmdSpi_custom->methodGetDatas_Before();
		LeaveCriticalSection(&cs);
		//ϵͳʱ�䣬�Ժ������
		unsigned int ttt= GetTickCount();
		
		//ȡ��������                                             //ȡ250ms����Щ�����600,700ms�ģ�������500ms��û�취��
		if (data == NULL)
		{	
			if (ttt - (dce_state->firsttime) > TIMEOUT && !dce_state->flag_exchange)     
			{
				dce_state->methodIsMising();
		    }
 		   if (ttt - (shfe_state->firsttime) > TIMEOUT && !shfe_state->flag_exchange)
 			{
 				shfe_state->methodIsMising();
 			}
			 if (ttt - (czce_state->firsttime) > TIMEOUT && !czce_state->flag_exchange)
			{
				czce_state->methodIsMising();
			}
			 if (ttt - (cffex_state->firsttime) > TIMEOUT && !cffex_state->flag_exchange)
			{
				cffex_state->methodIsMising();
			}
			continue;
		}
		methodWriteFile(*data,"original");            //��ԭʼ������



		//������Ͼ���ʱ�ᷢ��һЩ9����֮ǰ�����ݣ���7������ݵȵȡ�
		//���˲���������
		if (filter_Unreasonable_Price(data->UpdateTime))
		{
			continue;
		}	

		//�ҵ�ÿһ�ֺ�Լ���Ӧ�Ľ�����
		string s = cmdSpi_custom->exchange_map[data->InstrumentID];

		//ԭ������û�н�����ID�����ڼ���  20150921  tangb
		sprintf(data->ExchangeID ,"%s" , s.c_str());

		#pragma region ���ֽ�����

		//ȡ������
		if (s=="dce")
		{
		
			dce_state->methodMoreThan200(data);	
		}	
		else if(s=="shfe")
		{

			shfe_state->methodMoreThan200(data);	
		}
		else if(s=="czce")
		{
			
			czce_state->methodMoreThan200(data);	
		}
		else if(s=="cffex")
		{
		
			cffex_state->methodMoreThan200(data);	
		}
		#pragma endregion ���ֽ�����

	}//end while

}//end FUN



void method_AddContracts(CThostFtdcDepthMarketDataField_Custom_Clean_Contract * ccc){
	#pragma region ����ʱ���fronttime_1��fronttime_2����

	//����״̬
	ccc->data = datatemp;
	//�޸�������ʱ�䣬��������
	ccc->end_writetime_Millisec = datatemp->UpdateMillisec;
	ccc->flag_isfirst = true;


	int time = method_TimeToInt_Int();
	if (time> CLOSE_EVENING + TIME_DIF &&time< OPEN_MORNING)             //24500 ~ 90000
	{	if (((string)ccc->data->InstrumentID).substr(0,2)=="IF")
		{
			ccc->fronttime_1 = OPEN_MORNING+TIME_DIF;         
			ccc->fronttime_2 = OPEN_MORNING+TIME_DIF;      
		}
		else{
			ccc->fronttime_1 = OPEN_MORNING;         
			ccc->fronttime_2 = OPEN_MORNING;
		}
	}else if (time > CLOSE_NOON + TIME_DIF &&time< OPEN_NOON)				//114500 ~ 130000		
	{
		if (((string)ccc->data->InstrumentID).substr(0,2)=="IF")
		{
			ccc->fronttime_1 = OPEN_NOON;
			ccc->fronttime_2 = OPEN_NOON;
		}else
		{
			ccc->fronttime_1 = OPEN_NOON+2*TIME_DIF;
			ccc->fronttime_2 = OPEN_NOON+2*TIME_DIF;
		}
		
		
	}else if (time>CLOSE_NOON + 2 * TIME_DIF &&time < OPEN_EVENING)		   //153000 ~ 210000	
	{
		ccc->fronttime_1 = OPEN_EVENING;
		ccc->fronttime_2 = OPEN_EVENING;	
		
	}else{                                                                //�ڿ��̺�������
		ccc->fronttime_1 = ccc->data->UpdateTime;
		ccc->fronttime_2 = ccc->data->UpdateTime;
	}
	//�������һ�����ݵ�ʱ��
	//(this->lasttime) = ccc->data->ReceiveTime;              
	//д����Ӧ��map��ȥ
	//(*(totalTable))[ccc->data->InstrumentID] = ccc;
	#pragma endregion ����ʱ���fronttime_1��fronttime_2����	
	
}
/*************************************************************************************************************/
/*                            ���������еĺ�������															 */
/*************************************************************************************************************/

#pragma region �������� 
/************************************************************************/
/*                             ��ȡ�������Ҵ���200msʱ                  */
/************************************************************************/
//��ȡ�������Ҵ���200msʱ������Լ�ı�־��λfalse��
void CThostFtdcDepthMarketDataField_Custom_Clean::methodMoreThan200(CThostFtdcDepthMarketDataField_Custom *datatemp){

	

	//��Լ�ǵ�һ�Σ�����map�л�û�иú�Լ
	if (this->totalTable->find(datatemp->InstrumentID) == this->totalTable->end())
	{
		CThostFtdcDepthMarketDataField_Custom_Clean_Contract * ccc =
			new CThostFtdcDepthMarketDataField_Custom_Clean_Contract();		
		
		method_AddContracts(ccc);
		//д����Ӧ��map��ȥ
		(*(totalTable))[ccc->data->InstrumentID] = ccc;
	}


	
	    #pragma region ��һ���ĵ�һ������
	
	//���map���Ѿ����˺�Լ�����ж��ǲ�����һ���ĵ�һ������
	//˵���Ǹý���������һ���ĵ�һ������
		if ((datatemp->ReceiveTime) - (this->lasttime)> TIMEOUT){
		//this->lasttimeΪĳһ����������һ�����ݵ�����޸�ʱ�䣬�����洦������methodIsRepetition�������޸�

		//�Ǻ��뼶GetTickCount()
		(this->firsttime) = datatemp->ReceiveTime;
		//�ı���µ�״̬
		map<string,CThostFtdcDepthMarketDataField_Custom_Clean_Contract*>::iterator iter;
		for (iter = this->totalTable->begin();iter != this->totalTable->end();iter++)
		{
			//��Լ��־
			(iter->second->flag_contract) = false;
		}
		//��������־
	
		(this->flag_exchange) = false;
	}
	#pragma endregion ��һ���ĵ�һ������
	

	//��������
	this->methodIsRepetition(datatemp);
	
};
/************************************************************************/
/*                            ���������ظ�                              */
/************************************************************************/
void CThostFtdcDepthMarketDataField_Custom_Clean::methodIsRepetition(CThostFtdcDepthMarketDataField_Custom *datatemp){
	//��Լ���ǵ�һ�Σ��ʹ���Ӧ��map��ȡ��
	CThostFtdcDepthMarketDataField_Custom_Clean_Contract *ccc = (*(this->totalTable))[datatemp->InstrumentID];
    
	//�����Լû�����¹�
		if (!((ccc->flag_contract)))
		{

			*(ccc->data) = *datatemp;
			//�����ڽ���ʱ�俪����ʱû����ֵ�����,����93024������                   //20151009    tangbao
			//Ҫ������235959�����Բ���(ccc->fronttime_2==0||ccc->fronttime_2==0)&&datatemp->UpdateTime!=0
			//����Ϊ||����Ϊ��methodIsMising�״α��޸��ˣ�������235959ʱ������
			//if ((ccc->fronttime_2==0||ccc->fronttime_2==0)&&datatemp->UpdateTime!=0)
			if (ccc->is_initflag)
			{
				ccc->fronttime_1= datatemp->UpdateTime;
				ccc->fronttime_2= datatemp->UpdateTime;
				ccc->is_initflag= false;
			}


			if (ccc->fronttime_1 == ccc->fronttime_2)
			{
			  ccc->fronttime_1++;
			  ccc->data->UpdateTime = ccc->fronttime_1;
			}else
			{
				ccc->data->UpdateTime = ccc->fronttime_1;
				ccc->fronttime_2 =  ccc->fronttime_1;
			}
			//�޸�Ϊ��׼ʱ��  ����102460 -> 102500       235959+1 -> 0
			ccc->data->UpdateTime = method_Template_TimeInt(ccc->data->UpdateTime);
			ccc->fronttime_2 =   method_Template_TimeInt(ccc->fronttime_2);
			ccc->fronttime_1 =   method_Template_TimeInt(ccc->fronttime_1);
	
			//�޸�0����500ms,100ms����400ms
			ccc->end_writetime_Millisec = (ccc->end_writetime_Millisec+500)%1000;
			ccc->data->UpdateMillisec = ccc->end_writetime_Millisec;


			//�ŵ���������Ļ�������ȥ
			cmdSpi_custom->methodAddDatas_After(*(ccc->data));
			//д���ļ�
			methodWriteFile(*(ccc->data),"tick");

			#pragma region  ���������ӳ�ʱ��,Ʒ������ʱ��֮��û����

			/************************************************************************/
			/* ���ѭ����Ҫ������Ծ��Լ�Ĳ��������⣬��Ҫ�ǽ��������ӳ�ʱ����ɵ�,*/
			/*��132717Ϊ��һ�ʣ�132719Ϊ��һ�ʣ��м�û��132718���͵ò�ȫ��,��ָҲ��
			/*�����������*/
			/************************************************************************/
			//��Ʒ�ڻ�������15���Ӻ�30������Ϣ�����������ʱ�䲻������   
			//��101500 ~ 103000						  20151012  tangb
			if (
				datatemp->UpdateTime - ccc->data->UpdateTime >1000
				/*&&
				((datatemp->UpdateTime>101500 && datatemp->UpdateTime<103100)
				||
				(datatemp->UpdateTime>130000 && datatemp->UpdateTime<133100))*/
				)  
			{
				ccc->fronttime_1=datatemp->UpdateTime;
				ccc->fronttime_2=datatemp->UpdateTime;
				ccc->data->UpdateTime = datatemp->UpdateTime;		 //��������޸�ʱ��
				
			}

			//������������޸�ʱ�������ǰ��������޸�ʱ��,�������
			while(datatemp->UpdateTime > ccc->data->UpdateTime)
			{
				if (ccc->fronttime_1 == ccc->fronttime_2)
				{ 
					ccc->fronttime_1++;
				    ccc->data->UpdateTime = ccc->fronttime_1;
				}else
				{
					ccc->data->UpdateTime = ccc->fronttime_1;
					ccc->fronttime_2 =  ccc->fronttime_1;
				}
				ccc->data->UpdateTime = method_Template_TimeInt(ccc->data->UpdateTime);
				ccc->fronttime_2 =   method_Template_TimeInt(ccc->fronttime_2);
				ccc->fronttime_1 =   method_Template_TimeInt(ccc->fronttime_1);

				//�޸�0����500ms,100ms����400ms
				ccc->end_writetime_Millisec = (ccc->end_writetime_Millisec+500)%1000;
				ccc->data->UpdateMillisec = ccc->end_writetime_Millisec;

				//�ŵ���������Ļ�������ȥ
				cmdSpi_custom->methodAddDatas_After(*(ccc->data));
				//д���ļ�
				CThostFtdcDepthMarketDataField_Custom ttemmmm = *(ccc->data);
				char *ss="��2";
				strcat_s(ttemmmm.ExchangeID,sizeof(ttemmmm.ExchangeID),ss);
				methodWriteFile(ttemmmm,"tick");
			}
			#pragma endregion ���������ӳ�ʱ��

			//�Ѹ���
			(ccc->flag_contract) = true;
			//�������һ�����ݵ�ʱ�䣬�Ǻ��뼶GetTickCount()
			(this->lasttime) = datatemp->ReceiveTime;
			//����map
			(*(this->totalTable))[datatemp->InstrumentID] = ccc;

		}
		else
		{
			//�����Լ�Ѿ������¹�

		}
};
/************************************************************************/
/*                            �ֲ�����								    */
/************************************************************************/
void CThostFtdcDepthMarketDataField_Custom_Clean::methodIsMising(/*int timenow*/)
{
	
#pragma region ѭ��������

	//���������û�����¹�
	if (!((this->flag_exchange)))
	{
    	map<string,CThostFtdcDepthMarketDataField_Custom_Clean_Contract *>::iterator iter;
		for (iter = this->totalTable->begin();iter!=this->totalTable->end();iter++)
		{
			string contract = contract;
			CThostFtdcDepthMarketDataField_Custom_Clean_Contract* cData = iter->second;

			//�����Լû�޸Ĺ�
			if (!cData->flag_contract)                             //iter->second->flag_contract)
			{
				//����������ʱ��ͺ�����
				if (cData->fronttime_1 == cData->fronttime_2)
				{
					cData->data->UpdateTime = cData->fronttime_1 + 1;
					cData->fronttime_2 = cData->fronttime_1;
					cData->fronttime_1 = cData->fronttime_1 + 1;
				}else
				{
					cData->data->UpdateTime = cData->fronttime_1;//timenow;
					cData->fronttime_2 =  cData->fronttime_1;
				}
				cData->data->UpdateTime = method_Template_TimeInt(cData->data->UpdateTime);
				cData->fronttime_2 = method_Template_TimeInt(cData->fronttime_2);
				cData->fronttime_1 = method_Template_TimeInt(cData->fronttime_1);

				//�޸�0����500ms,100ms����400ms
			    cData->end_writetime_Millisec= (cData->end_writetime_Millisec+500)%1000;
				cData->data->UpdateMillisec =cData->end_writetime_Millisec;

				(cData->flag_contract) = true;    
		
				//�ŵ����������Ļ�������ȥ
				cmdSpi_custom->methodAddDatas_After(*(cData->data));
				//����map
				(*(this->totalTable))[contract] = cData;
				//д���ļ�
				CThostFtdcDepthMarketDataField_Custom ttemmmm = *(cData->data);
				char *ss="��1";
				strcat_s(ttemmmm.ExchangeID,sizeof(ttemmmm.ExchangeID),ss);
				methodWriteFile(ttemmmm,"tick");//*(iter->second->data)
			}

		}
		//�������Ѿ������¹�
		(this->flag_exchange) = true;	
	}
	#pragma endregion ѭ��������

};

#pragma endregion �������� 

#pragma region ��ӳ���
/************************************************************************/                       							 
/*							1)�������ǰ����							*/
/************************************************************************/
void CMdSpi_Custom::methodAddDatas_Before(CThostFtdcDepthMarketDataField* data,unsigned int receivetime){
	//CThostFtdcDepthMarketDataField  ->  CThostFtdcDepthMarketDataField_Custom	
	CThostFtdcDepthMarketDataField_Custom *cdata = cmdSpi_custom->methodChangeToCustom(data,receivetime);
	cmdSpi_custom->before_data_buffer.push(cdata);
};
/************************************************************************/                       							 
/*							2)ȡ����ǰ����							    */
/************************************************************************/
CThostFtdcDepthMarketDataField_Custom* CMdSpi_Custom::methodGetDatas_Before(){
	try
	{
		CThostFtdcDepthMarketDataField_Custom* da = NULL;

		if (cmdSpi_custom->before_data_buffer.size())
		{
			da= cmdSpi_custom->before_data_buffer.front();
			cmdSpi_custom->before_data_buffer.pop();
		}

		return da;
	}
	catch (std::exception& e)
	{
		std::cerr <<"exception caught:methodGetDatas_Before "<< e.what() <<'\n';
	}	

};
/************************************************************************/                       							 
/*							3)������������							 */
/************************************************************************/
void CMdSpi_Custom::methodAddDatas_After(CThostFtdcDepthMarketDataField_Custom data){

	EnterCriticalSection(&cs);
	cmdSpi_custom->after_data_buffer.push(data);
	//20150824 tangb  ����д���ݿ�Ķ���
	//cmdSpi_custom->data_to_database.push(data);
	LeaveCriticalSection(&cs);

};
/************************************************************************/                       							 
/*							4)ȡ���������								*/
/************************************************************************/
CThostFtdcDepthMarketDataField_Custom* CMdSpi_Custom::methodGetDatas_After(){
	try
	{
		CThostFtdcDepthMarketDataField_Custom* da=NULL;	
		if (!(cmdSpi_custom->after_data_buffer.empty()))
		{
			da= &(cmdSpi_custom->after_data_buffer.front());
			cmdSpi_custom->after_data_buffer.pop();
		}

		return da;
	}
	catch (std::exception& e)
	{
		std::cerr <<"exception caught:methodGetDatas_After "<< e.what() <<'\n';
	}	

};

#pragma endregion ��ӳ���

/************************************************************************/                       							 
/*							ת������							     	 */
/************************************************************************/
CThostFtdcDepthMarketDataField_Custom* CMdSpi_Custom::methodChangeToCustom(
	CThostFtdcDepthMarketDataField *data,unsigned int receivetime)
{

	//CThostFtdcDepthMarketDataField  ->  CThostFtdcDepthMarketDataField_Custom	
	try
	{
		CThostFtdcDepthMarketDataField_Custom *custom=new CThostFtdcDepthMarketDataField_Custom();
		custom->TradingDay= method_TimeToInt(data->TradingDay);
		// 	strcpy_s(custom->InstrumentID,sizeof(custom->InstrumentID),data->InstrumentID);
		// 	strcpy_s(custom->ExchangeID,sizeof(custom->ExchangeID),data->ExchangeID);
		//strncpy_s(b,_countof(b),a,strlen(a));

		strncpy_s(custom->InstrumentID,_countof(custom->InstrumentID),data->InstrumentID,strlen(data->InstrumentID));
		strncpy_s(custom->ExchangeID,_countof(custom->ExchangeID),data->ExchangeID,strlen(data->ExchangeID));
		custom->LastPrice=data->LastPrice;
		custom->PreClosePrice=data->PreClosePrice;
		custom->PreOpenInterest=data->PreOpenInterest;
		custom->Volume=data->Volume;
		custom->Turnover=data->Turnover;
		custom->OpenPrice=data->OpenPrice;
		custom->ClosePrice=data->ClosePrice;
		custom->OpenInterest=data->OpenInterest;
		custom->UpperLimitPrice=data->UpperLimitPrice;
		custom->LowerLimitPrice=data->LowerLimitPrice;
		custom->PreDelta=data->PreDelta;
		custom->CurrDelta=data->CurrDelta;
		custom->UpdateTime=method_TimeToInt(data->UpdateTime);
		custom->UpdateMillisec=data->UpdateMillisec;
		custom->BidPrice1=data->BidPrice1;
		custom->BidVolume1=data->BidVolume1;
		custom->AskPrice1=data->AskPrice1;
		custom->AskVolume1=data->AskVolume1;
		custom->AveragePrice=data->AveragePrice;
		custom->OpenInterestRange = 0;
		custom->ReceiveTime = receivetime;
		return custom; 
	}
	catch (std::exception& e)
	{
		std::cerr <<"exception caught:methodChangeToCustom "<< e.what() <<'\n';
	}	
};




/************************************************************************************************************/
/*                 ԭ��CTPϵͳ�������ݺ�������																*/
/************************************************************************************************************/
#pragma region ԭ��CTPϵͳ�������ݺ�������

void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< "OnRspError" << endl;
	IsErrorRspInfo(pRspInfo); 
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << "OnFrontDisconnected" << endl;
	cerr << "--->>> Reason = " << nReason << endl;
}

void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << "OnHeartBeatWarning" << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CMdSpi::OnFrontConnected()
{
	cerr << "--->>> " << "OnFrontConnected" << endl;
	///�û���¼����
	ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	cerr << "--->>> �����û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///��ȡ��ǰ������
		cerr << "--->>> ��ȡ��ǰ������ = " << pUserApi->GetTradingDay() << endl;
		// ����������
		SubscribeMarketData();	
	}
}

void CMdSpi::SubscribeMarketData()
{
	int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
	cerr << "--->>> �������鶩������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr <<pSpecificInstrument->InstrumentID<< endl;
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "OnRspUnSubMarketData" << endl;
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{	

	EnterCriticalSection(&cs);
	//��¼�������ݵ�ʱ��
	unsigned int receivetime = GetTickCount();
	//CThostFtdcDepthMarketDataField  ->  CThostFtdcDepthMarketDataField_Custom	
	cmdSpi_custom->methodAddDatas_Before(pDepthMarketData,receivetime);

	LeaveCriticalSection(&cs);
	//	cerr <<pDepthMarketData->InstrumentID<<"-- "<<pDepthMarketData->UpdateTime<<" =="<<pDepthMarketData->LastPrice<<endl;
	// 	if (((string)pDepthMarketData->InstrumentID).substr(0,2)=="IF")
	// 	{
	// 	
	// 	cerr << pDepthMarketData->InstrumentID<<" "<<pDepthMarketData->UpdateTime<<" "<<pDepthMarketData->LastPrice<<" "<</*pDepthMarketData->ReceiveTime<<" "<<*/endl;
	// 	}
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

#pragma endregion ԭ��CTPϵͳ�������ݺ�������

