' GRBL-DISPLAY

Option usb off
Mode 3


Font #2
Open "COM1:19200" As #1

main_page

Do
  If TouchVal(6) Then ' Run GCode
    fileselect("*.tap",0)
    gcode_line=1
    Colour White, Black
    If filename$<>"Cancel" Then
      'run_gcode
      open_gcode
    EndIf
    main_page
  EndIf

  If TouchVal(7) Then ' HOME Button
    gcode$ = "$h" ' send command
    send_gcode
  EndIf
    
  If TouchVal(8) Then ' Go Zero Button
    gcode$ = "g53 g0 z1.0" ' lift to upper position
    send_gcode
    gcode$ = "g0 x0.0 y0.0" ' send G-Code block
    send_gcode
    gcode$ = "g0 z0.0" '
    send_gcode
  EndIf

  If TouchVal(9) Then ' EXIT Button
    Close #1
    Touch Remove All
    Cls Black:Font #2:Colour Yellow, Black
    Print: Print "Good Bye"
    Run "AUTORUN.BAS"
    End
  EndIf
  get_coord
  Pause 20
Loop

'###############################################################################

Sub print_status
  Font #2 : Colour White, Black
  Print @(0, 0) "Status: " CLR$(Yellow, 0) status$ Tab(25)
End Sub

Sub print_error
  Font #2 : Colour Red, Black
  Print @(0, 0) "Error on line: " gcode_line Tab(39)
  Font #1
  Print CLR$(White, 0) status$ CLR$(Red, 0) Tab(79)
End Sub

'###############################################################################

Sub send_gcode
  'returns err=1 if response was not "ok"
  dummy$ = Input$(200, #1)
  Print #1, gcode$
  Line Input #1,status$
  If status$ = "ok" Then
    err = 0
  Else
    err = 1
  EndIf
End Sub

Sub run_gcode_window
  Touch Remove All       'erase buttons
  Touch Size 150,35      'set new bigger button size
  Touch Create 9,0,220,"STOP",Red,B
End Sub

Sub display_gcode
  x = 0 : y = 0 : z = 0
  xold = 0 : yold = 0 : feed = 0
  xmax = 0 : ymax = 0 : zmax = 0
  gcode_line = 1
  Open filename$ For input As #2
  Do
    Line Input #2, gcode$
    posz=Instr(1,gcode$,"Z")
    posx=Instr(1,gcode$,"X")
    posy=Instr(1,gcode$,"Y")
    If posx Then
      myval$ = Mid$(gcode$,posx+1)
      x = 1 + (Val(myval$) * scale)
      If x > xmax Then
        xmax = x
      EndIf
    EndIf
    If posy Then
      myval$ = Mid$(gcode$,posy+1)
      y = 247 - (Val(myval$) * scale)
      If y < ymax Then
        ymax = y
      EndIf
    EndIf
    If posz Then
      myval$ = Mid$(gcode$,posz+1)
      z = Val(myval$)
      If z < zmax Then
        zmax = z
      EndIf      
    EndIf
    If posx Or posy Then
      If z < 0 Then
        Pixel(x, 20+(-10*z)) = green ' Z-Tiefe
        Line (xold, yold)-(x, y), white
        'Pixel(xold, yold)) = red
        'Pixel(x, y)) = red
      Else
        Pixel(x,y) = red
      EndIf
    EndIf
    xold = x
    yold = y
    Print @(3, 3) "Line: " gcode_line "   "
    gcode_line = gcode_line + 1
  Loop Until Eof(#2)
  xmax = 247-xmas
  ymax = ymax-1
  Print @(3, 3) "Line: " gcode_line  "  X max: " xmax "  Y max: " ymax "  Z max: " zmax
  close #2
End Sub

Sub open_gcode
  Cls Black
  Touch Remove All      'erase buttons
  Touch Size 75,20      'set new button size 
  Touch Create 10,0,251,"100",white,R
  Touch Create 11,80,251,"200",white,R
  Touch Create 12,160,251,"400",white,R
  Touch Create 13,240,251,"800",white,R
  Font #2 
  Print @(320, 253) "% Scale"
  TouchVal(11)=1 ' scale 2 radio button
  scale = 2
  Font #1 
  do
    Line (1, 1)-(478, 247),Black,BF
    Line (0, 0)-(479, 248),White,B
    'Line (0, 15)-(479, 15),White
    Line (1, 20)-(478, 20),white
    For i = 47 to 247 step 20
      Line (1, i)-(478, i),blue
    Next i
    For i = 1 to 471 step 20
      Line (i, 21)-(i, 247),blue
    Next i
    Print @(364, 3) clr$(yellow, black) "Z: 1 mm/div"
    Print @(364, 233) clr$(blue, black) "XY: " 20/scale "mm/div "
    Line (1, 30)-(478, 30),yellow
    Line (1, 40)-(478, 40),yellow
    
    end_loop = 0
    display_gcode
    Touch Size 100,35      'set new button size 
    Touch Create 14,370,80,"Run",green,B
    Touch Create 15,370,130,"Exit",Cyan,B
    Touch Create 16,370,180,"Stop",red,B
    Touch Disable 16
    do
      if Touched(10) then
        end_loop = 1
        scale = 1
      endif
      if Touched(11) then
        end_loop = 1
        scale = 2
      endif
      if Touched(12) then
        end_loop = 1
        scale = 4
      endif
      if Touched(13) then
        end_loop = 1
        scale = 8
      endif
      if TouchVal(14) then
        Touch Disable All
        Touch Enable 16
        Open filename$ For input As #2
        Do
          Line Input #2, gcode$
          send_gcode
          gcode_line=gcode_line+1
          If err=1 Then
            end_loop = 1
            Touch Enable 16
          EndIf
        Loop Until Eof(#2) Or err>1 or TouchVal(16)
        Touch Enable All
        Touch Disable 16
        Close #2
        gcode$ = "g53 g0 z1.0" ' lift to upper position        
        send_gcode
        end_loop = 1
      endif
      if TouchVal(15) then
        Cls Black
        Touch Remove All      'erase buttons
        exit sub
      endif
      pause 50
    loop until end_loop
  loop
end sub

Sub main_page
  Cls Black : Colour White, Black
  Touch Remove All
  Touch Size 100,35      'set new bigger button size
  Touch Create 7,170,220,"Homing",Red,B
  Touch Create 8,280,220,"GoZero",Cyan,B
  Touch Size 80,35       'set new bigger button size
  Touch Create 9,390,220,"Exit",Green,B
  
  Touch Size 155,35      'set new bigger button size
  Touch Create 6,0,220,"Open GCode",Purple,B
  Colour White, Black
  get_coord
End Sub

Sub get_coord
  Print #1, "?"; ' send the query out of the serial port, no cr
  Input #1, status$, dummy$, m_x, m_y, m_z, dummy$, w_x, w_y, w_z
  print_status
  If status$  <> "Jog" Then
    Colour Blue, White
    Print @(0, 40) "Machine Position  ";
    Colour Red, White
    Print "Work Position     "
    Font #2,2
    Print @(0, 70) CLR$(Cyan, 0) Format$(m_x,"%07.2f")     "  " CLR$(Yellow, 0) Format$(w_x,"%07.2f")     CLR$(7, 0) " X"
    Print CLR$(Blue, 0) Format$(m_y,"%07.2f")      "  " CLR$(Red, 0) Format$(w_y,"%07.2f")CLR$(7, 0) " Y"
    Print CLR$(Purple, 0) Format$(m_z,"%07.2f")    "  " CLR$(Green, 0) Format$(w_z,"%07.2f")      CLR$(7, 0) " Z"
    dummy$ = Input$(200, #1)
    Font #2
  EndIf
End Sub


' #########################################################################################

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

