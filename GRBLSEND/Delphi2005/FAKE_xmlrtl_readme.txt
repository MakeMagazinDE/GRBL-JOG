Delphi 2005 PE Edition, available freely from:

  - French : http://delphi.developpez.com/delphi2005/


For Delphi 2005 PE, if you want to compile and install 
the design-time support for GLScene, you will need to:
- rename FAKE_xmlrtl.dcp to xmlrtl.dcp
- copy/move it to your BDS\3.0\lib directory
This will take care of the warning (doesn't add xmlrtl 
functionality however, the dcp is empty).

Source:
http://andy.jgknet.de/oss/kylix/wiki/index.php/Delphi_2005_Personal_Edition_xmlrtl.dcp_fake