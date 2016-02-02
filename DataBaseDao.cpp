
#include "DataBaseDao.h"
using namespace std;

//�������ݿ�
void Ado_Connect_Mysql(CThostFtdcDepthMarketDataField_Custom_Other ctd_data)
{
#pragma region �������ݿ�
	//////////////////////////////////////////////////////////////////////////�������ݿ�
	CoInitialize(NULL); 
	_ConnectionPtr m_pConnection=NULL;
	//����Connection����
	m_pConnection.CreateInstance("ADODB.Connection");//(__uuidof(Connection));   
	try 
	{  
		//��������ʱ��  
		m_pConnection->ConnectionTimeout = 5;
		_bstr_t connstr = "DSN=MySqlODBC;server=";
		connstr += local_host ;
		connstr += ";database=";
		connstr += data_base;;
		//�����ݿ�����  
		HRESULT hr = m_pConnection->Open(connstr,user_name,user_pwd,adModeUnknown); //adModeUnknown ȱʡ����ǰ�����Ȩδ����
		//m_pConnection->Open("Data Source=adotest;UID=sa;PWD=;","","",adModeUnknown);
	}  
	catch(_com_error &e)  
	{  
		cout<<"���Ӵ���"<<endl;
		return ;  
	}   
	//////////////////////////////////////////////////////////////////////////�������ݿ�


#pragma endregion �������ݿ�

#pragma region ִ��SQL���
	//////////////////////////////////////////////////////////////////////////�������
	//ִ��SELETE���
	_RecordsetPtr m_pRecordset;
	//m_pRecordset.CreateInstance("ADODB.Recordset");
	m_pRecordset.CreateInstance(__uuidof( Recordset )); //����ʵ��
	//һ��Ҫ��
	_bstr_t openRecordset="SELECT  * FROM ";
	openRecordset+= table;
	openRecordset +=" limit 1";
	m_pRecordset->Open(openRecordset,m_pConnection.GetInterfacePtr
		(),adOpenStatic,adLockOptimistic,adCmdText);// ִ��SQL��䣬�õ���¼��, m_pConnection������֪
	try{
		if (m_pRecordset!=NULL)
		{
			if (ctd_data.TradingDay!=0)
			{
				m_pRecordset->AddNew();	
				m_pRecordset->PutCollect("exchange_id",_variant_t(ctd_data.ExchangeID));
				m_pRecordset->PutCollect("instrument_id",_variant_t(ctd_data.InstrumentID));
				m_pRecordset->PutCollect("trading_day",_variant_t((long)(ctd_data.TradingDay)));
				m_pRecordset->PutCollect("update_time",_variant_t((long)(ctd_data.UpdateTime)));			
				m_pRecordset->PutCollect("open_price",_variant_t(ctd_data.OpenPrice));
				m_pRecordset->PutCollect("close_price",_variant_t(ctd_data.ClosePrice));
				m_pRecordset->PutCollect("highest_price",_variant_t(ctd_data.HighestPrice));
				m_pRecordset->PutCollect("lowest_price",_variant_t(ctd_data.LowestPrice));	
				m_pRecordset->PutCollect("turnover",_variant_t(ctd_data.Turnover));			
				m_pRecordset->PutCollect("open_interest",_variant_t((long)(ctd_data.OpenInterest)));
				m_pRecordset->PutCollect("open_interestrange",_variant_t((long)(ctd_data.OpenInterestRange)));
				m_pRecordset->PutCollect("bid_price1",_variant_t(ctd_data.BidPrice1));
				m_pRecordset->PutCollect("ask_price1",_variant_t(ctd_data.AskPrice1));
				m_pRecordset->PutCollect("volume",_variant_t((long)(ctd_data.Volume)));
				m_pRecordset->PutCollect("bid_volume1",_variant_t(ctd_data.BidVolume1));
				m_pRecordset->PutCollect("ask_volume1",_variant_t(ctd_data.AskVolume1));
				m_pRecordset->Update();     ///һ��Ҫ���浽���У���Ȼ��Ч

			}

		}

	}catch(_com_error &e){
		cout<<e.ErrorMessage()<<endl;
		//continue;
	}

	//////////////////////////////////////////////////////////////////////////�������
#pragma endregion  ִ��SQL���

#pragma region �ر���������
	try
	{
		//�������
		//memset(all_contracts_array.All_Contracts_Data, 0x0, all_contracts_array.index_all * sizeof(CThostFtdcDepthMarketDataField_Custom));
		m_pRecordset->Close();
		//m_pRecordset->Release();
		m_pConnection->Close();
		m_pConnection.Release();
		CoUninitialize();
	}
	catch(_com_error &e){
		cout<<e.ErrorMessage()<<endl;
		//continue;
	}

#pragma endregion �ر���������

	return ;
}