VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "Form1"
   ClientHeight    =   3195
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   LinkTopic       =   "Form1"
   ScaleHeight     =   3195
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows ‚ÌŠù’è’l
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Form_Load()
    Dim Pk4 As New D3Treefs
    Dim Listfs
    Dim I%
    Debug.Print Pk4.Addfs("C:\HDA5\Doom 3\base")
    Listfs = Pk4.GetListfs()
'    For Each F In Listfs
'        If Not Pk4.SelectFile(F) Then Stop
'        Debug.Print I, Pk4.File, Pk4.FileSize, Pk4.FileTime
'    Next
    For I = LBound(Listfs) To UBound(Listfs)
        If Not Pk4.SelectFile(Listfs(I)) Then Stop
        Debug.Print I, Pk4.File, Pk4.FileSize, Pk4.FileTime
    Next
    End
End Sub
