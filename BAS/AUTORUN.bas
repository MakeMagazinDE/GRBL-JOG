' AUTORUN uses file selection utility by Carsten Meyer, cm@ct.de 8/2013
option usb off
FileSelect("*.bas", 0)	' open a .bas file

Mode 3:	Cls: 	Font #1: Color white, black
touch remove all
if filename$="Cancel" then
	run
endif
run filename$

Sub FileInfo
  if filename$ <> "" then
    Open filename$ For Input As #9
    fileLen = LOF(#9)
    close #9
    Font #2
    Colour Green, Black
    Print @(0,250) "Len:" fileLen " Bytes" TAB(20)
  else
    Print @(0,250) TAB(20)    
  endif
End Sub

Sub FileSelect(FileMask$, saveBtn)
' file selection utility by Carsten Meyer, cm@ct.de 8/2013
' returns filename$ (is "NewFile" if NEW selected or "Cancel")
' Note: removes all active Touch Items
  Local i, x, x_old, y, y_old, firststart
  Dim file_arr$(20)  'must be defined in main prgm
  SetTick 0, 0   ' disable Interrupt
  file_arr$(0)= Dir$(FileMask$,FILE)     'change file extension to your needs
  For i = 1 To 20
    file_arr$(i)=Dir$()
  Next i
  Cls 0:Font #2:Color White, Black:Locate 0,0
  For i = 10 To 19
    Print Tab(20) Format$(i, "%02.0f") ": " file_arr$(i)
  Next i
  Locate 0,0
  For i = 0 To 9
    Print Format$(i, "%02.0f") ": " file_arr$(i)
  Next i
  Touch release
  Touch disable all
  Touch size 462,185     'set dummy button size
  Touch create 23,5,5,"",0,n,d   ' None (dummy button), disable automatic touch handling
  Touch size 100,35      'set new bigger button size
  If saveBtn Then
    Touch create 22,150,220,"NEW",Red,B
    Touch create 21,260,220,"SAVE",Green,B
    Touch create 20,370,220,"CANCEL",Cyan,B
  Else
    Touch create 21,260,220,"OPEN",Green,B
    Touch create 20,370,220,"CANCEL",Cyan,B
  EndIf
  Line(0,0)-(239,19),-1,bf
  Font #1:Color Green, Black
  Print @(0,235) "touch file to select..."
  filenum = 0 : x_old = 0 : y_old = 0 : firststart = 1
  filename$=file_arr$(filenum)
  FileInfo
  
  Do ' handle all buttons/switch events
    'Font #2
    'Colour Green, Black
    'Print @(0,220) Time$
    If Touched(23) Then ' dummy button
      x = Touched(#X) \ 240  ' recent coordinates, integer division
      y = Touched(#Y) \ 20
      If y < 10 Then
        x = x * 10
        If x <> x_old Or y <> y_old Or firststart Then
          ' select file by touch, invert rect
          Line(x_old*24,y_old*20)-(x_old*24+239,y_old*20+19),-1,bf
          Font #2
          Line(x*24,y*20)-(x*24+239,y*20+19),-1,bf
          If  x+y <> filenum Then
            Touch Beep (1)
          EndIf
          filenum = x+y
          firststart = 0
          filename$=file_arr$(filenum)
          FileInfo
       EndIf
      EndIf
      x_old = x : y_old = y
    EndIf
    If TouchVal(20) Then
      filename$="Cancel"
      Erase file_arr$
      Touch disable all
      Cls Black
      Exit Sub
      EndIf
      If TouchVal(21) Then ' return a filename
      filename$=file_arr$(filenum)
      If filename$<>"" Then
        Erase file_arr$
        Touch remove 20,21,22,23
        Cls Black
        Exit Sub
      EndIf
    EndIf
    If TouchVal(22) Then
      ' New, return filename "NewFile"
      filename$="NewFile"
      Erase file_arr$
      Touch remove 20,21,22,23
      Cls Black
      Exit Sub
    EndIf
    Pause 50
  Loop
End Sub

Function yes_no_box(box_header$, box_line1$, box_line2$)
  Touch disable all
  Font #2
  Line(100,70)-(380,210),White,bf
  Line(100,70)-(380,210),Blue,b
  Line(102,72)-(378,208),Blue,b
  Line(103,73)-(377,207),Blue,b
  Print @(115,80) CLR$(Blue,White) box_header$
  Print @(115,103) CLR$(Black,White) box_line1$
  Print @(115,123) CLR$(Black,White) box_line2$
  Touch size 100,35      'set new bigger button size
  Touch create 21,128,160,"YES",Green,B
  Touch create 20,248,160,"NO",Red,B
  Do
    If TouchVal(21) Then
      Line(100,70)-(380,210),Black,bf
      yes_no_box = 1
      Touch remove 20,21
      Exit Function
    EndIf
    If TouchVal(20) Then
      Line(100,70)-(380,210),Black,bf
      yes_no_box = 0
      Touch remove 20,21
      Exit Function
    EndIf
    Pause 50
  Loop
End Function

Sub alert_box(box_header$, box_line1$, box_line2$)
  Touch disable all
  Font #2
  Line(100,70)-(380,210),White,bf
  Line(100,70)-(380,210),Red,b
  Line(102,72)-(378,208),Red,b
  Line(103,73)-(377,207),Red,b
  Print @(115,80) CLR$(Red,White) box_header$
  Print @(115,103) CLR$(Black,White) box_line1$
  Print @(115,123) CLR$(Black,White) box_line2$
  Touch size 100,35      'set new bigger button size
  Touch create 20,190,160,"OK",Red,B
  Do
    If TouchVal(20) Then
      Line(100,70)-(380,210),Black,bf
      Touch remove 20
      Exit Sub
    EndIf
    Pause 50
  Loop
End Sub

