					/************************************************************************/
					/*  ���ݴ�������ļ�����Ҫ��MdSpi.cpp�ļ������ݴ����5�룬1���ӵ�����   */
					/************************************************************************/
#include "MdSpi.h"
#include "OperatorData.h"
#include "Custom_Methods.h"
#include "DataBaseDao.h"
using namespace std;


// USER_API����
extern CThostFtdcMdApi* pUserApi;
//���߳�
//extern FILE * file;
bool is_afternoon=true,is_evening=true;																 //����������һ��ʱ��λ
extern CRITICAL_SECTION cs;
extern All_Contracts_Array all_contracts_array;                                //�������к�Լ������
extern TSMD_Map* tsmd_map;                                                      //Tick,5Seconds,One_minute,One_Day�����
CThostFtdcDepthMarketDataField_Custom data_static;                                     //ȡ��������,�ĵ�ȫ�ֱ���
extern CMdSpi_Custom * cmdSpi_custom;	

#pragma region ��ʼ�������ݿ�

#pragma region ��ʼ��

//ҹ������
void methodYepan(int yepan_time) 
{
	if (yepan_time == 0)                        //ҹ��0��
	{
		map<string,TSMD_Arrays*>::iterator iter;
		for (iter=tsmd_map->tsmd_a->begin();iter!=tsmd_map->tsmd_a->end();iter++)
		{
			iter->second->time_fivesecond = 0;   //ҹ��0��
			iter->second->time_oneminute = 0;
			iter->second->time_oneday = 0;
		}
	}
}
#pragma endregion ��ʼ��



	#pragma region  ADO ����mysql��ʽ2
//
////�������ݿ�
//void Ado_Connect_Mysql(CThostFtdcDepthMarketDataField_Custom_Other ctd_data)
//{
//#pragma region �������ݿ�
//	//////////////////////////////////////////////////////////////////////////�������ݿ�
//	CoInitialize(NULL); 
//	_ConnectionPtr m_pConnection=NULL;
//	//����Connection����
//	m_pConnection.CreateInstance("ADODB.Connection");//(__uuidof(Connection));   
//	try 
//	{  
//		//��������ʱ��  
//		m_pConnection->ConnectionTimeout = 5;
//		_bstr_t connstr = "DSN=MySqlODBC;server=";
//		connstr += local_host ;
//		connstr += ";database=";
//		connstr += data_base;;
//		//�����ݿ�����  
//		HRESULT hr = m_pConnection->Open(connstr,user_name,user_pwd,adModeUnknown); //adModeUnknown ȱʡ����ǰ�����Ȩδ����
//		//m_pConnection->Open("Data Source=adotest;UID=sa;PWD=;","","",adModeUnknown);
//	}  
//	catch(_com_error &e)  
//	{  
//		cout<<"���Ӵ���"<<endl;
//		return ;  
//	}   
//	//////////////////////////////////////////////////////////////////////////�������ݿ�
//
//
//#pragma endregion �������ݿ�
//
//#pragma region ִ��SQL���
//	//////////////////////////////////////////////////////////////////////////�������
//	//ִ��SELETE���
//	_RecordsetPtr m_pRecordset;
//	//m_pRecordset.CreateInstance("ADODB.Recordset");
//	m_pRecordset.CreateInstance(__uuidof( Recordset )); //����ʵ��
//	//һ��Ҫ��
//	_bstr_t openRecordset="SELECT  * FROM ";
//	openRecordset+= table;
//	openRecordset +=" limit 1";
//	m_pRecordset->Open(openRecordset,m_pConnection.GetInterfacePtr
//		(),adOpenStatic,adLockOptimistic,adCmdText);// ִ��SQL��䣬�õ���¼��, m_pConnection������֪
//	try{
//		if (m_pRecordset!=NULL)
//		{
//			if (ctd_data.TradingDay!=0)
//			{
//				m_pRecordset->AddNew();	
//				m_pRecordset->PutCollect("exchange_id",_variant_t(ctd_data.ExchangeID));
//				m_pRecordset->PutCollect("instrument_id",_variant_t(ctd_data.InstrumentID));
//				m_pRecordset->PutCollect("trading_day",_variant_t((long)(ctd_data.TradingDay)));
//				m_pRecordset->PutCollect("update_time",_variant_t((long)(ctd_data.UpdateTime)));			
//				m_pRecordset->PutCollect("open_price",_variant_t(ctd_data.OpenPrice));
//				m_pRecordset->PutCollect("close_price",_variant_t(ctd_data.ClosePrice));
//				m_pRecordset->PutCollect("highest_price",_variant_t(ctd_data.HighestPrice));
//				m_pRecordset->PutCollect("lowest_price",_variant_t(ctd_data.LowestPrice));	
//				m_pRecordset->PutCollect("turnover",_variant_t(ctd_data.Turnover));			
//				m_pRecordset->PutCollect("open_interest",_variant_t((long)(ctd_data.OpenInterest)));
//				m_pRecordset->PutCollect("open_interestrange",_variant_t((long)(ctd_data.OpenInterestRange)));
//				m_pRecordset->PutCollect("bid_price1",_variant_t(ctd_data.BidPrice1));
//				m_pRecordset->PutCollect("ask_price1",_variant_t(ctd_data.AskPrice1));
//				m_pRecordset->PutCollect("volume",_variant_t((long)(ctd_data.Volume)));
//				m_pRecordset->PutCollect("bid_volume1",_variant_t(ctd_data.BidVolume1));
//				m_pRecordset->PutCollect("ask_volume1",_variant_t(ctd_data.AskVolume1));
//				m_pRecordset->Update();     ///һ��Ҫ���浽���У���Ȼ��Ч
//
//			}
//
//		}
//
//	}catch(_com_error &e){
//		cout<<e.ErrorMessage()<<endl;
//		//continue;
//	}
//
//	//////////////////////////////////////////////////////////////////////////�������
//#pragma endregion  ִ��SQL���
//
//#pragma region �ر���������
//	try
//	{
//		//�������
//		memset(all_contracts_array.All_Contracts_Data, 0x0, all_contracts_array.index_all * sizeof(CThostFtdcDepthMarketDataField_Custom));
//		m_pRecordset->Close();
//		//m_pRecordset->Release();
//		m_pConnection->Close();
//		m_pConnection.Release();
//		CoUninitialize();
//	}
//	catch(_com_error &e){
//		cout<<e.ErrorMessage()<<endl;
//		//continue;
//	}
//
//#pragma endregion �ر���������
//
//	return ;
//}
#pragma endregion   ADO ����mysql ������1����


#pragma endregion ��ʼ�������ݿ�



/************************************************************************/
/*							���ݷ����߳�                                */
/************************************************************************/
DWORD WINAPI Classify_Data(LPVOID lpParamter)
{

	Sleep(5000);

	bool flag = false;        //ÿ��ֻ����һ������

	while(1)
	{
		if(method_Rest_Time())
		{
			Sleep(1000);
			//������ʱ������
			if (tsmd_map->tsmd_a->size()!=0)
			{
				tsmd_map= NULL;
				tsmd_map = new TSMD_Map();
			}
		}
		//�����ٽ���
		EnterCriticalSection(&cs);
		CThostFtdcDepthMarketDataField_Custom* dd =&(cmdSpi_custom->methodGetDatas_After());
		//�ͷ��ٽ���
		LeaveCriticalSection(&cs);
 		if (dd!=NULL)
 		{
			data_static =*dd;		
			flag = true;   				
		}
		if (flag)
		{
			//��ʼ������
			methodYepan(data_static.UpdateTime);
			flag = false;  

			//ת������
			//CThostFtdcDepthMarketDataField_Custom  ->  CThostFtdcDepthMarketDataField_Custom_Tick
			CThostFtdcDepthMarketDataField_Custom_Tick ct = methodChangeData(data_static);
			
#pragma region //�����map�Ҳ���,��ʼ��

					//�����map�Ҳ���
				if (tsmd_map->tsmd_a->find((string)(data_static.InstrumentID))==tsmd_map->tsmd_a->end())
				{				 
					if (ct.TradingDay==0)
					{
						continue;
					}
					//��������
					TSMD_Arrays *tcreate=new TSMD_Arrays();
					tcreate->contract = (string)(data_static.InstrumentID);

					//ָ��ŵ�Map��
					(*(tsmd_map->tsmd_a))[(string)(data_static.InstrumentID)]=tcreate;					

					#pragma region ����1����

					//����1����
					//////////////////////////////////////////////////////////////////////////
					//1���Ӽ����ʼ��
					tcreate->time_oneminute = method_TimeToInt_60(ct.UpdateTime-ct.UpdateTime%100);//60
					//����1���ӱ�׼
					tcreate->data_oneminute[OM_TICKCOUNT-1].Volume = 0;
					tcreate->data_oneminute[OM_TICKCOUNT-1].Turnover = 0;
					tcreate->data_oneminute[OM_TICKCOUNT-1].OpenInterest = data_static.PreOpenInterest;
					//����1����д��data_tickOneminute[7300]ȥ
					tcreate->methodUpdate_Oneminute(tcreate,ct);
					//////////////////////////////////////////////////////////////////////////
					#pragma endregion ����1����

				}
#pragma endregion //�����map�Ҳ�������ʼ��
				else{
					//��ȡ������,��Ʒ���Ѿ����ֿ���
					TSMD_Arrays * torigin = (*(tsmd_map->tsmd_a))[(string)(data_static.InstrumentID)];
					torigin->contract = (string)(data_static.InstrumentID);

					//����1���ӵĺ���
					torigin->method_Oneminute(torigin,ct);
				}

			
		}//end flag				

	}//end while

}//end FUN


#pragma region ��������
//��������

//���Զ����tick����ת����5�������tick���ݻ���1���ӵ����ݣ������ոߵͼ�Ϊͬһ�۸�
CThostFtdcDepthMarketDataField_Custom_Other TSMD_Arrays::methodTickToOther(
	CThostFtdcDepthMarketDataField_Custom_Tick original_data_tick)
{

	CThostFtdcDepthMarketDataField_Custom_Other custom_tick_other;

	strncpy_s(custom_tick_other.ExchangeID,_countof(custom_tick_other.ExchangeID),
		original_data_tick.ExchangeID,strlen(original_data_tick.ExchangeID));

	strncpy_s(custom_tick_other.InstrumentID,_countof(custom_tick_other.InstrumentID),
		original_data_tick.InstrumentID,strlen(original_data_tick.InstrumentID));

	custom_tick_other.TradingDay=original_data_tick.TradingDay;
	//���̼ۣ����̼ۣ���߼ۣ���ͼ�
	custom_tick_other.OpenPrice=original_data_tick.LastPrice;
	custom_tick_other.ClosePrice=original_data_tick.LastPrice;
	custom_tick_other.HighestPrice=original_data_tick.LastPrice;
	custom_tick_other.LowestPrice=original_data_tick.LastPrice;

	custom_tick_other.Volume=original_data_tick.Volume;
	custom_tick_other.Turnover=original_data_tick.Turnover;
	custom_tick_other.OpenInterest=original_data_tick.OpenInterest;
	custom_tick_other.UpdateTime=original_data_tick.UpdateTime;
	custom_tick_other.BidPrice1=original_data_tick.BidPrice1;
	custom_tick_other.BidVolume1=original_data_tick.BidVolume1;
	custom_tick_other.AskPrice1=original_data_tick.AskPrice1;
	custom_tick_other.AskVolume1=original_data_tick.AskVolume1;
	custom_tick_other.OpenInterestRange = original_data_tick.OpenInterestRange;

	return custom_tick_other;
};

//д��data_oneminute[650]������ȥ
void TSMD_Arrays::methodUpdate_Oneminute(TSMD_Arrays *tsmdarrays,
	CThostFtdcDepthMarketDataField_Custom_Tick custom_tick){
	//�����ոߵ͸���ͬһ�۸�
	CThostFtdcDepthMarketDataField_Custom_Other co = tsmdarrays->methodTickToOther(custom_tick);

	tsmdarrays->data_oneminute[(tsmdarrays->index_oneminute)] = co;      //����data_oneminute[650]����
	//������һ
	tsmdarrays->index_oneminute++;
	//���±�׼
	tsmdarrays->data_oneminute[OM_TICKCOUNT-1] = co;
};

//��һ��tick���ݴ����ҳ�1�����ڵĿ��ոߵͼ۵ȵ�
void TSMD_Arrays::method_Oneminute(TSMD_Arrays *tsmdarrays,
	CThostFtdcDepthMarketDataField_Custom_Tick custom_tick)
{
	int time_60=method_TimeToInt_60(custom_tick.UpdateTime);
	if(time_60 - tsmdarrays->time_oneminute<60&&time_60 - tsmdarrays->time_oneminute>=0)
	{
		//��ֹԽ��
		if (tsmdarrays->index_oneminute>OM_TICKCOUNT-5)
		{
			tsmdarrays->data_oneminute[0] = tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1];
			tsmdarrays->index_oneminute=1;
		}
		//���������߼�
		if (custom_tick.LastPrice > tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].HighestPrice)
		{
			//������߼�
			tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].HighestPrice = custom_tick.LastPrice;

		}else if(custom_tick.LastPrice < tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].LowestPrice)
		{
			//������ͼ�
			tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].LowestPrice = custom_tick.LastPrice;		

		}
		//�������̼�
		tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].ClosePrice = custom_tick.LastPrice;

		//ί��ί��
		tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].BidPrice1=custom_tick.BidPrice1;
		tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].BidVolume1=custom_tick.BidVolume1;
		tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].AskPrice1=custom_tick.AskPrice1;
		tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].AskVolume1=custom_tick.AskVolume1;
		//���³ֲ���
		tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].OpenInterest=custom_tick.OpenInterest;
		//���³ɽ������ɽ���ֱֲ仯
		tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].Volume = 
			custom_tick.Volume - tsmdarrays->data_oneminute[OM_TICKCOUNT-1].Volume;

		tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].Turnover =
			custom_tick.Turnover - tsmdarrays->data_oneminute[OM_TICKCOUNT-1].Turnover;

		tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1].OpenInterestRange = 
			custom_tick.OpenInterest- tsmdarrays->data_oneminute[OM_TICKCOUNT-1].OpenInterest;

	}
	else if (time_60 - tsmdarrays->time_oneminute>=60)
	{
		//����̨���
		CThostFtdcDepthMarketDataField_Custom_Other fff = tsmdarrays->data_oneminute[tsmdarrays->index_oneminute-1];
		//cerr << fff.InstrumentID<<" "<<fff.UpdateTime<<" ��߼ۣ�"<<fff.HighestPrice<<"��ͼۣ� "<<fff.LowestPrice<<" "<<endl;
		//cerr <</* fff.InstrumentID<<*/"        "<<fff.UpdateTime<<" �ɽ�����"<<fff.Volume<<" �ɽ��"<<fff.Turnover<<" �ֲ�����"<<fff.OpenInterest<<" �ֲ"<<fff.OpenInterestRange<<endl;
	
		//дMysql���ݿ�
		methodWriteFile2(fff,"today-1min");
	    Ado_Connect_Mysql(fff);
		//����ʱ��
		tsmdarrays->time_oneminute = time_60;//tsmdarrays->time_oneminute + 60;
		//д������
		tsmdarrays->methodUpdate_Oneminute(tsmdarrays,custom_tick);

	}
	
};


 
#pragma endregion ��������


// 	CThostFtdcDepthMarketDataField_Custom_Other fff;
// 	fff.TradingDay=10;
// 	char *a="cffex";
// 	char *b="IF1510";
// 	//string a="cffex";
// 	//string b="IF1510";
// 	strcpy_s(fff.ExchangeID,sizeof(fff.ExchangeID),a);
// 	//strcat(fff.ExchangeID,"\0");
// 	strcpy_s(fff.InstrumentID,sizeof(fff.InstrumentID),b);
// 	
// 
// 	Ado_Connect_Mysql(fff);


//д��data_tick[73000]������ȥ
/*void TSMD_Arrays::methodUpdate_tick(TSMD_Arrays *tsmdarrays,CThostFtdcDepthMarketDataField_Custom_Tick custom_tick){

	tsmdarrays->data_tick[tsmdarrays->index_tick]=custom_tick;    //����data_tick[73000]����
	//���³ɽ������ɽ���ֲ�
	tsmdarrays->data_tick[tsmdarrays->index_tick].Volume = custom_tick.Volume -tsmdarrays->data_tick[72999].Volume;
	tsmdarrays->data_tick[tsmdarrays->index_tick].Turnover = custom_tick.Turnover - tsmdarrays->data_tick[72999].Turnover;
	tsmdarrays->data_tick[tsmdarrays->index_tick].OpenInterestRange = custom_tick.OpenInterest - tsmdarrays->data_tick[72999].OpenInterest;

	// 	CThostFtdcDepthMarketDataField_Custom_Tick fff = tsmdarrays->data_tick[tsmdarrays->index_tick];
	// 	cerr << fff.InstrumentID<<" "<<fff.UpdateTime<<" "<<fff.Volume<<" "<<fff.OpenInterest<<" "<<fff.OpenInterestRange<<endl;
	//������һ
	tsmdarrays->index_tick++;   
	//���±�׼
	tsmdarrays->data_tick[TICKCOUNT-1]=custom_tick;
	//����̨���
	//  	 	CThostFtdcDepthMarketDataField_Custom_Tick fff = tsmdarrays->data_tick[tsmdarrays->index_tick-1];
	//  	  	cerr << fff.InstrumentID<<" "<<fff.UpdateTime<<" "<<fff.LastPrice<<"�ɽ����� "<<fff.Volume<<"�ֲ����� "<<fff.OpenInterest<<" �ֲ"<<fff.OpenInterestRange<<endl;

}; */ 
//д��data_Fiveseconds[7300]������ȥ
/*void TSMD_Arrays::methodUpdate_Fiveseconds(TSMD_Arrays *tsmdarrays,
	CThostFtdcDepthMarketDataField_Custom_Tick custom_tick)
{
	//�����ոߵ͸���ͬһ�۸�
	CThostFtdcDepthMarketDataField_Custom_Other co = tsmdarrays->methodTickToOther(custom_tick);

	tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds] = co;      //����data_Fiveseconds[7300]����
	//������һ
	tsmdarrays->index_fiveseconds++;
	//���±�׼
	tsmdarrays->data_fiveseconds[FS_TICKCOUNT-1] = co;					 //�ɽ������ɽ���ֱֲ仯�õ�
};*/
//��һ��tick���ݴ����ҳ�5���ڵĿ��ոߵͼ۵ȵ�
//void TSMD_Arrays::method_Fiveseconds(TSMD_Arrays *tsmdarrays,
//	CThostFtdcDepthMarketDataField_Custom_Tick custom_tick)
//{
//
//	int time_60=method_TimeToInt_60(custom_tick.UpdateTime);
//	if(time_60 - tsmdarrays->time_fivesecond < 5/*&&time_60 - tsmdarrays->time_fivesecond>0*/)
//	{
//		//���������߼�
//		if (custom_tick.LastPrice > tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].HighestPrice)
//		{
//			//������߼�
//			tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].HighestPrice = custom_tick.LastPrice;
//
//		}else if(custom_tick.LastPrice < tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].LowestPrice)
//		{
//			//������ͼ�
//			tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].LowestPrice = custom_tick.LastPrice;		
//
//		}
//		//�������̼�
//		tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].ClosePrice = custom_tick.LastPrice;
//
//		//ί��ί��
//		tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].BidPrice1=custom_tick.BidPrice1;
//		tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].BidVolume1=custom_tick.BidVolume1;
//		tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].AskPrice1=custom_tick.AskPrice1;
//		tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].AskVolume1=custom_tick.AskVolume1;
//		//���³ֲ���
//		tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].OpenInterest=custom_tick.OpenInterest;
//		//���³ɽ������ɽ���ֱֲ仯
//		tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].Volume =
//			custom_tick.Volume - tsmdarrays->data_fiveseconds[FS_TICKCOUNT-1].Volume; 
//
//		tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].Turnover =
//			custom_tick.Turnover - tsmdarrays->data_fiveseconds[FS_TICKCOUNT-1].Turnover;	
//
//		tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].OpenInterestRange = 
//			custom_tick.OpenInterest- tsmdarrays->data_fiveseconds[FS_TICKCOUNT-1].OpenInterest;
//
//
//
//	}else{
//		// 				//���³ɽ������ɽ���ֱֲ仯
//		// 				tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].Volume = custom_tick.Volume - tsmdarrays->data_fiveseconds[7299].Volume;
//		// 				tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].Turnover=custom_tick.Turnover - tsmdarrays->data_fiveseconds[7299].Turnover;		
//		// 				tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].OpenInterestRange= custom_tick.OpenInterest- tsmdarrays->data_fiveseconds[7299].OpenInterest;
//		// 				//���³ֲ���
//		// 				tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1].OpenInterest=custom_tick.OpenInterest;
//		//����̨���
// 		CThostFtdcDepthMarketDataField_Custom_Other fff = tsmdarrays->data_fiveseconds[tsmdarrays->index_fiveseconds-1];
////		cerr << fff.InstrumentID<<" "<<fff.UpdateTime<<" ��߼ۣ�"<<fff.HighestPrice<<"��ͼۣ� "<<fff.LowestPrice<<" "<<endl;
////		cerr <</* fff.InstrumentID<<*/"        "<<fff.UpdateTime<<" �ɽ�����"<<fff.Volume<<" �ɽ��"<<fff.Turnover<<" �ֲ�����"<<fff.OpenInterest<<" �ֲ"<<fff.OpenInterestRange<<endl;
//// 		string path=".\\datas2\\today-5sec";
//// 		FILE* file = _fsopen(path.c_str(),"ab+",_SH_DENYWR);
//// 		fwrite(&fff,sizeof(fff),1,file);
//// 		fclose(file);
//		//дMysql���ݿ�
//		methodWriteFile2(fff,"today-5sec");
//		Ado_Connect_Mysql(fff);
//		//����ʱ��
//		tsmdarrays->time_fivesecond = tsmdarrays->time_fivesecond + 5;
//		//д������
//		tsmdarrays->methodUpdate_Fiveseconds(tsmdarrays,custom_tick);
//
//	}
//};
//void TSMD_Arrays::method_Oneday(TSMD_Arrays *tsmdarrays,
//	CThostFtdcDepthMarketDataField_Custom_Tick custom_tick)
//{
//
//	strncpy_s(tsmdarrays->data_oneday.ExchangeID,_countof(tsmdarrays->data_oneday.ExchangeID),
//		custom_tick.ExchangeID,strlen(custom_tick.ExchangeID));
//
//	strncpy_s(tsmdarrays->data_oneday.InstrumentID,_countof(tsmdarrays->data_oneday.InstrumentID),
//		custom_tick.InstrumentID,strlen(custom_tick.InstrumentID));
//
//	tsmdarrays->data_oneday.TradingDay=custom_tick.TradingDay;
//
//	//���̼ۣ����̼ۣ���߼ۣ���ͼ�
//	/*tsmdarrays->data_oneday.OpenPrice=custom_tick.LastPrice;*/
//	tsmdarrays->data_oneday.ClosePrice=custom_tick.LastPrice;
//	if (tsmdarrays->data_oneday.HighestPrice < custom_tick.LastPrice)
//	{
//		tsmdarrays->data_oneday.HighestPrice = custom_tick.LastPrice;
//	}else if (tsmdarrays->data_oneday.LowestPrice > custom_tick.LastPrice)
//	{
//		tsmdarrays->data_oneday.LowestPrice = custom_tick.LastPrice;
//	}
//	tsmdarrays->data_oneday.Volume=custom_tick.Volume;
//	tsmdarrays->data_oneday.Turnover=custom_tick.Turnover;
//	tsmdarrays->data_oneday.OpenInterest=custom_tick.OpenInterest;
//	tsmdarrays->data_oneday.UpdateTime=custom_tick.UpdateTime;
//	tsmdarrays->data_oneday.BidPrice1=custom_tick.BidPrice1;
//	tsmdarrays->data_oneday.BidVolume1=custom_tick.BidVolume1;
//	tsmdarrays->data_oneday.AskPrice1=custom_tick.AskPrice1;
//	tsmdarrays->data_oneday.AskVolume1=custom_tick.AskVolume1;
//	tsmdarrays->data_oneday.OpenInterestRange = custom_tick.OpenInterestRange;
//};


