
// keyplayers_Probability_GUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "keyplayers_Probability_GUI.h"
#include "keyplayers_Probability_GUIDlg.h"
#include "afxdialogex.h"
#include "iostream"
#include "fstream"
#include "iomanip"
#include "conio.h"
#include "cstdlib"
#include "windows.h"
#include "ctime"
#include "fstream"
#include "string"
#include <vector>
#include <sstream>

using namespace std;

#define CTH 10

int MTLIKE[15][15]={0,0},n,countActive[1024][1024] = {0,0}, countReachMore[15][15] = {0}, saveActive[15][15][100001] = {0}, vertex, a[1024]={0}, spread[100001] = {0}, minSpread = 0, maxSpread = 0;
float MTWEIGHTED[15][15]={0,0}, b[1024]={0};
double LenPath[15][15] = {0,0}, LenPathAffect[15][15] = {0,0}, avgSpread = 0.0;
string member[15],affectionTop[15],stringLenPath[15], collection[15], stringLenPathAffect[15], affectionTopAffect[15];
float LenSum[15]={0}, spreadRatio[15][15]={0.0}, edgeRandom[15][15][100001] = {0.0}, ratioRandom[15][15][100001] = {0.0};
bool markActive[15] = {0}, m_sDone = false, m_IsPath[1024][1024] = {false, false}, m_sDoneAffect = false;
HANDLE myMutex;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Ckeyplayers_Probability_GUIDlg dialog




Ckeyplayers_Probability_GUIDlg::Ckeyplayers_Probability_GUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ckeyplayers_Probability_GUIDlg::IDD, pParent)
	, m_sFileIn(_T(""))
	, m_sVertex(_T(""))
	, m_sIteration(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ckeyplayers_Probability_GUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILEIN, m_sFileIn);
	DDX_Text(pDX, IDC_EDIT_VERTEX, m_sVertex);
	DDX_Text(pDX, IDC_EDIT_ITERATION, m_sIteration);
}

BEGIN_MESSAGE_MAP(Ckeyplayers_Probability_GUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &Ckeyplayers_Probability_GUIDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &Ckeyplayers_Probability_GUIDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_OPEN_FILE, &Ckeyplayers_Probability_GUIDlg::OnBnClickedBtnOpenFile)
	ON_BN_CLICKED(IDC_BUTTON1, &Ckeyplayers_Probability_GUIDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_ABOUT, &Ckeyplayers_Probability_GUIDlg::OnBnClickedBtnAbout)
	ON_BN_CLICKED(IDC_BTN_AFFECT, &Ckeyplayers_Probability_GUIDlg::OnBnClickedBtnAffect)
	ON_BN_CLICKED(IDC_BTN_RESET, &Ckeyplayers_Probability_GUIDlg::OnBnClickedBtnReset)
END_MESSAGE_MAP()


// Ckeyplayers_Probability_GUIDlg message handlers

BOOL Ckeyplayers_Probability_GUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_sFileIn = "";
	m_sVertex = "";
	m_sIteration = "";
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Ckeyplayers_Probability_GUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Ckeyplayers_Probability_GUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Ckeyplayers_Probability_GUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

string IntToString ( int number )
{
  std::ostringstream oss;

  // Works just like cout
  oss<< number;

  // Return the underlying string
  return oss.str();
}

string FloatToString ( float number )
{
  std::ostringstream oss;

  // Works just like cout
  oss<< number;

  // Return the underlying string
  return oss.str();
}

void writeMT(float mtg[1557], int vertex, int iteration)
{
	string path="Result_Probability.txt";
	ofstream write (path);
	//write<<"Suc anh huong cua dinh "<<vertex<< ":" <<member[vertex] <<" : "<<LenSum[vertex];
	//write<<"\n";
	write<<"Xac suat lan truyen y tuong thanh cong cua dinh "<<vertex<<" toi cac dinh khac nhu sau: "<<"\n";
	//write<<affectionTop[vertex]<<"\n";
	write<<stringLenPath[vertex]<<"\n";
	write<<"So luong dinh duoc active tu dinh nguon "<<vertex<<" qua cac lan lap nhu sau: "<<"\n";
	//write<<affectionTop[vertex]<<"\n";
	for (int i = 1; i<= iteration; i++)
	{
		write<<spread[i]<<",";
	}
	write<<"\nGia tri nho nhat: "<<minSpread;
	write<<"\nGia tri lon nhat: "<<maxSpread;
	write<<"\nGia tri trung binh: "<<avgSpread;
}

vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  return internal;
}

void readMT(string filename)
{
    ifstream readMTLike (filename);
	ifstream readMTLike1 (filename);
	if(!readMTLike.is_open())
    {
        cout<<"Khong the mo file.\n";
        return ;
    }
    else 
	{		
		string line;
		std::getline(readMTLike, line);
		vector<string> sep = split(line, ' ');
		n = sep.size();
   		for(int i = 0; i < n; i++)
		{
			int	j = i+1;
			member[j] = sep[i];
		}
        for(int i = 1; i <= n; i++)
			for(int j=1;j<=n;j++)
			{
				readMTLike1>>MTLIKE[i][j];
				//spreadRatio[i][j] = 1.0;
			}
    }
    readMTLike.close();
	readMTLike1.close();
}

void createMTPro(int numOfIteration)
{
	int i,j;
	srand(time(NULL));
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			if(MTWEIGHTED[i][j] > 0)
			{
				for ( int k = 0; k<=numOfIteration; k++)
				{
					edgeRandom[i][j][k] = ((float) rand() / (RAND_MAX)) + 0.0;
				}
			}
		}
	}
}

void createMT(string filename)
{
	readMT(filename);
	int i,j;
	for(i=1;i<=n;i++)
	{
		for(j=1;j<=n;j++)
		{
			if(i==j)
			{
				MTWEIGHTED[i][j]=0;
			}
			else
			{
				if(MTLIKE[i][j]!=0)
				{
					{
						MTWEIGHTED[i][j]=(1.0*MTLIKE[i][j])/MTLIKE[i][i];						
					}
				}
				else
					MTWEIGHTED[i][j]=MTLIKE[i][j];
			}
		}
	}
}

bool FindVertexInString(string collection1, int a)
{
	if (collection1.find(IntToString(a)) != string::npos) return true;
	return false;
}

void CountActiveReach( string collection1, int index, bool flag, int k )
{
	for (int i = 1; i<=n; i++)
	{
		if (FindVertexInString(collection1, i) == true && i != index)
		{
			if ( flag == true )
			{
				countActive[i][index]++;
			}
			else
			{
				saveActive[i][index][k]++;
			}
		}
	}
}

void ProcessVertex( int index, int k )
{
	markActive[index] = true;
	for ( int j = 1; j <= n; j++)
	{
		if (markActive[j] == false && MTWEIGHTED[index][j] > 0)
		{
			if ( edgeRandom[index][j][k] <= MTWEIGHTED[index][j] )
			{
				collection[j].clear();
				collection[j].append(collection[index]);
				collection[j].append(IntToString( j ));
				if (collection[j][collection[j].length()-1] != ',')
				{
					collection[j].append(",");
				}
				CountActiveReach(collection[j], j, true, k);
				if (collection[j][collection[j].length()-1] != ',')
				{
					collection[j].append(",");
				}
				//TRACE("%s", collection[j].c_str());
				markActive[j] = true;
				//if ( ratioRandom[j][index][k] <= spreadRatio[j][index] )
				{
					ProcessVertex(j, k);
				}
			}
			else
			{
				collection[j].clear();
				collection[j].append(collection[index]);
				if (collection[j][collection[j].length()-1] != ',')
				{
					collection[j].append(",");
				}
				CountActiveReach(collection[j], j, false, k);
			}
		}
	}
}

void ResetMarkActive()
{
	for (int i = 1; i<=n; i++)
	{
		markActive[i] = false;
	}
}

void SetNumOfActive( int i, int k)
{
	for ( int j = 1; j<=n; j++)
	{
		if ( markActive[j] == false || i == j )
		{	
			saveActive[i][j][k] = 0;
		}
		else
		{
			saveActive[i][j][k]++;
		}
	}
}

void CountSpreadEveryTimes(int vertex, int k)
{
	spread[k] = 0;
	for (int i = 1; i<=n; i++)
	{
		if ( markActive[i] == true && vertex != i ) spread[k]++;
	}
}
void CalculateValueSpread(int numOfIteration)
{
	double sum = 0.0;
	for (int i = 1; i<=numOfIteration; i++)
	{
		if(spread[i] < minSpread ) minSpread = spread[i];
		if (spread[i] > maxSpread) maxSpread = spread[i];
		sum += spread[i];
	}
	avgSpread = sum/numOfIteration;
}

void handlerResult(int vertex, int numOfIteration )
{
	//createMT("E:/DATA/Study/social network/Coding/Bai bao/data_test/4.txt","E:/DATA/Study/social network/Coding/Bai bao/data_test/ratio4.txt","E:/DATA/Study/social network/Coding/Thuyet trinh/New folder/data/dulieu6_member.txt");
	for ( int i = 1; i<=n; i++)
	{
		collection[i].clear();
	}
	collection[0].append(IntToString(0));
	if (collection[0][collection[0].length()-1] != ',')
	{
		collection[0].append(",");
	}
	collection[vertex].append(IntToString(vertex));
	if (collection[vertex][collection[vertex].length()-1] != ',')
	{
		collection[vertex].append(",");
	}
	for ( int i = 1; i<= numOfIteration; i++)
	{
		ResetMarkActive();
		ProcessVertex(vertex,i);
		SetNumOfActive(vertex, i);
		CountSpreadEveryTimes(vertex, i);
	}
	CalculateValueSpread(numOfIteration);
	for( int i=1; i<=n; i++)
	{
	for( int j=1; j<=n; j++)
		{
			if (countActive[i][j] > 0)
				LenPath[i][j] = (float)(countActive[i][j]/float(numOfIteration));
			if ( LenPath[i][j] > 0 )
			{
				string topValue = IntToString( j );				
				affectionTop[i].append(topValue);
				affectionTop[i].append(" ");
				stringLenPath[i].append(topValue);
				stringLenPath[i].append(" : ");
				stringLenPath[i].append(FloatToString( LenPath[i][j] ));
				stringLenPath[i].append("\n");
			}
			
			int numActive = 0, activeAverage = 0;
			/*for ( int k = 1; k<=numOfIteration; k++)
			{
				if (saveActive[i][j][k]>0)
				{
					stringLenPath[i].append(IntToString( saveActive[i][j][k] ));
					stringLenPath[i].append(",");
					numActive++;
					activeAverage += saveActive[i][j][k];
				}
			}*/
			/*if ( activeAverage > 0 && numActive> 0)
			{
				stringLenPath[i].append("\nGia tri trung binh: ");
				float temp = (float)(activeAverage/(float)numActive);
				stringLenPath[i].append(FloatToString(temp));
				stringLenPath[i].append("\n\n");
			}*/
			
			//stringLenPath[i].append("\n");
			//cout<<"LenPath["<<i<<"]["<<j<<"]"<<LenPath[i][j]<<"\n";
			LenSum[i] += LenPath[i][j];
			//cout<<"First Active"<<firstActive[i][j]<<"\n";
		}
		//cout<<"LenSum["<<i<<"]"<<LenSum[i]<<"\n";
		

	}
	writeMT(LenSum, vertex, numOfIteration);
	///printf("Spread");
}

/*Xuat ket qua tim duoc ra man hinh*/
void CalculateLen2(int arrayStack2[1024],int countStack2, int source, int end)
{
	float lenValue = 1.0;
	//printf("%d",Stack2[1]);
	for (int i = 2; i<= countStack2; i++)
	{
		lenValue *= MTWEIGHTED[arrayStack2[i-1]][arrayStack2[i]];
	}
	//cout<<"lenvalue"<<lenValue<<"\n";
	if (LenPathAffect[source][end] == 0)
	{
		LenPathAffect[source][end] = 1 - lenValue;
		m_IsPath[source][end] = true;
	}
	else
	{
		LenPathAffect[source][end] *= ( 1- lenValue); 
		m_IsPath[source][end] = true;
	}
}

/*Kiem tra dinh i co nam trong Stack2, neu co tra ve ket qua 0 va neu khong co tra ve ket qua 1*/
char CheckVertexInStack(int i, int arrayStack2[1024],int countVertex2)
{
	for(int j=1;j<=countVertex2; j++)
	if(i==arrayStack2[j])   return 0;
	return 1;
}

/*Xoa tat ca cac phan tu giong nhau o dau Stack1 va Stack2 khi co duong di hoac gap dinh treo khong the di duoc nua*/
void DeleteArrayVertex(int arrayVertex1[1024],int &countStack1,int arrayVertex2[1024],int &countStack2) 
{
	while(arrayVertex1[countStack1]==arrayVertex2[countStack2]) 
	{
		countStack1--;
		countStack2--;
	}
	countStack2++;
	arrayVertex2[countStack2]=arrayVertex1[countStack1];
}

/*Tim kiem tat ca cac duong di neu co, neu bien Dem>0 thi ton tai duong di va nguoc lai neu Dem=0 thi khong co duong di tu source den end*/
void FindPathTwoVertex2(float A[15][15], int source, int end)
{
	int arrayVertex1[1024],arrayVertex2[1024];
	int countStack1=1,countStack2=1, hangVertex;
	int index;

	//init 2 array stack
	arrayVertex1[countStack1]=source;
	arrayVertex2[countStack2]=source;

	while( countStack1>0 && countStack2>0 )
	{
		index = arrayVertex1[countStack1];
		if ( index == end )
		{
			arrayVertex2[0] = source;
			CalculateLen2(arrayVertex2,countStack2, source, end);
			DeleteArrayVertex(arrayVertex1,countStack1,arrayVertex2,countStack2);
		}
		else
		{
			hangVertex = 1; //Gia su ton tai dinh treo

			for(int i=1; i<=n; i++)
				if(A[index][i] > 0 && CheckVertexInStack(i,arrayVertex2,countStack2)==1)
				{
					countStack1++;
					arrayVertex1[countStack1] = i;
					hangVertex = 0;
				}
      
			if(hangVertex==1)
			{
				DeleteArrayVertex(arrayVertex1,countStack1,arrayVertex2,countStack2);
			}
			else 
			{
				countStack2++;
				arrayVertex2[countStack2]=arrayVertex1[countStack1];
			}
			if(arrayVertex2[countStack2]==end)
			{			
				arrayVertex2[0] = source;
				CalculateLen2(arrayVertex2,countStack2, source, end);
				DeleteArrayVertex(arrayVertex1,countStack1,arrayVertex2,countStack2);
			}
		}
	}
}

DWORD WINAPI taske(LPVOID) 
{
	int node=0;
	while(node<n)
	{
		 DWORD dwWaitResult; 
         dwWaitResult = WaitForSingleObject(myMutex, INFINITE);
        switch (dwWaitResult) 
        {
            case WAIT_OBJECT_0: 
            { 
					node = vertex; 
					vertex++;
					ReleaseMutex(myMutex);
					if (node<=n)
					{
						for(int j=1;j<=n;j++)
							if ( node !=j )
							{
								//LenOfTwoVertexDijkstra(node, j, MTWEIGHTED);
								FindPathTwoVertex2(MTWEIGHTED, node, j);
							}
					}

				break; 
			} 
			
			  case WAIT_ABANDONED: 
					 return FALSE; 
        }
   }
	return 0;
}

void swap(int &a,int &b)
{
	int temp=a;
	a=b;
	b=temp;
}

void swapf(float &a,float &b)
{
	float temp=a;
	a=b;
	b=temp;
}

void SortMT()
{
	for(int i=1;i<n;i++)
		for(int j=i+1;j<=n;j++)
			if(b[i]<b[j])
			{
				swapf(b[i],b[j]);
				swap(a[i],a[j]);
				swap(member[i],member[j]);
				swap(affectionTopAffect[i], affectionTopAffect[j]);
				swap(stringLenPathAffect[i], stringLenPathAffect[j]);
			}
}

void writeMTAffect(int vertex)
{
	string path="Result_keyplayer_1.txt";
	ofstream write (path);
	//write<<n<<"\n";
	//write<<"Danh sach "<<top<<" phan tu co suc anh huong lon nhat trong do thi: "<<endl;
	//write<<"Dinh "<<vertex<<" anh huong toi dinh: "<<"\n";
	//write<<affectionTopAffect[vertex]<<"\n";
	write<<"Suc anh huong cua dinh "<<vertex<<" toi cac dinh khac nhu sau:\n";
	write<<stringLenPathAffect[vertex]<<"\n";
}

void handlerResultAffect()
{
	if ( m_sDoneAffect == false)
	{	
		vertex = 1;
		myMutex = CreateMutex(NULL, FALSE, NULL);           
		if (myMutex == NULL) 
		{
			printf("CreateMutex error: %d\n", GetLastError());
		}
		HANDLE thr[100]; 
		DWORD thrid; 
		for(int i=0;i<CTH;i++)
		{
			thr[i]=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)taske,0,0,&thrid);
			if( thr[i] == NULL )
			{
				printf("CreateThread error: %d\n", GetLastError());
			}
		}
		WaitForMultipleObjects(CTH, thr, TRUE, INFINITE);
		for( int i=0; i < CTH; i++ )
			CloseHandle(thr[i]);
		CloseHandle(myMutex);
	}

	for( int i=1; i<=n; i++)
	{
		for( int j=1; j<=n; j++)
		{
			//if ( LenPath[i][j] > 0 || m_IsPath[i][j] == true )
			if ( m_IsPath[i][j] == true )
			{
				//if ( m_sDoneAffect == false )
				//{
					LenPathAffect[i][j] = 1 - LenPathAffect[i][j];
				//}
				string topValue = IntToString( j );
				affectionTopAffect[i].empty();
				affectionTopAffect[i].append(topValue);
				affectionTopAffect[i].append(" ");
				stringLenPathAffect[i].append(topValue);
				stringLenPathAffect[i].append(" : ");
				stringLenPathAffect[i].append(FloatToString( LenPathAffect[i][j] ));
				stringLenPathAffect[i].append("\n");
			}				
		}
	}
}

void Ckeyplayers_Probability_GUIDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void Ckeyplayers_Probability_GUIDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void Ckeyplayers_Probability_GUIDlg::OnBnClickedBtnOpenFile()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrTitle = L"Choose the input file";
	dlg.m_ofn.lpstrInitialDir = L"E:\\DATA\\Study\\social network\\Coding\\BAO CAO\\Source Code\\data_test";
	if ( dlg.DoModal() == IDOK)
	{
		m_sFileIn = dlg.m_ofn.lpstrFile;
		UpdateData(FALSE);
	}
}


void Ckeyplayers_Probability_GUIDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if ( m_sDone == false )
	{
		UpdateData(TRUE);
		time_t rawtime;
		time(&rawtime);	
		CString tempTime = (CString)ctime(&rawtime);
		SetDlgItemText(IDC_START_TIME2,tempTime);
		SetDlgItemText(IDC_STATUS2, L"Đang xử lý...");
		int l_vertex;
		l_vertex = _tstoi(m_sVertex);
		int l_iteration;
		l_iteration = _tstoi(m_sIteration);
		if ( ProcessFiles(m_sFileIn, l_vertex, l_iteration, false, false) == TRUE)
		{
			if (m_sDoneAffect == false)
			{
				// Convert a TCHAR string to a LPCSTR
				CT2CA pszConvertedAnsiString (m_sFileIn);
				// construct a std::string using the LPCSTR input
				string strMtLike (pszConvertedAnsiString);
				//TRACE("ket qua tra ve là : [%s]\n", strStd.c_str());
				createMT(strMtLike);
			}
			if ( ProcessFiles(m_sFileIn, l_vertex, l_iteration, true, false) == TRUE)
			{
				createMTPro(l_iteration);
				handlerResult(l_vertex, l_iteration);
				//CString m_sResult = L"Danh sach " + m_sNumOfKey + L" phan tu co suc anh huong lon nhat trong do thi:\n" ;
				CString m_sResult;
				for(int i=1;i<=n;i++)
				{
					CString affectTopStr, lenPathStr;
					if ( LenPath[l_vertex][i] > 0 )
					{
						affectTopStr.Format(L"%d", i);
						lenPathStr.Format(L"%f", LenPath[l_vertex][i]);
						m_sResult += L"            " + affectTopStr + L"                                              " + lenPathStr + L"\r\n";
						SetDlgItemText(IDC_EDIT_RESULT, m_sResult);
						m_sDone = true;
						SetDlgItemText(IDC_STATUS2, L"Đã xong!");
						time(&rawtime);	
						CString tempTime = (CString)ctime(&rawtime);
						SetDlgItemText(IDC_END_TIME2,tempTime);
					}
				}
				CString m_sResultSpread, minStr, maxStr, evgStr;
				minStr.Format(L"%d", minSpread);
				maxStr.Format(L"%d", maxSpread);
				evgStr.Format(L"%f", avgSpread);
				m_sResultSpread += L" Min  :        " + minStr + L"\r\n" + L" Max :        " + maxStr + L"\r\n" + L" Average: " + evgStr + L"\r\n";
				SetDlgItemText(IDC_EDIT_SPREAD, m_sResultSpread);
				
			}
		}
	}
	else
	{
		MessageBox(L"Đã thực thi xong", L"Information", MB_OK | MB_ICONINFORMATION);
	}
}


BOOL Ckeyplayers_Probability_GUIDlg::ProcessFiles(CString sFileIn, int vertex, int sIteration, bool loadFile, bool flagAffect)
{
	if ( loadFile == false)
	{
		CFile fileIn;
		if ( fileIn.Open(sFileIn, CFile::modeRead) == FALSE )
		{
			CString sMsg;
			sMsg.Format(L"Không thể mở File %s", sFileIn);
			MessageBox(sMsg, L"Error", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		if (vertex < 1) 
		{
			CString sMsg;
			sMsg.Format(L"Vui lòng nhập đỉnh bắt đầu");
			MessageBox(sMsg, L"Error", MB_OK | MB_ICONERROR);
			return FALSE;
		}
		if (flagAffect == false)
		{
			if ( sIteration < 1 || sIteration > 100000)
			{
				CString sMsg;
				sMsg.Format(L"Vui lòng kiểm tra số lần lặp");
				MessageBox(sMsg, L"Error", MB_OK | MB_ICONERROR);
				return FALSE;
			}
		}
		int a;
		fileIn.Read(&a, sizeof(int));
	}
	else
	{
		if ( vertex < 1 || vertex > n )
		{
			CString sMsg;
			sMsg.Format(L"Vui lòng kiểm tra đỉnh bắt đầu");
			MessageBox(sMsg, L"Error", MB_OK | MB_ICONERROR);
			return FALSE;
		}		
	}
	return TRUE;
}


void Ckeyplayers_Probability_GUIDlg::OnBnClickedBtnAbout()
{
	// TODO: Add your control notification handler code here
	MessageBox(L"Mô hình lan truyền ý tưởng thành công v1.0\r\nCH1301114 - Huỳnh Thanh Việt", L"Information", MB_OK | MB_ICONINFORMATION);
}


void Ckeyplayers_Probability_GUIDlg::OnBnClickedBtnAffect()
{
	// TODO: Add your control notification handler code here
	if ( m_sDoneAffect == false )
	{
		UpdateData(TRUE);
		time_t rawtime;
		time(&rawtime);	
		CString tempTime = (CString)ctime(&rawtime);
		SetDlgItemText(IDC_START_TIME, tempTime);
		SetDlgItemText(IDC_STATUS, L"Đang xử lý...");
		//int top;
		//top = _tstoi(m_sNumOfKey);
		//float l_threshold;
		//l_threshold = _tstof(m_sThreshold);
		int l_vertex;
		l_vertex = _tstoi(m_sVertex);
		int l_iteration;
		l_iteration = _tstoi(m_sIteration);
		if ( ProcessFiles(m_sFileIn, l_vertex, l_iteration, false, true) == TRUE)
		{
			if (m_sDone == false)
			{
				// Convert a TCHAR string to a LPCSTR
				CT2CA pszConvertedAnsiString (m_sFileIn);
				// construct a std::string using the LPCSTR input
				string strMtLike (pszConvertedAnsiString);
				//TRACE("ket qua tra ve là : [%s]\n", strStd.c_str());
				createMT(strMtLike);
			}
			if ( ProcessFiles(m_sFileIn, l_vertex, l_iteration, true, true) == TRUE)
			{
				handlerResultAffect();
				int l_vertex;
				l_vertex = _tstoi(m_sVertex);
				writeMTAffect(l_vertex);
				CString m_sResult;
				for(int i=1;i<=n;i++)
				{
					CString affectTopStr, lenPathStr;
					if ( LenPathAffect[l_vertex][i] > 0 )
					{
						affectTopStr.Format(L"%d", i);
						lenPathStr.Format(L"%f", LenPathAffect[l_vertex][i]);
						m_sResult += L"            " + affectTopStr + L"                                              " + lenPathStr + L"\r\n";
						SetDlgItemText(IDC_EDIT_RESULT2, m_sResult);
						SetDlgItemText(IDC_STATUS, L"Đã xong!");
						m_sDoneAffect = true;
						time(&rawtime);	
						CString tempTime = (CString)ctime(&rawtime);
						SetDlgItemText(IDC_END_TIME,tempTime);
					}
				}
			}
		}
	}
	else
	{
		MessageBox(L"Đã tính xong", L"Information", MB_OK | MB_ICONINFORMATION);
	}
}


void Ckeyplayers_Probability_GUIDlg::OnBnClickedBtnReset()
{
	// TODO: Add your control notification handler code here
	if (n > 0)
	{
		vertex = 0;
		for( int i = 0; i<=n; i++)
		{
			a[i] = 0;
			b[i] = 0.0;
			member[i] = "";
			affectionTop[i] ="";
			stringLenPath[i] = "";
			collection[i] = "";
			stringLenPathAffect[i] = "";
			affectionTopAffect[i] = "";
			LenSum[i] = 0.0;
			markActive[i] = false;
			m_sDoneAffect = false;
			m_sDone = false;
			for ( int j = 0; j<=n; j++)
			{
				MTLIKE[i][j] = 0;
				countActive[i][j] = 0;
				countReachMore[i][j] = 0;
				MTWEIGHTED[i][j] = 0.0;
				LenPath[i][j] = 0.0;
				LenPathAffect[i][j] = 0.0;
				m_IsPath[i][j] = false;
				spreadRatio[i][j] = 0.0;
				m_IsPath[i][j] = false;
				//effect[i][j] = 0;
				for (int k = 0; k<=100001; k++)
				{
					saveActive[i][j][k] = 0;
					edgeRandom[i][j][k] = 0.0;
					ratioRandom[i][j][k] = 0.0;
					spread[k] = 0;
				}
			}
		}
		n = 0;
		minSpread = 0;
		maxSpread = 0;
		avgSpread = 0.0;
		CString temp;
		temp.Format(L"%s", "");
		UpdateData(FALSE);
		SetDlgItemText(IDC_EDIT_RESULT, temp);
		SetDlgItemText(IDC_STATUS2, temp);
		SetDlgItemText(IDC_START_TIME2,temp);
		SetDlgItemText(IDC_END_TIME2,temp);

		SetDlgItemText(IDC_EDIT_RESULT2, temp);
		SetDlgItemText(IDC_STATUS, temp);
		SetDlgItemText(IDC_START_TIME,temp);
		SetDlgItemText(IDC_END_TIME,temp);

		SetDlgItemText(IDC_EDIT_FILEIN, temp);
		SetDlgItemText(IDC_EDIT_VERTEX, temp);
		SetDlgItemText(IDC_EDIT_ITERATION, temp);
		
		m_sFileIn = "";
		m_sVertex = "";
		m_sIteration = "";
	}
}
