#pragma once

class COutputWizard;

// OutputWizardPage

class OutputWizardPage : public CPropertyPage
{
	DECLARE_DYNAMIC(OutputWizardPage)

private:
	COutputWizard* w_;

public:
	COutputWizard* GetWizard() const { return w_; }

public:
	OutputWizardPage(UINT id = -1, COutputWizard* w = 0);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual LRESULT OnWizardBack();
	virtual LRESULT OnWizardNext();
};


