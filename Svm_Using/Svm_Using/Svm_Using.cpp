// Svm_Using.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Svm_Using.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//


// CSvm_UsingApp

BEGIN_MESSAGE_MAP(CSvm_UsingApp, CWinApp)
END_MESSAGE_MAP()


// CSvm_UsingApp ����

CSvm_UsingApp::CSvm_UsingApp(float C,float gamma,float epsilon)
{
	m_svmpara=Initialize_svm_parameter(C,gamma,epsilon); 
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSvm_UsingApp ����

CSvm_UsingApp theApp;


// CSvm_UsingApp ��ʼ��

BOOL CSvm_UsingApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

svm_parameter CSvm_UsingApp::Initialize_svm_parameter(float C,float gamma,float epsilon)  
{  
    svm_parameter svmpara;//svm����ز���  
    svmpara.svm_type = C_SVC;  ///svm�����ͣ��������ࡢԤ����ϵȣ��������svm���з�����ô����ѡ��C_SVC��������������Ԥ����ô����ѡ��EPSILON_SVR
    svmpara.kernel_type =RBF;  
    svmpara.degree = 3;  
    svmpara.gamma = gamma;  // Ĭ�ϴ�С��ѡ�������ĵ��� 1/num_features���˺����е�gamma�������ã�ֻ��Զ���ʽ/rbf/sigmoid�˺�����  
    svmpara.coef0 = 0;  
    svmpara.nu = 0.5;  
    svmpara.cache_size = 1;//������С  
    svmpara.C = C;  
    svmpara.eps = 1e-3;  
    svmpara.p = epsilon;  
    svmpara.shrinking = 1;  
    svmpara.probability = 0;  
    svmpara.nr_weight = 0;  
    svmpara.weight_label = NULL;  
    svmpara.weight = NULL;  
    return svmpara;  
}  

void CSvm_UsingApp::TrainModel(vector<vector<double>>traindata)  
{  
     
    int sample_num=traindata.size();//ѵ����������  
    int feature_dimn=3;//����������ά��Ϊ1  
    double *y=new double[sample_num];  
    double **x=new double *[sample_num];  
    for (int i=0;i<sample_num;i++)  
    {  
        x[i]=new double[feature_dimn];  
    }  
  
    for (int i=0;i<sample_num;i++)  
    {  
        y[i]=traindata[i][feature_dimn];  
        for (int j=0;j<feature_dimn;j++)  
        {  
            x[i][j]=traindata[i][j];  
        }  
    }

//��ȡ�����Сֵ  
    GetMax_Min(y,sample_num,m_miny,m_maxy);//ѵ������y�������Сֵ��ȡ  
    m_minx=new double[feature_dimn];//���������Сֵ��ȡ  
    m_maxx=new double[feature_dimn];  
    double *pdata=new double[sample_num];  
    for (int j=0;j<feature_dimn;j++)  
    {  
        for (int i=0;i<sample_num;i++)  
        {  
            pdata[i]=x[i][j];     
        }  
        GetMax_Min(pdata,sample_num,m_minx[j],m_maxx[j]);  
    }  
 //ѵ�����ݹ�һ��  
    for (int i=0;i<sample_num;i++)  
    {  
        y[i]=2*(y[i]-m_miny)/(m_maxy-m_miny)-1;  
        for (int j=0;j<feature_dimn;j++)  
        {  
            x[i][j]=2*(x[i][j]-m_minx[j])/(m_maxx[j]-m_minx[j])-1;  
        }  
    }  
//ѵ����������  
    svm_problem svmpro;//svmѵ������  
    svmpro.l=sample_num;  
    svmpro.y=new double[sample_num];  
    svmpro.x=new svm_node *[sample_num];//ѵ�����ݵ���������  
    for (int i=0;i<sample_num;i++)  
    {  
        svmpro.y[i]=y[i];//��ѵ�����ݵ�y����Ϊ�����ǩ  
        svmpro.x[i]=new svm_node[feature_dimn+1];  
        for (int j=0;j<feature_dimn;j++)  
        {  
            svm_node node_ij;  
            node_ij.index=j+1;//��Ҫע�����svm_node�ĵ�һ�����ݵ�����Ϊ1����ֵΪ��һλ����ֵ����һ��ʼ�������ˣ���������ɴ�0��ʼ  
            node_ij.value=x[i][j];  
            svmpro.x[i][j]=node_ij;  
        }  
        svm_node node_last;//��Ҫ������һά����������Ϊ-1  
        node_last.index=-1;  
        svmpro.x[i][feature_dimn]=node_last;  
    }  
//��֤�����ѵ�����ݡ���ʼ���Ĳ����Ƿ�����  
    const char *error_msg;  
    error_msg = svm_check_parameter(&svmpro,&m_svmpara);  
    if(error_msg)  
    {  
        AfxMessageBox(error_msg);  
    }  
//����ѵ��  
    m_svmmodel=svm_train(&svmpro,&m_svmpara);  
} 
void CSvm_UsingApp::Predict(float x,float &y)  
{  
  
    x=2*(x-m_minx[0])/(m_maxx[0]-m_minx[0])-1;  
    svm_node *testX1=new svm_node[1+1];  
    testX1[0].index=1;  
    testX1[0].value=x;  
    testX1[1].index=-1;  
    y=(svm_predict(m_svmmodel,testX1)+1)*(m_maxy-m_miny)*0.5+m_miny;  
  
  
}  
//�����ݵ����ֵ����Сֵ  
void CSvm_UsingApp::GetMax_Min(double*pdata,int data_num,double &minpt,double&maxpt)  
{  
  
    double minx=1e10;  
    double maxx=-1e10;  
    for (int i=0;i<data_num;i++)  
    {  
        if (pdata[i]<minx)  
        {  
            minx=pdata[i];  
        }  
        if (pdata[i]>maxx)  
        {  
            maxx=pdata[i];  
        }  
    }  
    minpt=minx;  
    maxpt=maxx;  
}  


