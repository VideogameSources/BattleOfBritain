/*
	 Battle of Britain
	 Copyright (C) 2000, 2001 Empire Interactive (Europe) Ltd,
	 677 High Road, North Finchley, London N12 0DA

	 Please see the document licence.doc for the full licence agreement

2. LICENCE
 2.1 	
 	Subject to the provisions of this Agreement we now grant to you the 
 	following rights in respect of the Source Code:
  2.1.1 
  	the non-exclusive right to Exploit  the Source Code and Executable 
  	Code on any medium; and 
  2.1.2 
  	the non-exclusive right to create and distribute Derivative Works.
 2.2 	
 	Subject to the provisions of this Agreement we now grant you the
	following rights in respect of the Object Code:
  2.2.1 
	the non-exclusive right to Exploit the Object Code on the same
	terms and conditions set out in clause 3, provided that any
	distribution is done so on the terms of this Agreement and is
	accompanied by the Source Code and Executable Code (as
	applicable).

3. GENERAL OBLIGATIONS
 3.1 
 	In consideration of the licence granted in clause 2.1 you now agree:
  3.1.1 
	that when you distribute the Source Code or Executable Code or
	any Derivative Works to Recipients you will also include the
	terms of this Agreement;
  3.1.2 
	that when you make the Source Code, Executable Code or any
	Derivative Works ("Materials") available to download, you will
	ensure that Recipients must accept the terms of this Agreement
	before being allowed to download such Materials;
  3.1.3 
	that by Exploiting the Source Code or Executable Code you may
	not impose any further restrictions on a Recipient's subsequent
	Exploitation of the Source Code or Executable Code other than
	those contained in the terms and conditions of this Agreement;
  3.1.4 
	not (and not to allow any third party) to profit or make any
	charge for the Source Code, or Executable Code, any
	Exploitation of the Source Code or Executable Code, or for any
	Derivative Works;
  3.1.5 
	not to place any restrictions on the operability of the Source 
	Code;
  3.1.6 
	to attach prominent notices to any Derivative Works stating
	that you have changed the Source Code or Executable Code and to
	include the details anddate of such change; and
  3.1.7 
  	not to Exploit the Source Code or Executable Code otherwise than
	as expressly permitted by  this Agreement.

questions about this file may be asked at bob@rowansoftware.com a
better place to ask is http://www.simhq.com/ or even :-
http://www.simhq.com/cgi-bin/boards/cgi-bin/forumdisplay.cgi?action=topics&forum=Battle+of+Britain&number=40&DaysPrune=20&LastLogin=
*/

// RowanDialog: // Visitors.cpp : implementation file
//

#include "stdafx.h"
#include "mig.h"
#include "Visitors.h"
#include "comms.h"
#include	"winmove.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#ifndef THIS_FILE_DEFINED
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisitorsBook dialog


CVisitorsBook::CVisitorsBook(CWnd* pParent /*=NULL*/)
	: RowanDialog(CVisitorsBook::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVisitorsBook)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVisitorsBook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisitorsBook)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX,IDC_RLIST_VISITORS,m_IDC_RLIST_VISITORS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVisitorsBook, CDialog)
	//{{AFX_MSG_MAP(CVisitorsBook)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisitorsBook message handlers

BOOL CVisitorsBook::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRListBox* rlistbox;
	CString string;

	rlistbox=GETDLGITEM(IDC_RLIST_VISITORS);
	rlistbox->Clear();
	rlistbox->AddColumn(120);
	rlistbox->AddColumn(60);

	string.LoadString(IDS_NAME);
	rlistbox->AddString(string,0);

	string.LoadString(IDS_STATUS);
	rlistbox->AddString(string,1);


	LPVISITORINFO	temp=_DPlay.VisitorsBook;

	while (temp)
	{
		rlistbox->AddString(temp->vis_name,0);
		if (temp->vis_status)
			rlistbox->AddString("Accepted",1);
		else
			rlistbox->AddString("Excluded",1);

		temp=temp->next;
	}
	
	vis_timer=SetTimer(DPlay::TIMER_VISITORS,0,NULL); // set sheet up first before starting timer

//DeadCode AMM 16Nov99 	_DPlay.SendInVisitorsBookMessage();
//DeadCode AMM 16Nov99 	DPlay::H2H_Player[_DPlay.mySlot].status=CPS_VISITORBOOK;
	_DPlay.SendStatusChange(DPlay::CPS_VISITORBOOK);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CVisitorsBook, CDialog)
    //{{AFX_EVENTSINK_MAP(CVisitorsBook)
	ON_EVENT(CVisitorsBook, IDC_RLIST_VISITORS, 1 /* Select */, OnSelectRlistVisitors, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CVisitorsBook::OnSelectRlistVisitors(long row, long column) 
{
	LPVISITORINFO	temp=_DPlay.VisitorsBook;
	CRListBox* rlistbox;
	CString string;

	if (column)
	{
		while (--row!=0 && temp)
		{
			temp=temp->next;
		}

		if (temp)
		{
			_DPlay.ToggleVisitorStatus(temp->vis_name);
	
// refresh 

			rlistbox=GETDLGITEM(IDC_RLIST_VISITORS);
			rlistbox->Clear();
			rlistbox->AddColumn(120);
			rlistbox->AddColumn(60);

			string.LoadString(IDS_NAME);
			rlistbox->AddString(string,0);

			string.LoadString(IDS_STATUS);
			rlistbox->AddString(string,1);

			temp=_DPlay.VisitorsBook;

			while (temp)
			{
				rlistbox->AddString(temp->vis_name,0);
				if (temp->vis_status)
					rlistbox->AddString("Accepted",1);
				else
					rlistbox->AddString("Excluded",1);

				temp=temp->next;
			}
		}
	}
}

void CVisitorsBook::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent==vis_timer && _DPlay.PlayerCreated)
	{
		_DPlay.UIUpdateMainSheet();
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CVisitorsBook::OnDestroy() 
{
	CDialog::OnDestroy();
	
	KillTimer(vis_timer);
}
