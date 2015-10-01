' GRBL-DISPLAY

Option usb off
Mode 3


Font #2
Open "COM1:19200" As #1

main_page_create
ll_x = 0 : ll_y = 0 : ur_x = 0 : ur_y = 0	' Utils coord
cc_x = 0 : cc_y = 0 : ce_x = 0 : ce_y = 0

Do
  If TouchVal(0) Then ' Zero X
    Touch Beep 50
    gcode$ = "G92 X0.0"
    send_gcode
  EndIf
  
  If TouchVal(1) Then ' Zero Y
    Touch Beep 50
    gcode$ = "G92 Y0.0"
    send_gcode
  EndIf
  
  If TouchVal(2) Then ' Zero Z
    Touch Beep 50
    gcode$ = "G92 Z10.0"
    send_gcode
  EndIf

	get_coord
  print_status
  print_coord

  If TouchVal(7) Then ' HOME Button
    gcode$ = "$h" ' send command
    send_gcode
  EndIf
    
  If TouchVal(8) Then ' Go Zero Button
    gcode$ = "G53 G0 Z-2.0" ' lift to upper absolute position
    send_gcode
    gcode$ = "G0 X0.0 Y0.0" ' send G-Code block
    send_gcode
    gcode$ = "G0 Z10.0"
    send_gcode
  EndIf

  If TouchVal(9) Then ' UTILS Button
	  utils_page_create
    utils_page_loop
    main_page_create
  EndIf
  
  If TouchVal(6) Then ' Exit
    Close #1
    Touch Remove All
    Cls Black:Font #2:Colour Yellow, Black
    Print: Print "Good Bye"
    Run "AUTORUN.BAS"
    End
  EndIf
Loop

'###############################################################################

Sub print_status
  Font #2
  Print @(0, 0) "Status: " CLR$(Yellow, 0) status$ Tab(25)
End Sub

Sub print_error
  Font #2 : Colour Red, Black
  Print @(0, 0) "Error on line: " gcode_line Tab(39)
  Font #1
  Print CLR$(White, 0) status$ CLR$(Red, 0) Tab(79)
  Colour White, Black
End Sub

'###############################################################################

Sub send_gcode
  'returns err=1 if response was not "ok"
  dummy$ = Input$(100, #1)
  Print #1, gcode$
  Line Input #1,status$
  If status$ = "ok" Then
    err = 0
  Else
    err = 1
  EndIf
  Pause 10
  dummy$ = Input$(100, #1)
End Sub

'###############################################################################

Sub utils_page_loop
  LastFill = -1
  ll_x = -1 : ll_y = -1
  ur_x = -1 : ur_y = -1
  cc_x = -1 : cc_y = -1
  ce_x = -1 : ce_y = -1
  Do
    get_coord
    print_shortcoord
  	if LastFill <> TouchVal(4) Then
 		  if TouchVal(4) Then
        Line (20,50)-(200,150),Cyan,BF
        Circle (300,100),50,Purple,1,F
      Else
        Line (20,50)-(200,150),Black,BF 'erase fill
        Circle (300,100),50,Black,1,F
        Line (20,50)-(200,150),Cyan,B
        Line (21,51)-(199,149),Cyan,B
        Circle (300,100),50,Purple
        Circle (300,100),49,Purple
      EndIf
      Touch Size 20,20       'set new button size
      Touch Create 11,10,140,"",Red,B 'lower left
      Touch Create 12,190,40,"",Red,B 'upper right
      Touch Create 13,290,90,"",Red,B 'circle center
      Touch Create 14,340,90,"",Red,B 'circle edge
      LastFill = TouchVal(4)
    EndIf
   	feed = TouchVal(19) * 10 + 100
    Font #2
   	Print @(415, 90) feed "   "
    Font #1
   	
   	
    If TouchVal(11) Then 'lower left
    	ll_x = wx
    	ll_y = wy
    	dwn_z = wz
    	Print @(32, 154) ll_x; ","; ll_y
    EndIf
    If TouchVal(12) Then 'upper right
    	ur_x = wx
    	ur_y = wy
    	dwn_z = wz
    	Print @(214, 35) ur_x; ","; ur_y
    EndIf
    If TouchVal(13) Then 'circle center
    	cc_x = wx
    	cc_y = wy
    	dwn_z = wz
    	Print @(340, 154) "Center "; cc_x; ","; cc_y
    EndIf
    If TouchVal(14) Then 'circle edge
    	ce_x = wx
    	ce_y = wy
    	dwn_z = wz
    	Print @(362, 95) ce_x; ","; ce_y
    EndIf
    
    If TouchVal(2) Then 'Run Rect 
    	If (ur_x > ll_x) or (ur_y > ll_y) then 'coord OK
      	t_ll_x = ll_x : t_ll_y = ll_y   	
      	t_ur_x = ur_x : t_ur_y = ur_y   	
        if TouchVal(4) then
          Touch Beep 200 
        	do while (t_ll_x <= t_ur_x) and (t_ll_y <= t_ur_y)
            send_rect(t_ll_x, t_ll_y, t_ur_x, t_ur_y)
        		t_ll_x = t_ll_x + 1.5
        		t_ll_y = t_ll_y + 1.5
        		t_ur_x = t_ur_x - 1.5
        		t_ur_y = t_ur_y - 1.5
       	  loop
       	else 
          Touch Beep 50 
          send_rect(t_ll_x, t_ll_y, t_ur_x, t_ur_y)
        endif
      Else
      	Touch Beep 500
      EndIf
    EndIf
    If TouchVal(3) Then 'Run Rect 
    	Touch Beep 500 ' not implemented yet
    EndIf
  Loop Until TouchVal(1)
End Sub


Sub send_rect(my_ll_x, my_ll_y, my_ur_x, my_ur_y)
  gcode$ = "M3"
  send_gcode
  gcode$ = "G1 X" + FORMAT$( my_ur_x , "%07.2f") + " Y" + FORMAT$( my_ur_y , "%07.2f") + " F" +  FORMAT$(feed,"%g")
  send_gcode
  gcode$ = "G1 X" + FORMAT$( my_ur_x , "%07.2f") + " Y" + FORMAT$( my_ll_y , "%07.2f")
  send_gcode
  gcode$ = "G1 X" + FORMAT$( my_ll_x , "%07.2f") + " Y" + FORMAT$( my_ll_y , "%07.2f")
  send_gcode
  gcode$ = "G1 X" + FORMAT$( my_ll_x , "%07.2f") + " Y" + FORMAT$( my_ur_y , "%07.2f")
  send_gcode
  gcode$ = "G1 X" + FORMAT$( my_ur_x , "%07.2f") + " Y" + FORMAT$( my_ur_y , "%07.2f") 
  send_gcode	
End sub


Sub utils_page_create
  Cls Black : Colour White, Black
  Touch Remove All
  Touch Size 80,35       'set new bigger button size
  Touch Create 1,390,220,"Main",Blue,B
  Touch Size 130,35
  Touch Create 2,110,220,"RunRect",Cyan,B
  Touch Create 3,250,220,"RunCirc",Purple,B
  Touch Size 25,25
  Touch Create 4,8,225,"Fill",White,C 'Fill checkbox
  Touch Size 20,100       'set new slider size
  Touch Create 19, 390, 40, "feed", Blue,V,B 
  TouchVal(19) = 40
  feed = 500
  Font #2
  Print @(415, 40) "Feed"
  Print @(415, 90) feed
  Font #1
  Colour Red, Black
  Print @(0, 185) "Click on RED Buttons when machine is at this position."
  Print @(0, 193) "Set Z manually on upper left"
  Colour White, Black
End Sub


Sub main_page_create
  Cls Black : Colour White, Black
  Touch Remove All
  Touch Size 100,35      'set new bigger button size
  Touch Create 7,170,220,"Homing",Red,B
  Touch Create 8,280,220,"GoZero",Cyan,B
  Touch Size 80,35       'set new bigger button size
  Touch Create 6,10,220,"Exit",Green,B
  Touch Create 9,390,220,"Utils",Blue,B
  TouchVal(6) = 0
  Touch Size 185,35      'set new bigger button size
  Touch Create 0,230, 70,"Zero X",Black,B ' Zero X
  Touch Create 1,230,110,"Zero Y",Black,B ' Zero Y
  Touch Create 2,230,150,"Zero Z",Black,B ' Zero Z

  'Touch Size 80,35      'set new bigger button size
  'Touch Create 10,320,2,"Test",Blue,B
End Sub

Sub print_coord
  Colour Blue, White
  Font #2
  Print @(0, 40) "Machine Position  ";
  Colour Red, White
  Print "Work Position     "
  Font #2,2
  Print @(0, 70) CLR$(Cyan, 0) Format$(mx,"%07.2f")     "  " CLR$(Yellow, 0) Format$(wx,"%07.2f")     CLR$(7, 0) " X"
  Print CLR$(Blue, 0) Format$(my,"%07.2f")      "  " CLR$(Red, 0) Format$(wy,"%07.2f")CLR$(7, 0) " Y"
  Print CLR$(Purple, 0) Format$(mz,"%07.2f")    "  " CLR$(Green, 0) Format$(wz,"%07.2f")      CLR$(7, 0) " Z"
  Font #2
  Colour White, Black
End Sub

Sub print_shortcoord
  Font #2
  Print @(0, 0)   CLR$(7, 0) "X:" CLR$(Yellow, 0) Format$(wx,"%07.2f")     
  Print @(150, 0) CLR$(7, 0) "Y:" CLR$(Red, 0) Format$(wy,"%07.2f")
  Print @(300, 0) CLR$(7, 0) "Z:" CLR$(Green, 0) Format$(wz,"%07.2f")      
  Colour White, Black
End Sub

Sub get_coord
  local lwx, lwy, lwz, lmx, lmy, lmz
  dummy$ = Input$(200, #1)
  Print #1, "?"; ' send the query out of the serial port, no cr
  Pause 20
  
  Input #1, status$, mpos$, lmx, lmy, lmz, wpos$, lwx, lwy, lwz
  If status$  = "JogX" Then
    wx = val(mpos$)
  EndIf
  If status$  = "JogY" Then
    wy = val(mpos$)
  EndIf
  If status$  = "JogZ" Then
    wz = val(mpos$)
  EndIf
  If wpos$ = "WPos" then
    mx = lmx
    my = lmy
    mz = lmz
    wx = lwx
    wy = lwy
    wz = lwz
  EndIf
  dummy$ = Input$(100, #1)
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
  Colour White, Black
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
  Colour White, Black
End Sub

