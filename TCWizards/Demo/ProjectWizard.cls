VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "ProjectWizard"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Implements IProjectWizard

Property Get IProjectWizard_Title() As String
    IProjectWizard_Title = "Demo-Project-Wizard"
End Property

Property Get IProjectWizard_Description() As String
    IProjectWizard_Description = "Demonstrates, how to implement a project wizard with VisualBasic"
End Property

Property Get IProjectWizard_IconIndex() As Long
    IProjectWizard_IconIndex = -1
End Property

Public Function IProjectWizard_DoModal(ByVal project As IProject, ByVal cr As String) As Long
    Dim msg As String
    msg = "The new project ist named " + project.Title + vbLf + vbLf + "Choose 'Cancel' to prevent the object from being created."
    If MsgBox(msg, vbOKCancel + vbInformation, "DemoWizard for TeXnicCenter") = vbOK Then
        IProjectWizard_DoModal = 1
    Else
        IProjectWizard_DoModal = 0
    End If
End Function

