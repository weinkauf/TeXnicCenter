// OutputWizardPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "OutputWizard.h"
#include "OutputWizardPage.h"


// OutputWizardPage

IMPLEMENT_DYNAMIC(OutputWizardPage, CPropertyPage)

OutputWizardPage::OutputWizardPage(UINT id, COutputWizard* w)
		: w_(w), CPropertyPage(id)
{
	ASSERT(w);
}

BEGIN_MESSAGE_MAP(OutputWizardPage, CPropertyPage)
END_MESSAGE_MAP()



// OutputWizardPage message handlers


LRESULT OutputWizardPage::OnWizardBack()
{
	GetWizard()->OnBack();
	CPropertyPage::OnWizardBack();
	return -1;
}

LRESULT OutputWizardPage::OnWizardNext()
{
	GetWizard()->OnNext();
	return CPropertyPage::OnWizardNext();
}
