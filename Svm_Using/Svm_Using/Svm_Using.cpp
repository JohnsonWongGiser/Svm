// Svm_Using.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Svm_Using.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//


// CSvm_UsingApp

BEGIN_MESSAGE_MAP(CSvm_UsingApp, CWinApp)
END_MESSAGE_MAP()


// CSvm_UsingApp 构造

CSvm_UsingApp::CSvm_UsingApp(float C,float gamma,float epsilon)
{
	m_svmpara=Initialize_svm_parameter(C,gamma,epsilon); 
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSvm_UsingApp 对象

CSvm_UsingApp theApp;


// CSvm_UsingApp 初始化

BOOL CSvm_UsingApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

svm_parameter CSvm_UsingApp::Initialize_svm_parameter(float C,float gamma,float epsilon)  
{  
    svm_parameter svmpara;//svm的相关参数  
    svmpara.svm_type = C_SVC;  ///svm的类型，包括分类、预测拟合等，如果想用svm进行分类那么可以选择C_SVC，如果想用于拟合预测那么可以选择EPSILON_SVR
    svmpara.kernel_type =RBF;  
    svmpara.degree = 3;  
    svmpara.gamma = gamma;  // 默认大小可选择特征的倒数 1/num_features，核函数中的gamma函数设置（只针对多项式/rbf/sigmoid核函数）  
    svmpara.coef0 = 0;  
    svmpara.nu = 0.5;  
    svmpara.cache_size = 1;//缓存块大小  
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
     
    int sample_num=traindata.size();//训练样本个数  
    int feature_dimn=3;//样本的特征维数为1  
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

//获取最大最小值  
    GetMax_Min(y,sample_num,m_miny,m_maxy);//训练数据y的最大最小值获取  
    m_minx=new double[feature_dimn];//特征最大最小值获取  
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
 //训练数据归一化  
    for (int i=0;i<sample_num;i++)  
    {  
        y[i]=2*(y[i]-m_miny)/(m_maxy-m_miny)-1;  
        for (int j=0;j<feature_dimn;j++)  
        {  
            x[i][j]=2*(x[i][j]-m_minx[j])/(m_maxx[j]-m_minx[j])-1;  
        }  
    }  
//训练数据输入  
    svm_problem svmpro;//svm训练数据  
    svmpro.l=sample_num;  
    svmpro.y=new double[sample_num];  
    svmpro.x=new svm_node *[sample_num];//训练数据的特征向量  
    for (int i=0;i<sample_num;i++)  
    {  
        svmpro.y[i]=y[i];//用训练数据的y，作为输入标签  
        svmpro.x[i]=new svm_node[feature_dimn+1];  
        for (int j=0;j<feature_dimn;j++)  
        {  
            svm_node node_ij;  
            node_ij.index=j+1;//需要注意的是svm_node的第一个数据的索引为1，数值为第一位特征值，我一开始这里搞错了，把索引搞成从0开始  
            node_ij.value=x[i][j];  
            svmpro.x[i][j]=node_ij;  
        }  
        svm_node node_last;//需要添加最后一维特征的索引为-1  
        node_last.index=-1;  
        svmpro.x[i][feature_dimn]=node_last;  
    }  
//验证输入的训练数据、初始化的参数是否有误  
    const char *error_msg;  
    error_msg = svm_check_parameter(&svmpro,&m_svmpara);  
    if(error_msg)  
    {  
        AfxMessageBox(error_msg);  
    }  
//数据训练  
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
//求数据的最大值和最小值  
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


